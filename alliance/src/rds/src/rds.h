/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rds210.h                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RDS_210_H
# define RDS_210_H

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             Alloc                           |
|                                                             |
\------------------------------------------------------------*/

# define RDS_ALLOC_BLOCK   0
# define RDS_ALLOC_HEAP    1

/*------------------------------------------------------------\
|                                                             |
|                         Allocation Macros                   |
|                                                             |
\------------------------------------------------------------*/

# define rdsallocblock( SizeOf )                      \
                                                      \
         ( rdsalloc( ( SizeOf ), RDS_ALLOC_BLOCK ) )

# define rdsallocheap( SizeOf )                       \
                                                      \
         ( rdsalloc( ( SizeOf ), RDS_ALLOC_HEAP ) )

# define rdsfreeblock( Pointer )                      \
                                                      \
         ( rdsfree( (char *)( Pointer ), (unsigned int)0 ) )

# define rdsfreeheap( Pointer, SizeOf )               \
                                                      \
         ( rdsfree( (char *)( Pointer ), (unsigned int)( SizeOf ) ) )

/*------------------------------------------------------------\
|                                                             |
|                         Instance Symmetry                   |
|                                                             |
\------------------------------------------------------------*/

# define RDS_NOSYM  0
# define RDS_ROT_P  1
# define RDS_SYMXY  2
# define RDS_ROT_M  3
# define RDS_SYM_X  4
# define RDS_SY_RM  5
# define RDS_SYM_Y  6
# define RDS_SY_RP  7

# define RDS_MAX_TRANSF ( RDS_SY_RP + 1 )

/*------------------------------------------------------------\
|                                                             |
|                          Rds Layers                         |
|                                                             |
\------------------------------------------------------------*/

