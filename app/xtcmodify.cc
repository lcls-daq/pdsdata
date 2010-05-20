
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"
#include "pdsdata/camera/FrameV1.hh"
#include "pdsdata/fccd/FccdConfigV1.hh"

using namespace Pds;

class myLevelIter : public XtcIterator {
public:
  enum {Stop, Continue};
  myLevelIter(Xtc* xtc, unsigned depth) : XtcIterator(xtc), _depth(depth) {}

  void process(const DetInfo& d, const Camera::FrameV1& f) {
    printf("*** Processing frame object\n");
  }
  void process(const DetInfo&, const FCCD::FccdConfigV1&) {
    printf("*** Processing FCCD config object\n");
  }
  int process(Xtc* xtc) {
    unsigned i=_depth; while (i--) printf("  ");
    Level::Type level = xtc->src.level();
    printf("%s level  payload size %d contains: %s: ",Level::name(level), xtc->sizeofPayload(), TypeId::name(xtc->contains.id()));
    const DetInfo& info = *(DetInfo*)(&xtc->src);
    if (level==Level::Source) {
      printf("%s,%d  %s,%d\n",
             DetInfo::name(info.detector()),info.detId(),
             DetInfo::name(info.device()),info.devId());
    } else {
      ProcInfo& info = *(ProcInfo*)(&xtc->src);
      printf("IpAddress 0x%x ProcessId 0x%x\n",info.ipAddr(),info.processId());
    }
    if (level < 0 || level >= Level::NumberOfLevels )
    {
        printf("Unsupported Level %d\n", (int) level);
        return Continue;
    }    
    switch (xtc->contains.id()) {
    case (TypeId::Id_Xtc) : {
      myLevelIter iter(xtc,_depth+1);
      iter.iterate();
      break;
    }
    case (TypeId::Id_Frame) :
      process(info, *(const Camera::FrameV1*)(xtc->payload()));
      break;
    case (TypeId::Id_FccdConfig) :
      process(info, *(const FCCD::FccdConfigV1*)(xtc->payload()));
      break;
    default :
      break;
    }
    return Continue;
  }
private:
  unsigned _depth;
};

void usage(char* progname) {
  fprintf(stderr,"Usage: %s -f <filename> -o <outfilename> [-h]\n", progname);
}

int main(int argc, char* argv[]) {
  int c;
  char* xtcname=0;
  char* outfilename=0;
  int parseErr = 0;

  while ((c = getopt(argc, argv, "hf:o:")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'f':
      xtcname = optarg;
      break;
    case 'o':
      outfilename = optarg;
      break;
    default:
      parseErr++;
    }
  }
  
  if (!xtcname || !outfilename) {
    usage(argv[0]);
    exit(2);
  }

  int fd = open(xtcname, O_RDONLY | O_LARGEFILE);
  if (fd < 0) {
    perror("Unable to open file %s\n");
    exit(2);
  }

  FILE* writefd = fopen(outfilename, "w");
  printf("*********opening file %p\n",writefd);
  
  XtcFileIterator iter(fd,0x900000);
  Dgram* dg;
  
  while ((dg = iter.next())) {
    printf("%s transition: time 0x%x/0x%x, payloadSize 0x%x\n",TransitionId::name(dg->seq.service()),
           dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(),dg->xtc.sizeofPayload());
    myLevelIter iter(&(dg->xtc),0);
    iter.iterate();
    ::fwrite(dg,sizeof(*dg)+dg->xtc.sizeofPayload(),1,writefd);
  }

  ::close(fd);
  ::fclose(writefd);
  return 0;
}
