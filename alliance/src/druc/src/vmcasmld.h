/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  VMCASMLD.H                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcAsmLd variables                    |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_FLATTEN_RULES                                   0
#define DRUC_HIERARCHICAL_RULES                              1

/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcAsmLd Fonctions                    |
|                                                             |
\------------------------------------------------------------*/
extern int   DrucReadNextCodeAsm ();
extern int   DrucInitInstruction ();
extern int   DrucReadAsm         ();
extern void  DrucComputeCtm      ();
