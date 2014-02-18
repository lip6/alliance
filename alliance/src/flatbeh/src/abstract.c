/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <string.h>
#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include <abl.h>
#include <abe.h>
#include <abv.h>
#include <aut.h>
#include "abstract.h"
#include "utils.h"

/*------------------------------------------------------------------------------
create	 	: Regenere un ABL en substituant les variables formelles
                  par les signaux de la lofig
--------------------------------------------------------------------------------
parametres  	: en ABL, un modele d'instance et une table de hasch.
--------------------------------------------------------------------------------
return		: un ABL.
------------------------------------------------------------------------------*/
chain_list *create(expr, model, tableTH)
chain_list *expr;
loins_list *model;
authtable* tableTH;
{
chain_list *ret;
locon_list *con;

if (ABL_ATOM(expr))
   {
   if (ABL_ATOM_VALUE(expr) == namealloc("'1'"))
      return createablatom("'1'");

   if (ABL_ATOM_VALUE(expr) == namealloc("'0'"))
      return createablatom("'0'");

   for(con = model->LOCON; (con && (ABL_ATOM_VALUE(expr) != con->NAME));
       con = con->NEXT);
   if (con)
      {
	/* variable de expr est un connecteur de l'interface */
      chain_list *lstCon;
      chain_list *retour = (chain_list *)NULL;
      ptype_list *liste = getptype(con->SIG->USER, (long)LOFIGCHAIN);

      if (!liste)
         {
         printf("ERROR : No lofigchain for %s of %s\n", con->NAME, model->FIGNAME);
         exit(-1);
         }

      if (!(((chain_list *)liste->DATA)->NEXT))
         {
         printf("ERROR : Nothing connected to %s (Index %ld-TYPE %c) of %s\n\n",
                con->NAME, con->SIG->INDEX, con->SIG->TYPE, model->INSNAME);
         printf("Please verify your CATAL file !\n\n");
         exit(-1);
         }

      if (con->SIG->TYPE == EXTERNAL)
         {
         for(lstCon = (chain_list *)liste->DATA; lstCon; lstCon = lstCon->NEXT)
            {
            locon_list *conAux = (locon_list *)lstCon->DATA;

            if (conAux->ROOT != (void *)model)
	       {
               if ((conAux->TYPE == EXTERNAL) && (conAux->DIRECTION == IN || conAux->DIRECTION == INOUT))
                  retour = createablatom(conAux->NAME);
               else
                  if ((conAux->TYPE == INTERNAL) &&
                      ((conAux->DIRECTION == OUT) ||
                       (conAux->DIRECTION == INOUT) ||
                       (conAux->DIRECTION == TRISTATE) ||
                       (conAux->DIRECTION == TRANSCV)))
                     {
                     char * nameAtom = mbkalloc(strlen(getsigname(con->SIG)) + 4);
                     sprintf(nameAtom, "%s", getsigname(con->SIG));
                     retour = createablatom(nameAtom);
                     }
               }
            }
         }
      else
         {
         for(lstCon = (chain_list *)liste->DATA; lstCon; lstCon = lstCon->NEXT)
            {
            locon_list *conAux = (locon_list *)lstCon->DATA;
		/*printf(" %s    %c\n",conAux->NAME,conAux->DIRECTION);*/

            if (conAux->ROOT != (void *)model)
               {
               if ((conAux->DIRECTION == OUT) ||
                   (conAux->DIRECTION == INOUT) ||
                   (conAux->DIRECTION == TRISTATE) ||
                   (conAux->DIRECTION == TRANSCV))
                  {
                  char * nameAtom = mbkalloc(strlen(getsigname(con->SIG)) + 4); 

                  sprintf(nameAtom, "%s", getsigname(con->SIG));
                  retour = createablatom(nameAtom);
                  }
               }
	     else if ((conAux->DIRECTION == INOUT) || (conAux->DIRECTION == OUT))
		     {
			char * nameAtom = mbkalloc(strlen(getsigname(con->SIG)) + 4);
                  	sprintf(nameAtom, "%s", getsigname(con->SIG));
                  	retour = createablatom(nameAtom);

			}
            }
         }
      if (!retour)
         {
         printf("ERROR : nothing for connector %s of %s [%s]\n",
                con->NAME, model->INSNAME, model->FIGNAME);
         exit(-1);
         }
      return retour;
      }
   else
      {
	  /* ce n'est pas un connecteur de l'interface */
      authelem *val;

      if ((val = searchauthelem(tableTH, ABL_ATOM_VALUE(expr))) != NULL)
         return createablatom((char *)val->VALUE);
      else
         {
         char *auxName = (char *)mbkalloc(strlen(model->INSNAME) +
                                          strlen(ABL_ATOM_VALUE(expr)) + 2);
         sprintf(auxName, "%s_%s", model->INSNAME, ABL_ATOM_VALUE(expr));

         /*printf("WARNING : %s not foundn\n", ABL_ATOM_VALUE(expr));*/
         addauthelem(tableTH, ABL_ATOM_VALUE(expr), (long) auxName);
         return createablatom(auxName);
         }
      }
   }
else
   {
   chain_list *arg;

   ret = createabloper(ABL_OPER(expr));
   for(arg = ABL_CDR(expr); arg; arg = ABL_CDR(arg))
      addablqexpr(ret, create(ABL_CAR(arg), model, tableTH));
   return ret;
   }
}

