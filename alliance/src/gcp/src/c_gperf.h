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
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/* C code produced by gperf version 2.5 (GNU C++ version) */
/* Command-line: gperf -p -j1 -i 1 -g -o -t -G -N is_reserved_word -k1,3,$ ./c-parse.gperf  */
/* Command-line: gperf -p -j1 -i 1 -g -o -t -N is_reserved_word -k1,3,$ c-parse.gperf  */ 
struct resword { char *name; short token; enum rid rid; };

#define TOTAL_KEYWORDS 79
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 20
#define MIN_HASH_VALUE 10
#define MAX_HASH_VALUE 144
/* maximum key range = 135, duplicates = 0 */

#ifdef __GNUC__
inline
#endif
static unsigned int
hash (str, len)
     register char *str;
     register int unsigned len;
{
  static unsigned char asso_values[] =
    {
     145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
     145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
     145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
     145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
     145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
     145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
     145, 145, 145, 145,  25, 145, 145, 145, 145, 145,
     145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
     145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
     145, 145, 145, 145, 145,   1, 145,  46,   8,  15,
      61,   6,  36,  48,   3,   5, 145,  18,  63,  25,
      29,  76,   1, 145,  13,   2,   1,  51,  37,   9,
       9,   1,   3, 145, 145, 145, 145, 145,
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 3:
        hval += asso_values[(unsigned int)str[2]];
      case 2:
      case 1:
        hval += asso_values[(unsigned int)str[0]];
        break;
    }
  return hval + asso_values[(unsigned int)str[len - 1]];
}

static struct resword wordlist[] =
{
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"",}, 
      {"int",  tok_TYPESPEC, RID_INT},
      {"",}, {"",}, 
      {"__typeof__",  tok_TYPEOF, NORID},
      {"__signed__",  tok_TYPESPEC, RID_SIGNED},
      {"__imag__",  tok_IMAGPART, NORID},
      {"switch",  tok_SWITCH, NORID},
      {"__inline__",  tok_SCSPEC, RID_INLINE},
      {"else",  tok_ELSE, NORID},
      {"__iterator__",  tok_SCSPEC, RID_ITERATOR},
      {"__inline",  tok_SCSPEC, RID_INLINE},
      {"__extension__",  tok_EXTENSION, NORID},
      {"struct",  tok_STRUCT, NORID},
      {"__real__",  tok_REALPART, NORID},
      {"__const",  tok_TYPE_QUAL, RID_CONST},
      {"while",  tok_WHILE, NORID},
      {"__const__",  tok_TYPE_QUAL, RID_CONST},
      {"case",  tok_CASE, NORID},
      {"__complex__",  tok_TYPESPEC, RID_COMPLEX},
      {"__iterator",  tok_SCSPEC, RID_ITERATOR},
      {"bycopy",  tok_TYPE_QUAL, RID_BYCOPY},
      {"",}, {"",}, {"",}, 
      {"__complex",  tok_TYPESPEC, RID_COMPLEX},
      {"",}, 
      {"in",  tok_TYPE_QUAL, RID_IN},
      {"break",  tok_BREAK, NORID},
      {"@defs",  tok_DEFS, NORID},
      {"",}, {"",}, {"",}, 
      {"extern",  tok_SCSPEC, RID_EXTERN},
      {"if",  tok_IF, NORID},
      {"typeof",  tok_TYPEOF, NORID},
      {"typedef",  tok_SCSPEC, RID_TYPEDEF},
      {"__typeof",  tok_TYPEOF, NORID},
      {"sizeof",  tok_SIZEOF, NORID},
      {"",}, 
      {"return",  tok_RETURN, NORID},
      {"const",  tok_TYPE_QUAL, RID_CONST},
      {"__volatile__",  tok_TYPE_QUAL, RID_VOLATILE},
      {"@private",  tok_PRIVATE, NORID},
      {"@selector",  tok_SELECTOR, NORID},
      {"__volatile",  tok_TYPE_QUAL, RID_VOLATILE},
      {"__asm__",  tok_ASM_KEYWORD, NORID},
      {"",}, {"",}, 
      {"continue",  tok_CONTINUE, NORID},
      {"__alignof__",  tok_ALIGNOF, NORID},
      {"__imag",  tok_IMAGPART, NORID},
      {"__attribute__",  tok_ATTRIBUTE, NORID},
      {"",}, {"",}, 
      {"__attribute",  tok_ATTRIBUTE, NORID},
      {"for",  tok_FOR, NORID},
      {"",}, 
      {"@encode",  tok_ENCODE, NORID},
      {"id",  tok_OBJECTNAME, RID_ID},
      {"static",  tok_SCSPEC, RID_STATIC},
      {"@interface",  tok_INTERFACE, NORID},
      {"",}, 
      {"__signed",  tok_TYPESPEC, RID_SIGNED},
      {"",}, 
      {"__label__",  tok_LABEL, NORID},
      {"",}, {"",}, 
      {"__asm",  tok_ASM_KEYWORD, NORID},
      {"char",  tok_TYPESPEC, RID_CHAR},
      {"",}, 
      {"inline",  tok_SCSPEC, RID_INLINE},
      {"out",  tok_TYPE_QUAL, RID_OUT},
      {"register",  tok_SCSPEC, RID_REGISTER},
      {"__real",  tok_REALPART, NORID},
      {"short",  tok_TYPESPEC, RID_SHORT},
      {"",}, 
      {"enum",  tok_ENUM, NORID},
      {"inout",  tok_TYPE_QUAL, RID_INOUT},
      {"",}, 
      {"oneway",  tok_TYPE_QUAL, RID_ONEWAY},
      {"union",  tok_UNION, NORID},
      {"",}, 
      {"__alignof",  tok_ALIGNOF, NORID},
      {"",}, 
      {"@implementation",  tok_IMPLEMENTATION, NORID},
      {"",}, 
      {"@class",  tok_CLASS, NORID},
      {"",}, 
      {"@public",  tok_PUBLIC, NORID},
      {"asm",  tok_ASM_KEYWORD, NORID},
      {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"default",  tok_DEFAULT, NORID},
      {"",}, 
      {"void",  tok_TYPESPEC, RID_VOID},
      {"",}, 
      {"@protected",  tok_PROTECTED, NORID},
      {"@protocol",  tok_PROTOCOL, NORID},
      {"",}, {"",}, {"",}, 
      {"volatile",  tok_TYPE_QUAL, RID_VOLATILE},
      {"",}, {"",}, 
      {"signed",  tok_TYPESPEC, RID_SIGNED},
      {"float",  tok_TYPESPEC, RID_FLOAT},
      {"@end",  tok_END, NORID},
      {"",}, {"",}, 
      {"unsigned",  tok_TYPESPEC, RID_UNSIGNED},
      {"@compatibility_alias",  tok_ALIAS, NORID},
      {"double",  tok_TYPESPEC, RID_DOUBLE},
      {"",}, {"",}, 
      {"auto",  tok_SCSPEC, RID_AUTO},
      {"",}, 
      {"goto",  tok_GOTO, NORID},
      {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, {"",}, 
      {"do",  tok_DO, NORID},
      {"",}, {"",}, {"",}, {"",}, 
      {"long",  tok_TYPESPEC, RID_LONG},
};

#ifdef __GNUC__
inline
#endif
struct resword *
c_is_reserved_word (str, len)
     register char *str;
     register unsigned int len;
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register char *s = wordlist[key].name;

          if (*s == *str && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
