#ifndef PDS_GENERICPGP_DDL_H
#define PDS_GENERICPGP_DDL_H 1

// *** Do not edit this file, it is auto-generated ***

#include <vector>
#include <iosfwd>
#include <cstddef>
#include <cstring>
#include "pdsdata/xtc/TypeId.hh"
#include "ndarray/ndarray.h"
namespace Pds {
namespace GenericPgp {

/** @class CDimension

  
*/

#pragma pack(push,4)

class CDimension {
public:
  CDimension(uint32_t arg__rows, uint32_t arg__columns)
    : _rows(arg__rows), _columns(arg__columns)
  {
  }
  CDimension() {}
  CDimension(const CDimension& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  CDimension& operator=(const CDimension& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  uint32_t rows() const { return _rows; }
  uint32_t columns() const { return _columns; }
  static uint32_t _sizeof() { return 8; }
private:
  uint32_t	_rows;
  uint32_t	_columns;
};
#pragma pack(pop)

/** @class CRegister

  
*/

#pragma pack(push,4)

class CRegister {
public:
  enum Action {
    RegisterRead = 0, /**< Read and store */
    RegisterWrite = 1, /**< Write */
    RegisterWriteA = 2, /**< Write and wait for ack */
    RegisterVerify = 3, /**< Read and verify */
    Spin = 4, /**< Spin lock */
    Usleep = 5, /**< Sleep */
    Flush = 6, /**< Flush pending reads */
  };
  CRegister(GenericPgp::CRegister::Action arg__action, uint32_t arg__datasize, uint32_t arg__address, uint32_t arg__offset, uint32_t arg__mask)
    : _Action(((arg__action) & 0xff)|(((arg__datasize) & 0xffffff)<<8)), _address(arg__address), _offset(arg__offset), _mask(arg__mask)
  {
  }
  CRegister() {}
  /** Configuration action */
  GenericPgp::CRegister::Action action() const { return Action(this->_Action & 0xff); }
  /** Size of register access (in uint32_t's) */
  uint32_t datasize() const { return uint32_t((this->_Action>>8) & 0xffffff); }
  /** Register access address */
  uint32_t address() const { return _address; }
  /** Payload offset */
  uint32_t offset() const { return _offset; }
  /** Register value mask */
  uint32_t mask() const { return _mask; }
  static uint32_t _sizeof() { return 16; }
private:
  uint32_t	_Action;
  uint32_t	_address;	/**< Register access address */
  uint32_t	_offset;	/**< Payload offset */
  uint32_t	_mask;	/**< Register value mask */
};
std::ostream& operator<<(std::ostream& str, GenericPgp::CRegister::Action enval);
#pragma pack(pop)

/** @class CStream

  
*/

#pragma pack(push,4)

class CStream {
public:
  CStream(uint32_t arg__pgp_channel, uint32_t arg__data_type, uint32_t arg__config_type, uint32_t arg__config_offset)
    : _pgp_channel(arg__pgp_channel), _data_type(arg__data_type), _config_type(arg__config_type), _config_offset(arg__config_offset)
  {
  }
  CStream() {}
  /** PGP virtual channel ID */
  uint32_t pgp_channel() const { return _pgp_channel; }
  /** Event data type ID */
  uint32_t data_type() const { return _data_type; }
  /** Configuration data type ID */
  uint32_t config_type() const { return _config_type; }
  /** Location of configuration data */
  uint32_t config_offset() const { return _config_offset; }
  static uint32_t _sizeof() { return 16; }
private:
  uint32_t	_pgp_channel;	/**< PGP virtual channel ID */
  uint32_t	_data_type;	/**< Event data type ID */
  uint32_t	_config_type;	/**< Configuration data type ID */
  uint32_t	_config_offset;	/**< Location of configuration data */
};
#pragma pack(pop)

/** @class ConfigV1

  
*/

#pragma pack(push,4)

class ConfigV1 {
public:
  enum { TypeId = Pds::TypeId::Id_GenericPgpConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 1 /**< XTC type version number */ };
  ConfigV1(uint32_t arg__id, const GenericPgp::CDimension& arg__frame_dim, const GenericPgp::CDimension& arg__aux_dim, const GenericPgp::CDimension& arg__env_dim, uint32_t arg__number_of_registers, uint32_t arg__number_of_sequences, uint32_t arg__number_of_streams, uint32_t arg__payload_size, const uint32_t* arg__pixel_settings, const uint32_t* arg__sequence_length, const GenericPgp::CRegister* arg__sequence, const GenericPgp::CStream* arg__stream, const uint32_t* arg__payload);
  ConfigV1(const GenericPgp::CDimension& arg__frame_dim, uint32_t arg__number_of_registers, uint32_t arg__number_of_sequences, uint32_t arg__number_of_streams, uint32_t arg__payload_size)
    : _frame_dim(arg__frame_dim), _number_of_registers(arg__number_of_registers), _number_of_sequences(arg__number_of_sequences), _number_of_streams(arg__number_of_streams), _payload_size(arg__payload_size)
  {
  }
  ConfigV1() {}
  ConfigV1(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  ConfigV1& operator=(const ConfigV1& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  /** Serial number identifying the array */
  uint32_t id() const { return _id; }
  /** Dimensions of the frame data from the array */
  const GenericPgp::CDimension& frame_dim() const { return _frame_dim; }
  /** Dimensions of the auxillary data from the array */
  const GenericPgp::CDimension& aux_dim() const { return _aux_dim; }
  /** Dimensions of the environmental data from the array */
  const GenericPgp::CDimension& env_dim() const { return _env_dim; }
  /** Number of registers in the sequence array */
  uint32_t number_of_registers() const { return _number_of_registers; }
  /** Number of (sub)sequences of register operations in the array */
  uint32_t number_of_sequences() const { return _number_of_sequences; }
  uint32_t number_of_streams() const { return _number_of_streams; }
  uint32_t payload_size() const { return _payload_size; }
  /**     Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 2> pixel_settings(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=44;
    const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), this->frame_dim().rows(), this->frame_dim().columns());
  }
  /**     Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 2> pixel_settings() const { ptrdiff_t offset=44;
  const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
  return make_ndarray(data, this->frame_dim().rows(), this->frame_dim().columns()); }
  /** Lengths of (sub)sequence register operations in the array

    Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 1> sequence_length(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=44+(4*((this->frame_dim().rows())*(this->frame_dim().columns())));
    const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), this->number_of_sequences());
  }
  /** Lengths of (sub)sequence register operations in the array

    Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 1> sequence_length() const { ptrdiff_t offset=44+(4*((this->frame_dim().rows())*(this->frame_dim().columns())));
  const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
  return make_ndarray(data, this->number_of_sequences()); }
  /** Register Operations

    Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const GenericPgp::CRegister, 1> sequence(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=(44+(4*((this->frame_dim().rows())*(this->frame_dim().columns()))))+(4*(this->number_of_sequences()));
    const GenericPgp::CRegister* data = (const GenericPgp::CRegister*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const GenericPgp::CRegister>(owner, data), this->number_of_registers());
  }
  /** Register Operations

    Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const GenericPgp::CRegister, 1> sequence() const { ptrdiff_t offset=(44+(4*((this->frame_dim().rows())*(this->frame_dim().columns()))))+(4*(this->number_of_sequences()));
  const GenericPgp::CRegister* data = (const GenericPgp::CRegister*)(((char*)this)+offset);
  return make_ndarray(data, this->number_of_registers()); }
  /** Stream readout configuration

    Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const GenericPgp::CStream, 1> stream(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=((44+(4*((this->frame_dim().rows())*(this->frame_dim().columns()))))+(4*(this->number_of_sequences())))+(16*(this->number_of_registers()));
    const GenericPgp::CStream* data = (const GenericPgp::CStream*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const GenericPgp::CStream>(owner, data), this->number_of_streams());
  }
  /** Stream readout configuration

    Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const GenericPgp::CStream, 1> stream() const { ptrdiff_t offset=((44+(4*((this->frame_dim().rows())*(this->frame_dim().columns()))))+(4*(this->number_of_sequences())))+(16*(this->number_of_registers()));
  const GenericPgp::CStream* data = (const GenericPgp::CStream*)(((char*)this)+offset);
  return make_ndarray(data, this->number_of_streams()); }
  /** Stream and Register Data

    Note: this overloaded method accepts shared pointer argument which must point to an object containing
    this instance, the returned ndarray object can be used even after this instance disappears. */
  template <typename T>
  ndarray<const uint32_t, 1> payload(const boost::shared_ptr<T>& owner) const { 
    ptrdiff_t offset=(((44+(4*((this->frame_dim().rows())*(this->frame_dim().columns()))))+(4*(this->number_of_sequences())))+(16*(this->number_of_registers())))+(16*(this->number_of_streams()));
    const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
    return make_ndarray(boost::shared_ptr<const uint32_t>(owner, data), this->payload_size());
  }
  /** Stream and Register Data

    Note: this method returns ndarray instance which does not control lifetime
    of the data, do not use returned ndarray after this instance disappears. */
  ndarray<const uint32_t, 1> payload() const { ptrdiff_t offset=(((44+(4*((this->frame_dim().rows())*(this->frame_dim().columns()))))+(4*(this->number_of_sequences())))+(16*(this->number_of_registers())))+(16*(this->number_of_streams()));
  const uint32_t* data = (const uint32_t*)(((char*)this)+offset);
  return make_ndarray(data, this->payload_size()); }
  /** Number of rows in a readout unit */
  uint32_t numberOfRows() const { return this->frame_dim().rows(); }
  /** Number of columns in a readout unit */
  uint32_t numberOfColumns() const { return this->frame_dim().columns(); }
  /** Number of rows in the auxillary data */
  uint32_t lastRowExclusions() const { return this->aux_dim().rows(); }
  /** Number of elements in environmental data */
  uint32_t numberOfAsics() const { return this->env_dim().columns(); }
  uint32_t _sizeof() const { return (((((((((((((((4+(GenericPgp::CDimension::_sizeof()))+(GenericPgp::CDimension::_sizeof()))+(GenericPgp::CDimension::_sizeof()))+4)+4)+4)+4)+(4*(this->frame_dim().rows())*(this->frame_dim().columns())))+(4*(this->number_of_sequences())))+(GenericPgp::CRegister::_sizeof()*(this->number_of_registers())))+(GenericPgp::CStream::_sizeof()*(this->number_of_streams())))+(4*(this->payload_size())))+4)-1)/4)*4; }
private:
  uint32_t	_id;	/**< Serial number identifying the array */
  GenericPgp::CDimension	_frame_dim;	/**< Dimensions of the frame data from the array */
  GenericPgp::CDimension	_aux_dim;	/**< Dimensions of the auxillary data from the array */
  GenericPgp::CDimension	_env_dim;	/**< Dimensions of the environmental data from the array */
  uint32_t	_number_of_registers;	/**< Number of registers in the sequence array */
  uint32_t	_number_of_sequences;	/**< Number of (sub)sequences of register operations in the array */
  uint32_t	_number_of_streams;
  uint32_t	_payload_size;
  //uint32_t	_pixel_settings[this->frame_dim().rows()][this->frame_dim().columns()];
  //uint32_t	_sequence_length[this->number_of_sequences()];
  //GenericPgp::CRegister	_sequence[this->number_of_registers()];
  //GenericPgp::CStream	_stream[this->number_of_streams()];
  //uint32_t	_payload[this->payload_size()];
};
#pragma pack(pop)
} // namespace GenericPgp
} // namespace Pds
#endif // PDS_GENERICPGP_DDL_H
