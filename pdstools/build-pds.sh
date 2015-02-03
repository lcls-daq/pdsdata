#!/bin/bash 

#===================================================================
# Script for building pdsdata and psalg for DAQ purposes.
#
# The main goal of this script was to make it easy to
#     1) build pdsdata for all targets for local use (development) 
#     2) create a formal pdsdata/psalg release in /reg/common/package
#
# The script is configurable and can build either of the two packages,
# any of the targets, and install the build products in a 
# specified install directory for both rhel5 and rhel6.
#
# The script does not interact with SVN to create a release tag
# nor does it try to set up an offline environment for compilation.
# The user still needs to do this part manually:
#      source /reg/g/psdm/etc/ana_env.csh
#      newrel ana-current myrelease
#      cd myrelease
#      sit_setup
#      addpkg psddl HEAD
#      scons
#      addpkg psddldata HEAD
#      svn co file:///afs/slac/g/pcds/svn/pdsdata/trunk pdsdata
#      
# 
# This script is in pdsdata/pdstools/build-pds.sh.  Call it as follows:
# cd myrelease
# ./pdstools/build-pds.sh
# 
# The script assumes it will be run from this directory. 
#===================================================================


#============================================================
# Functions
#============================================================

function build()
{
    make -C $1 clean
    make -C $1 CFLAGS='-I${NDARRAY} -I${BOOST}' $2
    make -C $1 INSTALLDIR=$3 install
}

function usage_brief()
{
    echo -e "\nUsage: $0 [-p <package>] [-i <installdir>] [-t <target>] [-r,v <version> ] [-g] [-h]\n" 1>&2;
    echo -e "       -p <package>   \tBuild specified package only. Default builds pdsdata."
    echo -e "                      \t<package> = pdsdata,psalg\n";
    echo -e "       -i <installdir>\tDirectory in which to install build products. Default is current directory";
    echo -e "                      \t<installdir> = install path, e.g. /reg/common/package/ or ~/myrelease/install";
    echo -e "                      \t Script will create pdsdata or psalg subdirectory in this root directory as needed \n";
    echo -e "       -t <target>    \tBuild specified target only.  Default builds all."; 
    echo -e "                      \t<target> = i386-linux-opt,i386-linux-dbg,x86_64-linux-opt,x86_64-linux-dbg";
    echo -e "                      \t<target> = opt,dbg,i386,x86(or x86_64)";
    echo -e "                      \t<target> = opt  --> i386-linux-opt, x86_64-linux-opt";
    echo -e "                      \t<target> = dbg  --> i386-linux-dbg, x86_64-linux-dbg\n";
    echo -e "                      \t<target> = i386 --> i386-linux-opt, i386-linux-dbg\n";
    echo -e "                      \t<target> = x86  --> x86_64-linux-opt, x86_64-linux-dbg\n";
    echo -e "       -r,v <version> \tCreates a new release in /reg/common/package/<package>/<version>";
    echo -e "                      \tIgnores -i and -t flags.";
    echo -e "                      \t<version> = test, devel, X.Y.Z, e.g. 8.1.3 (or next version number)";
    echo -e "       -g             \tBuild machine-generated code for pdsdata - exclude devel types"
    echo -e "       -d             \tBuild machine-generated code for pdsdata - include devel types"
    echo -e "       -n             \tDO NOT build machine-generated code for pdsdata                      "
    echo -e "       -h             \tPrint this usage information\n";
    echo -e "For full usage information with examples, try $0 -h\n"
    exit 1;
}

