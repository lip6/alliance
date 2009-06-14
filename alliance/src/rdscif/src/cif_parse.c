/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSCIF                         |
|                                                             |
| File    :                 cif_parse.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   24/08/93                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <ctype.h>
# include <memory.h>
# include <math.h>

# include <mut.h>
# include <mph.h>
# include <rds.h>
# include <rpr.h>
# include <rfm.h>
# include <rwi.h>
# include <rut.h>
# include <rtl.h>

# include "cif.h"
# include "cif_error.h"
# include "cif_parse.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

static FILE        *Stream;
static char         CifNextChar;
static long         CifLineCounter;

static long         CifScaleA;
static long         CifScaleB;

static char         CifCurrentLayer;

static symbol_list *CifTopSymbol;
static rdsfig_list *CifCurrentFigure;
static symbol_list *CifCurrentSymbol;
static char        *CifInstanceName;
static long         CifInstanceCounter;

static symbol_list    *HEAD_SYMBOL[ CIF_MAX_SYMBOL_HEAD ];
static node_list      *HEAD_NODE      = (node_list      *)NULL; 
static connector_list *HEAD_CONNECTOR = (connector_list *)NULL; 

static char         CifBuffer[ CIF_MAX_BUFFER ];
static char         CifFirstSymbol = 1;
static char        *CifOboxKeyword = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Symbol Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Cif Add Symbol                      |
|                                                             |
\------------------------------------------------------------*/

static symbol_list *CifAddSymbol( Number )

    long  Number;
{
    symbol_list *Symbol;
    symbol_list *Scan;
    int          Index;

    Index = Number & CIF_MASK_SYMBOL;

    for ( Scan  = HEAD_SYMBOL[ Index ];
          Scan != (symbol_list *)NULL;
          Scan  = Scan->NEXT )
    {
      if ( Scan->NUMBER == Number ) return( Scan );
    }

    Symbol = (symbol_list *)rdsalloc( sizeof(symbol_list), RDS_ALLOC_HEAP );

    Symbol->NEXT         = HEAD_SYMBOL[ Index ];
    Symbol->NUMBER       = Number;
    HEAD_SYMBOL[ Index ] = Symbol;

    return( Symbol );
}

/*------------------------------------------------------------\
|                                                             |
|                        Cif Free Symbol                      |
|                                                             |
\------------------------------------------------------------*/

