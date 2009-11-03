
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"

class myLevelIter : public XtcIterator {
public:
  enum {Stop, Continue};
  myLevelIter(Xtc* xtc, unsigned depth) : XtcIterator(xtc), _depth(depth) {}

  int process(Xtc* xtc) {
    unsigned i=_depth; while (i--) printf("  ");
    Level::Type level = xtc->src.level();
    printf("%s level contains %s  dmg %x: ",
	   Level::name(level), 
	   TypeId::name(xtc->contains.id()),
	   xtc->damage.value());
    if (xtc->contains.id() == TypeId::Id_Xtc) {
      myLevelIter iter(xtc,_depth+1);
      iter.iterate();
    }
    return Continue;
  }
private:
  unsigned _depth;
};

void usage(char* progname) {
  fprintf(stderr,"Usage: %s -f <filename> [-h]\n", progname);
}

int main(int argc, char* argv[]) {
  int c;
  char* xtcname=0;
  int parseErr = 0;
  unsigned damage = -1;

  while ((c = getopt(argc, argv, "hf:d:")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'f':
      xtcname = optarg;
      break;
    case 'd':
      damage = strtoul(optarg,NULL,0);
      break;
    default:
      parseErr++;
    }
  }
  
  if (!xtcname) {
    usage(argv[0]);
    exit(2);
  }

  FILE* file = fopen(xtcname,"r");
  if (!file) {
    perror("Unable to open file %s\n");
    exit(2);
  }

  XtcFileIterator iter(file,0x900000);
  Dgram* dg;
  unsigned long long bytes=0;
  while ((dg = iter.next())) {
    if (dg->xtc.damage.value()&damage) {
      printf("%s transition: time %08x/%08x  stamp %08x/%08x, payloadSize 0x%x  pos 0x%llx\n",
	     TransitionId::name(dg->seq.service()),
	     dg->seq.clock().seconds(),dg->seq.clock().nanoseconds(),
	     dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(),dg->xtc.sizeofPayload(),
	     bytes);
      myLevelIter iter(&(dg->xtc),0);
      iter.iterate();
    }
    bytes += sizeof(*dg) + dg->xtc.sizeofPayload();
  }

  fclose(file);
  return 0;
}