# define RDS_LAYER_NWELL     0
# define RDS_LAYER_PWELL     1
# define RDS_LAYER_RADJ0     2
# define RDS_LAYER_RADJ1     3
# define RDS_LAYER_RADJ2     4
# define RDS_LAYER_RADJ3     5
# define RDS_LAYER_RADJ4     6
# define RDS_LAYER_LADJ0     7
# define RDS_LAYER_LADJ1     8
# define RDS_LAYER_LADJ2     9
# define RDS_LAYER_LADJ3     10
# define RDS_LAYER_LADJ4     11
# define RDS_LAYER_CADJ0     12
# define RDS_LAYER_CADJ1     13
# define RDS_LAYER_CADJ2     14
# define RDS_LAYER_CADJ3     15
# define RDS_LAYER_CADJ4     16
# define RDS_LAYER_NADJ0     17
# define RDS_LAYER_NADJ1     18
# define RDS_LAYER_NADJ2     19
# define RDS_LAYER_NADJ3     20
# define RDS_LAYER_NADJ4     21
# define RDS_LAYER_PADJ0     22
# define RDS_LAYER_PADJ1     23
# define RDS_LAYER_PADJ2     24
# define RDS_LAYER_PADJ3     25
# define RDS_LAYER_PADJ4     26
# define RDS_LAYER_TADJ0     27
# define RDS_LAYER_TADJ1     28
# define RDS_LAYER_TADJ2     29
# define RDS_LAYER_TADJ3     30
# define RDS_LAYER_TADJ4     31
# define RDS_LAYER_NISO      32
# define RDS_LAYER_SIPROT    33
# define RDS_LAYER_NIMP      34
# define RDS_LAYER_PIMP      35
# define RDS_LAYER_ACTIV     36
# define RDS_LAYER_NDIF      37
# define RDS_LAYER_PDIF      38
# define RDS_LAYER_NTIE      39
# define RDS_LAYER_PTIE      40
# define RDS_LAYER_POLY      41
# define RDS_LAYER_GATE      42
# define RDS_LAYER_VPOLY     43
# define RDS_LAYER_TPOLY     44
# define RDS_LAYER_POLY2     45
# define RDS_LAYER_CONT      46
# define RDS_LAYER_TPOLY2    47
# define RDS_LAYER_TCONT     48
# define RDS_LAYER_CONT2     49
# define RDS_LAYER_OBS0      50
# define RDS_LAYER_ALU1      51
# define RDS_LAYER_VALU1     52
# define RDS_LAYER_TALU1     53
# define RDS_LAYER_VIA1      54
# define RDS_LAYER_TVIA1     55
# define RDS_LAYER_OBS1      56
# define RDS_LAYER_ALU2      57
# define RDS_LAYER_VALU2     58
# define RDS_LAYER_TALU2     59
# define RDS_LAYER_VIA2      60
# define RDS_LAYER_TVIA2     61
# define RDS_LAYER_OBS2      62
# define RDS_LAYER_ALU3      63
# define RDS_LAYER_VALU3     64
# define RDS_LAYER_TALU3     65
# define RDS_LAYER_VIA3      66
# define RDS_LAYER_TVIA3     67
# define RDS_LAYER_OBS3      68
# define RDS_LAYER_ALU4      69
# define RDS_LAYER_VALU4     70
# define RDS_LAYER_TALU4     71
# define RDS_LAYER_VIA4      72
# define RDS_LAYER_TVIA4     73
# define RDS_LAYER_OBS4      74
# define RDS_LAYER_ALU5      75
# define RDS_LAYER_VALU5     76
# define RDS_LAYER_TALU5     77
# define RDS_LAYER_VIA5      78
# define RDS_LAYER_TVIA5     79
# define RDS_LAYER_OBS5      80
# define RDS_LAYER_ALU6      81
# define RDS_LAYER_VALU6     82
# define RDS_LAYER_TALU6     83
# define RDS_LAYER_VIA6      84
# define RDS_LAYER_TVIA6     85
# define RDS_LAYER_OBS6      86
# define RDS_LAYER_ALU7      87
# define RDS_LAYER_VALU7     88
# define RDS_LAYER_TALU7     89
# define RDS_LAYER_VIA7      90
# define RDS_LAYER_TVIA7     91
# define RDS_LAYER_OBS7      92
# define RDS_LAYER_ALU8      93
# define RDS_LAYER_VALU8     94
# define RDS_LAYER_TALU8     95
# define RDS_LAYER_VIA8      96
# define RDS_LAYER_TVIA8     97
# define RDS_LAYER_OBS8      98
# define RDS_LAYER_ALU9      99
# define RDS_LAYER_OBS9      100
# define RDS_LAYER_DEF0      101
# define RDS_LAYER_DEF1      102
# define RDS_LAYER_DEF2      103
# define RDS_LAYER_DEF3      104
# define RDS_LAYER_DEF4      105
# define RDS_LAYER_DEF5      106
# define RDS_LAYER_DEF6      107
# define RDS_LAYER_DEF7      108
# define RDS_LAYER_DEF8      109
# define RDS_LAYER_DEF9      110
# define RDS_LAYER_CBOTALU   111
# define RDS_LAYER_ALUCAP    112
# define RDS_LAYER_TOPCAP    113
# define RDS_LAYER_BOTCAP    114
# define RDS_LAYER_VIACAP    115
# define RDS_LAYER_CPAS      116
# define RDS_LAYER_REF       117
# define RDS_LAYER_USER0     118
# define RDS_LAYER_USER1     119
# define RDS_LAYER_USER2     120
# define RDS_LAYER_USER3     121
# define RDS_LAYER_USER4     122
# define RDS_LAYER_USER5     123
# define RDS_LAYER_USER6     124
# define RDS_LAYER_USER7     125
# define RDS_LAYER_USER8     126
# define RDS_LAYER_USER9     127
# define RDS_LAYER_ABOX      128

# define RDS_ALL_LAYER ( RDS_LAYER_ABOX + 1 )

# define RDS_LAYER_UNUSED  (unsigned char)255

/*------------------------------------------------------------\
|                                                             |
|                       Dynamic Layers                        |
|                                                             |
\------------------------------------------------------------*/

