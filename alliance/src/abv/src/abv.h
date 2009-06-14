/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/* ###--------------------------------------------------------------### */
/*									*/
/* file		: abv201.h						*/
/* date		: Feb 10 1995						*/
/* version	: v201							*/
/* contents	: defines and structure definitions used in BVL library	*/
/*									*/
/* ###--------------------------------------------------------------### */

#ifndef ABV_BVLDEF
#define ABV_BVLDEF

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__) || defined(__cplusplus)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

#define	ABV_TRACE_MASK    0x00000001	/* print messages when parsing	*/
#define	ABV_KEEP_AUX_MASK 0x00000002	/* keep internal signals	*/
#define ABV_SYNTH_MASK    0x00000004    /* special mode for synthesis   */

	/* ###------------------------------------------------------### */
	/*    functions							*/
	/* ###------------------------------------------------------### */

extern struct befig *vhdlloadbefig __P((struct befig *pt_befig, char *figname, long trace_mode));
extern void          vhdlsavebefig __P((struct befig *pthedbefig, long trace_mode));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