function usage()
{
    echo -e "\nUsage: $0 [-p <package>] [-i <installdir>] [-t <target>] [-r,v <version> ] [-g] [-h]\n" 1>&2;
    echo -e "       -p <package>   \tBuild specified package only. Default builds pdsdata."
    echo -e "                      \t<package> = pdsdata,psalg\n";
    echo -e "       -i <installdir>\tDirectory in which to install build products. Default is current directory";
    echo -e "                      \t<installdir> = install path, e.g. /reg/common/package/ or ~/myrelease/install";
    echo -e "                      \t Script will create pdsdata or psalg subdirectory in this root directory as needed \n";
    echo -e "       -t <target>    \tBuild specified target only.  Default builds all."; 
    echo -e "                      \t  <target> = i386-linux-opt,i386-linux-dbg,x86_64-linux-opt,x86_64-linux-dbg";
    echo -e "                      \t  <target> = opt,dbg,i386,x86(or x86_64)";
    echo -e "                      \t  <target> = opt  --> i386-linux-opt, x86_64-linux-opt";
    echo -e "                      \t  <target> = dbg  --> i386-linux-dbg, x86_64-linux-dbg";
    echo -e "                      \t  <target> = i386 --> i386-linux-opt, i386-linux-dbg";
    echo -e "                      \t  <target> = x86  --> x86_64-linux-opt, x86_64-linux-dbg";
    echo -e "                      \tIf compiling on rhel6, the target name will automatically be changed to x86_64-rhel6-*\n"
    echo -e "       -r,v <version> \tCreates a new release in /reg/common/package/<package>/<version>";
    echo -e "                      \tIgnores -i and -t flags.";
    echo -e "                      \t<version> = test, devel, X.Y.Z, e.g. 8.1.3 (or next version number)";
    echo -e "       -d             \tBuild machine-generated code for pdsdata and get generated code for devel types"
    echo -e "       -g             \tBuild machine-generated code for pdsdata and DO NOT generate code for devel types"
    echo -e "       -n             \tDO NOT build machine-generated code for pdsdata                      "
    echo -e "       -h             \tPrint this usage information\n";
    echo -e "----------";
    echo -e "Examples:  ";
    echo -e "----------";
    echo -e "Build pdsdata for all targets in current working directory:";
    echo -e "\t$0 ";
    echo -e "Build psalg for i386-linux-opt in current working directory:";
    echo -e "\t$0 -p psalg -t i386-linux-opt";
    echo -e "Build psalg for i386-linux-opt and x86_64-linux-opt in current working directory:";
    echo -e "\t$0 -p psalg -t i386-linux-opt,x86_64-linux-opt";
    echo -e "\t$0 -t opt";
    echo -e "\t$0 -t i386";
    echo -e "Create a pdsdata release, version 9.0.0";
    echo -e "\t$0 -r 9.0.0";
    echo -e "\t$0 -v 9.0.0";
    echo -e "Create a pdsdata release, version 9.0.0,and a psalg release, version 2.0.0"
    echo -e "\t$0 -p pdsdata,psalg -v 9.0.0,2.0.0  <----- Note: package order is the same as version number order"
    echo -e "Create a pdsdata and psalg release in various include directories:"
    echo -e "\t$0 -p pdsdata,psalg  --> Creates <current_directory>/install/pdsdata and <current_directory>/install/psalg"
    echo -e "\t$0 -p pdsdata,psalg -i ~/foo/bar  --> Creates ~/foo/bar/pdsdata and ~/foo/bar/psalg"
    echo -e "\t$0 -p pdsdata,psalg -i ~/blah/this,~/blah/that  --> Creates ~/blah/this/pdsdata and ~/blah/that/psalg\n"
    
    exit 1;
}

#=================================================================================
# cd to the directory above pdsdata to compile
#=================================================================================
ORIGINALDIR="${PWD}"
FILE=`find . -name "SConstruct" -type -f -print -quit`
if [ -n "$FILE" ]; then
    echo "Found $FILE"
else
    cd ..
    if [ -n "$FILE" ]; then    
	echo "Found $FILE"
    else
	cd ../
    fi
fi

echo `pwd`

#=================================================================================
# Set reasonable values for defaults: targets=all, packages=pdsdata, version=test
#=================================================================================
DEF_TARGETS="i386-linux-opt i386-linux-dbg x86_64-linux-opt x86_64-linux-dbg"
DEF_PACKAGES=( "pdsdata" )
CURRENTDIR="${PWD}"
DEF_INSTALLROOT=${CURRENTDIR}/install
DEF_INSTALLDIR=()

TARGETS=$DEF_TARGETS
PACKAGES=$DEF_PACKAGES
BUILD_MACHGEN_CODE="false"
BUILD_MACHGENDEVEL_CODE="false"
MAKE_FORMAL_RELEASE="false"
x86_64_arch='unknown'
if [[ `uname -r` == *el5* ]]; then
    x86_64_arch='x86_64-linux'
elif [[ `uname -r` == *el6* ]]; then
    x86_64_arch='x86_64-rhel6'
fi

