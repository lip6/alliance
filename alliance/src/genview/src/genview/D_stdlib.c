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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                  D_stdlib.c                       |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   04.30.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include MGN_H
# include "G_global.h"
# include "G_user.h"
# include "G_cache.h"
# include "d_interp_lib.h"
# include "d_fonc.h"
# include "d_asm.h"

# include "D_stdlib.h"


/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

extern int I_pc;        /* program counter */
extern int I_cc0;       /* code condition  */
extern char *I_sp;      /* stack        */
extern int I_reg[];     /* registers    */

FONC TabFonc[] = 
{
    "__flsbuf", app_flsbuf,
    "_atoi", app_atoi,
    "_bcopy", app_bcopy,
    "_genCOPY_UP_ALL_CON", app_COPY_UP_ALL_CON,
    "_genCOPY_UP_ALL_REF", app_COPY_UP_ALL_REF,
    "_genCOPY_UP_CON", app_COPY_UP_CON,
    "_genCOPY_UP_CON_FACE", app_COPY_UP_CON_FACE,
    "_genCOPY_UP_REF", app_COPY_UP_REF,
    "_genCOPY_UP_SEG", app_COPY_UP_SEG,
    "_genDEF_AB", app_DEF_AB,
    "_genDEF_PHFIG", app_DEF_PHFIG,
    "_genDEF_PHINS", app_DEF_PHINS,
    "_genFLATTEN_ALL_PHINS", app_FLATTEN_ALL_PHINS,
    "_genFLATTEN_PHFIG", app_FLATTEN_PHFIG,
    "_genGET_CON_X", app_GET_CON_X,
    "_genGET_CON_Y", app_GET_CON_Y,
    "_genGET_INS_X", app_GET_INS_X,
    "_genGET_INS_Y", app_GET_INS_Y,
    "_genGET_PHFIG_REF_X", app_GET_PHFIG_REF_X,
    "_genGET_PHFIG_REF_Y", app_GET_PHFIG_REF_Y,
    "_genGET_REF_X", app_GET_REF_X,
    "_genGET_REF_Y", app_GET_REF_Y,
    "_genHEIGHT", app_HEIGHT,
    "_genNAME", app_NAME,
    "_genPHCON", app_PHCON,
    "_genPHREF", app_PHREF,
    "_genPHSEG", app_PHSEG,
    "_genPHVIA", app_PHVIA,
    "_genPLACE", app_PLACE,
    "_genPLACE_BOTTOM", app_PLACE_BOTTOM,
    "_genPLACE_CON_REF", app_PLACE_CON_REF,
    "_genPLACE_LEFT", app_PLACE_LEFT,
    "_genPLACE_ON", app_PLACE_ON,
    "_genPLACE_REF", app_PLACE_REF,
    "_genPLACE_RIGHT", app_PLACE_RIGHT,
    "_genPLACE_SEG_REF", app_PLACE_SEG_REF,
    "_genPLACE_TOP", app_PLACE_TOP,
    "_genPLACE_VIA_REF", app_PLACE_VIA_REF,
    "_genSAVE_PHFIG", (void (*)(void)) genSAVE_PHFIG,
    "_genTHRU_CON_H", app_THRU_CON_H,
    "_genTHRU_CON_V", app_THRU_CON_V,
    "_genTHRU_H", app_THRU_H,
    "_genTHRU_V", app_THRU_V,
    "_genWIDTH", app_WIDTH,
    "_genWIRE1", app_WIRE1,
    "_genWIRE2", app_WIRE2,
    "_genWIRE3", app_WIRE3,
    "_malloc", app_malloc,
    "_namealloc", app_namealloc,
    "_printf", app_printf,
    "_sprintf", app_sprintf,
    "_strcmp", app_strcmp,
    "_strlen", app_strlen,
    "~", NULL
};

int taille_TabFonc = sizeof(TabFonc)/sizeof(TabFonc[0]) - 1;

/*  retrieve a parameter from the stack :
   Needs the position of the parameter in the stack - word aligned - and its
   type.
   The MULT_TYP contains either the value of the parameter or its absolute
   address in memory. */
