
// -*- C++ -*-
//
// $Id: RDefs.h,v 1.6 2012/05/02 14:49:23 jpc Exp $
//
// +-----------------------------------------------------------------+ 
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./RDefs.h"                                |
// +-----------------------------------------------------------------+




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
    public: long        xoffsetgrid;
    public: long        yoffsetgrid;
    public: MNet        nets;
    public: bool        loaded;
    public: bool        insave;
    public: bool        rglobal;
    public: bool        ischip;

    // MBK dedicated Attributes.
    public: MBK::CFig    *fig;
    public: MBK::MPowers  powers;

    // Constructor.
    public: CRBox (void);
    public: CRBox (int rtype, bool debug);
    private: CRBox ( const CRBox& );

    // Destructor.
    public: ~CRBox (void);

    // Modifiers.
    public: CNet *findnet (string &signame);
    public: CNet *findnet (char *signame);
    public: CNet *getnet  (const string &signame);
    public: void  mbkload (MBK::CFig *mbkfig
                          , int z
                          , int zup
                          , int rtype
                          , bool halfpitch
                          , bool xhalfpitch
                          , bool yalfpitch
                          , bool rotate
                          , set<string>* subNetList=NULL );
    public: void  mbksave (string &name) ;
    public: void  route   (void);

  };



# endif
