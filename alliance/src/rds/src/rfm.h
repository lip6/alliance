/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   Rfm 212                         |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   01.08.93                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef RFM_212
# define RFM_212

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/*------------------------------------------------------------\
|                                                             |
|                       Special Macros                        |
|                                                             |
\------------------------------------------------------------*/

# define RfmRoundHigh(X)                                \
                                                        \
    (((X)>=0) ? ((((X) + ( RDS_PHYSICAL_GRID >> 1 ))    \
                       / RDS_PHYSICAL_GRID ) *          \
                         RDS_PHYSICAL_GRID )            \
              :  (((X) / RDS_PHYSICAL_GRID ) *          \
                         RDS_PHYSICAL_GRID )            )

# define RfmRoundLow(X)                                 \
                                                        \
    (((X)>=0) ? (((X) / RDS_PHYSICAL_GRID ) *           \
                        RDS_PHYSICAL_GRID )             \
              : ((((X) - ( RDS_PHYSICAL_GRID >> 1 ))    \
                       /  RDS_PHYSICAL_GRID ) *         \
                          RDS_PHYSICAL_GRID )           )

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern   rdsrec_list * segmbkrds __P((rdsfig_list *Figure, phseg_list *Segment, char Lynx));
  extern   rdsrec_list * viambkrds __P((rdsfig_list *Figure, phvia_list *Via, char Lynx));
  extern   rdsrec_list * refmbkrds __P((rdsfig_list *Figure, phref_list *Reference));
  extern   rdsrec_list * conmbkrds __P((rdsfig_list *Figure, phcon_list *Connector));
  extern   rdsrec_list * aboxmbkrds __P((phfig_list *FigureMbk, rdsfig_list *FigureRds));

  extern   rdsrec_list * insconmbkrds __P((phins_list *InstanceMbk, phfig_list *ModelMbk, rdsins_list *InstanceRds));
  extern   rdsrec_list * insviambkrds __P((phins_list *InstanceMbk, phfig_list *ModelMbk, rdsins_list *InstanceRds, char Lynx));
  extern   rdsrec_list * insrefmbkrds __P((phins_list *InstanceMbk, phfig_list *ModelMbk, rdsins_list *InstanceRds));
  extern   rdsrec_list * inssegmbkrds __P((phins_list *InstanceMbk, phfig_list *ModelMbk, rdsins_list *InstanceRds, char Lynx));

  extern   rdsins_list * insmbkrds __P((rdsfig_list *FigureRds, phins_list *InstanceMbk, char Mode, char Lynx));
  extern   rdsfig_list * figmbkrds __P((phfig_list *FigureMbk, unsigned int Size, char Lynx));
  extern   rdsfig_list * modelmbkrds __P((rdsfig_list *Figure, char Lynx));

  extern           void  viewrfmrec __P((rdsrec_list *Rectangle));
  extern           void  viewrfmseg __P((phseg_list *Segment, rdsrec_list *Rectangle));
  extern           void  viewrfmvia __P((phvia_list *Via, rdsrec_list *Rectangle));
  extern           void  viewrfmref __P((phref_list *Reference, rdsrec_list *Rectangle));
  extern           void  viewrfmcon __P((phcon_list *Connector, rdsrec_list *Rectangle));
  extern           void  viewrfmins __P((phins_list *InstanceMbk, rdsins_list *InstanceRds));
  extern           void  viewrfmfig __P((phfig_list *FigureMbk, rdsfig_list *FigureRds));
 
  extern           void  roundrdsrec __P((rdsrec_list *Rectangle));

# endif
