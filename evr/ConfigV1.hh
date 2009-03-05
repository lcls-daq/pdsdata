#ifndef Evr_ConfigV1_hh
#define Evr_ConfigV1_hh

#include "pdsdata/xtc/TypeId.hh"
#include <stdint.h>

namespace EvrData {

  class PulseConfig {
  public:
    PulseConfig ();
    PulseConfig (unsigned pulse,                 // Pulse ID
		 int      trigger,               // Pulse input control
		 int      set,
		 int      clear,
		 bool     polarity,              // Pulse output control
		 bool     map_set_enable,
		 bool     map_reset_enable,
		 bool     map_trigger_enable,
		 unsigned prescale,              // Pulse parameters
		 unsigned delay,
		 unsigned width);
  public:
    unsigned pulse  () const;

    int trigger() const;
    int set    () const;
    int clear  () const;

    bool polarity          () const;
    bool map_set_enable    () const;
    bool map_reset_enable  () const;
    bool map_trigger_enable() const;

    unsigned prescale() const;
    unsigned delay   () const;
    unsigned width   () const;
  private:
    uint32_t _pulse;
    uint32_t _input_control;
    uint32_t _output_control;
    uint32_t _prescale;
    uint32_t _delay;
    uint32_t _width;
  };

  class OutputMap {
  public:
    enum Source { Pulse, DBus, Prescaler, Force_High, Force_Low };
    enum Conn { FrontPanel, UnivIO };
  public:
    OutputMap ();
    OutputMap ( Source, unsigned source_id, 
		Conn  , unsigned conn_id );
  public:
    Source   source   () const;
    unsigned source_id() const;
    Conn     conn     () const;
    unsigned conn_id  () const;
  public:
    unsigned map      () const;
  private:
    uint32_t _v;
  };

  class ConfigV1 {
  public:
    ConfigV1 ();
    ConfigV1 (unsigned npulses,
	      const PulseConfig* pulses,
	      unsigned noutputs,
	      const OutputMap* outputs);

    unsigned npulses() const;
    const PulseConfig& pulse(unsigned) const;

    unsigned noutputs() const;
    const OutputMap& output_map(unsigned) const;

    unsigned size() const;
  private:
    uint32_t _npulses;
    uint32_t _noutputs;
  };
}

#endif
