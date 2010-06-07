#include "pdsdata/encoder/DataV1.hh"

int Pds::Encoder::DataV1::value() const
{
  int v(_encoder_count<<8);
  return v>>8;
}
