/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "lab1.y"

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
               if ((term1[i].symbol != '#') || (term1[j].symbol != '#'))
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

      newElem->var.polinom = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeResult));
      memcpy(newElem->var.polinom, result, sizeof(struct term_struct) * (sizeResult)); 
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

            scan->var.polinom = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeResult));
            memcpy(scan->var.polinom, result, sizeof(struct term_struct) * (sizeResult));
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
   

#line 370 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    SYMBOL = 259,                  /* SYMBOL  */
    PRINT = 260,                   /* PRINT  */
    COMMENT = 261,                 /* COMMENT  */
    VAR = 262,                     /* VAR  */
    DELETE = 263                   /* DELETE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define NUMBER 258
#define SYMBOL 259
#define PRINT 260
#define COMMENT 261
#define VAR 262
#define DELETE 263

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 307 "lab1.y"

   struct vars_struct* vars;
   struct term_struct* terms;

#line 444 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_SYMBOL = 4,                     /* SYMBOL  */
  YYSYMBOL_PRINT = 5,                      /* PRINT  */
  YYSYMBOL_COMMENT = 6,                    /* COMMENT  */
  YYSYMBOL_VAR = 7,                        /* VAR  */
  YYSYMBOL_DELETE = 8,                     /* DELETE  */
  YYSYMBOL_9_ = 9,                         /* '+'  */
  YYSYMBOL_10_ = 10,                       /* '-'  */
  YYSYMBOL_11_ = 11,                       /* '*'  */
  YYSYMBOL_12_ = 12,                       /* '^'  */
  YYSYMBOL_13_ = 13,                       /* '('  */
  YYSYMBOL_14_ = 14,                       /* ')'  */
  YYSYMBOL_15_ = 15,                       /* '='  */
  YYSYMBOL_YYACCEPT = 16,                  /* $accept  */
  YYSYMBOL_main = 17,                      /* main  */
  YYSYMBOL_start = 18,                     /* start  */
  YYSYMBOL_expression = 19,                /* expression  */
  YYSYMBOL_variable = 20,                  /* variable  */
  YYSYMBOL_A = 21,                         /* A  */
  YYSYMBOL_one_term = 22,                  /* one_term  */
  YYSYMBOL_base = 23,                      /* base  */
  YYSYMBOL_C = 24,                         /* C  */
  YYSYMBOL_D = 25                          /* D  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   126

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  16
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  42
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  78

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   263


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      13,    14,    11,     9,     2,    10,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    15,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    12,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   314,   314,   315,   317,   323,   331,   344,   349,   356,
     373,   391,   401,   418,   452,   455,   458,   459,   460,   461,
     464,   469,   475,   480,   486,   514,   519,   522,   545,   548,
     549,   555,   559,   566,   571,   575,   582,   587,   593,   598,
     604,   609,   618
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "SYMBOL",
  "PRINT", "COMMENT", "VAR", "DELETE", "'+'", "'-'", "'*'", "'^'", "'('",
  "')'", "'='", "$accept", "main", "start", "expression", "variable", "A",
  "one_term", "base", "C", "D", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-39)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -39,    93,   -39,     5,     7,    12,   -39,   -39,     4,   -39,
      19,    21,   -39,    49,    29,    48,    55,     4,   -39,    -6,
      37,   -39,    56,   -39,   -39,   -39,    62,    23,    61,   -39,
     -39,    94,   112,    11,    11,    11,    73,    40,    40,    40,
      73,   -39,    72,    11,    11,    23,    23,    75,   -39,    85,
      82,   -39,    51,    40,    61,    61,   -39,   -39,    73,   -39,
     -39,    82,    82,   100,    78,   -39,    45,   106,    82,    82,
      82,    82,   -39,   -39,    45,    45,    84,   -39
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     3,     5,     0,     6,
      31,    32,    15,     0,     0,     0,     0,     0,     4,     7,
       8,    26,    28,    30,    29,    16,    17,    11,    23,    18,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    25,     0,     0,     9,    10,    12,    34,     0,
       0,    13,     0,     0,    20,    21,    22,    27,     0,    35,
      42,     0,     0,     0,    17,    24,    39,     0,     0,     0,
       0,     0,    33,    40,    36,    37,    38,    41
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -39,   -39,   -39,   -39,    -7,    -8,   -39,   -39,   -38,     9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     6,    18,    27,    28,    21,    22,    51,    63
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      20,    19,    57,    33,    34,    35,    36,    10,    11,    32,
      31,    12,     7,    13,    14,    15,    16,    17,    12,     9,
      65,    43,     8,    23,    44,    24,    45,    46,    47,    54,
      55,    56,    10,    11,    35,    36,    12,    31,    13,    26,
      15,    16,    17,    10,    11,    32,    37,    38,    39,    13,
      52,    15,    16,    53,    10,    11,    70,    71,    25,    29,
      13,    64,    15,    16,    53,    10,    11,    30,    40,    12,
      66,    67,    39,    15,    16,    17,    48,    74,    75,    76,
      77,    10,    11,    49,    58,    60,    50,    36,    59,    15,
      16,    53,    61,     2,     0,    62,    71,     0,     3,     0,
       4,     5,     0,    33,    34,    35,    36,     0,    41,    68,
      69,    70,    71,     0,    72,    68,    69,    70,    71,     0,
      73,    37,    38,    39,     0,     0,    42
};

static const yytype_int8 yycheck[] =
{
       8,     8,    40,     9,    10,    11,    12,     3,     4,    17,
      17,     7,     7,     9,    10,    11,    12,    13,     7,     7,
      58,    10,    15,     4,    13,     4,    33,    34,    35,    37,
      38,    39,     3,     4,    11,    12,     7,    44,     9,    10,
      11,    12,    13,     3,     4,    53,     9,    10,    11,     9,
      10,    11,    12,    13,     3,     4,    11,    12,     9,    11,
       9,    10,    11,    12,    13,     3,     4,    12,    12,     7,
      61,    62,    11,    11,    12,    13,     3,    68,    69,    70,
      71,     3,     4,    10,    12,     3,    13,    12,     3,    11,
      12,    13,    10,     0,    -1,    13,    12,    -1,     5,    -1,
       7,     8,    -1,     9,    10,    11,    12,    -1,    14,     9,
      10,    11,    12,    -1,    14,     9,    10,    11,    12,    -1,
      14,     9,    10,    11,    -1,    -1,    14
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    17,     0,     5,     7,     8,    18,     7,    15,     7,
       3,     4,     7,     9,    10,    11,    12,    13,    19,    20,
      21,    22,    23,     4,     4,     9,    10,    20,    21,    11,
      12,    20,    21,     9,    10,    11,    12,     9,    10,    11,
      12,    14,    14,    10,    13,    20,    20,    20,     3,    10,
      13,    24,    10,    13,    21,    21,    21,    24,    12,     3,
       3,    10,    13,    25,    10,    24,    25,    25,     9,    10,
      11,    12,    14,    14,    25,    25,    25,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    16,    17,    17,    18,    18,    18,    19,    19,    20,
      20,    20,    20,    20,    20,    20,    21,    21,    21,    21,
      21,    21,    21,    21,    21,    21,    21,    22,    22,    23,
      23,    23,    23,    24,    24,    24,    25,    25,    25,    25,
      25,    25,    25
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     3,     2,     2,     1,     1,     3,
       3,     2,     3,     3,     3,     1,     2,     2,     2,     2,
       3,     3,     3,     2,     5,     3,     1,     3,     1,     2,
       2,     1,     1,     3,     1,     2,     3,     3,     3,     2,
       3,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* start: VAR '=' expression  */