# define RDS_NWELL     RDS_DYNAMIC_LAYER[ RDS_LAYER_NWELL   ]
# define RDS_PWELL     RDS_DYNAMIC_LAYER[ RDS_LAYER_PWELL   ]
# define RDS_RADJ0     RDS_DYNAMIC_LAYER[ RDS_LAYER_RADJ0   ]
# define RDS_RADJ1     RDS_DYNAMIC_LAYER[ RDS_LAYER_RADJ1   ]
# define RDS_RADJ2     RDS_DYNAMIC_LAYER[ RDS_LAYER_RADJ2   ]
# define RDS_RADJ3     RDS_DYNAMIC_LAYER[ RDS_LAYER_RADJ3   ]
# define RDS_RADJ4     RDS_DYNAMIC_LAYER[ RDS_LAYER_RADJ4   ]
# define RDS_LADJ0     RDS_DYNAMIC_LAYER[ RDS_LAYER_LADJ0   ]
# define RDS_LADJ1     RDS_DYNAMIC_LAYER[ RDS_LAYER_LADJ1   ]
# define RDS_LADJ2     RDS_DYNAMIC_LAYER[ RDS_LAYER_LADJ2   ]
# define RDS_LADJ3     RDS_DYNAMIC_LAYER[ RDS_LAYER_LADJ3   ]
# define RDS_LADJ4     RDS_DYNAMIC_LAYER[ RDS_LAYER_LADJ4   ]
# define RDS_CADJ0     RDS_DYNAMIC_LAYER[ RDS_LAYER_CADJ0   ]
# define RDS_CADJ1     RDS_DYNAMIC_LAYER[ RDS_LAYER_CADJ1   ]
# define RDS_CADJ2     RDS_DYNAMIC_LAYER[ RDS_LAYER_CADJ2   ]
# define RDS_CADJ3     RDS_DYNAMIC_LAYER[ RDS_LAYER_CADJ3   ]
# define RDS_CADJ4     RDS_DYNAMIC_LAYER[ RDS_LAYER_CADJ4   ]
# define RDS_NADJ0     RDS_DYNAMIC_LAYER[ RDS_LAYER_NADJ0   ]
# define RDS_NADJ1     RDS_DYNAMIC_LAYER[ RDS_LAYER_NADJ1   ]
# define RDS_NADJ2     RDS_DYNAMIC_LAYER[ RDS_LAYER_NADJ2   ]
# define RDS_NADJ3     RDS_DYNAMIC_LAYER[ RDS_LAYER_NADJ3   ]
# define RDS_NADJ4     RDS_DYNAMIC_LAYER[ RDS_LAYER_NADJ4   ]
# define RDS_PADJ0     RDS_DYNAMIC_LAYER[ RDS_LAYER_PADJ0   ]
# define RDS_PADJ1     RDS_DYNAMIC_LAYER[ RDS_LAYER_PADJ1   ]
# define RDS_PADJ2     RDS_DYNAMIC_LAYER[ RDS_LAYER_PADJ2   ]
# define RDS_PADJ3     RDS_DYNAMIC_LAYER[ RDS_LAYER_PADJ3   ]
# define RDS_PADJ4     RDS_DYNAMIC_LAYER[ RDS_LAYER_PADJ4   ]
# define RDS_TADJ0     RDS_DYNAMIC_LAYER[ RDS_LAYER_TADJ0   ]
# define RDS_TADJ1     RDS_DYNAMIC_LAYER[ RDS_LAYER_TADJ1   ]
# define RDS_TADJ2     RDS_DYNAMIC_LAYER[ RDS_LAYER_TADJ2   ]
# define RDS_TADJ3     RDS_DYNAMIC_LAYER[ RDS_LAYER_TADJ3   ]
# define RDS_TADJ4     RDS_DYNAMIC_LAYER[ RDS_LAYER_TADJ4   ]
# define RDS_NISO      RDS_DYNAMIC_LAYER[ RDS_LAYER_NISO    ]
# define RDS_SIPROT    RDS_DYNAMIC_LAYER[ RDS_LAYER_SIPROT  ]
# define RDS_NIMP      RDS_DYNAMIC_LAYER[ RDS_LAYER_NIMP    ]
# define RDS_PIMP      RDS_DYNAMIC_LAYER[ RDS_LAYER_PIMP    ]
# define RDS_ACTIV     RDS_DYNAMIC_LAYER[ RDS_LAYER_ACTIV   ]
# define RDS_NDIF      RDS_DYNAMIC_LAYER[ RDS_LAYER_NDIF    ]
# define RDS_PDIF      RDS_DYNAMIC_LAYER[ RDS_LAYER_PDIF    ]
# define RDS_NTIE      RDS_DYNAMIC_LAYER[ RDS_LAYER_NTIE    ]
# define RDS_PTIE      RDS_DYNAMIC_LAYER[ RDS_LAYER_PTIE    ]
# define RDS_POLY      RDS_DYNAMIC_LAYER[ RDS_LAYER_POLY    ]
# define RDS_GATE      RDS_DYNAMIC_LAYER[ RDS_LAYER_GATE    ]
# define RDS_VPOLY     RDS_DYNAMIC_LAYER[ RDS_LAYER_VPOLY   ]
# define RDS_TPOLY     RDS_DYNAMIC_LAYER[ RDS_LAYER_TPOLY   ]
# define RDS_POLY2     RDS_DYNAMIC_LAYER[ RDS_LAYER_POLY2   ]
# define RDS_CONT      RDS_DYNAMIC_LAYER[ RDS_LAYER_CONT    ]
# define RDS_TPOLY2    RDS_DYNAMIC_LAYER[ RDS_LAYER_TPOLY2  ]
# define RDS_TCONT     RDS_DYNAMIC_LAYER[ RDS_LAYER_TCONT   ]
# define RDS_CONT2     RDS_DYNAMIC_LAYER[ RDS_LAYER_CONT2   ]
# define RDS_OBS0      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS0    ]
# define RDS_ALU1      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU1    ]
# define RDS_VALU1     RDS_DYNAMIC_LAYER[ RDS_LAYER_VALU1   ]
# define RDS_TALU1     RDS_DYNAMIC_LAYER[ RDS_LAYER_TALU1   ]
# define RDS_VIA1      RDS_DYNAMIC_LAYER[ RDS_LAYER_VIA1    ]
# define RDS_TVIA1     RDS_DYNAMIC_LAYER[ RDS_LAYER_TVIA1   ]
# define RDS_OBS1      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS1    ]
# define RDS_ALU2      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU2    ]
# define RDS_VALU2     RDS_DYNAMIC_LAYER[ RDS_LAYER_VALU2   ]
# define RDS_TALU2     RDS_DYNAMIC_LAYER[ RDS_LAYER_TALU2   ]
# define RDS_VIA2      RDS_DYNAMIC_LAYER[ RDS_LAYER_VIA2    ]
# define RDS_TVIA2     RDS_DYNAMIC_LAYER[ RDS_LAYER_TVIA2   ]
# define RDS_OBS2      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS2    ]
# define RDS_ALU3      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU3    ]
# define RDS_VALU3     RDS_DYNAMIC_LAYER[ RDS_LAYER_VALU3   ]
# define RDS_TALU3     RDS_DYNAMIC_LAYER[ RDS_LAYER_TALU3   ]
# define RDS_VIA3      RDS_DYNAMIC_LAYER[ RDS_LAYER_VIA3    ]
# define RDS_TVIA3     RDS_DYNAMIC_LAYER[ RDS_LAYER_TVIA3   ]
# define RDS_OBS3      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS3    ]
# define RDS_ALU4      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU4    ]
# define RDS_VALU4     RDS_DYNAMIC_LAYER[ RDS_LAYER_VALU4   ]
# define RDS_TALU4     RDS_DYNAMIC_LAYER[ RDS_LAYER_TALU4   ]
# define RDS_VIA4      RDS_DYNAMIC_LAYER[ RDS_LAYER_VIA4    ]
# define RDS_TVIA4     RDS_DYNAMIC_LAYER[ RDS_LAYER_TVIA4   ]
# define RDS_OBS4      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS4    ]
# define RDS_ALU5      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU5    ]
# define RDS_VALU5     RDS_DYNAMIC_LAYER[ RDS_LAYER_VALU5   ]
# define RDS_TALU5     RDS_DYNAMIC_LAYER[ RDS_LAYER_TALU5   ]
# define RDS_VIA5      RDS_DYNAMIC_LAYER[ RDS_LAYER_VIA5    ]
# define RDS_TVIA5     RDS_DYNAMIC_LAYER[ RDS_LAYER_TVIA5   ]
# define RDS_OBS5      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS5    ]
# define RDS_ALU6      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU6    ]
# define RDS_VALU6     RDS_DYNAMIC_LAYER[ RDS_LAYER_VALU6   ]
# define RDS_TALU6     RDS_DYNAMIC_LAYER[ RDS_LAYER_TALU6   ]
# define RDS_VIA6      RDS_DYNAMIC_LAYER[ RDS_LAYER_VIA6    ]
# define RDS_TVIA6     RDS_DYNAMIC_LAYER[ RDS_LAYER_TVIA6   ]
# define RDS_OBS6      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS6    ]
# define RDS_ALU7      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU7    ]
# define RDS_VALU7     RDS_DYNAMIC_LAYER[ RDS_LAYER_VALU7   ]
# define RDS_TALU7     RDS_DYNAMIC_LAYER[ RDS_LAYER_TALU7   ]
# define RDS_VIA7      RDS_DYNAMIC_LAYER[ RDS_LAYER_VIA7    ]
# define RDS_TVIA7     RDS_DYNAMIC_LAYER[ RDS_LAYER_TVIA7   ]
# define RDS_OBS7      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS7    ]
# define RDS_ALU8      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU8    ]
# define RDS_VALU8     RDS_DYNAMIC_LAYER[ RDS_LAYER_VALU8   ]
# define RDS_TALU8     RDS_DYNAMIC_LAYER[ RDS_LAYER_TALU8   ]
# define RDS_VIA8      RDS_DYNAMIC_LAYER[ RDS_LAYER_VIA8    ]
# define RDS_TVIA8     RDS_DYNAMIC_LAYER[ RDS_LAYER_TVIA8   ]
# define RDS_OBS8      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS8    ]
# define RDS_ALU9      RDS_DYNAMIC_LAYER[ RDS_LAYER_ALU9    ]
# define RDS_OBS9      RDS_DYNAMIC_LAYER[ RDS_LAYER_OBS9    ]
# define RDS_DEF0      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF0    ]
# define RDS_DEF1      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF1    ]
# define RDS_DEF2      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF2    ]
# define RDS_DEF3      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF3    ]
# define RDS_DEF4      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF4    ]
# define RDS_DEF5      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF5    ]
# define RDS_DEF6      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF6    ]
# define RDS_DEF7      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF7    ]
# define RDS_DEF8      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF8    ]
# define RDS_DEF9      RDS_DYNAMIC_LAYER[ RDS_LAYER_DEF9    ]
# define RDS_CBOTALU   RDS_DYNAMIC_LAYER[ RDS_LAYER_CBOTALU ]
# define RDS_ALUCAP    RDS_DYNAMIC_LAYER[ RDS_LAYER_ALUCAP  ]
# define RDS_TOPCAP    RDS_DYNAMIC_LAYER[ RDS_LAYER_TOPCAP  ]
# define RDS_BOTCAP    RDS_DYNAMIC_LAYER[ RDS_LAYER_BOTCAP  ]
# define RDS_VIACAP    RDS_DYNAMIC_LAYER[ RDS_LAYER_VIACAP  ]
# define RDS_CPAS      RDS_DYNAMIC_LAYER[ RDS_LAYER_CPAS    ]
# define RDS_REF       RDS_DYNAMIC_LAYER[ RDS_LAYER_REF     ]
# define RDS_USER0     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER0   ]
# define RDS_USER1     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER1   ]
# define RDS_USER2     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER2   ]
# define RDS_USER3     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER3   ]
# define RDS_USER4     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER4   ]
# define RDS_USER5     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER5   ]
# define RDS_USER6     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER6   ]
# define RDS_USER7     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER7   ]
# define RDS_USER8     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER8   ]
# define RDS_USER9     RDS_DYNAMIC_LAYER[ RDS_LAYER_USER9   ]
# define RDS_ABOX      RDS_DYNAMIC_LAYER[ RDS_LAYER_ABOX    ]

