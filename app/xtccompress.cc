#include "pdsdata/camera/CompressedFrameV1.hh"
#include "pdsdata/camera/FrameV1.hh"
#include "pdsdata/camera/FrameCompressor.hh"

#include "pdsdata/cspad/ConfigV1.hh"
#include "pdsdata/cspad/ConfigV2.hh"
#include "pdsdata/cspad/ConfigV3.hh"
#include "pdsdata/cspad/ConfigV4.hh"
#include "pdsdata/cspad/ElementV1.hh"
#include "pdsdata/cspad/ElementV2.hh"
#include "pdsdata/cspad/ElementIterator.hh"
#include "pdsdata/cspad/CspadCompressor.hh"
#include "pdsdata/cspad/CompressorOMP.hh"

#include "pdsdata/xtc/Dgram.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <map>
using std::map;
using Pds::Camera::FrameCompressor;
using Pds::CsPad::CspadCompressor;
using Pds::CsPad::CompressorOMP;

using namespace Pds;

class XtcMapKey {
public:
  XtcMapKey(const Xtc& xtc) :
    _level(xtc.src.level()),
    _phy  (xtc.src.phy()),
    _t    (xtc.contains) {}
  XtcMapKey(const Xtc& xtc, TypeId t) :
    _level(xtc.src.level()),
    _phy  (xtc.src.phy()),
    _t    (t) {}
public:
  bool operator<(const XtcMapKey& o) const
  {
      if (_level != o._level) return _level < o._level;
      if (_phy   != o._phy  ) return _phy   < o._phy ;
      return _t.value() < o._t.value();
  }
private:
  unsigned _level;
  unsigned _phy;
  TypeId   _t;
};

class myIter {
public:
  enum Status {Stop, Continue};
  myIter(bool extract) : _extract(extract), _aligned(true), _pwrite(0) 
  {
    for(unsigned i=0; i<MaxCspadImages; i++) {
      _cspad_params[i].width  = 2*CsPad::MaxRowsPerASIC;
      _cspad_params[i].height = CsPad::ColumnsPerASIC;
      _cspad_params[i].depth  = 2;
    }
  }
  ~myIter() 
  {
    for(map<int,FrameCompressor*>::iterator it=_frame_cmp_map.begin();
        it!=_frame_cmp_map.end(); it++) {
      FrameCompressor* frameCmp = (*it).second;
      delete frameCmp;
    }
  }  
private:
  //
  //  Iterate through the Xtc and compress, decompress, copy into new Xtc
  //
  void iterate(Xtc* root) {
    if (root->damage.value() & ( 1 << Damage::IncompleteContribution)) {
      return _write(root,root->extent);
    }
    
    int remaining = root->sizeofPayload();
    Xtc* xtc     = (Xtc*)root->payload();

    uint32_t* pwrite = _pwrite;
    _write(root, sizeof(Xtc));
    
    while(remaining > 0)
      {
	unsigned extent = xtc->extent;
	if(extent==0) {
	  printf("Breaking on zero extent\n");
	  break; // try to skip corrupt event
	}
	process(xtc);
	remaining -= extent;
	xtc        = (Xtc*)((char*)xtc+extent);
      }

    reinterpret_cast<Xtc*>(pwrite)->extent = (_pwrite-pwrite)*sizeof(uint32_t);
  }
  
