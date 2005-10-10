
// -*- C++ -*-
//
// $Id: RBox.cpp,v 1.4 2005/10/10 15:34:06 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./RBox.cpp"                              |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "RDefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Destructor  :  "CRBox::~CRBox()".

CRBox::~CRBox (void)
{
  MNet::iterator  itNet, endNet;


  endNet = nets.end  ();
  for (itNet = nets.begin (); itNet != endNet; itNet++)
    delete itNet->second;

  delete netsched;
  delete drgrid;
}




// -------------------------------------------------------------------
// Constructor  :  "CRBox::CRBox()".

CRBox::CRBox (void)
{
  drgrid    = NULL;
  netsched  = NULL;
  loaded    = false;
  insave    = false;
}




// -------------------------------------------------------------------
// Constructor  :  "CRBox::CRBox()".

CRBox::CRBox (int rtype, bool debug)
{
  int     X, Y, Z;
  CNet   *pNet;
  CTerm  *pTerm;
  //CCoord  coord;


  cdebug << "ENTERING: CRBox::CRBox ()\n";
  X = 10 ; //10;
  Y = 15 ; //15;
  Z = 3;


  // Creating routing matrix.
  cdebug << "  Routing matrix size := (10, 15, 5)\n";

  drgrid = new CDRGrid (0, 0, X, Y, Z, 4);


  // Adding signal "sig_one".
  cdebug << "  Creating net \"sig_one\".\n";
  pNet = getnet ("sig_one");


  // Adding terminal "i0" of "sig_one".
  cdebug << "    Adding terminal \"i0\".\n";
  cdebug << "      Adding CA \"(0,0,0)\".\n";
  pNet->newaccess ("i0", 0, 0, 0);
  //cdebug << "      Adding CA \"(0,0,1)\".\n";
  //nets["sig_one"]->newaccess ("i0", 0, 0, 1);
  //cdebug << "      Adding CA \"(0,1,1)\".\n";
  //nets["sig_one"]->newaccess ("i0", 0, 1, 1);


  // Adding terminal "i1" of "sig_one".
  cdebug << "    Adding terminal \"i1\".\n";
  cdebug << "      Adding CA \"(9,0,0)\".\n";
  pNet->newaccess ("i1", 9, 0, 0);


  // Adding terminal "o" of "sig_one".
  cdebug << "    Adding terminal \"o\".\n";
  //cdebug << "      Adding CA \"(5,10,0)\".\n";
  //nets["sig_one"]->newaccess ("o", 5, 10, 0);

  //cdebug << "      Adding CA \"(7,12,0)\".\n";
  //nets["sig_one"]->newaccess ("o", 7, 12, 0);


  // Adding signal "sig_two".
  //cdebug << "  Creating net \"sig_two\".\n";
  //nets["sig_two"] = new CNet ("sig_two");


  // Adding terminal "i0" of "sig_two".
  //cdebug << "    Adding terminal \"i0\".\n";
  //cdebug << "      Adding CA \"(4,1,0)\".\n";
  //nets["sig_two"]->newaccess ("i0", 4, 1, 0);


  // Adding terminal "o" of "sig_two".
  //cdebug << "    Adding terminal \"o\".\n";
  //cdebug << "      Adding CA \"(4,11,0)\".\n";
  //nets["sig_two"]->newaccess ("o", 4, 11, 0);


  //{
  //  int j;

  //  for (j = 0; j < Y; j++) {
  //    (*::grid)[coord.set (160, j, 1)]->data.pri = 128;
  //    (*::grid)[coord.set (160, j, 2)]->data.pri = 128;
  //  } 
  //}
  // Putting obstacles.
  //(*::grid)[coord.set (5, 5, 2)]->data.pri = 128;
  //(*::grid)[coord.set (6, 5, 2)]->data.pri = 128;
  //(*::grid)[coord.set (7, 5, 2)]->data.pri = 128;
  //(*::grid)[coord.set (8, 5, 2)]->data.pri = 128;
  //(*::grid)[coord.set (9, 5, 2)]->data.pri = 128;


  netsched  = new CASimple (&nets, drgrid);

  cdebug << "EXITING: CRBox::CRBox ()\n";
}




// -------------------------------------------------------------------
// Method  :  "CRBox::route()".

void CRBox::route (void)
{
  netsched->run (rglobal);
}




// -------------------------------------------------------------------
// Method  :  "CRBox::findnet()".

CNet *CRBox::findnet (char *signame)
{
  string  name;


  name = signame;

  return (findnet(name));
}




// -------------------------------------------------------------------
// Method  :  "CRBox::findnet()".

CNet *CRBox::findnet (string &signame)
{
  MNet::iterator  itNet, endNet;
            CNet *pNet;


  endNet = nets.end  ();
  itNet  = nets.find (signame);

  if (itNet == endNet) {
    pNet = NULL;
  } else
    pNet = itNet->second;


  return (pNet);
}




// -------------------------------------------------------------------
// Method  :  "CRBox::getnet()".

CNet *CRBox::getnet (char *signame)
{
  string  name;


  name = signame;

  return (getnet(name));
}




// -------------------------------------------------------------------
// Method  :  "CRBox::getnet()".

CNet *CRBox::getnet (string &signame)
{
  MNet::iterator  itNet, endNet;
            CNet *pNet;


  endNet = nets.end  ();
  itNet  = nets.find (signame);

  if (itNet == endNet) {
    pNet = nets[signame] = new CNet (drgrid, signame);
  } else
    pNet = itNet->second;


  return (pNet);
}
