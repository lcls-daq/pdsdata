#ifndef PDS_BLD_DDL_H
#define PDS_BLD_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
#include "pdsdata/psddl/acqiris.ddl.h"
#include "pdsdata/psddl/camera.ddl.h"
#include "pdsdata/psddl/ipimb.ddl.h"
#include "pdsdata/psddl/lusi.ddl.h"
#include "pdsdata/psddl/pulnix.ddl.h"
namespace Pds {
namespace Bld {

/** @class BldDataFEEGasDetEnergy

  Four energy measurements from Front End Enclosure Gas Detector.
               PV names: GDET:FEE1:11:ENRC, GDET:FEE1:12:ENRC, GDET:FEE1:21:ENRC, GDET:FEE1:22:ENRC.
*/

#pragma pack(push,4)

class BldDataFEEGasDetEnergy {
public:
  enum { TypeId = Pds::TypeId::Id_FEEGasDetEnergy /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };
  BldDataFEEGasDetEnergy(double arg__f_11_ENRC, double arg__f_12_ENRC, double arg__f_21_ENRC, double arg__f_22_ENRC)
    : _f_11_ENRC(arg__f_11_ENRC), _f_12_ENRC(arg__f_12_ENRC), _f_21_ENRC(arg__f_21_ENRC), _f_22_ENRC(arg__f_22_ENRC)
  {
  }
  BldDataFEEGasDetEnergy() {}
  /** Value of GDET:FEE1:11:ENRC, in mJ. */
  double f_11_ENRC() const { return _f_11_ENRC; }
  /** Value of GDET:FEE1:12:ENRC, in mJ. */
  double f_12_ENRC() const { return _f_12_ENRC; }
  /** Value of GDET:FEE1:21:ENRC, in mJ. */
  double f_21_ENRC() const { return _f_21_ENRC; }
  /** Value of GDET:FEE1:22:ENRC, in mJ. */
  double f_22_ENRC() const { return _f_22_ENRC; }
  static uint32_t _sizeof() { return 32; }
private:
  double	_f_11_ENRC;	/**< Value of GDET:FEE1:11:ENRC, in mJ. */
  double	_f_12_ENRC;	/**< Value of GDET:FEE1:12:ENRC, in mJ. */
  double	_f_21_ENRC;	/**< Value of GDET:FEE1:21:ENRC, in mJ. */
  double	_f_22_ENRC;	/**< Value of GDET:FEE1:22:ENRC, in mJ. */
};
#pragma pack(pop)

/** @class BldDataEBeamV0

  Beam parameters.
*/

#pragma pack(push,4)

class BldDataEBeamV0 {
public:
  enum { TypeId = Pds::TypeId::Id_EBeam /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };

