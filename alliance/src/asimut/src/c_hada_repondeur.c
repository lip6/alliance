
/* ###--------------------------------------------------------------### */
/* file		: c_hada_repondeur.c					*/
/* date		: Nov 18 1997						*/
/* author	: Sebatien Lozac'h					*/
/* content	: C model of hadamard's test bench			*/
/* warning	: This model can only be instancied once		*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <stdlib.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

#include "vh_ltype.h"
#include "vh_xspec.h"
#include "vh_xcomm.h"

#define MAX 64
#define ENTREE "fic_entree"
#define ENTREE2 "fic_entree2"
#define SORTIE "fic_sortie"


void c_hada_repondeur (pt_lkdins)
struct lkdins *pt_lkdins;

  {
  
  #define RMAX 1000
  #define PROB 900

  static int deja_ouvert=0;
  static int nb_entrees=0;
  static int nb_sorties=0;
  static int ck_old=0;
  static int empty_old=0;
  static int full_old=0;

  static FILE *fouvrir;
  static FILE *fecrire;

/*-------  Declaration des entrees  -----------*/

  char         ck    ;
  char         reset ;
  char         read  ;
  char         write ;
  int          hadout;                /* Entree sur 14 bits */

  char         empty ;
  char         full  ;
  unsigned int data  ;                /* Sortie sur 8 bits */

  #define POSI_vss                           0
  #define POSI_vdd        POSI_vss        +  1
  #define POSI_hadout     POSI_vdd        +  1
  #define POSI_write      POSI_hadout     + 14
  #define POSI_read       POSI_write      +  1
  #define POSI_reset      POSI_read       +  1
  #define POSI_ck         POSI_reset      +  1

/*-------  Declaration des sorties  -----------*/


  #define POSO_data              0
  #define POSO_full  POSO_data + 8
  #define POSO_empty POSO_full + 1
 
/*-------  Affectation des entrees  -----------*/

  ck     = vhx_readinput (pt_lkdins, POSI_ck    ,  1);
  read   = vhx_readinput (pt_lkdins, POSI_read  ,  1);
  reset  = vhx_readinput (pt_lkdins, POSI_reset ,  1);
  write  = vhx_readinput (pt_lkdins, POSI_write ,  1);
  hadout = vhx_readinput (pt_lkdins, POSI_hadout, 14);

/*-------  Ouverture des 2 Fichiers  -----------*/

  if (deja_ouvert == 0)
    {
    if ((fouvrir = fopen (ENTREE,"r")) == NULL)
      { fprintf(stderr,"CANNOT OPEN FILE : %s \n", ENTREE);exit (3);}
    if ( (fecrire=fopen(SORTIE,"w"))== NULL )
      { fprintf(stderr,"CANNOT OPEN FILE : %s \n", SORTIE);exit (3);}
    deja_ouvert = 1;
    }

/*-------  Description du comportement  -----------*/

        if ((ck_old==0) && (ck==1))
          {
          if (reset == 1)
            {
            nb_entrees=MAX; 
            nb_sorties=0;
            empty=1;
            full=0;
            }
          else if ((nb_entrees == MAX) && (nb_sorties == 0))
            {
            empty=0;
            full=0;
            if (fscanf(fouvrir,"%11d",&data) == EOF)
              {
              rewind(fouvrir);
              fscanf(fouvrir,"%11d",&data);
              }
            fprintf(stderr,"NEW INPUT FIFO VALUE : %u\n", data);
            vhx_writeout(data,pt_lkdins,POSO_data,8);
            nb_entrees--;
            }
          else 
            {
            if (( (random() % RMAX) > PROB) || ((nb_entrees==0) && (nb_sorties != 0)))
              { empty=1; }
            else
              { empty=0; }

            if (( (random() % RMAX) < PROB) || ((nb_sorties==0) && (nb_entrees != 0)))        
              { full=0; }
            else
              { full=1; }

            if ((empty_old == 0) && (read == 1) && (nb_entrees == 0))
                {
                nb_sorties=MAX;
                }

            if ((empty_old == 0) && (read == 1) && (nb_entrees != 0))
                {
                if (fscanf(fouvrir,"%11d",&data) == EOF)
                  {
                  rewind(fouvrir);
                  fscanf(fouvrir,"%11d",&data);
                  }
                fprintf(stderr,"NEW INPUT FIFO VALUE : %u\n", data);
                vhx_writeout(data,pt_lkdins,POSO_data,8);
                nb_entrees--;
                }

            if ((full_old == 0) && (write == 1) && (nb_sorties != 0))
                {
                fprintf(stderr,"NEW OUPUT FIFO VALUE : %d\n", hadout);
                fprintf(fecrire,"0x%x\t",hadout);
                nb_sorties--;
                if ( (nb_sorties%8) == 0 ) fprintf(fecrire,"\n");

                if ( (nb_sorties == 0))
                    {
                    nb_entrees=MAX;
                    fprintf(fecrire,"\n");
                    fflush(fecrire);
                    }
                }
            }
          vhx_writeout(empty,pt_lkdins,POSO_empty,1);
          vhx_writeout(full,pt_lkdins,POSO_full,1);
          full_old=full;
          empty_old=empty;
          }
        ck_old=ck;
        
}
