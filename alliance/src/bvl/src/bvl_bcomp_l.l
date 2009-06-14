%{
#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "beh.h"
#include "bvl.h"
#include "bvl_bcomp_y.h"


/* ###--------------------------------------------------------------### */
/* function	: search						*/
/* description	: check that an identifier is a reserved word or not	*/
/* called func.	: addht, addhtitem, gethtitem, namealloc		*/
/* ###--------------------------------------------------------------### */

static long search (key)

char  *key;

  {
  static ht *pt_hash = NULL;

  if (pt_hash == NULL)
    {
    pt_hash = addht (107);

    addhtitem (pt_hash, namealloc("abs")          , ABS          );
    addhtitem (pt_hash, namealloc("access")       , ACCESS       );
    addhtitem (pt_hash, namealloc("after")        , AFTER        );
    addhtitem (pt_hash, namealloc("alias")        , ALIAS        );
    addhtitem (pt_hash, namealloc("all")          , ALL          );
    addhtitem (pt_hash, namealloc("and")          , tok_AND         );
    addhtitem (pt_hash, namealloc("architecture") , ARCHITECTURE );
    addhtitem (pt_hash, namealloc("array")        , ARRAY        );
    addhtitem (pt_hash, namealloc("assert")       , ASSERT       );
    addhtitem (pt_hash, namealloc("attribute")    , ATTRIBUTE    );

    addhtitem (pt_hash, namealloc("begin")        , _BEGIN       );
    addhtitem (pt_hash, namealloc("bit")          , BIT          );
    addhtitem (pt_hash, namealloc("bit_vector")   , BIT_VECTOR   );
    addhtitem (pt_hash, namealloc("block")        , BLOCK        );
    addhtitem (pt_hash, namealloc("body")         , BODY         );
    addhtitem (pt_hash, namealloc("buffer")       , BUFFER       );
    addhtitem (pt_hash, namealloc("bus")          , BUS          );

    addhtitem (pt_hash, namealloc("case")         , CASE         );
    addhtitem (pt_hash, namealloc("component")    , COMPONENT    );
    addhtitem (pt_hash, namealloc("configuration"), CONFIGURATION);
    addhtitem (pt_hash, namealloc("constant")     , CONSTANT     );

    addhtitem (pt_hash, namealloc("disconnect")   , DISCONNECT   );
    addhtitem (pt_hash, namealloc("downto")       , DOWNTO       );

    addhtitem (pt_hash, namealloc("else")         , ELSE         );
    addhtitem (pt_hash, namealloc("elsif")        , ELSIF        );
    addhtitem (pt_hash, namealloc("end")          , _END         );
    addhtitem (pt_hash, namealloc("entity")       , ENTITY       );
    addhtitem (pt_hash, namealloc("error")        , ERROR        );
    addhtitem (pt_hash, namealloc("exit")         , _EXIT        );

    addhtitem (pt_hash, namealloc("file")         , _FILE        );
    addhtitem (pt_hash, namealloc("for")          , FOR          );
    addhtitem (pt_hash, namealloc("fs")           , FS           );
    addhtitem (pt_hash, namealloc("function")     , FUNCTION     );

    addhtitem (pt_hash, namealloc("generate")     , GENERATE     );
    addhtitem (pt_hash, namealloc("generic")      , GENERIC      );
    addhtitem (pt_hash, namealloc("guarded")      , GUARDED      );

    addhtitem (pt_hash, namealloc("if")           , IF           );
    addhtitem (pt_hash, namealloc("in")           , _IN          );
    addhtitem (pt_hash, namealloc("inout")        , _INOUT       );
    addhtitem (pt_hash, namealloc("is")           , IS           );

    addhtitem (pt_hash, namealloc("label")        , _LABEL       );
    addhtitem (pt_hash, namealloc("library")      , LIBRARY      );
    addhtitem (pt_hash, namealloc("linkage")      , _LINKAGE     );
    addhtitem (pt_hash, namealloc("loop")         , LOOP         );

    addhtitem (pt_hash, namealloc("map")          , MAP          );
    addhtitem (pt_hash, namealloc("mod")          , MOD          );
    addhtitem (pt_hash, namealloc("ms")           , MS           );
    addhtitem (pt_hash, namealloc("mux_bit")      , MUX_BIT      );
    addhtitem (pt_hash, namealloc("mux_vector")   , MUX_VECTOR   );

    addhtitem (pt_hash, namealloc("nand")         , _NAND        );
    addhtitem (pt_hash, namealloc("natural")      , NATURAL      );
    addhtitem (pt_hash, namealloc("new")          , NEW          );
    addhtitem (pt_hash, namealloc("next")         , _NEXT        );
    addhtitem (pt_hash, namealloc("nor")          , _NOR         );
    addhtitem (pt_hash, namealloc("not")          , _NOT         );
    addhtitem (pt_hash, namealloc("ns")           , NS           );
    addhtitem (pt_hash, namealloc("null")         , tok_NULL        );

    addhtitem (pt_hash, namealloc("of")           , OF           );
    addhtitem (pt_hash, namealloc("on")           , ON           );
    addhtitem (pt_hash, namealloc("open")         , OPEN         );
    addhtitem (pt_hash, namealloc("or")           , _OR          );
    addhtitem (pt_hash, namealloc("others")       , OTHERS       );
    addhtitem (pt_hash, namealloc("out")          , _OUT         );

    addhtitem (pt_hash, namealloc("package")      , _PACKAGE      );
    addhtitem (pt_hash, namealloc("port")         , PORT         );
    addhtitem (pt_hash, namealloc("procedure")    , PROCEDURE    );
    addhtitem (pt_hash, namealloc("process")      , PROCESS      );
    addhtitem (pt_hash, namealloc("ps")           , PS           );

    addhtitem (pt_hash, namealloc("range")        , RANGE        );
    addhtitem (pt_hash, namealloc("record")       , RECORD       );
    addhtitem (pt_hash, namealloc("reg_bit")      , REG_BIT      );
    addhtitem (pt_hash, namealloc("reg_vector")   , REG_VECTOR   );
    addhtitem (pt_hash, namealloc("register")     , REGISTER     );
    addhtitem (pt_hash, namealloc("rem")          , REM          );
    addhtitem (pt_hash, namealloc("report")       , REPORT       );
    addhtitem (pt_hash, namealloc("return")       , RETURN       );

    addhtitem (pt_hash, namealloc("select")       , SELECT       );
    addhtitem (pt_hash, namealloc("severity")     , SEVERITY     );
    addhtitem (pt_hash, namealloc("signal")       , SIGNAL       );
    addhtitem (pt_hash, namealloc("stable")       , _STABLE      );
    addhtitem (pt_hash, namealloc("subtype")      , SUBTYPE      );

    addhtitem (pt_hash, namealloc("then")         , THEN         );
    addhtitem (pt_hash, namealloc("to")           , TO           );
    addhtitem (pt_hash, namealloc("transport")    , TRANSPORT    );
    addhtitem (pt_hash, namealloc("type")         , _TYPE        );

    addhtitem (pt_hash, namealloc("units")        , UNITS        );
    addhtitem (pt_hash, namealloc("until")        , UNTIL        );
    addhtitem (pt_hash, namealloc("us")           , US           );
    addhtitem (pt_hash, namealloc("use")          , USE          );

    addhtitem (pt_hash, namealloc("variable")     , VARIABLE     );

    addhtitem (pt_hash, namealloc("wait")         , WAIT         );
    addhtitem (pt_hash, namealloc("warning")      , WARNING      );
    addhtitem (pt_hash, namealloc("when")         , WHEN         );
    addhtitem (pt_hash, namealloc("while")        , WHILE        );
    addhtitem (pt_hash, namealloc("with")         , WITH         );
    addhtitem (pt_hash, namealloc("wor_bit")      , WOR_BIT      );
    addhtitem (pt_hash, namealloc("wor_vector")   , WOR_VECTOR   );

    addhtitem (pt_hash, namealloc("xor")          , _XOR         );
    }

  return (gethtitem (pt_hash, namealloc(key)));
  }

%}