/*------------------------------------------------------------\
|                                                             |
|                             Layer                           |
|                                                             |
\------------------------------------------------------------*/

# define RDS_LAYER_MASK       (long)(0x000000FF)

/*------------------------------------------------------------\
|                                                             |
|                         Layer Macros                        |
|                                                             |
\------------------------------------------------------------*/

# define GetRdsLayer( R )                               \
                                                        \
  ((unsigned int)( ((R)->FLAGS) & RDS_LAYER_MASK )      )

# define SetRdsLayer( R, L )                            \
                                                        \
  ((R)->FLAGS = ((R)->FLAGS & ~RDS_LAYER_MASK) | (L)    )

/*------------------------------------------------------------\
|                                                             |
|                           Mbk Type                          |
|                                                             |
\------------------------------------------------------------*/

# define MBK_CONNECTOR_MASK   (long)(0x00000100)
# define MBK_SEGMENT_MASK     (long)(0x00000200)
# define MBK_REFERENCE_MASK   (long)(0x00000300)
# define MBK_VIA_MASK         (long)(0x00000400)
# define MBK_INSTANCE_MASK    (long)(0x00000500)
# define MBK_FIGURE_MASK      (long)(0x00000600)

# define RDS_MBK_TYPE_MASK    (long)(0x00000F00)

