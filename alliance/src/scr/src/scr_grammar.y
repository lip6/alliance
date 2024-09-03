%{
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
# include <mut.h>
# include <mlo.h> 
# include <mph.h> 
# include <apr.h>
#include "SCR_Type.h"

char	*con_name;
long	vect_begin;
long	vect_end;
long	vect_flag = 0;
char	con_orient;
long	con_order;
long	order_begin;
long	order_end;
char	buffer[256];
int yylex();
int yyerror();

extern PlaceConList     *LeadPlaceConList;
/******************************************************************************/
/*                              PROTOTYPES                                    */
/******************************************************************************/
CaracConList	*MakeCaracConList();
PlaceConList	*MakePlaceConList();
/******************************************************************************/



int tpllineno=1;
%}

%union
  {
  char  cara;
  long   valu;
  char  *text;
  }

%token <valu> M_PLACE_PHCON_NORTH
%token <valu> M_PLACE_PHCON_SOUTH
%token <valu> M_PLACE_PHCON_WEST
%token <valu> M_PLACE_PHCON_EAST
%token <valu> M_VER_FEED_THROUGH
%token <valu> M_HOR_FEED_THROUGH
%token <valu> Semicolon
%token <valu> Colon
%token <valu> Coma
%token <text> Identifier
%token <valu> Number
%token <valu> Lparen
%token <valu> Rparen
%token <valu> Lcro
%token <valu> Rcro

%start        placement

%%
placement
	: ldirective
	;

ldirective 
	:
	ldirective
	directive
	| directive
	;

directive :
	M_PLACE_PHCON_NORTH
		{ con_orient = 'N'; vect_flag = 0; }
	name
	order
        Semicolon
	| M_PLACE_PHCON_SOUTH
		{ con_orient = 'S'; vect_flag = 0; }
	name
	order
        Semicolon
	| M_PLACE_PHCON_WEST
		{ con_orient = 'W'; vect_flag = 0; }
	name
	order
        Semicolon
	| M_PLACE_PHCON_EAST
		{ con_orient = 'E'; vect_flag = 0; }
	name
	order
        Semicolon
	| M_VER_FEED_THROUGH
	Number
        Semicolon
		{ if (LeadPlaceConList == NULL) {
		   LeadPlaceConList = (PlaceConList *) mbkalloc (sizeof(PlaceConList));
		   LeadPlaceConList->NORTH_CON = NULL;
		   LeadPlaceConList->SOUTH_CON = NULL;
		   LeadPlaceConList->WEST_CON  = NULL;
		   LeadPlaceConList->EAST_CON  = NULL;
		   LeadPlaceConList->VER_FEED  = $2;
		   LeadPlaceConList->HOR_FEED  = 0;
 		  }
                  else LeadPlaceConList->VER_FEED  = $2;
		}
	| M_HOR_FEED_THROUGH
	Number
        Semicolon
		{ if (LeadPlaceConList == NULL) {
		   LeadPlaceConList = (PlaceConList *) mbkalloc (sizeof(PlaceConList));
		   LeadPlaceConList->NORTH_CON = NULL;
		   LeadPlaceConList->SOUTH_CON = NULL;
		   LeadPlaceConList->WEST_CON  = NULL;
		   LeadPlaceConList->EAST_CON  = NULL;
		   LeadPlaceConList->VER_FEED  = 0;
		   LeadPlaceConList->HOR_FEED  = $2;
 		  }
                  else LeadPlaceConList->HOR_FEED  = $2;
		}
	;

name 
	:
	Identifier
		{ con_name = $1; }
	| Identifier
	Lcro
	Number 
	Rcro
		{sprintf(buffer,"%s %ld", $1, $3); 
		 con_name = namealloc(buffer);
		}
	| Identifier
	Lparen
	Number 
	Rparen
		{sprintf(buffer,"%s %ld", $1, $3); 
		 con_name = namealloc(buffer);
		}
	| Identifier
	Lparen
	Number 
	Colon
	Number
	Rparen
		{ con_name = $1, vect_begin = $3; vect_end = $5; vect_flag = 1; }
	| Identifier
	Lcro
	Number 
	Colon
	Number
	Rcro
		{ con_name = $1, vect_begin = $3; vect_end = $5; vect_flag = 1; }
	;

