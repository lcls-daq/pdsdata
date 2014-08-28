
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "pdsdata/xtc/XtcFileIterator.hh"

using namespace Pds;

void usage(char* progname) {
  fprintf(stderr,"Usage: %s -f <filename> -o <outfilename> [-i <imagefilename>] [-c calibCycleCountToOutput] [-s <selectpayloadname>] [-h]\n", progname);
}

int main(int argc, char* argv[]) {
  int c;
  char* xtcname=0;
  char* outfilename=0;
  unsigned copycount=0;
  int parseErr = 0;

  while ((c = getopt(argc, argv, "f:o:c:")) != -1) {
    switch (c) {
    case 'f':
      xtcname = optarg;
      printf("xtcreduce file(%s)\n", xtcname);
      break;
    case 'o':
      outfilename = optarg;
      printf("xtcreduce outfile(%s)\n", outfilename);
      break;
    case 'c':
      if (sscanf(optarg, "%d", &copycount) != 1) {
        usage(argv[0]);
        exit(2);
      }
      break;
    default:
      usage(argv[0]);
      parseErr++;
    }
  }
  
  if (!xtcname || !outfilename) {
    usage(argv[0]);
    exit(2);
  }

  int xtcinfd = open(xtcname, O_RDONLY | O_LARGEFILE);
  if (xtcinfd < 0) {
    perror("Unable to open input file");
    exit(2);
  }

  FILE* xtcoutfd = fopen(outfilename, "w");
  if (xtcoutfd == NULL) {
    perror("Unable to open output file");
    exit(2);
  }
  
  XtcFileIterator iter(xtcinfd,0x900000);
  Dgram* dg;
  
  unsigned ncalibCopied = 0;

  while ((dg = iter.next())) {
    TransitionId::Value service = dg->seq.service();
    if (ncalibCopied < copycount || service==TransitionId::EndRun) {
      printf("%s transition: time 0x%x/0x%x, payloadSize 0x%x\n",TransitionId::name(dg->seq.service()),
           dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(),dg->xtc.sizeofPayload());

      if (::fwrite(dg,sizeof(*dg)+dg->xtc.sizeofPayload(),1,xtcoutfd) != 1) {
        perror("::fwrite");
        break;
      }
      if (service == TransitionId::EndCalibCycle) ncalibCopied++;
    }
  }

  ::close(xtcinfd);
  ::fclose(xtcoutfd);
  return 0;
}
