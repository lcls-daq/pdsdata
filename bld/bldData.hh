#ifndef BLD_DATA_H
#define BLD_DATA_H

#include <stdint.h>

namespace Pds 
{
    
class BldDataFEEGasDetEnergy
{
    // PV names: GDET:FEE1:11:ENRC,GDET:FEE1:12:ENRC,GDET:FEE1:21:ENRC,GDET:FEE1:22:ENRC
public:
    /* Data structure */
    double f_11_ENRC;   /* in mJ */ 
    double f_12_ENRC;   /* in mJ */ 
    double f_21_ENRC;   /* in mJ */
    double f_22_ENRC;   /* in mJ */
    
    int print() const;
};

class BldDataEBeam
{
public:
    uint32_t    uDamageMask;
    double      fEbeamCharge;    /* in nC */ 
    double      fEbeamL3Energy;  /* in MeV */ 
    double      fEbeamLTUPosX;   /* in mm */ 
    double      fEbeamLTUPosY;   /* in mm */ 
    double      fEbeamLTUAngX;   /* in mrad */ 
    double      fEbeamLTUAngY;   /* in mrad */  
    
    int print() const;    
};

class BldDataPhaseCavity
{    
public:
    /* Data structure */
    double f1;   /* in ?? Unit */ 
    double f2;   /* in ?? Unit */ 
    double f3;   /* in ?? Unit */ 
    double f4;   /* in ?? Unit */ 
    
    int print() const;
};

}
#endif