order 
	:
	Number 
		{long j = 0;
                 con_order = $1; 
                 if ((vect_flag == 1) && (con_order == -1)) {
                  if (vect_begin < vect_end) {
                   for(j = vect_begin ; j <= vect_end ; j++) {
                    sprintf(buffer,"%s %ld",con_name,j);
                    LeadPlaceConList = MakePlaceConList(con_orient,buffer,con_order);
                   }
                  } 
                  else {
                   for(j = vect_begin ; j >= vect_end ; j--) {
                    sprintf(buffer,"%s %ld",con_name,j);
                    LeadPlaceConList = MakePlaceConList(con_orient,buffer,con_order);
                   }
                  } 
                  vect_flag = 0;
                 } 
                 else
		  LeadPlaceConList = MakePlaceConList(con_orient,con_name,con_order);
		}
	| Number 
	Colon
	Number 
		{long i = 0; long j = 0;
		 order_begin = $1; order_end = $3; 
		 if (LABS(order_begin,order_end) != LABS(vect_begin,vect_end))
		  yyerror();
		 else {
		  if (vect_begin < vect_end) {
		   if (order_begin < order_end) {
		    for(j = vect_begin, i = order_begin ; i <= order_end ; j++, i++) {
		     sprintf(buffer,"%s %ld",con_name,j);
		     LeadPlaceConList = MakePlaceConList(con_orient,buffer,i);
		    }
		   }
		   else {
		    for(j = vect_begin, i = order_begin ; i >= order_end ; j++, i--) {
		     sprintf(buffer,"%s %ld",con_name,j);
		     LeadPlaceConList = MakePlaceConList(con_orient,buffer,i);
		    }
		   }
		  }
		  else {
		   if (order_begin < order_end) {
		    for(j = vect_begin, i = order_begin ; i <= order_end ; j--, i++) {
		     sprintf(buffer,"%s %ld",con_name,j);
		     LeadPlaceConList = MakePlaceConList(con_orient,buffer,i);
		    }
		   }
		   else {
		    for(j = vect_begin, i = order_begin ; i >= order_end ; j--, i--) {
		     sprintf(buffer,"%s %ld",con_name,j);
		     LeadPlaceConList = MakePlaceConList(con_orient,buffer,i);
		    }
		   }
		  }
		 }
                }

	;

%%

int yyerror() 
{
 fprintf(stdout,"SYNTAX ERROR at line %d \n",tpllineno);
 exit(1);
}

int yywrap() {
	return 1;
}


/******************************************************************************/
/*      Function : MakeCaracConList()    le 17 juillet 1993                   */
/******************************************************************************/
CaracConList	*MakeCaracConList(LeadCaracConList, Name, OrderSlice)
CaracConList	*LeadCaracConList;
char		*Name;
long		OrderSlice;

{
 CaracConList	*ptNewPhCon = NULL;
 CaracConList	*CurPhCon   = NULL;
 CaracConList	*PrevPhCon  = NULL;

 ptNewPhCon = (CaracConList *) mbkalloc (sizeof(CaracConList));
 ptNewPhCon->NEXT = NULL;
 ptNewPhCon->NAME = namealloc(Name);
 ptNewPhCon->USER = OrderSlice;

 if (LeadCaracConList == NULL) return(ptNewPhCon) ;
 for(PrevPhCon = NULL, CurPhCon = LeadCaracConList; (CurPhCon && (OrderSlice > CurPhCon->USER));
     PrevPhCon = CurPhCon, CurPhCon = CurPhCon->NEXT);
 if (OrderSlice != -1) {
  if (CurPhCon && (OrderSlice == CurPhCon->USER)) {
   fprintf(stdout,"The connectors ( %s ) and ( %s ) have a same order\n", CurPhCon->NAME, Name);
   exit(1);
  }
 }
 if (PrevPhCon == NULL ) {
  ptNewPhCon->NEXT = LeadCaracConList;
  LeadCaracConList = ptNewPhCon;
 }
 else {
  ptNewPhCon->NEXT = CurPhCon;
  PrevPhCon->NEXT = ptNewPhCon;
 }
 return(LeadCaracConList);
}
 
/******************************************************************************/
/*      Function : MakePlaceConList()    le 17 juillet 1993                   */
/******************************************************************************/
PlaceConList	*MakePlaceConList(Side, ConName, OrderSlice)
char		Side;
char		*ConName;
long		OrderSlice;

{

 if (isvdd(ConName)) {
  fprintf(stdout,"scr_warning : The power connector %s is placed automatically by scr.\n",ConName);
  return(LeadPlaceConList);
 }
 else
  if (isvss(ConName)) {
   fprintf(stdout,"scr_warning : The ground connector %s is placed automatically by scr.\n",ConName);
   return(LeadPlaceConList);
  }

 if (LeadPlaceConList == NULL) {
  LeadPlaceConList = (PlaceConList *) mbkalloc (sizeof(PlaceConList));
  LeadPlaceConList->NORTH_CON = NULL;
  LeadPlaceConList->SOUTH_CON = NULL;
  LeadPlaceConList->WEST_CON  = NULL;
  LeadPlaceConList->EAST_CON  = NULL;
  LeadPlaceConList->VER_FEED  = 0;
  LeadPlaceConList->HOR_FEED  = 0;
 }
 if (Side == 'N')
  LeadPlaceConList->NORTH_CON = MakeCaracConList(LeadPlaceConList->NORTH_CON,ConName,OrderSlice);
 else
  if (Side == 'S')
   LeadPlaceConList->SOUTH_CON = MakeCaracConList(LeadPlaceConList->SOUTH_CON,ConName,OrderSlice);
  else
   if (Side == 'W')
    LeadPlaceConList->WEST_CON = MakeCaracConList(LeadPlaceConList->WEST_CON,ConName,OrderSlice);
   else
    if (Side == 'E')
     LeadPlaceConList->EAST_CON = MakeCaracConList(LeadPlaceConList->EAST_CON,ConName,OrderSlice);
 return(LeadPlaceConList);
}
/******************************************************************************/