upper_case_letter 	  [A-Z]
digit 			  [0-9]
special_character   	  [\#\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\_\|]
space_character 	  [ \t]
format_effector		  [\t\v\r\l\f]
end_of_line		  \n
lower_case_letter 	  [a-z]
other_special_character   [\!\$\@\?\[\\\]\^\`\{\}\~]

graphic_character	  ({basic_graphic_character}|{lower_case_letter}|{other_special_character})
basic_graphic_character	  ({upper_case_letter}|{digit}|{special_character}|{space_character})
letter		   	  ({upper_case_letter}|{lower_case_letter})
letter_or_digit	   	  ({letter}|{digit})
decimal_literal	   	  {integer}(\.{integer})?({exponent})?
integer		   	  {digit}(_?{digit})*
exponent	   	  ([eE][-+]?{integer})
base		   	  {integer}
based_integer	   	  {extended_digit}(_?{extended_digit})*
extended_digit	   	  ({digit}|[a-fA-F])
base_specifier	  	  (B|b|O|o|X|x)

%%
[ \t]		;
\n		{ BEH_LINNUM++;        }
\&		{ return (Ampersand ); }
\'		{ return (Apostrophe); }
\(		{ return (LeftParen ); }
\)		{ return (RightParen); }
"**"		{ return (DoubleStar); }
\*		{ return (Star      ); }
\+		{ return (Plus      ); }
\,		{ return (Comma     ); }
\-		{ return (Minus     ); }
":="		{ return (VarAsgn   ); }
\:		{ return (Colon     ); }
\;		{ return (Semicolon ); }
"<="		{ return (_LESym    ); }
">="		{ return (_GESym    ); }
\<		{ return (_LTSym    ); }
\>		{ return (_GTSym    ); }
=		{ return (_EQSym    ); }
\/=		{ return (_NESym    ); }
"=>"		{ return (Arrow     ); }
"<>"		{ return (Box       ); }
\|		{ return (Bar       ); }
!		{ return (Bar       ); }
\.		{ return (Dot       ); }
\/		{ return (Slash     ); }

{letter}(_?{letter_or_digit})*		{
		long        itoken;

		itoken = search (yytext);
		if (itoken == EMPTYHT) 
		  {
		  yylval.text = namealloc (yytext);
		  return (Identifier);
		  }
		else
		  {
		  return (itoken);
		  }
					}
({decimal_literal})|({base}#{based_integer}(\.{based_integer})?#({exponent})?)|({base}:{based_integer}(\.{based_integer})?:({exponent})?)		{

		yylval.text = mbkalloc ((unsigned long)strlen(yytext)+1);
	 	strcpy (yylval.text, yytext);
		return (AbstractLit);
									}
'({graphic_character}|\"|\%)' 						{
		yylval.text = namealloc (yytext);
		return (CharacterLit);
									}
(\"({graphic_character}|(\"\")|\%)*\")|(\%({graphic_character}|(\%\%)|\")*\%) {
		yylval.text = namealloc (yytext);
		return (StringLit);
									}
{base_specifier}((\"{extended_digit}(_?{extended_digit})*\")|(\%{extended_digit}(_?{extended_digit})*\%)) 							{
		yylval.text = namealloc (yytext);
		return (BitStringLit);
									}
\-\-.*$ 	;
.		{ return (*yytext); }
%%

/* ###--------------------------------------------------------------### */
/* function	: yywrap						*/
/* description	: return 1						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

int yywrap ()
  {
  return (1);
  }

