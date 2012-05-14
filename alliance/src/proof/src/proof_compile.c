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

/*
 * Tool    : Logic Synthesis - boolean optimisation
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 * Modified by Ludo 2002
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bvl.h"

# include "proof_util.h"

befig_list *VHB_HEDFIG;

/*----------------------------------------------------------------------------
remegalExpr   	: elimine les horreurs du style 
                        (not (xor a '0'))  --> not a
                        (not (xor a '1'))  --> a
------------------------------------------------------------------------------
retour		: une expression
------------------------------------------------------------------------------*/

static chain_list *
remegalExpr (expr)
     chain_list *expr;
{
  if (ATOM (expr))
    return (copyExpr (expr));
  else
    {
      chain_list *ret;

      if ((OPER (expr) == NOT) && !ATOM (CADR (expr)) && (OPER (CADR (expr)) == XOR))
	{
	  chain_list *filsExpr;
	  chain_list *filsCst;

	  filsExpr = CADR (expr);
	  filsCst = CADR (CDR (filsExpr));

	  if (ATOM (filsCst) && !strcmp (VALUE_ATOM (filsCst), "'1'"))
	    {
	      return (copyExpr (CADR (filsExpr)));
	    }
	  if (ATOM (filsCst) && !strcmp (VALUE_ATOM (filsCst), "'0'"))
	    {
	      return (notExpr (copyExpr (CADR (filsExpr))));
	    }
	}

      ret = createExpr (OPER (expr));

      while ((expr = CDR (expr)))
	{
	  addQExpr (ret, remegalExpr (CAR (expr)));
	}
      return (ret);
    }
}

/*-------------------------------------------------------------------------
renameExpr 	: renommage d'une expression avec une fonction
---------------------------------------------------------------------------
retour		: un void.
---------------------------------------------------------------------------*/
void
renameExpr (expr, func)
     chain_list *expr;
     char *(*func) ();
{
  if (ATOM (expr))
    expr->DATA = (void *) (*func) (VALUE_ATOM (expr));
  else
    {
      while ((expr = CDR (expr)))
	{
	  renameExpr (CAR (expr), func);
	}
    }
}


/*-------------------------------------------------------------------------
renameBeh 	: renommage d'une befig 
---------------------------------------------------------------------------
retour		: un void.
---------------------------------------------------------------------------*/