#line 317 "lab1.y"
                         {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[0].terms));
         if (changeElem((yyvsp[0].terms), sizeOfArray, (yyvsp[-2].vars->nameVar)) == 0) {
            addVarToList((yyvsp[0].terms), sizeOfArray, (yyvsp[-2].vars->nameVar));
         }
      }
#line 1499 "y.tab.c"
    break;

  case 5: /* start: PRINT VAR  */
#line 323 "lab1.y"
                {
         struct list *result = checkElem((yyvsp[0].vars->nameVar));
         if (result != NULL) {
            printf("%s = ", result->var.nameVar);
            int sizeOfArray = getSizeOfArrayStruct(result->var.polinom);
            printResult(result->var.polinom, sizeOfArray);
         }
      }
#line 1512 "y.tab.c"
    break;

  case 6: /* start: DELETE VAR  */
#line 331 "lab1.y"
                 {
         struct list *result = checkElem((yyvsp[0].vars->nameVar));
         if (result != NULL) {
            if (deleteElemFromList((yyvsp[0].vars->nameVar)) == 0) {
               yyerror("Error: Delete non-increased variable!");
            }
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      }
#line 1528 "y.tab.c"
    break;

  case 7: /* expression: variable  */
#line 344 "lab1.y"
               {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[0].vars->polinom));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray));
         memcpy((yyval.terms), (yyvsp[0].vars->polinom), sizeof(struct term_struct) * (sizeOfArray));
      }
