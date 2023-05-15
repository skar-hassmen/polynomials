%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "term_struct.h"
  #include "y.tab.h"
%}

%%
"<< " {
   return PRINT;
}

"<" {
   return ERROR_PRINT;
}

"!! " {
   return DELETE;
}

"!" {
   return ERROR_DELETE;
}

"//"[^\n]*\n {
   return COMMENT;
}

\$[a-zA-Z_][a-zA-Z0-9_]* {
   strcpy(yylval.vars.nameVar, yytext);
   yylval.vars.polinom = NULL;
   return VAR;
}

[[:digit:]]* {
   yylval.terms = (struct term_struct*)malloc(sizeof(struct term_struct));
   yylval.terms->coefficient = atoi(yytext);
   yylval.terms->symbol = '#';
   yylval.terms->degree = 1;
   return NUMBER;
}

[[:alpha:]] {
   yylval.terms = (struct term_struct*)malloc(sizeof(struct term_struct));
   yylval.terms->coefficient = 1;
   yylval.terms->symbol = yytext[0];
   yylval.terms->degree = 1;
   return SYMBOL;
}

[()-=+*^] {
   return yytext[0];
}

[\n] {
   yylineno++;
}
[\r\t];
[' '];
%%

int yywrap() {
 return 1;
}