MULT_TYP
  GetParam(pos_param, type_param)
int pos_param;
int type_param;
{
MULT_TYP res;

  switch (type_param) {
    case INT:
      res.ent = READ_INT(I_reg[31] + pos_param);
      return res;
    case REAL:
      res.real = READ_DOUBLE(I_reg[31] + pos_param);
      return res;
    case PTR:
      res.ptr = (char *)READ_INT(I_reg[31] + pos_param);
      return res;
    default:
      Erreur("Internal error -> GetParam : Uknown type\n");
  }
  /*NOTREACHED*/
}

void app_atoi(void)
{
MULT_TYP param;
char *str;

  param = GetParam(0, PTR);
  str = param.ptr;
  I_reg[25] = atoi(str);
}

void app_malloc(void)
{
MULT_TYP param;
int taille;
char *ptr;

  param = GetParam(0, INT);
  taille = param.ent;
  ptr = (char *)mbkalloc(taille);
  I_reg[25] =(int)ptr;
}

/*  gcc requirement :
   When copying structures into structures, the compiler generates calls to
   bcopy, so we must have one at hand. */
void app_bcopy(void)
{
char *src, *dest;
int taille;
MULT_TYP param;

  param = GetParam(0, PTR);
  src = param.ptr;
  param = GetParam(4, PTR);
  dest = param.ptr;
  param = GetParam(8, INT);
  taille = param.ent;
  bcopy(src, dest, taille);
}

/* standart io requirements :
   flsbuf is called under certain circumstances, I was told!
   When putc is used on sparc, I believe, but putc shouldn't be used anyway */

static char flsbuf(c, fp)
char c;
char *fp;
{
  return c;
}

void app_flsbuf(void)
{
int car;
char *fp;
MULT_TYP param;

  param = GetParam(0, INT);
  car = param.ent;
  param = GetParam(4, PTR);
  fp = param.ptr;
  I_reg[25] = flsbuf(car, fp);
}

/* user requirements :
   In order to handle strings, two functions are available.
   If more is needed, then add them. */
void app_strlen(void)
{
MULT_TYP param;
char *str;

  param = GetParam(0, PTR);
  str = param.ptr;
  I_reg[25] =(int)strlen(str);
}

void app_strcmp(void)
{
MULT_TYP param;
char *str1, *str2;

  param = GetParam(0, PTR);
  str1 = param.ptr;
  param = GetParam(4, PTR);
  str2 = param.ptr;
  I_reg[25] =(int)strcmp(str1, str2);
}

void app_namealloc(void)
{
MULT_TYP param;
char *name;
char *namealloc();

  param = GetParam(0, PTR);
  name = param.ptr;
  I_reg[25] =(int)namealloc(name);
}

/* printf emulation functions :
   the following functions fake the behavior of the printf and sprintf
   functions. */

/* Octal conversion :
   An octal constant, given in a string, is converted to it's decimal
   equivalent. */
void
  ConvOct(resu, str)
char *resu, *str;
{
int dep = 0;
int dep_res = 0;
int cste = 0;

  while (str[dep]) {
    if (str[dep] == '\\') {
      if (str[dep + 1] == '\\') {
        resu[dep_res] = '\\';
        dep_res++;
        dep += 2;
      } else if (str[dep + 1] >= '0' && str[dep + 1] <= '9') {
      /* octal constant */ 
        cste = 0;
        cste = 8 * ((8 * (str[dep + 1] - '0')) +
        (str[dep + 2] - '0')) + (str[dep + 3] - '0');
        resu[dep_res] = cste;
        dep_res++;
        dep += 4;
      } else {
        resu[dep_res] = str[dep + 1];
        dep_res++;
        dep += 2;
      }
    } else {
      resu[dep_res] = str[dep];
      dep_res++;
      dep++;
    }
  }
  resu[dep_res] = '\0';
}

/* partial printf :
   This does a sprintf with a single argument. */ 
