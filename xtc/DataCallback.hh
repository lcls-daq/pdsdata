#ifndef Pds_DataCallback_hh
#define Pds_DataCallback_hh

namespace Pds {
  namespace Acqiris {
    class ConfigV1;
  }
  class WaveformV1;
  
  class DataCallback {
  public:
    virtual void process(const Acqiris::ConfigV1&)=0;
    virtual void process(const WaveformV1&)=0;
  };
}

#endif