/*------------------------------------------------------------\
|                                                             |
|                          Mbk Type Macro                     |
|                                                             |
\------------------------------------------------------------*/

# define GetRdsMbkType( R )                              \
                                                         \
  ((R)->FLAGS & RDS_MBK_TYPE_MASK                        )

# define SetRdsMbkType( R, T )                           \
                                                         \
  ((R)->FLAGS = ( (R)->FLAGS & ~RDS_MBK_TYPE_MASK) | (T) )

/*------------------------------------------------------------\
|                                                             |
|                           Is Mbk Type                       |
|                                                             |
\------------------------------------------------------------*/

# define IsRdsConnector( R )                              \
                                                          \
  (((R)->FLAGS & RDS_MBK_TYPE_MASK) == MBK_CONNECTOR_MASK )

# define IsRdsSegment( R )                                \
                                                          \
  (((R)->FLAGS & RDS_MBK_TYPE_MASK) == MBK_SEGMENT_MASK   )

# define IsRdsReference( R )                              \
                                                          \
  (((R)->FLAGS & RDS_MBK_TYPE_MASK) == MBK_REFERENCE_MASK )

# define IsRdsVia( R )                                    \
                                                          \
  (((R)->FLAGS & RDS_MBK_TYPE_MASK) == MBK_VIA_MASK       )

