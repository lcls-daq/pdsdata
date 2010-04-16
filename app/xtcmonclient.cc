#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "XtcMonitorClient.hh"

void usage(char* progname) {
  fprintf(stderr,"Usage: %s [-p <partitionTag>] [-i <index>] [-h]\n", progname);
}


int main(int argc, char* argv[]) {
  int c;
  const char* partitionTag;
  unsigned index = 0;

  while ((c = getopt(argc, argv, "?hi:p:")) != -1) {
    switch (c) {
    case '?':
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'i':
      index = strtoul(optarg,NULL,0);
      break;
    case 'p':
      partitionTag = optarg;
      break;
    default:
      usage(argv[0]);
    }
  }

  Pds::XtcMonitorClient myClient;
  fprintf(stderr, "myClient returned: %d\n", myClient.run(partitionTag,index));
  
  return 1;
}