/*------------------------------------------------------------------------------
searchMod	 : 
-------------------------------------------------------
parametres 	 : une lofig.
-------------------------------------------------------
return 		 : une befig.
------------------------------------------------------------------------------*/
befig_list *searchMod(liste, name)
befig_list *liste;
char *name;
{
befig_list *l;

for(l = liste; l; l = l->NEXT)
   {
/*
   printf("\tModel %s\n", l->NAME);
*/
   if (l->NAME == name)
      return l;
   }
return (befig_list *)NULL;
}

/*---------------------------------------------------------------------------
addConDirLof     : MAJ direction des connecteurs externes (fct de no_system.c)
	           certains connecteurs deviennent des inout apres mise a plat		
-----------------------------------------------------------------------------
retour           : lofig physiquement modifiee
---------------------------------------------------------------------------*/
void addConDirLof(lofig)
lofig_list *lofig;
{
locon_list *connect;

for(connect = lofig->LOCON; connect; connect = connect->NEXT)
   {
   chain_list *l;
   ptype_list *liste = getptype(connect->SIG->USER, (long)LOFIGCHAIN);
   char dir = ((locon_list *)(((chain_list *)(liste->DATA))->DATA))->DIRECTION;
 
   for (l = (chain_list *)(liste->DATA); l; l = l->NEXT)
       if (((locon_list *)(l->DATA))->TYPE != EXTERNAL &&
           dir != INOUT && ((locon_list*)(l->DATA))->DIRECTION != dir)
          dir = INOUT;
   connect->DIRECTION = dir;
   }
}
/*------------------------------------------------------------------------------
RestoreDir       : met a jour les directions de connecteurs  
-------------------------------------------------------
parametres       : une loins.
-------------------------------------------------------
return           : 
------------------------------------------------------------------------------*/

void RestoreDir(ptr_loins, ptr_befig)
loins_list *ptr_loins;
befig_list *ptr_befig;
{
struct bepor *ptr_bepor;
struct locon *ptr_locon;



for (ptr_locon = ptr_loins->LOCON; ptr_locon; ptr_locon = ptr_locon->NEXT) 
{
for (ptr_bepor = ptr_befig->BEPOR; ptr_bepor; ptr_bepor = ptr_bepor->NEXT)
    {
	if (ptr_locon->NAME == ptr_bepor->NAME)
	{
        switch(ptr_bepor->DIRECTION)
                {
                case 'I': ptr_locon->DIRECTION = IN ;break;
                case 'O': ptr_locon->DIRECTION = OUT ;break;
                case 'B': ptr_locon->DIRECTION = INOUT;break;
                case 'Z': ptr_locon->DIRECTION = TRISTATE;break;
                case 'T': ptr_locon->DIRECTION = TRANSCV;break;
                default:;
                }
	/*printf(" Loins %s   Locon %s   Direction %c\n",ptr_loins->INSNAME,ptr_locon->NAME, ptr_locon->DIRECTION);*/
	}
    }
}

}

