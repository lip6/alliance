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

/* unix traps for the interpreter. Avoid exiting like a pig */
#include <X11/Xlib.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include "d_asm.h"

jmp_buf sjbuf; /* save the stack */
BOOL Interrupt = FALSE;  /* true if  */
static char buffer[200];

/* arithmetics exeptions */
void
  arith_exept(sig, code)
int sig, code;
{
extern int LigneCourante;

  fflush(stdout);
  fflush(stderr);
  signal(SIGFPE, arith_exept);
  d_clean_err();
  sprintf(buffer,"arithmetic exception line %d\n", LigneCourante);
  T_print(buffer);
  Interrupt = TRUE;
  longjmp(sjbuf, 0);
}

/* bus error, unaligned data */
void
  bus_err(sig, code)
int sig, code;
{
extern int LigneCourante;

  fflush(stdout);
  fflush(stderr);
  signal(SIGSEGV, bus_err);
  d_clean_err();
  sprintf(buffer,"Bus error line %d\n", LigneCourante);
  T_print(buffer);
  longjmp(sjbuf, 0);
}

/* abort system call or kill */
void
  recup_abort(sig, code)
int sig, code;
{
  fflush(stdout);
  fflush(stderr);
  signal(SIGABRT, recup_abort);
  d_clean_err();
  T_print("abort caught\n");
  Interrupt = TRUE;
  longjmp(sjbuf, 0);
}

/* control c */
void
  ctrl_c(sig, code)
int sig, code;
{
extern BOOL D_ModeTrace;

  fflush(stdout);
  fflush(stderr);
  signal(SIGINT, ctrl_c);
  T_print("interrupted by user\n");
/*
  d_clean_err();
  Interrupt = TRUE;
  D_ModeTrace = TRUE;
*/
  longjmp(sjbuf, 0);
}

/* segmentation fault */
void
  seg_fault(sig, code)
int sig, code;
{
extern int LigneCourante;

  fflush(stdout);
  fflush(stderr);
  signal(SIGSEGV, seg_fault);
  d_clean_err();
  sprintf(buffer,"Segmentation fault line %d\n", LigneCourante);
  T_print(buffer);
  longjmp(sjbuf, 0);
}

/* mbk EXIT :
   process terminates, and does not exit since I wouldn't be able
   to stay under genview if it would! */
void
  terminate(sig, code)
{
extern int LigneCourante;

  fflush(stdout);
  fflush(stderr);
  signal(SIGTERM, terminate);
  sprintf(buffer, LigneCourante < 1 ? "Program got an EXIT from 'mbk'\n" :
        "Programm got an EXIT from 'mbk' line %d\n", LigneCourante);
  T_print(buffer);
  d_clean_err();
  longjmp(sjbuf, 0);
}

/* prepare signal catching */
unix_trap()
{
  signal(SIGTERM, terminate);
  signal(SIGINT, ctrl_c);
  signal(SIGALRM, SIG_IGN);

/*
  signal(SIGFPE, arith_exept);
  signal(SIGBUS, bus_err);
  signal(SIGSEGV, seg_fault);
*/
}