void EffectuePrintf(resu, format, dep_in_pile)
char *resu, *format;
int *dep_in_pile;
{
MULT_TYP param;
int dep = 0;
int lg = strlen(format);
char res[80];

  while (format[dep] && format[dep] != '%')
    dep++;

  if (dep == lg)
    strcpy(resu, format);
  else {
    while (format[dep]) {
      switch (format[dep]) {
        case 'd':
        case 'u':
        case 'o':
        case 'c':
        case 'x':
        case 'D':
        case 'O':
        case 'X':
          param = GetParam(*dep_in_pile, INT);
          sprintf(resu, format, param.ent);
          dep = lg;
          *dep_in_pile += 4;
          continue;
        case 's':
          param = GetParam(*dep_in_pile, PTR);
          ConvOct(res, param.ptr);
          sprintf(resu, format, res);
          dep = lg;
          *dep_in_pile += 4;
          continue;
        case 'f':
        case 'e':
        case 'g':
        case 'E':
        case 'G':
          param = GetParam(*dep_in_pile, REAL);
          sprintf(resu, format, param.real);
          dep = lg;
          *dep_in_pile += 8;
          continue;
        default:
          break;
      }
      dep++;
    }
  }
  ConvOct(resu, resu);
}

/* basical interface to printf :
   The dep parameter is the start of the stack where to find the printf
   arguments.
   It scans the string for '%' and does the corresponding sprintfs. */

char *base_printf(dep)
int dep;
{
MULT_TYP param;
char *format, sous_format[81], resu[100], resu_tot[250];
char *resultat;
int deb = 0, fin = 0;
int dep_in_pile = 0;
int i, j;

  param = GetParam(dep, PTR);
  dep_in_pile += 4 + dep;
  format = param.ptr;
  resu_tot[0] = '\0';
  sous_format[0] = '\0';

  while (1) {
    if (!format[fin] || format[fin] == '%' || fin - deb == 80) {
      for (j = 0, i = deb; i < fin; i++, j++)
        sous_format[j] = format[i];
      sous_format[j] = '\0';

      EffectuePrintf(resu, sous_format, &dep_in_pile);
      deb = fin;
      strcat(resu_tot, resu);
      if (!format[fin])
        break;
    }
    fin++;
  }
  resultat = mbkalloc(strlen(resu_tot) + 1);
  strcpy(resultat, resu_tot);
  return resultat;
}

void app_printf(void)
{
char *resu;

  resu = base_printf(0);
  T_print(resu);
  mbkfree(resu);
}

void app_sprintf(void)
{
MULT_TYP param;
char *resu_base, *dest;

  param = GetParam(0, PTR);
  dest = param.ptr;

  resu_base = base_printf(4);

  strcpy(dest, resu_base);
  mbkfree(resu_base);
  I_reg[25] = (int)dest;
}

/* genlib calls :
   this are the encapsulated versions of the genlib's calls. */

void app_NAME(void)
{
  MULT_TYP param;
  char *resu;

  resu = base_printf(0);
  I_reg[25] = (int)resu;
}

void app_DEF_PHFIG(void)
{
MULT_TYP param;

  param = GetParam(0, PTR);
  genDEF_PHFIG(param.ptr);
  /* genDEF_PHFIG reinitializes the TERM signal handler.
      I must trap it my way, so I've got to call it again. */
  unix_trap();
  Init_fig();
}

void app_DEF_PHINS(void)
{
MULT_TYP param;

  param = GetParam(0, PTR);
  genDEF_PHINS(param.ptr);
  New_insref();
}

void app_DEF_AB(void)
{
MULT_TYP param;
long dx1, dy1, dx2, dy2;

  param = GetParam(0, INT);
  dx1 = param.ent;
  param = GetParam(4, INT);
  dy1 = param.ent;
  param = GetParam(8, INT);
  dx2 = param.ent;
  param = GetParam(12, INT);
  dy2 = param.ent;

  genDEF_AB(dx1, dy1, dx2, dy2);

  Update_AB();
}

