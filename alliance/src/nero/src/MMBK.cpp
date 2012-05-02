
// -*- C++ -*-
//
// $Id: MMBK.cpp,v 1.10 2012/05/02 14:49:23 jpc Exp $
//
// /-----------------------------------------------------------------\ 
// |                                                                 |
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./UMBK.cpp"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// \-----------------------------------------------------------------/




# include  "MDefs.h"




// -------------------------------------------------------------------
// MBK namespace.

namespace MBK {


  // ---------------------------------------------------------------
  // Pre-defined objects.

  // The creation of this object loads the MBK environment.
  CEnv  env;




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Constructor  :  "CXRect::CXRect()".

CXRect::CXRect ( CDRGrid* agrid ) : drgrid(agrid)
{ }




// -------------------------------------------------------------------
// Method  :  "CXRect::setSeg()".

void  CXRect::setSeg (phseg_list &rSeg)
{
  seg.NAME  = rSeg.NAME;
  seg.LAYER = rSeg.LAYER;
  seg.WIDTH = rSeg.WIDTH;
  seg.X1    = rSeg.X1;
  seg.Y1    = rSeg.Y1;
  seg.X2    = rSeg.X2;
  seg.Y2    = rSeg.Y2;
  seg.TYPE  = rSeg.TYPE;

  seg2rect ();
  rect2grid ();
}




// -------------------------------------------------------------------
// Method  :  "CXRect::seg2rect()".

void  CXRect::seg2rect (void)
{
  long  width   = env.layer2width   (seg.LAYER);
  long  spacing = env.layer2spacing (seg.LAYER);
  long  expand;

  // Special case of "point like" segments.
  if ((seg.X1 == seg.X2) && (seg.Y1 == seg.Y2)) {
    rect.x1 = seg.X1;
    rect.y1 = seg.Y1;
    rect.x2 = seg.X2;
    rect.y2 = seg.Y2;

    return;
  }

  switch (seg.TYPE) {
    case UP:
    case DOWN:
      //rect.x1 = seg.X1 - (seg.WIDTH - width) / 2;
      //rect.x2 = seg.X2 + (seg.WIDTH - width) / 2;
      expand = (seg.WIDTH + spacing - env.grid_dy) / 2;
      if ( expand < 0 ) expand = 0;
      rect.x1 = seg.X1 - expand;
      rect.x2 = seg.X2 + expand;
      rect.y1 = seg.Y1;
      rect.y2 = seg.Y2;
      break;
    case LEFT:
    case RIGHT:
    default:
      rect.x1 = seg.X1;
      rect.x2 = seg.X2;
      //rect.y1 = seg.Y1 - (seg.WIDTH - width) / 2;
      //rect.y2 = seg.Y2 + (seg.WIDTH - width) / 2;
      expand = (seg.WIDTH + spacing - env.grid_dx) / 2;
      if ( expand < 0 ) expand = 0;
      rect.y1 = seg.Y1 - expand;
      rect.y2 = seg.Y2 + expand;
      break;
  }
}




// -------------------------------------------------------------------
// Method  :  "CXRect::rect2grid()".

void  CXRect::rect2grid (void)
{
  grid.x1 = (rect.x1 - drgrid->xoffset) / env.grid_dx;
  grid.y1 = (rect.y1 - drgrid->yoffset) / env.grid_dy;

  grid.x2 = (rect.x2 - drgrid->xoffset) / env.grid_dx;
  grid.y2 = (rect.y2 - drgrid->yoffset) / env.grid_dy;

  if (((rect.x2 - drgrid->xoffset) % env.grid_dx) != 0) grid.x2 += 1;
  if (((rect.y2 - drgrid->yoffset) % env.grid_dy) != 0) grid.y2 += 1;

  if (    (grid.x2 < 0) || (grid.x1 >= drgrid->X)
       || (grid.y2 < 0) || (grid.y1 >= drgrid->Y) ) {
    grid.x1 = -1;
  } else {
    if (grid.x1 < 0          ) grid.x1 = 0;
    if (grid.y1 < 0          ) grid.y1 = 0;
    if (grid.x2 > drgrid->X-1) grid.x2 = drgrid->X - 1;
    if (grid.y2 > drgrid->Y-1) grid.y2 = drgrid->Y - 1;
  }
}




// -------------------------------------------------------------------
// Method  :  "CXRect::isInGrid()".

bool  CXRect::isInGrid ()
{
  if (grid.x1 < 0) return false;

  return true;
}




// -------------------------------------------------------------------
// Function (friend CXRect)  :  "operator<<()".

ostream &operator<< (ostream &o, const CXRect *rect)
{
  o << "+         CXRect::seg ("
    << rect->seg.X1 << ", "
    << rect->seg.Y1 << "), ("
    << rect->seg.X2 << ", "
    << rect->seg.Y2 << ") "
    << rect->seg.WIDTH << " "
    << layer2a(rect->seg.LAYER) << "\n";

  o << "+         CXRect::rect(" << &(rect->rect) << ")\n";
  o << "+         CXRect::grid(" << &(rect->grid) << ")\n";

  return (o);
}




// -------------------------------------------------------------------
// Constructor  :  "CEnv::CEnv()".

CEnv::CEnv (void)
{
  // Load the UNIX environmment for MBK.
  mbkenv ();

  // Copy constants values from constants namespace ("D::").
  // Routing constants, loaded now that we have SCALE_X.
  D::X_GRID             = ::MBK::SCALE (MBK_X_GRID);
  D::Y_GRID             = ::MBK::SCALE (MBK_Y_GRID);
  D::WIDTH_VSS          = ::MBK::SCALE (MBK_WIDTH_VSS);
  D::WIDTH_VDD          = ::MBK::SCALE (MBK_WIDTH_VDD);
  D::Y_SLICE            = ::MBK::SCALE (MBK_Y_SLICE);
  D::TRACK_WIDTH_ALU1   = ::MBK::SCALE (MBK_TRACK_WIDTH_ALU1);
  D::TRACK_WIDTH_ALU2   = ::MBK::SCALE (MBK_TRACK_WIDTH_ALU2);
  D::TRACK_WIDTH_ALU3   = ::MBK::SCALE (MBK_TRACK_WIDTH_ALU3);
  D::TRACK_WIDTH_ALU4   = ::MBK::SCALE (MBK_TRACK_WIDTH_ALU4);
  D::TRACK_WIDTH_ALU5   = ::MBK::SCALE (MBK_TRACK_WIDTH_ALU5);
  D::TRACK_WIDTH_ALU6   = ::MBK::SCALE (MBK_TRACK_WIDTH_ALU6);
  D::TRACK_WIDTH_ALU7   = ::MBK::SCALE (MBK_TRACK_WIDTH_ALU7);
  D::TRACK_WIDTH_ALU8   = ::MBK::SCALE (MBK_TRACK_WIDTH_ALU8);
  D::TRACK_SPACING_ALU1 = ::MBK::SCALE (MBK_TRACK_SPACING_ALU1);
  D::TRACK_SPACING_ALU2 = ::MBK::SCALE (MBK_TRACK_SPACING_ALU2);
  D::TRACK_SPACING_ALU3 = ::MBK::SCALE (MBK_TRACK_SPACING_ALU3);
  D::TRACK_SPACING_ALU4 = ::MBK::SCALE (MBK_TRACK_SPACING_ALU4);
  D::TRACK_SPACING_ALU5 = ::MBK::SCALE (MBK_TRACK_SPACING_ALU5);
  D::TRACK_SPACING_ALU6 = ::MBK::SCALE (MBK_TRACK_SPACING_ALU6);
  D::TRACK_SPACING_ALU7 = ::MBK::SCALE (MBK_TRACK_SPACING_ALU7);
  D::TRACK_SPACING_ALU8 = ::MBK::SCALE (MBK_TRACK_SPACING_ALU8);

  // Grid spacing.
  grid_dx = D::X_GRID;
  grid_dy = D::Y_GRID;

  // Layers minimal width.
  ALU2W[ALU1]  = D::TRACK_WIDTH_ALU1;
  ALU2W[ALU2]  = D::TRACK_WIDTH_ALU2;
  ALU2W[ALU3]  = D::TRACK_WIDTH_ALU3;
  ALU2W[ALU4]  = D::TRACK_WIDTH_ALU4;
  ALU2W[ALU5]  = D::TRACK_WIDTH_ALU5;
  ALU2W[ALU6]  = D::TRACK_WIDTH_ALU6;
  ALU2W[ALU7]  = D::TRACK_WIDTH_ALU7;
  ALU2W[ALU8]  = D::TRACK_WIDTH_ALU8;
  ALU2W[CALU1] = D::TRACK_WIDTH_ALU1;
  ALU2W[CALU2] = D::TRACK_WIDTH_ALU2;
  ALU2W[CALU3] = D::TRACK_WIDTH_ALU3;
  ALU2W[CALU4] = D::TRACK_WIDTH_ALU4;
  ALU2W[CALU5] = D::TRACK_WIDTH_ALU5;
  ALU2W[CALU6] = D::TRACK_WIDTH_ALU6;
  ALU2W[CALU7] = D::TRACK_WIDTH_ALU7;
  ALU2W[CALU8] = D::TRACK_WIDTH_ALU8;
  ALU2W[TALU1] = D::TRACK_WIDTH_ALU1;
  ALU2W[TALU2] = D::TRACK_WIDTH_ALU2;
  ALU2W[TALU3] = D::TRACK_WIDTH_ALU3;
  ALU2W[TALU4] = D::TRACK_WIDTH_ALU4;
  ALU2W[TALU5] = D::TRACK_WIDTH_ALU5;
  ALU2W[TALU6] = D::TRACK_WIDTH_ALU6;
  ALU2W[TALU7] = D::TRACK_WIDTH_ALU7;
  ALU2W[TALU8] = D::TRACK_WIDTH_ALU8;

  // Layers minimal width.
  ALU2S[ALU1]  = D::TRACK_SPACING_ALU1;
  ALU2S[ALU2]  = D::TRACK_SPACING_ALU2;
  ALU2S[ALU3]  = D::TRACK_SPACING_ALU3;
  ALU2S[ALU4]  = D::TRACK_SPACING_ALU4;
  ALU2S[ALU5]  = D::TRACK_SPACING_ALU5;
  ALU2S[ALU6]  = D::TRACK_SPACING_ALU6;
  ALU2S[ALU7]  = D::TRACK_SPACING_ALU7;
  ALU2S[ALU8]  = D::TRACK_SPACING_ALU8;
  ALU2S[CALU1] = D::TRACK_SPACING_ALU1;
  ALU2S[CALU2] = D::TRACK_SPACING_ALU2;
  ALU2S[CALU3] = D::TRACK_SPACING_ALU3;
  ALU2S[CALU4] = D::TRACK_SPACING_ALU4;
  ALU2S[CALU5] = D::TRACK_SPACING_ALU5;
  ALU2S[CALU6] = D::TRACK_SPACING_ALU6;
  ALU2S[CALU7] = D::TRACK_SPACING_ALU7;
  ALU2S[CALU8] = D::TRACK_SPACING_ALU8;
  ALU2S[TALU1] = D::TRACK_SPACING_ALU1;
  ALU2S[TALU2] = D::TRACK_SPACING_ALU2;
  ALU2S[TALU3] = D::TRACK_SPACING_ALU3;
  ALU2S[TALU4] = D::TRACK_SPACING_ALU4;
  ALU2S[TALU5] = D::TRACK_SPACING_ALU5;
  ALU2S[TALU6] = D::TRACK_SPACING_ALU6;
  ALU2S[TALU7] = D::TRACK_SPACING_ALU7;
  ALU2S[TALU8] = D::TRACK_SPACING_ALU8;

  // Layer to Z translation table.
  ALU2Z[ALU1]  = 0;
  ALU2Z[ALU2]  = 1;
  ALU2Z[ALU3]  = 2;
  ALU2Z[ALU4]  = 3;
  ALU2Z[ALU5]  = 4;
  ALU2Z[ALU6]  = 5;
  ALU2Z[ALU7]  = 6;
  ALU2Z[ALU8]  = 7;
  ALU2Z[CALU1] = 0;
  ALU2Z[CALU2] = 1;
  ALU2Z[CALU3] = 2;
  ALU2Z[CALU4] = 3;
  ALU2Z[CALU5] = 4;
  ALU2Z[CALU6] = 5;
  ALU2Z[CALU7] = 6;
  ALU2Z[CALU8] = 7;
  ALU2Z[TALU1] = 0;
  ALU2Z[TALU2] = 1;
  ALU2Z[TALU3] = 2;
  ALU2Z[TALU4] = 3;
  ALU2Z[TALU5] = 4;
  ALU2Z[TALU6] = 5;
  ALU2Z[TALU7] = 6;
  ALU2Z[TALU8] = 7;

  regcomp(&pxLibRegex,"p.*px",REG_EXTENDED|REG_NOSUB);
}




// -------------------------------------------------------------------
// Method  :  "CEnv::layer2width()".

long  CEnv::layer2width (char layer) throw (except_done)
{
  MLayer::iterator  itLayer;


  if ((itLayer = ALU2W.find (layer)) == ALU2W.end ()) {
    cerr << herr ("CEnv::layer2width ():\n");
    cerr << "  Layer id " << (int)layer << " is not supported.\n";
    throw except_done ();
  }
    
  return (itLayer->second);
}




// -------------------------------------------------------------------
// Method  :  "CEnv::layer2spacing()".

long  CEnv::layer2spacing (char layer) throw (except_done)
{
  MLayer::iterator  itLayer;


  if ((itLayer = ALU2S.find (layer)) == ALU2S.end ()) {
    cerr << herr ("CEnv::layer2spacing ():\n");
    cerr << "  Layer id " << (int)layer << " is not supported.\n";
    throw except_done ();
  }
    
  return (itLayer->second);
}




// -------------------------------------------------------------------
// Method  :  "CEnv::layer2z()".

long  CEnv::layer2z (char layer) throw (except_done)
{
  MLayer::iterator  itLayer;


  if ((itLayer = ALU2Z.find (layer)) == ALU2Z.end ()) {
    cerr << herr ("CEnv::layer2z ():\n");
    cerr << "  Layer id " << (int)layer << " is not supported.\n";
    throw except_done ();
  }
    
  return (itLayer->second);
}




// -------------------------------------------------------------------
// Method  :  "CEnv::z2alu()".

char  CEnv::z2alu (int z) throw (except_done)
{
  MLayer::iterator  itLayer, endLayer;


  endLayer = ALU2Z.end ();
  for (itLayer = ALU2Z.begin(); itLayer != endLayer; itLayer++) {
    if (isALU(itLayer->first) && (itLayer->second == z))
      return (itLayer->first);
  }

  cerr << herr ("CEnv::z2alu ():\n");
  cerr << "  Z index " << z << " is out of bound.\n";

  throw except_done ();
}




// -------------------------------------------------------------------
// Method  :  "CEnv::z2calu()".

char  CEnv::z2calu (int z) throw (except_done)
{
  MLayer::iterator  itLayer, endLayer;


  endLayer = ALU2Z.end ();
  for (itLayer = ALU2Z.begin(); itLayer != endLayer; itLayer++) {
    if (isCALU(itLayer->first) && (itLayer->second == z))
      return (itLayer->first);
  }

  cerr << herr ("CEnv::z2calu ():\n");
  cerr << "  Z index " << z << " is out of bound.\n";

  throw except_done ();
}




// -------------------------------------------------------------------
// Method  :  "CEnv::z2via()".

char  CEnv::z2via (int z) throw (except_done)
{
  char viaType;


  switch (z) {
    case 1: viaType = CONT_VIA;  break;
    case 2: viaType = CONT_VIA2; break;
    case 3: viaType = CONT_VIA3; break;
    case 4: viaType = CONT_VIA4; break;
    case 5: viaType = CONT_VIA5; break;
    case 6: viaType = CONT_VIA6; break;
    default:
      cerr << herr ("CEnv::z2via ():\n");
      cerr << "  Z index " << z << " is out of bound.\n";
      throw except_done ();
  }

  return (viaType);
}




// -------------------------------------------------------------------
// Friend of "CEnv" :  "&operator<<()".

ostream &operator<< (ostream &o, const CEnv &self)
{
  int  i;

  o << "\n"
    << "  o  MBK environment :\n\n"
    << "     MBK_IN_LO       := " << IN_LO    << "\n"
    << "     MBK_OUT_LO      := " << OUT_LO   << "\n"
    << "     MBK_IN_PH       := " << IN_PH    << "\n"
    << "     MBK_OUT_PH      := " << OUT_PH   << "\n"
    << "     MBK_WORK_LIB    := " << WORK_LIB << "\n";

  for(i = 0; CATA_LIB[i] != (char *)NULL; i++) {
    if (i == 0) o << "     MBK_CATA_LIB    := ";
    else        o << "                        ";
    o << CATA_LIB[i] << "\n";
  }

  o << "     MBK_CATAL_NAME  := " << CATAL << "\n"
    << "     MBK_VDD         := " << VDD   << "\n"
    << "     MBK_VSS         := " << VSS   << "\n"
    << "     MBK_SEPAR       := " << SEPAR << "\n"
    << "\n";

  return (o);
}




// -------------------------------------------------------------------
// Constructor :  "CLofig::CLofig()".

CLofig::CLofig (string &name)
{
  loins_list *pLoins;


  cmess1 << "  o  Loading netlist \"" << name << "\"...\n";
  fig = getlofig ((char *)name.c_str (), 'A');

  // Build the instances dictionnary (map).
  for (pLoins = fig->LOINS; pLoins != NULL; pLoins = pLoins->NEXT) {
    instances[pLoins->INSNAME] = pLoins;
  }
}




// -------------------------------------------------------------------
// Destructor :  "~CLofig::CLofig()".

CLofig::~CLofig (void)
{
  dellofig (fig->NAME);
}




// -------------------------------------------------------------------
// Method :  "CLofig::rflatten()".

void CLofig::rflatten (char concat, char catal)
{
  loins_list *pLoins;
  losig_list *pLosig;
  chain_list *pChain;


  cmess2 << "  o  Flattening netlist...\n";
  rflattenlofig (fig, concat, catal);

  // Rebuild the instances dictionnary (map).
  instances.clear ();
  for (pLoins = fig->LOINS; pLoins != NULL; pLoins = pLoins->NEXT) {
    instances[pLoins->INSNAME] = pLoins;
  }

  // Build the signal dictionnary.
  // Load all name aliases.
  for (pLosig = fig->LOSIG; pLosig != NULL; pLosig = pLosig->NEXT) {
    //signals[getsigname(pLosig)] = pLosig;
    for (pChain = pLosig->NAMECHAIN ; pChain !=NULL; pChain = pChain->NEXT) {
      signals[(char*)pChain->DATA] = pLosig;
    }
  }
}




// -------------------------------------------------------------------
// Method :  "CLofig::chain()".

void CLofig::chain (void)
{
  cmess2 << "  o  Building netlist dual representation (lofigchain)...\n";
  lofigchain (fig);
}




// -------------------------------------------------------------------
// Method :  "CLofig::save()".

void CLofig::save (void)
{
  cmess1 << "  o  Saving netlist \"" << fig->NAME << "\"...\n";
  savelofig (fig);
}




// -------------------------------------------------------------------
// Method :  "CLofig::saveas()".

void CLofig::saveas (string &name)
{
  char *current_name;

  cmess1 << "  o  Saving netlist as \"" << name << "\"...\n";

  current_name = fig->NAME;
  fig->NAME = namealloc ((char*)name.c_str ());

  savelofig (fig);

  fig->NAME = current_name;
}




// -------------------------------------------------------------------
// Constructor :  "CPhfig::CPhfig()".

CPhfig::CPhfig (string &name)
{
  phins_list *pPhins;


  cmess1 << "  o  Loading layout \"" << name << "\"...\n";
  fig = getphfig ((char *)name.c_str (), 'A');

  // Build the instances dictionnary (map).
  for (pPhins = fig->PHINS; pPhins != NULL; pPhins = pPhins->NEXT) {
    instances[pPhins->INSNAME] = pPhins;
  }
}




// -------------------------------------------------------------------
// Destructor :  "~CPhfig::CPhfig()".

CPhfig::~CPhfig (void)
{
  delphfig (fig->NAME);
}




// -------------------------------------------------------------------
// Method :  "CPhfig::rflatten()".

void CPhfig::rflatten (char concat, char catal)
{
  phins_list *pPhins;


  cmess2 << "  o  Flattening layout...\n";
  rflattenphfig (fig, concat, catal);

  // Rebuild the instances dictionnary (map).
  instances.clear ();
  for (pPhins = fig->PHINS; pPhins != NULL; pPhins = pPhins->NEXT) {
    instances[pPhins->INSNAME] = pPhins;
  }
}




// -------------------------------------------------------------------
// Method :  "CPhfig::onslice()".

bool CPhfig::onslice (long Y, long yoff)
{
  if (Y < fig->YAB1) return (false);
  if (Y > fig->YAB2) return (false);

  if (!(((Y - D::WIDTH_VDD / 2) - yoff ) % D::Y_SLICE)) return (true);
  if (!(((Y + D::WIDTH_VDD / 2) - yoff ) % D::Y_SLICE)) return (true);
  if (!(((Y - D::WIDTH_VSS / 2) - yoff ) % D::Y_SLICE)) return (true);
  if (!(((Y + D::WIDTH_VSS / 2) - yoff ) % D::Y_SLICE)) return (true);

  return (false);
}




// -------------------------------------------------------------------
// Method :  "CPhfig::save()".

void CPhfig::save (void)
{
  cmess1 << "  o  Saving layout \"" << fig->NAME << "\"...\n";
  savephfig (fig);
}




// -------------------------------------------------------------------
// Method :  "CPhfig::saveas()".

void CPhfig::saveas (string &name)
{
  char *current_name;

  cmess1 << "  o  Saving layout as \"" << name << "\"...\n";

  current_name = fig->NAME;
  fig->NAME = namealloc ((char*)name.c_str ());

  savephfig (fig);

  fig->NAME = current_name;
}




// -------------------------------------------------------------------
// Constructor :  "CIns::CIns()".

CIns::CIns ( loins_list *lo
           , phins_list *ph
           , phfig_list *mod
           , phfig_list *fig
           )
{
  loins = lo;
  phins = ph;
  model = mod;
  phfig = fig;
}




// -------------------------------------------------------------------
// Method :  "CIns::getmodel()".

phfig_list *CIns::getmodel (void)
{
  if (!model) model = getphfig (phins->FIGNAME, 'A');

  return (model);
}




// -------------------------------------------------------------------
// Method :  "CIns::flatseg()".

phseg_list &CIns::flatseg (phseg_list &flatSeg, phseg_list &seg)
{
  flatSeg.NAME  = seg.NAME;
  flatSeg.LAYER = seg.LAYER;
  flatSeg.WIDTH = seg.WIDTH;


  xyflat ( &flatSeg.X1,     &flatSeg.Y1
         ,      seg.X1,          seg.Y1
         , phins->XINS, phins->YINS
         , model->XAB1, model->YAB1
         , model->XAB2, model->YAB2
         , phins->TRANSF
         );

  xyflat ( &flatSeg.X2  ,   &flatSeg.Y2
         ,      seg.X2  ,        seg.Y2
         ,   phins->XINS, phins->YINS
         ,   model->XAB1, model->YAB1
         ,   model->XAB2, model->YAB2
         ,   phins->TRANSF
         );

  // Ensure that (X1,Y1) < (X2,Y2) and type is correct.
  if (flatSeg.X1 > flatSeg.X2) { swap (flatSeg.X1, flatSeg.X2); }
  if (flatSeg.Y1 > flatSeg.Y2) { swap (flatSeg.Y1, flatSeg.Y2); }

  if (flatSeg.Y1 == flatSeg.Y2) {
    if (flatSeg.X1 < flatSeg.X2) flatSeg.TYPE = LEFT;
    else                         flatSeg.TYPE = RIGHT;
  } else {
    if (flatSeg.Y1 < flatSeg.Y2) flatSeg.TYPE = UP;
    else                         flatSeg.TYPE = DOWN;
  }

  // Return the flattened segment;
  return (flatSeg);
}




// -------------------------------------------------------------------
// Constructor :  "CFig::CFig()".

CFig::CFig (string &nameLofig, string &namePhfig) throw (except_done):
  lofig (nameLofig), phfig (namePhfig)
{
  MLoins::iterator  itLoins, endLoins;
  MPhins::iterator  itPhins, endPhins;
  long              errors;


  // Flatten both views & build dual representation.
  phfig.rflatten ();
  lofig.rflatten ();
  lofig.chain ();

  cmess2 << "  o  Binding logical & physical views...\n";

  errors   = 0;
  endLoins = lofig.instances.end ();
  endPhins = phfig.instances.end ();

  // Match loins with phins.
  for (itLoins = lofig.instances.begin(); itLoins != endLoins; itLoins++) {
    // Find the matched phins.
    itPhins = phfig.instances.find (itLoins->first);

    if (itPhins == endPhins) {
      errors += 1;

      cerr << herr ("");
      cerr << " logical instance \"" << itLoins->second->INSNAME << "\"";
      cerr << " of model \"" << itLoins->second->FIGNAME << "\"";
      cerr << " doesn't appear in physical view.\n";
      continue;
    }

    instances[itLoins->first] = new CIns ( itLoins->second
                                         , itPhins->second
                                         , NULL
                                         , phfig.fig
                                         );
  }

  // If the match is not complete, do a reverse one to find
  // unmatched phins (mostly feedthrough cells).
  if (   (errors > 0)
      || ( lofig.instances.size() != phfig.instances.size() ) ) {
    for (itPhins = phfig.instances.begin(); itPhins != endPhins; itPhins++) {
      // Add feedthrough cells to the orphan list.
      if (incatalogfeed (itPhins->second->FIGNAME) != 0) {
        orphans[itPhins->first] = new CIns ( NULL
                                           , itPhins->second
                                           , NULL
                                           , this->phfig.fig
                                           );

        continue;
      }

      // Find the matched loins.
      itLoins = lofig.instances.find (itPhins->first);

      if (itLoins == endLoins) {
        errors += 1;

        cerr << herr ("");
        cerr << " physical instance \"" << itPhins->second->INSNAME << "\"";
        cerr << " of model \"" << itPhins->second->FIGNAME << "\"";
        cerr << " doesn't appear in logical view.";
        continue;
      }
    }
  }

  // The nelist and lofig doesn't match.
  if (errors > 0) throw except_done ();
}




// -------------------------------------------------------------------
// Destructor :  "CFig::~CFig()".

CFig::~CFig (void)
{
  MIns::iterator  itIns, beginIns, endIns;


  cmess2 << "  o  Deleting CFig...";

  // Delete the CIns structures used in the map.
  endIns = instances.end ();
  for (itIns  = instances.begin(); itIns != endIns; itIns++)
    delete itIns->second;

  endIns = orphans.end ();
  for (itIns  = orphans.begin(); itIns != endIns; itIns++)
    delete itIns->second;
}




// -------------------------------------------------------------------
// Method :  "CFig::addphseg()".

void  CFig::addphseg (phseg_list &seg, bool isTerm, bool isChip )
{
  if ( !isChip || !isobs(seg.LAYER) ) {
    MBK::addphseg ( phfig.fig
                  , seg.LAYER
                  , seg.WIDTH
                  , seg.X1
                  , seg.Y1
                  , seg.X2
                  , seg.Y2
                  , seg.NAME
                  );
  }

  if ( !isTerm && !isChip ) {
    MBK::addphseg ( phfig.fig
                  , layer2TALU(seg.LAYER)
                  , seg.WIDTH
                  , seg.X1
                  , seg.Y1
                  , seg.X2
                  , seg.Y2
                  , "obs"
                  );
  }
}


// -------------------------------------------------------------------
// Method :  "CFig::addphvia()".

void  CFig::addphvia (phvia_list &VIA)
{
  MBK::addphvia ( phfig.fig
                , VIA.TYPE
                , VIA.XVIA
                , VIA.YVIA
                , VIA.DX
                , VIA.DY
                , VIA.NAME
                );
}


// -------------------------------------------------------------------
// Method :  "CFig::addphcon()".

void  CFig::addphcon (phcon_list &con)
{
  MBK::addphcon ( phfig.fig
                , con.ORIENT
                , con.NAME
                , con.XCON
                , con.YCON
                , con.LAYER
                , con.WIDTH
                );
}


// -------------------------------------------------------------------
// Method :  "CFig::addphref()".

void  CFig::addphref (phref_list &ref)
{
  MBK::addphref ( phfig.fig
                , ref.FIGNAME
                , ref.NAME
                , ref.XREF
                , ref.YREF
                );
}




// -------------------------------------------------------------------
// Function  :  "layer2a()".

extern char *layer2a(char layer)
{
  switch (layer) {
    case NDIF:  return ("NDIF");
    case PDIF:  return ("PDIF");
    case ALU1:  return ("ALU1");
    case ALU2:  return ("ALU2");
    case ALU3:  return ("ALU3");
    case ALU4:  return ("ALU4");
    case ALU5:  return ("ALU5");
    case ALU6:  return ("ALU6");
    case ALU7:  return ("ALU7");
    case ALU8:  return ("ALU8");
    case ALU9:  return ("ALU9");
    case CALU1: return ("CALU1");
    case CALU2: return ("CALU2");
    case CALU3: return ("CALU3");
    case CALU4: return ("CALU4");
    case CALU5: return ("CALU5");
    case CALU6: return ("CALU6");
    case CALU7: return ("CALU7");
    case CALU8: return ("CALU8");
    case CALU9: return ("CALU9");
    case TALU1: return ("TALU1");
    case TALU2: return ("TALU2");
    case TALU3: return ("TALU3");
    case TALU4: return ("TALU4");
    case TALU5: return ("TALU5");
    case TALU6: return ("TALU6");
    case TALU7: return ("TALU7");
    case TALU8: return ("TALU8");
    case TALU9: return ("TALU9");
  }

  return ("unknown layer");
}




// -------------------------------------------------------------------
// Function  :  "isobs()".

extern bool  isobs(char layer)
{
  switch (layer) {
    case TALU1:
    case TALU2:
    case TALU3:
    case TALU4:
    case TALU5:
    case TALU6:
    case TALU7:
    case TALU8:
    case TALU9:
      return (true);
  }

  return (false);
}




// -------------------------------------------------------------------
// Function  :  "isALU()".

extern bool  isALU(char layer)
{
  switch (layer) {
    case ALU1:
    case ALU2:
    case ALU3:
    case ALU4:
    case ALU5:
    case ALU6:
    case ALU7:
    case ALU8:
    case ALU9: return (true);
  }
}




// -------------------------------------------------------------------
// Function  :  "isCALU()".

bool  isCALU (char layer)
{
  switch (layer) {
    case CALU1:
    case CALU2:
    case CALU3:
    case CALU4:
    case CALU5:
    case CALU6:
    case CALU7:
    case CALU8:
    case CALU9: return (true);
  }


  return (false);
}




// -------------------------------------------------------------------
// Function  :  "layer2TALU()".

char  layer2TALU (char layer)
{
  switch (layer) {
    case  ALU1:
    case TALU1:
      return (TALU1);
    case  ALU2:
    case TALU2:
      return (TALU2);
    case  ALU3:
    case TALU3:
      return (TALU3);
    case  ALU4:
    case TALU4:
      return (TALU4);
    case  ALU5:
    case TALU5:
      return (TALU5);
    case  ALU6:
    case TALU6:
      return (TALU6);
    case TALU7:
    case  ALU7:
      return (TALU7);
    case TALU8:
    case  ALU8:
      return (TALU8);
    case TALU9:
    case  ALU9:
      return (TALU9);
  }

  return (TALU1);
}





// -------------------------------------------------------------------
// Function  :  "layer2CALU()".

char  layer2CALU (char layer)
{
  switch (layer) {
    case  ALU1:
    case TALU1:
      return (CALU1);
    case  ALU2:
    case TALU2:
      return (CALU2);
    case  ALU3:
    case TALU3:
      return (CALU3);
    case  ALU4:
    case TALU4:
      return (CALU4);
    case  ALU5:
    case TALU5:
      return (CALU5);
    case  ALU6:
    case TALU6:
      return (CALU6);
    case TALU7:
    case  ALU7:
      return (CALU7);
    case TALU8:
    case  ALU8:
      return (CALU8);
    case TALU9:
    case  ALU9:
      return (CALU9);
  }

  return (CALU1);
}




// -------------------------------------------------------------------
// Function  :  "layer2ALU()".

char  layer2ALU (char layer)
{
  switch (layer) {
    case  ALU1:
    case CALU1:
    case TALU1:
      return (ALU1);
    case  ALU2:
    case CALU2:
    case TALU2:
      return (ALU2);
    case  ALU3:
    case CALU3:
    case TALU3:
      return (ALU3);
    case  ALU4:
    case CALU4:
    case TALU4:
      return (ALU4);
    case  ALU5:
    case CALU5:
    case TALU5:
      return (ALU5);
    case  ALU6:
    case CALU6:
    case TALU6:
      return (ALU6);
    case TALU7:
    case CALU7:
    case  ALU7:
      return (ALU7);
    case TALU8:
    case CALU8:
    case  ALU8:
      return (ALU8);
    case TALU9:
    case CALU9:
    case  ALU9:
      return (ALU9);
  }

  return (CALU1);
}





// -------------------------------------------------------------------
// Function  :  "cmpALU()".

long  cmpALU (char layer1, char layer2)
{
  switch(layer1) {
    case ALU1:
      switch(layer2) {
        case  ALU1: return(F_EQUAL_M);
        case CALU1: return(F_EQUAL_C);
        case TALU1: return(F_EQUAL_T);
      }
      break;

    case ALU2:
      switch(layer2) {
        case  ALU2: return(F_EQUAL_M);
        case CALU2: return(F_EQUAL_C);
        case TALU2: return(F_EQUAL_T);
      }
      break;

    case ALU3:
      switch(layer2) {
        case  ALU3: return(F_EQUAL_M);
        case CALU3: return(F_EQUAL_C);
        case TALU3: return(F_EQUAL_T);
      }
      break;

    case ALU4:
      switch(layer2) {
        case  ALU4: return(F_EQUAL_M);
        case CALU4: return(F_EQUAL_C);
        case TALU4: return(F_EQUAL_T);
      }
      break;

    case ALU5:
      switch(layer2) {
        case  ALU5: return(F_EQUAL_M);
        case CALU5: return(F_EQUAL_C);
        case TALU5: return(F_EQUAL_T);
      }
      break;

    case ALU6:
      switch(layer2) {
        case  ALU6: return(F_EQUAL_M);
        case CALU6: return(F_EQUAL_C);
        case TALU6: return(F_EQUAL_T);
      }
      break;

    case ALU7:
      switch(layer2) {
        case  ALU7: return(F_EQUAL_M);
        case CALU7: return(F_EQUAL_C);
        case TALU7: return(F_EQUAL_T);
      }
      break;

    case ALU8:
      switch(layer2) {
        case  ALU8: return(F_EQUAL_M);
        case CALU8: return(F_EQUAL_C);
        case TALU8: return(F_EQUAL_T);
      }
      break;

    case ALU9:
      switch(layer2) {
        case  ALU9: return(F_EQUAL_M);
        case CALU9: return(F_EQUAL_C);
        case TALU9: return(F_EQUAL_T);
      }
      break;
  }

  return(0);
}





// -------------------------------------------------------------------
// Function  :  "topVIALayer()".

char  topVIALayer (char type)
{
  switch (type) {
    case CONT_VIA:   return (ALU1); break;
    case CONT_VIA2:  return (ALU2); break;
    case CONT_VIA3:  return (ALU3); break;
    case CONT_VIA4:  return (ALU4); break;
    case CONT_VIA5:  return (ALU5); break;
    case CONT_VIA6:  return (ALU6); break;
    case CONT_VIA7:  return (ALU7); break;
    case CONT_TURN1: return (ALU1); break;
    case CONT_TURN2: return (ALU2); break;
    case CONT_TURN3: return (ALU3); break;
    case CONT_TURN4: return (ALU4); break;
    case CONT_TURN5: return (ALU5); break;
    case CONT_TURN6: return (ALU6); break;
    case CONT_TURN7: return (ALU7); break;
  }

  return (ALU8);
}





// -------------------------------------------------------------------
// Function  :  "bottomVIALayer()".

char  bottomVIALayer (char type)
{
  switch (type) {
    case CONT_VIA:   return (ALU2); break;
    case CONT_VIA2:  return (ALU3); break;
    case CONT_VIA3:  return (ALU4); break;
    case CONT_VIA4:  return (ALU5); break;
    case CONT_VIA5:  return (ALU6); break;
    case CONT_VIA6:  return (ALU7); break;
    case CONT_VIA7:  return (ALU8); break;
    case CONT_TURN1: return (ALU1); break;
    case CONT_TURN2: return (ALU2); break;
    case CONT_TURN3: return (ALU3); break;
    case CONT_TURN4: return (ALU4); break;
    case CONT_TURN5: return (ALU5); break;
    case CONT_TURN6: return (ALU6); break;
    case CONT_TURN7: return (ALU7); break;
  }

  return (ALU9);
}




// -------------------------------------------------------------------
// Function  :  "isPxLib()".

bool  IsPxLib ( phfig_list* model )
{
  return regexec(&env.pxLibRegex,model->NAME,0,NULL,0) == 0;
}




// -------------------------------------------------------------------
// End of MBK namespace.

}
