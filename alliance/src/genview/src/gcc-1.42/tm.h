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

/*
 * Description de la machine virtuelle
 *
 * Ecrit par Arnaud COMPAN & Francois PECHEUX ** avril-juin 1990
 *
 */

#define DBX_DEBUGGING_INFO

#ifdef MY_SUN
#define CPP_PREDEFINES "-Dsun -Dunix"
#endif

#ifdef MY_VAX
#define CPP_PREDEFINES "-Dvax -Dunix"
#endif

extern int target_flags;

#define TARGET_DEFAULT 1
#define TARGET_SWITCHES  {{ "", TARGET_DEFAULT}}

#define BITS_BIG_ENDIAN
#define BYTES_BIG_ENDIAN
#define BITS_PER_UNIT 8
#define BITS_PER_WORD 32
#define UNITS_PER_WORD 4
#define POINTER_SIZE 32
#define POINTER_BOUNDARY 32
#define PARM_BOUNDARY 32
#define STACK_BOUNDARY 32
#define FUNCTION_BOUNDARY 32
#define BIGGEST_ALIGNMENT 32
#define EMPTY_FIELD_BOUNDARY 32
#define STRUCTURE_SIZE_BOUNDARY 32
#define STRICT_ALIGNMENT 

#define MAX_FIXED_MODE_SIZE BITS_PER_WORD

#define FIRST_PSEUDO_REGISTER 32 

#define FIXED_REGISTERS     \
  {                         \
    0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 1  \
  }

#define CALL_USED_REGISTERS \
  {                         \
    1, 1, 1, 1, 1, 1, 1, 1, \
    0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, \
    0, 1, 0, 0, 1, 1, 0, 1  \
  }

#define HARD_REGNO_NREGS(REGNO, MODE) \
  ((GET_MODE_SIZE(MODE) + UNITS_PER_WORD -1) / UNITS_PER_WORD)

#define HARD_REGNO_MODE_OK(REGNO, MODE) 1

#define MODES_TIEABLE_P(MODE1, MODE2) ( (MODE1) == (MODE2) )

#define STACK_POINTER_REGNUM 31 
#define FRAME_POINTER_REGNUM 30 
#define FRAME_POINTER_REQUIRED 0
#define ARG_POINTER_REGNUM 30 
#define STATIC_CHAIN_REGNUM 29 
#define STRUCT_VALUE_REGNUM 28 

enum reg_class { NO_REGS, ALL_REGS, LIM_REG_CLASSES };
#define N_REG_CLASSES (int) LIM_REG_CLASSES
#define GENERAL_REGS ALL_REGS
#define REG_CLASS_NAMES {"NO_REGS", "ALL_REGS" }
#define REG_CLASS_CONTENTS {0, -1}
#define REGNO_REG_CLASS(REGNO) ALL_REGS
#define INDEX_REG_CLASS ALL_REGS
#define BASE_REG_CLASS ALL_REGS
#define REG_CLASS_FROM_LETTER(C) NO_REGS
#define REGNO_OK_FOR_INDEX_P(REGNO) \
  ((REGNO) < 32 || (unsigned) reg_renumber[REGNO] < 32)
#define REGNO_OK_FOR_BASE_P(REGNO)  \
  ((REGNO) < 32 || (unsigned) reg_renumber[REGNO] < 32)
#define PREFERRED_RELOAD_CLASS(X,CLASS) (CLASS)
#define CLASS_MAX_NREGS(CLASS, MODE)    \
  ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)
#define CONST_OK_FOR_LETTER_P(VALUE, C) (0)
#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C) (0)

#define STACK_GROWS_DOWNWARD 1
#define FRAME_GROWS_DOWNWARD 1

#define FIRST_PARM_OFFSET(FNDECL) (8)
#define RETURN_POPS_ARGS(FUNTYPE) (0)

#define FUNCTION_VALUE(VALTYPE, FUNC)  \
  gen_rtx (REG, TYPE_MODE (VALTYPE), 25)
