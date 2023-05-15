%{
   #include <stdio.h>
   #include <stdlib.h>
   #include "term_struct.h"

   extern FILE* yyin;
   extern int yylineno;

   char symbol_expr = '\0';
   int MAX_VALUE = 2147483647, MIN_VALUE = -2147483648; 

   struct list *begL = NULL;
   struct list *endL = NULL;
   int sizeList = 0;

   void checkOtherSymbols(char ch) {
      if (symbol_expr == '\0') {
         symbol_expr = ch;
      }
      else if (symbol_expr != ch) {
         yyerror("Syntax Error: Different symbols of unknowns are introduced in the expression!");
      }
   }

   int getSizeOfArrayStruct(struct term_struct* term) {
      int sizeArray = 0;
      
      while (term[sizeArray].symbol != '\0')
         sizeArray++;

      return sizeArray;
   }

   struct term_struct* addition(int sizeArray1, int sizeArray2, struct term_struct* term1, struct term_struct* term2) {
      struct term_struct* resultArray = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeArray1 + sizeArray2 + 1));
      int resultSize = sizeArray1, i, j, flag = 0;
      memcpy(resultArray, term1, sizeof(struct term_struct) * (sizeArray1));

      long long overflow;

      for (i = 0; i < sizeArray2; i++) {
         for (j = 0; j < sizeArray1; j++) {
            if ((term2[i].degree == resultArray[j].degree) && (term2[i].symbol == resultArray[j].symbol)) {
               overflow = resultArray[j].coefficient;
               if (overflow + term2[i].coefficient > MAX_VALUE || overflow + term2[i].coefficient < MIN_VALUE)
                  yyerror("Error: An integer type \"int\" has overflowed!");

               resultArray[j].coefficient += term2[i].coefficient;
               flag = 1;
            }
         }
         if (flag == 0) {
            memcpy(&resultArray[resultSize], &term2[i], sizeof(struct term_struct));
            resultSize++;
         }
         flag = 0;
      }

      resultArray[resultSize].symbol = '\0';

      return resultArray;
   }

   struct term_struct* changeSign(int sizeArray, struct term_struct* resultArray) {
      for (int i = 0; i < sizeArray; i++)
         resultArray[i].coefficient *= (-1);
      resultArray[sizeArray].symbol = '\0';

      return resultArray;
   }

   struct term_struct* multiple(int sizeArray1, int sizeArray2, struct term_struct* term1, struct term_struct* term2) {
      struct term_struct* temp = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeArray1 * sizeArray2));
      int sizeTemp = 0, i, j;
      long long overflow;

      for (i = 0; i < sizeArray1; i++) {
         for (j = 0; j < sizeArray2; j++) {
            int degree, coefficient;
            degree = term1[i].degree + term2[j].degree;
            if (degree == 0)
               temp[sizeTemp].symbol = '#';
            else {
               if ((term1[i].symbol != '#') || (term2[j].symbol != '#'))
                  temp[sizeTemp].symbol = symbol_expr;
               else
                  temp[sizeTemp].symbol = '#';
            }


            overflow = term1[i].coefficient;
            if (overflow * term2[j].coefficient > MAX_VALUE || overflow * term2[j].coefficient < MIN_VALUE)
               yyerror("Error: An integer type \"int\" has overflowed!");
            
            coefficient = term1[i].coefficient * term2[j].coefficient;
            temp[sizeTemp].coefficient = coefficient;
            if (term1[i].symbol == term2[j].symbol && term1[i].symbol != '#')
               temp[sizeTemp].degree = degree;
            else {
               if (term1[i].symbol != '#') {
                  temp[sizeTemp].degree = term1[i].degree;
               }
               else if (term2[j].symbol != '#') {
                  temp[sizeTemp].degree = term2[j].degree;
               }
               else {
                  temp[sizeTemp].degree = 1;
               }
            }
               
            sizeTemp++;
         }
      }

      struct term_struct* resultArray = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeArray1 * sizeArray2 + 1));
      int sizeResult = 0, sum = 0;

      for (i = 0; i < sizeTemp; i++) {

         sum = temp[i].coefficient;
         if (sum == 0)
            continue;

         for (j = 1; j < sizeTemp; j++) {
            if ((temp[i].degree == temp[j].degree) && (i != j) && (temp[i].symbol == temp[j].symbol)) {
               sum += temp[j].coefficient;
               temp[j].coefficient = 0;
            }
         }

         if (sum != 0) {
            resultArray[sizeResult].coefficient = sum;
            resultArray[sizeResult].degree = temp[i].degree;
            resultArray[sizeResult].symbol = temp[i].symbol;

            sizeResult++;
         }
         sum = 0;
      }

      free(temp);
      resultArray[sizeResult].symbol = '\0';

      return resultArray;
   }

   void printResult(struct term_struct* result, int sizeResult) {

      if (sizeResult == 0) {
         printf("0");
      }
      else {
         for (int i = sizeResult - 1; i > 0; i--) {
            for (int j = 0; j < i; j++) {
               if (result[j].degree < result[j + 1].degree) {
                  struct term_struct temp[1];

                  temp[0].coefficient = result[j].coefficient;
                  temp[0].degree = result[j].degree;
                  temp[0].symbol = result[j].symbol;

                  result[j].coefficient = result[j + 1].coefficient; 
                  result[j].degree = result[j + 1].degree; 
                  result[j].symbol = result[j + 1].symbol; 

                  result[j + 1].coefficient = temp[0].coefficient;
                  result[j + 1].degree = temp[0].degree;
                  result[j + 1].symbol = temp[0].symbol;
               }
               else if ((result[j].degree == result[j + 1].degree) && ((result[j].symbol < result[j + 1].symbol))) {
                  struct term_struct temp[1];

                  temp[0].coefficient = result[j].coefficient;
                  temp[0].degree = result[j].degree;
                  temp[0].symbol = result[j].symbol;

                  result[j].coefficient = result[j + 1].coefficient; 
                  result[j].degree = result[j + 1].degree; 
                  result[j].symbol = result[j + 1].symbol; 

                  result[j + 1].coefficient = temp[0].coefficient;
                  result[j + 1].degree = temp[0].degree;
                  result[j + 1].symbol = temp[0].symbol;
               }
            }
         }
         
         char sign = '\0';
         for (int i = 0; i < sizeResult; i++) {
            if (result[i].symbol != '#') {
               int coeff = result[i].coefficient;
               if (i != 0 && coeff > 0)
                  sign = '+';
               else if (coeff < 0) {
                  sign = '-';
                  coeff *= -1;
               }

               printf("%c", sign);
               if (coeff != 1) {
                  printf("%d", coeff);
               }
               printf("%c", result[i].symbol);

               if (result[i].degree != 1) {
                  printf("%c%d", '^', result[i].degree);
               }
               
               sign = '\0';
            }
            else {
               if (i != 0 && result[i].coefficient > 0)
                  printf("+", sign);
               printf("%d", result[i].coefficient);
            }
         }
      }
      printf("\n");
   }

   int addElemToEndList(struct term_struct* result, int sizeResult, const char* name) {
      struct list *newElem = (struct list*)malloc(sizeof(struct list));
      endL->next = newElem;
      endL = newElem;
      newElem->next = NULL;

      newElem->var.polinom = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeResult + 1));
      memcpy(newElem->var.polinom, result, sizeof(struct term_struct) * (sizeResult + 1)); 
      strcpy(newElem->var.nameVar, name);
      sizeList++;
   }

   int addFirstElemToList(struct term_struct* result, int sizeResult, const char* name) {
      begL = (struct list*)malloc(sizeof(struct list));
      endL = begL;
      endL->next = NULL;

      begL->var.polinom = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeResult + 1));
      memcpy(begL->var.polinom, result, sizeof(struct term_struct) * (sizeResult + 1)); 
      strcpy(begL->var.nameVar, name);
      sizeList++;
   }

   int changeElem(struct term_struct* result, int sizeResult, const char* name) {
      struct list *scan = begL;
      while (scan != NULL) {
         if (strcmp(scan->var.nameVar, name) == 0) {
            struct term_struct* tmp = scan->var.polinom; 

            scan->var.polinom = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeResult + 1));
            memcpy(scan->var.polinom, result, sizeof(struct term_struct) * (sizeResult + 1));
            free(tmp);

            return 1;
         }
         scan = scan->next;
      }
      return 0;
   }

   struct list *checkElem(const char* name) {
      struct list *scan = begL;
      while (scan != NULL) {
         if (strcmp(scan->var.nameVar, name) == 0) {
            return scan;
         }
         scan = scan->next;
      }
      return NULL;
   } 

   void addVarToList(struct term_struct* result, int sizeResult, const char* name) {
      if (begL != NULL) {
         addElemToEndList(result, sizeResult, name);
      }
      else {
         addFirstElemToList(result, sizeResult, name);
      }
   }

   int deleteElemFromList(const char* name) {
      struct list *scan = begL;
      struct list *prevNode = begL;

      while (scan != NULL) {
         if (strcmp(scan->var.nameVar, name) == 0) {
            prevNode->next = scan->next;
            scan->next = NULL;
            free(scan->var.polinom);
            free(scan);
            return 1;
         }
         prevNode = scan;
         scan = scan->next;
      }
      return 0;
   }
   