void
renameBeh (beh, func)
     befig_list *beh;
     char *(*func) ();
{
  beout_list *out;
  bereg_list *reg;
  bebus_list *bus;
  bebux_list *bux;
  biabl_list *biabl;
  beaux_list *aux;
  berin_list *in;
  bepor_list *port;
  bemsg_list *msg;
  begen_list *generic;
  int i;

  for (generic = beh->BEGEN; generic; generic = generic->NEXT)
    {
      char *gen = generic->NAME;
      char *val = generic->NAME;

      /* on avance dans val jusqu'au '_'  */

      for (i = 0; i < strlen (gen) && gen[i] != '_'; i++)
	val++;


      if (i < strlen (gen))	/* le '_' existe bien */
	{
	  char *newval;
	  char *field;
	  char *racine = (char *) mbkalloc (1024);

	  strncpy (racine, gen, ++i);
	  racine[i] = '\0';

	  val++;
	  newval = (*func) (val);
	  field = (char *) mbkalloc (1024 + strlen (newval));
	  sprintf (field, "%s%s", racine, newval);

	  generic->NAME = namealloc (field);

	  mbkfree (field);	/* chaine */
	  mbkfree (racine);	/* chaine */
	}
    }

  port = beh->BEPOR;
  while (port)
    {
      port->NAME = (*func) (port->NAME);
      port = port->NEXT;
    }

  in = beh->BERIN;
  while (in)
    {
      in->NAME = (*func) (in->NAME);
      in = in->NEXT;
    }

  aux = beh->BEAUX;
  while (aux)
    {
      aux->NAME = (*func) (aux->NAME);
      if (aux->ABL)
	{
	  renameExpr (aux->ABL, func);
	}
      aux = aux->NEXT;
    }

  msg = beh->BEMSG;
  while (msg)
    {
      if (msg->ABL)
	{
	  renameExpr (msg->ABL, func);
	}
      msg = msg->NEXT;
    }

  out = beh->BEOUT;
  while (out)
    {
      out->NAME = (*func) (out->NAME);
      if (out->ABL)
	{
	  renameExpr (out->ABL, func);
	}
      out = out->NEXT;
    }

  reg = beh->BEREG;
  while (reg)
    {
      reg->NAME = (*func) (reg->NAME);
      biabl = reg->BIABL;
      while (biabl)
	{
	  if (biabl->CNDABL && biabl->VALABL)
	    {
	      renameExpr (biabl->CNDABL, func);
	      renameExpr (biabl->VALABL, func);
	    }
	  biabl = biabl->NEXT;
	}
      reg = reg->NEXT;
    }

  bus = beh->BEBUS;
  while (bus)
    {
      bus->NAME = (*func) (bus->NAME);
      biabl = bus->BIABL;
      while (biabl)
	{
	  if (biabl->CNDABL && biabl->VALABL)
	    {
	      renameExpr (biabl->CNDABL, func);
	      renameExpr (biabl->VALABL, func);
	    }
	  biabl = biabl->NEXT;
	}
      bus = bus->NEXT;
    }

  bux = beh->BEBUX;
  while (bux)
    {
      bux->NAME = (*func) (bux->NAME);
      biabl = bux->BIABL;
      while (biabl)
	{
	  if (biabl->CNDABL && biabl->VALABL)
	    {
	      renameExpr (biabl->CNDABL, func);
	      renameExpr (biabl->VALABL, func);
	    }
	  biabl = biabl->NEXT;
	}
      bux = bux->NEXT;
    }
}

