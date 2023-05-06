%{
   #include <stdio.h>
   #include <stdlib.h>
   #include "term_struct.h"

   extern FILE* yyin;

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

      for (i = 0; i < sizeArray2; i++) {
         for (j = 0; j < sizeArray1; j++) {
            if (term2[i].degree == resultArray[j].degree) {
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

      for (i = 0; i < sizeArray1; i++) {
         for (j = 0; j < sizeArray2; j++) {
            int degree, coefficient;
            degree = term1[i].degree + term2[j].degree;

            if (degree == 0)
               temp[sizeTemp].symbol = '#';
            else
               temp[sizeTemp].symbol = term1[0].symbol;

            coefficient = term1[i].coefficient * term2[j].coefficient;
            temp[sizeTemp].coefficient = coefficient;
            temp[sizeTemp].degree = degree;

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
            if (temp[i].degree == temp[j].degree && i != j) {
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
      }

      free(temp);
      resultArray[sizeResult].symbol = '\0';

      return resultArray;
   }

   void printResult(struct term_struct* result, int sizeResult) {
      printf("Result: ");

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
            }
         }
         
         char sign = '\0';
         for (int i = 0; i < sizeResult; i++) {
            if (result[i].symbol != '#') {
               if (i != 0 && result[i].coefficient > 0)
                  sign = '+';

               printf("%c", sign);
               if (result[i].coefficient != 1) {
                  printf("%d", result[i].coefficient);
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
   
%}

%token NUMBER SYMBOL SIGN_DEGREE

%left '+' '-' 
%left '*' 
%left '(' ')'

%union {
   struct vars_struct* vars;
   struct term_struct* terms;
   char sign_degree;
}


%%

   result: A {
      int sizeOfArray = getSizeOfArrayStruct($<terms>$);
      printResult($<terms>$, sizeOfArray);
      return 0;
   };

   A: 
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
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray));
      } |
      '(' A ')' '^' C {
         int sizeOfArray = getSizeOfArrayStruct($<terms>2);
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray + 1));
         $<terms[0].degree>$ = $<terms[0].coefficient>5;
         int multi = $<terms[0].coefficient>$;
         for (int i = 1; i < $<terms[0].degree>$; i++) {
            multi *= $<terms[0].coefficient>$;
         }
         $<terms[0].coefficient>$ = multi;
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
         $<terms[0].degree>$ = $<terms[0].coefficient>3;
      } |
      base;

   base: 
      NUMBER SYMBOL {
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         $<terms[0].symbol>1 = ($<terms[0].symbol>2);
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
         $<terms[0].degree>$ = 1;
      } |
      NUMBER {
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));

      } |
      SYMBOL {
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
         int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
         int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
         $<terms>$ = multiple(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
      } |
      NUMBER {
         $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
      };
   // result: A {
   //    int sizeOfArray = getSizeOfArrayStruct($<terms>$);
   //    printResult($<terms>$, sizeOfArray);
   //    return 0;
   // };

   // A:
   //    A'+'A {
   //          int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
   //          int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
   //          $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
   //       } |

   //    A'-'A {
   //       int sizeOfArray1 = getSizeOfArrayStruct($<terms>1); // 3x^-3+2x^6+4x^3+5x^3-2x^3-2x^-3
   //       int sizeOfArray2 = getSizeOfArrayStruct($<terms>3); // 4x^2*4x   4x^2*4x^0   4x^2+3x*7x^3+14x^4  (4x^2+3x*7x^3+14x^4)*0
   //       $<terms>3 = changeSign(sizeOfArray2, $<terms>3);
   //       $<terms>$ = addition(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
   //    } |

   //    A'*'A {
   //       int sizeOfArray1 = getSizeOfArrayStruct($<terms>1);
   //       int sizeOfArray2 = getSizeOfArrayStruct($<terms>3);
   //       $<terms>$ = multiple(sizeOfArray1, sizeOfArray2, $<terms>1, $<terms>3);
   //    } |

   //    '-'A {
   //       int sizeOfArray = getSizeOfArrayStruct($<terms>2);
   //       $<terms>2 = changeSign(sizeOfArray, $<terms>2);
   //       $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
   //       memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray));

   //    } |

   //    '('A')' {
   //       int sizeOfArray = getSizeOfArrayStruct($<terms>2);
   //       $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
   //       memcpy($<terms>$, $<terms>2, sizeof(struct term_struct) * (sizeOfArray + 1));
   //    } |
   //    one_term;

   // one_term:
   //    NUMBER symbol_with_degree {
   //       $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct) * 2);
   //       memcpy(&$<terms[0]>$, $<terms>2, sizeof(struct term_struct));
   //       $<terms[0].coefficient>$ = $<terms->coefficient>1;
         
   //       $<terms[1].symbol>$ = '\0';
   //    }  |

   //    symbol_with_degree {
   //       $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct) * 2);
   //       memcpy(&$<terms[0]>$, $<terms>1, sizeof(struct term_struct)); 

   //       $<terms[1].symbol>$ = '\0';
   //    };

   // symbol_with_degree:
   //    SYMBOL SIGN_DEGREE NUMBER {
   //       int sign_number = 1;
         
   //       if ($<sign_degree>2 == '-')
   //          sign_number = -1;

   //       $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
   //       $<terms[0].degree>1 = ($<terms[0].coefficient>3 * sign_number);
   //       memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
   //       //free
   //    } |

   //    SYMBOL {
   //       $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
   //       memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
   //    } |

   //    NUMBER {
   //       $<terms>$ = (struct term_struct*)malloc(sizeof(struct term_struct));
   //       memcpy($<terms>$, $<terms>1, sizeof(struct term_struct));
   //       //free
   //    };
   

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

void yyerror() {
   printf("\nEntered data is invalid!\n\n");
}


