#ifndef BLD_DATA_H
#define BLD_DATA_H

#include <stdint.h>
#include "pdsdata/ipimb/ConfigV1.hh"
#include "pdsdata/ipimb/DataV1.hh"
#include "pdsdata/ipimb/ConfigV2.hh"
#include "pdsdata/ipimb/DataV2.hh"
#include "pdsdata/lusi/IpmFexV1.hh"
#include "pdsdata/lusi/PimImageConfigV1.hh"
#include "pdsdata/pulnix/TM6740ConfigV2.hh"
#include "pdsdata/camera/FrameV1.hh"
#include "pdsdata/xtc/DetInfo.hh"

typedef Pds::Ipimb::DataV1   IpimbDataV1;
typedef Pds::Ipimb::ConfigV1 IpimbConfigV1; 
typedef Pds::Ipimb::DataV2   IpimbDataV2;
typedef Pds::Ipimb::ConfigV2 IpimbConfigV2; 
typedef Pds::Lusi::IpmFexV1  IpmFexDataV1;

namespace Pds 
{
    
#pragma pack(4)

class BldDataFEEGasDetEnergy
{
    // PV names: GDET:FEE1:11:ENRC,GDET:FEE1:12:ENRC,GDET:FEE1:21:ENRC,GDET:FEE1:22:ENRC
public:
  enum { version=0 };
    double f_11_ENRC;   /* in mJ */ 
    double f_12_ENRC;   /* in mJ */ 
    double f_21_ENRC;   /* in mJ */
    double f_22_ENRC;   /* in mJ */
    
    int print() const;
};

class BldDataEBeamV0
{
public:
  enum { version=0 };
    uint32_t    uDamageMask;
    double      fEbeamCharge;    /* in nC */ 
    double      fEbeamL3Energy;  /* in MeV */ 
    double      fEbeamLTUPosX;   /* in mm */ 
    double      fEbeamLTUPosY;   /* in mm */ 
    double      fEbeamLTUAngX;   /* in mrad */ 
    double      fEbeamLTUAngY;   /* in mrad */  
    
    int print() const;    
};


class BldDataEBeamV1
{
public:
  enum { version=1 };
    uint32_t    uDamageMask;
    double      fEbeamCharge;    /* in nC */ 
    double      fEbeamL3Energy;  /* in MeV */ 
    double      fEbeamLTUPosX;   /* in mm */ 
    double      fEbeamLTUPosY;   /* in mm */ 
    double      fEbeamLTUAngX;   /* in mrad */ 
    double      fEbeamLTUAngY;   /* in mrad */  
    double      fEbeamPkCurrBC2; /* in Amps */  
    
    int print() const;    
};


class BldDataEBeamV2
{
public:
  enum { version=2 };
    uint32_t    uDamageMask;
    double      fEbeamCharge;    /* in nC */ 
    double      fEbeamL3Energy;  /* in MeV */ 
    double      fEbeamLTUPosX;   /* in mm */ 
    double      fEbeamLTUPosY;   /* in mm */ 
    double      fEbeamLTUAngX;   /* in mrad */ 
    double      fEbeamLTUAngY;   /* in mrad */  
    double      fEbeamPkCurrBC2; /* in Amps */  
    double      fEbeamEnergyBC2; /* in MeV */  
    
    int print() const;    
};

class BldDataEBeamV3
{
public:
  enum { version=3 };
    uint32_t    uDamageMask;
    double      fEbeamCharge;     /* in nC */ 
    double      fEbeamL3Energy;   /* in MeV */ 
    double      fEbeamLTUPosX;    /* in mm */ 
    double      fEbeamLTUPosY;    /* in mm */ 
    double      fEbeamLTUAngX;    /* in mrad */ 
    double      fEbeamLTUAngY;    /* in mrad */  
    double      fEbeamPkCurrBC2;  /* in Amps */ 
    double      fEbeamEnergyBC2;  /* in mm */
    double      fEbeamPkCurrBC1; /* Amps */
    double      fEbeamEnergyBC1;  /* in mm */
    
    int print() const;    
};


typedef BldDataEBeamV3 BldDataEBeam;

class BldDataPhaseCavity
{
    // PV names: UND:R02:IOC:16:BAT:FitTime1, UND:R02:IOC:16:BAT:FitTime2, 
    //           UND:R02:IOC:16:BAT:Charge1,  UND:R02:IOC:16:BAT:Charge2
public:
  enum { version=0 };
    double fFitTime1;   /* in pico-seconds */ 
    double fFitTime2;   /* in pico-seconds */ 
    double fCharge1;    /* in pico-columbs */ 
    double fCharge2;    /* in pico-columbs */ 
    
    int print() const;
};


class BldDataIpimbV0
{
public:
  enum { version=0 }; 
    IpimbDataV1    ipimbData;
    IpimbConfigV1  ipimbConfig;
    IpmFexDataV1   ipmFexData;
    
    int print() const;    
};


class BldDataIpimbV1
{
public:
  enum { version=1 }; 
    IpimbDataV2    ipimbData;
    IpimbConfigV2  ipimbConfig;
    IpmFexDataV1   ipmFexData;
    
    int print() const;    
};

typedef BldDataIpimbV1 BldDataIpimb;

class BldDataPimV1
{
public:
  enum { version=1 };
  Pulnix::TM6740ConfigV2   camConfig;
  Lusi::PimImageConfigV1   pimConfig;
  Camera::FrameV1          frame;
};

class BldDataGMDV0
{
public:
  enum { version=0 };
  char     strGasType[32];  // Gas Type
  float    fPressure;       // Pressure from Spinning Rotor Gauge, SXR:GMD:SRG:01:Pressure
  float    fTemperature;    // Temp from PT100, SXR:GMD:RTD:40:RAW_AI
  float    fCurrent;        // Current from Keithley Electrometer, SXR:GMD:ETM:01:Reading
  int32_t  iHvMeshElectron; // HV Mesh Electron, SXR:GMD:VHQ1:ChA:VoltageMeasure
  int32_t  iHvMeshIon;      // HV Mesh Ion,      SXR:GMD:VHQ1:ChB:VoltageMeasure
  int32_t  iHvMultIon;      // HV Mult Ion,      SXR:GMD:VHQ1:ChB:VoltageMeasure
  float    fChargeQ;        // Charge Q, SXR:GMD:IMD:Charge_Q
  float    fPhotonEnergy;   // Photon Energy, SIOC:SYS0:ML00:AO627
  float    fPhotonsPerPulse;// Photons Per Pulse, SXR:GMD:IMD:CalcIMD:PhotonsPerPulse
  float    fSpare1;         // Spare 1
  float    fSpare2;         // Spare 2
  float    fSpare3;         // Spare 3
  float    fSpare4;         // Spare 4
  float    fSpare5;         // Spare 5
  float    fSpare6;         // Spare 6
      
  int print() const;    
};

typedef BldDataGMDV0 BldDataGMD;

#pragma pack()
}
#endif
