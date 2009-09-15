#ifndef BLD_DATA_H
#define BLD_DATA_H

class BldDataFEEGasDetEnergy
{
    // PV names: GDET:FEE1:11:ENRC,GDET:FEE1:12:ENRC,GDET:FEE1:21:ENRC,GDET:FEE1:22:ENRC
public:
    /* Data structure */
    double f_11_ENRC;
    double f_12_ENRC;
    double f_21_ENRC;
    double f_22_ENRC;
    
    int print() const;
};

#endif
