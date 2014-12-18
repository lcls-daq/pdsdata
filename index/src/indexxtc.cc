#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "pdsdata/xtc/XtcFileIterator.hh"
#include "pdsdata/index/IndexXtcIterL1Accept.hh"
//#include "pdsdata/index/IndexList.hh"

using namespace Pds;
using Index::XtcIterL1Accept;
using Index::IndexList;
using Index::XtcObj;
using std::vector;

void usage(char *progname)
{
  printf(
    "Usage:  %s  [-f <xtc filename>] [-i <input index>] [-o <index filename>] [-s <size threshold>] [-h]\n"
    "  Options:\n"
    "    -h                     Show usage.\n"
    "    -f <xtc filename>      Set input xtc filename\n"
    "    -i <index filename>    Set input index filename\n"
    "       Note 1: -i option is used for testing. The program will parse\n"
    "         the index file to see if it is valid, and output to another\n"
    "         index file if -o option is specified\n"
    "       Note 2: -i will overwrite -f option\n"
    "    -o <index filename>    Set output index filename\n"
    "    -s <size threshold>    Set L1Accept xtc size threshold\n",
    progname
  );
}

int generateIndex(char* sXtcFilename, char* sOutputIndex, uint32_t uSizeThreshold);
int readIndex    (char* sInputIndex , char* sOutputIndex);

int main(int argc, char *argv[])
{
  int c;
  char      *sXtcFilename    = NULL;
  char      *sOutputIndex    = NULL;
  char      *sInputIndex     = NULL;
  uint32_t  uSizeThreshold   = 1024; // 1KB

  while ((c = getopt(argc, argv, "hf:o:i:s:")) != -1)
  {
    switch (c)
    {
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'f':
      sXtcFilename    = optarg;
      break;
    case 'o':
      sOutputIndex    = optarg;
      break;
    case 'i':
      sInputIndex     = optarg;
      break;
    case 's':
      uSizeThreshold  = strtoul(optarg, NULL, 0);
      break;
    default:
      printf( "Unknown option: -%c", c );
    }
  }

  if (! (sXtcFilename || sInputIndex) )
  {
    usage(argv[0]);
    exit(2);
  }

  if ( sInputIndex )
    return readIndex( sInputIndex, sOutputIndex );
  else
    return generateIndex( sXtcFilename, sOutputIndex, uSizeThreshold );
}

int writeData(int fd, void* buf, int count)
{
  for (int curPos = 0; curPos < count;)
  {
    int numWritten = ::write(fd, (char*)buf + curPos, count - curPos);
    if (numWritten == -1) return 1;
    curPos += numWritten;
  }
  return 0;
}

