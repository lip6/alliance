
%option noinput
%option nounput

%{
#include <stdio.h>
#include <string.h>
#include "elp_y.h"
#undef ECHO
#define ECHO

extern double atof() ;
%}

dble [0-9.eE+-]+

%%
^#.*$                       {ECHO ;                     }
[ \t\n;]+                   {ECHO ;                     }
[=:]                        {ECHO ; return *yytext     ;}
[t,T]echnologie             {ECHO ; return telpTECNAME ;}
[v,V]ersion                 {ECHO ; return telpTECVER  ;}
LMLT                        {ECHO ; return telpLMLT    ;}
WMLT                        {ECHO ; return telpWMLT    ;}
DLN                         {ECHO ; return telpDLN     ;}
DWN                         {ECHO ; return telpDWN     ;}
DLP                         {ECHO ; return telpDLP     ;}
DWP                         {ECHO ; return telpDWP     ;}
DLE                         {ECHO ; return telpDLE     ;}
DWE                         {ECHO ; return telpDWE     ;}
DLD                         {ECHO ; return telpDLD     ;}
DWD                         {ECHO ; return telpDWD     ;}
VTN                         {ECHO ; return telpVTN     ;}
AN                          {ECHO ; return telpAN      ;}
BN                          {ECHO ; return telpBN      ;}
RNT                         {ECHO ; return telpRNT     ;}
VTP                         {ECHO ; return telpVTP     ;}
AP                          {ECHO ; return telpAP      ;}
BP                          {ECHO ; return telpBP      ;}
RPT                         {ECHO ; return telpRPT     ;}
VDDmax                      {ECHO ; return telpVDDmax  ;}
VDDdeg                      {ECHO ; return telpVDDdeg  ;}
VSSdeg                      {ECHO ; return telpVSSdeg  ;}
VTHR                        {ECHO ; return telpVTHR    ;}
CGSN                        {ECHO ; return telpCGSN    ;}
CGPN                        {ECHO ; return telpCGPN    ;}
CGSP                        {ECHO ; return telpCGSP    ;}
CGPP                        {ECHO ; return telpCGPP    ;}
CDSN                        {ECHO ; return telpCDSN    ;}
CDPN                        {ECHO ; return telpCDPN    ;}
CDWN                        {ECHO ; return telpCDWN    ;}
CDSP                        {ECHO ; return telpCDSP    ;}
CDPP                        {ECHO ; return telpCDPP    ;}
CDWP                        {ECHO ; return telpCDWP    ;}
CSSN                        {ECHO ; return telpCSSN    ;}
CSPN                        {ECHO ; return telpCSPN    ;}
CSWN                        {ECHO ; return telpCSWN    ;}
CSSP                        {ECHO ; return telpCSSP    ;}
CSPP                        {ECHO ; return telpCSPP    ;}
CSWP                        {ECHO ; return telpCSWP    ;}
CGDE                        {ECHO ; return telpCGDE    ;}
CGSE                        {ECHO ; return telpCGSE    ;}
CGDD                        {ECHO ; return telpCGDD    ;}
CGSD                        {ECHO ; return telpCGSD    ;}
ESIM                        {ECHO ; return telpESIM    ;}
MODEL                       {ECHO ; return telpMODEL   ;}
LEVEL                       {ECHO ; return telpLEVEL   ;}
ACM                         {ECHO ; return telpACM     ;}
TEMP                        {ECHO ; return telpTEMP    ;}
SLOPE                       {ECHO ; return telpSLOPE   ;}
{dble}                      {
                            ECHO ;
                            yylval.elpdbl = atof(yytext) ;
                            return telpDOUBLE ;
                            }
[a-zA-Z&_!?$%][^ =:\t\n()]* {
                            ECHO ;
                            strcpy(yylval.elpstr,yytext) ;
                            return telpSTRING ;
                            }

%%
