/*------------------------------------------------------------\
|                                                             |
| Tool    :                     FMI                           |
|                                                             |
| File    :                  fmi_optim.c                      |
|                                                             |
| Author  :                  Jean Marie Alexandre             |
|                                                             |
| Date    :                   20.09.00                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

# include "mut.h"
# include LOG_H
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"


# include "fmi_optim.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Parse Fsm File                       |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                    Fmi Del Elm                              |
|                                                             |
\------------------------------------------------------------*/

void
delfmielm (elm)
     fmicelm_list *elm;
{
  fmicelm_list *DelElem;
  fmicelm_list *ScanElem;

  ScanElem = elm;

  while ( ScanElem != (fmicelm_list *) 0)
    {
      DelElem  = ScanElem;
      ScanElem = ScanElem->NEXT;
      autfreeheap ((char *) DelElem, sizeof (fmicelm_list));
    }
}

/*------------------------------------------------------------\
|                                                             |
|                    Fmi Del Classe                           |
|                                                             |
\------------------------------------------------------------*/

void
delfmiclasse (classe)
     fmiclass_list *classe;
{
  fmiclass_list *DelClass;
  fmiclass_list *ScanClass;

  ScanClass = classe;

  while (ScanClass != (fmiclass_list *) 0)
    {
      DelClass  = ScanClass;
      ScanClass = ScanClass->NEXT;

      autfreeheap((char *)DelClass, sizeof(fmiclass_list));
    }
}

/*------------------------------------------------------------\
|                                                             |
|                        Fmi Alloc Elm                        |
|                                                             |
\------------------------------------------------------------*/

fmicelm_list *
allocfmielm ()
{
  return (fmicelm_list *) (autallocheap(sizeof (fmicelm_list)));
}



/*------------------------------------------------------------\
|                                                             |
|                        Fmi Alloc Classe                     |
|                                                             |
\------------------------------------------------------------*/

fmiclass_list *
allocfmiclasse ()
{
  return (fmiclass_list *) (autallocheap(sizeof (fmiclass_list)));
}



/*------------------------------------------------------------\
|                                                             |
|                        AddCLassElm                          |
|                                                             |
\------------------------------------------------------------*/

fmicelm_list *
AddClassElm (elm, state)
     fmicelm_list *elm;
     fsmstate_list *state;

{
  fmicelm_list *new;

  new = allocfmielm ();

  new->STATE = state;
  new->NEXT = elm;

  return new;

}


/*------------------------------------------------------------\
|                                                             |
|                        AddCLass                             |
|                                                             |
\------------------------------------------------------------*/

fmiclass_list *
AddClass (classe)
     fmiclass_list *classe;

{
  fmiclass_list *new;

  new = allocfmiclasse ();
  new->NEXT = classe;
  return new;

}

/*------------------------------------------------------------\
|                                                             |
|                        FmiRazUser                           |
|                                                             |
\------------------------------------------------------------*/

fsmstate_list *
FmiRazUser (state)
     fsmstate_list *state;

{

  fsmstate_list *ScanState;
  fsmstate_list *head;

  head = state;

  for (ScanState = state;
       ScanState != (fsmstate_list *) 0; ScanState = ScanState->NEXT)
    {
      ScanState->USER = NULL;

    }
  return (head);
}

/*------------------------------------------------------------\
|                                                             |
|                        FmiRazFlag                           |
|                                                             |
\------------------------------------------------------------*/

void
FmiRazFlag (classe)
     fmiclass_list *classe;

{

  fmiclass_list *ScanClasse;
  fmicelm_list *Scanelm;
  fmiclass_list *head;

  head = classe;

  for (ScanClasse = classe;
       ScanClasse != (fmiclass_list *) 0; ScanClasse = ScanClasse->NEXT)
    {
      for (Scanelm = ScanClasse->ELM;
	   Scanelm != (fmicelm_list *) 0; Scanelm = Scanelm->NEXT)
	{
	  ClearFmiClass (Scanelm->STATE);
	}
    }
}


/*------------------------------------------------------------\
|                                                             |
|                        FmiClassedansUser                    |
|                                                             |
\------------------------------------------------------------*/

fmiclass_list *
FmiClassedansUser (classe)
     fmiclass_list *classe;
{

  fmicelm_list *Scanelm;
  fmiclass_list *Scanclasse;

  for (Scanclasse = classe;
       Scanclasse != (fmiclass_list *) 0; Scanclasse = Scanclasse->NEXT)
    {
      for (Scanelm = Scanclasse->ELM;
	   Scanelm != (fmicelm_list *) 0; Scanelm = Scanelm->NEXT)
	{
	  Scanelm->STATE->USER = (void *) Scanclasse;
	}
    }
  return (classe);
}


/*------------------------------------------------------------\
|                                                             |
|                        Fmitestin                            |
|                                                             |
\------------------------------------------------------------*/



