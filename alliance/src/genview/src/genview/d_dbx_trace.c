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

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "d_dbx.h"

#define ADDRESS          -1   /* special type for hexa address output */
#define INT              1    /* gcc int type */
#define CHAR             2    /* gcc char type */
#define LINT             3    /* gcc long int type */
#define UINT             4    /* gcc unsigned int type */
#define ULINT            5    /* gcc unsigned long int type */
#define SHORT            6    /* gcc short type */
#define LLINT            7    /* gcc long long type */
#define USHORT           8    /* gcc unsigned short type */
#define LLUINT           9    /* gcc long long unsigned type */
#define SCHAR            10   /* gcc signed char type */
#define UCHAR            11   /* gcc unsigned char type */
#define FLOAT            12   /* gcc float type */
#define DOUBLE           13   /* gcc double type */
#define LDOUBLE          14   /* gcc long double type */
#define VOID             15   /* gcc void type */

#define READ_INT(X)      *((int *)(X))
#define READ_UNSINT(X)   *((unsigned int *)(X))
#define READ_CHAR(X)     *((char *)(X))
#define READ_UNSCHAR(X)  *((unsigned char *)(X))
#define READ_SHORT(X)    *((short *)(X))
#define READ_UNSSHORT(X) *((unsigned short *)(X))
#define READ_FLOAT(X)    *((float *)(X))
#define READ_DOUBLE(X)   *((double *)(X))

/* forward declarations */
void D_addTrace();
void D_deleteTrace();
void D_displayTrace();
void D_traceAll();
void D_traceVariable();
static void display_variable();
static void display_object();
static PTVARIABLE get_function();
static PTVARIABLE get_global_variable();
static PTVARIABLE get_local_variable();
static char *read_array();
static char *read_pointer();
static char *read_terminal();

/* to be put in the other file, the yac one, to avoid externals ? */
extern PTVARIABLE firstVariable;
extern PTSTRUCT firstStruct;

static PTVARIABLE get_global_variable(name)
char *name;
{
PTVARIABLE var;

	for (var = firstVariable; var; var = var->next)
		if ((var->scope == GLOBAL_SCOPE 
				|| var->scope == STATIC_FILE_SCOPE)
				&& !strcmp(var->name, name))
			return var;
	return NULL;
}

static PTVARIABLE get_function(name)
char *name;
{
PTVARIABLE var;
PTFUNC func;
extern PTFUNC first_func;

	for (var = firstVariable; var; var = var->next)
		if (var->scope == FUNCTION_DEFINITION && !strcmp(var->name, name)) {
			for (func = first_func; func; func = func->next_func)
				if (!(strcmp(func->name_func + 1, name)))
					break;
			if (!func)
				return (PTVARIABLE)-1;
			var->address = func->adr_func;
			return var;
		}
	return NULL;
}

static PTVARIABLE get_local_variable(func, varName)
PTVARIABLE func;
char *varName;
{
PTVARIABLE var;

	for (var = func->next; var && var->scope != FUNCTION_DEFINITION;
			var = var->next)
		if (!strcmp(var->name, varName))
			return var;
	return NULL;
}

char buffer[255];

void D_traceVariable(str)
char *str;
{
char *varName, *funcName;
int coreAddress;
PTVARIABLE func, var;
PTSTRUCT st;
static char string[80]; /* max len autorized by m_win input */
                        /* avoids to malloc and free for each call for */
                        /* so little space */

	strcpy(string, str);
	if ((varName = strchr(string, ':')) == NULL) {
		varName = string;
		funcName = NULL;
		if (!(var = get_global_variable(varName))) {
			sprintf(buffer, "No global variable called '%s'!\n", varName);
			T_print(buffer);
			D_deleteTrace(string);
			return;
		}
		coreAddress = var->address;
	} else {
		funcName = string;
		*varName++ = '\0'; /* points now on variable name */
		func = get_function(funcName);
		if (!(func = get_function(funcName))) {
			sprintf(buffer, "No function called '%s'!\n", funcName);
			T_print(buffer);
			*--varName = ':';
			D_deleteTrace(string);
			return;
		} else if (func == (PTVARIABLE)-1) {
			sprintf(buffer, "function '%s' not activated yet!\n", funcName);
			T_print(buffer);
			return;
		}
		if (!(var = get_local_variable(func, varName))) {
			sprintf(buffer, "No local variable called '%s' in function '%s'!\n",
						varName, funcName);
			T_print(buffer);
			*--varName = ' ';
			D_deleteTrace(string);
			return;
		}
		coreAddress = var->scope == STATIC_LOCAL_SCOPE ?
								var->address : func->address + var->address;
	}
	display_variable(funcName, var, coreAddress);
}