# define IsRdsInstance( R )                               \
                                                          \
  (((R)->FLAGS & RDS_MBK_TYPE_MASK) == MBK_INSTANCE_MASK  )

# define IsRdsFigure( R )                                 \
                                                          \
  (((R)->FLAGS & RDS_MBK_TYPE_MASK) == MBK_FIGURE_MASK    )

/*------------------------------------------------------------\
|                                                             |
|                          Set Mbk Type                       |
|                                                             |
\------------------------------------------------------------*/

# define SetRdsConnector( R )                                 \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
        & ~RDS_MBK_TYPE_MASK ) | MBK_CONNECTOR_MASK           )

# define SetRdsSegment( R )                                   \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
        & ~RDS_MBK_TYPE_MASK ) | MBK_SEGMENT_MASK             )
      
# define SetRdsReference( R )                                 \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
        & ~RDS_MBK_TYPE_MASK ) | MBK_REFERENCE_MASK           )

# define SetRdsVia( R )                                       \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
        & ~RDS_MBK_TYPE_MASK ) | MBK_VIA_MASK                 )

# define SetRdsInstance( R )                                  \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
        & ~RDS_MBK_TYPE_MASK ) | MBK_INSTANCE_MASK            )

# define SetRdsFigure( R )                                    \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
        & ~RDS_MBK_TYPE_MASK ) | MBK_FIGURE_MASK              )

/*------------------------------------------------------------\
|                                                             |
|                           Extract Type                      |
|                                                             |
\------------------------------------------------------------*/

# define RDS_FIG_REC_MASK         (long)(0x00008000)

# define RDS_CON_EXTER_MASK       (long)(0x00001000)
# define RDS_CON_INTER_MASK       (long)(0x00002000)
# define RDS_REF_REF_MASK         (long)(0x00003000)
# define RDS_REF_CON_MASK         (long)(0x00004000)
# define RDS_TRANS_GATE_MASK      (long)(0x00005000)

# define RDS_EXTRACT_TYPE_MASK    (long)(0x00007000)

/*------------------------------------------------------------\
|                                                             |
|                      Extract Type Macros                    |
|                                                             |
\------------------------------------------------------------*/

