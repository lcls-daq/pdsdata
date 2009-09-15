#include <stdio.h>
#include "pdsdata/bld/bldData.hh"

int BldDataFEEGasDetEnergy::print() const
{    
    printf("GDET:FEE:11:ENRC: %lf\n", f_11_ENRC );
    printf("GDET:FEE:12:ENRC: %lf\n", f_12_ENRC );
    printf("GDET:FEE:21:ENRC: %lf\n", f_21_ENRC );
    printf("GDET:FEE:22:ENRC: %lf\n", f_22_ENRC );
    
    return 0;
}
