
// -*- C++ -*-
//
// $Id: MPower.cpp,v 1.2 2002/10/13 14:22:47 jpc Exp $
//
// /-----------------------------------------------------------------\ 
// |                                                                 |
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./UPower.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// \-----------------------------------------------------------------/




# include  "MDefs.h"




// -------------------------------------------------------------------
// MBK namespace.

namespace MBK {


//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Constructor  :  "CPower::CPower ()".

CPower::CPower (void)
{
  type = 0;
}




// -------------------------------------------------------------------
// Function  (friend "CPower")  :  "operator<< ()".

ostream &operator<< (ostream &o, CPower &self)
{
  LInter *inter;


  if (self.type == C_POWER_VDD) o << "vdd ";
  else                          o << "vss ";

  inter = static_cast<LInter *>(&self);

  return (o << *inter);
}




// -------------------------------------------------------------------
// Constructor  :  "CPowers::CPowers ()".

CPowers::CPowers ( CFig *fig
                 , char  atype
                 , int   alayer
                 , long  awidth
                 ) throw (except_done)
{
  LPower::iterator  itLine, beginLine, endLine;
  phins_list       *ins;
  phfig_list       *model;
  phseg_list       *seg, flatSeg;
  string            mess1,  mess2;
  char              ORIENT1, ORIENT2;
  char              segType;
  long              lbound, rbound, key;


  type  = atype;
  width = awidth;
  layer = layer2CALU (alayer);

  switch (type) {
    default:
    case C_HORIZONTAL:
      mess1   = "horizontal";
      mess2   = "EAST/WEST";
      ORIENT1 = EAST;
      ORIENT2 = WEST;

      AB1 = fig->XAB1 ();
      AB2 = fig->XAB2 ();
      break;
    case C_VERTICAL:
      mess1   = "vertical";
      mess2   = "NORTH/SOUTH";
      ORIENT1 = NORTH;
      ORIENT2 = SOUTH;

      AB1 = fig->YAB1 ();
      AB2 = fig->YAB2 ();
      break;
  }


  // Loop over all the instances.
  for (ins = fig->phfig.fig->PHINS; ins != NULL; ins = ins->NEXT) {
    model = getphfig (ins->FIGNAME, 'A');

    // Find the power segments (CALUx).
    for (seg = model->PHSEG; seg != NULL; seg = seg->NEXT) {
      
      // Skip no power segments.
      if (!(cmpALU (alayer, seg->LAYER) & F_CALU)) continue;
      segType = C_POWER_NONE;
      if (ISVDD (seg->NAME)) segType = C_POWER_VDD;
      if (ISVSS (seg->NAME)) segType = C_POWER_VSS;
      if (segType == C_POWER_NONE) continue;

      xyflat ( &(flatSeg.X1), &(flatSeg.Y1)
             ,       seg->X1, seg->Y1
             ,     ins->XINS, ins->YINS
             ,   model->XAB1, model->YAB1
             ,   model->XAB2, model->YAB2
             ,   ins->TRANSF
             );

      xyflat ( &(flatSeg.X2), &(flatSeg.Y2)
             ,       seg->X2, seg->Y2
             ,     ins->XINS, ins->YINS
             ,   model->XAB1, model->YAB1
             ,   model->XAB2, model->YAB2
             ,   ins->TRANSF
             );

      // Check the segment width.
      if (seg->WIDTH != width) {
        cerr << hwarn ("");
        cerr << "  " << layer2a (layer) << " \"" << seg->NAME
             <<"\" segment at ("
             << UNSCALE (seg->X1) << ","
             << UNSCALE (seg->Y1) << ") doesn't have the rigth witdth :"
             << UNSCALE (seg->WIDTH) << " instead of "
             << UNSCALE (width) << ".\n";
        cerr << "  (instance \"" << ins->INSNAME << "\" of model \""
             << model->NAME << "\")\n";
        
        continue;
      }

      // Check the segment direction & position.
      switch (type) {
        default:
        case C_HORIZONTAL:
          lbound = flatSeg.X1;
          rbound = flatSeg.X2;
          key = flatSeg.Y1;

          if (flatSeg.Y1 != flatSeg.Y2) {
            cerr << hwarn ("");
            cerr << "  " << layer2a (layer) << " \"" << seg->NAME
                 <<"\" segment at ("
                 << UNSCALE (seg->X1) << ","
                 << UNSCALE (seg->Y1) << ") is not " << mess1;
            cerr << "  (instance \"" << ins->INSNAME << "\" of model \""
                 << model->NAME << "\")\n";

            continue;
          }

          if (   (cmpALU (alayer, CALU1) & F_CALU)
              && !fig->phfig.onslice (flatSeg.Y1)) {
            cerr << hwarn ("");
            cerr << "  " << layer2a (layer) << " \"" << seg->NAME
                 <<"\" segment at ("
                 << UNSCALE (seg->X1) << ","
                 << UNSCALE (seg->Y1) << ") is not on a slice boundary.\n";
            cerr << "  (instance \"" << ins->INSNAME << "\" of model \""
                 << model->NAME << "\")\n";
            cerr << "  (valide slices boundaries are"
                 << " ((n * " << D::Y_SLICE << ") - " << D::WIDTH_VSS / 2 << ") or"
                 << " ((n * " << D::Y_SLICE << ") + " << D::WIDTH_VSS / 2 << ") )\n";
            
            continue;
          }
          break;

        case C_VERTICAL:
          lbound = flatSeg.Y1;
          rbound = flatSeg.Y2;
          key = flatSeg.X1;

          if (flatSeg.X1 != flatSeg.X2) {
            cerr << hwarn ("");
            cerr << "  " << layer2a (layer) << " \"" << seg->NAME
                 <<"\" segment at ("
                 << UNSCALE (seg->X1) << ","
                 << UNSCALE (seg->Y1) << ") is not " << mess1;
            cerr << "  (instance \"" << ins->INSNAME << "\" of model \""
                 << model->NAME << "\")\n";

            continue;
          }
          break;
      }
      
      
      beginLine = powerLines.begin ();
        endLine = powerLines.end ();


      // Check if the power line is of the same type.
      // (no short circuits between VDD & VSS.
      itLine = powerLines.find (key);
      if (itLine != endLine) {
        if (itLine->second.type != segType) {
          cerr << herr ("");
          cerr << "  " << layer2a (layer) << " \"" << seg->NAME
               <<"\" segment at ("
               << UNSCALE (seg->X1) << ","
               << UNSCALE (seg->Y1) << ") conflict with power line at"
               << UNSCALE (key) << ".\n";

          throw except_done ();
        }
      }


      // Merge the segment with the power line (at long last...).
      powerLines[key].add (lbound, rbound);
      powerLines[key].type = segType;
    }
  }
}




// -------------------------------------------------------------------
// Method  :  "CPowers::dump ()".

void  CPowers::dump (CFig *fig)
{
  LPower::iterator  itLine, beginLine, endLine;
  LInter::iterator  iInter, beginInter, endInter;
  phseg_list        seg;
  phcon_list        con1, con2;


  seg.LAYER  = layer;
  seg.WIDTH  = width;
  con1.LAYER = MBK::layer2ALU(layer);
  con1.WIDTH = width;
  con2.LAYER = MBK::layer2ALU(layer);
  con2.WIDTH = width;

  beginLine = powerLines.begin ();
  endLine   = powerLines.end ();

  // Transform power lines into CALUx segments.
  for (itLine = beginLine; itLine != endLine; itLine++) {
    beginInter = itLine->second.begin ();
    endInter   = itLine->second.end ();

    for (iInter = beginInter; iInter != endInter; iInter++) {
      switch (itLine->second.type) {
        default:
        case C_POWER_VDD: seg.NAME = (char*)"vdd"; break;
        case C_POWER_VSS: seg.NAME = (char*)"vss"; break;
      }
      con1.NAME   = seg.NAME;
      con1.ORIENT = (char)0;
      con2.NAME   = seg.NAME;
      con2.ORIENT = (char)0;

      switch (this->type) {
        default:
        case C_HORIZONTAL:
          seg.X1 = iInter->first;
          seg.X2 = iInter->second;
          seg.Y1 = itLine->first;
          seg.Y2 = itLine->first;

          // Add power terminals at end of power lines.
          if (iInter->first == AB1) {
            con1.XCON   = AB1;
            con1.YCON   = itLine->first;
            con1.ORIENT = WEST;
          }

          if (iInter->second == AB2) {
            con2.XCON   = AB2;
            con2.YCON   = itLine->first;
            con2.ORIENT = EAST;
          }

          break;
        case C_VERTICAL:
          seg.X1 = itLine->first;
          seg.X2 = itLine->first;
          seg.Y1 = iInter->first;
          seg.Y2 = iInter->second;
          break;

          // Add power terminals at end of power lines.
          if (iInter->first == AB1) {
            con1.XCON   = itLine->first;
            con1.YCON   = AB1;
            con1.ORIENT = SOUTH;
          }

          if (iInter->second == AB2) {
            con2.XCON   = itLine->first;
            con2.YCON   = AB2;
            con2.ORIENT = NORTH;
          }
      }

      fig->addphseg (seg);

      if (con1.ORIENT != (char)0) fig->addphcon (con1);
      if (con2.ORIENT != (char)0) fig->addphcon (con2);
    }
  }

  cdebug << *this;
}




// -------------------------------------------------------------------
// Friend of "CPowers"  :  "&operator<<()".

ostream &operator<< (ostream &o, CPowers &self)
{
  LPower::iterator  itLine, beginLine, endLine;


  o << "+ CPowers:\n";
  o << "+   ";

  if (self.type == C_HORIZONTAL) o << "horizontal";
  else                           o << "vertical  ";

  o << " " << layer2a (self.layer) << " " << UNSCALE (self.width);
  o << " [" << UNSCALE (self.AB1)
    <<  "," << UNSCALE (self.AB2) << "]\n";

  beginLine = self.powerLines.begin ();
  endLine   = self.powerLines.end ();

  for (itLine = beginLine; itLine != endLine; itLine++) {
    o << "+  " << itLine->first << " " << itLine->second << "\n";
  }

  return (o);
}




// -------------------------------------------------------------------
// End of MBK namespace.

}