export NDARRAY=/reg/common/package/ndarray/1.1.3/x86_64-rhel5-gcc41-opt
export BOOST=/reg/common/package/boost/1.49.0-python2.7/x86_64-rhel5-gcc41-opt/include

#=================================================================================
# Parse input arguments
#=================================================================================
OPTSTRING="hdgnp:i:t:r:v:"
while getopts $OPTSTRING opt; do
    case $opt in 
	h) 
	    usage;
	    exit 0
	    ;;
	d) 
	    BUILD_MACHGEN_CODE="true"
	    BUILD_MACHGENDEVEL_CODE="true"
	    ;;
	g)
 	    BUILD_MACHGEN_CODE="true"
	    ;;
	n)
	    BUILD_MACHGEN_CODE="false"
	    BUILD_MACHGENDEVEL_CODE="false"
	    ;;
	p)
	    PACKAGES=(${OPTARG/,/' '})
	    ;;
	i) 
	    DEF_INSTALLDIR=(${OPTARG/,/' '})		
	    ;;
	t)
	    for tgt in ${OPTARG/,/''}; do
		if [ "opt" == "$tgt" ]; then
		    TARGETS="i386-linux-opt x86_64-linux-opt"
		elif [ "dbg" == "$tgt" ]; then
		    TARGETS="i386-linux-dbg x86_64-linux-dbg";
		elif [ "i386" == "$tgt" ]; then
		    TARGETS="i386-linux-opt i386-linux-dbg";
		elif [[ "x86" == "$tgt" ]] || [[ "x86_64" == "$tgt" ]]; then
		    TARGETS="x86_64-linux-opt x86_64-linux-dbg";
		else
		    TARGETS=${OPTARG/,/' '}
		fi
	    done
	    ;;
	r|v) 
	    MAKE_FORMAL_RELEASE="true"
	    BUILD_MACHGEN_CODE="true"	    
 	    VERSION=(${OPTARG/,/' '});
	    ;;
	\?)
	    echo "Option requires an argument.">&2;
	    echo "(If you're looking for default behavior, omit the flag entirely)";
	    usage_brief;
	    exit 1
	    ;;
    esac
done

#=================================================================================
# Consistency checks of input parameters:  
#=================================================================================
#        package names should be correctly spelled (valid: [pdsdata,psalg])
#        target names should be correctly spelled (valid: [opt,dbg,i386,x86_64,
#               i386-linux-opt,i386-linux-dbg,x86_64-linux-opt,x86_64-linux-dbg])
#        -r,-v ignores -i and -t input paramters; sets installdir and targets to release defaults
#        if -v is selected then the # packages == # version numbers provided
#
#        if not a formal release and multiple packages are given, create separate install dir for each package