int
Fmitestin (State1, State2)
     fsmstate_list *State1;
     fsmstate_list *State2;
{
  chain_list *ScanChain1;
  chain_list *ScanChain2;
  fsmtrans_list *Trans1;
  fsmtrans_list *Trans2;
  fsmstate_list *SuccState1;
  fsmstate_list *SuccState2;
  bddnode *BddNode1;
  bddnode *BddNode2;
  bddnode *BddNode;

  for (ScanChain1 = State1->FROM;
       ScanChain1 != (chain_list *) 0; ScanChain1 = ScanChain1->NEXT)
    {
      Trans1 = (fsmtrans_list *) ScanChain1->DATA;
      SuccState1 = Trans1->TO;

      BddNode1 = Trans1->BDD;

      for (ScanChain2 = State2->FROM;
	   ScanChain2 != (chain_list *) 0; ScanChain2 = ScanChain2->NEXT)
	{
	  Trans2 = (fsmtrans_list *) ScanChain2->DATA;
	  BddNode2 = Trans2->BDD;

	  BddNode =
	    applybddnode ((bddsystem *) 0, ABL_AND, BddNode1, BddNode2);
	  decbddrefext (BddNode);

	  if (BddNode != BddLocalSystem->ZERO)
	    {
	      SuccState2 = Trans2->TO;

	      if (SuccState2->USER != SuccState1->USER)
		{
		  return 0;
		}
	    }
	}
    }
  return 1;
}


/*------------------------------------------------------------\
|                                                             |
|                        Fmisucc                              |
|                                                             |
\------------------------------------------------------------*/

int
Fmisucc (State1, State2)
     fsmstate_list *State1;
     fsmstate_list *State2;

{
  return (State1->USER == State2->USER) && Fmitestin (State1, State2);
}

/*------------------------------------------------------------\
|                                                             |
|                        FmiRec                               |
|                                                             |
\------------------------------------------------------------*/
fsmfig_list *
FmiRec (fsm, classe)
     fsmfig_list *fsm;
     fmiclass_list *classe;
{
  fmiclass_list *ScanClasse;
  fsmtrans_list *ScanTrans;
  chain_list *ScanChain1;
  fsmstate_list *State, *FromState, *ToState;
  fsmtrans_list *trans;
  fsmfig_list *NewFsm;

  fsmin_list *ScanIn;
  fsmout_list *ScanOut;
  fsmport_list *ScanPort;
  fsmlocout_list *ScanLocout;

  for (ScanTrans = fsm->TRANS; ScanTrans; ScanTrans = ScanTrans->NEXT)
    {
      ClearFmiClass (ScanTrans);
    }

  /* Duplication of the old FSM entity */

  NewFsm = addfsmfig ("$$");

  NewFsm->CLOCK = fsm->CLOCK;
  NewFsm->CLOCK_ABL = dupablexpr (fsm->CLOCK_ABL);

  for (ScanIn = fsm->IN; ScanIn; ScanIn = ScanIn->NEXT)
    {
      addfsmin (NewFsm, ScanIn->NAME);
    }

  for (ScanOut = fsm->OUT; ScanOut; ScanOut = ScanOut->NEXT)
    {
      addfsmout (NewFsm, ScanOut->NAME);
    }

  for (ScanPort = fsm->PORT; ScanPort; ScanPort = ScanPort->NEXT)
    {
      addfsmport (NewFsm, ScanPort->NAME, ScanPort->DIR, ScanPort->TYPE);
    }

  for (ScanClasse = classe;
       ScanClasse != (fmiclass_list *) 0; ScanClasse = ScanClasse->NEXT)
    {
      State = addfsmstate (NewFsm, ScanClasse->ELM->STATE->NAME);
      for (ScanLocout = ScanClasse->ELM->STATE->LOCOUT;
	   ScanLocout; ScanLocout = ScanLocout->NEXT)
	{
	  addfsmlocout (State, searchfsmout (NewFsm, ScanLocout->OUT->NAME),
			dupablexpr (ScanLocout->ABL), NULL);
	}
    }

  if (fsm->FIRST_STATE) {
    NewFsm->FIRST_STATE =
      searchfsmstate (NewFsm,
		      ((fmiclass_list *) fsm->FIRST_STATE->USER)->ELM->STATE->
		      NAME);
    SetFsmFirstState (NewFsm->FIRST_STATE);
  } else
  {
    NewFsm->FIRST_STATE = NULL;
  }


  for (ScanClasse = classe;
       ScanClasse != (fmiclass_list *) 0; ScanClasse = ScanClasse->NEXT)
    {
      State = ScanClasse->ELM->STATE;
      for (ScanChain1 = State->FROM;
	   ScanChain1 != (chain_list *) 0; ScanChain1 = ScanChain1->NEXT)
	{
	  FromState = searchfsmstate (NewFsm, ScanClasse->ELM->STATE->NAME);
	  ToState = searchfsmstate (NewFsm,
				    ((fmiclass_list
				      *) ((fsmtrans_list *)
					  ScanChain1->DATA)->TO->USER)->
				    ELM->STATE->NAME);
	  trans =
	    addfsmtrans (NewFsm, FromState, ToState,
			 dupablexpr (((fsmtrans_list *) ScanChain1->DATA)->
				     ABL));
	}
    }

  if ( IsFsmFigMixedRtl( fsm ) )
  {
    NewFsm->FIGURE = fsm->FIGURE;
    SetFsmFigMixedRtl( NewFsm );

    ClearFsmFigMixedRtl( fsm );
    fsm->FIGURE = (void *)0;
  }

  delfsmfig( fsm->NAME );

  return NewFsm;
}

