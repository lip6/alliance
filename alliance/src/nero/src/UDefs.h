
// -*- C++ -*-
//
// $Id: UDefs.h,v 1.4 2012/05/02 14:49:23 jpc Exp $
//
// /-----------------------------------------------------------------\ 
// |                                                                 |
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./UDefs.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// \-----------------------------------------------------------------/




# ifndef  __UDefs__
# define  __UDefs__  1



# include  <iostream>
# include  <iomanip>
# include  <fstream>
# include  <sstream>
# include  <exception>
# include  <iterator>
# include  <utility>
# include  <string>
# include  <vector>
# include  <queue>
# include  <list>
# include  <map>
# include  <set>
# include  <stdexcept>
# include  <cstdlib>
# include  <cmath>

# include  <unistd.h>




// -------------------------------------------------------------------
// Load "std" namespace.

using namespace std;




// -------------------------------------------------------------------
// Namespace  :  "D".

namespace D {

  extern const int   CHUNK_SIZE;

  extern const int   cost_ALU2_X;
  extern const int   cost_ALU2_Y;
  extern const int   cost_ALU3_X;
  extern const int   cost_ALU3_Y;
  extern const int   cost_VIA;
  extern const bool  optim_AStar_queue;
  extern const bool  optim_AStar_order;
  extern const int   GLOBAL_HP;

  extern const int  ROUTING_CHOOSE;
  extern const int  ROUTING_GLOBAL;
  extern const int  ROUTING_LOCAL;


  // MBK Routing related constants.
  extern const long _X_GRID;
  extern const long _Y_GRID;
  extern const long _Y_SLICE;
  extern const long _WIDTH_VSS;
  extern const long _WIDTH_VDD;
  extern const long _TRACK_WIDTH_ALU1;
  extern const long _TRACK_WIDTH_ALU2;
  extern const long _TRACK_WIDTH_ALU3;
  extern const long _TRACK_WIDTH_ALU4;
  extern const long _TRACK_WIDTH_ALU5;
  extern const long _TRACK_WIDTH_ALU6;
  extern const long _TRACK_WIDTH_ALU7;
  extern const long _TRACK_WIDTH_ALU8;
  extern const long _TRACK_SPACING_ALU1;
  extern const long _TRACK_SPACING_ALU2;
  extern const long _TRACK_SPACING_ALU3;
  extern const long _TRACK_SPACING_ALU4;
  extern const long _TRACK_SPACING_ALU5;
  extern const long _TRACK_SPACING_ALU6;
  extern const long _TRACK_SPACING_ALU7;
  extern const long _TRACK_SPACING_ALU8;

  extern long  X_GRID;
  extern long  Y_GRID;
  extern long  WIDTH_VSS;
  extern long  WIDTH_VDD;
  extern long  Y_SLICE;
  extern long  TRACK_WIDTH_ALU1;
  extern long  TRACK_WIDTH_ALU2;
  extern long  TRACK_WIDTH_ALU3;
  extern long  TRACK_WIDTH_ALU4;
  extern long  TRACK_WIDTH_ALU5;
  extern long  TRACK_WIDTH_ALU6;
  extern long  TRACK_WIDTH_ALU7;
  extern long  TRACK_WIDTH_ALU8;
  extern long  TRACK_SPACING_ALU1;
  extern long  TRACK_SPACING_ALU2;
  extern long  TRACK_SPACING_ALU3;
  extern long  TRACK_SPACING_ALU4;
  extern long  TRACK_SPACING_ALU5;
  extern long  TRACK_SPACING_ALU6;
  extern long  TRACK_SPACING_ALU7;
  extern long  TRACK_SPACING_ALU8;

}