# Check that package names are pdsdata or psalg
for ((n=0;n<${#PACKAGES[@]};++n)); do
    if [[ "${PACKAGES[$n]}" != "pdsdata" && "${PACKAGES[$n]}" != "psalg" ]]; then
	echo "Error: Parameter <package>=${pkg} is invalid.  Use pdsdata or psalg (or check your spelling).";
	exit 1;
    fi
done

# Check that target names are correctly spelled
for target in $TARGETS; do
    if [[ "${target}" != "i386-linux-dbg" && "${target}" != "i386-linux-opt" && "${target}" != "x86_64-linux-opt" && "${target}" != "x86_64-linux-dbg" ]]; then
	echo "Error:  Parameter <target>=${target} is invalid.  Check your spelling.";
	exit 1;
    fi
done

# If -r or -v is given, set default targets, check length of arrays, and set install directory appropriately
if [ $MAKE_FORMAL_RELEASE == "true" ]; then 
    NPACKAGES=${#PACKAGES[@]}
    NVERSION=${#VERSION[@]}
    TARGETS=$DEF_TARGETS

    # Sanity check - make sure the number of packages equals the number of versions
    if [ $NPACKAGES -ne  $NVERSION ]; then
	echo "Error:  Number of packages (${#PACKAGES[@]}) != number of versions (${#VERSION[@]}):"
	for ((n=0;n<${#PACKAGES[@]};++n)); do
	    echo -e "\t${PACKAGES[$n]}\t--> ${VERSION[$n]}"
	done
	exit 1
    fi

    # Set install directory appropriately for package/version
    for ((n=0;n<${#PACKAGES[@]};++n)); do
	INSTALLDIRS[$n]="/reg/common/package/${PACKAGES[$n]}/${VERSION[$n]}"
    done

# If not a formal release, create a separate install directory for each package
#      If there are two install directories given, use the directories given (append pkg name)
#      If only one, assume both packages will go in that install directory (in separate pkg directories).
#      If none, use current working directory (with pkg subdirectories)
else
    # Set install directory appropriately for package/version
    if [ ${#DEF_INSTALLDIR[@]} -eq 0 ]; then
	for ((n=0;n<${#PACKAGES[@]};++n)); do
	    INSTALLDIRS[$n]="${DEF_INSTALLROOT}/${PACKAGES[$n]}"
	done
    elif [ ${#DEF_INSTALLDIR[@]} -eq 1 ]; then
	for ((n=0;n<${#PACKAGES[@]};++n)); do
	    INSTALLDIRS[$n]="${DEF_INSTALLDIR}/${PACKAGES[$n]}"
	done
    elif [ ${#DEF_INSTALLDIR[@]} -eq 2 ]; then
	for ((n=0;n<${#PACKAGES[@]};++n)); do
	    INSTALLDIRS[$n]="${DEF_INSTALLDIR[$n]}/${PACKAGES[$n]}"
	done
    else
	echo -e "Error:  Too many install directories."
	exit 1
    fi

    NPACKAGES=${#PACKAGES[@]}
    NINSTALLDIR=${#INSTALLDIRS[@]}
    if [ $NPACKAGES -ne $NINSTALLDIR ]; then
	echo "Error:  Number of packages ($NPACKAGES} not equal to number of install directories ($NINSTALLDIR)"
    fi
fi
#===================================================================================
# Print packages, targets, install directories, and release version (if appropriate)
#===================================================================================
echo -e "\n======================================================================================="
echo -e "Packages to be built: " ${PACKAGES[@]};
echo -e "Targets to be built:  " $TARGETS;
echo -e "Install directory:    "
for ((n=0;n<${#PACKAGES[@]};++n)); do 
    echo -e "\t${INSTALLDIRS[$n]}" 
done

if [ $MAKE_FORMAL_RELEASE == "true" ]; then 
echo "Making formal release(s) for:"
    for ((n=0;n<${#PACKAGES[@]};++n)); do
	echo -e "\t${PACKAGES[$n]} release\t--> ${VERSION[$n]}"
    done
fi
echo -e "======================================================================================="

#============================================================
# If requested, build machine-generated code
#============================================================
if [ $BUILD_MACHGEN_CODE == "true" ]; then

    if [ $BUILD_MACHGENDEVEL_CODE == "true" ]; then
	echo -e "\n===  Build pdsdata machine-generated code with devel tag ==="
	make -C pdsdata GENDEVEL=1 gen
    else
	echo -e "\n===  Build pdsdata machine-generated code, skip devel tag ==="
	make -C pdsdata gen	
    fi
fi

#============================================================
# Do build and install for all specified packages.  
#============================================================
for ((n=0;n<${#PACKAGES[@]};++n)); do 
    export MYINSTALLDIR=${INSTALLDIRS[$n]}

    if [[ ! -e $MYINSTALLDIR ]]; then
	echo -e "\nCreating directory $MYINSTALLDIR"
	mkdir -p $MYINSTALLDIR
    elif [[ -d $MYINSTALLDIR ]]; then
	echo -e "\nDirectory $MYINSTALLDIR already exists. Continue and overwrite? (y/n)"
	read ANSWER
	if [[ "$ANSWER" =~ ^([yY])|([yY][eE][sS])$ ]]; then
	    echo "Regenerating contents of $MYINSTALLDIR"
	else
	    echo "Exiting."
	    exit 1
	fi
    fi

    mkdir -p $MYINSTALLDIR

    for target in $TARGETS; do
	tgt=${target/x86_64-linux/$x86_64_arch}
	inst=${MYINSTALLDIR}/$tgt
	echo $inst
	echo -e "\n=== Build ${PACKAGE[$n]} ${tgt} and install to ${MYINSTALLDIR} ==="
	echo ${PACKAGES[$n]}  $tgt $inst
	build ${PACKAGES[$n]} $tgt $inst
    done
done

#=================================================================================
# Return to the original directory when done
#=================================================================================
cd $ORIGINALDIR