# define GetRdsExtractType( R )                               \
                                                              \
   ( (R)->FLAGS & RDS_EXTRACT_TYPE_MASK                       )

# define SetRdsExtractType( R, E )                            \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
        & ~RDS_EXTRACT_TYPE_MASK ) | (E)                      )

/*------------------------------------------------------------\
|                                                             |
|                        Is Extract Type                      |
|                                                             |
\------------------------------------------------------------*/

# define IsRdsTransGate( R )                                  \
                                                              \
  (((R)->FLAGS & RDS_EXTRACT_TYPE_MASK) == RDS_TRANS_GATE_MASK)

# define IsRdsConExter( R )                                   \
                                                              \
  (((R)->FLAGS & RDS_EXTRACT_TYPE_MASK) == RDS_CON_EXTER_MASK )

# define IsRdsConInter( R )                                   \
                                                              \
  (((R)->FLAGS & RDS_EXTRACT_TYPE_MASK) == RDS_CON_INTER_MASK )

# define IsRdsRefRef( R )                                     \
                                                              \
  (((R)->FLAGS & RDS_EXTRACT_TYPE_MASK) == RDS_REF_REF_MASK   )
 
# define IsRdsRefCon( R )                                     \
                                                              \
  (((R)->FLAGS & RDS_EXTRACT_TYPE_MASK) == RDS_REF_CON_MASK   )

# define IsRdsFigRec( R )                                     \
                                                              \
  ((R)->FLAGS & RDS_FIG_REC_MASK                              )

/*------------------------------------------------------------\
|                                                             |
|                        Set Extract Type                     |
|                                                             |
\------------------------------------------------------------*/

# define SetRdsTransGate( R )                                 \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
     & ~RDS_EXTRACT_TYPE_MASK ) | RDS_TRANS_GATE_MASK         )

# define SetRdsConExter( R )                                  \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
     & ~RDS_EXTRACT_TYPE_MASK ) | RDS_CON_EXTER_MASK          )

# define SetRdsConInter( R )                                  \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
     & ~RDS_EXTRACT_TYPE_MASK ) | RDS_CON_INTER_MASK          )

# define SetRdsRefRef( R )                                    \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
     & ~RDS_EXTRACT_TYPE_MASK ) | RDS_REF_REF_MASK            )

# define SetRdsRefCon( R )                                    \
                                                              \
   ( (R)->FLAGS = ((R)->FLAGS                                 \
                                                              \
     & ~RDS_EXTRACT_TYPE_MASK ) | RDS_REF_CON_MASK            )

# define SetRdsFigRec( R )                                    \
                                                              \
   ( (R)->FLAGS |= RDS_FIG_REC_MASK                           )

# define ClearRdsFigRec( R )                                  \
                                                              \
   ( (R)->FLAGS &= ~RDS_FIG_REC_MASK                          )

/*------------------------------------------------------------\
|                                                             |
|                          Debug Macros                       |
|                                                             |
\------------------------------------------------------------*/

# define rdsbegin()                                           \
                                                              \
  do                                                          \
  {                                                           \
    if ( RDS_DEBUG_ON ) addrdsdebug(__LINE__, basename(__FILE__));       \
  }                                                           \
  while ( 0 )

# define rdsend()                                             \
                                                              \
  do                                                          \
  {                                                           \
    if ( RDS_DEBUG_ON ) delrdsdebug();                        \
  }                                                           \
  while ( 0 )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Rds Rectangle                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdsrec_list

  {
    struct rdsrec_list *NEXT;
    long                X;
    long                Y;
    long                DX;
    long                DY;
    long                FLAGS;
    char               *NAME;
    void               *USER;

  } rdsrec_list;

