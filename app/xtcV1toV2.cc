#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "pdsdata/xtc/XtcFileIterator.hh"

using std::cerr;
using std::endl;
using namespace Pds;

static const unsigned MAX_SIZE = 0x1000000;

#if 0
static void sigfunc(int sig_no) {
  cout << endl << "Caught " << strsignal(sig_no) << " signal, exiting..." << endl;
  exit(0);
}
#endif

void usage(char* progname) {
  cerr << "Usage: " << progname << " -i <input file> -o <output_file>" << endl;
}

int main(int argc, char* argv[]) {
  // Exactly one of these values must be supplied
  char* inFile = NULL;
  char* outFile = NULL;

  int c;
  while ((c = getopt(argc, argv, "i:o:h?"))!=-1) {  
    switch (c) {
      case 'i':
        inFile = optarg;
        break;
      case 'o':
        outFile = optarg;
        break;
      default:
        fprintf(stderr, "Unrecognized option -%c!\n", c);
        usage(argv[0]);
        exit(0);
    }
  }
  int ifd = open(inFile , O_RDONLY | O_LARGEFILE);
  if (ifd<0) {
    perror("Opening input file");
    return -1; 
  }

  FILE* ffd = fopen(outFile,"w");
  if (!ffd) {
    perror("Opening output file");
    return -2; 
  }

  XtcFileIterator it(ifd, MAX_SIZE);

  Dgram* dg;
  while((dg=it.next())) {
    //Convert LCLS1 Datagram header to LCLS2
    {
      uint64_t fiducials = dg->seq.stamp().fiducials()&0x1ffff00ffffffULL;
      unsigned control   = (dg->seq.stamp().fiducials()>>24)&0xff;
      *new(const_cast<Pds::TimeStamp*>(&dg->seq.stamp())) Pds::TimeStamp(fiducials,control);
    }
    ::fwrite(dg, dg->xtc.sizeofPayload()+sizeof(*dg), 1, ffd);
  }
  ::fclose(ffd);
}