  void process(Xtc* xtc) {
    switch (xtc->contains.id()) {

    case (TypeId::Id_Xtc) : {
      iterate(xtc);
      return;
    }

    case (TypeId::Id_Frame) : {
      if (xtc->damage.value()) break;
      if (xtc->contains.version()!=1) break;
      if (_extract) break;

      const Camera::FrameV1* pframe = reinterpret_cast<const Camera::FrameV1*>(xtc->payload());
      if (_compress(xtc, *pframe))
        return;

      break;
    }

    case (TypeId::Id_CompressedFrame) : {
      if (xtc->damage.value()) {
        printf("CompressedFrame with damage\n");
        break;
      }
      if (xtc->contains.version()!=1) {
        printf("CompressedFrame version not 1\n");
        break;
      }
      
      if (!_extract) break;

      const Camera::CompressedFrameV1* pframe = 
        reinterpret_cast<const Camera::CompressedFrameV1*>(xtc->payload());
      if (_decompress(xtc, *pframe))
        return;

      break;
    }

    case (TypeId::Id_CspadConfig) : {
      _cache_config( xtc );
      break;
    }

    case (TypeId::Id_CspadElement) : {
      if (xtc->damage.value()) break;
      if (_extract) break;

      CsPad::ElementIterator* iter = _lookup_iterator(xtc);
      if (_compress(xtc, iter))
        return;
      
      break;
    }

    case (TypeId::Id_CspadCompressedElement) : {
      if (!_extract) break;

      CsPad::ElementIterator* iter = _lookup_iterator(xtc);
      if (_decompress(xtc, iter))
        return;
      
      break;
    }

    default :
      break;
    }
    _write(xtc,xtc->extent);
  }

private:
  //
  //  Xtc headers are 32b aligned.  
  //  Compressed data is not.
  //  Enforce alignment during Xtc construction.
  //
  void _write(const void* p, ssize_t sz) 
  {
    if (!_aligned)
      perror("Writing 32b data alignment not guaranteed\n");

    //    printf("_write %p %p %x\n",_pwrite,p,sz);

    const uint32_t* pread = (uint32_t*)p;
    if (_pwrite!=pread) {
      const uint32_t* const end = pread+(sz>>2);
      while(pread < end)
	*_pwrite++ = *pread++;
    }
    else
      _pwrite += sz>>2;
  }
  void _uwrite(const void* p, ssize_t sz) 
  {
    if (_aligned)
      perror("Writing 8b data when 32b alignment required\n");

    const uint8_t* pread = (uint8_t*)p;
    if (_upwrite!=pread) {
      const uint8_t* const end = pread+sz;
      while(pread < end)
	*_upwrite++ = *pread++;
    }
    else
      _upwrite += sz;
  }
  void _align_unlock()
  {
    _aligned = false;
    _upwrite = (uint8_t*)_pwrite;
  }
  void _align_lock()
  {
    _pwrite += (_upwrite - (uint8_t*)_pwrite +3)>>2;
    _aligned = true;
  }
  
public:
  void iterate(const Dgram* dg, uint32_t* pwrite) 
  {
    _pwrite = pwrite;
    _write(dg, sizeof(*dg)-sizeof(Xtc));
    iterate(const_cast<Xtc*>(&(dg->xtc)));
  }

private:
  //
  //  Compress Camera::FrameV1
  //
  bool _compress(const Xtc* xtc,
                 const Camera::FrameV1& frame)
  {
    FrameCompressor::ImageParams params;
    params.width  = frame.width ();
    params.height = frame.height();
    params.depth  = frame.depth ();

    unsigned phy = xtc->src.phy();
    map<int,FrameCompressor*>::const_iterator it=_frame_cmp_map.find( phy );
    if ( it==_frame_cmp_map.end() ) {
      _frame_cmp_map[ phy ] = new FrameCompressor;
    }
    FrameCompressor* frameCmp = _frame_cmp_map[ phy ];

    void*  outbuf;
    size_t outsz;

    if (frameCmp->compress(frame.data(),
                           params, 
                           outbuf, 
                           outsz) == FrameCompressor::Success) {
          
      // Copy the xtc header
      uint32_t* pwrite = _pwrite;
      { 
        Xtc nxtc(*xtc);
        nxtc.contains = TypeId(TypeId::Id_CompressedFrame,Camera::CompressedFrameV1::Version);
        _write(&nxtc, sizeof(Xtc));
      }
          
      _align_unlock();

      Camera::CompressedFrameV1 cframe(frame);
      _uwrite(&cframe, sizeof(cframe));
          
      _uwrite(outbuf, outsz);

      _align_lock();

      //  Update the extent of the container
      reinterpret_cast<Xtc*>(pwrite)->extent = (_pwrite-pwrite)*sizeof(uint32_t);
      return true;
    }
    return false;
  }

