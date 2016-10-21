#ifndef Pds_XtcMonitorMsg_hh
#define Pds_XtcMonitorMsg_hh

#include <stdint.h>

namespace Pds {
  class XtcMonitorMsg {
  public:
    XtcMonitorMsg() : _offset_size(0),
                      _port       (0),
                      _queue      (0),
                      _return     (0),
                      _reserved   (0) {}
    XtcMonitorMsg(int port) : _offset_size(0),
                              _port       (port),
                              _queue      (0),
                              _return     (0),
                              _reserved   (0) {}
    ~XtcMonitorMsg() {} 
  public:
    //  Location of buffer
    uint64_t bufferOffset () const { return _offset_size; } 
    //  Size of shared memory segment
    uint64_t sizeOfMem    () const { return _offset_size; }
    //  TCP port for initialization
    int trPort            () const { return _port; }
    //  Message queue identifier
    int queueIndex        () const { return _queue; }
    //  Message queue for returned events
    int return_queue      () const { return _return; }

  public:
    void bufferOffset     (uint64_t v) { _offset_size = v; }
    void sizeOfMem        (uint64_t v) { _offset_size = v; }
    void queueIndex       (int v)      { _queue  = v; }
    void return_queue     (int v)      { _return = v; }
  public:
    static void sharedMemoryName     (const char* tag, char* buffer);
    static void eventInputQueue      (const char* tag, unsigned client, char* buffer);
    static void eventOutputQueue     (const char* tag, unsigned client, char* buffer);
    static void transitionInputQueue (const char* tag, unsigned client, char* buffer);
    static void discoveryQueue       (const char* tag, char* buffer);
    static void registerQueue        (const char* tag, char* buffer, int id);
  private:
    uint64_t _offset_size;
    int16_t  _port;
    int16_t  _queue;
    int16_t  _return;
    int16_t  _reserved;
  };
};

#endif