#define LIBCALL_VALUE(MODE) gen_rtx (REG, MODE, 25)
#define FUNCTION_VALUE_REGNO_P(N) ((N)==25)
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) (0)
#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) 0
#define CUMULATIVE_ARGS int
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE) ((CUM) = 0)
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)    \
  ((CUM) += ((MODE) != BLKmode                          \
            ? (GET_MODE_SIZE (MODE) + 3) / 4            \
            : (int_size_in_bytes (TYPE) + 3) / 4))
#define FUNCTION_ARG_REGNO_P(N) (0)

#define FUNCTION_PROLOGUE(FILE, SIZE)                     \
{                                                         \
extern char call_used_regs[];                             \
int i;                                                    \
                                                          \
  fprintf(FILE, "\tsub.si d31,d31,#4\n");                 \
  fprintf(FILE, "\tmov.si (d31),d30\n");                  \
  fprintf(FILE, "\tmov.si d30,d31\n");                    \
  if (SIZE != 0)                                          \
    fprintf(FILE, "\tsub.si d31,d31,#%d\n",SIZE);         \
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)             \
    if (regs_ever_live[i] && !call_used_regs[i] &&        \
        i != FRAME_POINTER_REGNUM) {                      \
      fprintf(FILE, "\tsub.si d31,d31,#4\n");             \
      fprintf(FILE, "\tmov.si (d31),%s\n",reg_names[i]);  \
    }                                                     \
}

#define FUNCTION_PROFILER(FILE, LABELNO)  \
  fprintf(FILE,"\t; ----- Profiler\n") ;

#define EXIT_IGNORE_STACK 0

#define FUNCTION_EPILOGUE(FILE, SIZE)                     \
{                                                         \
extern char call_used_regs[];                             \
int i;                                                    \
                                                          \
  for (i = FIRST_PSEUDO_REGISTER - 1; i >= 0; i--)        \
    if (regs_ever_live[i] && !call_used_regs[i] &&        \
        i!=FRAME_POINTER_REGNUM) {                        \
      fprintf(FILE, "\tmov.si %s,(d31)\n",reg_names[i]);  \
      fprintf(FILE, "\tadd.si d31,d31,#4\n");             \
    }                                                     \
  if (SIZE != 0)                                          \
    fprintf(FILE, "\tadd.si d31,d31,#%d\n", SIZE);        \
  fprintf(FILE, "\tmov.si d30,(d31)\n");                  \
  fprintf(FILE, "\tadd.si d31,d31,#4\n");                 \
  fprintf(FILE, "\tret\n");                               \
}

#define FIX_FRAME_POINTER_ADDRESS(ADDR,DEPTH)             \
{                                                         \
  puts("Ah que schisme dans FIX_FRAME_POINTER_ADDRESS");  \
  abort();                                                \
}

#define CONSTANT_ADDRESS_P(X) CONSTANT_P (X)

#define MAX_REGS_PER_ADDRESS 1

#ifndef REG_OK_STRICT
#define REG_OK_FOR_INDEX_P(X) (1)
#define REG_OK_FOR_BASE_P(X)  (1)
#else
#define REG_OK_FOR_INDEX_P(X) REGNO_OK_FOR_INDEX_P (REGNO (X))
#define REG_OK_FOR_BASE_P(X)  REGNO_OK_FOR_BASE_P (REGNO (X))
#endif

#define REG_OK_FOR_P(X) (REG_OK_FOR_INDEX_P(X) || REG_OK_FOR_BASE_P(X))

#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)           \
{                                                         \
  if (CONSTANT_ADDRESS_P(X))                              \
    goto ADDR ;                                           \
  else if (GET_CODE(X) == CONST_INT)                      \
    goto ADDR ;                                           \
  else if (REG_P(X) && REG_OK_FOR_P(X))                   \
    goto ADDR ;                                           \
  else if (GET_CODE(X) == SYMBOL_REF)                     \
    goto ADDR ;                                           \
  else if (GET_CODE(X) == PLUS) {                         \
    if (REG_P(XEXP(X,0)) && REG_OK_FOR_P(XEXP(X,0)) &&    \
        GET_CODE(XEXP(X,1))==CONST_INT)                   \
      goto ADDR ;                                         \
    if (REG_P(XEXP(X,1)) && REG_OK_FOR_P(XEXP(X,1))       \
        && GET_CODE(XEXP(X,0))==CONST_INT)                \
      goto ADDR ;                                         \
  }                                                       \
}