/*------------------------------------------------------------\
|                                                             |
|                        FmiOptimizeFsm                       |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *
FmiOptimizeFsm (FsmFigure)
     fsmfig_list *FsmFigure;
{

  fsmstate_list *ScanState1;
  fsmstate_list *ScanState2;
  fmiclass_list *ScanClasse;
  fmiclass_list *classetete = NULL;
  fmicelm_list *ScanElm, *Elm;

  fmiclass_list *new;
  fmiclass_list *act;
  int modif = 0;
  long int nombre;
  long int n;

  /*****************    premier passage   ******************/

  for (ScanState1 = FsmFigure->STATE;
       ScanState1 != (fsmstate_list *) 0; ScanState1 = ScanState1->NEXT)
    {
      if (ScanState1->USER != (bddnode *) 0 && (!IsFmiClass (ScanState1)))
	{
	  classetete = AddClass (classetete);
	  classetete->ELM = AddClassElm (NULL, ScanState1);
	  SetFmiClass (ScanState1);

	  /* on considère maintenant que ScanState1 est dans une classe   */

	  for (ScanState2 = ScanState1->NEXT;
	       ScanState2 != (fsmstate_list *) 0;
	       ScanState2 = ScanState2->NEXT)
	    {

	      if (ScanState2->USER != (bddnode *) 0
		  && (!IsFmiClass (ScanState2)))
		{
		  if ((bddnode *) ScanState2->USER ==
		      (bddnode *) ScanState1->USER)
		    {
		      classetete->ELM =
			AddClassElm (classetete->ELM, ScanState2);
		      SetFmiClass (ScanState2);
		    }

		}
	    }
	}
    }




# ifdef DEBUG
  for (new = classetete; new; new = new->NEXT)
    {
      printf ("{");
      for (ScanElm = new->ELM; ScanElm; ScanElm = ScanElm->NEXT)
	printf ("%s%s", ScanElm->STATE->NAME, ScanElm->NEXT ? "," : "");
      printf ("}\n");
    }
  printf ("\n");
# endif


/* remise a jour des champs USER et FLAG de STATE */

  FmiRazUser (FsmFigure->STATE);
  FmiRazFlag (classetete);


/* on met la classe dans le champ USER de des states pour
 * que chaque état sache dans quelle classe il se trouve    */


  FmiClassedansUser (classetete);

/*******************    deuxième passage        ************/

  act = classetete;

  do
    {
      modif = 0;
      new = NULL;

      for (ScanClasse = act;
	   ScanClasse != (fmiclass_list *) 0; ScanClasse = ScanClasse->NEXT)
	{
	  for (ScanElm = ScanClasse->ELM;
	       ScanElm != (fmicelm_list *) 0; ScanElm = ScanElm->NEXT)
	    {
	      if (!IsFmiClass (ScanElm->STATE))
		{
		  new = AddClass (new);
		  new->ELM = AddClassElm (NULL, ScanElm->STATE);
		  SetFmiClass (ScanElm->STATE);

		  for (Elm = ScanElm->NEXT;
		       Elm != (fmicelm_list *) 0; Elm = Elm->NEXT)
		    {
		      if (!IsFmiClass (Elm->STATE))
			{
			  if (Fmisucc (ScanElm->STATE, Elm->STATE))
			    {
			      new->ELM = AddClassElm (new->ELM, Elm->STATE);
			      SetFmiClass (Elm->STATE);
			    }
			  else
			    {
			      modif = 1;
			    }
			}
		    }
		}
	    }
	}
      FmiRazFlag (new);

      if (modif == 1)
      {
	FmiClassedansUser (new);
      act = new;
      }
    }
  while (modif != 0);

  classetete = act;

# ifdef DEBUG
  for (new = classetete; new; new = new->NEXT)
    {
      printf ("{");
      for (ScanElm = new->ELM; ScanElm; ScanElm = ScanElm->NEXT)
	printf ("%s%s", ScanElm->STATE->NAME, ScanElm->NEXT ? "," : "");
      printf ("}\n");
    }
# endif

/*   à partir de la classe, on reconstruit la nouvelle classe   */

  nombre = FsmFigure->NUMBER_STATE;

  FsmFigure = FmiRec (FsmFigure, classetete);

/*delfmielm(classetete->ELM);
delfmiclasse(classetete);
delfmielm(act->ELM);
delfmiclasse(act);
*/

  n = nombre;
  nombre = nombre - FsmFigure->NUMBER_STATE;
  fprintf( stdout, "\t--> Initial number states    : %ld\n", n);
  fprintf( stdout, "\t--> Number of deleted states : %ld\n", nombre);

  return (FsmFigure);
}
