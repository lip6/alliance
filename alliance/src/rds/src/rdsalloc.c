/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rdsalloc.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <memory.h>
# include <string.h>

# include <mut.h>
# include "rds.h"

# include "rdsalloc.h"
# include "rdserror.h"

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
/*------------------------------------------------------------\
|                                                             |
|                           Head List                         |
|                                                             |
\------------------------------------------------------------*/

  rdsfig_list *HEAD_RDSFIG = (rdsfig_list *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                            Name                             |
|                                                             |
\------------------------------------------------------------*/

  char *RDS_ALL_LAYER_NAME[ RDS_ALL_LAYER ] =

     {
       "RDS_NWELL",
       "RDS_PWELL",
       "RDS_NIMP",
       "RDS_PIMP",
       "RDS_ACTIV",
       "RDS_NDIF",
       "RDS_PDIF",
       "RDS_NTIE",
       "RDS_PTIE",
       "RDS_POLY",
       "RDS_GATE",
       "RDS_VPOLY",
       "RDS_TPOLY",
       "RDS_POLY2",
       "RDS_CONT",
       "RDS_TPOLY2",
       "RDS_TCONT",
       "RDS_CONT2",
       "RDS_ALU1",
       "RDS_VALU1",
       "RDS_TALU1",
       "RDS_VIA1",
       "RDS_TVIA1",
       "RDS_ALU2",
       "RDS_VALU2",
       "RDS_TALU2",
       "RDS_VIA2",
       "RDS_TVIA2",
       "RDS_ALU3",
       "RDS_VALU3",
       "RDS_TALU3",
       "RDS_VIA3",
       "RDS_TVIA3",
       "RDS_ALU4",
       "RDS_VALU4",
       "RDS_TALU4",
       "RDS_VIA4",
       "RDS_TVIA4",
       "RDS_ALU5",
       "RDS_VALU5",
       "RDS_TALU5",
       "RDS_VIA5",
       "RDS_TVIA5",
       "RDS_ALU6",
       "RDS_VALU6",
       "RDS_TALU6",
       "RDS_VIA6",
       "RDS_TVIA6",
       "RDS_ALU7",
       "RDS_VALU7",
       "RDS_TALU7",
       "RDS_VIA7",
       "RDS_TVIA7",
       "RDS_ALU8",
       "RDS_VALU8",
       "RDS_TALU8",
       "RDS_VIA8",
       "RDS_TVIA8",
       "RDS_ALU9",
       "RDS_CPAS",
       "RDS_REF",
       "RDS_USER0",
       "RDS_USER1",
       "RDS_USER2",
       "RDS_USER3",
       "RDS_USER4",
       "RDS_USER5",
       "RDS_USER6",
       "RDS_USER7",
       "RDS_USER8",
       "RDS_USER9",
       "RDS_ABOX"
     };

  char *RDS_TRANSF_NAME [ RDS_MAX_TRANSF ] = 
 
     {
       "NOSYM",
       "ROT_P", 
       "SYMXY",
       "ROT_M",
       "SYM_X",
       "SY_RM",
       "SYM_Y",
       "SY_RP"
     };

  char **RDS_LAYER_NAME = RDS_ALL_LAYER_NAME;

/*------------------------------------------------------------\
|                                                             |
|                          Dynamic Layers                     |
|                                                             |
\------------------------------------------------------------*/

  unsigned char RDS_DYNAMIC_LAYER[ RDS_ALL_LAYER ] =

    {
      RDS_LAYER_NWELL,
      RDS_LAYER_PWELL,
      RDS_LAYER_NIMP ,
      RDS_LAYER_PIMP ,
      RDS_LAYER_ACTIV,
      RDS_LAYER_NDIF ,
      RDS_LAYER_PDIF ,
      RDS_LAYER_NTIE ,
      RDS_LAYER_PTIE ,
      RDS_LAYER_POLY ,
      RDS_LAYER_GATE ,
      RDS_LAYER_VPOLY,
      RDS_LAYER_TPOLY,
      RDS_LAYER_POLY2,
      RDS_LAYER_CONT ,
      RDS_LAYER_TPOLY2,
      RDS_LAYER_TCONT,
      RDS_LAYER_CONT2,
      RDS_LAYER_ALU1 ,
      RDS_LAYER_VALU1,
      RDS_LAYER_TALU1,
      RDS_LAYER_VIA1 ,
      RDS_LAYER_TVIA1,
      RDS_LAYER_ALU2 ,
      RDS_LAYER_VALU2,
      RDS_LAYER_TALU2,
      RDS_LAYER_VIA2 ,
      RDS_LAYER_TVIA2,
      RDS_LAYER_ALU3 ,
      RDS_LAYER_VALU3,
      RDS_LAYER_TALU3,
      RDS_LAYER_VIA3 ,
      RDS_LAYER_TVIA3,
      RDS_LAYER_ALU4 ,
      RDS_LAYER_VALU4,
      RDS_LAYER_TALU4,
      RDS_LAYER_VIA4 ,
      RDS_LAYER_TVIA4,
      RDS_LAYER_ALU5 ,
      RDS_LAYER_VALU5,
      RDS_LAYER_TALU5,
      RDS_LAYER_VIA5 ,
      RDS_LAYER_TVIA5,
      RDS_LAYER_ALU6 ,
      RDS_LAYER_VALU6,
      RDS_LAYER_TALU6,
      RDS_LAYER_VIA6 ,
      RDS_LAYER_TVIA6,
      RDS_LAYER_ALU7 ,
      RDS_LAYER_VALU7,
      RDS_LAYER_TALU7,
      RDS_LAYER_VIA7 ,
      RDS_LAYER_TVIA7,
      RDS_LAYER_ALU8 ,
      RDS_LAYER_VALU8,
      RDS_LAYER_TALU8,
      RDS_LAYER_VIA8 ,
      RDS_LAYER_TVIA8,
      RDS_LAYER_ALU9 ,
      RDS_LAYER_CPAS ,
      RDS_LAYER_REF  ,
      RDS_LAYER_USER0,
      RDS_LAYER_USER1,
      RDS_LAYER_USER2,
      RDS_LAYER_USER3,
      RDS_LAYER_USER4,
      RDS_LAYER_USER5,
      RDS_LAYER_USER6,
      RDS_LAYER_USER7,
      RDS_LAYER_USER8,
      RDS_LAYER_USER9,
      RDS_LAYER_ABOX
    };

  unsigned char *RDS_STATIC_LAYER = RDS_DYNAMIC_LAYER;
  unsigned char  RDS_MAX_LAYER = RDS_ALL_LAYER;

/*------------------------------------------------------------\
|                                                             |
|                         Alloc Heap                          |
|                                                             |
\------------------------------------------------------------*/

         char  *RdsHeapHeadArray[ 256 ];
  static char   InitRdsHeapHeadArray = 1;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Rds Alloc                          |
|                                                             |
\------------------------------------------------------------*/

char *rdsalloc( Size, Heap )

      unsigned long Size; 
      char          Heap;
{
  long          Counter;
  unsigned long Index;
  char         *Pointer;
  char        **Prev;
  char         *Scan;

  if ( InitRdsHeapHeadArray == 1 )
  { 
    InitRdsHeapHeadArray = 0;

    for ( Counter = 0; Counter < 256 ; Counter++ )
    {
      RdsHeapHeadArray[ Counter ] = (char *)NULL;
    }
  }

  Index = ( Size + 3 ) >> 2;

  if ( ( Heap    == RDS_ALLOC_BLOCK ) ||
       ( Index   >  255             ) )
  {
    if ( !( Pointer = malloc( Size ) ) ) 
    {
      rdserror( RDS_ALLOC_ERROR, (char *)NULL );
    }

    bzero( Pointer, Size );
  
    return( Pointer );
  } 

  Size = Index << 2;

  if ( RdsHeapHeadArray[ Index ] == (char *)NULL )
  {
    if ( !( Pointer = malloc( Size << RDS_ALLOC_SHIFT ) ) ) 
    {
      rdserror( RDS_ALLOC_ERROR, (char *)NULL );
    }

    Scan = (char *)(Pointer + Size);

    Prev = &RdsHeapHeadArray[ Index ];

    for ( Counter = 1; Counter < RDS_ALLOC_SIZE; Counter++ )
    {
      *Prev = Scan; Prev = (char **)Scan; Scan = (char *)(Scan + Size);
    }

    *Prev = (char *)NULL;

    RdsHeapHeadArray[ Index ] = (char *)(Pointer + Size);

    bzero( Pointer, Size );

    return( Pointer );
  }

  Pointer = RdsHeapHeadArray[ Index ];

  RdsHeapHeadArray[ Index ] = *((char **)RdsHeapHeadArray[ Index ]);

  bzero( Pointer, Size );

  return( Pointer );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rds Alloc Figure                     |
|                                                             |
\------------------------------------------------------------*/
 
rdsfig_list *allocrdsfig()
{
  rdsfig_list *Figure;
  void        *Pointer;

  Figure  = (rdsfig_list *)rdsallocblock( sizeof( rdsfig_list) );
  Pointer = (void *)rdsallocblock( sizeof(rdsrec_list *) * RDS_MAX_LAYER );
  Figure->LAYERTAB = (rdsrec_list **)Pointer;
  Pointer = (void *)rdsallocblock( sizeof(rdsrec_list *) * RDS_MAX_LAYER );
  Figure->BBOXTAB  = (rdsrec_list **)Pointer;
  Pointer = (void *)rdsallocblock( sizeof( char ) * RDS_MAX_LAYER );
  Figure->FLAGTAB  = (char *)Pointer;

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rds Alloc Instance                    |
|                                                             |
\------------------------------------------------------------*/

rdsins_list *allocrdsins()
{
  rdsins_list *Instance;
  void        *Pointer;

  Instance = (rdsins_list *)rdsallocblock( sizeof( rdsins_list ) );
  Pointer = (void *)rdsallocblock( sizeof(rdsrec_list *) * RDS_MAX_LAYER );
  Instance->LAYERTAB = (rdsrec_list **)Pointer;
  Pointer = (void *)rdsallocblock( sizeof(rdsrec_list *) * RDS_MAX_LAYER );
  Instance->COURONNE = (rdsrec_list **)Pointer;

  return( Instance );
}

/*------------------------------------------------------------\
|                                                             |
|                     Rds Alloc Rectangle                     |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *allocrdsrec( Size )

   unsigned long Size;
{
  return( (rdsrec_list *)( rdsallocheap( sizeof( rdsrec_list ) + Size ) ) );
}