#line 1538 "y.tab.c"
    break;

  case 8: /* expression: A  */
#line 349 "lab1.y"
        {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[0].terms));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray));
         memcpy((yyval.terms), (yyvsp[0].terms), sizeof(struct term_struct) * (sizeOfArray));
      }
#line 1548 "y.tab.c"
    break;

  case 9: /* variable: variable '+' variable  */
#line 356 "lab1.y"
                            {
         struct list *result1 = checkElem((yyvsp[-2].vars->nameVar));
         struct list *result2 = checkElem((yyvsp[0].vars->nameVar));
         if (result1 != NULL && result2 != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct(result1->var.polinom);
            int sizeOfArray2 = getSizeOfArrayStruct(result2->var.polinom);
            (yyval.vars) = (struct vars_struct*)malloc(sizeof(struct vars_struct));
            (yyval.vars->polinom) = addition(sizeOfArray1, sizeOfArray2, result1->var.polinom, result2->var.polinom);
            free((yyvsp[-2].vars->polinom));
            free((yyvsp[0].vars->polinom));
            free((yyvsp[-2].vars));
            free((yyvsp[0].vars));
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      }
#line 1570 "y.tab.c"
    break;

  case 10: /* variable: variable '-' variable  */
#line 373 "lab1.y"
                            {
         struct list *result1 = checkElem((yyvsp[-2].vars->nameVar));
         struct list *result2 = checkElem((yyvsp[0].vars->nameVar));
         if (result1 != NULL && result2 != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct(result1->var.polinom);
            int sizeOfArray2 = getSizeOfArrayStruct(result2->var.polinom);
            (yyvsp[0].vars->polinom) = changeSign(sizeOfArray2, (yyvsp[0].vars->polinom));
            (yyval.vars) = (struct vars_struct*)malloc(sizeof(struct vars_struct));
            (yyval.vars->polinom) = addition(sizeOfArray1, sizeOfArray2, result1->var.polinom, result2->var.polinom);
            free((yyvsp[-2].vars->polinom));
            free((yyvsp[0].vars->polinom));
            free((yyvsp[-2].vars));
            free((yyvsp[0].vars));
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      }
#line 1593 "y.tab.c"
    break;

  case 11: /* variable: '-' variable  */
#line 391 "lab1.y"
                   {
         struct list *result = checkElem((yyvsp[0].vars->nameVar));
         if (result != NULL) {
            int sizeOfArray = getSizeOfArrayStruct(result->var.polinom);
            (yyval.vars->polinom) = changeSign(sizeOfArray, result->var.polinom);
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      }
#line 1608 "y.tab.c"
    break;

  case 12: /* variable: variable '*' variable  */
#line 401 "lab1.y"
                            {
         struct list *result1 = checkElem((yyvsp[-2].vars->nameVar));
         struct list *result2 = checkElem((yyvsp[0].vars->nameVar));
         if (result1 != NULL && result2 != NULL) {
            int sizeOfArray1 = getSizeOfArrayStruct(result1->var.polinom);
            int sizeOfArray2 = getSizeOfArrayStruct(result2->var.polinom);
            (yyval.vars) = (struct vars_struct*)malloc(sizeof(struct vars_struct));
            (yyval.vars->polinom) = multiple(sizeOfArray1, sizeOfArray2, result1->var.polinom, result2->var.polinom);
            free((yyvsp[-2].vars->polinom));
            free((yyvsp[0].vars->polinom));
            free((yyvsp[-2].vars));
            free((yyvsp[0].vars));
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      }
#line 1630 "y.tab.c"
    break;

  case 13: /* variable: variable '^' C  */
#line 418 "lab1.y"
                     {
         struct list *result = checkElem((yyvsp[-1].vars->nameVar));
         if (result != NULL) {
            int sizeOfArray = getSizeOfArrayStruct(result->var.polinom);
            int degree = (yyvsp[0].terms[0].coefficient);
            if (degree > 1) {
               struct term_struct* tmp = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
               memcpy(tmp, result->var.polinom, sizeof(struct term_struct) * (sizeOfArray));
               int sizeOfArray2 = sizeOfArray;
               for (int k = 0; k < degree - 1; k++) {
                  struct term_struct* fr = result->var.polinom;
                  result->var.polinom = multiple(sizeOfArray2, sizeOfArray, result->var.polinom, tmp);
                  sizeOfArray2 = getSizeOfArrayStruct(result->var.polinom);
                  if (k < degree - 1)
                     free(fr);
               }
               sizeOfArray = sizeOfArray2;
            }
            else if (degree == 0) {
               sizeOfArray = 1;
               struct term_struct* fr = result->var.polinom;
               result->var.polinom = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
               free(fr);
               (yyvsp[-1].terms[0].coefficient) = 1;
               (yyvsp[-1].terms[0].symbol) = '#';
               (yyvsp[-1].terms[0].degree) = 1;
            }
            (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
            memcpy((yyval.terms), result->var.polinom, sizeof(struct term_struct) * (sizeOfArray + 1));
         }
         else {
            yyerror("Error: Non-increased variable!");
         }
      }
#line 1669 "y.tab.c"
    break;

  case 14: /* variable: '(' variable ')'  */
#line 452 "lab1.y"
                       {
         (yyval.vars) = (yyvsp[-1].vars);
      }
#line 1677 "y.tab.c"
    break;

  case 16: /* A: '+' '+'  */
#line 458 "lab1.y"
              { yyerror("Syntax Error: Two or more identical operation symbols entered!"); }
#line 1683 "y.tab.c"
    break;

  case 17: /* A: '-' '-'  */
#line 459 "lab1.y"
              { yyerror("Syntax Error: Two or more identical operation symbols entered!"); }
#line 1689 "y.tab.c"
    break;

  case 18: /* A: '*' '*'  */
#line 460 "lab1.y"
              { yyerror("Syntax Error: Two or more identical operation symbols entered!"); }
#line 1695 "y.tab.c"
    break;

  case 19: /* A: '^' '^'  */
#line 461 "lab1.y"
              { yyerror("Syntax Error: Two or more identical operation symbols entered!"); }
#line 1701 "y.tab.c"
    break;

  case 20: /* A: A '+' A  */
#line 464 "lab1.y"
              {
         int sizeOfArray1 = getSizeOfArrayStruct((yyvsp[-2].terms));
         int sizeOfArray2 = getSizeOfArrayStruct((yyvsp[0].terms));
         (yyval.terms) = addition(sizeOfArray1, sizeOfArray2, (yyvsp[-2].terms), (yyvsp[0].terms));
      }
#line 1711 "y.tab.c"
    break;

  case 21: /* A: A '-' A  */
#line 469 "lab1.y"
              {
         int sizeOfArray1 = getSizeOfArrayStruct((yyvsp[-2].terms)); // 3x^-3+2x^6+4x^3+5x^3-2x^3-2x^-3
         int sizeOfArray2 = getSizeOfArrayStruct((yyvsp[0].terms)); // 4x^2*4x   4x^2*4x^0   4x^2+3x*7x^3+14x^4  (4x^2+3x*7x^3+14x^4)*0
         (yyvsp[0].terms) = changeSign(sizeOfArray2, (yyvsp[0].terms));
         (yyval.terms) = addition(sizeOfArray1, sizeOfArray2, (yyvsp[-2].terms), (yyvsp[0].terms));
      }
#line 1722 "y.tab.c"
    break;

  case 22: /* A: A '*' A  */
#line 475 "lab1.y"
              {
         int sizeOfArray1 = getSizeOfArrayStruct((yyvsp[-2].terms));
         int sizeOfArray2 = getSizeOfArrayStruct((yyvsp[0].terms));
         (yyval.terms) = multiple(sizeOfArray1, sizeOfArray2, (yyvsp[-2].terms), (yyvsp[0].terms));
      }
#line 1732 "y.tab.c"
    break;

  case 23: /* A: '-' A  */
#line 480 "lab1.y"
            {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[0].terms));
         (yyvsp[0].terms) = changeSign(sizeOfArray, (yyvsp[0].terms));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray + 1));
         memcpy((yyval.terms), (yyvsp[0].terms), sizeof(struct term_struct) * (sizeOfArray + 1));
      }
#line 1743 "y.tab.c"
    break;

  case 24: /* A: '(' A ')' '^' C  */
#line 486 "lab1.y"
                      {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[-3].terms));
         int degree = (yyvsp[0].terms[0].coefficient);
         if (degree > 1) {
            struct term_struct* tmp = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
            memcpy(tmp, (yyvsp[-3].terms), sizeof(struct term_struct) * (sizeOfArray));
            int sizeOfArray2 = sizeOfArray;
            for (int k = 0; k < degree - 1; k++) {
               struct term_struct* fr = (yyvsp[-3].terms);
               (yyvsp[-3].terms) = multiple(sizeOfArray2, sizeOfArray, (yyvsp[-3].terms), tmp);
               sizeOfArray2 = getSizeOfArrayStruct((yyvsp[-3].terms));
               if (k < degree - 1)
                  free(fr);
            }
            sizeOfArray = sizeOfArray2;
         }
         else if (degree == 0) {
            sizeOfArray = 1;
            struct term_struct* fr = (yyvsp[-3].terms);
            (yyvsp[-3].terms) = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
            free(fr);
            (yyvsp[-3].terms[0].coefficient) = 1;
            (yyvsp[-3].terms[0].symbol) = '#';
            (yyvsp[-3].terms[0].degree) = 1;
         }
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy((yyval.terms), (yyvsp[-3].terms), sizeof(struct term_struct) * (sizeOfArray + 1));
      }