int generateIndex(char* sXtcFilename, char* sOutputIndex, uint32_t uSizeThreshold)
{
  int fd = open(sXtcFilename, O_RDONLY | O_LARGEFILE);
  if (fd < 0)
  {
    printf("Unable to open xtc file %s\n", sXtcFilename);
    return 1;
  }


  int fdIndex = -1;
  if ( sOutputIndex != NULL )
  {
    fdIndex = open(sOutputIndex, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
    if ( fdIndex == -1 )
    {
      printf( "generateIndex(): Open index file %s failed (%s)\n", sOutputIndex, strerror(errno) );
      ::close(fd);
      return 2;
    }
  }

  vector<XtcObj>    xtcObjPool;
  XtcObj            xtcIndexConfig;
  XtcObj            xtcIndexTag;
  XtcFileIterator   iterFile  (fd, 0x4000000); // largest L1 data: 64 MB
  unsigned          lquiet    (0);
  int               debug      = 0;

  Dgram *dg;
  int64_t i64Offset = lseek64(fd, 0, SEEK_CUR);
  while ((dg = iterFile.next()))
  {
    uint32_t dgramOffset = sizeof(Dgram);
    if (debug == 1) printf("dg 0x%Lx\n", (long long) i64Offset);
    if (dg->seq.service() == TransitionId::L1Accept)
    {
      xtcObjPool.clear();
      XtcIterL1Accept iterL1Accept(&(dg->xtc), 0, i64Offset + sizeof(*dg), dgramOffset, uSizeThreshold, xtcObjPool, lquiet);
      iterL1Accept.iterate();

      typedef std::vector<XtcIterL1Accept::XtcInfo> XtcInfoList;
      XtcInfoList& xtcInfoList = iterL1Accept.xtcInfoList();
      for (size_t i=0; i < xtcInfoList.size(); ++i)
      {
        Xtc* pOrgXtc = (Xtc*)((char*)dg + (long) (xtcInfoList[i].i64Offset - i64Offset));
        Xtc* pXtc    = (xtcInfoList[i].iPoolIndex == -1? pOrgXtc : &xtcObjPool[xtcInfoList[i].iPoolIndex].xtc);
        if (debug == 1) printf("  %sOffset 0x%Lx size 0x%x xtcSize 0x%x (pool %d) orgSize 0x%x (dgram offset 0x%lx)\n", std::string(xtcInfoList[i].depth*2, ' ').c_str(),
               (long long) xtcInfoList[i].i64Offset, xtcInfoList[i].uSize, pXtc->extent,
               xtcInfoList[i].iPoolIndex, pOrgXtc->extent,
               (long) (xtcInfoList[i].i64Offset - i64Offset));
      }

      if (fdIndex != -1)
      {
        if (writeData(fdIndex, dg, sizeof(*dg)-sizeof(Xtc)) != 0)
        {
          printf("generateIndex(): failed to write to indx file\n");
          return 3;
        }

        new ((char*)&xtcIndexTag) Xtc           (dg->xtc);
        new (xtcIndexTag.tagV1)   Index::TagV1  (i64Offset, dg->xtc.extent);
        xtcIndexTag.xtc.src      = Src(Level::Recorder);
        xtcIndexTag.xtc.contains = TypeId(TypeId::Id_IndexTag, 1);
        xtcIndexTag.xtc.extent   = sizeof(Xtc) + sizeof(Index::TagV1);

        XtcInfoList& xtcInfoList = iterL1Accept.xtcInfoList();
        xtcInfoList[0].uSize    += xtcIndexTag.xtc.extent;
        for (size_t i=0; i < xtcInfoList.size(); ++i)
        {
          Xtc* pOrgXtc       = (Xtc*)((char*)dg + (long) (xtcInfoList[i].i64Offset - i64Offset));
          Xtc* pXtc          = (xtcInfoList[i].iPoolIndex == -1? pOrgXtc : &xtcObjPool[xtcInfoList[i].iPoolIndex].xtc);
          pXtc->extent       = xtcInfoList[i].uSize;
          uint32_t sizeWrite = ((i == xtcInfoList.size()-1 || xtcInfoList[i].depth >= xtcInfoList[i+1].depth) ?
                                 pXtc->extent : sizeof(Xtc));
          if(writeData(fdIndex, pXtc, sizeWrite) != 0)
          {
            printf("generateIndex(): failed to write to indx file\n");
            return 3;
          }
          if (i ==0 && writeData(fdIndex, (char*)&xtcIndexTag, xtcIndexTag.xtc.extent) != 0)
          {
            printf("generateIndex(): failed to write to indx file\n");
            return 3;
          }
        }
      } // if (fdIndex != -1)
    }
    else if (dg->seq.service() == TransitionId::Configure)
    {
      if (fdIndex != -1)
      {
        new ((char*)&xtcIndexConfig)  Xtc             (dg->xtc);
        new (xtcIndexConfig.configV1) Index::ConfigV1 (uSizeThreshold);
        xtcIndexConfig.xtc.src      = Src(Level::Recorder);
        xtcIndexConfig.xtc.contains = TypeId(TypeId::Id_IndexConfig, 1);
        xtcIndexConfig.xtc.extent   = sizeof(Xtc) + sizeof(Index::ConfigV1);

        uint32_t sizeofPayloadOrg = dg->xtc.sizeofPayload();
        dg->xtc.extent += xtcIndexConfig.xtc.extent;
        if (writeData(fdIndex, dg, sizeof(*dg)) != 0 ||
            writeData(fdIndex, (char*)&xtcIndexConfig, xtcIndexConfig.xtc.extent) != 0 ||
            writeData(fdIndex, dg->xtc.payload(), sizeofPayloadOrg) != 0)
        {
          printf("generateIndex(): failed to write to indx file\n");
          return 3;
        }
      }
    }
    else
    {
      if (fdIndex != -1 &&
          writeData(fdIndex, dg, sizeof(*dg)+dg->xtc.sizeofPayload()) != 0)
      {
        printf("generateIndex(): failed to write to indx file\n");
        return 3;
      }
    }

    i64Offset = lseek64(fd, 0, SEEK_CUR); // get the file offset for the next iteration
  }

  if(fdIndex != -1)
    ::close(fdIndex);

  ::close(fd);

  return 0;
}

int readIndex(char* sInputIndex, char* sOutputIndex)
{
  int fd = open(sInputIndex, O_RDONLY | O_LARGEFILE);
  if (fd < 0)
  {
    printf("Unable to open xtc file %s\n", sInputIndex);
    return 1;
  }

  //IndexList indexList;
  //indexList.readFromFile(fd);

  ::close(fd);

  //int iVerbose = 2;
  //indexList.printList(iVerbose);
  //
  //if ( sOutputIndex != NULL )
  //{
  //  //int fdIndex = open(sOutputIndex, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
  //  int fdIndex = open(sOutputIndex, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ); //!! debug
  //  if ( fdIndex == -1 )
  //    printf( "xtcIndex(): Open index file %s failed (%s)\n", sOutputIndex, strerror(errno) );
  //  else
  //  {
  //    indexList.writeToFile(fdIndex);
  //    ::close(fdIndex);
  //  }
  //}

  return 0;
}
