%{
#include "elp.h"

  int  yylex  ();
  void yyerror();
%}

%union {
       double elpdbl    ;
       char   elpstr[50];
       }

%token telpTECNAME telpTECVER telpLMLT telpWMLT telpDLN telpDLP telpDWN telpDWP
%token telpDLE telpDLD telpDWE telpDWD
%token telpVTN telpAN telpBN telpRNT telpVTP telpAP telpBP telpRPT
%token telp telpVDDmax telpVDDdeg telpVSSdeg telpVTHR
%token telpCGSN telpCGPN telpCGSP telpCGPP
%token telpCDSN telpCDPN telpCDWN telpCDSP telpCDPP telpCDWP
%token telpCSSN telpCSPN telpCSWN telpCSSP telpCSPP telpCSWP
%token telpCGDE telpCGSE telpCGDD telpCGSD
%token telpESIM telpMODEL telpLEVEL telpACM telpTEMP telpSLOPE
%token <elpdbl> telpDOUBLE
%token <elpstr> telpSTRING
%start elp

%%

elp       : telpTECNAME ':' telpSTRING telpTECVER ':' telpDOUBLE
                {
                strcpy(elpTechnoName,$3) ;
                elpTechnoVersion = $6 ;
                }
            variables
            ;

variables : /* empty */
          | variables elpvar
          ;

elpvar    :
            /* les parametres de shrink des transistors */
            telpLMLT '=' telpDOUBLE {elpShrink[elpLMLT] = $3 ;}
          | telpWMLT '=' telpDOUBLE {elpShrink[elpWMLT] = $3 ;}
          | telpDLN  '=' telpDOUBLE {elpShrink[elpDLN]  = $3 ;}
          | telpDWN  '=' telpDOUBLE {elpShrink[elpDWN]  = $3 ;}
          | telpDLP  '=' telpDOUBLE {elpShrink[elpDLP]  = $3 ;}
          | telpDWP  '=' telpDOUBLE {elpShrink[elpDWP]  = $3 ;}
          | telpDLE  '=' telpDOUBLE {elpShrink[elpDLE]  = $3 ;}
          | telpDWE  '=' telpDOUBLE {elpShrink[elpDWE]  = $3 ;}
          | telpDLD  '=' telpDOUBLE {elpShrink[elpDLD]  = $3 ;}
          | telpDWD  '=' telpDOUBLE {elpShrink[elpDWD]  = $3 ;}

            /* les caracteristiques des transistors */
          | telpVTN  '=' telpDOUBLE {elpModel[elpNMOS][elpVT] = $3 ;}
          | telpAN   '=' telpDOUBLE {elpModel[elpNMOS][elpA]  = $3 ;}
          | telpBN   '=' telpDOUBLE {elpModel[elpNMOS][elpB]  = $3 ;}
          | telpRNT  '=' telpDOUBLE {elpModel[elpNMOS][elpRT] = $3 ;}
          | telpVTP  '=' telpDOUBLE {elpModel[elpPMOS][elpVT] = $3 ;}
          | telpAP   '=' telpDOUBLE {elpModel[elpPMOS][elpA]  = $3 ;}
          | telpBP   '=' telpDOUBLE {elpModel[elpPMOS][elpB]  = $3 ;}
          | telpRPT  '=' telpDOUBLE {elpModel[elpPMOS][elpRT] = $3 ;}

            /* les tensions */
          | telpVDDmax '=' telpDOUBLE {elpVoltage[elpVDDMAX] = $3 ;}
          | telpVDDdeg '=' telpDOUBLE {elpVoltage[elpVDDDEG] = $3 ;}
          | telpVSSdeg '=' telpDOUBLE {elpVoltage[elpVSSDEG] = $3 ;}
          | telpVTHR   '=' telpDOUBLE {elpVoltage[elpVTHR]   = $3 ;}

            /* les capacites dynamiques */
          | telpCGSN   '=' telpDOUBLE {elpCapa[elpNMOS][elpCGS] = $3 ;}
          | telpCGPN   '=' telpDOUBLE {elpCapa[elpNMOS][elpCGP] = $3 ;}
          | telpCGSP   '=' telpDOUBLE {elpCapa[elpPMOS][elpCGS] = $3 ;}
          | telpCGPP   '=' telpDOUBLE {elpCapa[elpPMOS][elpCGP] = $3 ;}
          | telpCDSN   '=' telpDOUBLE {elpCapa[elpNMOS][elpCDS] = $3 ;}
          | telpCDPN   '=' telpDOUBLE {elpCapa[elpNMOS][elpCDP] = $3 ;}
          | telpCDWN   '=' telpDOUBLE {elpCapa[elpNMOS][elpCDW] = $3 ;}
          | telpCDSP   '=' telpDOUBLE {elpCapa[elpPMOS][elpCDS] = $3 ;}
          | telpCDPP   '=' telpDOUBLE {elpCapa[elpPMOS][elpCDP] = $3 ;}
          | telpCDWP   '=' telpDOUBLE {elpCapa[elpPMOS][elpCDW] = $3 ;}
          | telpCSSN   '=' telpDOUBLE {elpCapa[elpNMOS][elpCSS] = $3 ;}
          | telpCSPN   '=' telpDOUBLE {elpCapa[elpNMOS][elpCSP] = $3 ;}
          | telpCSWN   '=' telpDOUBLE {elpCapa[elpNMOS][elpCSW] = $3 ;}
          | telpCSSP   '=' telpDOUBLE {elpCapa[elpPMOS][elpCSS] = $3 ;}
          | telpCSPP   '=' telpDOUBLE {elpCapa[elpPMOS][elpCSP] = $3 ;}
          | telpCSWP   '=' telpDOUBLE {elpCapa[elpPMOS][elpCSW] = $3 ;}
          | telpCGDE   '=' telpDOUBLE {elpCapa[elpEMES][elpCSGD] = $3 ;}
          | telpCGSD   '=' telpDOUBLE {elpCapa[elpDMES][elpCSGS] = $3 ;}
          | telpCGDD   '=' telpDOUBLE {elpCapa[elpDMES][elpCSGD] = $3 ;}
          | telpCGSE   '=' telpDOUBLE {elpCapa[elpEMES][elpCSGS] = $3 ;}

            /* les parametres du simulateur electrique */
          | telpESIM   '=' telpSTRING {strcpy(elpEsimName,$3)       ;}
          | telpMODEL  '=' telpSTRING {strcpy(elpModelName,$3)      ;}
          | telpLEVEL  '=' telpDOUBLE {elpEsimLevel = $3            ;}

            /* les parametres generaux */
          | telpACM    '=' telpDOUBLE {elpGeneral[elpACM]   = $3 ;}
          | telpTEMP   '=' telpDOUBLE {elpGeneral[elpTEMP]  = $3 ;}
          | telpSLOPE  '=' telpDOUBLE {elpGeneral[elpSLOPE] = $3 ;}
          ;
%%

extern int  yylineno ;
extern char yytext[] ;

void yyerror()

{
elpError(1001,yytext,yylineno) ; /* si erreur de syntaxe => erreur 1001 */
}


int yywrap() {return 1 ;}

