// see usage() routine below for description

// for the exafel demo, ran it like this:
// xtcfilter -t exafel/demo/hit_timestamps.txt -f /reg/d/psdm/cxi/cxid9114/xtc/e438-r0095-s01-c00.xtc -o junk.xtc -i 0xac151a26 0xac151a9a
// 0xac151a26: CxiDg3-0|Opal1000-0 and CxiDg4-0|Tm6740-0
// 0xac151a9a: DetInfo CxiDs1-0|Cspad-0
// 0xac151a8d:  EpicsArch-0|NoDevice-1 (to filter that out of smd)

// in general for the exafel repacking we need to:
// - run xtcfilter removing the misses and unused large detectors
// - generate the smd files using https://github.com/chrisvam/psana_cpo/blob/master/xtcsmldata_smd.py
// - run xtcfilter on the smd files removing epics (workaround for slow psana processing of epics)

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "pdsdata/xtc/DetInfo.hh"
#include "pdsdata/xtc/BldInfo.hh"
#include "pdsdata/xtc/ProcInfo.hh"
#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/xtc/XtcFileIterator.hh"

using namespace Pds;

class filterIter : public XtcIterator {
public:
  enum {Stop, Continue};
  filterIter(Xtc* xtc, Xtc* newxtc, std::vector<uint32_t> ipaddrs, bool dumpIpInfo) : XtcIterator(xtc), _newxtc(newxtc), _ipaddrs(ipaddrs), _dumpIpInfo(dumpIpInfo) {}

  int process(Xtc* xtc) {
    Level::Type level = xtc->src.level();
    if (_dumpIpInfo)
      printf("%s level payload size %d contains %s damage 0x%x: ",
             Level::name(level), xtc->sizeofPayload(), TypeId::name(xtc->contains.id()),
             xtc->damage.value());
    if (level==Level::Source) {
      DetInfo& info = *(DetInfo*)(&xtc->src);
      if (_dumpIpInfo) printf("DetInfo %s\n",DetInfo::name(info));
    } else if (level==Level::Reporter) {
      BldInfo& info = *(BldInfo*)(&xtc->src);
      if (_dumpIpInfo) printf("BldInfo %s\n",BldInfo::name(info));
    } else if (level==Level::Segment) {
      ProcInfo& info = *(ProcInfo*)(&xtc->src);
      bool copyEvent = true;
      for(std::vector<uint32_t>::iterator it = _ipaddrs.begin(); it != _ipaddrs.end(); ++it) {
        if (info.ipAddr()==*it) copyEvent = false;
      }
      if (copyEvent) {
        void* dest = _newxtc->alloc(xtc->extent);
        memcpy(dest,xtc,xtc->extent);
      }
      if (_dumpIpInfo)
        printf("IpAddress 0x%x ProcessId 0x%x\n",info.ipAddr(),info.processId());
    } else if (level==Level::Recorder) {
      // this is to save the EventOffset info in smd files
      void* dest = _newxtc->alloc(xtc->extent);
      memcpy(dest,xtc,xtc->extent);
    }
    switch (xtc->contains.id()) {
    case (TypeId::Id_Xtc) : {
      iterate(xtc);
      break;
    }
    default :
      break;
    }
    return Continue;
  }
private:
  Xtc* _newxtc;
  std::vector<uint32_t> _ipaddrs;
  bool _dumpIpInfo;
};

void usage(char* progname) {
  fprintf(stderr,"\nReads old xtc files, then dumps out new xtc files\n"
          "while only keeping a specific set of timestamps.  It also\n"
          "removes segment-level Xtc's for particular detectors\n"
          "(described by ip addresses).  On the first event it dumps\n"
          "out information allowing the user to determine the appropriate\n"
          "ip addresses\n\n");

  fprintf(stderr,"Usage: %s -f <filename> -o <outfilename> [-t timestampfilename] [-i ipaddr1 ipaddr2...] [-n nevtmax] [-h]\n", progname);
}

unsigned long long cctbx_time(Dgram& dg) {
  unsigned sec = dg.seq.clock().seconds();
  unsigned nsec = dg.seq.clock().nanoseconds();
  unsigned msec = nsec/1000000;
  struct tm * timeinfo;
  time_t tval = sec;
  timeinfo = gmtime (&tval);

  char buffer [80];
  strftime (buffer,80,"%Y%m%d%H%M%S",timeinfo);

  unsigned long long cctbx_sec;
  sscanf(buffer,"%llu",&cctbx_sec);
  return ((unsigned long long)cctbx_sec)*1000+msec;
}

