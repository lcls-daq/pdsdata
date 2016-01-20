#!/bin/bash 

#===================================================================
# Script for building pdsdata 
#
# This script is in pdsdata/pdstools/build.sh.  Call it as follows:
# cd myrelease
# ./pdstools/build.sh
# 
# The script assumes it will be run from this directory. 
#===================================================================

function build()
{
    make -C $1 clean 
    make -C $1 CFLAGS='-I${NDARRAY} -I${BOOST}' $2 
    make -C $1 INSTALLDIR=$3 install
}
export NDARRAY=/reg/common/package/ndarray/1.1.3/x86_64-rhel5-gcc41-opt
export BOOST=/reg/common/package/boost/1.49.0-python2.7/x86_64-rhel5-gcc41-opt/include


echo "--------------------------------------------------"
echo " Make machine generated code"
echo "--------------------------------------------------"
#make -C pdsdata gen
make -C pdsdata GENDEVEL=1 gen

#============================================================
# Do build and install for all specified packages.  
#============================================================
CURRENTDIR="${PWD}"

ARCH=`uname -r`
if [[ $ARCH == *el7* ]]; then
    TGT="x86_64-rhel7"
elif [[ $ARCH == *el6* ]]; then
    TGT="x86_64-rhel6"
elif [[ $ARCH == *el5* ]]; then
    TGT="x86_64-linux"
fi

OPT="-opt -dbg"
VER="8.4.6"

for opt in $OPT; do
    tgt=${TGT}${opt}
    echo $tgt
#    INS=${CURRENTDIR}/install/${tgt}
    INS="/reg/common/package/pdsdata/${VER}/${tgt}"

    echo -e "\n============================================================================================="
    echo "Building pdsdata for $tgt and installing in $INS"
    echo -e "============================================================================================="
    build pdsdata $tgt $INS
done

#TGT="x86_64-linux-dbg"
#INS=${CURRENTDIR}/install/${TGT}
#
#echo -e "\n============================================================================================="
#echo "Building pdsdata for $TGT and installing in $INS"
#echo -e "============================================================================================="
#build pdsdata $TGT $INS


#=================================================================================
# Return to the original directory when done
#=================================================================================
cd $CURRENTDIR