void app_PLACE(void)
{
MULT_TYP param;
char *figname, *insname;
long x, y;
char sym;

  param = GetParam(0, PTR);
  figname = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, INT);
  sym = (char)param.ent;
  param = GetParam(12, INT);
  x = param.ent;
  param = GetParam(16, INT);
  y = param.ent;

  Fill_obj(genPLACE(figname, insname, sym, x, y));
  if (run_mode != RUN) V_drawInsRef();
  genDEF_AB(0, 0, 0, 0); Update_AB();
}

void app_PLACE_RIGHT(void)
{
MULT_TYP param;
char *figname, *insname;
char sym;

  param = GetParam(0, PTR);
  figname = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, INT);
  sym = (char)param.ent;

  Fill_obj(genPLACE_RIGHT(figname, insname, sym));
  if (run_mode != RUN) V_drawInsRef();
  genDEF_AB(0, 0, 0, 0); Update_AB();
}

void app_PLACE_LEFT(void)
{
MULT_TYP param;
char *figname, *insname;
char sym;

  param = GetParam(0, PTR);
  figname = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, INT);
  sym = (char)param.ent;

  Fill_obj(genPLACE_LEFT(figname, insname, sym));
  if (run_mode != RUN) V_drawInsRef();
  genDEF_AB(0, 0, 0, 0); Update_AB();
}

void app_PLACE_TOP(void)
{
MULT_TYP param;
char *figname, *insname;
char sym;

  param = GetParam(0, PTR);
  figname = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, INT);
  sym = (char)param.ent;

  Fill_obj(genPLACE_TOP(figname, insname, sym));
  if (run_mode != RUN) V_drawInsRef();
  genDEF_AB(0, 0, 0, 0); Update_AB();
}

void app_PLACE_BOTTOM(void)
{
MULT_TYP param;
char *figname, *insname;
char sym;

  param = GetParam(0, PTR);
  figname = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, INT);
  sym = (char)param.ent;

  Fill_obj(genPLACE_BOTTOM(figname, insname, sym));
  if (run_mode != RUN) V_drawInsRef();
  genDEF_AB(0, 0, 0, 0); Update_AB();
}

void app_PLACE_REF(void)
{
MULT_TYP param;
char *figname, *insname;
char sym;
char *insref, *refname;

  param = GetParam(0, PTR);
  figname = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, INT);
  sym = (char)param.ent;
  param = GetParam(12, PTR);
  insref = param.ptr;
  param = GetParam(16, PTR);
  refname = param.ptr;

  Fill_obj(genPLACE_REF(figname, insname, sym, insref, refname));
  if (run_mode != RUN) V_drawInsRef();
  genDEF_AB(0, 0, 0, 0); Update_AB();
}

void app_PLACE_ON(void)
{
MULT_TYP param;
char *figname, *t1, *t2, *c1, *c2;
long i1, i2;
char sym;

  param = GetParam(0, PTR);
  figname = param.ptr;
  param = GetParam(4, PTR);
  t1 = param.ptr;
  param = GetParam(8, PTR);
  c1 = param.ptr;
  param = GetParam(12, INT);
  i1 = param.ent;
  param = GetParam(16, INT);
  sym = (char)param.ent;
  param = GetParam(20, PTR);
  t2 = param.ptr;
  param = GetParam(24, PTR);
  c2 = param.ptr;
  param = GetParam(28, INT);
  i2 = param.ent;

  Fill_obj(genPLACE_ON(figname, t1, c1, i1, sym, t2, c2, i2));
  if (run_mode != RUN) V_drawInsRef();
  genDEF_AB(0, 0, 0, 0); Update_AB();
}

void app_PHCON(void)
{
MULT_TYP param;
char orient, *name;
long x, y, width;
char layer;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, PTR);
  name = param.ptr;
  param = GetParam(12, INT);
  orient = (char)param.ent;
  param = GetParam(16, INT);
  x = param.ent;
  param = GetParam(20, INT);
  y = param.ent;

  Fill_obj(genPHCON(layer, width, name, orient, x, y));
}

