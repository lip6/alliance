/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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

/*******************************************************************************
* dbx defines for variable scope in order to retains correct data              *
*******************************************************************************/
#define GLOBAL_SCOPE        'G'  /* global scope */
#define FUNCTION_DEFINITION 'F'  /* function */
#define PARAMETER           'p'  /* parameter */
#define TYPE_DEFINITION     't'  /* new type, typedef */
#define NEW_STRUCTURE       'T'  /* new structure */
#define STATIC_FILE_SCOPE   'S'  /* static storage, file scope */
#define STATIC_LOCAL_SCOPE  'V'  /* static storage, local scope */

/*******************************************************************************
* dbx defines for storage class needed for address calculations                *
*******************************************************************************/
#define GLOB                32   /* global variable, all file scope */
#define A_STAT              38   /* static arrays, global or local */
#define S_STAT              40   /* simple static variable, global or local */
#define AUTO                128  /* local variable */
#define FUNCT               36   /* function */
#define PARAM               160  /* parameter */

/*******************************************************************************
* structures used to represent dbx symbolic datas                              *
*******************************************************************************/
/* This is needed to have in memory an image of the source file types and
variables in order to be able to trace the values of variables */

typedef struct TypeDefinition {      /* type declaration                      */
	struct TypeDefinition *next;      /* next one                              */
	char                  *name;      /* type name, for info only              */
	int                   typeNumber; /* dbx type in "name", actual C type     */
	int                   object;     /* final, pointer, array, structure, ... */
	int                   typeFrom;   /* comes from this type when complex     */
	int                   size;       /* structure total size                  */
	struct Element        *member;    /* link on members or elements           */
} STRUCT, *PTSTRUCT;

typedef struct Element {             /* structure field                       */
	struct Element        *next;      /* next one                              */
	char                  *name;      /* structure field name                  */
	int                   typeNumber; /* dbx type number in "name"             */
	int                   size;       /* field size                            */
	int                   address;    /* field address                         */
} ELEMENT, *PTELEMENT;


typedef struct Variable {            /* variable list                         */
	struct Variable       *next;      /* next variable                         */
	char                  *name;      /* variable name                         */
	int                   typeNumber; /* dbx type number in "name"             */
	int                   size;       /* variable size                         */
	int                   address;    /* address depends on class              */
	int                   class;      /* dbx variable storage class            */
	int                   scope;      /* dbx variable scope                    */
} VARIABLE, *PTVARIABLE;

typedef struct func {
	char              *name_func;     /* function name                         */
	int               adr_func;       /* function address in memory            */
	struct func       *next_func;     /* next function                         */
} FUNC, *PTFUNC;

/* structure de sauvegarde de nom de fonction : interp_br.c, yacc */
typedef struct fonc {
	char              *name;
	int               code_adr;
	struct fonc       *next_fonc;
} fonc, *ptfonc;


/* structure pour la liste TRACE */
typedef struct var {
	char              *var;        /* variable to be traced */
	struct var        *next;       /* next one */
} VARI , *PT_VAR;
