
/* ###--------------------------------------------------------------### */
/* file		: vh_lspec.c						*/
/* date		: Jun  3 1997						*/
/* version	: v2.02							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* contents	: This file contains the set of functions used by the	*/
/*		  linker to build data structures, and to check the	*/
/*		  consistency of the description :			*/
/*									*/
/*		  vhl_addlkdfig, vhl_addlkdins, vhl_addsimsig,		*/
/*		  vhl_addbussig, vhl_addwrbsig, vhl_addwrireg,		*/
/*		  vhl_addwriaux, vhl_addwribux, vhl_addprjbvl,		*/
/*		  vhl_addprjrvl, vhl_addredlst, vhl_addwrimsg,		*/
/*		  vhl_makgst   , vhl_linker   , vhl_restor   ,		*/
/*		  vhl_resord   , vhl_iscmodel , vhl_makdepend,		*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "mlo.h"
#include "log.h"
#include "beh.h"
#include "pat.h"
#include "vh_ltype.h"
#include "vh_globals.h"
#include "vh_util.h"
#include "vh_lspec.h"

/* ###--------------------------------------------------------------### */
/* function	: vhl_iscmodel						*/
/* description	: check if a behavioural model has a C architecture and	*/
/*		  return the pointer of the C function if found		*/
/* called funct	: namealloc						*/
/* ###--------------------------------------------------------------### */

void (*vhl_iscmodel (str) ) ()