  //
  //  Decompress Camera::FrameV1
  //
  bool _decompress(const Xtc* xtc,
                   const Camera::CompressedFrameV1& frame)
  {      
    FrameCompressor::ImageParams params;
    params.width  = frame.width ();
    params.height = frame.height();
    params.depth  = frame.depth ();

    unsigned phy = xtc->src.phy();
    map<int,FrameCompressor*>::const_iterator it=_frame_cmp_map.find( phy );
    if ( it==_frame_cmp_map.end() ) {
      _frame_cmp_map[ phy ] = new FrameCompressor;
    }
    FrameCompressor* frameCmp = _frame_cmp_map[ phy ];

    void*  outbuf;
    size_t outsz = xtc->sizeofPayload() - sizeof(frame);

    if (frameCmp->decompress(frame.data(), 
                             outsz,
                             params, 
                             outbuf) == FrameCompressor::Success) {
          
      // Copy the xtc header
      uint32_t* pwrite = _pwrite;
      { 
        Xtc nxtc(*xtc);
        nxtc.contains = TypeId(TypeId::Id_Frame,Camera::FrameV1::Version);
        _write(&nxtc, sizeof(Xtc));
      }

      Camera::FrameV1 cframe(frame.width (),
                             frame.height(),
                             frame.depth (),
                             frame.offset());
      _write(&cframe, sizeof(cframe));
          
      _write(outbuf, cframe.data_size());

      //  Update the extent of the container
      reinterpret_cast<Xtc*>(pwrite)->extent = (_pwrite-pwrite)*sizeof(uint32_t);
      return true;
    }
    return false;
  }

  bool _compress(const Xtc* xtc, 
                 CsPad::ElementIterator* iter) 
  {
    if (!iter) return false;

    unsigned phy = xtc->src.phy();
    map<int,CompressorOMP<CspadCompressor>*>::const_iterator it=_cspad_cmp_map.find( phy );
    if ( it==_cspad_cmp_map.end() )
      _cspad_cmp_map[ phy ] = new CompressorOMP<CspadCompressor>(MaxCspadImages);

    CompressorOMP<CspadCompressor>* frameCmp = _cspad_cmp_map[ phy ];

    const void* inbuf [MaxCspadImages];
    unsigned iImages = 0;
    {
      CsPad::ElementIterator it(*iter);
      const CsPad::ElementHeader* hdr;
      while( (hdr=it.next()) ) {
        const CsPad::Section* s;
        unsigned id;
        while( (s=it.next(id)) ) {
          inbuf [iImages] = s;
          iImages++;
        }
      }
    }
    
    void*  outbuf[MaxCspadImages];
    size_t  outsz[MaxCspadImages];
    int     stat [MaxCspadImages];

    if (frameCmp->compress(inbuf,
                           _cspad_params,
                           outbuf,
                           outsz,
                           stat,
                           iImages) == CompressorOMP<CspadCompressor>::Success) {

      // Copy the xtc header
      uint32_t* pwrite = _pwrite;
      { 
        Xtc nxtc(*xtc);
        nxtc.contains = TypeId(TypeId::Id_CspadCompressedElement,
                               xtc->contains.version());
        _write(&nxtc, sizeof(Xtc));
      }

      _align_unlock();

      // Copy the quadrant headers and the section compressed images
      unsigned n = 0;
      CsPad::ElementIterator it(*iter);
      const CsPad::ElementHeader* hdr;
      while( (hdr=it.next()) ) {

        _uwrite(hdr, sizeof(*hdr));

        const CsPad::Section* s;
        unsigned id;
        while( (s=it.next(id)) ) {
          _uwrite(outbuf[n], outsz[n]);
          n++;
        }

        uint32_t qw = it.getQuadWord();
        _uwrite(&qw, sizeof(qw));
      }
      
      _align_lock();

      //  Update the extent of the container
      reinterpret_cast<Xtc*>(pwrite)->extent = (_pwrite-pwrite)*sizeof(uint32_t);

      delete iter;
      return true;
    }
    delete iter;
    return false;
  }

  bool _decompress(const Xtc* xtc, 
                   CsPad::ElementIterator* iter) 
  {
    if (!iter) return false;

    // Copy the xtc header
    uint32_t* pwrite = _pwrite;
    { 
      Xtc nxtc(*xtc);
      nxtc.contains = TypeId(TypeId::Id_CspadElement,
                             xtc->contains.version());
      _write(&nxtc, sizeof(Xtc));
    }

    // Copy the quadrant headers and the section images
    CsPad::ElementIterator it(*iter);
    const CsPad::ElementHeader* hdr;
    while( (hdr=it.next()) ) {
      
      _write(hdr, sizeof(*hdr));
      
      const CsPad::Section* s;
      unsigned id;
      while( (s=it.next(id)) ) {
        _write(s, sizeof(*s));
      }

      uint32_t qw = it.getQuadWord();
      _write(&qw, sizeof(qw));
    }

    //  Update the extent of the container
    reinterpret_cast<Xtc*>(pwrite)->extent = (_pwrite-pwrite)*sizeof(uint32_t);

    delete iter;
    return true;
  }