#define LEGITIMIZE_ADDRESS(X,OLDX,MODE,WIN) { } 

#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL)

#define LEGITIMATE_CONSTANT_P(X) (1)

#define CASE_VECTOR_MODE SImode
#define CASE_TAKES_INDEX_RAW

/* #define CASE_VECTOR_PC_RELATIVE */

#define IMPLICIT_FIX_EXPR FIX_ROUND_EXPR

#define EASY_DIV_EXPR TRUNC_DIV_EXPR

#define DEFAULT_SIGNED_CHAR 1

#define MOVE_MAX 4

#define SLOW_BYTE_ACCESS 0

#define SHIFT_COUNT_TRUNCATED

#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1

#define STORE_FLAG_VALUE 1

#define Pmode SImode

#define FUNCTION_MODE SImode

#define NO_FUNCTION_CSE

#define CONST_COSTS(RTX,CODE)                                  \
  case CONST_INT:                                              \
  case CONST:                                                  \
  case LABEL_REF:                                              \
  case SYMBOL_REF:                                             \
  case CONST_DOUBLE:                                           \
  case PLUS:                                                   \
    return 1;

#define CC_IN_FCCR 04000

#define NOTICE_UPDATE_CC(EXP, INSN) notice_update_cc((EXP))

#define TEXT_SECTION_ASM_OP "\n\t.text"
#define DATA_SECTION_ASM_OP "\n\t.data"
#define ASM_APP_ON ""
#define ASM_APP_OFF ""

#define ASM_FILE_START(FILE)                                   \
  do {                                                         \
  extern char *version_string, *language_string;               \
    {                                                          \
    int len = strlen(dump_base_name);                          \
    char *na = dump_base_name + len;                           \
    char shorter[15];                                          \
      while (na > dump_base_name) {                            \
        if (na[-1] == '/')                                     \
          break;                                               \
        na--;                                                  \
      }                                                        \
      strncpy (shorter, na, 14);                               \
      shorter[14] = '\0';                                      \
      fprintf(FILE, "\t.file\t\"%s\"\n", shorter);             \
    }                                                          \
    fprintf(FILE, "\t.version\t\"%s %s\"\n",                   \
             language_string, version_string);                 \
  } while (0)
 
#define REGISTER_NAMES                                         \
{                                                              \
  "d0","d1","d2","d3","d4","d5","d6","d7","d8","d9",           \
  "d10","d11","d12","d13","d14","d15","d16","d17","d18","d19", \
  "d20","d21","d22","d23","d24","d25","d26","d27","d28","d29", \
  "d30","d31"                                                  \
}
 
#define DBX_REGISTER_NUMBER(REGNO) (REGNO)

#define ASM_OUTPUT_LABEL(FILE,NAME)                           \
  (assemble_name(FILE, NAME), fputs (":\n", FILE))

#define ASM_GLOBALIZE_LABEL(FILE,NAME)                         \
{                                                              \
  fputs("\t.global\t", FILE);                                  \
  assemble_name(FILE, NAME);                                   \
  fputs("\n", FILE);                                           \
}

#define ASM_OUTPUT_LABELREF(FILE,NAME)                         \
  fprintf(FILE, "_%s", NAME)

#define ASM_OUTPUT_INTERNAL_LABEL(FILE,PREFIX,NUM)             \
  fprintf(FILE, ".%s%d:\n", PREFIX, NUM)

#define ASM_GENERATE_INTERNAL_LABEL(LABEL,PREFIX,NUM)          \
  sprintf(LABEL, "*#.%s%d", PREFIX, NUM)

#define ASM_OUTPUT_DOUBLE(FILE,VALUE)                          \
  fprintf(FILE, "\t.double %.20e\n", (VALUE))

#define ASM_OUTPUT_FLOAT(FILE,VALUE)                           \
  fprintf(FILE, "\t.float %.12e\n", (VALUE))

