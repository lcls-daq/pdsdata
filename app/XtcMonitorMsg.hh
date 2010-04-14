#ifndef Pds_XtcMonitorMsg_hh
#define Pds_XtcMonitorMsg_hh

namespace Pds {
  class XtcMonitorMsg {
  public:
    XtcMonitorMsg() {}
    XtcMonitorMsg(int bufferIndex) {_bufferIndex = bufferIndex;}
    ~XtcMonitorMsg() {}; 
  public:
    int bufferIndex     () const {return _bufferIndex;}
    int numberOfBuffers () const { return _numberOfBuffers; }
    int sizeOfBuffers   () const { return _sizeOfBuffers; }
  public:
    XtcMonitorMsg* bufferIndex(int b) {_bufferIndex=b; return this;}
    void numberOfBuffers      (int n) {_numberOfBuffers = n;} 
    void sizeOfBuffers        (int s) {_sizeOfBuffers = s;} 
  private:
    int _bufferIndex;
    int _numberOfBuffers;
    unsigned _sizeOfBuffers;
  };
};

#endif