  /** Constants defining bit mask for individual damage bits in value returned from damageMask() */
  enum DamageMask {
    EbeamChargeDamage = 0x001,
    EbeamL3EnergyDamage = 0x002,
    EbeamLTUPosXDamage = 0x004,
    EbeamLTUPosYDamage = 0x008,
    EbeamLTUAngXDamage = 0x010,
    EbeamLTUAngYDamage = 0x020,
  };
  BldDataEBeamV0(uint32_t arg__uDamageMask, double arg__fEbeamCharge, double arg__fEbeamL3Energy, double arg__fEbeamLTUPosX, double arg__fEbeamLTUPosY, double arg__fEbeamLTUAngX, double arg__fEbeamLTUAngY)
    : _uDamageMask(arg__uDamageMask), _fEbeamCharge(arg__fEbeamCharge), _fEbeamL3Energy(arg__fEbeamL3Energy), _fEbeamLTUPosX(arg__fEbeamLTUPosX), _fEbeamLTUPosY(arg__fEbeamLTUPosY), _fEbeamLTUAngX(arg__fEbeamLTUAngX), _fEbeamLTUAngY(arg__fEbeamLTUAngY)
  {
  }
  BldDataEBeamV0() {}
  /** Damage mask. */
  uint32_t damageMask() const { return _uDamageMask; }
  /** Beam charge in nC. */
  double ebeamCharge() const { return _fEbeamCharge; }
  /** Beam energy in MeV. */
  double ebeamL3Energy() const { return _fEbeamL3Energy; }
  /** LTU beam position in mm. */
  double ebeamLTUPosX() const { return _fEbeamLTUPosX; }
  /** LTU beam position in mm. */
  double ebeamLTUPosY() const { return _fEbeamLTUPosY; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngX() const { return _fEbeamLTUAngX; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngY() const { return _fEbeamLTUAngY; }
  static uint32_t _sizeof() { return 52; }
private:
  uint32_t	_uDamageMask;	/**< Damage mask. */
  double	_fEbeamCharge;	/**< Beam charge in nC. */
  double	_fEbeamL3Energy;	/**< Beam energy in MeV. */
  double	_fEbeamLTUPosX;	/**< LTU beam position in mm. */
  double	_fEbeamLTUPosY;	/**< LTU beam position in mm. */
  double	_fEbeamLTUAngX;	/**< LTU beam angle in mrad. */
  double	_fEbeamLTUAngY;	/**< LTU beam angle in mrad. */
};
std::ostream& operator<<(std::ostream& str, Bld::BldDataEBeamV0::DamageMask enval);
#pragma pack(pop)

/** @class BldDataEBeamV1

  
*/

#pragma pack(push,4)

class BldDataEBeamV1 {
public:
  enum { TypeId = Pds::TypeId::Id_EBeam /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };

  /** Constants defining bit mask for individual damage bits in value returned from damageMask() */
  enum DamageMask {
    EbeamChargeDamage = 0x001,
    EbeamL3EnergyDamage = 0x002,
    EbeamLTUPosXDamage = 0x004,
    EbeamLTUPosYDamage = 0x008,
    EbeamLTUAngXDamage = 0x010,
    EbeamLTUAngYDamage = 0x020,
    EbeamPkCurrBC2Damage = 0x040,
  };
  BldDataEBeamV1(uint32_t arg__uDamageMask, double arg__fEbeamCharge, double arg__fEbeamL3Energy, double arg__fEbeamLTUPosX, double arg__fEbeamLTUPosY, double arg__fEbeamLTUAngX, double arg__fEbeamLTUAngY, double arg__fEbeamPkCurrBC2)
    : _uDamageMask(arg__uDamageMask), _fEbeamCharge(arg__fEbeamCharge), _fEbeamL3Energy(arg__fEbeamL3Energy), _fEbeamLTUPosX(arg__fEbeamLTUPosX), _fEbeamLTUPosY(arg__fEbeamLTUPosY), _fEbeamLTUAngX(arg__fEbeamLTUAngX), _fEbeamLTUAngY(arg__fEbeamLTUAngY), _fEbeamPkCurrBC2(arg__fEbeamPkCurrBC2)
  {
  }
  BldDataEBeamV1() {}
  /** Damage mask. */
  uint32_t damageMask() const { return _uDamageMask; }
  /** Beam charge in nC. */
  double ebeamCharge() const { return _fEbeamCharge; }
  /** Beam energy in MeV. */
  double ebeamL3Energy() const { return _fEbeamL3Energy; }
  /** LTU beam position in mm. */
  double ebeamLTUPosX() const { return _fEbeamLTUPosX; }
  /** LTU beam position in mm. */
  double ebeamLTUPosY() const { return _fEbeamLTUPosY; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngX() const { return _fEbeamLTUAngX; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngY() const { return _fEbeamLTUAngY; }
  /** Beam current in Amps. */
  double ebeamPkCurrBC2() const { return _fEbeamPkCurrBC2; }
  static uint32_t _sizeof() { return 60; }
private:
  uint32_t	_uDamageMask;	/**< Damage mask. */
  double	_fEbeamCharge;	/**< Beam charge in nC. */
  double	_fEbeamL3Energy;	/**< Beam energy in MeV. */
  double	_fEbeamLTUPosX;	/**< LTU beam position in mm. */
  double	_fEbeamLTUPosY;	/**< LTU beam position in mm. */
  double	_fEbeamLTUAngX;	/**< LTU beam angle in mrad. */
  double	_fEbeamLTUAngY;	/**< LTU beam angle in mrad. */
  double	_fEbeamPkCurrBC2;	/**< Beam current in Amps. */
};
std::ostream& operator<<(std::ostream& str, Bld::BldDataEBeamV1::DamageMask enval);
#pragma pack(pop)

/** @class BldDataEBeamV2

  
*/

#pragma pack(push,4)

class BldDataEBeamV2 {
public:
  enum { TypeId = Pds::TypeId::Id_EBeam /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 2 /**< XTC type version number */ };

  /** Constants defining bit mask for individual damage bits in value returned from damageMask() */
  enum DamageMask {
    EbeamChargeDamage = 0x001,
    EbeamL3EnergyDamage = 0x002,
    EbeamLTUPosXDamage = 0x004,
    EbeamLTUPosYDamage = 0x008,
    EbeamLTUAngXDamage = 0x010,
    EbeamLTUAngYDamage = 0x020,
    EbeamPkCurrBC2Damage = 0x040,
    EbeamEnergyBC2Damage = 0x080,
  };
  BldDataEBeamV2(uint32_t arg__uDamageMask, double arg__fEbeamCharge, double arg__fEbeamL3Energy, double arg__fEbeamLTUPosX, double arg__fEbeamLTUPosY, double arg__fEbeamLTUAngX, double arg__fEbeamLTUAngY, double arg__fEbeamPkCurrBC2, double arg__fEbeamEnergyBC2)
    : _uDamageMask(arg__uDamageMask), _fEbeamCharge(arg__fEbeamCharge), _fEbeamL3Energy(arg__fEbeamL3Energy), _fEbeamLTUPosX(arg__fEbeamLTUPosX), _fEbeamLTUPosY(arg__fEbeamLTUPosY), _fEbeamLTUAngX(arg__fEbeamLTUAngX), _fEbeamLTUAngY(arg__fEbeamLTUAngY), _fEbeamPkCurrBC2(arg__fEbeamPkCurrBC2), _fEbeamEnergyBC2(arg__fEbeamEnergyBC2)
  {
  }
  BldDataEBeamV2() {}
  /** Damage mask. */
  uint32_t damageMask() const { return _uDamageMask; }
  /** Beam charge in nC. */
  double ebeamCharge() const { return _fEbeamCharge; }
  /** Beam energy in MeV. */
  double ebeamL3Energy() const { return _fEbeamL3Energy; }
  /** LTU beam position in mm. */
  double ebeamLTUPosX() const { return _fEbeamLTUPosX; }
  /** LTU beam position in mm. */
  double ebeamLTUPosY() const { return _fEbeamLTUPosY; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngX() const { return _fEbeamLTUAngX; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngY() const { return _fEbeamLTUAngY; }
  /** Beam current in Amps. */
  double ebeamPkCurrBC2() const { return _fEbeamPkCurrBC2; }
  /** Beam energy in MeV. */
  double ebeamEnergyBC2() const { return _fEbeamEnergyBC2; }
  static uint32_t _sizeof() { return 68; }
private:
  uint32_t	_uDamageMask;	/**< Damage mask. */
  double	_fEbeamCharge;	/**< Beam charge in nC. */
  double	_fEbeamL3Energy;	/**< Beam energy in MeV. */
  double	_fEbeamLTUPosX;	/**< LTU beam position in mm. */
  double	_fEbeamLTUPosY;	/**< LTU beam position in mm. */
  double	_fEbeamLTUAngX;	/**< LTU beam angle in mrad. */
  double	_fEbeamLTUAngY;	/**< LTU beam angle in mrad. */
  double	_fEbeamPkCurrBC2;	/**< Beam current in Amps. */
  double	_fEbeamEnergyBC2;	/**< Beam energy in MeV. */
};
std::ostream& operator<<(std::ostream& str, Bld::BldDataEBeamV2::DamageMask enval);
#pragma pack(pop)

/** @class BldDataEBeamV3

  
*/

#pragma pack(push,4)

class BldDataEBeamV3 {
public:
  enum { TypeId = Pds::TypeId::Id_EBeam /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 3 /**< XTC type version number */ };

  /** Constants defining bit mask for individual damage bits in value returned from damageMask() */
  enum DamageMask {
    EbeamChargeDamage = 0x001,
    EbeamL3EnergyDamage = 0x002,
    EbeamLTUPosXDamage = 0x004,
    EbeamLTUPosYDamage = 0x008,
    EbeamLTUAngXDamage = 0x010,
    EbeamLTUAngYDamage = 0x020,
    EbeamPkCurrBC2Damage = 0x040,
    EbeamEnergyBC2Damage = 0x080,
    EbeamPkCurrBC1Damage = 0x100,
    EbeamEnergyBC1Damage = 0x200,
  };
  BldDataEBeamV3(uint32_t arg__uDamageMask, double arg__fEbeamCharge, double arg__fEbeamL3Energy, double arg__fEbeamLTUPosX, double arg__fEbeamLTUPosY, double arg__fEbeamLTUAngX, double arg__fEbeamLTUAngY, double arg__fEbeamPkCurrBC2, double arg__fEbeamEnergyBC2, double arg__fEbeamPkCurrBC1, double arg__fEbeamEnergyBC1)
    : _uDamageMask(arg__uDamageMask), _fEbeamCharge(arg__fEbeamCharge), _fEbeamL3Energy(arg__fEbeamL3Energy), _fEbeamLTUPosX(arg__fEbeamLTUPosX), _fEbeamLTUPosY(arg__fEbeamLTUPosY), _fEbeamLTUAngX(arg__fEbeamLTUAngX), _fEbeamLTUAngY(arg__fEbeamLTUAngY), _fEbeamPkCurrBC2(arg__fEbeamPkCurrBC2), _fEbeamEnergyBC2(arg__fEbeamEnergyBC2), _fEbeamPkCurrBC1(arg__fEbeamPkCurrBC1), _fEbeamEnergyBC1(arg__fEbeamEnergyBC1)
  {
  }
  BldDataEBeamV3() {}
  /** Damage mask. */
  uint32_t damageMask() const { return _uDamageMask; }
  /** Beam charge in nC. */
  double ebeamCharge() const { return _fEbeamCharge; }
  /** Beam energy in MeV. */
  double ebeamL3Energy() const { return _fEbeamL3Energy; }
  /** LTU beam position in mm. */
  double ebeamLTUPosX() const { return _fEbeamLTUPosX; }
  /** LTU beam position in mm. */
  double ebeamLTUPosY() const { return _fEbeamLTUPosY; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngX() const { return _fEbeamLTUAngX; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngY() const { return _fEbeamLTUAngY; }
  /** Beam current in Amps. */
  double ebeamPkCurrBC2() const { return _fEbeamPkCurrBC2; }
  /** Beam position in mm (related to beam energy). */
  double ebeamEnergyBC2() const { return _fEbeamEnergyBC2; }
  /** Beam current in Amps. */
  double ebeamPkCurrBC1() const { return _fEbeamPkCurrBC1; }
  /** Beam position in mm (related to beam energy). */
  double ebeamEnergyBC1() const { return _fEbeamEnergyBC1; }
  static uint32_t _sizeof() { return 84; }
private:
  uint32_t	_uDamageMask;	/**< Damage mask. */
  double	_fEbeamCharge;	/**< Beam charge in nC. */
  double	_fEbeamL3Energy;	/**< Beam energy in MeV. */
  double	_fEbeamLTUPosX;	/**< LTU beam position in mm. */
  double	_fEbeamLTUPosY;	/**< LTU beam position in mm. */
  double	_fEbeamLTUAngX;	/**< LTU beam angle in mrad. */
  double	_fEbeamLTUAngY;	/**< LTU beam angle in mrad. */
  double	_fEbeamPkCurrBC2;	/**< Beam current in Amps. */
  double	_fEbeamEnergyBC2;	/**< Beam position in mm (related to beam energy). */
  double	_fEbeamPkCurrBC1;	/**< Beam current in Amps. */
  double	_fEbeamEnergyBC1;	/**< Beam position in mm (related to beam energy). */
};
std::ostream& operator<<(std::ostream& str, Bld::BldDataEBeamV3::DamageMask enval);
#pragma pack(pop)

/** @class BldDataEBeamV4

  
*/

#pragma pack(push,4)

class BldDataEBeamV4 {
public:
  enum { TypeId = Pds::TypeId::Id_EBeam /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 4 /**< XTC type version number */ };

  /** Constants defining bit mask for individual damage bits in value returned from damageMask() */
  enum DamageMask {
    EbeamChargeDamage = 0x001,
    EbeamL3EnergyDamage = 0x002,
    EbeamLTUPosXDamage = 0x004,
    EbeamLTUPosYDamage = 0x008,
    EbeamLTUAngXDamage = 0x010,
    EbeamLTUAngYDamage = 0x020,
    EbeamPkCurrBC2Damage = 0x040,
    EbeamEnergyBC2Damage = 0x080,
    EbeamPkCurrBC1Damage = 0x100,
    EbeamEnergyBC1Damage = 0x200,
    EbeamUndPosXDamage = 0x400,
    EbeamUndPosYDamage = 0x800,
    EbeamUndAngXDamage = 0x1000,
    EbeamUndAngYDamage = 0x2000,
  };
  BldDataEBeamV4(uint32_t arg__uDamageMask, double arg__fEbeamCharge, double arg__fEbeamL3Energy, double arg__fEbeamLTUPosX, double arg__fEbeamLTUPosY, double arg__fEbeamLTUAngX, double arg__fEbeamLTUAngY, double arg__fEbeamPkCurrBC2, double arg__fEbeamEnergyBC2, double arg__fEbeamPkCurrBC1, double arg__fEbeamEnergyBC1, double arg__fEbeamUndPosX, double arg__fEbeamUndPosY, double arg__fEbeamUndAngX, double arg__fEbeamUndAngY)
    : _uDamageMask(arg__uDamageMask), _fEbeamCharge(arg__fEbeamCharge), _fEbeamL3Energy(arg__fEbeamL3Energy), _fEbeamLTUPosX(arg__fEbeamLTUPosX), _fEbeamLTUPosY(arg__fEbeamLTUPosY), _fEbeamLTUAngX(arg__fEbeamLTUAngX), _fEbeamLTUAngY(arg__fEbeamLTUAngY), _fEbeamPkCurrBC2(arg__fEbeamPkCurrBC2), _fEbeamEnergyBC2(arg__fEbeamEnergyBC2), _fEbeamPkCurrBC1(arg__fEbeamPkCurrBC1), _fEbeamEnergyBC1(arg__fEbeamEnergyBC1), _fEbeamUndPosX(arg__fEbeamUndPosX), _fEbeamUndPosY(arg__fEbeamUndPosY), _fEbeamUndAngX(arg__fEbeamUndAngX), _fEbeamUndAngY(arg__fEbeamUndAngY)
  {
  }
  BldDataEBeamV4() {}
  /** Damage mask. */
  uint32_t damageMask() const { return _uDamageMask; }
  /** Beam charge in nC. */
  double ebeamCharge() const { return _fEbeamCharge; }
  /** Beam energy in MeV. */
  double ebeamL3Energy() const { return _fEbeamL3Energy; }
  /** LTU beam position (BPMS:LTU1:720 through 750) in mm. */
  double ebeamLTUPosX() const { return _fEbeamLTUPosX; }
  /** LTU beam position in mm. */
  double ebeamLTUPosY() const { return _fEbeamLTUPosY; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngX() const { return _fEbeamLTUAngX; }
  /** LTU beam angle in mrad. */
  double ebeamLTUAngY() const { return _fEbeamLTUAngY; }
  /** Beam current in Amps. */
  double ebeamPkCurrBC2() const { return _fEbeamPkCurrBC2; }
  /** Beam position in mm (related to beam energy). */
  double ebeamEnergyBC2() const { return _fEbeamEnergyBC2; }
  /** Beam current in Amps. */
  double ebeamPkCurrBC1() const { return _fEbeamPkCurrBC1; }
  /** Beam position in mm (related to beam energy). */
  double ebeamEnergyBC1() const { return _fEbeamEnergyBC1; }
  /** Undulator launch feedback (BPMs U4 through U10) beam x-position in mm. */
  double ebeamUndPosX() const { return _fEbeamUndPosX; }
  /** Undulator launch feedback beam y-position in mm. */
  double ebeamUndPosY() const { return _fEbeamUndPosY; }
  /** Undulator launch feedback beam x-angle in mrad. */
  double ebeamUndAngX() const { return _fEbeamUndAngX; }
  /** Undulator launch feedback beam y-angle in mrad. */
  double ebeamUndAngY() const { return _fEbeamUndAngY; }
  static uint32_t _sizeof() { return 116; }
private:
  uint32_t	_uDamageMask;	/**< Damage mask. */
  double	_fEbeamCharge;	/**< Beam charge in nC. */
  double	_fEbeamL3Energy;	/**< Beam energy in MeV. */
  double	_fEbeamLTUPosX;	/**< LTU beam position (BPMS:LTU1:720 through 750) in mm. */
  double	_fEbeamLTUPosY;	/**< LTU beam position in mm. */
  double	_fEbeamLTUAngX;	/**< LTU beam angle in mrad. */
  double	_fEbeamLTUAngY;	/**< LTU beam angle in mrad. */
  double	_fEbeamPkCurrBC2;	/**< Beam current in Amps. */
  double	_fEbeamEnergyBC2;	/**< Beam position in mm (related to beam energy). */
  double	_fEbeamPkCurrBC1;	/**< Beam current in Amps. */
  double	_fEbeamEnergyBC1;	/**< Beam position in mm (related to beam energy). */
  double	_fEbeamUndPosX;	/**< Undulator launch feedback (BPMs U4 through U10) beam x-position in mm. */
  double	_fEbeamUndPosY;	/**< Undulator launch feedback beam y-position in mm. */
  double	_fEbeamUndAngX;	/**< Undulator launch feedback beam x-angle in mrad. */
  double	_fEbeamUndAngY;	/**< Undulator launch feedback beam y-angle in mrad. */
};
std::ostream& operator<<(std::ostream& str, Bld::BldDataEBeamV4::DamageMask enval);
#pragma pack(pop)

/** @class BldDataPhaseCavity

  PV names: UND:R02:IOC:16:BAT:FitTime1, UND:R02:IOC:16:BAT:FitTime2, 
                UND:R02:IOC:16:BAT:Charge1,  UND:R02:IOC:16:BAT:Charge2
*/

#pragma pack(push,4)

class BldDataPhaseCavity {
public:
  enum { TypeId = Pds::TypeId::Id_PhaseCavity /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };
  BldDataPhaseCavity(double arg__fFitTime1, double arg__fFitTime2, double arg__fCharge1, double arg__fCharge2)
    : _fFitTime1(arg__fFitTime1), _fFitTime2(arg__fFitTime2), _fCharge1(arg__fCharge1), _fCharge2(arg__fCharge2)
  {
  }
  BldDataPhaseCavity() {}
  /** UND:R02:IOC:16:BAT:FitTime1 value in pico-seconds. */
  double fitTime1() const { return _fFitTime1; }
  /** UND:R02:IOC:16:BAT:FitTime2 value in pico-seconds. */
  double fitTime2() const { return _fFitTime2; }
  /** UND:R02:IOC:16:BAT:Charge1 value in pico-columbs. */
  double charge1() const { return _fCharge1; }
  /** UND:R02:IOC:16:BAT:Charge2 value in pico-columbs. */
  double charge2() const { return _fCharge2; }
  static uint32_t _sizeof() { return 32; }
private:
  double	_fFitTime1;	/**< UND:R02:IOC:16:BAT:FitTime1 value in pico-seconds. */
  double	_fFitTime2;	/**< UND:R02:IOC:16:BAT:FitTime2 value in pico-seconds. */
  double	_fCharge1;	/**< UND:R02:IOC:16:BAT:Charge1 value in pico-columbs. */
  double	_fCharge2;	/**< UND:R02:IOC:16:BAT:Charge2 value in pico-columbs. */
};
#pragma pack(pop)

/** @class BldDataIpimbV0

  Combined structure which includes Ipimb.DataV1, Ipimb.ConfigV1, and 
            Lusi.IpmFexV1 objects.
*/

#pragma pack(push,4)

class BldDataIpimbV0 {
public:
  enum { TypeId = Pds::TypeId::Id_SharedIpimb /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };
  BldDataIpimbV0() {}
  BldDataIpimbV0(const BldDataIpimbV0& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  BldDataIpimbV0& operator=(const BldDataIpimbV0& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  const Ipimb::DataV1& ipimbData() const { return _ipimbData; }
  const Ipimb::ConfigV1& ipimbConfig() const { return _ipimbConfig; }
  const Lusi::IpmFexV1& ipmFexData() const { return _ipmFexData; }
  static uint32_t _sizeof() { return ((((((0+(Ipimb::DataV1::_sizeof()))+(Ipimb::ConfigV1::_sizeof()))+(Lusi::IpmFexV1::_sizeof()))+4)-1)/4)*4; }
private:
  Ipimb::DataV1	_ipimbData;
  Ipimb::ConfigV1	_ipimbConfig;
  Lusi::IpmFexV1	_ipmFexData;
};
#pragma pack(pop)

/** @class BldDataIpimbV1

  Combined structure which includes Ipimb.DataV2, Ipimb.ConfigV2, and 
            Lusi.IpmFexV1 objects.
*/

#pragma pack(push,4)

class BldDataIpimbV1 {
public:
  enum { TypeId = Pds::TypeId::Id_SharedIpimb /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  BldDataIpimbV1() {}
  BldDataIpimbV1(const BldDataIpimbV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  BldDataIpimbV1& operator=(const BldDataIpimbV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  const Ipimb::DataV2& ipimbData() const { return _ipimbData; }
  const Ipimb::ConfigV2& ipimbConfig() const { return _ipimbConfig; }
  const Lusi::IpmFexV1& ipmFexData() const { return _ipmFexData; }
  static uint32_t _sizeof() { return ((((((0+(Ipimb::DataV2::_sizeof()))+(Ipimb::ConfigV2::_sizeof()))+(Lusi::IpmFexV1::_sizeof()))+4)-1)/4)*4; }
private:
  Ipimb::DataV2	_ipimbData;
  Ipimb::ConfigV2	_ipimbConfig;
  Lusi::IpmFexV1	_ipmFexData;
};
#pragma pack(pop)

/** @class BldDataPimV1

  Combined structure which includes Pulnix.TM6740ConfigV2, Lusi.PimImageConfigV1, and 
            Camera.FrameV1 objects.
*/

#pragma pack(push,4)

class BldDataPimV1 {
public:
  enum { TypeId = Pds::TypeId::Id_SharedPim /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  BldDataPimV1() {}
  BldDataPimV1(const BldDataPimV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  BldDataPimV1& operator=(const BldDataPimV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  const Pulnix::TM6740ConfigV2& camConfig() const { return _camConfig; }
  const Lusi::PimImageConfigV1& pimConfig() const { return _pimConfig; }
  const Camera::FrameV1& frame() const { return _frame; }
  uint32_t _sizeof() const { return ((((((0+(Pulnix::TM6740ConfigV2::_sizeof()))+(Lusi::PimImageConfigV1::_sizeof()))+(this->_frame._sizeof()))+4)-1)/4)*4; }
private:
  Pulnix::TM6740ConfigV2	_camConfig;
  Lusi::PimImageConfigV1	_pimConfig;
  Camera::FrameV1	_frame;
};
#pragma pack(pop)

/** @class BldDataGMDV0

  Gas Monitor Detector data.
*/

#pragma pack(push,4)

class BldDataGMDV0 {
public:
  enum { TypeId = Pds::TypeId::Id_GMD /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };
  BldDataGMDV0() {}
  BldDataGMDV0(const BldDataGMDV0& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  BldDataGMDV0& operator=(const BldDataGMDV0& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  /** String describing gas type */
  const char* gasType() const { return _strGasType; }
  /** Pressure from Spinning Rotor Gauge */
  double pressure() const { return _fPressure; }
  /** Temp from PT100 */
  double temperature() const { return _fTemperature; }
  /** Current from Keithley Electrometer */
  double current() const { return _fCurrent; }
  /** HV Mesh Electron */
  double hvMeshElectron() const { return _fHvMeshElectron; }
  /** HV Mesh Ion */
  double hvMeshIon() const { return _fHvMeshIon; }
  /** HV Mult Ion */
  double hvMultIon() const { return _fHvMultIon; }
  /** Charge Q */
  double chargeQ() const { return _fChargeQ; }
  /** Photon Energy */
  double photonEnergy() const { return _fPhotonEnergy; }
  /** Pulse Intensity derived from Electron Multiplier */
  double multPulseIntensity() const { return _fMultPulseIntensity; }
  /** Pulse Intensity derived from ION cup current */
  double keithleyPulseIntensity() const { return _fKeithleyPulseIntensity; }
  /** Pulse Energy derived from Electron Multiplier */
  double pulseEnergy() const { return _fPulseEnergy; }
  /** Pulse Energy from FEE Gas Detector */
  double pulseEnergyFEE() const { return _fPulseEnergyFEE; }
  /** Transmission derived from Electron Multiplier */
  double transmission() const { return _fTransmission; }
  /** Transmission from FEE Gas Detector */
  double transmissionFEE() const { return _fTransmissionFEE; }
  static uint32_t _sizeof() { return (((((((((((((((((((0+(1*(32)))+8)+8)+8)+8)+8)+8)+8)+8)+8)+8)+8)+8)+8)+8)+8)+4)-1)/4)*4; }
private:
  char	_strGasType[32];	/**< String describing gas type */
  double	_fPressure;	/**< Pressure from Spinning Rotor Gauge */
  double	_fTemperature;	/**< Temp from PT100 */
  double	_fCurrent;	/**< Current from Keithley Electrometer */
  double	_fHvMeshElectron;	/**< HV Mesh Electron */
  double	_fHvMeshIon;	/**< HV Mesh Ion */
  double	_fHvMultIon;	/**< HV Mult Ion */
  double	_fChargeQ;	/**< Charge Q */
  double	_fPhotonEnergy;	/**< Photon Energy */
  double	_fMultPulseIntensity;	/**< Pulse Intensity derived from Electron Multiplier */
  double	_fKeithleyPulseIntensity;	/**< Pulse Intensity derived from ION cup current */
  double	_fPulseEnergy;	/**< Pulse Energy derived from Electron Multiplier */
  double	_fPulseEnergyFEE;	/**< Pulse Energy from FEE Gas Detector */
  double	_fTransmission;	/**< Transmission derived from Electron Multiplier */
  double	_fTransmissionFEE;	/**< Transmission from FEE Gas Detector */
  double	_fSpare6;
};
#pragma pack(pop)

/** @class BldDataGMDV1

  Gas Monitor Detector data.
*/

#pragma pack(push,4)

class BldDataGMDV1 {
public:
  enum { TypeId = Pds::TypeId::Id_GMD /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  BldDataGMDV1() {}
  BldDataGMDV1(const BldDataGMDV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  BldDataGMDV1& operator=(const BldDataGMDV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  /** Shot to shot pulse energy (mJ) */
  double milliJoulesPerPulse() const { return _fMilliJoulesPerPulse; }
  /** Average pulse energy from ION cup current (mJ) */
  double milliJoulesAverage() const { return _fMilliJoulesAverage; }
  /** Bg corrected waveform integrated within limits in raw A/D counts */
  double correctedSumPerPulse() const { return _fCorrectedSumPerPulse; }
  /** Avg background value per sample in raw A/D counts */
  double bgValuePerSample() const { return _fBgValuePerSample; }
  /** Shot by shot pulse energy in arbitrary units */
  double relativeEnergyPerPulse() const { return _fRelativeEnergyPerPulse; }
  static uint32_t _sizeof() { return 48; }
private:
  double	_fMilliJoulesPerPulse;	/**< Shot to shot pulse energy (mJ) */
  double	_fMilliJoulesAverage;	/**< Average pulse energy from ION cup current (mJ) */
  double	_fCorrectedSumPerPulse;	/**< Bg corrected waveform integrated within limits in raw A/D counts */
  double	_fBgValuePerSample;	/**< Avg background value per sample in raw A/D counts */
  double	_fRelativeEnergyPerPulse;	/**< Shot by shot pulse energy in arbitrary units */
  double	_fSpare1;
};
#pragma pack(pop)

/** @class BldDataAcqADCV1

  Combined structure which includes Acqiris.ConfigV1 and 
            Acqiris.DataDescV1 objects.
*/

#pragma pack(push,4)

class BldDataAcqADCV1 {
public:
  enum { TypeId = Pds::TypeId::Id_SharedAcqADC /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  BldDataAcqADCV1() {}
private:
  BldDataAcqADCV1(const BldDataAcqADCV1&);
  BldDataAcqADCV1& operator=(const BldDataAcqADCV1&);
public:
  const Acqiris::ConfigV1& config() const { return _config; }
  const Acqiris::DataDescV1& data() const { return _data; }
private:
  Acqiris::ConfigV1	_config;
  Acqiris::DataDescV1	_data;
};
#pragma pack(pop)

/** @class BldDataSpectrometerV0

  Structure which contains image projections for spectrometers.
*/

#pragma pack(push,4)

class BldDataSpectrometerV0 {
public:
  enum { TypeId = Pds::TypeId::Id_Spectrometer /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 0 /**< XTC type version number */ };
  BldDataSpectrometerV0(const uint32_t* arg__hproj, const uint32_t* arg__vproj)
  {
    if (arg__hproj) std::copy(arg__hproj, arg__hproj+(1024), &_hproj[0]);
    if (arg__vproj) std::copy(arg__vproj, arg__vproj+(256), &_vproj[0]);
  }
  BldDataSpectrometerV0() {}
  BldDataSpectrometerV0(const BldDataSpectrometerV0& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  BldDataSpectrometerV0& operator=(const BldDataSpectrometerV0& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 1> hproj(const boost::shared_ptr<T>& owner) const { 
    const uint32_t* data = &_hproj[0];
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), 1024);
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 1> hproj() const { return make_ndarray(&_hproj[0], 1024); }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 1> vproj(const boost::shared_ptr<T>& owner) const { 
    const uint32_t* data = &_vproj[0];
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), 256);
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 1> vproj() const { return make_ndarray(&_vproj[0], 256); }
  static uint32_t _sizeof() { return (((((0+(4*(1024)))+(4*(256)))+4)-1)/4)*4; }
private:
  uint32_t	_hproj[1024];
  uint32_t	_vproj[256];
};
#pragma pack(pop)
} // namespace Bld
} // namespace Pds
#endif // PDS_BLD_DDL_H