#define ASM_OUTPUT_INT(FILE,VALUE)                             \
( fprintf(FILE, "\t.int "),                                    \
  output_addr_const(FILE, (VALUE)),                            \
  fprintf(FILE, "\n")                                          \
)

#define ASM_OUTPUT_SHORT(FILE,VALUE)                           \
( fprintf(FILE, "\t.short "),                                  \
  output_addr_const(FILE, (VALUE)),                            \
  fprintf(FILE, "\n")                                          \
)
 
#define ASM_OUTPUT_CHAR(FILE,VALUE)                            \
( fprintf(FILE, "\t.char "),                                   \
  output_addr_const(FILE, (VALUE)),                            \
  fprintf(FILE, "\n")                                          \
)

#define ASM_OUTPUT_BYTE(FILE,VALUE)                            \
  fprintf(FILE, "\t.byte 0x%x\n", (VALUE))
 
#define ASM_OUTPUT_ASCII(FILE, P, SIZE) output_ascii(FILE, P, SIZE)

#define ASM_OUTPUT_ADDR_VEC_PROLOGUE(FILE, MODE, LEN)          \
  fprintf(FILE, "\tjmp r1\n");

#define ASM_OUTPUT_REG_PUSH(FILE,REGNO)                        \
  fprintf(FILE, "push \n",                                     \
           ((REGNO) < 32 ? "" : "f"), reg_names[REGNO])
 
#define ASM_OUTPUT_REG_POP(FILE,REGNO)                         \
  fprintf(FILE, "pop \n",                                      \
           ((REGNO) < 32 ? "" : "f"), reg_names[REGNO])
 
#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE)                   \
  fprintf(FILE, "\t.L%d\n", VALUE)
          
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, VALUE, REL)             \
  fprintf(FILE, "\tword .L%d-.L%d\n", VALUE, REL)
 
#define ASM_OUTPUT_ALIGN(FILE,LOG)                             \
  if ((LOG) != 0)                                              \
    fprintf (FILE, "\t.align %d\n", 1<<(LOG))
 
#define ASM_OUTPUT_SKIP(FILE,SIZE)                             \
  fprintf(FILE, "\t.skip %d\n", (SIZE))
 
#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)           \
( fputs ("\t.comm ", (FILE)),                                  \
  assemble_name ((FILE), (NAME)),                              \
  fprintf ((FILE), ",%d\n", (ROUNDED))                         \
)
 
#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)            \
( fprintf ((FILE), "\t.data\n"),                               \
  fprintf ((FILE), "\t.align %d\n", (SIZE) <= 4 ? 4 : 8),      \
  assemble_name ((FILE), (NAME)),                              \
  fprintf ((FILE), ":\n\t.skip %d\n", (ROUNDED)),              \
  fprintf ((FILE), "\t.text\n")                                \
)
  
#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO)         \
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10),           \
  sprintf ((OUTPUT), "%s_%d", (NAME), (LABELNO))               \
)
  
#define ASM_OPEN_PAREN "("
#define ASM_CLOSE_PAREN ")"
 
/* Define results of standard character escape sequences.  */
#define TARGET_BELL 007
#define TARGET_BS 010
#define TARGET_TAB 011
#define TARGET_NEWLINE 012
#define TARGET_VT 013
#define TARGET_FF 014
#define TARGET_CR 015