static void CifFreeSymbol()
{
    symbol_list *Scan;
    symbol_list *Del;
    int          Index;

    for ( Index = 0; Index < CIF_MAX_SYMBOL_HEAD; Index++ )
    {
      if ( ! CifFirstSymbol )
      {
        Scan = HEAD_SYMBOL[ Index ];

        while ( Scan != (symbol_list *)NULL )
        {
          Del  = Scan;
          Scan = Scan->NEXT;

          rdsfreeheap( Del, sizeof( symbol_list ) );
        }
      }
  
      HEAD_SYMBOL[ Index ] = (symbol_list *)NULL;
    }

    CifFirstSymbol = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                         Cif Search Symbol                   |
|                                                             |
\------------------------------------------------------------*/

static symbol_list *CifSearchSymbol( Number )

    long  Number;
{
    symbol_list *Symbol;
    int          Index;
   
    Index = Number & CIF_MASK_SYMBOL;

    for ( Symbol  = HEAD_SYMBOL[ Index ];
          Symbol != (symbol_list *)NULL;
          Symbol  = Symbol->NEXT )
    {
      if ( Symbol->NUMBER == Number )
      {
        return( Symbol );
      }
    }

    return( CifAddSymbol( Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Cif Del Symbol                      |
|                                                             |
\------------------------------------------------------------*/

static void CifDelSymbol( Number )

    long  Number;
{
    symbol_list *Symbol;
    int          Index;

    Index = Number & CIF_MASK_SYMBOL;

    for ( Symbol  = HEAD_SYMBOL[ Index ];
          Symbol != (symbol_list *)NULL;
          Symbol  = Symbol->NEXT )
    {
      if ( Symbol->NUMBER == Number )
      {
        if ( Symbol->MODEL != (rdsfig_list *)NULL )
        {
          delrdsfig( Symbol->MODEL->NAME );
        }

        Symbol->MODEL = (rdsfig_list *)NULL;
      }
    }
}

/*------------------------------------------------------------\
|                                                             |
|                        Node Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Cif Add Node                        |
|                                                             |
\------------------------------------------------------------*/

static node_list *CifAddNode( Name, X, Y, Layer )

    char *Name;
    long  X;
    long  Y;
    char  Layer;
{
    node_list *Node;

    Node = (node_list *)rdsalloc( sizeof(node_list), RDS_ALLOC_HEAP );

    Node->NEXT  = HEAD_NODE;
    Node->NAME  = Name;
    Node->X     = CIF_TO_RDS_UNIT( X );
    Node->Y     = CIF_TO_RDS_UNIT( Y );
    Node->LAYER = Layer;
    HEAD_NODE   = Node;

    return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                         Cif Free Node                       |
|                                                             |
\------------------------------------------------------------*/

static void CifFreeNode()
{
    node_list *Scan;
    node_list *Del;

    Scan = HEAD_NODE;

    while ( Scan != (node_list *)NULL )
    {
      Del  = Scan;
      Scan = Scan->NEXT;

      rdsfreeheap( Del, sizeof( node_list ) );
    }

    HEAD_NODE = (node_list *)NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                       Connector Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Cif Add Connector                    |
|                                                             |
\------------------------------------------------------------*/

static connector_list *CifAddConnector( Name, X, Y, Width, Layer )

    char *Name;
    long  X;
    long  Y;
    long  Width;
    char  Layer;
{
    connector_list *Connector;

    Connector = 

      (connector_list *)rdsalloc( sizeof(connector_list), RDS_ALLOC_HEAP );

    Connector->NEXT  = HEAD_CONNECTOR;
    Connector->NAME  = Name;
    Connector->X     = CIF_TO_RDS_UNIT( X );
    Connector->Y     = CIF_TO_RDS_UNIT( Y );
    Connector->WIDTH = CIF_TO_RDS_UNIT( Width );
    Connector->LAYER = Layer;
    HEAD_CONNECTOR   = Connector;

    return( Connector );
}

/*------------------------------------------------------------\
|                                                             |
|                       Cif Free Connector                    |
|                                                             |
\------------------------------------------------------------*/

static void CifFreeConnector()
{
    connector_list *Scan;
    connector_list *Del;

    Scan = HEAD_CONNECTOR;

    while ( Scan != (connector_list *)NULL )
    {
      Del  = Scan;
      Scan = Scan->NEXT;

      rdsfreeheap( Del, sizeof( connector_list ) );
    }

    HEAD_CONNECTOR = (connector_list *)NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                         Cif Get Rectangle                   |
|                                                             |
\------------------------------------------------------------*/

static rdsrec_list *CifGetRectangle( Figure, X, Y, Layer )

     rdsfig_list  *Figure;
     long          X;
     long          Y;
     char          Layer;
{
    rdsrec_list *Scan;

    for ( Scan  = Figure->LAYERTAB[ (int)Layer ];
          Scan != (rdsrec_list * )NULL;
          Scan  = Scan->NEXT)
    {
      if ( ( X >= Scan->X            ) &&  
           ( Y >= Scan->Y            ) &&  
           ( X <= Scan->X + Scan->DX ) &&
           ( Y <= Scan->Y + Scan->DY ) )
      {
        return Scan;
      }
    }

    return ( (rdsrec_list * )NULL );
}
 
/*------------------------------------------------------------\
|                                                             |
|                      Character Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         CifGetChar                          |
|                                                             |
\------------------------------------------------------------*/

static char CifGetChar()
{
    int ReadChar;

    ReadChar = CifNextChar;

    if ( ReadChar != EOF )
    {
      CifNextChar = fgetc( Stream );

      if ( CifNextChar == '\n' ) CifLineCounter++;
    }

    return( (char)ReadChar );
}

/*------------------------------------------------------------\
|                                                             |
|                       CifSkipBlank                          |
|                                                             |
\------------------------------------------------------------*/

static void CifSkipBlank()
{
    char  Found;

    Found = 0;
 
    while ( ! Found )
    {
      switch ( CifNextChar ) 
      {
        case '(' :
        case ')' :
        case ';' :
        case '-' :
        case EOF : 

          Found = 1;
          break;

        default  : 

          if ( isdigit( (int)CifNextChar ) || 
               isupper( (int)CifNextChar ) ) Found = 1;
          else
            CifGetChar();
      }
    }
}

/*------------------------------------------------------------\
|                                                             |
|                       CifSkipSeparator                      |
|                                                             |
\------------------------------------------------------------*/

static void CifSkipSeparator()
{
    char  Found;

    Found = 0;

    while ( ! Found )
    {

      switch ( CifNextChar )
      {
        case '(' :
        case ')' :
        case ';' :
        case '-' :
        case EOF :

          Found = 1;
          break;

        default  :

          if ( isdigit( (int)CifNextChar ) ) Found = 1;
          else                          CifGetChar();
      }
    }
}

/*------------------------------------------------------------\
|                                                             |
|                       CifSkipSpace                          |
|                                                             |
\------------------------------------------------------------*/

static void CifSkipSpace()
{
    while ( isspace( (int)CifNextChar ) )
    {
      CifGetChar();
    }
}

/*------------------------------------------------------------\
|                                                             |
|                       CifSkipUserText                       |
|                                                             |
\------------------------------------------------------------*/

static void CifSkipUserText()
{
    while ( ( CifNextChar != (char)EOF ) &&
            ( CifNextChar != ';'       ) )
    {
      CifGetChar();
    }
}

/*------------------------------------------------------------\
|                                                             |
|                       CifReadSemi                           |
|                                                             |
\------------------------------------------------------------*/

static void CifReadSemi()

{
    if ( ( CifNextChar != (char)EOF ) &&
         ( CifNextChar != ';'       ) )
    {
      ciferror( CIF_ERROR_EXPECTED_SEMI, NULL, CifLineCounter );
    }

    CifGetChar();
}

/*------------------------------------------------------------\
|                                                             |
|                       CifReadSpace                          |
|                                                             |
\------------------------------------------------------------*/

static void CifReadSpace()
{
    char Found;

    Found = 0;

    while ( isspace( (int)CifNextChar ) )
    {
      CifGetChar(); Found = 1;
    }

    if ( ! Found )
    {
      ciferror( CIF_ERROR_EXPECTED_SPACE, NULL, CifLineCounter );
    }
}

/*------------------------------------------------------------\
|                                                             |
|                       CifReadName                           |
|                                                             |
\------------------------------------------------------------*/

# if 0
static void CifReadName()
{
    char Found;

    Found = 0;

    while ( ( ! isspace( CifNextChar ) ) &&
            ( CifNextChar != '{'       ) &&
            ( CifNextChar != '}'       ) &&
            ( CifNextChar != (char)EOF ) &&
            ( CifNextChar != ';'       ) )
    {
      Found = 1; CifGetChar();
    }

    if ( ! Found )
    {
      ciferror( CIF_ERROR_EXPECTED_NAME, NULL, CifLineCounter );
    }
}
# endif

/*------------------------------------------------------------\
|                                                             |
|                       CifReadComment                        |
|                                                             |
\------------------------------------------------------------*/

 static void CifReadComment()
 {
   int Count;

   Count = 1;

   while ( Count ) 
   {
     switch ( CifGetChar() ) 
     {
       case '(' : Count = Count + 1;
       break;

       case ')' : Count = Count - 1;
       break;

       case EOF : 

        ciferror( CIF_ERROR_BAD_COMMENT, NULL, CifLineCounter );
    }
}
}


/*------------------------------------------------------------\
|                                                             |
|                         CifGetName                          |
|                                                             |
\------------------------------------------------------------*/

static char *CifGetName()
{
    char  Found;
    char *Name;

    Found = 0;
    Name  = CifBuffer;

    while ( ( ! isspace( (int)CifNextChar ) ) &&
            ( CifNextChar != (char)EOF ) &&
            ( CifNextChar != '{'       ) &&
            ( CifNextChar != '}'       ) &&
            ( CifNextChar != ';'       ) )
    {
      *Name++ = CifGetChar(); Found = 1;
    }

    *Name = '\0';

    if ( ! Found )
    {
      ciferror( CIF_ERROR_EXPECTED_NAME, NULL, CifLineCounter );
    }

    return( namealloc( CifBuffer ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       CifSkipOrient                         |
|                                                             |
\------------------------------------------------------------*/

static void CifSkipOrient()
{
    if ( CifNextChar == '{' )
    {
      while ( ( CifNextChar != '}'       ) &&
              ( CifNextChar != (char)EOF ) )
      {
        CifGetChar();
      }

      if ( CifNextChar == '}' ) CifGetChar();
    }
}

/*------------------------------------------------------------\
|                                                             |
|                           CifGetLayer                       |
|                                                             |
\------------------------------------------------------------*/

static char CifGetLayer()
{
    char  Found;
    char *Name;
    int   Layer;
    char *CifLayer;

    Found = 0;
    Name  = CifBuffer;

    while ( ( ( CifNextChar >= 'A' ) &&
              ( CifNextChar <= 'Z' ) ) ||
            ( isdigit( (int)CifNextChar   ) ) )
    {
      *Name++ = CifGetChar(); Found = 1;
    }

    *Name = '\0';

    if ( ! Found )
    {
      ciferror( CIF_ERROR_EXPECTED_LAYER, NULL, CifLineCounter );
    }

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      CifLayer = GET_CIF_LAYER( Layer );

      if ( CifLayer != RDS_CIF_LAYER_EMPTY )
      {
        if ( ! strcmp( CifLayer, CifBuffer ) ) 
        {
          return( Layer );
        }
      }
    }

    ciferror( CIF_ERROR_LAYER_UNKNOWN, NULL, CifLineCounter );

    return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        CifGetUnsigned                       |
|                                                             |
\------------------------------------------------------------*/

static unsigned long CifGetUnsigned()
{
    char          Error;
    unsigned long Value;

    Error = 1;
    Value = 0;

    CifSkipSeparator(); 

    while ( ( Value < CIF_MAX_UNSIGNED ) && 
            ( CifNextChar >= '0'       ) && 
            ( CifNextChar <= '9'       ) )
    {
      Value *= 10;
      Value += CifGetChar() - '0';
      Error  = 0;
    }

    if ( Error ) 
    {
      ciferror( CIF_ERROR_EXPECTED_UNSIGNED, NULL, CifLineCounter );
    }

    if ( ( CifNextChar >= '0' ) &&
         ( CifNextChar <= '9' ) )
    {
      ciferror( CIF_ERROR_UNSIGNED_OVERFLOW, NULL, CifLineCounter );
    }
   
    return( Value );
}

/*------------------------------------------------------------\
|                                                             |
|                        CifGetSigned                         |
|                                                             |
\------------------------------------------------------------*/

static unsigned long CifGetSigned()
{
    char          Error;
    char          Neg;
    unsigned long Value;

    Error = 1;
    Value = 0;

    CifSkipSeparator();

    Neg = ( CifNextChar == '-' );

    if ( Neg )
    {
      CifGetChar();
    }

    while ( ( Value < CIF_MAX_SIGNED ) &&
            ( CifNextChar >= '0'     ) &&
            ( CifNextChar <= '9'     ) )
    {
      Value *= 10;
      Value += CifGetChar() - '0';
      Error  = 0;
    }

    if ( Error )
    {
      ciferror( CIF_ERROR_EXPECTED_SIGNED, NULL, CifLineCounter );
    }

    if ( ( CifNextChar >= '0' ) &&
         ( CifNextChar <= '9' ) )
    {
      ciferror( CIF_ERROR_SIGNED_OVERFLOW, NULL, CifLineCounter );
    }

    if ( Neg ) Value = - Value;

    return( Value );
}

/*------------------------------------------------------------\
|                                                             |
|                        CifGetPoint                          |
|                                                             |
\------------------------------------------------------------*/

static void CifGetPoint( Point )

    coord_t *Point;
{
    Point->X = CifGetSigned();
    Point->Y = CifGetSigned();
}

/*------------------------------------------------------------\
|                                                             |
|                        CifGetPolygon                        |
|                                                             |
\------------------------------------------------------------*/

static void CifGetPolygon()
{
    long     FirstX;
    long     FirstY;
    int      Counter;
    int      NewCounter;
    coord_t *NewArray;
    coord_t  PointArray[ CIF_MAX_POINT ];

    Counter = 0;
    FirstX  = FirstY = 0;

    CifSkipSeparator();

    while ( ( ( CifNextChar >= '0' ) && 
              ( CifNextChar <= '9' ) ) ||
            ( CifNextChar == '-'     ) ) 
    {
      CifGetPoint( &PointArray[ Counter++ ] );

      if ( Counter == 1 )
      {
        FirstX = PointArray[0].X;
        FirstY = PointArray[0].Y;
      }

      if ( Counter  >= CIF_MAX_POINT )
      {
        ciferror( CIF_ERROR_POLYGON_OVERFLOW, NULL, CifLineCounter );
      }

      CifSkipSeparator();
    }

    if ( Counter == 0 )
    {
      ciferror( CIF_ERROR_POLYGON_EMPTY, NULL, CifLineCounter );
    }

    PointArray[ Counter   ].X = FirstX;
    PointArray[ Counter++ ].Y = FirstY;

    if ( ispolyrec( PointArray, Counter ) == 0 )
    {
      ciferror( CIF_ERROR_POLYGON_NOT_REC, NULL, CifLineCounter );
    }

    poly2diag( PointArray, Counter, 
               ( ((double)CifScaleA*RDS_UNIT) / (CifScaleB*CIF_UNIT) ),
               &NewArray, &NewCounter );

    diag2rec( CifCurrentFigure, CifCurrentLayer, NewArray, NewCounter );
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandPolygon                      |
|                                                             |
\------------------------------------------------------------*/

void CifCommandPolygon()
{
    CifGetPolygon();
}

/*------------------------------------------------------------\
|                                                             |
|                        CifCommandBox                        |
|                                                             |
\------------------------------------------------------------*/

void CifCommandBox()
{
    rdsrec_list *Rec;
    long         Swap;
    long         RotX;
    long         RotY;
    long         X;
    long         Y;
    long         Dx;
    long         Dy;
    coord_t      Center;

    Dx = CifGetUnsigned();
    Dy = CifGetUnsigned();

    CifGetPoint( &Center );

    CifSkipSeparator();

    if ( ( ( CifNextChar >= '0' ) &&
           ( CifNextChar <= '9' ) ) ||
         ( CifNextChar == '-'     ) )
    {
      RotX = CifGetSigned();
      RotY = CifGetSigned();

      if ( ( RotX != 0 ) &&
           ( RotY != 0 ) )
      {
        ciferror( CIF_ERROR_EXPECTED_MANHATTAN, NULL, CifLineCounter );
      }
    }
    else
    {
      RotY = 0;
    }

    if ( RotY != 0 )
    {
      Swap = Dx;
      Dx   = Dy;
      Dy   = Swap;
    }

    X  = CIF_TO_RDS_UNIT( Center.X - ( Dx >> 1 ) );
    Y  = CIF_TO_RDS_UNIT( Center.Y - ( Dy >> 1 ) );
    Dx = CIF_TO_RDS_UNIT( Dx );
    Dy = CIF_TO_RDS_UNIT( Dy );

    Rec = addrdsfigrec( CifCurrentFigure, NULL,
                        CifCurrentLayer, X, Y, Dx, Dy );

    if ( CheckPhysicalGridAligned( X  ) ||
         CheckPhysicalGridAligned( Y  ) ||
         CheckPhysicalGridAligned( Dx ) ||
         CheckPhysicalGridAligned( Dy ) )
    {
      cifwarning( CIF_WARNING_PHYSICAL_GRID, NULL, 0 );
      roundrdsrec( Rec );
    }
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandRoundFlash                   |
|                                                             |
\------------------------------------------------------------*/

void CifCommandRoundFlash()
{
    cifwarning( CIF_WARNING_ROUND_FLASH, NULL, CifLineCounter );

    CifGetUnsigned();

    CifGetSigned();
    CifGetSigned();
}

/*------------------------------------------------------------\
|                                                             |
|                         CifCommandWire                      |
|                                                             |
\------------------------------------------------------------*/
  
void CifCommandWire()
{
    CifGetUnsigned();
    CifGetPolygon();
}

/*------------------------------------------------------------\
|                                                             |
|                         CifCommandLayer                     |
|                                                             |
\------------------------------------------------------------*/

void CifCommandLayer()
{
    CifSkipBlank();
    CifCurrentLayer = CifGetLayer();
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandDefineStart                  |
|                                                             |
\------------------------------------------------------------*/

void CifCommandDefineStart()
{
    if ( CifCurrentSymbol != (symbol_list *)NULL )
    {
      ciferror( CIF_ERROR_SYMBOL_STARTED, NULL, CifLineCounter );
    }

    CifCurrentSymbol = CifAddSymbol( CifGetUnsigned() );

    CifSkipSeparator();

    if ( isdigit( (int)CifNextChar ) )
    {
      CifScaleA = CifGetUnsigned();
      CifScaleB = CifGetUnsigned();
    } 
    else
    {
      CifScaleA = 1;
      CifScaleB = 1;
    }
}

/*------------------------------------------------------------\
|                                                             |
|                    CifCommandDefineFinished                 |
|                                                             |
\------------------------------------------------------------*/

void CifCommandDefineFinished()
{
    node_list      *ScanNode;
    connector_list *ScanConnector;
    rdsfig_list    *Figure;
    rdsrec_list    *Rec;
    char            Reference;
    long            XAB1;
    long            XAB2;
    long            YAB1;
    long            YAB2;
    long            X;
    long            Y;
    long            Dx;
    long            Dy;

    if ( CifCurrentSymbol == (symbol_list *)NULL )
    {
      ciferror( CIF_ERROR_SYMBOL_NOT_STARTED, NULL, CifLineCounter );
    }

    Figure = CifCurrentSymbol->MODEL;

    if ( Figure == (rdsfig_list *)NULL )
    {
      ciferror( CIF_ERROR_BAD_SYMBOL, NULL, CifLineCounter );
    }

    CifScaleA = 1;
    CifScaleB = 1;

    for ( ScanNode  = HEAD_NODE; 
          ScanNode != (node_list *)NULL;
          ScanNode  = ScanNode->NEXT )
    {
      Rec = CifGetRectangle( Figure,
                             ScanNode->X, ScanNode->Y, 
                             ScanNode->LAYER );

      if ( Rec == (rdsrec_list *)NULL )
      {
        cifwarning( CIF_WARNING_UNCONNECTED_NODE, 
                    ScanNode->NAME, 
                    (long)CifCurrentSymbol->MODEL->NAME );
      }
      else Rec->NAME = ScanNode->NAME;
    }

    CifFreeNode();

    if ( HEAD_CONNECTOR != (connector_list *)NULL )
    {
      Rec = Figure->LAYERTAB[ RDS_ABOX ];

      if ( Rec != (rdsrec_list *)NULL )
      {
        XAB1 = Rec->X;
        XAB2 = XAB1 + Rec->DX;
        YAB1 = Rec->Y;
        YAB2 = YAB1 + Rec->DY;
      }
      else
      {
        XAB2 = XAB1 = HEAD_CONNECTOR->X;
        YAB2 = YAB1 = HEAD_CONNECTOR->Y;

        for ( ScanConnector  = HEAD_CONNECTOR->NEXT;
              ScanConnector != (connector_list *)NULL;
              ScanConnector  = ScanConnector->NEXT )
        {
          if ( ScanConnector->X < XAB1 ) XAB1 = ScanConnector->X;
          else
          if ( ScanConnector->X > XAB2 ) XAB2 = ScanConnector->X;

          if ( ScanConnector->Y < YAB1 ) YAB1 = ScanConnector->Y;
          else
          if ( ScanConnector->Y > YAB2 ) YAB2 = ScanConnector->Y;
        }

        if ( ( XAB1 < XAB2 ) && ( YAB1 < YAB2 ) )
        {
          Rec = addrdsfigrec( Figure, Figure->NAME, RDS_ABOX,
                              XAB1, YAB1, XAB2 - XAB1, YAB2 - YAB1 );

          SetRdsFigure( Rec );
        }
      }

      for ( ScanConnector  = HEAD_CONNECTOR;
            ScanConnector != (connector_list *)NULL;
            ScanConnector  = ScanConnector->NEXT )
      {
        Reference = 1;

        if ( ( ScanConnector->X == XAB1 ) ||
             ( ScanConnector->X == XAB2 ) )
        {
          if ( ( ScanConnector->Y >= YAB1 ) && 
               ( ScanConnector->Y <= YAB2 ) )
          {
            Reference = 0;
          }

          Dx = GET_S2R_LAYER_WIDTH( ScanConnector->LAYER );
          Dy = ScanConnector->WIDTH;
        }
        else
        if ( ( ScanConnector->Y == YAB1 ) ||
             ( ScanConnector->Y == YAB2 ) )
        {
          if ( ( ScanConnector->X >= XAB1 ) && 
               ( ScanConnector->X <= XAB2 ) )
          {
            Reference = 0;
          }

          Dy = GET_S2R_LAYER_WIDTH( ScanConnector->LAYER );
          Dx = ScanConnector->WIDTH;
        }
        else
        {
          Dy = Dx = GET_S2R_LAYER_WIDTH( ScanConnector->LAYER );
        }

        X = ScanConnector->X - ( Dx >> 1 );
        Y = ScanConnector->Y - ( Dy >> 1 );

        Rec = addrdsfigrec( CifCurrentFigure, ScanConnector->NAME,
                            ScanConnector->LAYER,
                            X, Y, Dx, Dy );

        if ( Reference ) 
        {
          SetRdsReference( Rec );
          SetRdsRefCon( Rec );
        }
        else
        {
          SetRdsConnector( Rec );
          SetRdsConExter( Rec );
        }

        if ( CheckPhysicalGridAligned( X  ) ||
             CheckPhysicalGridAligned( Y  ) ||
             CheckPhysicalGridAligned( Dx ) ||
             CheckPhysicalGridAligned( Dy ) )
        {
          cifwarning( CIF_WARNING_PHYSICAL_GRID, NULL, 0 );
          roundrdsrec( Rec );
        }
      }

      CifFreeConnector();
    }

    CifCurrentSymbol = (symbol_list *)NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                    CifCommandDefineDelete                   |
|                                                             |
\------------------------------------------------------------*/

void CifCommandDefineDelete()
{
    if ( CifCurrentSymbol != (symbol_list *)NULL )
    {
      ciferror( CIF_ERROR_SYMBOL_STARTED, NULL, CifLineCounter );
    }

    CifDelSymbol( CifGetUnsigned() );
}

/*------------------------------------------------------------\
|                                                             |
|                       CifCommandCall                        |
|                                                             |
\------------------------------------------------------------*/

void CifCommandCall()
{
    symbol_list *Symbol;
    rdsins_list *Instance;
    long         Number;
    long         RotX;
    long         RotY;
    long         Xins;
    long         Yins;
    int          Sym;
    int          Top;
    int          Continue;

    if ( CifInstanceName == (char *)NULL )
    {
      sprintf( CifBuffer, "magic_%ld", CifInstanceCounter++ );
      CifInstanceName = namealloc( CifBuffer );
    }

    Number = CifGetUnsigned();
    Top    = ( CifNextChar == ';' );

    CifSkipBlank();

    Symbol   = CifSearchSymbol( Number );
    Continue = 1;
    Sym      = RDS_NOSYM;
    Xins     = 0; 
    Yins     = 0;

    do
    {
      CifSkipBlank();
 
      switch( CifNextChar )
      {
        case 'T' :

          CifGetChar();

          Xins += CifGetSigned();
          Yins += CifGetSigned();

        break;

        case 'M' : 

          CifGetChar();
          CifSkipBlank();

          switch( CifGetChar() )
          {
            case 'X' : Sym = RDS_SYMMETRY_TABLE[ Sym ][ RDS_SYM_X ];
            break;

            case 'Y' : Sym = RDS_SYMMETRY_TABLE[ Sym ][ RDS_SYM_Y ];
            break;

            default  : ciferror( CIF_ERROR_BAD_MIRROR, NULL, CifLineCounter );
          }

        break;

        case 'R' :

          CifGetChar();

          RotX = CifGetSigned();
          RotY = CifGetSigned();

          if ( RotX < 0 )
          {
            if ( RotY == 0 ) Sym = RDS_SYMMETRY_TABLE[ Sym ][ RDS_SYMXY ];
          }
          else
          if ( RotX == 0 )
          {
            if ( RotY > 0 )  Sym = RDS_SYMMETRY_TABLE[ Sym ][ RDS_ROT_P ];
            else
            if ( RotY < 0 )  Sym = RDS_SYMMETRY_TABLE[ Sym ][ RDS_ROT_M ];
          }

        break;

        case ';' : Continue = 0;
        break;

        default  : 
 
         ciferror( CIF_ERROR_BAD_TRANS, CifInstanceName, CifLineCounter );
      }
    }
    while ( Continue );

    if ( ! Top )
    {
      Xins = CIF_TO_RDS_UNIT( Xins );
      Yins = CIF_TO_RDS_UNIT( Yins );

      if ( CheckPhysicalGridAligned( Xins ) ||
           CheckPhysicalGridAligned( Yins ) )
      {
        cifwarning( CIF_WARNING_PHYSICAL_GRID, NULL, 0 );

        Xins = RfmRoundLow( Xins );
        Yins = RfmRoundLow( Yins );
      }

      if ( CifInstanceName == (char *)NULL )
      {
        sprintf( CifBuffer, "magic_%ld", CifInstanceCounter++ );
        CifInstanceName = namealloc( CifBuffer );
      }

      Instance = addrdsins( CifCurrentFigure, NULL, CifInstanceName,
                            Sym, Xins, Yins );

      Instance->FIGNAME = (char *)Symbol;

      CifInstanceName = (char *)NULL;
    }
    else 
    {
      CifTopSymbol = Symbol;
    }
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandComment                      |
|                                                             |
\------------------------------------------------------------*/
 
void CifCommandComment()
{
    CifReadComment();
}

/*------------------------------------------------------------\
|                                                             |
|                        CifCommandEnd                        |
|                                                             |
\------------------------------------------------------------*/

void CifCommandEnd()
{
    CifSkipBlank();

    if ( CifCurrentSymbol != (symbol_list *)NULL )
    {
      ciferror( CIF_ERROR_SYMBOL_STARTED, NULL, CifLineCounter );
    }

    if ( CifNextChar != (char)EOF )
    {
      ciferror( CIF_ERROR_EXPECTED_EOF, NULL, CifLineCounter );
    }
}

/*------------------------------------------------------------\
|                                                             |
|                       CifCommandUser                        |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser()
{
    cifwarning( CIF_WARNING_USER_COMMAND, NULL, CifLineCounter );

    CifSkipUserText();
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandUser4I                       |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser4I()
{
    CifReadSpace();
    CifInstanceName = CifGetName();
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandUser91                       |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser91()
{
    CifReadSpace();
    CifInstanceName = CifGetName();
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandUser4N                       |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser4N()
{
    coord_t  Point;
    char    *Name;

    CifReadSpace();
    Name = CifGetName();
    CifReadSpace();
    CifGetPoint( &Point );

    CifAddNode( Name, Point.X, Point.Y, CifCurrentLayer );
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandUser94                       |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser94()
{
    rdsrec_list *Rec;
    char        *Name;
    coord_t      Center;
    long         Dx;
    long         Dy;
    char         Layer;

    if ( CifOboxKeyword == (char *)NULL ) 
    {
      CifOboxKeyword = namealloc( "obox" );
    }

    CifReadSpace();
    CifSkipOrient();
    Name = CifGetName();
    CifReadSpace();
    CifGetPoint( &Center );
    CifSkipSpace();

    if ( Name == CifOboxKeyword )
    {
      Dx  = CIF_TO_RDS_UNIT( Center.X << 1 );
      Dy  = CIF_TO_RDS_UNIT( Center.Y << 1 );

      Rec = addrdsfigrec( CifCurrentFigure, NULL,
                          RDS_ABOX, 0, 0, Dx, Dy );

      if ( CheckPhysicalGridAligned( Dx ) ||
           CheckPhysicalGridAligned( Dy ) )
      {
        cifwarning( CIF_WARNING_PHYSICAL_GRID, NULL, 0 );
        roundrdsrec( Rec );
      }
    }
    else
    {
      if ( CifNextChar != ';' )
      {
        Layer = CifGetLayer();
      }
      else
      {
        Layer = CifCurrentLayer;
      }

      CifAddNode( Name, Center.X, Center.Y, Layer );
    }
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandUser4A                       |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser4A()
{
    rdsrec_list *Rec;
    coord_t      First;
    coord_t      Second;
    long         X; 
    long         Y; 
    long         Dx; 
    long         Dy; 

    CifReadSpace();
    CifGetPoint( &First );
    X = CIF_TO_RDS_UNIT( First.X );
    Y = CIF_TO_RDS_UNIT( First.Y );
    CifGetPoint( &Second );
    Dx = CIF_TO_RDS_UNIT( Second.X - First.X );
    Dy = CIF_TO_RDS_UNIT( Second.Y - First.Y );

    Rec = addrdsfigrec( CifCurrentFigure, 
                        CifCurrentFigure->NAME, RDS_ABOX,
                        X, Y, Dx, Dy );
    SetRdsFigure( Rec );

    if ( CheckPhysicalGridAligned( X  ) ||
         CheckPhysicalGridAligned( Y  ) ||
         CheckPhysicalGridAligned( Dx ) ||
         CheckPhysicalGridAligned( Dy ) )
    {
      cifwarning( CIF_WARNING_PHYSICAL_GRID, NULL, 0 );
      roundrdsrec( Rec );
    }
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandUser4X                       |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser4X()
{
    char        *Name;
    long         Width;
    coord_t      Center;

    CifReadSpace();
    Name = CifGetName();
    CifReadSpace();
    CifGetUnsigned();
    CifReadSpace();
    CifGetPoint( &Center );
    CifReadSpace();
    Width = CifGetUnsigned();

    CifSkipUserText();

    CifAddConnector( Name, Center.X, Center.Y, Width, CifCurrentLayer );
}

/*------------------------------------------------------------\
|                                                             |
|                      CifCommandUser95                       |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser95()
{
    char        *Name;
    long         X;
    long         Y;
    long         Dx;
    long         Dy;
    char         Layer;
    coord_t      Center;
    rdsrec_list *Rec;

    CifReadSpace();
    CifSkipOrient();
    Name = CifGetName();
    CifReadSpace();
    Dx  = CifGetUnsigned();
    Dy  = CifGetUnsigned();
    CifReadSpace();
    CifGetPoint( &Center );
    CifSkipSpace();

    if ( CifNextChar != ';' )
    {
      Layer = CifGetLayer();
    }
    else
    {
      Layer = CifCurrentLayer;
    }

    X  = CIF_TO_RDS_UNIT( Center.X - ( Dx >> 1 ) );
    Y  = CIF_TO_RDS_UNIT( Center.Y - ( Dy >> 1 ) );
    Dx = CIF_TO_RDS_UNIT( Dx );
    Dy = CIF_TO_RDS_UNIT( Dy );

    Rec = addrdsfigrec( CifCurrentFigure, Name,
                        Layer, X, Y, Dx, Dy );

    if ( CheckPhysicalGridAligned( X  ) ||
         CheckPhysicalGridAligned( Y  ) ||
         CheckPhysicalGridAligned( Dx ) ||
         CheckPhysicalGridAligned( Dy ) )
    {
      cifwarning( CIF_WARNING_PHYSICAL_GRID, NULL, 0 );
      roundrdsrec( Rec );
    }

    SetRdsConnector( Rec );
    SetRdsConExter( Rec );
}

/*------------------------------------------------------------\
|                                                             |
|                       CifCommandUser9                       |
|                                                             |
\------------------------------------------------------------*/

void CifCommandUser9()
{
    if ( CifCurrentSymbol == (symbol_list *)NULL )
    {
      ciferror( CIF_ERROR_SYMBOL_NOT_STARTED, NULL, CifLineCounter );
    }

    CifReadSpace();

    CifCurrentFigure        = addrdsfig( CifGetName(), CifCurrentFigure->SIZE );
    CifCurrentSymbol->MODEL = CifCurrentFigure;
}
 
/*------------------------------------------------------------\
|                                                             |
|                      CifParseCommand                        |
|                                                             |
\------------------------------------------------------------*/

static void CifParseCommand( Figure )

    rdsfig_list *Figure;
{
    rdsfig_list *TopFigure;
    rdsfig_list *ScanFig;
    rdsins_list *ScanIns;
    symbol_list *Symbol;
    int          Layer;

    CifLineCounter     = 1;
    CifNextChar        = '\0';
    CifScaleA          = 1;
    CifScaleB          = 1;
    CifCurrentFigure   = Figure;
    CifInstanceName    = (char *)NULL;
    CifInstanceCounter = 0;
    CifTopSymbol       = (symbol_list *)NULL;
    CifCurrentSymbol   = (symbol_list *)NULL;

    CifFreeSymbol();
    CifFreeNode();

    do
    {
      CifSkipBlank();

      switch( CifGetChar() )
      {
        case 'P' : CifCommandPolygon();
        break;

        case 'B' : CifCommandBox( NULL );
        break;

        case 'R' : CifCommandRoundFlash();
        break;

        case 'W' : CifCommandWire();
        break;

        case 'L' : CifCommandLayer();
        break;

        case 'D' :

          CifSkipBlank();

          switch ( CifGetChar() ) 
          {
            case 'S': CifCommandDefineStart();
            break;

            case 'F' : CifCommandDefineFinished();
            break;

            case 'D' : CifCommandDefineDelete();
            break;

            default : ciferror( CIF_ERROR_BAD_DEFINE, NULL, CifLineCounter);
          }

        break;

        case 'C' : CifCommandCall();
        break;

        case '(' : CifCommandComment();
        break;

        case 'E' : CifCommandEnd();
        break;

        case ';' : 
        break;

        case EOF : ciferror( CIF_ERROR_UNEXPECTED_EOF, NULL, CifLineCounter );
        break;

        case '9' : 

           switch ( CifNextChar )
           {
             case '1' : CifGetChar(); CifCommandUser91();
             break;

             case '4' : CifGetChar(); CifCommandUser94();
             break;

             case '5' : CifGetChar(); CifCommandUser95();
             break;

             default  : CifCommandUser9();
           }

        break;

        case '4' : 

          switch ( CifGetChar() )
          {
            case 'I' : CifCommandUser4I();
            break;

            case 'N' : CifCommandUser4N();
            break;

            case 'A' : CifCommandUser4A();
            break;

            case 'X' : CifCommandUser4X();
            break;

            default  : ciferror( CIF_ERROR_BAD_COMMAND, NULL, CifLineCounter );
          }

        break;

        default  : ciferror( CIF_ERROR_BAD_COMMAND, NULL, CifLineCounter );
      }

      CifSkipSpace();
      CifReadSemi();
    }
    while ( CifNextChar != (char)EOF );

    if ( ( CifTopSymbol        == (symbol_list *)NULL ) ||
         ( CifTopSymbol->MODEL == (rdsfig_list *)NULL ) )
    {
      ciferror( CIF_ERROR_NO_TOP_MODEL, NULL, 0 );
    }

    TopFigure = CifTopSymbol->MODEL;

    for ( ScanFig  = HEAD_RDSFIG; 
          ScanFig != Figure; 
          ScanFig  = ScanFig->NEXT )
    {
      for ( ScanIns  = ScanFig->INSTANCE;
            ScanIns != (rdsins_list *)NULL;
            ScanIns  = ScanIns->NEXT )
      {
        Symbol = (symbol_list *)(ScanIns->FIGNAME);

        if ( Symbol->MODEL == (rdsfig_list *)NULL )
        {
          ciferror( CIF_ERROR_SYMBOL_NOT_DEFINE, NULL, Symbol->NUMBER );
        }

        ScanIns->FIGNAME = Symbol->MODEL->NAME;
      }
    }

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      Figure->LAYERTAB[ Layer ] = TopFigure->LAYERTAB[ Layer ];
      TopFigure->LAYERTAB[ Layer ] = (rdsrec_list *)NULL;
    }

    Figure->INSTANCE = TopFigure->INSTANCE;
    TopFigure->INSTANCE = (rdsins_list *)NULL;

    delrdsfig( TopFigure->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                        cifloadrdsfig                        |
|                                                             |
\------------------------------------------------------------*/

  rdsfig_list *cifloadrdsfig( Figure, Name, Mode )

    rdsfig_list *Figure;
    char        *Name;
    char         Mode;
{
    Figure->MODE = 'L';

    Stream = mbkfopen( Name, "cif", "r");

    if ( Stream == (FILE *)NULL )
    {
      ciferror( CIF_ERROR_FILE_OPEN, Name, 0 );
    }

    CifParseCommand( Figure );

    if ( fclose( Stream ) )
    {
      ciferror( CIF_ERROR_FILE_CLOSE, Name, 0 );
    }

    applyrdsmode( Figure, Mode );

    return( Figure );
}
