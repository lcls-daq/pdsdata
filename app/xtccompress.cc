//
//  Unofficial example of XTC compression
//
#include "pdsdata/camera/FrameV1.hh"

#include "pdsdata/cspad/ConfigV1.hh"
#include "pdsdata/cspad/ConfigV2.hh"
#include "pdsdata/cspad/ConfigV3.hh"
#include "pdsdata/cspad/ConfigV4.hh"
#include "pdsdata/cspad/ElementV1.hh"
#include "pdsdata/cspad/ElementV2.hh"
#include "pdsdata/cspad/ElementIterator.hh"
#include "pdsdata/cspad2x2/ElementV1.hh"
#include "pdsdata/compress/Hist16Engine.hh"
#include "pdsdata/compress/HistNEngine.hh"
#include "pdsdata/compress/CompressedXtc.hh"

#include "pdsdata/pnCCD/ConfigV1.hh"
#include "pdsdata/pnCCD/ConfigV2.hh"
#include "pdsdata/pnCCD/FrameV1.hh"

#include "pdsdata/timepix/DataV1.hh"
#include "pdsdata/timepix/DataV2.hh"

#include "pdsdata/xtc/Dgram.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"

#include <boost/shared_ptr.hpp>

#include <string.h>

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <map>
using std::map;

static Pds::CompressedPayload::Engine _engine = Pds::CompressedPayload::None;

namespace Pds {
  //
  //  A key for std::map lookup of an Xtc
  //
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
};

using namespace Pds;

//
//  An Xtc iterator to compress or uncompress camera images.
//  The resulting Xtc structure maintains 32-bit alignment.
//  If the compression algorithms themselves maintain 32-bit alignment,
//    then these steps aren't necessary.
//
class myIter {
public:
  enum Status {Stop, Continue};
  myIter(bool extract) : _extract(extract), _aligned(true), _pwrite(0),
                         _outbuf(new char[0x1000000]),
                         _outsz (0x1000000)
  {
  }
  ~myIter() 
  {
    delete[] _outbuf;
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
          abort();
	  break; // try to skip corrupt event
	}
	process(xtc);
	remaining -= extent;
	xtc        = (Xtc*)((char*)xtc+extent);
      }

    reinterpret_cast<Xtc*>(pwrite)->extent = (_pwrite-pwrite)*sizeof(uint32_t);
  }
  
  void process(Xtc* xtc) {

    //
    //  We're only interested in compressing/decompressing
    //
    switch (xtc->contains.id()) {
    case (TypeId::Id_Xtc):
      iterate(xtc);
      return;
    case (TypeId::Id_pnCCDconfig):
    case (TypeId::Id_CspadConfig):
      _cache_config( xtc );
      break;
    default:
      break;
    }

    if (xtc->contains.compressed()) {

      if (_extract) {   // Anything to decompress?
        
        boost::shared_ptr<Xtc> pxtc = Pds::CompressedXtc::uncompress(*xtc);
        if (pxtc.get()) {
          _write(pxtc.get(), pxtc->extent);
          return;
        }
        else {
          printf("extract failed\n");
        }
      }
    }
    else if (!_extract) {  // Anything to compress?

      unsigned headerSize=0;
      unsigned depth=0;
      std::list<unsigned> headerOffsets;

      if (xtc->damage.value()) 
        ;
      else {

        switch (xtc->contains.id()) {

        case (TypeId::Id_Frame) :
          switch (xtc->contains.version()) {
          case 1:
          { const Camera::FrameV1& frame = *reinterpret_cast<const Camera::FrameV1*>(xtc->payload());
            headerOffsets.push_back(0);
            headerSize = sizeof(frame);
            depth      = frame.depth_bytes();
            break; }
          default: break; } break;

        case (TypeId::Id_pnCCDframe) :
          switch(xtc->contains.version()) {
          case 1:
          { const PNCCD::FrameV1* frame = reinterpret_cast<const PNCCD::FrameV1*>(xtc->payload());
            headerSize = sizeof(*frame);
            depth      = 2;
            const char* payload = xtc->payload();

#define PNCCD_VERSION(v) {                                              \
            XtcMapKey key(*xtc, TypeId(TypeId::Id_pnCCDconfig, v));     \
            if (_xtcmap.find(key)!=_xtcmap.end()) {                     \
              const PNCCD::ConfigV##v& config = *reinterpret_cast<const PNCCD::ConfigV##v*>(_xtcmap[key]->payload()); \
              headerOffsets.push_back(0);                               \
              headerOffsets.push_back((char*)(frame=frame->next(config)) - payload); \
              headerOffsets.push_back((char*)(frame=frame->next(config)) - payload); \
              headerOffsets.push_back((char*)(frame=frame->next(config)) - payload); \
              break; } }

            PNCCD_VERSION(1);
            PNCCD_VERSION(2);
            depth = 0;
            break; }
          default: break; } break;

        case (TypeId::Id_Cspad2x2Element) :
          { headerOffsets.push_back(0);
            headerSize = sizeof(CsPad2x2::ElementHeader);
            depth      = 2; 
            break; }

        case (TypeId::Id_CspadElement) :
          { CsPad::ElementIterator* iter = _lookup_iterator(xtc);
            const Pds::CsPad::ElementHeader* hdr;
            while( (hdr = iter->next()) )
              headerOffsets.push_back((char*)hdr - xtc->payload());
            headerSize = sizeof(CsPad::ElementHeader);
            depth      = 2;
            break; }

        case (TypeId::Id_TimepixData) :
          switch(xtc->contains.version()) {
          case 1:
          { const Timepix::DataV1& frame = *reinterpret_cast<const Timepix::DataV1*>(xtc->payload());
            headerOffsets.push_back(0);
            headerSize = sizeof(frame);
            depth      = frame.DepthBytes;
            break; }
          case 2:
          { const Timepix::DataV2& frame = *reinterpret_cast<const Timepix::DataV2*>(xtc->payload());
            headerOffsets.push_back(0);
            headerSize = sizeof(frame);
            depth      = Timepix::DataV1::DepthBytes;
            break; }
          default: break; } break;

        default:
          break; 
        }
      }

      if (depth) {
        Xtc* cxtc = new (_pwrite) CompressedXtc(*xtc, 
                                                headerOffsets,
                                                headerSize,
                                                depth,
                                                _engine);
        _pwrite += cxtc->extent/sizeof(uint32_t);
        return;
      }

    }
    _write(xtc,xtc->extent);
  }