#define PRINT_OPERAND(FILE, X, CODE)                                    \
{                                                                       \
  /* fprintf(FILE,"'%d'",GET_CODE(X)) ; */                              \
  if (GET_CODE (X) == REG)                                              \
    fprintf (FILE, "%s", reg_names[REGNO (X)]);                         \
  else if (GET_CODE (X) == MEM)                                         \
    output_address (XEXP (X, 0));                                       \
  else if (GET_CODE (X) == SYMBOL_REF)                                  \
    output_addr_const (FILE, X);                                        \
  else if (GET_CODE (X) == LABEL_REF)                                   \
    output_addr_const (FILE, X);                                        \
  else if (GET_CODE (X) == CONST)                                       \
    {                                                                   \
        fprintf(FILE,"$") ;                                             \
        output_addr_const (FILE, XEXP(X,0));                            \
    }                                                                   \
  else if (GET_CODE (X) == CONST_DOUBLE && GET_MODE (X) == SFmode)      \
    {                                                                   \
      union { double d; int i[2]; } u;                                  \
      union { float f; int i; } u1;                                     \
      u.i[0] = CONST_DOUBLE_LOW (X); u.i[1] = CONST_DOUBLE_HIGH (X);    \
      u1.f = u.d;                                                       \
      if (CODE == 'f')                                                  \
        fprintf (FILE, "#%f", u1.f);                                    \
      else                                                              \
        fprintf (FILE, "#%x", u1.i);                                    \
    }                                                                   \
  else if (GET_CODE (X) == CONST_DOUBLE && GET_MODE (X) != DImode)      \
    {                                                                   \
      union { double d; int i[2]; } u;                                  \
      u.i[0] = CONST_DOUBLE_LOW (X); u.i[1] = CONST_DOUBLE_HIGH (X);    \
      fprintf (FILE, "#%f", u.d);                                       \
    }                                                                   \
  else if (GET_CODE(X) == CONST_INT)                                    \
    {                                                                   \
      fprintf(FILE,"#") ;                                               \
      output_addr_const (FILE, X);                                      \
    }                                                                   \
  else if (GET_CODE(X) == PLUS)                                         \
    {                                                                   \
      if (GET_CODE(XEXP(X,0)) == CONST_INT)                             \
        output_addr_const (FILE, XEXP(X,0));                            \
      if (GET_CODE(XEXP(X,1)) == CONST_INT)                             \
        output_addr_const (FILE, XEXP(X,1));                            \
      if (GET_CODE(XEXP(X,0)) == REG)                                   \
        fprintf (FILE, "(%s)", reg_names[REGNO (XEXP(X,0))]);           \
      if (GET_CODE(XEXP(X,1)) == REG)                                   \
        fprintf (FILE, "(%s)", reg_names[REGNO (XEXP(X,1))]);           \
    }                                                                   \
  else                                                                  \
    fprintf(FILE, "*print_operand %d*",GET_CODE(X)) ;                   \
}

#define PRINT_OPERAND_ADDRESS(FILE, ADDR)                               \
{                                                                       \
  switch (GET_CODE (ADDR))                                              \
    {                                                                   \
      case REG:                                                         \
        fprintf (FILE, "(%s)", reg_names[REGNO (ADDR)]);                \
        break;                                                          \
      case MEM:                                                         \
        output_address (XEXP (ADDR, 0));                                \
        break;                                                          \
      case CONST_INT:                                                   \
        fprintf(FILE,"#") ;                                             \
        output_addr_const (FILE, ADDR);                                 \
        break;                                                          \
      case SYMBOL_REF:                                                  \
        fprintf(FILE,"$") ;                                             \
        output_addr_const (FILE, ADDR);                                 \
        break;                                                          \
      case CONST:   /* on suppose que c'est une addition */             \
        fprintf(FILE,"$") ;                                             \
        output_addr_const (FILE, XEXP(ADDR,0));                         \
        break ;                                                         \
      case PLUS:                                                        \
        if (GET_CODE(XEXP(ADDR,0)) == CONST_INT)                        \
            output_addr_const (FILE, XEXP(ADDR,0));                     \
        if (GET_CODE(XEXP(ADDR,1)) == CONST_INT)                        \
            output_addr_const (FILE, XEXP(ADDR,1));                     \
        if (GET_CODE(XEXP(ADDR,0)) == REG)                              \
          fprintf (FILE, "(%s)", reg_names[REGNO (XEXP(ADDR,0))]);      \
        if (GET_CODE(XEXP(ADDR,1)) == REG)                              \
          fprintf (FILE, "(%s)", reg_names[REGNO (XEXP(ADDR,1))]);      \
        break;                                                          \
      default:                                                          \
        fprintf (FILE, "*print_operand_address %d*",GET_CODE(ADDR));    \
        break;                                                          \
    }                                                                   \
}

#define SFVALUE float
#define INTIFY(FLOATVAL) FLOATVAL

#define ASM_OPERAND_LETTER '#'
#define STARTING_FRAME_OFFSET 0
