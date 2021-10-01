
// -*- C++ -*-
//
// $Id: MMBK.h,v 1.6 2005/10/17 23:11:06 jpc Exp $
//
// +-----------------------------------------------------------------+ 
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./UMBK.h"                                 |
// +-----------------------------------------------------------------+




#ifndef  __UMBK__
#define  __UMBK__  1


# include  <regex.h>


// -------------------------------------------------------------------
// MBK namespace.

namespace MBK {

# include  <mut.h>
# include  <mlo.h>
# include  <mlu.h>
# include  <mph.h>
# include  <mpu.h>
# include  <mgn.h>
# include  <abl.h>
# include  <aut.h>
# include  <beh.h>




// -------------------------------------------------------------------
// Module  :  "UMBK.cpp".


  // ---------------------------------------------------------------
  // Constants.



# define        F_EQUAL            (1L)
# define        F_CALU             (2L)
# define        F_TALU             (4L)
# define        F_EQUAL_M          (F_EQUAL)
# define        F_EQUAL_C          (F_EQUAL | F_CALU)
# define        F_EQUAL_T          (F_EQUAL | F_TALU)




  // ---------------------------------------------------------------
  // Types.

  typedef  map<string, loins_list*>  MLoins;
  typedef  map<string, losig_list*>  MLosig;
  typedef  map<string, phins_list*>  MPhins;
  typedef  map<char, long>           MLayer;




  // ---------------------------------------------------------------
  // Inline functions.

  inline long  SCALE   (long  x) { return (x * SCALE_X); }
  inline long  UNSCALE (long  x) { return (x / SCALE_X); }
  inline char *ISVSS   (char *s) { return (instr ((s), VSS, SEPAR)); }
  inline char *ISVDD   (char *s) { return (instr ((s), VDD, SEPAR)); }




  // ---------------------------------------------------------------
  // Functions.

  extern const char *layer2a (char layer);
  extern       bool  isobs (char layer);
  extern       bool  isALU (char layer);
  extern       bool  isCALU (char layer);
  extern       char  layer2ALU (char layer);
  extern       char  layer2CALU (char layer);
  extern       char  layer2TALU (char layer);
  extern       long  cmpALU (char layer1, char layer2);
  extern       char  topVIALayer (char type);
  extern       char  bottomVIALayer (char type);



  // ---------------------------------------------------------------
  // Extended rectangle class.

  class CXRect {

    // Attributes.
    public: CDRGrid*    drgrid; // Associated grid, to check limits.
    public: phseg_list  seg;    // MBK segment.
    public: CRect       rect;   // Rectangle (MBK coordinates).
    public: CRect       grid;   // Rectangle (routing grid units).

      // Constructor.
    public: CXRect (CDRGrid* agrid);

    // Predicate.
    public: bool isInGrid ();

    // Modifiers.
    public:  void  setSeg    (phseg_list &rSeg);
    private: void  seg2rect  (void);
    private: void  rect2grid (void);

    // Friend.
    friend ostream &operator<< (ostream &, const CXRect *);

  };




  // ---------------------------------------------------------------
  // MBK env wrapper class.

  struct CEnv {

    // Attributes.
    long     grid_dx;
    long     grid_dy;
    MLayer   ALU2W;
    MLayer   ALU2S;
    MLayer   ALU2Z;
    regex_t  pxLibRegex;

    // Constructor.
    CEnv (void);

    // Accessors.
    long  layer2spacing (char layer) ;
    long  layer2width   (char layer) ;
    long  layer2z       (char layer) ;
    char  z2calu        (int z) ;
    char  z2alu         (int z) ;
    char  z2via         (int z) ;
    long  z2width       (int z) { return (layer2width (z2alu(z))); }
    

    // Friends.
    friend ostream &operator<< (ostream &o, const CEnv &self);
  };




  // ---------------------------------------------------------------
  // MBK Loins wrapper class.

  struct CLofig {

    // Attributes.
    lofig_list *fig;
    MLoins      instances;
    MLosig      signals;
    
