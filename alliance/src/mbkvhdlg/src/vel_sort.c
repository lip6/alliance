/* ###--------------------------------------------------------------------### */
/* #                                                                        # */
/* # file : vel_sort.c                                                      # */
/* | date : Jun 03 1997                                                     | */
/* | version : 1.00                                                         | */
/* | author : Frederic Petrot, Denis Hommais                                | */
/* |                                                                        | */
/* # contents : modified sortlosig function                                 # */
/* #                                                                        # */
/* ###--------------------------------------------------------------------### */
/*
 * $Log: vel_sort.c,v $
 * Revision 1.1  2002/04/26 09:51:07  ludo
 * Initial revision
 *
 * Revision 1.2  2002/04/16 07:24:51  fred
 * Adding modifications to handle limited generics in VHDL files.
 *
 * Revision 1.1.1.1  2002/04/11 08:54:53  fred
 * Importing mbkvhdlg into the new Alliance CVS tree
 *
 * Revision 1.1.1.1  2002/02/28 12:58:31  disydent
 * Creation of Disydent CVS Tree
 *
 * Revision 1.1.1.1  2001/11/19 16:55:27  pwet
 * Changing the CVS tree structure of disydent
 *
 * Revision 1.1.1.1  2001/07/24 13:31:42  pwet
 * cvs tree of part of disydent
 *
 * Revision 1.1.1.1  2001/07/19 14:32:19  pwet
 * New cvs tree
 *
 * Revision 1.1  1998/09/16 16:11:35  pwet
 * passage a cvs
 *
 */

#include <stdlib.h>
#include <mut.h>
#include <mlo.h>

#ident "$Id: vel_sort.c,v 1.1 2002/04/26 09:51:07 ludo Exp $"

static char buffer[BUFSIZ];
static char str[BUFSIZ];

#if 0
/*******************************************************************************
*  generic compaison function :                                                *
*  unlike strcmp, ensure that 10 > 2 for vectorized strings.                   *
*  first, check strings, then check numerical values as numbers, not strings.  *
*******************************************************************************/
static int naturalstrcmp(s, t)
char *s, *t;
{
char *spt, *tpt , *st = s, *tt = t;
int u, ls, lt;

	spt = buffer, tpt = str;

	while ((!isspace(*st)) && *st)
		*spt++ = *st++;
	*spt = '\0';

	while ((!isspace(*tt)) && *tt)
		*tpt++ = *tt++;
	*tpt = '\0';

	if ((u = strcmp(buffer, str)) != 0)
		return u;

	if ((ls = strlen(s)) == (lt = strlen(t)))
		return strcmp(s, t);

	return ls - lt;
}
#endif

static int
	signalcmp(fls, sls)
losig_list **fls, **sls;
{
	return naturalstrcmp(getsigname(*fls), getsigname(*sls));
}

void
	sortlosigv(signals)
losig_list **signals;
{
long n, i;
losig_list *lc, **tc;

	if (!signals || !(*signals)) 
		return;

	/* first :
	   count signals. */
	for (n = 0, lc = *signals; lc; lc = lc->NEXT)
		n++;
	tc = (losig_list **)mbkalloc(n * sizeof(losig_list *));
	for (n = 0, lc = *signals; lc; lc = lc->NEXT)
		tc[n++] = lc;
	/* second :
	   sort them. */
	qsort((void *)tc, n, sizeof(losig_list *), signalcmp);
	/* tables :
	   the last element of the table is t[n - 1].
	   relink the list in the sorted order. */
	n--;
	*signals = tc[0];
	for (i = 0; i < n; i++)
		tc[i]->NEXT = tc[i + 1];
	tc[n]->NEXT = (losig_list *)NULL;
	mbkfree((void *)tc);
}
