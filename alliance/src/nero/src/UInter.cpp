
// -*- C++ -*-
//
// $Id: UInter.cpp,v 1.1 2002/10/02 21:23:48 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./UInter.cpp"                            |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "UDefs.h"




// -------------------------------------------------------------------
// Local Namespace (i.e. internal functions).

namespace {


  // ---------------------------------------------------------------
  // Local functions.

  static long  inbounds (long lbound, long rbound, long value);
  inline long  INBOUNDS (CInter &i, long v)
                 { return (inbounds (i.first, i.second, v)); }

  //  /------------------------------------------------------------\
  //  |                   Functions Definitions                    |
  //  \------------------------------------------------------------/


// -------------------------------------------------------------------
// Function  :  "inbounds()".

static long  inbounds (long lbound, long rbound, long value)
{
  if ((value >= lbound) && (value <= rbound)) return (F_MIN_IN);
  if  (value <  lbound)                       return (F_MIN_LOWER);

  return (F_MIN_GREATER);
}




// -------------------------------------------------------------------
// End of Local Namespace.

}




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Function (friend CInter)  :  "operator<<()".

ostream &operator<< (ostream &o, const CInter &self)
{
  return (o << "[" << self.first << "," << self.second << "]");
}




// -------------------------------------------------------------------
// Method  :  "CInter::cmp()".

long  CInter::cmp(CInter &b)
{
  long  state;

  state  = INBOUNDS(*this, b.first );
  state |= INBOUNDS(*this, b.second) << SHIFT_MAX;

  return (state);
}




// -------------------------------------------------------------------
// Friend of "LInter"  :  "&operator<<()".

ostream &operator<< (ostream &o, LInter &self)
{
  LInter::iterator  itInter, beginInter, endInter;


  beginInter = self.begin ();
  endInter   = self.end ();

  o << "(";
  for (itInter = beginInter; itInter != endInter; itInter++) {
    if (itInter != beginInter) o << " ";
    
    o << *itInter;
  }
  o << ")";

  return (o);
}




// -------------------------------------------------------------------
// Method  :  "LInter::add()".

void  LInter::add (long lbound, long rbound)
{
  LInter::iterator  itInter, beginInter, endInter, mergeInter;
  CInter            element;


  beginInter = begin ();
    endInter = end ();
  mergeInter = endInter;

  element.first  = min (lbound, rbound);
  element.second = max (lbound, rbound);


  // The list is empty : just add the new element.
  if (beginInter == endInter) {
    bmin = lbound;
    bmax = rbound;
    insert (beginInter, element);

    return;
  }


  // Update the whole list bounds.
  bmin = min (bmin, lbound);
  bmax = max (bmax, rbound);


  // Swept through the interval list for add or merge.
  for (itInter = beginInter; itInter != endInter;) {
    switch (element.cmp (*itInter)) {
      // "itInter" is strictly lower than "element". Let's see the next
      // interval.
      case C_INTER_LOWER: break;
      case C_INTER_OUT:   break;

      // The upper bound of "itInter" is inside "element" :
      // Update the upper bound of "itInter". "element" is now
      // merged with the list.
      case C_INTER_CROSS_MIN:
        itInter->second = element.second;
        mergeInter      = itInter;
        break;

      // "itInter" is completly inside "element".
      // - If "element" has not been merged yet (mergeInter == endInter),
      //     resize "itInter" to fit element. "element" merged.
      // - "element" is already merged : delete "itInter".
      //      Immediatly process the next element.
      case C_INTER_IN:
        if (mergeInter == endInter) {
          itInter->first  = element.first;
          itInter->second = element.second;
          mergeInter      = itInter;
        } else {
          itInter = erase (itInter);
          continue;
        }
        break;
 
      // The lower bound of "itInter" is inside "element" :
      // - If "element" has not been merged yet (mergeInter == endInter),
      //     Update the lower bound of "itInter". "element" merged.
      // - "element" is aleady merged : we have to update the upper
      //     bounds of "mergeInter" & "element" as it's used as the
      //     reference element for the next comparisons. Then delete
      //     "itInter".
      case C_INTER_CROSS_MAX:
        if (mergeInter == endInter) {
          itInter->first = element.first;
          mergeInter     = itInter;
        } else {
          mergeInter->second = itInter->second;
          element.second     = itInter->second;

          itInter = erase (itInter);
          continue;
        }
        break;

      // "itInter" is strictly greater than "element" (and element
      // is not included in any lower interval : add a new element
      // before "itInter" then break the loop.
      case C_INTER_GREATER:
        if (mergeInter == endInter) {
          itInter    = insert (itInter, element);
          mergeInter = itInter;
          itInter    = endInter;
          continue;
        }
        break;
    }

    // Proceed the next "itInter".
    itInter++;
  }


  // If the element is not merged here i's greater than any element
  // in the list. Add it in tail.
  if (mergeInter == endInter) push_back (element);
}