/* variables globales */
PT_VAR debut_var = 0;	/* tete et queue de la liste des */
PT_VAR fin_var = 0;		/* variables a tracer		*/


extern PTFUNC first_func;

static void display_variable(funcName, variable, address)
char *funcName;
PTVARIABLE variable;
int address;
{
	if (!funcName)
		fprintf(stdout,"%s = ", variable->name);
	else 
		fprintf(stdout,"%s:%s = ", funcName, variable->name);

	display_object(address, variable->typeNumber);
	T_print("\n"); /* flushes stdout on trace window */
}

extern I_Debut_Data, I_Fin_Data, I_Debut_Code, I_Fin_Code;
extern PTSTRUCT firstStruct;

static void display_object(address, type)
int address, type;
{
PTSTRUCT s, st;
int pointer;
char *read;
PTELEMENT elm;

#define TERMINAL    0
#define STRUCTURE   1
#define TYPEDEF     4
#define POINTER     8
#define ARRAY       16
#define FUNCTION    32
#define ENUMERATION 64
#define XREF        128
#define MAXLEN      255

	if (type <= 15) { /* simple straight forward printing */
		fprintf(stdout,"%s ", read_terminal(address, type));
		return;
	}

	for (s = firstStruct; s; s = s->next)
		if (s->typeNumber == type)
			break;

	switch(s->object) {
		case POINTER:
			if ((read = read_pointer(address, s->typeFrom)))
				fprintf(stdout, "%s ", read);
			return;
		case ARRAY:
			if ((read = read_array(address, s->typeFrom, s->size / 8)))
				fprintf(stdout, "%s ", read);
			/* fprintf(stdout, "\n"); */
			return;
		case STRUCTURE:
			fprintf(stdout,"{ ");
			for (elm = s->member; elm; elm = elm->next) {
				fprintf(stdout, "\n    %s : ", elm->name);
				display_object(address + elm->address / 8, elm->typeNumber);
			}
			fprintf(stdout,"\n} ");
			return;
		default :
			sprintf(buffer,"Don't know what the heck it is\n");
			return;
	}
}

static char *read_array(address, basicType, size)
int address, basicType, size;
{
int i, step, previousSize; 
static char string[255], *read_value;
static int basicSize;

	if (basicType <= 15) { /* nice, one dimension array */
		basicSize = step = basicType != CHAR ? getsize(basicType) / 8 : 1;
		if (basicType != CHAR)
			strcpy(string, "(");
		else
			strcpy(string, "\"");
		for (i = 0; i < size; i++) {
			read_value = read_terminal(address + i * step, basicType);
			if (!read_value)
				break;
			strcat(string, read_value);
			if (basicType == CHAR && read_value =='\0')
				break;
			if (basicType != CHAR)
				strcat(string, ", ");
		}
		if (basicType != CHAR) {
			string[strlen(string) - 2] = '\0';
			strcat(string, ")");
		} else
			strcat(string, "\"");
		return string;
	} else { /* oh, shit, complex array */
		previousSize = getsize(basicType) / 8;
		fprintf(stdout, "(\n");
		for (i = 0; i < size; i++)
			display_object(address + i * previousSize * basicSize, basicType);
		fprintf(stdout, ") ");
		return NULL;
	}
}

/*******************************************************************************
* get what's inside a pointer. this routine may cause a crash when called with *
* hasardous values. Trap of SIGSEV and SIGBUS needed.                          *
*******************************************************************************/
static char *read_pointer(address, basicType)
int address, basicType;
{
char string[255], *read_value;
int i, l;
int pointer = READ_INT(address); /* does what a pointer is meant for */

	if (!pointer)
		return "NULL";

	if (basicType <= 15) { /* pointer to terminals */
		sprintf(string, "(%#x) \"", pointer);
		l = strlen(string);
		if (basicType  == CHAR) { /* special case for strings */
			for (i = 0; i < MAXLEN; i++) {
				read_value = read_terminal(pointer + i, CHAR);
				if (!read_value)
					return "NULL";
				if ((string[l + i] = *read_value) == '\0') {
					string[l + i] = '"';
					string[l + i + 1] = '\0';
					break;
				}
			}
		} else
			strcpy(string, read_terminal(pointer, basicType));
		return string;
	} else { /* other elements, recursivly */
		display_object(pointer, basicType);
		return NULL;
	}
}