%}

%token NUMBER SYMBOL PRINT COMMENT VAR DELETE ERROR_PRINT ERROR_DELETE ERROR_COMMENT

%left '+' '-' 
%left '*' 
%left '^'
%left '(' ')'

%union {
   struct vars_struct vars;
   struct term_struct* terms;
}


%%
   main:
      | main start
      | main COMMENT start {

      }
      | main ERROR_COMMENT start {
         yyerror("Error Parse: Wrong command \"//\"!");
      };
   start:
      ERROR_PRINT VAR {
         yyerror("Error Parse: Wrong command \"<<\"!");
      } |
      ERROR_DELETE VAR {
         yyerror("Error Parse: Wrong command \"!!\"!"); 
      } |
      VAR '=' expression {
         int sizeOfArray = getSizeOfArrayStruct($<terms>3);
         if (changeElem($<terms>3, sizeOfArray, $<vars.nameVar>1) == 0) {
            addVarToList($<terms>3, sizeOfArray, $<vars.nameVar>1);
         }
      } |
      PRINT VAR {
         struct list *result = checkElem($<vars.nameVar>2);
         if (result != NULL) {
            printf("%s = ", result->var.nameVar);
            int sizeOfArray = getSizeOfArrayStruct(result->var.polinom);
            printResult(result->var.polinom, sizeOfArray);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      DELETE VAR {
         struct list *result = checkElem($<vars.nameVar>2);
         if (result != NULL) {
            if (deleteElemFromList($<vars.nameVar>2) == 0) {
               yyerror("Error: Delete non-increased variable!");
            }
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      };

   expression:
      variable {
         int sizeOfArray = getSizeOfArrayStruct($<vars.polinom>1);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<vars.polinom>1, sizeof(struct term_struct) * (sizeOfArray + 1));
      } |
      A {
         int sizeOfArray = getSizeOfArrayStruct($<terms>1);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct) * (sizeOfArray + 1));
      } |
      variable '+' A {
         struct list *result = checkElem($<vars.nameVar>1);
         if (result != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
            $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      A '+' variable {
         struct list *result = checkElem($<vars.nameVar>3);
         if (result != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>3);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>1);
            $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>3, $<terms>1);
         }
         else {
            yyerror("Error: Non-increased variable!");
         } 
      } |
      variable '-' A {
         struct list *result = checkElem($<vars.nameVar>1);
         if (result != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
            $<terms>3 = changeSign(sizeOfArray2, $<terms>3);
            $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      A '-' variable {
         struct list *result = checkElem($<vars.nameVar>3);
         if (result != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>3);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>1);
            $<terms>3 = changeSign(sizeOfArray2, $<terms>3);
            $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>3, $<terms>1);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      variable '*' A {
         struct list *result = checkElem($<vars.nameVar>1);
         if (result != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
            $<terms>$ = multiple(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      A '*' variable {
         struct list *result = checkElem($<vars.nameVar>3);
         if (result != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>3);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>1);
            $<terms>$ = multiple(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      };

   variable:
      variable '+' variable {
         struct list *result1 = checkElem($<vars.nameVar>1);
         struct list *result2 = checkElem($<vars.nameVar>3);
         if (result1 != NULL && result2 != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
            $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      variable '-' variable {
         struct list *result1 = checkElem($<vars.nameVar>1);
         struct list *result2 = checkElem($<vars.nameVar>3);
         if (result1 != NULL && result2 != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
            $<terms>3 = changeSign(sizeOfArray2, $<terms>3);
            $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      '-' variable {
         struct list *result = checkElem($<vars.nameVar>2);
         if (result != NULL) {
            int sizeOfArray = getSizeOfArrayStruct($<terms>2);
            $<terms>$ = changeSign(sizeOfArray, $<terms>2);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      variable '*' variable {
         struct list *result1 = checkElem($<vars.nameVar>1);
         struct list *result2 = checkElem($<vars.nameVar>3);
         if (result1 != NULL && result2 != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
            int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
            $<terms>$ = multiple(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      variable '^' C {
         struct list *result = checkElem($<vars.nameVar>2);
         if (result != NULL) {
            int sizeOfArray = getSizeOfArrayStruct($<terms>1);
            int degree = $<terms[0].coefficient>3;
            if (degree > 1) {
               struct term_struct* tmp = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
               memcpy(tmp, $<terms>1, sizeof(struct term_struct) * (sizeOfArray));
               int sizeOfArray2 = sizeOfArray;
               for (int k = 0; k < degree - 1; k++) {
                  struct term_struct* fr = $<terms>1;
                  $<terms>1 = multiple(sizeOfArray2, sizeOfArray, $<terms>1, tmp);
                  sizeOfArray2 = getSizeOfArrayStruct($<terms>1);
                  if (k < degree - 1)
                     free(fr);
               }
               sizeOfArray = sizeOfArray2;
            }
            else if (degree == 0) {
               sizeOfArray = 1;
               struct term_struct* fr = $<terms>1;
               $<terms>1 = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
               free(fr);

               $<terms[0].coefficient>1 = 1;
               $<terms[0].symbol>1 = '#';
               $<terms[0].degree>1 = 1;
            }
            $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
            memcpy($<terms>$, $<terms>1, sizeof(struct term_struct) * (sizeOfArray + 1));
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      } |
      '(' variable ')' {
         $<vars>$ = $<vars>2;
      } |
      VAR {
         struct list *result = checkElem($<vars.nameVar>1);
         if (result != NULL) {
            $<terms>$ = result->var.polinom;
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      };

   A: 
      '+' '+' { yyerror("Syntax Error: Two or more identical operation symbols entered!"); } |
      '-' '-' { yyerror("Syntax Error: Two or more identical operation symbols entered!"); } |
      '*' '*' { yyerror("Syntax Error: Two or more identical operation symbols entered!"); } |
      '^' '^' { yyerror("Syntax Error: Two or more identical operation symbols entered!"); } |


      A '+' A {
         int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
         int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
         $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
      } |
      A '-' A {
         int sizeOfArray1 = getSizeOfArrayStruct($<terms>1); // 3x^-3+2x^6+4x^3+5x^3-2x^3-2x^-3
         int sizeOfArray2 = getSizeOfArrayStruct($<terms>3); // 4x^2*4x   4x^2*4x^0   4x^2+3x*7x^3+14x^4  (4x^2+3x*7x^3+14x^4)*0
         $<terms>3 = changeSign(sizeOfArray2, $<terms>3);
         $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
      } |
      A '*' A {
         int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
         int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
         $<terms>$ = multiple(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
      } |
      '-' A {
         int sizeOfArray = getSizeOfArrayStruct($<terms>2);
         $<terms>2 = changeSign(sizeOfArray, $<terms>2);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray + 1));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray + 1));
      } |
      '(' A ')' '^' C {
         int sizeOfArray = getSizeOfArrayStruct($<terms>2);
         int degree = $<terms[0].coefficient>5;
         if (degree > 1) {
            struct term_struct* tmp = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
            memcpy(tmp, $<terms>2, sizeof(struct term_struct) * (sizeOfArray));
            int sizeOfArray2 = sizeOfArray;
            for (int k = 0; k < degree - 1; k++) {
               struct term_struct* fr = $<terms>2;
               $<terms>2 = multiple(sizeOfArray2, sizeOfArray, $<terms>2, tmp);
               sizeOfArray2 = getSizeOfArrayStruct($<terms>2);
               if (k < degree - 1)
                  free(fr);
            }
            sizeOfArray = sizeOfArray2;
         }
         else if (degree == 0) {
            sizeOfArray = 1;
            struct term_struct* fr = $<terms>2;
            $<terms>2 = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
            free(fr);
            $<terms[0].coefficient>2 = 1;
            $<terms[0].symbol>2 = '#';
            $<terms[0].degree>2 = 1;
         }
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray + 1));
      } |
      '(' A ')' {
         int sizeOfArray = getSizeOfArrayStruct($<terms>2);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray + 1));
      } |
      one_term;

   one_term: 
      base '^' C {
         int sizeOfArray = getSizeOfArrayStruct($<terms>1);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct) * (sizeOfArray + 1));
         if ($<terms[0].coefficient>3 == 0) {
            $<terms[0].coefficient>$ = 1;
         }
         else {
            long long overflow;

            $<terms[0].degree>$ = $<terms[0].coefficient>3;
            int num = $<terms[0].coefficient>$;
            if ((num > 1) && ($<terms[0].symbol>$ == '#')) {
               for (int i = 0; i < $<terms[0].degree>$ - 1; i++) {
                  overflow = $<terms[0].coefficient>$;
                  if (overflow * num > MAX_VALUE || overflow * num < MIN_VALUE)
                     yyerror("Error: An integer type \"int\" has overflowed!");
                  $<terms[0].coefficient>$ *= num;
               }
               $<terms[0].degree>$ = 1;
            }
         }
      } |
      base;

   base:
      SYMBOL SYMBOL { yyerror("Syntax Error: Two or more unknown variables entered without delimiter operations!"); } | 
      NUMBER SYMBOL {
         checkOtherSymbols($<terms[0].symbol>2);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         $<terms[0].symbol>1 = ($<terms[0].symbol>2);
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
      } |
      NUMBER {
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
      } |
      SYMBOL {
         checkOtherSymbols($<terms[0].symbol>1);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
      }

   C:
      '(' D ')' {
         int sizeOfArray = getSizeOfArrayStruct($<terms>2);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray + 1));
      } |
      NUMBER {
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
      } |
      '-' NUMBER {
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct));
         $<terms[0].coefficient>$ *= -1;
      };

   D:
      D '+' D {
         int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
         int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
         $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
      } |
      D '-' D {
         int sizeOfArray1 = getSizeOfArrayStruct($<terms>1); 
         int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
         $<terms>3 = changeSign(sizeOfArray2, $<terms>3);
         $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
      } |
      D '*' D {
         int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
         int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
         $<terms>$ = multiple(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
      } |
      '-' D {
         int sizeOfArray = getSizeOfArrayStruct($<terms>2);
         $<terms>2 = changeSign(sizeOfArray, $<terms>2);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray));
      } |
      '(' D ')' {
         int sizeOfArray = getSizeOfArrayStruct($<terms>2);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray + 1));
      } |
      D '^' D {
         int sizeOfArray = getSizeOfArrayStruct($<terms>1);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray + 1));
         int tmp = $<terms[0].coefficient>$;
         for (int i = 1; i < $<terms[0].coefficient>3; i++) {
            $<terms[0].coefficient>$ *= tmp;
         }
      } |
      NUMBER {
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
      };
%%

int main(int argc, void *argv[]) {

   // if (argc != 2) {
   //    printf("Wrong number of parameters!\n");
   //    return 1;
   // }

   printf("A program that works with polynomials.\nSupported operations: addition, subtraction and multiplication of polynomials.\n");
   yyin = fopen("input.txt", "r");

   if (yyin == NULL) {
		printf("File doesn`t exist. Create file \"input.txt\"\n");
		return 1;
	}

	yyparse();
	fclose(yyin);
		
   return 0;
}

void yyerror(const char* messageAboutError) {
   if (strlen(messageAboutError) > 0) {
      if (!strcmp(messageAboutError, "syntax error")) {
         printf("\nSyntax Error: Entered data is invalid! Line: %d.\n\n", yylineno);
      }
      else {
         printf("\n%s Line: %d.\n\n", messageAboutError, yylineno);
      }
   }
   else {
      printf("\nSyntax Error: Entered data is invalid! Line: %d.\n\n", yylineno);
   }

   exit(1);
}