/*------------------------------------------------------------------------------
laxExtract	 : retourne une befig a partir d'une lofig et d'une bibliotheque
-------------------------------------------------------
parametres 	 : une lofig.
-------------------------------------------------------
return 		 : une befig.
------------------------------------------------------------------------------*/
befig_list *laxExtract(lofig, trace)
lofig_list *lofig;
int trace;
{
befig_list *befig;
befig_list *ptbefig = NULL;
locon_list *con;
loins_list *inst;
chain_list *mod;
int cptLabel = 0;
authelem* ret;
authtable* busTH = createauthtable(100);


/*--------------- compilation des modeles --------------------------------*/

for(mod = lofig->MODELCHAIN; mod; mod = mod->NEXT)
	{
   	ptbefig = (befig_list *)vhdlloadbefig(ptbefig, (char *)mod->DATA, 0);
	printf(" loading %s \n",ptbefig->NAME);
	/*beh_depend(ptbefig);*/
	
	}

for(inst = lofig->LOINS; inst; inst = inst->NEXT)
   {
   befig_list *modBefig = searchMod(ptbefig, inst->FIGNAME);
   RestoreDir(inst,modBefig);
   }


/*ret = restorealldir(lofig);

if (!ret) exit(1);
printf("restore dir OK \n");*/

/*--------------- mise a jour des directions des conecteurs externes -----*/

addConDirLof(lofig);



/*-------------- creation de la befig resultat --------------------------*/

befig = beh_addbefig(NULL, lofig->NAME);

	/* mise a jour des BEPOR, BERIN, BEOUT */
for(con = lofig->LOCON; con; con = con->NEXT)
   {
   if ((isvdd(con->NAME)) || (isvss(con->NAME)))
      befig->BEPOR = beh_addbepor(befig->BEPOR, con->NAME, 'I', 'B');
   else
      if ((con->DIRECTION == TRISTATE) || (con->DIRECTION == TRANSCV))
		/* en fait il faut recuperer le type du modele ... */
         befig->BEPOR = beh_addbepor(befig->BEPOR, con->NAME,
                                     con->DIRECTION, 'M');
      else
         befig->BEPOR = beh_addbepor(befig->BEPOR, con->NAME, con->DIRECTION, 'B');

   if (trace)
      printf(" POR %s (DIR = %c, TYPE = %c)\n", con->NAME, con->DIRECTION,
             con->TYPE);

   if ((con->DIRECTION == IN) || (con->DIRECTION == INOUT))
      befig->BERIN = beh_addberin(befig->BERIN, con->NAME);
   }

	/* respect de l'ordre de l'interface */
befig->BEPOR = (bepor_list *)reverse((chain_list*)befig->BEPOR);


/*------------------ parcours des instances de la lofig ----------------------*/

for(inst = lofig->LOINS; inst; inst = inst->NEXT)
   {
   befig_list *modBefig = searchMod(ptbefig, inst->FIGNAME);
   int nbSorties = 0;

   for(con = inst->LOCON; con; con = con->NEXT)
      if ((con->DIRECTION == OUT) || (con->DIRECTION == INOUT) ||
          (con->DIRECTION == TRANSCV) || (con->DIRECTION == TRISTATE))
         nbSorties++;

   if (trace)
      printf("-- Inst %s [%s]\n", inst->INSNAME, inst->FIGNAME);

   if (modBefig)
      {
      authtable* intTH = createauthtable(10); /* TH des signaux internes */
      beaux_list *aux;
      bereg_list *reg;
      bebus_list *bus;
      bebux_list *bux;
      beout_list *sortie;

		/* Gestion des auxiliaires */
      for(aux = modBefig->BEAUX; aux; aux = aux->NEXT)
         {
         chain_list *newExpr;
         char *auxName = (char *)mbkalloc(strlen(inst->INSNAME) +
                                          strlen(aux->NAME) + 2);

         sprintf(auxName, "%s_%s", inst->INSNAME, aux->NAME);
         if (searchauthelem(intTH, aux->NAME) == NULL)
            addauthelem(intTH, aux->NAME, (long)auxName);

         newExpr = create(aux->ABL, inst, intTH);

         if (trace)
            {
            printf("     AUX %s (%s)\n", aux->NAME, auxName);
            printf("          ");
            viewablexpr(aux->ABL,ABL_VIEW_INFIX);
            printf("          ");
            viewablexpr(newExpr,ABL_VIEW_INFIX);
            }

         befig->BEAUX = beh_addbeaux(befig->BEAUX, auxName,
                                     newExpr, NULL);
         }

/*------------------- Gestion des registres ----------------------*/

      for(reg = modBefig->BEREG; reg; reg = reg->NEXT)
         {
         biabl_list *bi;
         biabl_list *newBi = (biabl_list *)NULL;
         char *regName=NULL;

         /*if register of standard cell*/
         if (!modBefig->BEBUS && !modBefig->BEBUX && !modBefig->BEREG->NEXT && modBefig->BEOUT && !modBefig->BEOUT->NEXT && ABL_ATOM(modBefig->BEOUT->ABL) && ABL_ATOM_VALUE(modBefig->BEOUT->ABL)==reg->NAME) {
            for(con = inst->LOCON; con; con = con->NEXT)
                if (modBefig->BEOUT->NAME==con->NAME) break;
            if (!con) {
               fprintf(stderr,"locon %s not found\n",modBefig->BEOUT->NAME);
               exit(1);
            }
               regName = getsigname(con->SIG);
         }
         else {
            regName = mbkalloc(strlen(inst->INSNAME) +
                                       strlen(reg->NAME) + 2);
				/* registre tres interne */
              sprintf(regName, "%s_%s", inst->INSNAME, reg->NAME);
         }  /*end of loop*/    

         if (trace)
            printf("     BEREG %s (%s)\n", reg->NAME, regName);

         for(bi = reg->BIABL; bi; bi = bi->NEXT)
            {
            chain_list *newVal;
            chain_list *newCnd;
            char *labelName = (char *)mbkalloc(12);

            addauthelem(intTH, reg->NAME, (long)regName);
            newVal = create(bi->VALABL, inst, intTH);
            newCnd = create(bi->CNDABL, inst, intTH);

            if (trace)
               {
               printf("          VAL");
               viewablexpr(bi->VALABL,ABL_VIEW_INFIX); 
               printf("             ");
               viewablexpr(newVal,ABL_VIEW_INFIX);
               printf("          CMD");
               viewablexpr(bi->CNDABL,ABL_VIEW_INFIX);
               printf("             ");
               viewablexpr(newCnd,ABL_VIEW_INFIX);
               }

            sprintf(labelName, "label_%d", cptLabel++);
            newBi = beh_addbiabl(newBi, labelName, newCnd, newVal);
            }
            befig->BEREG = beh_addbereg(befig->BEREG, regName, newBi, NULL);
         }

		/* Gestion des bus externes */
      for(bus = modBefig->BEBUS; bus; bus = bus->NEXT)
         {
         char *busName=NULL;
         biabl_list *bi;
         biabl_list *newBi = (biabl_list *)NULL;
         char *labelName = (char *)mbkalloc(12);

         if (trace)
            printf("     BUS %s\n", bus->NAME);

         for(bi = bus->BIABL; bi; bi = bi->NEXT)
            {
            chain_list *newVal;
            chain_list *newCnd;

            newVal = create(bi->VALABL, inst, intTH);
            newCnd = create(bi->CNDABL, inst, intTH);

            if (trace)
               {
               printf("          VAL");
               viewablexpr(bi->VALABL,ABL_VIEW_INFIX);
               printf("             ");
               viewablexpr(newVal,ABL_VIEW_INFIX);
               printf("          CMD");
               viewablexpr(bi->CNDABL,ABL_VIEW_INFIX);
               printf("             ");
               viewablexpr(newCnd,ABL_VIEW_INFIX);
               }

            sprintf(labelName, "label_%d", cptLabel++);
            newBi = beh_addbiabl(newBi, labelName, newCnd, newVal);
            }

         for(con = inst->LOCON; con; con = con->NEXT)
            if (con->NAME == bus->NAME)
               {
               int cptInt = 0;

               if (con->SIG->TYPE == EXTERNAL)
                  {
			/* on a reellement un bus externe */
                  chain_list *lstCon;
                  ptype_list *liste = getptype(con->SIG->USER,
                                               (long)LOFIGCHAIN);

                  if (liste)
                     {
                     for(lstCon = (chain_list *)liste->DATA;
                         lstCon; lstCon = lstCon->NEXT)
                        {
                        locon_list *conAux = (locon_list *)lstCon->DATA;

                        if (conAux->ROOT != (void *)inst)
                           {
                           if ((conAux->TYPE != EXTERNAL) &&
                               (conAux->DIRECTION == IN))
                              cptInt++;
                           if (conAux->TYPE == EXTERNAL)
                              busName = conAux->NAME;
                           }
                        }
                  
                     if ((ret = searchauthelem(busTH, busName)) == NULL)
                        {
#if THIS_IS_DISABLED
                        if (cptInt == 0)
                           befig->BEBUS = beh_addbebus(befig->BEBUS, busName,
                                                       newBi, NULL, bus->TYPE);
                        else
                           befig->BEBUS = beh_addbebus(befig->BEBUS, busName,
                                                       newBi, NULL, bus->TYPE);
#endif
                        befig->BEBUS = beh_addbebus(befig->BEBUS, busName,
                                                    newBi, NULL, bus->TYPE);
                        addauthelem(busTH, busName,(long) befig->BEBUS);
                        }
                     else
                        {
				/* le bus existe deja : 
                                   on accroche le nouveau biabl */
                        biabl_list *n;

                        for(n = newBi; n && n->NEXT; n = n->NEXT)
                           ;
                        n->NEXT = ((bebus_list *)ret->VALUE)->BIABL;
                        ((bebus_list *)ret->VALUE)->BIABL = newBi;
                        }
                     }
                  else
                     {
                     printf("ERROR : No LOFIGCHAIN for %s [%s] (%s)\n",
                            inst->INSNAME, inst->FIGNAME,
                            getsigname(con->SIG));
                     exit(-1);
                     }
                  }
               else
                  {
			/* signal interne dans la lofig  = bus interne*/
                  busName = (char *)mbkalloc(strlen(getsigname(con->SIG)) + 4);

                  sprintf(busName, "%s", getsigname(con->SIG));
                  busName = namealloc(busName);
                  if ((ret = searchauthelem(busTH, busName)) == NULL)
                     {
                     befig->BEBUX = beh_addbebux(befig->BEBUX, busName, newBi,
                                                 NULL, bus->TYPE);
                     addauthelem(busTH, busName, (long)befig->BEBUX);
                     }
                  else
                     {
				/* le bux existe deja : 
                                   on accroche le nouveau biabl */
                     biabl_list *n;

                     for(n = newBi; n && n->NEXT; n = n->NEXT)
                        ;
                     n->NEXT = ((bebux_list *)ret->VALUE)->BIABL;
                     ((bebux_list *)ret->VALUE)->BIABL = newBi;
                     }
                  }
               }
         addauthelem(intTH, bus->NAME, (long)busName);
         }

		/* Gestion des bus internes */
      for(bux = modBefig->BEBUX; bux; bux = bux->NEXT)
         {
         biabl_list *bi;
         biabl_list *newBi = (biabl_list *)NULL;
         char *buxName = (char *)mbkalloc(strlen(inst->INSNAME) +
                                          strlen(bux->NAME) + 2);

         sprintf(buxName, "%s_%s", inst->INSNAME, bux->NAME);
 
         if (trace)
            printf("     BUX %s\n", bux->NAME);
 
         for(bi = bux->BIABL; bi; bi = bi->NEXT)
            {
            chain_list *newVal;
            chain_list *newCnd;
            char *labelName = (char *)mbkalloc(12);
 
            addauthelem(intTH, bux->NAME, (long)buxName);
            newVal = create(bi->VALABL, inst, intTH);
            newCnd = create(bi->CNDABL, inst, intTH);
 
            if (trace)
               {
               printf("          VAL");
               viewablexpr(bi->VALABL,ABL_VIEW_INFIX);
               printf("             ");
               viewablexpr(newVal,ABL_VIEW_INFIX);
               printf("          CMD");
               viewablexpr(bi->CNDABL,ABL_VIEW_INFIX);
               printf("             ");
               viewablexpr(newCnd,ABL_VIEW_INFIX);
               }
 
            sprintf(labelName,"label_%d", cptLabel++);
            newBi = beh_addbiabl(newBi, labelName, newCnd, newVal);
            }
 
         if ((ret = searchauthelem(busTH, buxName)) == NULL)
            {
            befig->BEBUX = beh_addbebux(befig->BEBUX, buxName, newBi,
                                        NULL, bux->TYPE);
            addauthelem(busTH, buxName, (long)befig->BEBUX);
            }
         else
            {
		/* le bux existe deja : 
                   on accroche le nouveau biabl */
            biabl_list *n;

            for(n = newBi; n && n->NEXT; n = n->NEXT)
               ;
            n->NEXT = ((bebux_list *)ret->VALUE)->BIABL;
            ((bebux_list *)ret->VALUE)->BIABL = newBi;
            }

         }

		/* Gestion des sorties */
         for(sortie = modBefig->BEOUT; sortie; sortie = sortie->NEXT)
         {
         /*absorb beout for register of standard cell*/
         if (!modBefig->BEBUS && !modBefig->BEBUX && modBefig->BEREG && !modBefig->BEREG->NEXT && modBefig->BEOUT && !modBefig->BEOUT->NEXT && ABL_ATOM(modBefig->BEOUT->ABL) && ABL_ATOM_VALUE(modBefig->BEOUT->ABL)==modBefig->BEREG->NAME) continue;

         for(con = inst->LOCON; con; con = con->NEXT)
            if (sortie->NAME == con->NAME)
               {
               chain_list *newExpr;
               char *nameAux = (char *)mbkalloc(strlen(getsigname(con->SIG)) + 4);

               sprintf(nameAux, "%s", getsigname(con->SIG));
               newExpr = create(sortie->ABL, inst, intTH);

               if (trace)
                  {
                  printf("     OUT %s\n", sortie->NAME);
                  printf("             ");
                  viewablexpr(sortie->ABL,ABL_VIEW_INFIX);
                  printf("             ");
                  viewablexpr(newExpr,ABL_VIEW_INFIX);
                  }

               if (con->SIG->TYPE == INTERNAL)
                  {
                  befig->BEAUX = beh_addbeaux(befig->BEAUX,
                                              nameAux,
                                              newExpr, NULL);
                  }
               else
                  {
			/* signal interne dans la lofig */
                  chain_list *lstCon;
                  int cptInt = 0;
                  char *outName=NULL;
                  ptype_list *liste = getptype(con->SIG->USER, 
                                               (long)LOFIGCHAIN);

                  if (liste)
                     {
                     for(lstCon = (chain_list *)liste->DATA;
                         lstCon; lstCon = lstCon->NEXT)
                        {
                        locon_list *conAux = (locon_list *)lstCon->DATA;
         
                        if (conAux->ROOT != (void *)inst)
                           {
                           if ((conAux->TYPE != EXTERNAL) &&
                               (conAux->DIRECTION == IN))
                              cptInt++;
                           if (conAux->TYPE == EXTERNAL)
                              outName = conAux->NAME;
                           }
                        }
                     if (cptInt == 0)
				/* sortie directe */
                        befig->BEOUT = beh_addbeout(befig->BEOUT, outName,
                                                    newExpr, NULL);
                     else
                        {
				/* cas de la sortie reinjectee */
                        befig->BEAUX = beh_addbeaux(befig->BEAUX, nameAux,
                                                    newExpr, NULL);
                        befig->BEOUT = beh_addbeout(befig->BEOUT, outName,
                                                    createablatom(nameAux), NULL);
                        }
                     }
                  else
                     {
                     printf("ERROR : No LOFIGCHAIN for %s [%s] (%s)\n",
                            inst->INSNAME, inst->FIGNAME, 
                            getsigname(con->SIG));
                     exit(-1);
                     }
                  }
               }
         }
      destroyauthtable(intTH);
      }
   else
      {
      printf("Error : le model %s n'existe pas !\n", inst->FIGNAME);
      return (befig_list *)NULL;
      }
   }

	/* nettoyage de la bibliotheque : quand ca voudra bien marcher */
/*
beh_frebefig(ptbefig);
*/

destroyauthtable(busTH);
return(befig);
}