/*------------------------------------------------------------\
|                                                             |
|                          Rds Instance                       |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdsins_list

  {
    struct rdsins_list  *NEXT;
    char                *FIGNAME;
    char                *INSNAME;
    long                 X;
    long                 Y;
    char                 TRANSF;
    unsigned short       SIZE;
    char                 FLAGS;
    struct rdsrec_list **LAYERTAB;
    struct rdsrec_list **COURONNE;
    void                *USER;

  } rdsins_list;

/*------------------------------------------------------------\
|                                                             |
|                          Rds Figure                         |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdsfig_list

  {
    struct rdsfig_list   *NEXT;
    char                 *NAME;
    struct rdsrec_list  **LAYERTAB;
    struct rdsins_list   *INSTANCE;
    char                  MODE;
    unsigned short        SIZE;
    char                  FLAGS;
    char                 *FLAGTAB;
    struct rdsrec_list  **BBOXTAB;
    void                 *USER;

  } rdsfig_list;

/*------------------------------------------------------------\
|                                                             |
|                          Rds Debug                          |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rdsdebug_list
  {
    struct rdsdebug_list *NEXT;
    int                   LINE;
    char                 *NAME;

  } rdsdebug_list;
 
/*------------------------------------------------------------\
|                                                             |
|                      Global Variables                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Head List                           |
|                                                             |
\------------------------------------------------------------*/

  extern rdsfig_list   *HEAD_RDSFIG;
  extern rdsdebug_list *HEAD_RDSDEBUG;

/*------------------------------------------------------------\
|                                                             |
|                          Debug Mode                         |
|                                                             |
\------------------------------------------------------------*/

  extern char      RDS_DEBUG_ON;

/*------------------------------------------------------------\
|                                                             |
|                             Name                            |
|                                                             |
\------------------------------------------------------------*/

  extern char    **RDS_LAYER_NAME;
  extern char     *RDS_ALL_LAYER_NAME[ RDS_ALL_LAYER ];
  extern char     *RDS_TRANSF_NAME [ RDS_MAX_TRANSF ];

/*------------------------------------------------------------\
|                                                             |
|                       Dynamic Layers                        |
|                                                             |
\------------------------------------------------------------*/

  extern unsigned char  RDS_DYNAMIC_LAYER[ RDS_ALL_LAYER ];
  extern unsigned char *RDS_STATIC_LAYER;
  extern unsigned char  RDS_MAX_LAYER;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Alloc Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern          char * rdsalloc __P((unsigned long Size, char Heap));
  extern   rdsfig_list * allocrdsfig __P(());
  extern   rdsins_list * allocrdsins __P(());
  extern   rdsrec_list * allocrdsrec __P((unsigned long Size));
  extern rdsdebug_list * allocrdsdebug __P(());

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern           void  rdsfree __P((char *Pointer, unsigned int Size));
  extern           void  freerdsfig __P((rdsfig_list *Figure));
  extern           void  freerdsins __P((rdsins_list *Instance));
  extern           void  freerdsrec __P((rdsrec_list *Rectangle, unsigned int Size));
  extern           void  freerdsdebug __P((rdsdebug_list *Debug));

/*------------------------------------------------------------\
|                                                             |
|                          Add Functions                      |
|                                                             |
\------------------------------------------------------------*/

  extern   rdsrec_list * addrdsfigrec __P((rdsfig_list *Figure, char *Name, char Layer, long X, long Y, long Dx, long Dy));
  extern   rdsrec_list * addrdsinsrec __P((rdsins_list *Instance, char *Name, char Layer, long X, long Y, long Dx, long Dy));
  extern   rdsins_list * addrdsins __P((rdsfig_list *Figure, char *Model, char *Name, char Sym, long X, long Y));
  extern   rdsfig_list * addrdsfig __P((char *Name, unsigned short Size));
  extern           void  addrdsdebug __P((int Line, char *File));

/*------------------------------------------------------------\
|                                                             |
|                           Del Functions                     |
|                                                             |
\------------------------------------------------------------*/

  extern            int  delrdsfigrec __P((rdsfig_list *Figure, rdsrec_list *Rectangle));
  extern            int  delrdsinsrec __P((rdsins_list *Instance, rdsrec_list *Rectangle));
  extern            int  delrdsins __P((rdsfig_list *Figure, char *Name));
  extern            int  delrdsfig __P((char *Name));
  extern           void  delrdsdebug __P(());

/*------------------------------------------------------------\
|                                                             |
|                           View Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern           void  viewrdsrec __P((rdsrec_list *Rectangle));
  extern           void  viewrdsins __P((rdsins_list *Instance));
  extern           void  viewrdsfig __P((rdsfig_list *Figure));

/*------------------------------------------------------------\
|                                                             |
|                           Debug Functions                   |
|                                                             |
\------------------------------------------------------------*/

  extern           void  rdsdebug __P(());

# endif 
