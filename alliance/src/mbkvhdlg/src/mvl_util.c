/* ###--------------------------------------------------------------### */
/*									*/
/* file		: mvl_util.c						*/
/* date		: Jan  06 1993						*/
/* author	: P. BAZARGAN-SABET					*/
/* update	: VUONG H.N.  						*/
/*									*/
/* description	: This file contains some utility functions :		*/
/*		  mvl_addtab , mvl_chktab , mvl_fretab , mvl_error  ,	*/
/*		  mvl_addent , mvl_addrcd , yy_b_error , mvl_y_error ,	*/
/*		  yy_v_error , yy_b_wrap  , mvl_y_wrap  , yy_v_wrap  ,	*/
/*		  mvl_toolbug, mvl_message, mvl_reverse, mvl_warning,	*/
/*		  mvl_initab , mvl_deltab, 				*/
/*									*/
/* ###--------------------------------------------------------------### */
#ident "$Id: mvl_util.c,v 1.1 2002/04/26 09:51:07 ludo Exp $"
#include <stdio.h>
#include <string.h>
#include <mut.h>
#include <mlo.h>
#include "mvl_utype.h"
#include "mvl_util.h"
#include "mvl_utdef.h"

/* ###--------------------------------------------------------------### */
/*  function : mvl_deltab						*/
/* ###--------------------------------------------------------------### */

void mvl_deltab (head,key_str,ctx_str)