/*-------------------------------------------------------------------------
substPhyExprBeh 	: substitue une variable par une expression 
---------------------------------------------------------------------------
retour		: un void.
---------------------------------------------------------------------------*/
void
substPhyExprBeh (beh, name, expr)
     befig_list *beh;
     char *name;
     chain_list *expr;
{
  beout_list *out;
  beaux_list *aux;
  bebus_list *bus;
  bebux_list *bux;
  bereg_list *reg;
  biabl_list *biabl;

  out = beh->BEOUT;
  while (out)
    {
      if (out->ABL)
	{
	  if (searchExpr (out->ABL, name))
	    {
	      if (!ATOM (out->ABL))
		substPhyExpr (out->ABL, name, expr);
	      else
		out->ABL = copyExpr (expr);
	    }
	}
      out = out->NEXT;
    }

  aux = beh->BEAUX;
  while (aux)
    {
      if (aux->ABL)
	{
	  if (searchExpr (aux->ABL, name))
	    {
	      if (!ATOM (aux->ABL))
		substPhyExpr (aux->ABL, name, expr);
	      else
		aux->ABL = copyExpr (expr);
	    }
	}
      aux = aux->NEXT;
    }

  reg = beh->BEREG;
  while (reg)
    {
      biabl = reg->BIABL;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      if (searchExpr (biabl->VALABL, name))
		{
		  if (!ATOM (biabl->VALABL))
		    substPhyExpr (biabl->VALABL, name, expr);
		  else
		    biabl->VALABL = copyExpr (expr);
		}
	    }
	  if (biabl->CNDABL)
	    {
	      if (searchExpr (biabl->CNDABL, name))
		{
		  if (!ATOM (biabl->CNDABL))
		    substPhyExpr (biabl->CNDABL, name, expr);
		  else
		    biabl->CNDABL = copyExpr (expr);
		}
	    }
	  biabl = biabl->NEXT;
	}
      reg = reg->NEXT;
    }

  bus = beh->BEBUS;
  while (bus)
    {
      biabl = bus->BIABL;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      if (searchExpr (biabl->VALABL, name))
		{
		  if (!ATOM (biabl->VALABL))
		    substPhyExpr (biabl->VALABL, name, expr);
		  else
		    biabl->VALABL = copyExpr (expr);
		}
	    }
	  if (biabl->CNDABL)
	    {
	      if (searchExpr (biabl->CNDABL, name))
		{
		  if (!ATOM (biabl->CNDABL))
		    substPhyExpr (biabl->CNDABL, name, expr);
		  else
		    biabl->CNDABL = copyExpr (expr);
		}
	    }
	  biabl = biabl->NEXT;
	}
      bus = bus->NEXT;
    }

  bux = beh->BEBUX;
  while (bux)
    {
      biabl = bux->BIABL;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      if (searchExpr (biabl->VALABL, name))
		{
		  if (!ATOM (biabl->VALABL))
		    substPhyExpr (biabl->VALABL, name, expr);
		  else
		    biabl->VALABL = copyExpr (expr);
		}
	    }
	  if (biabl->CNDABL)
	    {
	      if (searchExpr (biabl->CNDABL, name))
		{
		  if (!ATOM (biabl->CNDABL))
		    substPhyExpr (biabl->CNDABL, name, expr);
		  else
		    biabl->CNDABL = copyExpr (expr);
		}
	    }
	  biabl = biabl->NEXT;
	}
      bux = bux->NEXT;
    }
}
/*----------------------------------------------------------------------------
compileVHDL   	: compile un vbe , cree eventuellement les BDD et verifie la 
		  coherence de la description. 
		  orderPI = permet d'imposer un ordonnancement initial
		  si sauveaux = 1 on conserve les variables intermediaires
		  si gcOk = 1 on peut effectuer un garbage collector.
		  si vhb_reorder > 1 on peut re-ordonnancer les bdds du circuit.
                  makeBdd = 1 : creation de BDD
		     -> Le systeme des BDD doit etre initialise auparavant.
		  trace : mode trace
		  func : fonction de renomage des signaux internes
		  beh : si <> NULL, la befig a deja ete compilee
------------------------------------------------------------------------------
retour		: NULL si la compilation n'a pas fonctionne 
                  l'ordonnancement des variables d'entrees sinon.
------------------------------------------------------------------------------*/

/*
  compileVHDL(argv[argc-2],NULL,0,NULL,1,0,0,0,1,nameIntVect,NULL)
*/