void app_PHSEG(void)
{
MULT_TYP param;
char layer;
long width, x1, y1, x2, y2;
char *name;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, PTR);
  name = param.ptr;
  param = GetParam(12, INT);
  x1 = param.ent;
  param = GetParam(16, INT);
  y1 = param.ent;
  param = GetParam(20, INT);
  x2 = param.ent;
  param = GetParam(24, INT);
  y2 = param.ent;

  Fill_obj(genPHSEG(layer, width, name, x1, y1, x2, y2));
}


void app_PHREF(void)
{
MULT_TYP param;
char *type, *name;
long x, y;

  param = GetParam(0, PTR);
  type = param.ptr;
  param = GetParam(4, PTR);
  name = param.ptr;
  param = GetParam(8, INT);
  x = param.ent;
  param = GetParam(12, INT);
  y = param.ent;

  Fill_obj(genPHREF(type, name, x, y ));
}

void app_PHVIA(void)
{
MULT_TYP param;
char type;
long x, y;

  param = GetParam(0, INT);
  type = (char)param.ent;
  param = GetParam(4, INT);
  x = param.ent;
  param = GetParam(8, INT);
  y = param.ent;

  Fill_obj(genPHVIA(type, x, y));
}

void app_PLACE_CON_REF(void)
{
MULT_TYP param;
char *insname, *refname, *conname;
char layer, face;
long width;

  param = GetParam(0, PTR);
  insname = param.ptr;
  param = GetParam(4, PTR);
  refname = param.ptr;
  param = GetParam(8, PTR);
  conname = param.ptr;
  param = GetParam(12, INT);
  layer = (char)param.ent;
  param = GetParam(16, INT);
  width = param.ent;
  param = GetParam(20, INT);
  face = (char)param.ent;

  Fill_obj(genPLACE_CON_REF(insname, refname, conname, layer, width, face));
}

void app_PLACE_SEG_REF(void)
{
MULT_TYP param;
char *insname, *refname;
char layer, face;
long width;

  param = GetParam(0, PTR);
  insname = param.ptr;
  param = GetParam(4, PTR);
  refname = param.ptr;
  param = GetParam(8, INT);
  layer = (char)param.ent;
  param = GetParam(12, INT);
  width = param.ent;
  param = GetParam(16, INT);
  face = (char)param.ent;

  Fill_obj(genPLACE_SEG_REF(insname, refname, layer, width, face));
}

void app_PLACE_VIA_REF(void)
{
MULT_TYP param;
char *insname, *refname;
char type;

  param = GetParam(0, PTR);
  insname = param.ptr;
  param = GetParam(4, PTR);
  refname = param.ptr;
  param = GetParam(8, INT);
  type = (char)param.ent;

  Fill_obj(genPLACE_VIA_REF(insname, refname, type));
}

void app_COPY_UP_SEG(void)
{
MULT_TYP param;
char *locname;
char *insname;
char *newname;

  param = GetParam(0, PTR);
  locname = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, PTR);
  newname = param.ptr;

  Fill_obj(genCOPY_UP_SEG(locname, insname, newname));
}

void app_COPY_UP_REF(void)
{
MULT_TYP param;
char *refname;
char *insname;
char *newname;

  param = GetParam(0, PTR);
  refname = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, PTR);
  newname = param.ptr;

  
  Fill_obj(genCOPY_UP_REF(refname, insname, newname));
}

void app_COPY_UP_CON_FACE(void)
{
MULT_TYP param;
char orient;
char *insname;
char *locname;
char *newname;

  param = GetParam(0, INT);
  orient = (char)param.ent;
  param = GetParam(4, PTR);
  locname = param.ptr;
  param = GetParam(8, PTR);
  insname = param.ptr;
  param = GetParam(12, PTR);
  newname = param.ptr;

  /* retrieving copy_up_con results */
  Fill_obj(genCOPY_UP_CON_FACE(orient, locname, insname, newname));
}