#line 1776 "y.tab.c"
    break;

  case 25: /* A: '(' A ')'  */
#line 514 "lab1.y"
                {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[-1].terms));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy((yyval.terms), (yyvsp[-1].terms), sizeof(struct term_struct) * (sizeOfArray + 1));
      }
#line 1786 "y.tab.c"
    break;

  case 27: /* one_term: base '^' C  */
#line 522 "lab1.y"
                 {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[-2].terms));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy((yyval.terms), (yyvsp[-2].terms), sizeof(struct term_struct) * (sizeOfArray + 1));
         if ((yyvsp[0].terms[0].coefficient) == 0) {
            (yyval.terms[0].coefficient) = 1;
         }
         else {
            long long overflow;

            (yyval.terms[0].degree) = (yyvsp[0].terms[0].coefficient);
            int num = (yyval.terms[0].coefficient);
            if ((num > 1) && ((yyval.terms[0].symbol) == '#')) {
               for (int i = 0; i < (yyval.terms[0].degree) - 1; i++) {
                  overflow = (yyval.terms[0].coefficient);
                  if (overflow * num > MAX_VALUE || overflow * num < MIN_VALUE)
                     yyerror("Error: An integer type \"int\" has overflowed!");
                  (yyval.terms[0].coefficient) *= num;
               }
               (yyval.terms[0].degree) = 1;
            }
         }
      }