private:
  //
  //  Xtc headers are 32b aligned.  
  //  Compressed data is not.
  //  Enforce alignment during Xtc construction.
  //
  char* _new(ssize_t sz)
  {
    uint32_t* p = _pwrite;
    _pwrite += sz>>2;
    return (char*)p;
  }

  void _write(const void* p, ssize_t sz) 
  {
    if (!_aligned)
      perror("Writing 32b data alignment not guaranteed\n");

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

  char*                                     _outbuf;
  unsigned                                  _outsz;

  map<XtcMapKey, const Xtc*>                _xtcmap;
};

void usage(char* progname) {
  fprintf(stderr,
          "Usage: %s -i <filename> [-o <filename>] [-n events] [-x] [-h] [-1] [-2]\n"
          "       -i <filename>  : input xtc file\n"
          "       -o <filename>  : output xtc file\n"
          "       -n <events>    : number to process\n"
          "       -x : extract (decompress)\n"
          "       -1 : use Hist16 algorithm\n"
          "       -2 : use HistN  algorithm\n"
          "If -o is omitted, then compress, uncompress, and memcmp the result\n",
          progname);
}

int main(int argc, char* argv[]) {
  int c;
  char* inxtcname=0;
  char* outxtcname=0;
  bool extract=false;
  int parseErr = 0;
  unsigned nevents = -1;

  while ((c = getopt(argc, argv, "hxn:i:o:12")) != -1) {
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
    case '1':
      _engine = Pds::CompressedPayload::Hist16;
      break;
    case '2':
      _engine = Pds::CompressedPayload::HistN;
      break;
    default:
      parseErr++;
    }
  }
  
  if (!inxtcname) {
    usage(argv[0]);
    exit(2);
  }

  int ifd = open(inxtcname, O_RDONLY | O_LARGEFILE);
  if (ifd < 0) {
    perror("Unable to open input file\n");
    exit(2);
  }

  FILE* ofd = 0;
  if (outxtcname) {
    ofd = fopen(outxtcname,"wx");
    if (ofd == 0) {
      perror("Unable to open output file\n");
      exit(2);
    }
  }
  
  const unsigned MAX_DG_SIZE = 0x2000000;
  XtcFileIterator iter(ifd,MAX_DG_SIZE);
  Dgram* dg;

  uint32_t* obuff = new uint32_t[MAX_DG_SIZE>>2];
  uint32_t* dbuff = new uint32_t[MAX_DG_SIZE>>2];

  unsigned long long total_payload=0, total_comp=0;

  myIter cmpiter(false);
  myIter deciter(true);

  while ((dg = iter.next())) {
    //    if (!dg->seq.isEvent())

    if (ofd) {
      if (extract)
        deciter.iterate(dg, obuff);
      else
        cmpiter.iterate(dg, obuff);
    }
    else {
      cmpiter.iterate(dg, obuff);
      deciter.iterate(reinterpret_cast<const Dgram*>(obuff), dbuff);
      if (memcmp(dg,dbuff,sizeof(*dg)+dg->xtc.sizeofPayload()))
        printf("  memcmp failed\n");
    }
    
    const Dgram* odg = reinterpret_cast<const Dgram*>(obuff);
    if (ofd) {
      fwrite(odg, sizeof(*odg) + odg->xtc.sizeofPayload(), 1, ofd);
      fflush(ofd);
    }

    printf("%s transition: time 0x%x/0x%x, payloadSize %d (%d)\n",TransitionId::name(dg->seq.service()),
           dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(), dg->xtc.sizeofPayload(), odg->xtc.sizeofPayload());

    total_payload += dg ->xtc.sizeofPayload();
    total_comp    += odg->xtc.sizeofPayload();

    if (dg->seq.isEvent())
      if (--nevents == 0)
        break;
  }
  
  printf("total payload %lld  comp %lld  %f%%\n",
         total_payload, total_comp, 100*double(total_comp)/double(total_payload));

  close (ifd);
  if (ofd)
    fclose(ofd);
  return 0;
}