struct dct_entry **head;
char              *key_str;
char              *ctx_str;

  {
  int               found = 0;
  int               index;
  struct dct_entry *entry_pnt;
  struct dct_entry *last_entry = NULL;
  struct dct_recrd *recrd_pnt;
  struct dct_recrd *last_recrd = NULL;

  index     = (int)key_str % MVL_HSZDFN;
  entry_pnt = head [index];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->key == key_str)
      {
      found = 1;
      break;
      }
    last_entry = entry_pnt;
    entry_pnt  = entry_pnt->next;
    }

  if (found == 1)
    {
    found = 0;
    recrd_pnt = entry_pnt->data;
    while (recrd_pnt != NULL)
      {
      if (recrd_pnt->key == ctx_str)
        {
        found = 1;
        break;
        }
      last_recrd = recrd_pnt;
      recrd_pnt  = recrd_pnt->next;
      }

    if (found == 1)
      {
      if (last_recrd == NULL)
        entry_pnt->data  = recrd_pnt->next;
      else
        last_recrd->next = recrd_pnt->next;

      recrd_pnt->next = MVL_DCRHED;
      MVL_DCRHED      = recrd_pnt;

      if (entry_pnt->data == NULL)
        {
        if (last_entry == NULL)
          head[index]      = entry_pnt->next;
        else
          last_entry->next = entry_pnt->next;

        entry_pnt->next = MVL_DCEHED;
        MVL_DCEHED      = entry_pnt;
        }
      }
    }
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_initab						*/
/* ###--------------------------------------------------------------### */
struct dct_entry **mvl_initab ()

  {
  struct dct_entry **head;
  int                i;

  head = (struct dct_entry **)
         mbkalloc (sizeof(struct dct_entry *) * MVL_HSZDFN);

  for (i=0 ; i<MVL_HSZDFN ; i++)
    head[i] = NULL;

  return (head);
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_warning						*/
/*  content  : print out warning messages on the standard error output  */
/* ###--------------------------------------------------------------### */
void mvl_warning (code,str1)
int   code;
char *str1;
  {
  static char first_time = 0;

  switch(code)
    {
    case 2:
      if (first_time != 1)
        {
        (void)fprintf (stderr,"Warning %d : ",code);
        (void)fprintf(stderr,"consistency checks will be disabled\n");
        first_time = 1;
        }
      break;

    case 42:
      (void) fprintf (stderr,"Warning : connection missing on port `%s`\n",
                      str1);
      break;

    default:
      {
      (void)fprintf(stderr,"Warning %d : ",code);
      (void)fprintf(stderr,"unknown Warning code\n");
      }
    }
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_toolbug						*/
/*  content  : print out bugs messages on the standard error output     */
/* ###--------------------------------------------------------------### */
void mvl_toolbug (code,str1,str2,nbr1)

int   code;
char *str1;
char *str2;
int   nbr1;

  {
  (void) fprintf (stderr,"Fatal error %d executing `%s`: ", code,str1);
  switch (code)
    {
    case 10:
      (void) fprintf (stderr,"decompiler called on empty lofig\n");
      break;
    }
  exit (1);
  }


/* ###--------------------------------------------------------------### */
/*  function : mvl_message						*/
/*  content  : print out messages on the standard error output     	*/
/* ###--------------------------------------------------------------### */
void mvl_message (code,str1,nmb1)

int   code;
char *str1;
int   nmb1;

  {
  switch (code)
    {
    default:
      (void) fprintf (stderr,"mvl_message : code %d unknown.\n",code);
    }
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_addtab						*/
/* ###--------------------------------------------------------------### */
void mvl_addtab (head,key_str,ctx_str,field,valu)

struct dct_entry **head;
char              *key_str;
char              *ctx_str;
int                field;
int                valu;

  {
  int               found = 0;
  int               index;
  struct dct_entry *entry_pnt;
  struct dct_recrd *recrd_pnt;

  index     = (int) key_str % MVL_HSZDFN;
  entry_pnt = head[index];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->key == key_str)
      {
      found = 1;
      break;
      }
    entry_pnt = entry_pnt->next;
    }

  if (found == 0)
    {
    head[index] = mvl_addent (head[index],key_str); 
    entry_pnt = head[index]; 
    }

  found = 0;
  recrd_pnt = entry_pnt->data;
  while (recrd_pnt != NULL)
    {
    if (recrd_pnt->key == ctx_str)
      {
      found = 1;
      break;
      }
    recrd_pnt = recrd_pnt->next;
    }

  if (found == 0)
    {
    entry_pnt->data = mvl_addrcd (entry_pnt->data,ctx_str); 
    recrd_pnt       = entry_pnt->data ;
    }

  switch (field)
    {
    case 0 :
      recrd_pnt->fd0_val = valu;
      break;
    case 1 :
      recrd_pnt->fd1_val = valu;
      break;
    case 2 :
      recrd_pnt->fd2_val = valu;
      break;
    case 3 :
      recrd_pnt->fd3_val = valu;
      break;
    case 4 :
      recrd_pnt->fd4_val = valu;
      break;
    case 5 :
      recrd_pnt->fd5_val = valu;
      break;
    case 6 :
      recrd_pnt->fd6_val = valu;
      break;
    case 7 :
      recrd_pnt->pnt_val = valu;
      break;
    }

  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_chktab						*/
/* ###--------------------------------------------------------------### */
int  mvl_chktab (head,key_str,ctx_str,field)

struct dct_entry **head;
char              *key_str;
char              *ctx_str;
int                field;

  {
  int               found = 0;
  int               valu = 0;
  struct dct_entry *entry_pnt;
  struct dct_recrd *recrd_pnt;

  entry_pnt = head [(int)key_str % MVL_HSZDFN];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->key == key_str)
      {
      found = 1;
      break;
      }
    entry_pnt = entry_pnt->next;
    }

  if (found == 1)
    {
    found = 0;
    recrd_pnt = entry_pnt->data;
    while (recrd_pnt != NULL)
      {
      if (recrd_pnt->key == ctx_str)
        {
        found = 1;
        break;
        }
      recrd_pnt = recrd_pnt->next;
      }
    if (found == 1)
      {
      switch (field)
        {
        case 0 :
          valu = recrd_pnt->fd0_val;
          break;
        case 1 :
          valu = recrd_pnt->fd1_val;
          break;
        case 2 :
          valu = recrd_pnt->fd2_val;
          break;
        case 3 :
          valu = recrd_pnt->fd3_val;
          break;
        case 4 :
          valu = recrd_pnt->fd4_val;
          break;
        case 5 :
          valu = recrd_pnt->fd5_val;
          break;
        case 6 :
          valu = recrd_pnt->fd6_val;
          break;
        case 7 :
          valu = recrd_pnt->pnt_val;
          break;
        }
      }
    }

  return (valu);
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_fretab						*/
/* ###--------------------------------------------------------------### */
void mvl_fretab (pt_hash)

struct dct_entry **pt_hash;
  {
  struct dct_entry *pt_entry;
  struct dct_entry *pt_nxtentry;
  struct dct_recrd *pt_record;
  int               i;

  if (pt_hash != NULL)
    {
    for (i=0 ; i<MVL_HSZDFN ; i++)
      {
      if ((pt_entry = pt_hash[i]) != NULL)
        {
        while (pt_entry != NULL)
          {
          pt_record = pt_entry->data;

          while (pt_record->next != NULL)
            pt_record = pt_record->next;

          pt_record->next = MVL_DCRHED;
          MVL_DCRHED      = pt_entry->data;

          pt_nxtentry     = pt_entry->next;
          pt_entry->next  = MVL_DCEHED;
          MVL_DCEHED      = pt_entry;
          pt_entry        = pt_nxtentry;
          }
        }
      }
    free (pt_hash);
    }
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_error						*/
/* ###--------------------------------------------------------------### */
void mvl_error (code,str1)

int   code;
char *str1;

  {
  MVL_ERRFLG++;
  if (code < 100)
    (void)fprintf (stderr,"`%s` Error %d line %d :",MVL_CURFIL,code,MVL_LINNUM);
  else
    {
    if (code < 200)
      (void)fprintf (stderr,"Error %d :",code);
    }

  switch (code)
    {
    case 1:
      (void) fprintf (stderr,"`%s` is incompatible with the entity name\n",str1);
      break;
    case 2:
      (void) fprintf (stderr,"bad entity declaration\n");
      break;
    case 3:
      (void) fprintf (stderr,"bad port clause declaration\n");
      break;
    case 4:
      (void) fprintf (stderr,"port `%s` already declared\n",str1);
      break;
    case 5:
      (void) fprintf (stderr,"illegal port declaration `%s` (mode, type, guard mark)\n",str1);
      break;
    case 6:
      (void) fprintf (stderr,"bad port declaration\n");
      break;
    case 7:
      (void) fprintf (stderr,"`%s` is incompatible with the architecture name\n",str1);
      break;
    case 8:
      (void) fprintf (stderr,"bad architecture declaration\n");
      break;
    case 9:
      (void) fprintf (stderr,"illegal declaration\n");
      break;
    case 10:
      (void) fprintf (stderr,"signal `%s` already declared\n",str1);
      break;
    case 11:
      (void) fprintf (stderr,"illegal signal declaration `%s` (type, guard mark)\n",str1);
      break;
    case 12:
      (void) fprintf (stderr,"component `%s` already declared\n",str1);
      break;
    case 13:
      (void) fprintf (stderr,"instance `%s` already declared\n",str1);
      break;
    case 14:
      (void) fprintf (stderr,"`%s` unknown component\n",str1);
      break;
    case 15:
      (void) fprintf (stderr,"illegal usage of implicit port map description\n");
      break;
    case 16:
      (void) fprintf (stderr,"`%s` unknown local port\n",str1);
      break;
    case 17:
      (void) fprintf (stderr,"`%s` unknown port or signal\n",str1);
      break;
    case 18:
      (void) fprintf (stderr,"illegal concurrent statement\n");
      break;
    case 31:
      (void) fprintf (stderr,"bad signal association\n");
      break;
    case 32:
      (void) fprintf (stderr,"null array not supported\n");
      break;
    case 33:
      (void) fprintf (stderr,"illegal constraint in declaration of type\n");
      break;
    case 36:
      (void) fprintf (stderr,"signal `%s` used out of declared range\n",str1);
      break;
    case 38:
      (void) fprintf (stderr,"width or/and type mismatch\n");
      break;
    case 41:
      (void) fprintf (stderr,"port `%s` connected to more than one signal\n",str1);
      break;
    case 76:
      (void) fprintf (stderr,"instance %s mismatch with the model\n",str1);
      break;
    case 107:
      (void) fprintf (stderr,"Cannot open result file\n");
      break;
    case 200:
      (void) fprintf (stderr,"\n	cannot continue further more.\n");
      (void) fprintf (stderr,"\n		Have a nice day...\n");
      break;

    default:
      (void) fprintf (stderr,"syntax error\n");
      break;
    }

  if (MVL_ERRFLG > MVL_MXRDFN)
    {
    (void) fprintf (stderr,"Too many errors. Cannot continue further more\n");
    (void) fprintf (stderr,"\n		Have a nice day...\n");
    exit (1);
    }

  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_addent						*/
/* ###--------------------------------------------------------------### */
static struct dct_entry *mvl_addent (head , key)

struct dct_entry *head;
char             *key;

  {
  struct dct_entry *entry;
  int               i;

  if (MVL_DCEHED == NULL)
    {
    MVL_DCEHED = (struct dct_entry *)
                 mbkalloc (sizeof(struct dct_entry) * MVL_ALODFN);

    entry = MVL_DCEHED;
    for (i=1 ; i<MVL_ALODFN ; i++)
      {
      entry->next = entry + 1;
      entry++;
      }
    entry->next = NULL;
    }

  entry       = MVL_DCEHED;
  MVL_DCEHED  = MVL_DCEHED->next;

  entry->next = head;
  entry->data = NULL;
  entry->key  = key;

  return (entry);
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_addrcd						*/
/* ###--------------------------------------------------------------### */
static struct dct_recrd *mvl_addrcd (head , key)

struct dct_recrd *head;
char             *key;

  {
  struct dct_recrd *recrd;
  int               i;

  if (MVL_DCRHED == NULL)
    {
    MVL_DCRHED = (struct dct_recrd *)
                 mbkalloc (sizeof(struct dct_recrd) * MVL_ALODFN);

    recrd = MVL_DCRHED;
    for (i=1 ; i<MVL_ALODFN ; i++)
      {
      recrd->next = recrd + 1;
      recrd++;
      }
    recrd->next = NULL;
    }

  recrd           = MVL_DCRHED;
  MVL_DCRHED      = MVL_DCRHED->next;

  recrd->next     = head;
  recrd->fd0_val  = 0;
  recrd->fd1_val  = 0;
  recrd->fd2_val  = 0;
  recrd->fd3_val  = 0;
  recrd->fd4_val  = 0;
  recrd->fd5_val  = 0;
  recrd->fd6_val  = 0;
  recrd->pnt_val  = 0;
  recrd->key      = key;

  return (recrd);
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl__y_error						*/
/* ###--------------------------------------------------------------### */
void mvl_y_error (str)

char *str;
  {
  MVL_ERRFLG++;
  (void)fprintf (stderr,"`%s` Error line %d : %s\n",MVL_CURFIL,MVL_LINNUM,str);
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_y_wrap						*/
/* ###--------------------------------------------------------------### */
int mvl_y_wrap ()
  {
  return (1);
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_avers						*/
/* ###--------------------------------------------------------------### */
char *mvl_avers ()
  {
  return ("-- V 1.3 --");
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_vhdlname						*/
/* ###--------------------------------------------------------------### */
char *mvl_vhdlname (name)

char *name;

  {
  char                     *new_name;
  char                     *prv_name;
  char                     *tmp_name;
  char                      buffer[200];
  int                       i,j,flag,number;
  static struct dct_entry **namtab=NULL;

  if (namtab == NULL)
    namtab = mvl_initab ();

  tmp_name = namealloc (name);
  new_name = (char *) mvl_chktab (namtab,tmp_name,NULL,MVL_PNTDFN);

  if (mvl_chktab (namtab,tmp_name,NULL,MVL_NAMDFN) == 0)
    {
    i = 0;
    j = 0;
    number = 0;
    flag = 1;
    while (tmp_name[i] != '\0')
      {
      buffer[j] = tmp_name[i];
      if ( ((tmp_name[i] >= 'a') && (tmp_name[i] <= 'z')) ||
           ((tmp_name[i] >= 'A') && (tmp_name[i] <= 'Z')) ||
           ((tmp_name[i] >= '0') && (tmp_name[i] <= '9') && (i != 0)) ||
           ((tmp_name[i] == '(') || (tmp_name[i] == ')')) )
        flag = 0;
      else
        {
        if (flag == 1)
          buffer[j++] = 'v';
        buffer[j] = '_';
        flag = 1;
        }
      i++;
      j++;
      }
    if (buffer[j-1] == '_')
      buffer[j++] = '0';
    buffer[j] = '\0';
    new_name = namealloc (buffer);

    prv_name = new_name;
    while (mvl_chktab (namtab,new_name,NULL,MVL_NEWDFN) != 0)
      {
      new_name = prv_name;
      sprintf (buffer,"%s_%d",new_name,number++);
      prv_name = new_name;
      new_name = namealloc (buffer);
      }
    mvl_addtab (namtab,new_name,NULL,MVL_NEWDFN,1);
    mvl_addtab (namtab,tmp_name,NULL,MVL_PNTDFN,(int)new_name);
    mvl_addtab (namtab,tmp_name,NULL,MVL_NAMDFN,1);
    }

  return (new_name);
  }


/* ###--------------------------------------------------------------### */
/*  function : mvl_name							*/
/* ###--------------------------------------------------------------### */
void mvl_name (name,new_name)

char *name;
char *new_name;

  {
  char *blank_space;

  /* Transformation des blancs en parentheses */
  strcpy(new_name,name);
  blank_space = strchr(new_name,' ');
  if(blank_space != NULL)
    {
    *blank_space = '(';
    blank_space = strchr(new_name,'\0');
    /* Transformation du dernier caractere en ) */
    if(blank_space != NULL)
      {
      *blank_space = ')';
      blank_space++;
      *blank_space = '\0';
      }
    }
  strcpy(new_name,mvl_vhdlname(new_name));
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_vectnam						*/
/* ###--------------------------------------------------------------### */
void *mvl_vectnam(pt_list,left,right,name,type)
  
void *pt_list;
int *left, *right;
char **name;
char type;

  {
  char *blank_space;
  char *sig_name;
  char name_tmp[200];
  char number[200];
  losig_list *ptsig;
  locon_list *ptcon;
  char END = 0;

  /* Case losig_list */
  if(type==0)
    {
    ptsig = (losig_list *)pt_list; 
    if (ptsig->TYPE == 'I')
      {
      *left = *right = -1;
      sig_name = getsigname(ptsig);
      *name = (char*)mbkalloc(strlen(sig_name) + 1);
      strcpy(*name,sig_name);
      blank_space = strchr(*name,' ');
      if (blank_space != NULL)
        {
        strcpy(number,blank_space);
        *right = atoi(number);
        *left = *right;
        *blank_space = '\0';
        }

      while(!END)
        {
        if(ptsig->NEXT != NULL)
          {
          strcpy(name_tmp,getsigname(ptsig->NEXT));
          blank_space = strchr(name_tmp,' ');
          if(blank_space!=NULL)
            {
	    strcpy(number,blank_space);
            *blank_space = '\0';
            if(!strcmp(*name,name_tmp))
              {
              *left = atoi(number);
              ptsig = ptsig->NEXT;
              }
            else
              END = 1;
            }
          else
            END = 1;
          }
        else
          END = 1;
        }
      return(ptsig);
      }
    else
      {
      *name = NULL;
      return(ptsig);
      }
    }

  /*case locon_list */
  if(type==1)
    {
    ptcon = (locon_list *)pt_list; 
    /* Extract the name and number of an element */
    *left = *right = -1;
    sig_name = ptcon->NAME;
    *name = (char *)mbkalloc(strlen(sig_name) + 1);
    strcpy(*name,sig_name);
    blank_space = strchr(*name,' ');
    if (blank_space != NULL)
      {
      strcpy(number,blank_space);
      *right = atoi(number);
      *left = *right;
      *blank_space = '\0';
      }

    while(END != 1)
      {
      if(ptcon->NEXT != NULL)
        {
        strcpy(name_tmp,ptcon->NEXT->NAME);
        blank_space = strchr(name_tmp,' ');
        if(blank_space!=NULL)
          {
          strcpy(number,blank_space);
          *blank_space = '\0';
          if(!strcmp(*name,name_tmp))
            {
            *right = atoi(number);
            ptcon = ptcon->NEXT;
            }
          else
            END = 1;
          }
        else
          END = 1;
        }
      else 
        END = 1;
      }
    return(ptcon);
    }
  /* To avoid Warning from GCC */
  return(NULL);
  }


/* ###--------------------------------------------------------------### */
/*  function : mvl_reverse						*/
/* ###--------------------------------------------------------------### */
struct chain *mvl_reverse (head)

struct chain *head;

  {
  struct chain *last_pnt = NULL;
  struct chain *curr_pnt = NULL;
  struct chain *next_pnt = NULL;

  if (head != NULL)
    {
    last_pnt       = head;
    curr_pnt       = head->NEXT;
    last_pnt->NEXT = NULL;

    if (curr_pnt != NULL)
      {
      next_pnt       = curr_pnt->NEXT;

      while (next_pnt != NULL)
        {
        curr_pnt->NEXT = last_pnt;

	/* ###------------------------------------------------------### */
	/*    Now shift the window to the next structure		*/
	/* ###------------------------------------------------------### */

        last_pnt = curr_pnt;
        curr_pnt = next_pnt;
        next_pnt = next_pnt->NEXT;
        }

      curr_pnt->NEXT = last_pnt;
      }
    else
      curr_pnt = head;
    }

  return (curr_pnt);
  }

/* ###--------------------------------------------------------------### */
/*  function : mvl_fill 						*/
/*  content  : Fill a lofig of mode 'P' with another lofig of mode 'A'  */
/* ###--------------------------------------------------------------### */
struct lofig *mvl_fill  (lofig_P, lofig_A)

struct lofig *lofig_P;
struct lofig *lofig_A;
 
  {
  struct locon *ptlocon_P, *ptlocon_A;
  struct chain *ptchain;
  struct lofig *ptlofig;
  struct losig *ptlosig;

  /* MODELCHAIN */
  ptchain = lofig_P->MODELCHAIN;
  lofig_P->MODELCHAIN = lofig_A->MODELCHAIN;

  /* LOCON */
  ptlocon_P = lofig_P->LOCON;
  ptlocon_A = lofig_A->LOCON;

  while(ptlocon_A != NULL)
    {
    if(ptlocon_A->NAME == ptlocon_P->NAME)
      {
      ptlocon_P->SIG = ptlocon_A->SIG;
      }
    else
      {
      (void)fprintf(stderr,"\n*** mbk error *** bad consistency in figure %s,\n external interface are different\n", lofig_P->NAME);
      }
    ptlocon_A = ptlocon_A->NEXT;
    ptlocon_P = ptlocon_P->NEXT;
    }

  /* LOSIG */
  ptlosig        = lofig_P->LOSIG;
  lofig_P->LOSIG = lofig_A->LOSIG;

  /* LOINS */
  lofig_P->LOINS = lofig_A->LOINS;

  /* LOTRS */
  lofig_P->LOTRS = lofig_A->LOTRS;

  /* USER  */
  lofig_P->USER  = lofig_A->USER;

  /* MODE  */
  lofig_P->MODE  = 'A';

  /* Freeing the memory zone unusable */

  freechain(ptchain);

  while (lofig_A->LOCON != NULL)
    {
    (void)dellocon(lofig_A, lofig_A->LOCON->NAME); 
    }

  ptlofig = addlofig(" bidon");
  ptlofig->LOSIG = ptlosig;
  (void)dellofig(ptlofig->NAME);

  

  return(lofig_P);
  }