#line 1814 "y.tab.c"
    break;

  case 29: /* base: SYMBOL SYMBOL  */
#line 548 "lab1.y"
                    { yyerror("Syntax Error: Two or more unknown variables entered without delimiter operations!"); }
#line 1820 "y.tab.c"
    break;

  case 30: /* base: NUMBER SYMBOL  */
#line 549 "lab1.y"
                    {
         checkOtherSymbols((yyvsp[0].terms[0].symbol));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct));
         (yyvsp[-1].terms[0].symbol) = ((yyvsp[0].terms[0].symbol));
         memcpy((yyval.terms), (yyvsp[-1].terms), sizeof(struct term_struct));
      }
#line 1831 "y.tab.c"
    break;

  case 31: /* base: NUMBER  */
#line 555 "lab1.y"
             {
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy((yyval.terms), (yyvsp[0].terms), sizeof(struct term_struct));
      }
#line 1840 "y.tab.c"
    break;

  case 32: /* base: SYMBOL  */
#line 559 "lab1.y"
             {
         checkOtherSymbols((yyvsp[0].terms[0].symbol));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy((yyval.terms), (yyvsp[0].terms), sizeof(struct term_struct));
      }
#line 1850 "y.tab.c"
    break;

  case 33: /* C: '(' D ')'  */
