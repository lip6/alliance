
// -*- C++ -*-
//
// $Id: RDefs.h,v 1.1 2002/10/02 21:23:48 jpc Exp $
//
// /-----------------------------------------------------------------\ 
// |                                                                 |
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./RDefs.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// \-----------------------------------------------------------------/




# ifndef  __RDefs__
# define  __RDefs__  1


# include  "ADefs.h"




// -------------------------------------------------------------------
// Module  :  "RBox.cpp".


  // ---------------------------------------------------------------
  // Routing box class.

  class CRBox {

    // Attributes.
    public: CASimple   *netsched;
    public: CDRGrid    *drgrid;
    public: MNet        nets;
    public: bool        loaded;
    public: bool        rglobal;

    // MBK dedicated Attributes.
    public: MBK::CFig    *fig;
    public: MBK::MPowers  powers;

    // Constructor.
    public: CRBox (void);
    public: CRBox (int rtype, bool debug);

    // Destructor.
    public: ~CRBox (void);

    // Modifiers.
    public: CNet *getnet  (string &signame);
    public: CNet *getnet  (char *signame);
    public: void  mbkload (MBK::CFig *mbkfig, int z, int rtype);
    public: void  mbksave (string &name);
    public: void  route   (void);

  };



# endif