chain_list *
compileVHDL (file, orderPI, order, par,
	     saveaux, gcOK, vhb_reorder, makeBdd, trace, func, beh)
     char *file;
     chain_list *orderPI;
     int order;
     void *par;
     int saveaux;
     int gcOK;
     int vhb_reorder;
     int makeBdd;
     int trace;
     char *(*func) ();
     befig_list *beh;
{
  befig_list *vhdlloadbefig ();
  beaux_list *aux;
  chain_list *ptChain;
  biabl_list *biabl;
  bereg_list *reg;
  int coInter;
  char *path;
  int i;
  int indice = 0;
  char *filep;
  chain_list *liste, *pAux = NULL;


  /* si BEH est NULL, on appelle le compilateur */

  if (beh == NULL)
    {
      filep = (char *) mbkalloc (strlen (file) + 2);

      strcpy (filep, file);

      file = filep;

      for (i = 0; i < strlen (file); i++)
	if (file[i] == '/')
	  indice = i;

      if (indice == 0)		/* pas de '/' dans  file */
	{
	  path = ".";
	  file[strlen (file) - 4] = '\0';
	}
      else
	{
	  path = (char *) mbkalloc (indice + 1);
	  strncpy (path, file, indice);
	  path[indice] = '\0';
	  for (i = 0; i <= indice; i++)
	    file++;
	  file[strlen (file) - 4] = '\0';
	}

      /* compilation du VHDL */

      if (trace)
	printf ("Compiling '%s' ...\n", file);

      beh = vhdlloadbefig (NULL, file, 0);
    }

  if (beh->ERRFLG != 0)
    return NULL;

  /* on empile sur VHB_HEDFIG */
  beh->NEXT = VHB_HEDFIG;
  VHB_HEDFIG = beh;

  /* mise a NULL du champ USER pour les biabl des registres */
  reg = beh->BEREG;
  while (reg)
    {
      biabl = reg->BIABL;
      while (biabl)
	{
	  biabl->USER = NULL;
	  biabl = biabl->NEXT;
	}
      reg = reg->NEXT;
    }


  /* verification de la declaration correcte d'attribut 'stable

     forme valide      (l = '0') and not l'stable   : front descendant
     (and (not l) (not (stable l)))

     ou                (l = '1') and not l'stable   : front montant
     (and l (not (stable l)))
   */

  if (beh->TYPE & BEH_STABLE)
    {
      reg = beh->BEREG;
      while (reg)
	{
	  biabl = reg->BIABL;
	  while (biabl)
	    {
	      chain_list *inter = biabl->CNDABL;

	      biabl->CNDABL = remegalExpr (biabl->CNDABL);
	      if (searchOperExpr (biabl->CNDABL, STABLE))
		{
		  chain_list *pattern1, *pattern2;
		  chain_list *pattern3, *pattern4;

		  pattern1 = charToExpr ("(and (not x) (not (stable x)))");
		  pattern2 = charToExpr ("(and x (not (stable x)))");
		  pattern3 = charToExpr ("(and (not (stable x)) (not x))");
		  pattern4 = charToExpr ("(and (not (stable x)) x)");

		  if (!PMExpr (biabl->CNDABL, pattern1) &&
		      !PMExpr (biabl->CNDABL, pattern2) &&
		      !PMExpr (biabl->CNDABL, pattern3) &&
		      !PMExpr (biabl->CNDABL, pattern4))
		    {
		      printf ("VHDL : Error - bad usage of the 'stable' attribut\n");
		      return (NULL);
		    }
		  else
		    {
		      biabl->USER = addptype (NULL, 0, 0);
		    }

		  /*    (and (not x) (not (stable x))) --> x */
		  /*    (and x (not (stable x))) --> (not x) */

		  if (PMExpr (biabl->CNDABL, pattern1) ||
		      PMExpr (biabl->CNDABL, pattern2))
		    {
		      if (ATOM (CADR (biabl->CNDABL)))
			biabl->CNDABL = notExpr (CADR (biabl->CNDABL));
		      else
			biabl->CNDABL = CADR (CADR (biabl->CNDABL));
		    }
		  else
		    {
		      if (ATOM (CADR (CDR (biabl->CNDABL))))
			biabl->CNDABL = notExpr (CADR (CDR (biabl->CNDABL)));
		      else
			biabl->CNDABL = CADR (CADR (CDR (biabl->CNDABL)));
		    }

		  freeExpr (pattern1);
		  freeExpr (pattern2);
		  freeExpr (pattern3);
		  freeExpr (pattern4);
		}
	      freeExpr (inter);
	      biabl = biabl->NEXT;
	    }
	  reg = reg->NEXT;
	}
    }


  /* on renomme les variables de la befig pour les vecteurs */

  if (func != NULL)
    renameBeh (beh, func);

  /* on de-vectorise les variables intermediaires */

  aux = beh->BEAUX;
  while (aux)
    {
      int i = 0;
      char *name = (char *) mbkalloc (strlen (aux->NAME) + 1);
      strcpy (name, aux->NAME);

      /* detection signal vectorise */

      while (name[i] != '\0' && name[i] != ' ')
	{
	  i++;
	}
      /* substitution avec un souligne */

      if (name[i] == ' ')
	{
	  name[i] = '_';
	  substPhyExprBeh (beh, aux->NAME, createAtom (name));
	  aux->NAME = namealloc (name);
	}

      mbkfree (name);		/* desa. de name */

      aux = aux->NEXT;
    }

    return ((chain_list *) 1);
}