#line 566 "lab1.y"
                {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[-1].terms));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy((yyval.terms), (yyvsp[-1].terms), sizeof(struct term_struct) * (sizeOfArray + 1));
      }
#line 1860 "y.tab.c"
    break;

  case 34: /* C: NUMBER  */
#line 571 "lab1.y"
             {
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy((yyval.terms), (yyvsp[0].terms), sizeof(struct term_struct));
      }
#line 1869 "y.tab.c"
    break;

  case 35: /* C: '-' NUMBER  */
#line 575 "lab1.y"
                 {
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy((yyval.terms), (yyvsp[0].terms), sizeof(struct term_struct));
         (yyval.terms[0].coefficient) *= -1;
      }
#line 1879 "y.tab.c"
    break;

  case 36: /* D: D '+' D  */
#line 582 "lab1.y"
              {
         int sizeOfArray1 = getSizeOfArrayStruct((yyvsp[-2].terms));
         int sizeOfArray2 = getSizeOfArrayStruct((yyvsp[0].terms));
         (yyval.terms) = addition(sizeOfArray1, sizeOfArray2, (yyvsp[-2].terms), (yyvsp[0].terms));
      }
#line 1889 "y.tab.c"
    break;

  case 37: /* D: D '-' D  */
#line 587 "lab1.y"
              {
         int sizeOfArray1 = getSizeOfArrayStruct((yyvsp[-2].terms)); 
         int sizeOfArray2 = getSizeOfArrayStruct((yyvsp[0].terms));
         (yyvsp[0].terms) = changeSign(sizeOfArray2, (yyvsp[0].terms));
         (yyval.terms) = addition(sizeOfArray1, sizeOfArray2, (yyvsp[-2].terms), (yyvsp[0].terms));
      }
#line 1900 "y.tab.c"
    break;

  case 38: /* D: D '*' D  */
#line 593 "lab1.y"
              {
         int sizeOfArray1 = getSizeOfArrayStruct((yyvsp[-2].terms));
         int sizeOfArray2 = getSizeOfArrayStruct((yyvsp[0].terms));
         (yyval.terms) = multiple(sizeOfArray1, sizeOfArray2, (yyvsp[-2].terms), (yyvsp[0].terms));
      }
#line 1910 "y.tab.c"
    break;

  case 39: /* D: '-' D  */
#line 598 "lab1.y"
            {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[0].terms));
         (yyvsp[0].terms) = changeSign(sizeOfArray, (yyvsp[0].terms));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct) * (sizeOfArray));
         memcpy((yyval.terms), (yyvsp[0].terms), sizeof(struct term_struct) * (sizeOfArray));
      }
#line 1921 "y.tab.c"
    break;

  case 40: /* D: '(' D ')'  */
#line 604 "lab1.y"
                {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[-1].terms));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy((yyval.terms), (yyvsp[-1].terms), sizeof(struct term_struct) * (sizeOfArray + 1));
      }
#line 1931 "y.tab.c"
    break;

  case 41: /* D: D '^' D  */
#line 609 "lab1.y"
              {
         int sizeOfArray = getSizeOfArrayStruct((yyvsp[-2].terms));
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct)*(sizeOfArray + 1));
         memcpy((yyval.terms), (yyvsp[-1].terms), sizeof(struct term_struct) * (sizeOfArray + 1));
         int tmp = (yyval.terms[0].coefficient);
         for (int i = 1; i < (yyvsp[0].terms[0].coefficient); i++) {
            (yyval.terms[0].coefficient) *= tmp;
         }
      }
#line 1945 "y.tab.c"
    break;

  case 42: /* D: NUMBER  */
#line 618 "lab1.y"
             {
         (yyval.terms) = (struct term_struct*)malloc(sizeof(struct term_struct));
         memcpy((yyval.terms), (yyvsp[0].terms), sizeof(struct term_struct));
      }
#line 1954 "y.tab.c"
    break;


#line 1958 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 622 "lab1.y"


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
         printf("\nSyntax Error: No rule found! Line: %d.\n\n", yylineno);
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