    // Constructor.
    CLofig (string &name);
    
    // Destructor.
    ~CLofig (void);

    // Modifiers.
    void rflatten (char concat=YES, char catal=YES);
    void chain    (void);
    void save     (void);
    void saveas   (string &name);

  };




  // ---------------------------------------------------------------
  // MBK Phfig wrapper class.

  struct CPhfig {

    // Attributes.
    phfig_list *fig;
    MPhins      instances;
    
    // Constructor.
    CPhfig (string &name);
    
    // Destructor.
    ~CPhfig (void);

    // Modifiers.
    void rflatten (char concat=YES, char catal=YES);
    bool onslice  (long Y, long xoff);
    void save     (void);
    void saveas   (string &name);

  };




  // ---------------------------------------------------------------
  // MBK loins+phins+model wrapper class.

  struct CIns {

    // Attributes.
    loins_list *loins;
    phins_list *phins;
    phfig_list *model;
    phfig_list *phfig;

    // Constructor.
    CIns ( loins_list *lo
         , phins_list *ph
         , phfig_list *mod=NULL
         , phfig_list *fig=NULL
         );

    // Accessor.
    phseg_list &flatseg (phseg_list &flatSeg, phseg_list &seg);

    // Modifier.
    phfig_list *getmodel (void);

  };


  typedef map<string, CIns*>  MIns;




  // ---------------------------------------------------------------
  // MBK lofig+phfig wrapper class.

  struct CFig {

    // Attributes.
    CLofig  lofig;
    CPhfig  phfig;
    MIns    instances;
    MIns    orphans;

    // Constructor.
    CFig (string &nameLofig, string &namePhfig) ;

    // Destructor.
    ~CFig (void);

    // Accessors.
    long  XAB1 (void) { return (phfig.fig->XAB1); }
    long  YAB1 (void) { return (phfig.fig->YAB1); }
    long  XAB2 (void) { return (phfig.fig->XAB2); }
    long  YAB2 (void) { return (phfig.fig->YAB2); }
    losig_list *LOSIG (void) { return (lofig.fig->LOSIG); }

    // Modifiers.
    void  addphseg (phseg_list &seg, bool isTerm=false, bool isChip=false );
    void  addphvia (phvia_list &VIA);
    void  addphcon (phcon_list &con);
    void  addphref (phref_list &ref);

  };




  // ---------------------------------------------------------------
  // MBK level Utilities functions.

  bool  IsPxLib ( phfig_list* model );



  // ---------------------------------------------------------------
  // Pre-defined objects.

  // The creation of this object loads the MBK environment.
  extern CEnv  env;




// -------------------------------------------------------------------
// Module  :  "UPower.cpp".


  // ---------------------------------------------------------------
  // Constants.


# define        C_POWER_NONE            ((char)0)
# define        C_POWER_VDD             ((char)1)
# define        C_POWER_VSS             ((char)2)
# define        C_VERTICAL              ((char)1)
# define        C_HORIZONTAL            ((char)2)




  // ---------------------------------------------------------------
  // Power line class.

  struct CPower : public LInter {

    // Attributes.
    long  type;

    // Constructors.
    CPower (void);

    // Friends.
    friend ostream &operator<< (ostream &o, const CPower &self);

  };

  typedef  map<long, CPower>  LPower;




  // ---------------------------------------------------------------
  // Power line set class.

  struct CPowers {

    // Attributes.
    long    xoffset;
    long    yoffset;
    char    type;
    char    layer;
    long    width;
    long    AB1;
    long    AB2;
    LPower  powerLines;

    // Constructor.
    CPowers (CFig*, long xoff, long yoff, char, int, long) ;

    // Methods.
    void  dump (CFig *fig);

    // Friends.
    friend ostream &operator<< (ostream &o, CPowers &self);

  };

  typedef map<int, CPowers*>  MPowers;




// -------------------------------------------------------------------
// End of MBK namespace.

}


#endif