/*******************************************************************************
* get the value at given address. this routine may cause a crash when called   *
* with hasardous values. Trap of SIGSEV and SIGBUS needed.                     *
*******************************************************************************/
static char *read_terminal(address, basicType)
int address, basicType;
{
static char string[30]; /* 30 = strlen(MAX_DOUBLE) */

	if (!address)
		return NULL;

	switch (basicType) {
		case INT:
		case LINT:
		case LLINT:
			sprintf(string,"%d", READ_INT(address));
			return string;

		case UINT:
		case ULINT:
		case LLUINT:
			sprintf(string,"%d", READ_UNSINT(address));
			return string;
	
		case CHAR:
		case SCHAR: /* default char is signed char */
			sprintf(string,"%c", READ_CHAR(address));
			return string;
	
		case UCHAR:
			sprintf(string,"%c", READ_UNSCHAR(address));
			return string;
		
		case SHORT:
			sprintf(string,"%d", READ_SHORT(address));
			return string;
	
		case USHORT:
			sprintf(string,"%d", READ_UNSSHORT(address));
			return string;
	
		case FLOAT:
			sprintf(string,"%f", READ_FLOAT(address));
			return string;
	
		case DOUBLE:
		case LDOUBLE:
			sprintf(string,"%f", READ_DOUBLE(address));
			return string;

		case VOID:
			sprintf(string,"can't trace a void, sorry");
			return string;

		case ADDRESS:
			sprintf(string,"%#X", READ_INT(address));
			return string;
	
		default : /* unexpected types */
			sprintf(string, "complex element");
			return string;
	}
}

int D_existTrace()
{
	return (int)debut_var;
}

void D_freeTrace()
{
PT_VAR next, trace;

	for(trace = debut_var; trace; trace = next) {
		next = trace->next;
		mbkfree(trace);
	}
	debut_var = (PT_VAR)NULL;
}

void D_addTrace(varName)
char *varName;
{
PT_VAR newVar, listVar, exListVar;
char *t, *s;
extern D_FirstExec, D_FinInterp;

	if (!D_FirstExec && D_FinInterp) {
		sprintf(buffer, "You must compile and assemble befor tracing!\n");
		T_print(buffer);
		return;
	}
		
	t = s = varName;
 
	/* kill spaces in the line */
	while (*t)
		if (isalnum(*t) || *t == '_' || *t == ':') /* legal C identifier */
			*s++ = *t++;
		else
			t++;
	*s = '\0';

	if (!strcmp(varName, ""))
		return;

	for (listVar = debut_var; listVar && strcmp(listVar->var, varName);
			listVar = listVar->next);

	if (listVar) { /* already belongs to the list */
		sprintf(buffer, "The variable is already beeing traced\n");
		T_print(buffer);
		return;
	}

	newVar = (PT_VAR)mbkalloc(sizeof(VARI));
	newVar->next = NULL;
	newVar->var = (char *)mbkalloc(strlen(varName) + 1);
	strcpy(newVar->var, varName);

	if (!debut_var)
		debut_var = newVar;
	else {
		for (listVar = debut_var; listVar; listVar = listVar->next)
			exListVar = listVar;
		exListVar->next = newVar;
	}
}

void D_deleteTrace(varName)
char *varName;
{
PT_VAR tracedVar, exVar = NULL;

	for (tracedVar = debut_var;
			tracedVar && strcmp(tracedVar->var, varName);
			tracedVar = tracedVar->next) 
		exVar = tracedVar;

	if (!tracedVar) {
		sprintf(buffer, "%s is not in the list", varName);
		T_print(buffer);
		return;
	}

	if (exVar)
		exVar->next = tracedVar->next;
	else
		debut_var = tracedVar->next;

	if (!(tracedVar->next))
		fin_var = exVar;

	mbkfree(tracedVar->var);
	mbkfree(tracedVar);
}

void D_traceAll()
{
PT_VAR tracedVar;

	for (tracedVar = debut_var; tracedVar; tracedVar = tracedVar->next)
		D_traceVariable(tracedVar->var);
}

void D_displayTrace()
{
PT_VAR tracedVar;

	fprintf(stdout, "\nVariables beeing traced now :\n");

	for(tracedVar = debut_var; tracedVar; tracedVar = tracedVar->next)
		fprintf(stdout,"   %s\n",tracedVar->var);
	T_print("\n");
}

/* check for arguments :
   I assume what the system assumes : argc is an int, argv a char **
   Not verifications are done, unix_trap() will do with signals. */
void D_mainArguments()
{
extern int mainArgc;
extern char *mainArgv;
PTVARIABLE mainVar;

	if (!(mainVar = get_function("main")))
		return;
	/* I assume Pointers have the same size as INT, 8 */
	if (mainVar->next && mainVar->next->class == PARAM) {
 		*(int *)(mainVar->address + 8) = mainArgc;
		if (mainVar->next->next && mainVar->next->next->class == PARAM) {
			*(int *)(mainVar->address + 12) = (int)mainArgv;
 			/* For trace purpose, I must change the address value also. */
			mainVar->next->next->address = (int)&mainArgv;
			mainVar->next->next->scope = STATIC_LOCAL_SCOPE;
		}
	}
}