void writeNewDgram(FILE* xtcoutfd, Dgram& olddg, Dgram& newdg, std::vector<uint32_t> ipaddrs, bool dumpIpInfo) {
  newdg.seq = olddg.seq;
  newdg.env = olddg.env;
  newdg.xtc = olddg.xtc;
  newdg.xtc.extent=sizeof(Xtc);
  filterIter iter(&(olddg.xtc),&(newdg.xtc), ipaddrs, dumpIpInfo);
  iter.iterate();
  if (::fwrite(&newdg,sizeof(newdg)+newdg.xtc.sizeofPayload(),1,xtcoutfd) != 1) {
    perror("::fwrite");
  }
}

int main(int argc, char* argv[]) {
  int c;
  char* outfilename=0;
  char* timefilename=0;
  int nevtmax = -1;
  int nevt;
  int parseErr = 0;
  std::vector<uint32_t> ipaddrs;
  std::vector<std::string> filenames;
  int argindex;
  char* next;
  std::string filename;
  
  while ((c = getopt(argc, argv, "hf:o:n:t:i:")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'f':
      argindex = optind-1;
      while(argindex < argc){
        filename = strdup(argv[argindex]);
        argindex++;
        if(filename[0] != '-'){ /* check if optarg is next switch */
          filenames.push_back(filename);
        }
        else {
          optind = argindex - 1;
          break;
        }
      }
      break;
    case 'i':
      argindex = optind-1;
      while(argindex < argc){
        next = strdup(argv[argindex]);
        argindex++;
        if(next[0] != '-'){ /* check if optarg is next switch */
          uint32_t ipaddr;
          sscanf(next,"%i",&ipaddr);
          printf("Will remove all data from ip addr 0x%x\n",ipaddr);
          ipaddrs.push_back(ipaddr);
        }
        else {
          optind = argindex - 1;
          break;
        }
      }
      break;
    case 't':
      timefilename = optarg;
      break;
    case 'o':
      outfilename = optarg;
      break;
    case 'n':
      if (sscanf(optarg, "%d", &nevtmax) != 1) {
        usage(argv[0]);
        exit(2);
      }
      break;
    default:
      parseErr++;
    }
  }

  if (filenames.size()==0 || !outfilename) {
    usage(argv[0]);
    exit(2);
  }

  std::vector<unsigned long long> hittime(1000000);
  if (timefilename) {
    FILE* hitf = fopen(timefilename,"r");
    char junkrunnum[20];
    unsigned ntime=0;
    // for the exafel demo, currently using the cctbx YYYYMMDDHHMMSSmmm
    // format (m is milliseconds).
    while (fscanf(hitf,"%6s%llu",junkrunnum,&hittime[ntime])!=EOF) {
      ntime++;
      if (ntime>=hittime.size()) {
        printf("Too many times in file: %d\n",ntime);
        return -1;
      }
    }
    hittime.resize(ntime);
    std::sort(hittime.begin(),hittime.end());
    printf("Found %d times\n",ntime);
  }

  FILE* xtcoutfd = fopen(outfilename, "w");
  if (xtcoutfd == NULL) {
    perror("Unable to open output file");
    exit(2);
  }
  
  Dgram* dg;

  void *newdgaddr = malloc(0x1500000);
  Dgram& newdg = *new(newdgaddr) Dgram();
  
  nevt = 0;
  unsigned nsaved = 0;
  bool dumpIpInfo = true;
  for(std::vector<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it) {
    printf("fname %s\n",it->c_str());
  }
  for(std::vector<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it) {

    int xtcinfd = open(it->c_str(), O_RDONLY | O_LARGEFILE);
    if (xtcinfd < 0) {
      perror("Unable to open input file");
      exit(2);
    }

    XtcFileIterator iter(xtcinfd,0x1500000);
    while ((dg = iter.next())) {
      if (nevt==nevtmax) break;
      nevt++; 
      // printf("%s transition: time 0x%x/0x%x, payloadSize 0x%x\n",
      //        TransitionId::name(dg->seq.service()),
      //        dg->seq.stamp().fiducials(),dg->seq.stamp().ticks(),
      //        dg->xtc.sizeofPayload());
      if ((dg->seq.service() == TransitionId::L1Accept)) {
        std::vector<unsigned long long>::iterator it;
        unsigned long long mytime = cctbx_time(*dg);
        bool writeEvent = true;
        if (timefilename) {
          it = lower_bound(hittime.begin(),hittime.end(),mytime);
          writeEvent = *it==mytime;
        }
        if (writeEvent) {
          nsaved++;
          writeNewDgram(xtcoutfd, *dg, newdg, ipaddrs, dumpIpInfo);
          if (dumpIpInfo) dumpIpInfo = false;
        }
      } else {
        writeNewDgram(xtcoutfd, *dg, newdg, ipaddrs, false);
      }
    }
    ::close(xtcinfd);
  }

  ::fclose(xtcoutfd);
  printf("Saved %d events\n",nsaved);
  return 0;
}