void app_COPY_UP_CON(void)
{
MULT_TYP param;
long  index;
char *insname;
char *locname;
char *newname;

  param = GetParam(0, INT);
  index = (long)param.ent;
  param = GetParam(4, PTR);
  locname = param.ptr;
  param = GetParam(8, PTR);
  insname = param.ptr;
  param = GetParam(12, PTR);
  newname = param.ptr;

  /* retrieving COPY_UP_CON results */
  Fill_obj(genCOPY_UP_CON(index, locname, insname, newname));
}

void app_THRU_CON_H(void)
{
MULT_TYP param;
chain_list *ptchain;
char layer;
char *name;
long width, y;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, PTR);
  name = param.ptr;
  param = GetParam(12, INT);
  y = param.ent;

  Fill_obj(genTHRU_CON_H(layer, width, name, y));
}

void app_THRU_CON_V(void)
{
MULT_TYP param;
char layer;
char *name;
long width, x;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, PTR);
  name = param.ptr;
  param = GetParam(12, INT);
  x = param.ent;

  Fill_obj(genTHRU_CON_V(layer, width, name, x));
}

void app_THRU_H(void)
{
MULT_TYP param;
char layer;
long width, y;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, INT);
  y = param.ent;

  Fill_obj(genTHRU_H(layer, width, y));
}

void app_THRU_V(void)
{
MULT_TYP param;
char layer;
long width, x;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, INT);
  x = param.ent;

  Fill_obj(genTHRU_V(layer, width, x));
}

void app_GET_CON_X(void)
{
MULT_TYP param;
char *insname;
char *refname;
char face;

  param = GetParam(0, PTR);
  insname = param.ptr;
  param = GetParam(4, PTR);
  refname = param.ptr;
  param = GetParam(8, INT);
  face = (char)param.ent;

  I_reg[25] = (int)genGET_CON_X(insname, refname, face);
}

void app_GET_CON_Y(void)
{
MULT_TYP param;
char *insname;
char *refname;
char face;

  param = GetParam(0, PTR);
  insname = param.ptr;
  param = GetParam(4, PTR);
  refname = param.ptr;
  param = GetParam(8, INT);
  face = (char)param.ent;

  I_reg[25] = (int)genGET_CON_Y(insname, refname, face);
}

void app_GET_INS_X(void)
{
MULT_TYP param;
char *insname;

  param = GetParam(0, PTR);
  insname = param.ptr;

  I_reg[25] = (int)genGET_INS_X(insname);
}

void app_GET_INS_Y(void)
{
MULT_TYP param;
char *insname;

  param = GetParam(0, PTR);
  insname = param.ptr;

  I_reg[25] = (int)genGET_INS_Y(insname);
}

void app_GET_PHFIG_REF_X(void)
{
MULT_TYP param;
char *refname;

  param = GetParam(0, PTR);
  refname = param.ptr;

  I_reg[25] = (int)genGET_PHFIG_REF_X(refname);
}

void app_GET_PHFIG_REF_Y(void)
{
MULT_TYP param;
char *refname;

  param = GetParam(0, PTR);
  refname = param.ptr;

  I_reg[25] = (int)genGET_PHFIG_REF_Y(refname);
}

void app_GET_REF_X(void)
{
MULT_TYP param;
char *insname;
char *refname;

  param = GetParam(0, PTR);
  insname = param.ptr;
  param = GetParam(4, PTR);
  refname = param.ptr;

  I_reg[25] = (int)genGET_REF_X(insname, refname);
}

void app_GET_REF_Y(void)
{
MULT_TYP param;
char *insname;
char *refname;

  param = GetParam(0, PTR);
  insname = param.ptr;
  param = GetParam(4, PTR);
  refname = param.ptr;

  I_reg[25] = (int)genGET_REF_Y(insname, refname);
}

void app_WIDTH(void)
{
MULT_TYP param;
char *figname;

  param = GetParam(0, PTR);
  figname = param.ptr;

  I_reg[25] = (int)genWIDTH(figname);
}

void app_HEIGHT(void)
{
MULT_TYP param;
char *figname;

  param = GetParam(0, PTR);
  figname = param.ptr;

  I_reg[25] = (int)genHEIGHT(figname);
}

