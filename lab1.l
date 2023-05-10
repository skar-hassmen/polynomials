%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "term_struct.h"
  #include "y.tab.h"
%}

%%
\$[[:alpha:]]+ {
  printf("%s", yytext);
}

[[:digit:]]* {
   yylval.terms = (struct term_struct*)malloc(sizeof(struct term_struct));
   yylval.terms->coefficient = atoi(yytext);
   yylval.terms->symbol = '#';
   yylval.terms->degree = 0;
   return NUMBER;
}

[[:alpha:]] {
   yylval.terms = (struct term_struct*)malloc(sizeof(struct term_struct));
   yylval.terms->coefficient = 1;
   yylval.terms->symbol = yytext[0];
   yylval.terms->degree = 1;
   return SYMBOL;
}

\^[-]? {
  yylval.sign_degree = yytext[1];
  return SIGN_DEGREE;
}

[\n] return 0;

. return yytext[0];

%%

int yywrap() {
 return 1;
}