// -------------------------------------------------------------------
// Module  :  "USys.cpp".


  // ---------------------------------------------------------------
  // Message header class.

  class CHeader {

    // Internal attributes.
    protected: string  _header;
    protected: string  _message;

    // Constructor.
    public: CHeader (string h);

    // Operators.
    public: CHeader operator() (string m);

    // Friends.
    public: friend ostream &operator<< (ostream &o, const CHeader &self);
  };




  // ---------------------------------------------------------------
  // Message stream class (write on "cout").

  class CMess {

    // Internal attributes.
    protected: static long  _state;
    protected:        long  _level;

    // Constructor.
    public: CMess (long level) { _level = level; }

    // Modifiers.
    public: void setVL (long state) { _state = state; }

    // Accessor.
    public: long getVL (void) { return (_state); }

    public: template<class T>
              CMess &operator<< (T s) {
                if (_state >= _level) { cout << s; cout.flush (); }
                return (*this);
              }

  };




  // ---------------------------------------------------------------
  // Debug stream class (write on "cerr").

  class CDebug {

    // Internal attributes.
    protected: bool  _state;

    // Constructor.
    public: CDebug (bool state) { _state = state; }

    // Modifiers.
    public: void on  () { _state = true; }
    public: void off () { _state = false; }

    public: template<class T>
              CDebug &operator<< (T s) {
                if (_state) { cerr << s; cerr.flush (); }
                return (*this);
              }
  };




  // ---------------------------------------------------------------
  // Signal interrupt handler class (async).

  class CInterrupt {

    // Modifiable attributes.
    public: bool  coredump;

    // Constructor.
    public: CInterrupt (void);
  };




  // ---------------------------------------------------------------
  // Software interrupt class (exception).

  class except_done : public exception {

    // Overridables.
    public: const char* what () const throw () {
              return ((char*)"This exception has already been processed.");
            }
  };




  // ---------------------------------------------------------------
  // Pre-defined objects.

  // Output stream manipulators (header writing).
  extern CHeader  herr;
  extern CHeader  hwarn;
  extern CHeader  herrmbk;
  extern CHeader  hwarnmbk;

  // Messages stream of corresponding to various verbosity levels.
  extern CMess  cmess0;
  extern CMess  cmess1;
  extern CMess  cmess2;

  // Debug stream.
  extern CDebug  cdebug;

  // Interrupt handler.
  extern CInterrupt  interrupt;




// -------------------------------------------------------------------
// Module  :  "UOpts.cpp".


  // ---------------------------------------------------------------
  // Command line options class.

  class COpts {

    // Internal objects.
    public: struct COpt {

      // Attributes.
      bool   found;
      bool   parsed;
      bool   has_arg;
      string value;

      // Constructor.
      COpt (bool arg=false, string val="") {
        parsed   = false;
        has_arg  = arg;
        value    = value;
      }

      // Friends.
      public: friend ostream &operator<< (ostream &o, const COpt &self);
      public: friend ostream &operator<< (ostream &o, const COpt *self);
    };

    // Internal types.
    typedef map<string, long> MOpt;
    typedef vector<COpt*>     TOpt;

    // Attributes.
    private: MOpt            tDict;
    private: TOpt            tList;
    private: string          tShort;
    public:  vector<string>  tVals;

    // Constructor.
    COpts (void) { tShort = "+"; }

    // Modifiers.
    public: void getopts (int argc, char *argv[]) throw (except_done);
    public: void add ( string key_short
                     , string key_long
                     , bool   arg=false
                     , string val=""
                     ) throw (except_done);

    // Operators.
    private: long         operator() (string key) throw (except_done);
    public:  struct COpt *operator[] (string key);

  };




//  ------------------------------------------------------------------
//  Module  :  "UInter.cpp".


# define        SHIFT_MAX            8
# define        F_MIN_IN             0x00000001
# define        F_MIN_LOWER          0x00000002
# define        F_MIN_GREATER        0x00000004
# define        F_MAX_IN            ((F_MIN_IN)      << SHIFT_MAX)
# define        F_MAX_LOWER         ((F_MIN_LOWER)   << SHIFT_MAX)
# define        F_MAX_GREATER       ((F_MIN_GREATER) << SHIFT_MAX)
# define        C_INTER_LOWER       (F_MIN_LOWER   | F_MAX_LOWER  )
# define        C_INTER_CROSS_MIN   (F_MIN_LOWER   | F_MAX_IN     )
# define        C_INTER_IN          (F_MIN_IN      | F_MAX_IN     )
# define        C_INTER_CROSS_MAX   (F_MIN_IN      | F_MAX_GREATER)
# define        C_INTER_GREATER     (F_MIN_GREATER | F_MAX_GREATER)
# define        C_INTER_OUT         (F_MIN_LOWER   | F_MAX_GREATER)




  // ---------------------------------------------------------------
  // Interval class.

  struct CInter : pair<long, long> {

    // Supplemental methods.
    public: long  cmp (CInter &);

    // Friends.
    friend ostream &operator<< (ostream &o, const CInter &self);

  };




  // ---------------------------------------------------------------
  // Interval list class.

  struct LInter : public list<CInter> {

    // Attributes.
    long  bmin; // Lower bound of the whole list.
    long  bmax; // Upper bound of the whole list.

    // Methods.
    void  add (long, long);

    // Friends.
    friend ostream &operator<< (ostream &o, LInter &self);

  };




// -------------------------------------------------------------------
// Miscellaneous.


//template<class T> inline T min (T a, T b) { return ((a < b) ? a : b); }
//template<class T> inline T max (T a, T b) { return ((a > b) ? a : b); }
//template<class T> inline void swap (T a, T b) {
//                           T tmp;
//
//                           tmp = a; a = b; b = tmp;
//                         }




// -------------------------------------------------------------------
// Module  :  "nero.cpp".


  // ---------------------------------------------------------------
  // Functions.


  void emergency  (void);




#endif