char *str;

  {
  struct lkdmdl  *model    ;
  char           *name     ;
  void          (*fonc) () = NULL;

  name = namealloc (str);

  model = VHL_MODELS;
  while (model != NULL)
    {
    if (model->NAME == name)
      {
      fonc = model->FONC;
      break;
      }
    model = model->NEXT;
    }

  return (fonc);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_makdepend						*/
/* description	: build dependency lists between signals		*/
/* called funct	: addptype						*/
/* ###--------------------------------------------------------------### */

struct ptype *vhl_makdepend (dict, pt_berin, ins_name)

struct beden **dict    ;
struct berin  *pt_berin;
char          *ins_name;

  {
  struct ptype  *list      = NULL;
  struct wrireg *pt_wrireg ;
  struct wriaux *pt_wriaux ;
  struct wribux *pt_wribux ;
  struct wrimsg *pt_wrimsg ;
  struct wrssig *pt_wrssig ;
  struct wrbsig *pt_wrbsig ;
  struct bereg  *pt_bereg  ;
  struct beaux  *pt_beaux  ;
  struct bebux  *pt_bebux  ;
  struct beout  *pt_beout  ;
  struct bebus  *pt_bebus  ;
  struct bemsg  *pt_bemsg  ;
  struct chain  *pt_chain  ;

  pt_chain = pt_berin->REG_REF;
  while (pt_chain != NULL)
    {
    pt_bereg  = (struct bereg  *) pt_chain->DATA;
    pt_wrireg = (struct wrireg *)
                beh_chktab (dict, pt_bereg->NAME, ins_name, VHL_PNTDFN);
    list      = addptype (list, 'R', pt_wrireg);
    pt_chain  = pt_chain->NEXT;
    }

  pt_chain = pt_berin->AUX_REF;
  while (pt_chain != NULL)
    {
    pt_beaux  = (struct beaux  *) pt_chain->DATA;
    pt_wriaux = (struct wriaux *)
                beh_chktab (dict, pt_beaux->NAME, ins_name, VHL_PNTDFN);
    list      = addptype (list, 'X', pt_wriaux);
    pt_chain  = pt_chain->NEXT;
    }

  pt_chain = pt_berin->DLY_REF;
  while (pt_chain != NULL)
    {
    pt_beaux  = (struct beaux  *) pt_chain->DATA;
    pt_wriaux = (struct wriaux *)
                beh_chktab (dict, pt_beaux->NAME, ins_name, VHL_PNTDFN);
    list      = addptype (list, 'D', pt_wriaux);
    pt_chain  = pt_chain->NEXT;
    }

  pt_chain = pt_berin->BUX_REF;
  while (pt_chain != NULL)
    {
    pt_bebux  = (struct bebux  *) pt_chain->DATA;
    pt_wribux = (struct wribux *)
                beh_chktab (dict, pt_bebux->NAME, ins_name, VHL_PNTDFN);
    list      = addptype (list, 'U', pt_wribux);
    pt_chain  = pt_chain->NEXT;
    }

  pt_chain = pt_berin->OUT_REF;
  while (pt_chain != NULL)
    {
    pt_beout  = (struct beout  *) pt_chain->DATA;
    pt_wrssig = (struct wrssig *)
                beh_chktab (dict, pt_beout->NAME, ins_name, VHL_PNTDFN);
    list      = addptype (list, 'S', pt_wrssig);
    pt_chain  = pt_chain->NEXT;
    }

  pt_chain = pt_berin->BUS_REF;
  while (pt_chain != NULL)
    {
    pt_bebus  = (struct bebus  *) pt_chain->DATA;
    pt_wrbsig = (struct wrbsig *)
                beh_chktab (dict, pt_bebus->NAME, ins_name, VHL_PNTDFN);
    list      = addptype (list, 'B', pt_wrbsig);
    pt_chain  = pt_chain->NEXT;
    }

  pt_chain = pt_berin->MSG_REF;
  while (pt_chain != NULL)
    {
    pt_bemsg  = (struct bemsg  *) pt_chain->DATA;
    pt_wrimsg = (struct wrimsg *)
                beh_chktab (dict, pt_bemsg, ins_name, VHL_PNTDFN);
    list      = addptype (list, 'M', pt_wrimsg);
    pt_chain  = pt_chain->NEXT;
    }

  return (list);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addprjbvl						*/
/* description	: Used to add an projected bi-value to an wrbsig	*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct prjbvl *vhl_addprjbvl (lastprjbvl)

struct prjbvl *lastprjbvl;

  {
  struct prjbvl *pt_prjbvl;

  pt_prjbvl = (struct prjbvl *) vhu_alloc (sizeof(struct prjbvl), VHU_ALIGN);

  pt_prjbvl->CNDVAL = 0;
  pt_prjbvl->DRVVAL = 0;

  pt_prjbvl->NEXT   = lastprjbvl;
  pt_prjbvl->GLNEXT = lastprjbvl;

  return (pt_prjbvl);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addprjrvl						*/
/* description	: Used to add an projected bi-value to an wrireg	*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct prjrvl *vhl_addprjrvl (lastprjrvl)

struct prjrvl *lastprjrvl;

  {
  struct prjrvl *pt_prjrvl;

  pt_prjrvl = (struct prjrvl *) vhu_alloc (sizeof(struct prjrvl), VHU_ALIGN);

  pt_prjrvl->CNDVAL = 0;
  pt_prjrvl->DRVVAL = 0;

  pt_prjrvl->NEXT   = lastprjrvl;

  return (pt_prjrvl);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addredlst						*/
/* description	: Used to add an redlst to an lkdins			*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct redlst *vhl_addredlst (count)

unsigned int count;

  {
  struct redlst *pt_redlst;
  struct redlst *pnt      ;
  int            i        ;

  pt_redlst = (struct redlst *)
              vhu_alloc (count * sizeof(struct redlst), VHU_ALIGN);

  pnt = pt_redlst;
  for (i=1 ; i<count ; i++)
    {
    pnt->VALU = NULL   ;
    pnt->NEXT = pnt + 1;
    pnt++;
    }
  pnt->VALU = NULL;
  pnt->NEXT = NULL;

  return (pt_redlst);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addsimsig						*/
/* description	: Used to add an structure simsig to an lkdfig		*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct simsig *vhl_addsimsig (lastsimsig, pt_losig)

struct simsig *lastsimsig;
struct losig  *pt_losig  ;

  {
  struct simsig *pt_simsig;

  pt_simsig = (struct simsig *) vhu_alloc (sizeof(struct simsig), VHU_ALIGN);

  pt_simsig->NAMECHAIN = pt_losig->NAMECHAIN;
  pt_simsig->DEPEND    = NULL;
  pt_simsig->CURVAL    = (unsigned char *)
                         vhu_alloc (sizeof(unsigned char), VHU_NOALIGN);
  pt_simsig->PRJVAL    = 0;
  *(pt_simsig->CURVAL) = 0;
  pt_simsig->NEXT      = lastsimsig;

  return (pt_simsig);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addbussig						*/
/* description	: Used to add an structure bussig to an lkdfig		*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct bussig *vhl_addbussig (lastbussig, pt_losig)

struct bussig *lastbussig;
struct losig  *pt_losig  ;

  {
  struct bussig *pt_bussig;

  pt_bussig = (struct bussig *) vhu_alloc (sizeof(struct bussig), VHU_ALIGN);

  pt_bussig->NAMECHAIN = pt_losig->NAMECHAIN;
  pt_bussig->DEPEND    = NULL;
  pt_bussig->CURVAL    = (unsigned char *)
                         vhu_alloc (sizeof(unsigned char), VHU_NOALIGN);

  pt_bussig->PRJBVL    = vhl_addprjbvl (NULL);
  pt_bussig->TYPE      = pt_losig->USER->TYPE;
  pt_bussig->ERRFLG    = 0;
  pt_bussig->NEXT      = lastbussig;

  *(pt_bussig->CURVAL) = 0;

  return (pt_bussig);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addwrbsig						*/
/* description	: add a list (array) of wrbsig structures to an		*/
/*		  instance and fill the dictionary. For a C model add a	*/
/*		  driver (prjbvl) for each bussed output port		*/
/* called funct	: beh_addtab, vhu_alloc, vhl_addprjbvl			*/
/* ###--------------------------------------------------------------### */

struct wrbsig *vhl_addwrbsig (dict, pt_lkdins, pt_bebus, count)

struct beden  **dict     ;
struct lkdins  *pt_lkdins;
struct bebus   *pt_bebus ;
unsigned int    count    ;

  {
  struct wrbsig *pt_wrbsig = NULL;
  struct wrbsig *pt_tmpsig = NULL;
  char          *ins_name  = pt_lkdins->INSNAME;
  struct binode *pt_binode ;
  unsigned int   i         ;

  if (count != 0)
    {
    pt_wrbsig = (struct wrbsig *)
                vhu_alloc (sizeof(struct wrbsig) * count, VHU_ALIGN);

    pt_tmpsig = pt_wrbsig;
    for (i=0 ; i<count ; i++)
      {
      pt_tmpsig->PRJBVL = NULL     ;
      pt_tmpsig->BUSSIG = NULL     ;
      pt_tmpsig->EVAL   = 0        ;
      pt_tmpsig->LEVEL  = 0        ;
      pt_tmpsig->LKDINS = pt_lkdins;
      pt_tmpsig->BEBUS  = pt_bebus ;

      beh_addtab (dict, pt_bebus->NAME, ins_name, VHL_PNTDFN, pt_tmpsig);

      if (pt_lkdins->FLAG == VHL_CMODEL)
        pt_tmpsig->PRJBVL = vhl_addprjbvl (pt_tmpsig->PRJBVL);
      else
        {
        pt_binode = pt_bebus->BINODE;
        while (pt_binode != NULL)
          {
          pt_tmpsig->PRJBVL = vhl_addprjbvl (pt_tmpsig->PRJBVL);
          pt_binode         = pt_binode->NEXT;
          }
        }

      pt_tmpsig->NEXT = pt_tmpsig + 1 ;
      pt_bebus        = pt_bebus->NEXT;
      pt_tmpsig ++;
      }

    (pt_tmpsig - 1)->NEXT = NULL;
    }

  return (pt_wrbsig);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addwrireg						*/
/* description	: Used to add an structure wrireg to an lkdfig		*/
/* called funct	: vhu_alloc, vhl_addprjrvl				*/
/* ###--------------------------------------------------------------### */

struct wrireg *vhl_addwrireg (pt_lkdins, pt_bereg)

struct lkdins *pt_lkdins;
struct bereg  *pt_bereg ;

  {
  struct wrireg *pt_wrireg;
  struct binode *pt_binode;

  pt_wrireg = (struct wrireg *) vhu_alloc (sizeof(struct wrireg), VHU_ALIGN);

  pt_wrireg->LKDINS    = pt_lkdins;
  pt_wrireg->DEPEND    = NULL     ;
  pt_wrireg->BEREG     = pt_bereg ;
  pt_wrireg->PRJRVL    = NULL     ;
  pt_wrireg->EVAL      = 0        ;
  pt_wrireg->ERRFLG    = 0        ;

  pt_wrireg->CURVAL    = (unsigned char *)
                         vhu_alloc (sizeof(unsigned char), VHU_NOALIGN);
  *(pt_wrireg->CURVAL) = 0 ;

  pt_binode = pt_bereg->BINODE;
  while (pt_binode != NULL)
    {
    pt_wrireg->PRJRVL = vhl_addprjrvl (pt_wrireg->PRJRVL);
    pt_binode         = pt_binode->NEXT;
    }

  pt_wrireg->NEXT   = pt_lkdins->WRIREG;
  pt_lkdins->WRIREG = pt_wrireg;

  return (pt_wrireg);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addwribux						*/
/* description	: Used to add an structure wribux to an lkdfig		*/
/* called funct	: vhu_alloc, vhl_addprjbvl				*/
/* ###--------------------------------------------------------------### */

struct wribux *vhl_addwribux (pt_lkdins, pt_bebux)

struct lkdins *pt_lkdins;
struct bebux  *pt_bebux ;

  {
  struct wribux *pt_wribux;
  struct binode *pt_binode;

  pt_wribux = (struct wribux *) vhu_alloc (sizeof(struct wribux), VHU_ALIGN);

  pt_wribux->LKDINS    = pt_lkdins;
  pt_wribux->DEPEND    = NULL     ;
  pt_wribux->BEBUX     = pt_bebux ;
  pt_wribux->PRJBVL    = NULL     ;
  pt_wribux->EVAL      = 0        ;
  pt_wribux->ERRFLG    = 0        ;
  pt_wribux->LEVEL     = 0        ;

  pt_wribux->CURVAL    = (unsigned char *)
                         vhu_alloc (sizeof(unsigned char), VHU_NOALIGN);
  *(pt_wribux->CURVAL) = 0;

  pt_binode = pt_bebux->BINODE;
  while (pt_binode != NULL)
    {
    pt_wribux->PRJBVL = vhl_addprjbvl (pt_wribux->PRJBVL);
    pt_binode         = pt_binode->NEXT;
    }

  pt_wribux->NEXT   = pt_lkdins->WRIBUX;
  pt_lkdins->WRIBUX = pt_wribux        ;

  return (pt_wribux);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addwrimsg						*/
/* description	: add an assert (WRIMSG) structure to an instance	*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct wrimsg *vhl_addwrimsg (pt_lkdins, pt_bemsg)

struct lkdins *pt_lkdins;
struct bemsg  *pt_bemsg ;

  {
  struct wrimsg *pt_wrimsg;

  pt_wrimsg = (struct wrimsg *) vhu_alloc (sizeof(struct wrimsg), VHU_ALIGN);

  pt_wrimsg->LKDINS    = pt_lkdins;
  pt_wrimsg->BEMSG     = pt_bemsg ;
  pt_wrimsg->EVAL      = 0        ;
  pt_wrimsg->ERRFLG    = 0        ;
  pt_wrimsg->LEVEL     = 0        ;

  pt_wrimsg->NEXT      = pt_lkdins->WRIMSG;
  pt_lkdins->WRIMSG    = pt_wrimsg        ;

  return (pt_wrimsg);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addwriaux						*/
/* description	: Used to add an structure wriaux to an instance	*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct wriaux *vhl_addwriaux (pt_lkdins, pt_beaux, flag)

struct lkdins *pt_lkdins;
struct beaux  *pt_beaux ;
char           flag     ;

  {
  struct wriaux *pt_wriaux;

  pt_wriaux = (struct wriaux *) vhu_alloc (sizeof(struct wriaux), VHU_ALIGN);

  pt_wriaux->LKDINS    = pt_lkdins;
  pt_wriaux->BEAUX     = pt_beaux ;
  pt_wriaux->DEPEND    = NULL     ;
  pt_wriaux->PRJVAL    = 0        ;
  pt_wriaux->LEVEL     = 0        ;
  pt_wriaux->EVAL      = 0        ;
  pt_wriaux->FLAG      = flag     ;

  pt_wriaux->CURVAL    = (unsigned char *)
                         vhu_alloc (sizeof(unsigned char), VHU_NOALIGN);
  *(pt_wriaux->CURVAL) = 0;

  pt_wriaux->NEXT   = pt_lkdins->WRIAUX;
  pt_lkdins->WRIAUX = pt_wriaux        ;

  return (pt_wriaux);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addwrssig						*/
/* description	: add a list (array) of wrssig structures to an		*/
/*		  instance and fill the dictionary			*/
/* called funct	: vhu_alloc, beh_addtab					*/
/* ###--------------------------------------------------------------### */

struct wrssig *vhl_addwrssig (dict, pt_lkdins, pt_beout, count)

struct beden  **dict     ;
struct lkdins  *pt_lkdins;
struct beout   *pt_beout ;
unsigned int    count    ;

  {
  struct wrssig *pt_wrssig = NULL              ;
  struct wrssig *pt_tmpsig = NULL              ;
  char          *ins_name  = pt_lkdins->INSNAME;
  unsigned int   i         ;

  if (count != 0)
    {
    pt_wrssig = (struct wrssig *)
                vhu_alloc (sizeof(struct wrssig) * count, VHU_ALIGN);

    pt_tmpsig = pt_wrssig;
    for (i=0 ; i<count ; i++)
      {
      pt_tmpsig->SIMSIG = NULL          ;
      pt_tmpsig->EVAL   = 0             ;
      pt_tmpsig->LEVEL  = 0             ;
      pt_tmpsig->LKDINS = pt_lkdins     ;
      pt_tmpsig->NEXT   = pt_tmpsig + 1 ;
      pt_tmpsig->BEOUT  = pt_beout      ;
      beh_addtab (dict, pt_beout->NAME, ins_name, VHL_PNTDFN, pt_tmpsig);

      pt_beout          = pt_beout->NEXT;
      pt_tmpsig ++;
      } 
    (pt_tmpsig - 1)->NEXT = NULL;
    }

  return (pt_wrssig);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addlkdins						*/
/* description	: prepare an empty LKDINS structure and then fill its	*/
/*		  fields.						*/
/* called funct	: vhl_addwrbsig, vhl_addwrireg, vhl_addredlst,		*/
/*		  vhl_addwriaux, vhl_addwribux, vhu_toolbug  ,		*/
/*		  beh_chktab   , beh_addtab   , reverse      ,		*/
/*		  vhl_addwrimsg, vhu_alloc    , addchain     ,		*/
/*		  vhl_makdepend,					*/
/* ###--------------------------------------------------------------### */

struct lkdins *vhl_addlkdins (dict, pt_lkdfig, pt_loins)

struct beden   **dict     ;
struct lkdfig   *pt_lkdfig;
struct loins    *pt_loins ;

  {
  unsigned int   count       ;
  struct locon  *pt_locon    ;
  struct losig  *pt_losig    ;
  struct befig  *pt_befig    ;
  struct berin  *pt_berin    ;
  struct bepor  *pt_bepor    ;
  struct beout  *pt_beout    ;
  struct bebus  *pt_bebus    ;
  struct bebux  *pt_bebux    ;
  struct bereg  *pt_bereg    ;
  struct beaux  *pt_beaux    ;
  struct bemsg  *pt_bemsg    ;
  struct lkdins *pt_lkdins   ;
  struct redlst *pt_redlst   ;
  struct simsig *pt_simsig   ;
  struct bussig *pt_bussig   ;
  struct wrssig *pt_wrssig   ;
  struct wrbsig *pt_wrbsig   ;
  struct wriaux *pt_wriaux   ;
  struct wribux *pt_wribux   ;
  struct wrireg *pt_wrireg   ;
  struct wrimsg *pt_wrimsg   ;
  struct prjbvl *pt_prjbvl   ;
  struct ptype  *dep_lst     ;
  void          *wrt_pnt     ;
  void          *pt_sig      ;
  char          *ins_name    ;
  char          *con_name    ;
  int            sig_typ     ;
  void         (*fonc_pnt) ();

  pt_lkdins          = (struct lkdins *)
                       vhu_alloc (sizeof(struct lkdins), VHU_ALIGN);
  pt_lkdins->INSNAME = pt_loins->INSNAME;
  ins_name           = pt_loins->INSNAME;
  pt_lkdins->EVAL    = 1;

	/* ###------------------------------------------------------### */
	/*    Extract from the dictionary the pointer of the BEFIG	*/
	/* related to the instance					*/
	/* ###------------------------------------------------------### */

  pt_befig = (struct befig *)
             beh_chktab (dict, pt_loins->FIGNAME, 1, VHL_PNTDFN);

  if (pt_befig == NULL)
    vhu_toolbug (13, "vhl_addlkdins", pt_loins->INSNAME, 0);

	/* ###------------------------------------------------------### */
	/*    Link with the instance's model				*/
	/* ###------------------------------------------------------### */

  if ((fonc_pnt = vhl_iscmodel (pt_befig->NAME)) != NULL)
    {
    pt_lkdins->BEFIG   = (struct befig *) fonc_pnt;
    pt_lkdins->FLAG    = VHL_CMODEL;
    }
  else
    {
    pt_lkdins->BEFIG   = pt_befig;
    pt_lkdins->FLAG    = VHL_VMODEL;
    }

  pt_lkdins->WRSSIG = NULL;
  pt_lkdins->WRBSIG = NULL;
  pt_lkdins->WRIAUX = NULL;
  pt_lkdins->WRIBUX = NULL;
  pt_lkdins->WRIREG = NULL;
  pt_lkdins->WRIMSG = NULL;
  pt_lkdins->REDLST = NULL;
  pt_lkdins->CIRCUI = pt_befig->CIRCUI;

  pt_lkdins->NEXT   = pt_lkdfig->LKDINS;
  pt_lkdfig->LKDINS = pt_lkdins;

	/* ###------------------------------------------------------### */
	/*    For each BERIN create an empty REDLST (due to evaluation	*/
	/* constraints redlst is organised as a table - see vhx_evalue)	*/
	/* ###------------------------------------------------------### */

  count   = 0;
  pt_berin = pt_befig->BERIN;
  while (pt_berin != NULL)
    {
    count++;
    pt_berin = pt_berin->NEXT;
    }

  if (count == 0)
    pt_lkdins->REDLST = NULL;
  else
    pt_lkdins->REDLST = vhl_addredlst (count);

	/* ###------------------------------------------------------### */
	/*    For each BEOUT create an empty WRSSIG			*/
	/* ###------------------------------------------------------### */

  count = 0;
  pt_beout = pt_befig->BEOUT;
  while (pt_beout != NULL)
    {
    count++;
    pt_beout = pt_beout->NEXT;
    }

  pt_lkdins->WRSSIG = vhl_addwrssig (dict, pt_lkdins, pt_befig->BEOUT, count);

	/* ###------------------------------------------------------### */
	/*    For each BEBUS create an WRBSIG				*/
	/* ###------------------------------------------------------### */

  count = 0;
  pt_bebus = pt_befig->BEBUS;
  while (pt_bebus != NULL)
    {
    count++;
    pt_bebus = pt_bebus->NEXT;
    }

  pt_lkdins->WRBSIG = vhl_addwrbsig (dict, pt_lkdins, pt_befig->BEBUS, count);

	/* ###------------------------------------------------------### */
	/*    if the instance's model is a VHDL file ...		*/
	/* ###------------------------------------------------------### */

  if (pt_lkdins->FLAG == VHL_VMODEL)
    {

	/* ###------------------------------------------------------### */
	/*    For each BEBUX create an WRIBUX. The WRIBUX list must be	*/
	/* reversed to obtain the same order of ports as in BEBUX list.	*/
	/*    Scan the BERIN list (and REDLST) to find the primary input*/
	/* that has the same name and make a link			*/
	/* ###------------------------------------------------------### */

    pt_bebux = pt_befig->BEBUX;
    while (pt_bebux != NULL)
      {
      pt_lkdins->WRIBUX = vhl_addwribux (pt_lkdins, pt_bebux);
      pt_wribux         = pt_lkdins->WRIBUX;

      beh_addtab (dict, pt_bebux->NAME, ins_name, VHL_PNTDFN, pt_wribux);
      beh_addtab (dict, pt_bebux->NAME, ins_name, VHL_SIGDFN, 'U'      );

      pt_bebux = pt_bebux->NEXT;
      }
    pt_lkdins->WRIBUX = (struct wribux *)
                        reverse ((struct chain *) pt_lkdins->WRIBUX);

	/* ###------------------------------------------------------### */
	/*    For each BEREG create an WRIREG. The WRIREG list must be	*/
	/* reversed to obtain the same order of ports as in BEREG list.	*/
	/*    Scan the BERIN list (and REDLST) to find the primary input*/
	/* that has the same name and make a link			*/
	/* ###------------------------------------------------------### */

    pt_bereg = pt_befig->BEREG;

    while (pt_bereg != NULL)
      {
      pt_lkdins->WRIREG = vhl_addwrireg (pt_lkdins, pt_bereg);
      pt_wrireg         = pt_lkdins->WRIREG;

      beh_addtab (dict, pt_bereg->NAME, ins_name, VHL_PNTDFN, pt_wrireg);
      beh_addtab (dict, pt_bereg->NAME, ins_name, VHL_SIGDFN, 'R'      );

      pt_bereg = pt_bereg->NEXT;
      }
    pt_lkdins->WRIREG = (struct wrireg *)
                        reverse ((struct chain *)pt_lkdins->WRIREG);

	/* ###------------------------------------------------------### */
	/*    For each BEDLY create an WRIAUX.				*/
	/* ###------------------------------------------------------### */

    pt_beaux = pt_befig->BEDLY;
    while (pt_beaux != NULL)
      {
      pt_lkdins->WRIAUX = vhl_addwriaux (pt_lkdins, pt_beaux, 'D');
      pt_wriaux         = pt_lkdins->WRIAUX;

      beh_addtab (dict, pt_beaux->NAME, ins_name, VHL_PNTDFN, pt_wriaux);
      beh_addtab (dict, pt_beaux->NAME, ins_name, VHL_SIGDFN, 'D'      );

      pt_beaux = pt_beaux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    For each BEAUX create an WRIAUX.				*/
	/* ###------------------------------------------------------### */

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      pt_lkdins->WRIAUX = vhl_addwriaux (pt_lkdins, pt_beaux, 'X');
      pt_wriaux         = pt_lkdins->WRIAUX;

      beh_addtab (dict, pt_beaux->NAME, ins_name, VHL_PNTDFN, pt_wriaux);
      beh_addtab (dict, pt_beaux->NAME, ins_name, VHL_SIGDFN, 'X'      );

      pt_beaux = pt_beaux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    The WRIAUX list must be reversed to obtain the same order	*/
	/* of as in BEAUX (top) and BEDLY (bottom) list.		*/
	/* ###------------------------------------------------------### */

    pt_lkdins->WRIAUX = (struct wriaux *)
                        reverse ((struct chain *) pt_lkdins->WRIAUX);

	/* ###------------------------------------------------------### */
	/*    for each assert instruction in the behavioural description*/
	/* (BEFIG) create a corresponding structure (WRIMSG) in the	*/
	/* instance.							*/
	/* ###------------------------------------------------------### */

    pt_bemsg = pt_befig->BEMSG;
    while (pt_bemsg != NULL)
      {
      pt_lkdins->WRIMSG = vhl_addwrimsg (pt_lkdins, pt_bemsg);
      pt_wrimsg         = pt_lkdins->WRIMSG;

      beh_addtab (dict, pt_bemsg, ins_name, VHL_PNTDFN, pt_wrimsg);
      pt_bemsg = pt_bemsg->NEXT;
      }
    }

	/* ###------------------------------------------------------### */
	/*    scan the port list of the instance (LOINS) looking for	*/
	/* simple and bussed outputs. Link the corresponding structure	*/
	/* (WRSSIG or WRBSIG) to the signal connected to the port (the	*/
	/* SIMSIG or the BUSSIG that corresponds to the LOSIG)		*/
	/* ###------------------------------------------------------### */

  pt_locon = pt_loins->LOCON;
  while (pt_locon != NULL)
    {
    con_name = pt_locon->NAME;
    pt_losig  = pt_locon->SIG;

    switch (pt_locon->DIRECTION)
      {

      case 'O' :
        pt_wrssig = (struct wrssig *)
                    beh_chktab (dict, con_name, ins_name, VHL_PNTDFN);
        pt_simsig = (struct simsig *)
                    beh_chktab (dict, pt_losig, NULL    , VHL_PNTDFN);

        pt_wrssig->SIMSIG = pt_simsig;
        break;

      case 'Z' :
        pt_wrbsig = (struct wrbsig *)
                    beh_chktab (dict, con_name, ins_name, VHL_PNTDFN);
        pt_bussig = (struct bussig *)
                    beh_chktab (dict, pt_losig, NULL    , VHL_PNTDFN);

        pt_wrbsig->BUSSIG = pt_bussig;
        pt_prjbvl         = pt_bussig->PRJBVL;

        while (pt_prjbvl->GLNEXT != NULL)
          pt_prjbvl = pt_prjbvl->GLNEXT;
        pt_prjbvl->GLNEXT = pt_wrbsig->PRJBVL;
        break;

      case 'I' :
      case 'B' :
      case 'T' :
        break;

      default:
        vhu_toolbug (18, "vhl_addlkdins", ins_name, pt_locon->DIRECTION);
      }

    pt_locon = pt_locon->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    scan the input list and make the link ...			*/
	/* ###------------------------------------------------------### */

  pt_berin  = pt_befig->BERIN;
  pt_redlst = pt_lkdins->REDLST;

  while (pt_berin != NULL)
    {
    if (pt_lkdins->FLAG == VHL_VMODEL)
      {
      dep_lst = vhl_makdepend (dict, pt_berin, ins_name);
/*-----
      dep_lst = addptype (dep_lst, 'I', pt_lkdins);
------*/
      }
    else
      dep_lst = addptype (NULL, 'I', pt_lkdins);

    wrt_pnt = (void *) beh_chktab (dict, pt_berin->NAME, ins_name, VHL_PNTDFN);
    sig_typ =          beh_chktab (dict, pt_berin->NAME, ins_name, VHL_SIGDFN);

    switch (sig_typ)
      {
	/* ###------------------------------------------------------### */
	/*    for internal signals (bussed internal signal, simple or	*/
	/* delayed signal and internal registers), make a link to the	*/
	/* current value.						*/
	/* ###------------------------------------------------------### */

      case 'U':
        pt_redlst->VALU = ((struct wribux *) wrt_pnt)->CURVAL;
        ((struct wribux *) wrt_pnt)->DEPEND = dep_lst;
        break;

      case 'D':
      case 'X':
        pt_redlst->VALU = ((struct wriaux *) wrt_pnt)->CURVAL;
        ((struct wriaux *) wrt_pnt)->DEPEND = dep_lst;
        break;

      case 'R':
        pt_redlst->VALU = ((struct wrireg *) wrt_pnt)->CURVAL;
        ((struct wrireg *) wrt_pnt)->DEPEND = dep_lst;
        break;

	/* ###------------------------------------------------------### */
	/*    for external signals (input or inout ports) :		*/
	/*     - scan the port list of the instance (LOINS) and find	*/
	/*       the signal connected to the port. Make a link to the	*/
	/*       current value of that signal.				*/
	/*     - for bussed inout ports, chain projected values through	*/
	/*       the global next (GLNEXT) pointer			*/
	/* ###------------------------------------------------------### */

      case 0 :
        pt_locon = pt_loins->LOCON;
        while (pt_locon != NULL)
          {
          if (pt_locon->NAME == pt_berin->NAME)
            break;
          pt_locon = pt_locon->NEXT;
          }

        if (pt_locon == NULL)
          vhu_toolbug (20, "vhl_addlkdins", pt_berin->NAME, 0);
        else
          {
          pt_sig    = (void *)beh_chktab (dict, pt_locon->SIG, NULL, VHL_PNTDFN);
          pt_bussig = (struct bussig *) pt_sig;
          pt_simsig = (struct simsig *) pt_sig;

          switch (pt_locon->DIRECTION)
            {
            case 'I':
              if ((char) beh_chktab (dict, pt_losig, NULL, VHL_SIGDFN) == 'B')
                {
                pt_bussig->DEPEND = (struct ptype *)
                                    append ((struct chain *) pt_bussig->DEPEND,
                                            (struct chain *) dep_lst          );

                pt_redlst->VALU   = pt_bussig->CURVAL;
                }
              else
                {
                pt_simsig->DEPEND = (struct ptype *)
                                    append ((struct chain *) pt_simsig->DEPEND,
                                            (struct chain *) dep_lst          );

                pt_redlst->VALU   = pt_simsig->CURVAL;
                }
              break;

            case 'B':
              ((struct wrssig *) wrt_pnt)->SIMSIG = pt_simsig;
              pt_simsig->DEPEND = (struct ptype *)
                                  append ((struct chain *) pt_simsig->DEPEND,
                                          (struct chain *) dep_lst          );

              pt_redlst->VALU   = pt_simsig->CURVAL;
              break;

            case 'T':
              ((struct wrbsig *) wrt_pnt)->BUSSIG = pt_bussig;
              pt_bussig->DEPEND = (struct ptype *)
                                  append ((struct chain *) pt_bussig->DEPEND,
                                          (struct chain *) dep_lst          );

              pt_redlst->VALU   = pt_bussig->CURVAL;
              pt_prjbvl         = pt_bussig->PRJBVL;

              while (pt_prjbvl->GLNEXT != NULL)
                pt_prjbvl = pt_prjbvl->GLNEXT;
              pt_prjbvl->GLNEXT = ((struct wrbsig *) wrt_pnt)->PRJBVL;

              break;

            default :
              vhu_toolbug (21, "vhl_addlkdins", pt_locon->NAME, pt_locon->DIRECTION);
            }
          }
        break;

      default :
        vhu_toolbug (19, "vhl_addlkdins", pt_berin->NAME, sig_typ);
      }

    pt_berin = pt_berin->NEXT;
    pt_redlst ++;
    }

  return (pt_lkdins);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addlkdfig						*/
/* description	: prepare an empty LKDFIG structure			*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct lkdfig *vhl_addlkdfig (pt_lofig)

struct lofig *pt_lofig;

  {
  struct lkdfig *pt_lkdfig;

  pt_lkdfig         = (struct lkdfig *)
                      vhu_alloc (sizeof(struct lkdfig), VHU_ALIGN);

  pt_lkdfig->NEXT   = VHL_HEDLKF    ;
  pt_lkdfig->NAME   = pt_lofig->NAME;
  pt_lkdfig->LKDINS = NULL          ;
  pt_lkdfig->SIMSIG = NULL          ;
  pt_lkdfig->BUSSIG = NULL          ;
  pt_lkdfig->VALSIZ = 0             ;

  VHL_HEDLKF = pt_lkdfig;

  return (pt_lkdfig);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_makgst						*/
/* description	: This function creates a ghost structure level to	*/
/*		  handle fully behavioural descriptions			*/
/* called funct	: addlofig, addlosig, addchain, vhu_alloc		*/
/* ###--------------------------------------------------------------### */

struct lofig *vhl_makgst (pt_befig)

struct befig *pt_befig;

  {
  struct chain *name_lst       ;
  struct locon *pt_locon       ;
  struct bepor *pt_bepor       ;
  struct lofig *pt_lofig       ;
  struct loins *pt_loins       ;
  struct losig *pt_losig       ;
  int           index          = 0;
  char          lofigname [80] ;

  sprintf (lofigname, "root_of_%s", pt_befig->NAME);
  pt_lofig = addlofig (lofigname);

  pt_loins = (struct loins *) vhu_alloc (sizeof(struct loins), VHU_ALIGN);

  pt_loins->INSNAME = pt_befig->NAME;
  pt_loins->FIGNAME = pt_befig->NAME;
  pt_loins->LOCON   = NULL;
  pt_loins->USER    = NULL;
  pt_loins->NEXT    = NULL;

  pt_lofig->LOINS = pt_loins;

  pt_bepor = pt_befig->BEPOR;
  while (pt_bepor != NULL)
    {
    name_lst      = addchain (NULL    , pt_bepor->NAME);
    pt_losig      = addlosig (pt_lofig, index, name_lst, 'E');

    if (pt_bepor->TYPE != 'B')
      pt_losig->USER = addptype (pt_losig->USER, pt_bepor->TYPE, NULL);

    pt_locon            = (struct locon *)
                          vhu_alloc (sizeof(struct locon), VHU_ALIGN);
    pt_locon->NAME      = pt_bepor->NAME     ;
    pt_locon->SIG       = pt_losig           ;
    pt_locon->ROOT      = (void *) pt_lofig  ;
    pt_locon->DIRECTION = pt_bepor->DIRECTION;
    pt_locon->TYPE      = 'E'                ;
    pt_locon->USER      = NULL               ;
    pt_locon->NEXT      = pt_lofig->LOCON    ;
    pt_lofig->LOCON     = pt_locon           ;

    pt_locon            = (struct locon *)
                          vhu_alloc (sizeof(struct locon), VHU_ALIGN);
    pt_locon->NAME      = pt_bepor->NAME     ;
    pt_locon->SIG       = pt_losig           ;
    pt_locon->ROOT      = (void *) pt_loins  ;
    pt_locon->DIRECTION = pt_bepor->DIRECTION;
    pt_locon->TYPE      = 'I'                ;
    pt_locon->USER      = NULL               ;
    pt_locon->NEXT      = pt_loins->LOCON    ;
    pt_loins->LOCON     = pt_locon           ;

    pt_bepor = pt_bepor->NEXT;
    index ++;
    }
  pt_lofig->LOCON = (struct locon *) reverse ((struct chain *) pt_lofig->LOCON);
  pt_loins->LOCON = (struct locon *) reverse ((struct chain *) pt_loins->LOCON);

  return (pt_lofig);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_linker						*/
/* description	: This function generates the structure used by the	*/
/*		  simulator						*/
/* called funct	: beh_addtab   , vhu_toolbug  , vhl_addlkdfig,		*/
/*		  vhl_addsimsig, vhl_addbussig, getsigname   ,		*/
/*		  vhu_splitname, vhl_addlkdins, addptype     ,		*/
/*		  mbkfopen     , vhu_error    , beh_chktab   ,		*/
/* ###--------------------------------------------------------------### */

int vhl_linker (dict, pt_paseq)

struct beden     **dict    ;
struct paseq      *pt_paseq;

  {
  struct lofig  *pt_lofig  ;
  struct losig  *pt_losig  ;
  struct loins  *pt_loins  ;
  struct locon  *pt_locon  ;
  struct lkdfig *pt_lkdfig ;
  struct lkdins *pt_lkdins ;
  struct bussig *pt_bussig ;
  struct simsig *pt_simsig ;
  char          *name      ;
  char          *sig_name  ;
  char          *ins_name  ;
  int            sig_type  ;
  int            sig_mode  ;
  int            max_iol   ;
  int            i         ;
  struct paiol  *pt_paiol  ;
  struct pains  *pt_pains  ;
  struct befig  *pt_befig  ;
  struct bepor  *pt_bepor  ;
  unsigned int   port_count;
  FILE          *filepnt   ;
  int            errflg    = 0;

	/* ###------------------------------------------------------### */
	/*    Find the root model of the description by scanning the	*/
	/* list of LOFIGs til the end. (The last LOFIG must be the root)*/
	/* ###------------------------------------------------------### */

  if ((pt_lofig = HEAD_LOFIG) == NULL)
    vhu_toolbug (13, "vhl_linker", NULL, 0);

  while (pt_lofig->NEXT != NULL)
    pt_lofig = pt_lofig->NEXT;

  pt_lkdfig = vhl_addlkdfig (pt_lofig);

	/* ###------------------------------------------------------### */
	/*   For each LOCON of root save its direction in the		*/
	/* dictionary							*/
	/* ###------------------------------------------------------### */

  pt_locon = pt_lofig->LOCON;
  while (pt_locon != NULL)
    {
    beh_addtab (dict, pt_locon->NAME, NULL, VHL_MODDFN, pt_locon->DIRECTION);
    pt_locon = pt_locon->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*   Create a BUSSIG or a SIMSIG for each LOSIG of root		*/
	/* ###------------------------------------------------------### */

  pt_locon = pt_lofig->LOCON;
  while (pt_locon != NULL)
    {
    pt_losig = pt_locon->SIG;
    if (pt_losig->USER == NULL)
      {
      pt_lkdfig->SIMSIG = vhl_addsimsig (pt_lkdfig->SIMSIG, pt_losig);
      pt_simsig         = pt_lkdfig->SIMSIG;

      beh_addtab (dict, pt_losig      , NULL, VHL_PNTDFN, pt_simsig);
      beh_addtab (dict, pt_losig      , NULL, VHL_SIGDFN, 'S'      );
      beh_addtab (dict, pt_locon->NAME, NULL, VHL_PNTDFN, pt_simsig);
      beh_addtab (dict, pt_locon->NAME, NULL, VHL_SIGDFN, 'S'      );
      }
    else
      {
      pt_lkdfig->BUSSIG = vhl_addbussig (pt_lkdfig->BUSSIG, pt_losig);
      pt_bussig         = pt_lkdfig->BUSSIG;

      beh_addtab (dict, pt_losig      , NULL, VHL_PNTDFN, pt_bussig);
      beh_addtab (dict, pt_losig      , NULL, VHL_SIGDFN, 'B'      );
      beh_addtab (dict, pt_locon->NAME, NULL, VHL_PNTDFN, pt_bussig);
      beh_addtab (dict, pt_locon->NAME, NULL, VHL_SIGDFN, 'B'      );
      }

    pt_locon = pt_locon->NEXT;
    }

  pt_losig = pt_lofig->LOSIG;
  while (pt_losig != NULL)
    {
    if (pt_losig->TYPE == 'I')
      {
      name = getsigname (pt_losig);
      vhu_splitname (name, &ins_name, &sig_name);
      if (pt_losig->USER == NULL)
        {
        pt_lkdfig->SIMSIG = vhl_addsimsig (pt_lkdfig->SIMSIG, pt_losig);
        pt_simsig         = pt_lkdfig->SIMSIG;

        beh_addtab (dict, pt_losig, NULL    , VHL_PNTDFN, pt_simsig);
        beh_addtab (dict, pt_losig, NULL    , VHL_SIGDFN, 'S'      );
        beh_addtab (dict, sig_name, ins_name, VHL_PNTDFN, pt_simsig);
        beh_addtab (dict, sig_name, ins_name, VHL_SIGDFN, 'S'      );
        }
      else
        {
        pt_lkdfig->BUSSIG = vhl_addbussig (pt_lkdfig->BUSSIG, pt_losig);
        pt_bussig         = pt_lkdfig->BUSSIG;

        beh_addtab (dict, pt_losig, NULL    , VHL_PNTDFN, pt_bussig);
        beh_addtab (dict, pt_losig, NULL    , VHL_SIGDFN, 'B'      );
        beh_addtab (dict, sig_name, ins_name, VHL_PNTDFN, pt_bussig);
        beh_addtab (dict, sig_name, ins_name, VHL_SIGDFN, 'B'      );
        }
      }

    pt_losig = pt_losig->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*   Create a LKDINS for each LOINS of root			*/
	/* ###------------------------------------------------------### */

  pt_loins = pt_lofig->LOINS;
  while (pt_loins != NULL)
    {
    pt_lkdins = vhl_addlkdins (dict, pt_lkdfig, pt_loins);
    pt_loins  = pt_loins->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*   Link structures generated by the pattern compiler with the	*/
	/* description (link instances to be inspected)			*/
	/* ###------------------------------------------------------### */

  pt_pains = pt_paseq->PAINS;
  while (pt_pains != NULL)
    {

    pt_lkdins = pt_lkdfig->LKDINS;
    while (pt_lkdins != NULL)
      {
      if (pt_lkdins->INSNAME == pt_pains->INSNAME)
        break;
      pt_lkdins = pt_lkdins->NEXT;
      }

    if ((pt_lkdins != NULL) && (pt_lkdins->FLAG == VHL_VMODEL))
      {
      pt_pains->INSTANCE = (void *) pt_lkdins;

      if ((filepnt = mbkfopen (pt_pains->FILNAME, "pat", WRITE_TEXT)) == NULL)
        errflg = vhu_error (116, pt_pains->FILNAME, NULL);
      else
        {
        pt_pains->FILPNT = filepnt;
        port_count = 0;
        pt_befig   = pt_lkdins->BEFIG;
        pt_bepor   = pt_befig->BEPOR ;
        while (pt_bepor != NULL)
          {
          port_count ++;
          pt_bepor = pt_bepor->NEXT;
          }
/*------
        pt_pains->VALUE = (char **) mbkalloc (sizeof (char *) * port_count);
----*/
        }
      }
    else
      errflg = vhu_error (117, pt_pains->INSNAME, NULL);

    pt_pains = pt_pains->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*   Link structures generated by the pattern compiler with the	*/
	/* description (through the PAIOL structures)			*/
	/* ###------------------------------------------------------### */

  max_iol  = pt_paseq->IOLNBR;
  pt_paiol = pt_paseq->PAIOL ;

  for (i=0 ; i<max_iol ; i++)
    {
    vhu_splitname (pt_paiol->NAME, &ins_name, &sig_name);

    sig_type = beh_chktab (dict, sig_name      , ins_name, VHL_SIGDFN);
    sig_mode = beh_chktab (dict, pt_paiol->NAME, NULL    , VHL_MODDFN);
    if (sig_type != 0)
      {
      switch (pt_paiol->MODE)
        {
        case ('I'):
          if (sig_mode != 'I')
            errflg = vhu_error (103, pt_paiol->NAME, NULL);
          break;
        case ('T'):
          if (sig_mode != 'T')
            errflg = vhu_error (103, pt_paiol->NAME, NULL);
          break;
        case ('O'):
          if ((sig_mode != 'O') && (sig_mode != 'B') && (sig_mode != 'Z'))
            errflg = vhu_error (103, pt_paiol->NAME, NULL);
          else
            pt_paiol->MODE = sig_mode;
          break;
        case ('S'):
          if (sig_mode != 0)
            errflg = vhu_error (103, pt_paiol->NAME, NULL);
          else
            {
            switch (sig_type)
              {
              case 'B':
                pt_paiol->MODE = 'X'; break;
              case 'X':
                pt_paiol->MODE = 'S'; break;
              case 'S':
                pt_paiol->MODE = 'W'; break;
              case 'U':
                pt_paiol->MODE = 'U'; break;
              case 'R':
                errflg = vhu_error (103, pt_paiol->NAME, NULL);
                break;
              }
            }
          break;
        case ('R'):
          if (sig_type != 'R')
            errflg = vhu_error (103, pt_paiol->NAME, NULL);
          break;
        default :
           fprintf (stderr, "Linker Error : Mode is unknown.\n");
        }
      }
    else
      errflg = vhu_error (104, pt_paiol->NAME, NULL);

    pt_paiol->SIG = (void *) beh_chktab (dict, sig_name, ins_name, VHL_PNTDFN);
    pt_paiol ++;
    }

  return (errflg);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_restor						*/
/* description	: restore the mode of external ports of the description	*/
/*		  when the declared mode is LINKAGE.			*/
/*									*/
/*		  First, it restores (if necessary) the informations on	*/
/*		  internal ports checking the consistency between the	*/
/*		  model and the instance. Then, it propagates restored	*/
/*		  informations through the signals til the external	*/
/*		  ports. In the final step it checks the consistency of	*/
/*		  connexions on the signals				*/
/* called funct	: beh_initab , beh_chktab, beh_addtab, beh_fretab,	*/
/*		  vhu_toolbug, vhu_error , vhu_alloc , addptype		*/
/* ###--------------------------------------------------------------### */

int vhl_restor (dict, pt_lofig, head_befig)

struct beden     **dict      ;
struct lofig      *pt_lofig  ;
struct befig      *head_befig;

  {
  struct loins      *pt_loins    ;
  struct locon      *pt_locon    ;
  struct losig      *pt_losig    ;
  struct befig      *pt_befig    ;
  struct bepor      *pt_bepor    ;
  int                errflg      = 0;
  int                update_flg  ;
  int                mode        ;
  int                new_mode    ;
  int                receiv_nbr  ;
  int                emettr_nbr  ;
  int                connec_nbr  ;
  struct beden     **lcl_dic     ;

  if (pt_lofig != NULL)
    {
    lcl_dic = beh_initab ();

	/* ###------------------------------------------------------### */
	/*    Find the root model of the description by scanning the	*/
	/* list of LOFIGs til the end. (The last LOFIG must be the root)*/
	/* ###------------------------------------------------------### */

    while (pt_lofig->NEXT != NULL)
      pt_lofig = pt_lofig->NEXT;

	/* ###------------------------------------------------------### */
	/*    Fill the dictionary with all signals (all signals are to	*/
	/* be checked or restored)					*/
	/* ###------------------------------------------------------### */

    pt_losig = pt_lofig->LOSIG;
    while (pt_losig != NULL)
      {
      beh_addtab (lcl_dic, pt_losig, NULL, VHL_SIGDFN, VHL_UKNDFN);
      pt_losig = pt_losig->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    For each LOINS ... :					*/
	/* ###------------------------------------------------------### */

    pt_loins = pt_lofig->LOINS;
    while (pt_loins != NULL)
      {
	/* ###------------------------------------------------------### */
	/*    Scan the list of BEFIGs to find the model of the instance	*/
	/* ###------------------------------------------------------### */

      if ((pt_befig = (struct befig *)
                      beh_chktab(dict,pt_loins->FIGNAME,1,VHL_PNTDFN)) == NULL)
        {
        pt_befig = head_befig;
        while ((pt_befig != NULL) && (pt_befig->NAME != pt_loins->FIGNAME))
          pt_befig = pt_befig->NEXT;

        if (pt_befig == NULL)
          vhu_toolbug (13, "vhl_restor", pt_loins->INSNAME, 0);
        else
          beh_addtab (dict, pt_loins->FIGNAME, 1, VHL_PNTDFN, pt_befig);
        }

	/* ###------------------------------------------------------### */
	/*    Check consistency of the instance and its model :		*/
	/*      - for each port names must be the same			*/
	/* ###------------------------------------------------------### */

      pt_bepor = pt_befig->BEPOR;
      pt_locon = pt_loins->LOCON;
      while ((pt_bepor != NULL) && (pt_locon != NULL))
        {
        update_flg = 0;
        if (pt_bepor->NAME != pt_locon->NAME)
          errflg = vhu_error (102, pt_befig->NAME, NULL);
        else
          {

          if (pt_locon->DIRECTION != 'X')
            {

	/* ###------------------------------------------------------### */
	/*    if the LOCON's mode is not 'linkage', modes must be the	*/
	/* same. In such a case if the LOSIG has to be restored, read	*/
	/* the old RDW flags of the signal then update these flags.	*/
	/* ###------------------------------------------------------### */

            if (pt_bepor->DIRECTION != pt_locon->DIRECTION)
              errflg = vhu_error (102, pt_befig->NAME, NULL);
            else
              {
              if ((mode = beh_chktab (lcl_dic,pt_locon->SIG,NULL,VHL_SIGDFN)) != 0)
                {
                update_flg = 1;
                receiv_nbr = beh_chktab (lcl_dic,pt_locon->SIG,NULL,VHL_RNBDFN);
                emettr_nbr = beh_chktab (lcl_dic,pt_locon->SIG,NULL,VHL_WNBDFN);
                connec_nbr = beh_chktab (lcl_dic,pt_locon->SIG,NULL,VHL_CNBDFN);
                }
              }
            }
          else
            {

	/* ###------------------------------------------------------### */
	/*    if the LOCON's mode is 'linkage', restore the mode, read	*/
	/* the old RDW flags of the LOSIG then update these flags.	*/
	/* ###------------------------------------------------------### */

            pt_locon->DIRECTION = pt_bepor->DIRECTION;
            update_flg = 1;
            mode       = beh_chktab (lcl_dic,pt_locon->SIG,NULL,VHL_SIGDFN);
            receiv_nbr = beh_chktab (lcl_dic,pt_locon->SIG,NULL,VHL_RNBDFN);
            emettr_nbr = beh_chktab (lcl_dic,pt_locon->SIG,NULL,VHL_WNBDFN);
            connec_nbr = beh_chktab (lcl_dic,pt_locon->SIG,NULL,VHL_CNBDFN);
            }

	/* ###------------------------------------------------------### */
	/*    Update RDW flags						*/
	/* ###------------------------------------------------------### */

          if (update_flg == 1)
            {
            connec_nbr++;
            switch (pt_bepor->DIRECTION)
              {
              case 'I' :
                new_mode = mode | VHL_REDDFN;
                receiv_nbr++;
                break;
              case 'O' :
                new_mode = mode | VHL_WRTDFN;
                emettr_nbr++;
                break;
              case 'Z' :
                new_mode = mode | VHL_DECDFN;
                if ((pt_locon->SIG->USER == NULL)
                ||  (pt_locon->SIG->USER->DATA != NULL))
                  {
                  pt_locon->SIG->USER = addptype (NULL, pt_bepor->TYPE, NULL);
                  }
                else
                  {
                  if (pt_locon->SIG->USER->TYPE !=  pt_bepor->TYPE)
                    errflg = vhu_error (111, getsigname (pt_locon->SIG), NULL);
                  }
                emettr_nbr++;
                break;
              case 'B' :
                new_mode = mode | VHL_WRTDFN | VHL_REDDFN;
                receiv_nbr++;
                emettr_nbr++;
                break;
              case 'T' :
                new_mode = mode | VHL_DECDFN | VHL_REDDFN;
                if ((pt_locon->SIG->USER == NULL)
                ||  (pt_locon->SIG->USER->DATA != NULL))
                  {
                  pt_locon->SIG->USER = addptype (NULL, pt_bepor->TYPE, NULL);
                  }
                else
                  {
                  if (pt_locon->SIG->USER->TYPE !=  pt_bepor->TYPE)
                    errflg = vhu_error (111, getsigname (pt_locon->SIG), NULL);
                  }
                receiv_nbr++;
                emettr_nbr++;
                break;
              }
            beh_addtab (lcl_dic, pt_locon->SIG, NULL, VHL_SIGDFN, new_mode  );
            beh_addtab (lcl_dic, pt_locon->SIG, NULL, VHL_RNBDFN, receiv_nbr);
            beh_addtab (lcl_dic, pt_locon->SIG, NULL, VHL_WNBDFN, emettr_nbr);
            beh_addtab (lcl_dic, pt_locon->SIG, NULL, VHL_CNBDFN, connec_nbr);
            }
          }

        pt_bepor = pt_bepor->NEXT;
        pt_locon = pt_locon->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*    It is an error if there is not the same number of BEPOR	*/
	/* and LOCON							*/
	/* ###------------------------------------------------------### */

      if ((pt_bepor != NULL) || (pt_locon != NULL))
        errflg = vhu_error (102, pt_befig->NAME, NULL);

      pt_loins = pt_loins->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    Scan the list of external LOCONs :			*/
	/*								*/
	/*      - increment the number of ports connected to the signal	*/
	/*        (the external port connected to the signal)		*/
	/*      - restore the mode of linkage ports			*/
	/*      - update the number of drivers and receivers connected	*/
	/*        to the signal :					*/
	/*          - a port of mode in          is a driver		*/
	/*          - a port of mode out         is a receiver		*/
	/*          - a port of mode inout       is a receiver		*/
	/*          - a port of mode out   (bus) is a driver		*/
	/*          - a port of mode inout (bus) is a dr. and a rec.	*/
	/* ###------------------------------------------------------### */

    pt_locon = pt_lofig->LOCON;
    while (pt_locon != NULL)
      {
      mode = beh_chktab (lcl_dic, pt_locon->SIG, NULL, VHL_SIGDFN);
      if (mode != 0)
        {
        connec_nbr = beh_chktab (lcl_dic, pt_locon->SIG, NULL, VHL_CNBDFN) + 1;
        receiv_nbr = beh_chktab (lcl_dic, pt_locon->SIG, NULL, VHL_RNBDFN);
        emettr_nbr = beh_chktab (lcl_dic, pt_locon->SIG, NULL, VHL_WNBDFN);
        mode      &= VHL_REDDFN | VHL_WRTDFN | VHL_DECDFN;

        switch (mode)
          {
          case (VHL_REDDFN):
            if ((pt_locon->DIRECTION != 'X') && (pt_locon->DIRECTION != 'I'))
              errflg = vhu_error (111, pt_locon->NAME, NULL);
            pt_locon->DIRECTION = 'I';
            emettr_nbr++;
            break;
          case (VHL_WRTDFN):
            if ((pt_locon->DIRECTION != 'X') && (pt_locon->DIRECTION != 'O'))
              errflg = vhu_error (111, pt_locon->NAME, NULL);
            pt_locon->DIRECTION = 'O';
            receiv_nbr++;
            break;
          case (VHL_DECDFN):
            if ((pt_locon->DIRECTION != 'X') && (pt_locon->DIRECTION != 'Z'))
              errflg = vhu_error (111, pt_locon->NAME, NULL);
            pt_locon->DIRECTION = 'Z';
            receiv_nbr++;
            break;
          case (VHL_REDDFN | VHL_WRTDFN):
            if ((pt_locon->DIRECTION != 'X') && (pt_locon->DIRECTION != 'B'))
              errflg = vhu_error (111, pt_locon->NAME, NULL);
            pt_locon->DIRECTION = 'B';
            receiv_nbr++;
            break;
          case (VHL_REDDFN | VHL_DECDFN):
            if ((pt_locon->DIRECTION != 'X') && (pt_locon->DIRECTION != 'T'))
              errflg = vhu_error (111, pt_locon->NAME, NULL);
            pt_locon->DIRECTION = 'T';
            emettr_nbr++;
            receiv_nbr++;
            break;

          case 0:
            switch (pt_locon->DIRECTION)
              {
              case 'I':
              case 'T':
                emettr_nbr++;
                break;
              case 'O':
              case 'B':
              case 'Z':
                receiv_nbr++;
                break;
              case 'X':
                vhu_warning (4, pt_locon->NAME, NULL);
                break;
              }
            break;

          }
        beh_addtab (lcl_dic, pt_locon->SIG, NULL, VHL_RNBDFN, receiv_nbr);
        beh_addtab (lcl_dic, pt_locon->SIG, NULL, VHL_WNBDFN, emettr_nbr);
        beh_addtab (lcl_dic, pt_locon->SIG, NULL, VHL_CNBDFN, connec_nbr);
        }
      pt_locon = pt_locon->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    For each LOSIG added into the dictionary check the	*/
	/* coherence of connections :					*/
	/*    - # of receivers      = 0   -> warning			*/
	/*    - # of emetters       = 0   -> error			*/
	/*    - # of connectors     = 0   -> error			*/
	/*    - # of steady drivers > 1   -> error			*/
	/*    - drivers of different type -> error			*/
	/*    - badly declared signal     -> error			*/
	/* ###------------------------------------------------------### */

    pt_losig = pt_lofig->LOSIG;
    while (pt_losig != NULL)
      {
      if ((mode=beh_chktab(lcl_dic, pt_losig, NULL, VHL_SIGDFN)) != 0)
        {
        receiv_nbr = beh_chktab (lcl_dic, pt_losig, NULL, VHL_RNBDFN);
        emettr_nbr = beh_chktab (lcl_dic, pt_losig, NULL, VHL_WNBDFN);
        connec_nbr = beh_chktab (lcl_dic, pt_losig, NULL, VHL_CNBDFN);

        if (receiv_nbr == 0)
          vhu_warning (3, getsigname (pt_losig), NULL);

        if (emettr_nbr == 0 && receiv_nbr != 0)
        {
          errflg = vhu_error (109, getsigname (pt_losig), NULL);
        }

        if ((emettr_nbr > 1) && ((mode & VHL_WRTDFN) == VHL_WRTDFN))
          errflg = vhu_error (114, getsigname (pt_losig), NULL);

        if (((mode & VHL_WRTDFN) == VHL_WRTDFN) && (pt_losig->USER != NULL)
                                     && (pt_losig->USER->DATA == NULL))
          errflg = vhu_error (111, getsigname (pt_losig), NULL);

        if (connec_nbr == 0 && receiv_nbr != 0)
          errflg = vhu_error (110, getsigname (pt_losig), NULL);

        if (((mode & VHL_WRTDFN) == VHL_WRTDFN) &&
            ((mode & VHL_DECDFN) == VHL_DECDFN))
          errflg = vhu_error (111, getsigname (pt_losig), NULL);
        }
      pt_losig = pt_losig->NEXT;
      }

    beh_fretab (lcl_dic);
    }

  return (errflg);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_resord						*/
/* description	: put the LOCONs of each instance  in the same order as	*/
/*		  the BEPOR.						*/
/* called func.	: mbkfree   , beh_initab , beh_addtab, beh_chktab,	*/
/*		  beh_fretab, vhu_toolbug, vhu_alloc			*/
/* ###--------------------------------------------------------------### */

void vhl_resord (dict, pt_lofig, head_befig)

struct beden **dict      ;
struct lofig  *pt_lofig  ;
struct befig  *head_befig;

  {
  struct loins  *pt_loins;
  struct locon  *pt_locon;
  struct locon  *nextcon ;
  struct befig  *pt_befig;
  struct bepor  *pt_bepor;
  char          *fig_name;
  struct beden **lcl_dic ;

  if (pt_lofig != NULL)
    {
    lcl_dic = beh_initab ();

	/* ###------------------------------------------------------### */
	/*    Find the root model of the description by scanning the	*/
	/* list of LOFIGs til the end. (The last LOFIG must be the root)*/
	/* ###------------------------------------------------------### */

    while (pt_lofig->NEXT != NULL)
      pt_lofig = pt_lofig->NEXT;

	/* ###------------------------------------------------------### */
	/*    For each LOINS ... :					*/
	/* ###------------------------------------------------------### */

    pt_loins = pt_lofig->LOINS;
    while (pt_loins != NULL)
      {
      fig_name = pt_loins->FIGNAME;

	/* ###------------------------------------------------------### */
	/*    Fill the local dictionary with the internal LOCONs' SIG	*/
	/* and DIRECTION fields. Then, delete the LOCON's list		*/
	/* ###------------------------------------------------------### */

      pt_locon = pt_loins->LOCON;
      while (pt_locon != NULL)
        {
        beh_addtab (lcl_dic, pt_locon->NAME, fig_name, VHL_SIGDFN, 
                    pt_locon->DIRECTION);
        beh_addtab (lcl_dic, pt_locon->NAME, fig_name, VHL_PNTDFN, 
                    pt_locon->SIG);

        nextcon = pt_locon->NEXT;
        mbkfree (pt_locon);
        pt_locon = nextcon;
        }
      pt_loins->LOCON = NULL;

	/* ###------------------------------------------------------### */
	/*    Scan the list of BEFIGs to find the model of the instance	*/
	/* ###------------------------------------------------------### */

      if ((pt_befig = (struct befig *)
                      beh_chktab (dict, fig_name, 1, VHL_PNTDFN)) == NULL)
        {
        pt_befig = head_befig;
        while ((pt_befig != NULL) && (pt_befig->NAME != fig_name))
          pt_befig = pt_befig->NEXT;

        if (pt_befig == NULL)
          vhu_toolbug (13, "vhl_restor", fig_name, 0);
        else
          beh_addtab (dict, fig_name, 1, VHL_PNTDFN, pt_befig);
        }

	/* ###------------------------------------------------------### */
	/*    Make a copy of list of BEPORs (create a list of LOCONs)	*/
	/* ###------------------------------------------------------### */

      pt_bepor = pt_befig->BEPOR;
      while (pt_bepor != NULL)
        {
        pt_locon = (struct locon *)
                   vhu_alloc (sizeof(struct locon), VHU_ALIGN);

        pt_locon->NAME      = pt_bepor->NAME ;
        pt_locon->ROOT      = pt_loins       ;
        pt_locon->TYPE      = 'I'            ;
        pt_locon->USER      = NULL           ;
        pt_locon->NEXT      = pt_loins->LOCON;
        pt_locon->DIRECTION = beh_chktab (lcl_dic, pt_locon->NAME, fig_name,
                                         VHL_SIGDFN);
        pt_locon->SIG       = (struct losig *)
                              beh_chktab (lcl_dic, pt_locon->NAME, fig_name,
                                          VHL_PNTDFN);

        pt_loins->LOCON = pt_locon;
        pt_bepor = pt_bepor->NEXT;
        }
      pt_loins->LOCON = (struct locon *)
                        reverse ((struct chain *) pt_loins->LOCON);

      pt_loins = pt_loins->NEXT;
      }

    beh_fretab (lcl_dic);
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_getref						*/
/* description	: get signal refernces for a distributed simulation	*/
/* called func.	: beh_chktab,						*/
/* ###--------------------------------------------------------------### */

unsigned int vhl_getref (dict, name , pt_sig, pt_type)

struct beden **dict   ;
char          *name   ;
void         **pt_sig ;
unsigned int  *pt_type;

  {
  unsigned int ret_valu = 0;

  (*pt_sig ) = (void         *) beh_chktab (dict, name, NULL, VHL_PNTDFN);
  (*pt_type) = (unsigned int  ) beh_chktab (dict, name, NULL, VHL_SIGDFN);

  if (((*pt_type) != 'S') && ((*pt_type) != 'B'))
    ret_valu = vhu_error (101, name, NULL);

  return (ret_valu);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhl_addlkdspy						*/
/* description	: add an LKDSPY structure in a list			*/
/* called funct	: vhu_alloc						*/
/* ###--------------------------------------------------------------### */

struct lkdspy *vhl_addlkdspy (pt_lkdspy, pt_paiol, iolnbr)

struct lkdspy *pt_lkdspy;
struct paiol  *pt_paiol ;
unsigned int   iolnbr;

  {
  struct lkdspy *new_lkdspy;

  new_lkdspy         = (struct lkdspy *)
                      vhu_alloc (sizeof(struct lkdspy), VHU_ALIGN);

  new_lkdspy->PAIOL  = pt_paiol     ;
  new_lkdspy->IOLNBR = iolnbr       ;
  new_lkdspy->NEXT   = pt_lkdspy    ;

  return (new_lkdspy);
  }
