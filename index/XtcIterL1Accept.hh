#ifndef Pds_Index_XtcIterL1Accept_hh
#define Pds_Index_XtcIterL1Accept_hh

#include "pdsdata/xtc/XtcIterator.hh"
#include "pdsdata/index/IndexList.hh"

namespace Pds
{  
namespace Index
{
  
class XtcIterL1Accept: public XtcIterator
{
public:
  enum
  { Stop, Continue };
  
  XtcIterL1Accept(Xtc * xtc, unsigned depth, long long int lliOffset, 
    IndexList& indexList ) :
    XtcIterator(xtc), _depth(depth), _lliOffset(lliOffset), 
    _pIndexList(&indexList)
  {
  }

  int  process(Xtc * xtc);
  
private:
  unsigned            _depth;
  long long int       _lliOffset;
  IndexList*          _pIndexList;
};

} // namespace Index
} // namespace Pds

#endif // #ifndef Pds_Index_XtcIterL1Accept_hh
