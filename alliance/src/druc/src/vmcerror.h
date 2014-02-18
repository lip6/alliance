/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  VMCERROR.H                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcError Defines                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcError Macros                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcError variables                    |
|                                                             |
\------------------------------------------------------------*/
extern rdsfig_list *DrucFigureRdsErreur;
extern FILE        *DrucErrorFile;
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcError Fonctions                    |
|                                                             |
\------------------------------------------------------------*/
extern void DrucErreurUnCorps           ();
extern void DrucErreurDeuxCorps         ();
extern void DrucErreurOutFileRectangle  ();
extern void DrucErreurOutFileRegle      ();
extern void DrucErreurOutFileLayer      ();
extern void DrucErreurUnRectangle       ();
extern void DrucErreurCleanFigureErreur ();
extern void DrucOpenErrorFile           ();
extern void DrucCloseErrorFile          ();