void app_WIRE1(void)
{
MULT_TYP param;
chain_list *ptchain;
char layer;
long width;
char *ins1, *ins2, *con1, *con2;
char face1, face2;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, PTR);
  ins1 = param.ptr;
  param = GetParam(12, PTR);
  con1 = param.ptr;
  param = GetParam(16, INT);
  face1 = (char)param.ent;
  param = GetParam(20, PTR);
  ins2 = param.ptr;
  param = GetParam(24, PTR);
  con2 = param.ptr;
  param = GetParam(28, INT);
  face2 = (char)param.ent;

  Fill_obj(genWIRE1(layer, width, ins1, con1, face1, ins2, con2, face2));
}

void app_WIRE2(void)
{
MULT_TYP param;
chain_list *ptchain;
char layer;
long width;
char *ins1, *ins2, *con1, *con2;
char face1, face2;
long x, y;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, PTR);
  ins1 = param.ptr;
  param = GetParam(12, PTR);
  con1 = param.ptr;
  param = GetParam(16, INT);
  face1 = (char)param.ent;
  param = GetParam(20, PTR);
  ins2 = param.ptr;
  param = GetParam(24, PTR);
  con2 = param.ptr;
  param = GetParam(28, INT);
  face2 = (char)param.ent;
  param = GetParam(32, INT);
  x = param.ent;
  param = GetParam(36, INT);
  y = param.ent;

  Fill_obj(genWIRE2(layer, width, ins1, con1, face1, ins2, con2, face2, x, y));
}

void app_WIRE3(void)
{
MULT_TYP param;
chain_list *ptchain;
char layer;
long width;
char *ins1, *ins2, *con1, *con2;
char face1, face2;
long x1, y1, x2, y2;

  param = GetParam(0, INT);
  layer = (char)param.ent;
  param = GetParam(4, INT);
  width = param.ent;
  param = GetParam(8, PTR);
  ins1 = param.ptr;
  param = GetParam(12, PTR);
  con1 = param.ptr;
  param = GetParam(16, INT);
  face1 = (char)param.ent;
  param = GetParam(20, PTR);
  ins2 = param.ptr;
  param = GetParam(24, PTR);
  con2 = param.ptr;
  param = GetParam(28, INT);
  face2 = (char)param.ent;
  param = GetParam(32, INT);
  x1 = param.ent;
  param = GetParam(36, INT);
  y1 = param.ent;
  param = GetParam(40, INT);
  x2 = param.ent;
  param = GetParam(44, INT);
  y2 = param.ent;

  Fill_obj(genWIRE3(layer, width, ins1, con1, face1, ins2, con2, face2,
                 x1, y1, x2, y2));
}

void app_COPY_UP_ALL_CON(void)
{
MULT_TYP param;
char orient;
char *insname;
char cheminom;
chain_list *pc;
chain_list *ps;

  param = GetParam(0, INT);
  orient = (char)param.ent;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, INT);
  cheminom = (char)param.ent;

  Fill_obj(genCOPY_UP_ALL_CON(orient, insname, cheminom));
}

void app_COPY_UP_ALL_REF(void)
{
MULT_TYP param;
char *type, *insname;
char cheminom;
chain_list *pc;

  param = GetParam(0, PTR);
  type = param.ptr;
  param = GetParam(4, PTR);
  insname = param.ptr;
  param = GetParam(8, INT);
  cheminom = (char)param.ent;

  Fill_obj(genCOPY_UP_ALL_REF(type, insname, cheminom));
}

void app_FLATTEN_PHFIG(void)
{
MULT_TYP param;
char *insname;
char cheminom;

  param = GetParam(0, PTR);
  insname = param.ptr;
  param = GetParam(4, INT);
  cheminom = (char)param.ent;

  genFLATTEN_PHFIG(insname, cheminom);

  Ram_load();
}

void app_FLATTEN_ALL_PHINS(void)
{
char concat, catal;

  concat = (char)GetParam(0, INT).ent;
  catal = (char)GetParam(4, INT).ent;

  genFLATTEN_ALL_PHINS(concat, catal);

  Ram_load();
}