  CsPad::ElementIterator* _lookup_iterator(const Xtc* xtc)
  {
#define CSPAD_VER(v) {                                                  \
      XtcMapKey key(*xtc, TypeId(TypeId::Id_CspadConfig, v));           \
      if (_xtcmap.find(key)!=_xtcmap.end())                             \
        return new CsPad::ElementIterator( *reinterpret_cast<const CsPad::ConfigV##v*>(_xtcmap[ key ]->payload()), *xtc ); \
    }

    CSPAD_VER(1);
    CSPAD_VER(2);
    CSPAD_VER(3);
    CSPAD_VER(4);
    return 0;

#undef CSPAD_VER
  }

private:
  //
  //  Cache a copy of this xtc
  //
  void _cache_config(const Xtc* xtc) {
    char* p = new char[xtc->extent];
    memcpy(p, xtc, xtc->extent);
    XtcMapKey key(*xtc);
    if (_xtcmap.find(key)!=_xtcmap.end())
      delete[] reinterpret_cast<const char*>(_xtcmap[key]);
    _xtcmap[ XtcMapKey(*xtc) ] = reinterpret_cast<const Xtc*>(p); 
  }
private:
  bool                                      _extract;
  bool                                      _aligned;
  uint32_t*                                 _pwrite;
  uint8_t*                                  _upwrite;
  map<int, FrameCompressor*>                _frame_cmp_map;
  enum { MaxCspadImages=32 };
  CspadCompressor::ImageParams              _cspad_params[MaxCspadImages];
  map<int, CompressorOMP<CspadCompressor>*> _cspad_cmp_map;
  map<XtcMapKey, const Xtc*>                _xtcmap;
};

void usage(char* progname) {
  fprintf(stderr,"Usage: %s -i <input filename> -o <output filename> [-x] [-h]\n  -x : extract (decompress)\n", progname);
}

int main(int argc, char* argv[]) {
  int c;
  char* inxtcname=0;
  char* outxtcname=0;
  bool extract=false;
  int parseErr = 0;
  unsigned nevents = -1;

  while ((c = getopt(argc, argv, "hxn:i:o:")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'x':
      extract = true;
      break;
    case 'n':
      nevents = atoi(optarg);
      break;
    case 'i':
      inxtcname = optarg;
      break;
    case 'o':
      outxtcname = optarg;
      break;
    default:
      parseErr++;
    }
  }
  
  if (!inxtcname || !outxtcname) {
    usage(argv[0]);
    exit(2);
  }

  int ifd = open(inxtcname, O_RDONLY | O_LARGEFILE);
  if (ifd < 0) {
    perror("Unable to open input file\n");
    exit(2);
  }

  FILE* ofd = fopen(outxtcname,"wx");
  if (ofd == 0) {
    perror("Unable to open output file\n");
    exit(2);
  }
  
  const unsigned MAX_DG_SIZE = 0x2000000;
  XtcFileIterator iter(ifd,MAX_DG_SIZE);
  Dgram* dg;

  uint32_t* obuff = new uint32_t[MAX_DG_SIZE>>2];

  myIter cmpiter(extract);

  while ((dg = iter.next())) {
    //    if (!dg->seq.isEvent())
    cmpiter.iterate(dg, obuff);
    
    const Dgram* odg = reinterpret_cast<const Dgram*>(obuff);
    fwrite(odg, sizeof(*odg) + odg->xtc.sizeofPayload(), 1, ofd);
    fflush(ofd);

    printf("%s transition: time 0x%x/0x%x, payloadSize %d (%d)\n",TransitionId::name(dg->seq.service()),
           dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(), dg->xtc.sizeofPayload(), odg->xtc.sizeofPayload());

    if (dg->seq.isEvent())
      if (--nevents == 0)
        break;
  }
  
  close (ifd);
  fclose(ofd);
  return 0;
}
