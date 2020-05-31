/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 36 "parse.y"


#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#include "square.h"
#include "node.h"

#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <assert.h>

extern FILE *yyin, *yyout;
extern int yyparse(parser_state*);
extern int yydebug;

static char*
strndup_(const char *str, size_t chars)
{
    char* buffer;
    size_t n;

    buffer = (char *)malloc(chars + 1);
    if(buffer)
    {
        for (n = 0;((n < chars) && (str[n] != 0));n++)
        {
          buffer[n] = str[n];
        }
        buffer[n] = 0;
    }

    return buffer;
}


/* State type in Lexcial */
typedef enum
{
  BEGIN,       /* start */
  INASSIGN,    /* assign */
  INCOMMENT,   /* comment */
  INNUM,       /* number */
  INID,        /* identifier */
  INSTR,       /* string */
  INEQ,        /* == */
  INNEQ,       /* != */
  INLESS,      /* < or <= */
  INGREATER,   /* > or >= */
  INAND,       /* & or && */
  INOR,        /* | or || */
  FINISH,      /* end */
}StateType;

#define MAXTOKENLEN 80
#define MAXFUNCNAME 50
#define MAXRESERVED 15

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

#define BUFLEN 4096
int lineno = 1;

BOOL is_ident;
/* function name */
squ_string fname;

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static BOOL EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

#define YYLEX_DECL() yylex(YYSTYPE *yylval,parser_state* p)
#define TokenType YYTOKENTYPE

static int yylineno = 1;

//#define yylval  (*((YYSTYPE*)(p->lval)))
#include "parse.tab.h"

static void
yyerror(parser_state *p, const char *s)
{
  p->nerr++;
  if (p->file_name)
  {
    fprintf(stderr, "%s[Error]:%d:%s\n", p->file_name, p->lineno, s);
  }
  else 
  {
    fprintf(stderr, "%s\n", s);
  }
  exit(1);
}

static void
yywarnning(parser_state* p,const char* s)
{
  if(p->file_name)
  {
    printf("%s[Warnning]:%d:%s\n",p ->file_name,p->lineno,s);
  }
  else
  {
    printf("[Warnning]:%s",s);
  }
}

static int
yywarp(void){
  return 1;
}

static int
getNextChar(void)
{ 
  if(!(linepos < bufsize))
  { 
    lineno++;
    if(fgets(lineBuf,BUFLEN-1,yyin))
    { 
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    { 
      EOF_flag = TRUE;
      return EOF;
    }
  }
  else
    return lineBuf[linepos++];
}


static void 
ungetNextChar(void)
{ 
  if (!EOF_flag)
  {
    linepos--;
  }
}

void
catToken(int c,int tokenStringIndex)
{
  if(tokenStringIndex < MAXTOKENLEN - 1)
  {
    tokenString[++tokenStringIndex] = c;
  }
}

/* lookup table of reserved words */
static struct
    { 
      char* str;
      int tok;
    } reservedWords[MAXRESERVED]
   = {
      {"if",keyword_if},
      {"else",keyword_else},
      {"null",keyword_null},
      {"import",keyword_import},
      {"false",keyword_false},
      {"true",keyword_false},
      {"break",keyword_break},
      {"return",keyword_return},
      {"goto",keyword_goto},
      {"block",keyword_block},
      {"func",keyword_func},
      {"class",keyword_class},
      {"loop",keyword_loop},
      {"when",keyword_when},
      {"is",keyword_is}
    };

/* look for existing keyword*/
/* linear search */
/* TODO: use binary search */
static int 
reservedLookup(squ_string s,YYSTYPE* yylval)
{ 
  int i;
  for (i = 0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
    {
      is_ident = FALSE;
      return reservedWords[i].tok;
    }
  is_ident = TRUE;

  if(is_ident)
  {
    yylval -> nd = node_ident_new(s);
    yylval -> id = node_ident_of(s);
  }

  return identifier;
}

static squ_int
lex_return(squ_int c,parser_state* p)
{
  p->lineno = p->tline;
  p->tline = yylineno;
  return c;
}

#define RETURN(c) return lex_return(c,p) 

TokenType getToken(YYSTYPE* yylval,parser_state* p){
  int result;
  int c;
  int tokenStringIndex = 0;
  StateType state = BEGIN;
  BOOL save;
  int yyleng = 0;
  while(state != FINISH)
  {
    c = getNextChar();
    save = TRUE;
    switch(state)
    {
      case BEGIN:
        if(isdigit(c))
        { 
          state = INNUM;
        }
        else if(isalpha(c))
        {
          state = INID;
        }
        else if(c == ':')
        {
          state = INASSIGN;
        }
        else if((c == ' ') || (c == '\t'))
        {
           save = FALSE;
        }
        else if((c == '\n') || (c == '\r'))
        {
          yylineno++;
          save = FALSE;
        }
        else if(c == '#')
        { 
          save = FALSE;
          state = INCOMMENT;
        }
        else if((c == '\"') || (c == '\''))
        {
          state = INSTR;
        }
        else if(c == '=')
        {
          state = INEQ;
        }
        else if(c == '!')
        {
          state = INNEQ;
        }
        else if(c == '<')
        {
          state = INLESS;
        }
        else if(c == '>')
        {
          state = INGREATER;
        }
        else if(c == '&')
        {
          state = INAND;
        }
        else if(c == '|')
        {
          state = INOR;
        }
        else
        {
          state = FINISH;
          switch(c)
          {
            case '+':
              result = op_add;
            break;
            case '-':
              result = op_sub;
            break;
            case '*':
              result = op_mul;
            break;
            case '/':
              result = op_div;
            break;
            case '%':
              result = op_mod;
            break;
            case '(':
              result = op_lp;
            break;
            case ')':
              result = op_rp;
            break;
            case '{':
              result = op_flp;
            break;
            case '}':
              result = op_frp;
            break;
          }
        }
      break;
      case INCOMMENT:
        save = FALSE;
        if(c == EOF)
        { 
          state = FINISH;
          result = ENDFILE;
        }
        else if(c == '#') 
        {
          state = BEGIN;
        }
      break;
      case INASSIGN:
        state = FINISH;
        if(c == '=')
        {
          result = op_assign;
        }
        else
        {
          ungetNextChar();
          save = FALSE;
          result = ERROR;
        }
      break;
      case INNUM:
        if(!isdigit(c))
        {
          ungetNextChar();
          save = FALSE;
          state = FINISH;
          result = lit_number;
          yylval -> nd = node_int_new(atoi(tokenString));
        }
      break;
      case INID:
        if(!isalpha(c))
        {
          ungetNextChar();
          save = FALSE;
          state = FINISH;
          result = identifier;
        }
      break;
      case INSTR:
        if((c == '"') || (c == '\''))
        {
          state = FINISH;
          save = FALSE;
          squ_string s = strndup_(tokenString,yyleng + 1);
          if(s[0] == '\'' || s[0] == '\"')
          {
            int i;
            for(i = 0; i < yyleng + 1;i++)
            {
              s[i] = s[i + 1];
            }
          }
          yylval -> nd = node_string_new(s);
          result = lit_string;
        }
        else
        {
          catToken(c,tokenStringIndex);
          yyleng++;
        }
      break;
      case INEQ:
        if(c == '=')
        {
          state = FINISH;
          result = op_eq;
        }
        else
        {
          result = op_assign;
        }
      break;
      case INNEQ:
        {
          state = FINISH;
          result = op_neq;
        }
      break;
      case INLESS:
        {
          if(c == '=')
          {
            result = op_le;
          }
          else
          {
            result = op_lt;
          }
          state = FINISH;
        }
      break;
      case INGREATER:
        {
          if(c == '=')
          {
            result = op_ge;
          }
          else
          {
            result = op_gt;
          }
          state = FINISH;
        }
      break;
      case INAND:
        {
          if(c == '&')
          {
            result = op_and;
          }
          else
          {
            result = op_amper;
          }
          state = FINISH;
        }
      break;
      case INOR:
        {
          if(c == '|')
          {
            result = op_or;
          }
          else
          {
            result = op_bar;
          }
          state = FINISH;
        }
      break;
      case FINISH:
      default:
        printf("Lexical Error: State : %d\n",state);
        state = FINISH;
        result = ERROR;
      break;
    }
    if((save) && (tokenStringIndex <= MAXTOKENLEN))
      tokenString[tokenStringIndex++] = (char)c;
    if(state == FINISH)
    { 
      tokenString[tokenStringIndex] = '\0';
      if(result == identifier)
        result = reservedLookup(tokenString,yylval);
    }
  }
  RETURN(result);
}


static int
yylex(YYSTYPE *yylval,parser_state* p)
{
  return getToken(yylval,p);
}



/* Line 371 of yacc.c  */
#line 550 "parse.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parse.tab.h".  */
#ifndef YY_YY_PARSE_TAB_H_INCLUDED
# define YY_YY_PARSE_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     keyword_if = 258,
     keyword_else = 259,
     keyword_break = 260,
     keyword_return = 261,
     keyword_null = 262,
     keyword_true = 263,
     keyword_false = 264,
     keyword_import = 265,
     keyword_goto = 266,
     keyword_block = 267,
     keyword_func = 268,
     keyword_class = 269,
     keyword_loop = 270,
     keyword_when = 271,
     keyword_is = 272,
     op_add = 273,
     op_sub = 274,
     op_mul = 275,
     op_div = 276,
     op_mod = 277,
     op_eq = 278,
     op_neq = 279,
     op_lt = 280,
     op_le = 281,
     op_gt = 282,
     op_ge = 283,
     op_and = 284,
     op_or = 285,
     op_bar = 286,
     op_amper = 287,
     op_next = 288,
     op_assign = 289,
     op_lp = 290,
     op_rp = 291,
     op_flp = 292,
     op_frp = 293,
     ERROR = 294,
     ENDFILE = 295,
     lit_number = 296,
     lit_string = 297,
     lit_true = 298,
     lit_false = 299,
     lit_null = 300,
     identifier = 301,
     op_LOWEST = 302,
     op_HIGHEST = 303
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 522 "parse.y"

  node* nd;
  squ_string id;


/* Line 387 of yacc.c  */
#line 647 "parse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (parser_state* p);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */
/* Line 390 of yacc.c  */
#line 567 "parse.y"


static int yylex(YYSTYPE* yylval,parser_state* p);
static void yyerror(parser_state* p, const char* s);
static int yywarp(void);
static void yywarnning(parser_state* p,const char* s);


/* Line 390 of yacc.c  */
#line 683 "parse.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  66
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   841

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNRULES -- Number of states.  */
#define YYNSTATES  232

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   303

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      60,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    58,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    51,     2,    56,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,    59,
      57,     2,     2,     2,    52,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    53,     2,    54,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    49,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    50
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    15,    18,    22,
      25,    28,    34,    38,    40,    42,    45,    47,    51,    55,
      59,    63,    67,    71,    75,    79,    83,    87,    91,    95,
      99,   103,   106,   109,   112,   115,   119,   123,   125,   129,
     133,   137,   141,   145,   149,   153,   157,   161,   165,   169,
     173,   177,   181,   184,   187,   190,   193,   197,   201,   203,
     204,   214,   216,   222,   223,   225,   227,   231,   232,   235,
     237,   239,   241,   245,   249,   252,   256,   260,   269,   277,
     279,   281,   283,   285,   290,   297,   301,   303,   305,   309,
     315,   324,   331,   339,   344,   348,   352,   354,   358,   359,
     361,   366,   370,   372,   375,   377,   381,   383,   385,   388,
     389,   391,   393,   395
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      62,     0,    -1,    63,    -1,    64,    83,    -1,    -1,    65,
      -1,    64,    84,    65,    -1,     1,    65,    -1,    66,    34,
      67,    -1,     6,    71,    -1,    10,    46,    -1,    65,    33,
      65,    33,    65,    -1,    14,    46,    79,    -1,     5,    -1,
      67,    -1,    11,    73,    -1,    46,    -1,    67,    18,    67,
      -1,    67,    19,    67,    -1,    67,    20,    67,    -1,    67,
      21,    67,    -1,    67,    22,    67,    -1,    67,    31,    67,
      -1,    67,    32,    67,    -1,    67,    27,    67,    -1,    67,
      28,    67,    -1,    67,    25,    67,    -1,    67,    26,    67,
      -1,    67,    23,    67,    -1,    67,    17,    67,    -1,    67,
      24,    67,    -1,    18,    67,    -1,    19,    67,    -1,    48,
      67,    -1,    49,    67,    -1,    67,    29,    67,    -1,    67,
      30,    67,    -1,    76,    -1,    68,    18,    68,    -1,    68,
      19,    68,    -1,    68,    20,    68,    -1,    68,    21,    68,
      -1,    68,    22,    68,    -1,    68,    31,    68,    -1,    68,
      32,    68,    -1,    68,    27,    68,    -1,    68,    28,    68,
      -1,    68,    25,    68,    -1,    68,    26,    68,    -1,    68,
      23,    68,    -1,    68,    17,    68,    -1,    68,    24,    68,
      -1,    18,    68,    -1,    19,    68,    -1,    48,    68,    -1,
      49,    68,    -1,    68,    29,    68,    -1,    68,    30,    68,
      -1,    75,    -1,    -1,    69,     4,     3,    35,    68,    36,
      37,    63,    38,    -1,    69,    -1,    69,     4,    37,    63,
      38,    -1,    -1,    72,    -1,    67,    -1,    72,    51,    67,
      -1,    -1,    52,    46,    -1,    41,    -1,    42,    -1,    46,
      -1,    35,    67,    36,    -1,    53,    72,    54,    -1,    53,
      54,    -1,    53,    78,    54,    -1,    53,    55,    54,    -1,
       3,    35,    68,    36,    37,    63,    38,    70,    -1,    16,
      35,    68,    36,    37,    63,    38,    -1,     7,    -1,     8,
      -1,     9,    -1,    74,    -1,    46,    35,    71,    36,    -1,
      75,    56,    46,    35,    71,    36,    -1,    75,    56,    46,
      -1,    74,    -1,    80,    -1,    12,    46,    80,    -1,    46,
      35,    71,    36,    79,    -1,    15,    37,    63,    38,    16,
      35,    68,    36,    -1,    13,    46,    35,    71,    36,    79,
      -1,    76,    56,    46,    35,    71,    36,    79,    -1,    76,
      56,    46,    79,    -1,    42,    55,    67,    -1,    46,    55,
      67,    -1,    77,    -1,    78,    51,    77,    -1,    -1,    80,
      -1,    37,    81,    63,    38,    -1,    37,    63,    38,    -1,
      57,    -1,    82,    57,    -1,    46,    -1,    82,    51,    46,
      -1,    84,    -1,    85,    -1,    84,    85,    -1,    -1,    58,
      -1,    29,    -1,    59,    -1,    60,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   652,   652,   658,   662,   665,   670,   675,   681,   685,
     689,   693,   697,   701,   705,   709,   712,   718,   722,   726,
     730,   734,   738,   742,   746,   750,   754,   758,   762,   766,
     770,   774,   778,   782,   786,   790,   794,   798,   804,   808,
     812,   816,   820,   824,   828,   832,   836,   840,   844,   848,
     852,   856,   860,   864,   868,   872,   876,   880,   884,   891,
     894,   900,   904,   911,   914,   920,   925,   932,   933,   936,
     937,   938,   942,   946,   950,   954,   958,   962,   966,   970,
     974,   978,   984,   988,   992,   996,  1002,  1003,  1007,  1011,
    1015,  1019,  1023,  1027,  1033,  1037,  1043,  1048,  1056,  1059,
    1065,  1069,  1075,  1079,  1085,  1090,  1097,  1100,  1101,  1105,
    1108,  1109,  1110,  1111
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "keyword_if", "keyword_else",
  "keyword_break", "keyword_return", "keyword_null", "keyword_true",
  "keyword_false", "keyword_import", "keyword_goto", "keyword_block",
  "keyword_func", "keyword_class", "keyword_loop", "keyword_when",
  "keyword_is", "op_add", "op_sub", "op_mul", "op_div", "op_mod", "op_eq",
  "op_neq", "op_lt", "op_le", "op_gt", "op_ge", "op_and", "op_or",
  "op_bar", "op_amper", "op_next", "op_assign", "op_lp", "op_rp", "op_flp",
  "op_frp", "ERROR", "ENDFILE", "lit_number", "lit_string", "lit_true",
  "lit_false", "lit_null", "identifier", "op_LOWEST", "'!'", "'~'",
  "op_HIGHEST", "','", "'@'", "'['", "']'", "':'", "'.'", "'<'", "' '",
  "';'", "'\\n'", "$accept", "program", "stmt_seq", "stmts", "stmt", "var",
  "expr", "condition", "opt_elsif", "opt_else", "opt_args", "args",
  "grade", "primary0", "cond", "primary", "map", "map_args", "opt_block",
  "block", "bparam", "f_args", "opt_terms", "terms", "term", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    33,   126,
     303,    44,    64,    91,    93,    58,    46,    60,    32,    59,
      10
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    63,    64,    64,    64,    64,    65,    65,
      65,    65,    65,    65,    65,    65,    66,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    69,
      69,    70,    70,    71,    71,    72,    72,    73,    73,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    75,    75,    75,    75,    76,    76,    76,    76,
      76,    76,    76,    76,    77,    77,    78,    78,    79,    79,
      80,    80,    81,    81,    82,    82,    83,    84,    84,    84,
      85,    85,    85,    85
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     3,     2,     3,     2,
       2,     5,     3,     1,     1,     2,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     3,     3,     1,     0,
       9,     1,     5,     0,     1,     1,     3,     0,     2,     1,
       1,     1,     3,     3,     2,     3,     3,     8,     7,     1,
       1,     1,     1,     4,     6,     3,     1,     1,     3,     5,
       8,     6,     7,     4,     3,     3,     1,     3,     0,     1,
       4,     3,     1,     2,     1,     3,     1,     1,     2,     0,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,    13,    63,    79,    80,    81,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,     0,     0,     0,     0,     2,   109,     5,     0,
      14,    86,    37,    87,     7,     0,    71,    65,     9,    64,
      10,     0,    15,     0,     0,    98,     0,     0,    31,    32,
       0,    71,   102,     0,     0,     0,    63,    33,    34,    70,
      71,    74,     0,     0,    96,     0,     1,   111,   110,   112,
     113,     3,   106,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,     0,     0,    82,
      58,     0,    68,    88,    63,    12,    99,     0,     0,    72,
     101,     0,     0,   103,     0,     0,     0,    76,    73,     0,
      75,     6,   108,     0,     8,    29,    17,    18,    19,    20,
      21,    28,    30,    26,    27,    24,    25,    35,    36,    22,
      23,    98,    52,    53,    63,    54,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,     0,   100,
     105,    98,    94,    95,     0,     0,    97,     0,    63,    93,
       0,    50,    38,    39,    40,    41,    42,    49,    51,    47,
      48,    45,    46,    56,    57,    43,    44,     0,    85,    98,
       0,     0,    89,    11,     0,    83,     0,    63,    91,     0,
       0,    98,    59,     0,     0,    78,    92,    61,    77,    84,
      90,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,    60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    25,    26,    27,    28,    29,    30,    98,   217,   218,
      38,    39,    42,    31,   100,    32,    64,    65,   105,    33,
      54,    55,    71,    72,    73
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -44
static const yytype_int16 yypact[] =
{
     218,   503,   -11,   -44,   550,   -44,   -44,   -44,   -40,   -22,
      -7,     6,    13,     7,    28,   550,   550,   550,   278,   -44,
     -44,    12,   550,   550,   450,    60,   -44,   -27,    38,    55,
     707,   -44,    34,   -44,    38,   570,    58,   707,   -44,    49,
     -44,    56,   -44,    68,    71,    68,   338,   570,   -44,   -44,
     607,   -17,   -44,    69,   338,   -41,   550,   -44,   -44,    53,
     -26,   -44,    57,   -43,   -44,   -29,   -44,   -44,   -44,   -44,
     -44,   -44,   396,   -44,   503,   550,   550,   550,   550,   550,
     550,   550,   550,   550,   550,   550,   550,   550,   550,   550,
     550,   550,    63,   570,   570,    75,   570,   570,   627,   -44,
      76,   550,   -44,   -44,   550,   -44,   -44,    93,   647,   -44,
     -44,    97,    91,   -44,    78,   550,   550,   -44,   -44,   -19,
     -44,    38,   -44,   105,   707,   777,    21,    21,   -44,   -44,
     -44,   777,   777,    36,    36,    36,    36,   789,   751,   723,
     453,     0,   -44,   -44,   550,   -44,   -44,   570,   570,   570,
     570,   570,   570,   570,   570,   570,   570,   570,   570,   570,
     570,   570,   570,   102,    94,   707,   106,   127,   107,   -44,
     -44,    68,   707,   707,    53,   108,   -44,   503,   550,   -44,
     128,   801,    29,    29,   -44,   -44,   -44,   801,   801,    48,
      48,    48,    48,   813,   764,   737,   506,   338,   130,    68,
     131,   338,   -44,   105,   132,   -44,   129,   550,   -44,   570,
     133,    68,   -44,   136,   667,   -44,   -44,   165,   -44,   -44,
     -44,     1,   141,   338,   570,   140,   687,   -44,   143,   338,
     144,   -44
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -44,   -44,   -18,   -44,    25,   -44,    -3,    -2,   -44,   -44,
      -8,   157,   -44,   -32,   -44,   -44,    65,   -44,   -37,   -38,
     -44,   -44,   -44,   -44,   113
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -105
static const yytype_int16 yytable[] =
{
      53,    37,    67,    99,   222,   103,    40,   106,   101,    56,
     112,   118,    48,    49,    50,    99,   113,   -16,    56,    57,
      58,    37,   119,   174,    35,   120,    34,   175,   107,   116,
      41,    68,    69,    70,  -104,   178,   111,    18,   223,    43,
    -104,    79,    80,    81,    46,   108,   -16,    56,   114,   150,
     151,   152,    44,    37,    77,    78,    79,    80,    81,    45,
      66,    99,    99,    47,    99,    99,   148,   149,   150,   151,
     152,    74,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,    75,
      92,   142,   143,    56,   145,   146,   166,   121,   165,   123,
     101,    37,   102,   106,   179,    18,   104,   110,   115,   141,
     144,   117,   172,   173,   171,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,   167,   164,   106,   202,   169,   180,   170,   177,   197,
     198,    37,   199,   200,   201,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   106,   208,   116,   205,   207,   209,   212,   211,   221,
     204,   215,   219,   106,   216,    37,   224,    99,   227,   206,
     229,    63,   231,   210,   176,   122,     0,     0,     0,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,   213,
       0,     0,   203,     0,    37,   225,     0,   214,     0,     0,
       0,   230,     0,     0,     0,     0,     0,     0,    -4,     1,
       0,     2,   226,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,     0,    15,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    -4,     0,     0,
       0,     0,     0,    17,     0,    18,     0,     0,     0,    19,
      20,     0,     0,     0,    21,     0,    22,    23,     0,     0,
       0,    24,     0,     0,     0,     0,    -4,    -4,    -4,     1,
       0,     2,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,     0,    15,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    -4,     0,     0,
       0,     0,     0,    17,     0,    18,    -4,     0,     0,    19,
      20,     0,     0,     0,    51,     0,    22,    23,     0,     0,
       0,    24,     0,     0,     0,    52,    -4,    -4,    -4,     1,
       0,     2,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,     0,    15,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    -4,     0,     0,
       0,     0,     0,    17,     0,    18,    -4,     0,     0,    19,
      20,     0,     0,     0,    21,     0,    22,    23,     0,     0,
       0,    24,     0,     0,     0,     0,    -4,    -4,    -4,     2,
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,    17,     0,    18,     0,     0,     0,    19,    20,     0,
       0,     0,    21,     0,    22,    23,     0,     0,     0,    24,
       0,     0,     0,     2,    68,    69,    70,     5,     6,     7,
       0,     0,    10,    11,     0,    13,    14,     0,    15,    16,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    17,     0,    18,     0,     0,
       0,    19,    59,     0,     0,     0,    60,     0,    22,    23,
       0,     0,     0,    24,    61,    62,     2,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       0,    15,    16,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    17,     0,
      18,     0,     0,     0,    19,    20,     0,     0,     0,    21,
       0,    22,    23,     2,     0,     0,    24,     5,     6,     7,
       0,     0,    10,    11,     0,    13,    14,     0,    15,    16,
       0,     0,     0,     2,     0,     0,     0,     5,     6,     7,
       0,     0,     0,     0,     0,    17,    14,    18,    93,    94,
       0,    19,    20,     0,     0,     0,    36,     0,    22,    23,
       0,     0,     0,    24,     0,    17,     0,     0,     0,     0,
       0,    19,    20,     0,     0,     0,    95,     0,    96,    97,
       0,     0,     0,    24,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
       0,     0,     0,   109,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,     0,     0,   163,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,     0,     0,   168,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,     0,     0,   220,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,     0,     0,   228,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,  -105,    77,    78,    79,    80,    81,
    -105,  -105,    84,    85,    86,    87,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,  -105,   148,
     149,   150,   151,   152,  -105,  -105,   155,   156,   157,   158,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-44)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-105)))

static const yytype_int16 yycheck[] =
{
      18,     4,    29,    35,     3,    43,    46,    45,    51,    35,
      51,    54,    15,    16,    17,    47,    57,    34,    35,    22,
      23,    24,    51,    42,    35,    54,     1,    46,    46,    55,
      52,    58,    59,    60,    51,    35,    54,    37,    37,    46,
      57,    20,    21,    22,    37,    47,    34,    35,    56,    20,
      21,    22,    46,    56,    18,    19,    20,    21,    22,    46,
       0,    93,    94,    35,    96,    97,    18,    19,    20,    21,
      22,    33,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    34,
      56,    93,    94,    35,    96,    97,   104,    72,   101,    74,
      51,   104,    46,   141,   141,    37,    35,    38,    55,    46,
      35,    54,   115,   116,    36,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    38,    56,   171,   171,    38,   144,    46,    33,    37,
      46,   144,    36,    16,    37,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   199,   199,    55,    36,    35,    35,    38,    36,     4,
     178,    38,    36,   211,   211,   178,    35,   209,    38,   197,
      37,    24,    38,   201,   119,    72,    -1,    -1,    -1,    -1,
      -1,    -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,   177,    -1,   207,   223,    -1,   209,    -1,    -1,
      -1,   229,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,
      -1,     3,   224,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    60,     1,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    41,
      42,    -1,    -1,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    57,    58,    59,    60,     1,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    41,
      42,    -1,    -1,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    58,    59,    60,     3,
      -1,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,     3,    58,    59,    60,     7,     8,     9,
      -1,    -1,    12,    13,    -1,    15,    16,    -1,    18,    19,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    35,    -1,    37,    -1,    -1,
      -1,    41,    42,    -1,    -1,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    53,    54,    55,     3,    -1,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    35,    -1,
      37,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    46,
      -1,    48,    49,     3,    -1,    -1,    53,     7,     8,     9,
      -1,    -1,    12,    13,    -1,    15,    16,    -1,    18,    19,
      -1,    -1,    -1,     3,    -1,    -1,    -1,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    35,    16,    37,    18,    19,
      -1,    41,    42,    -1,    -1,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    53,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    41,    42,    -1,    -1,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    53,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    36,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    35,    37,    41,
      42,    46,    48,    49,    53,    62,    63,    64,    65,    66,
      67,    74,    76,    80,    65,    35,    46,    67,    71,    72,
      46,    52,    73,    46,    46,    46,    37,    35,    67,    67,
      67,    46,    57,    63,    81,    82,    35,    67,    67,    42,
      46,    54,    55,    72,    77,    78,     0,    29,    58,    59,
      60,    83,    84,    85,    33,    34,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    56,    18,    19,    46,    48,    49,    68,    74,
      75,    51,    46,    80,    35,    79,    80,    63,    68,    36,
      38,    63,    51,    57,    71,    55,    55,    54,    54,    51,
      54,    65,    85,    65,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    46,    68,    68,    35,    68,    68,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    36,    56,    67,    71,    38,    36,    38,
      46,    36,    67,    67,    42,    46,    77,    33,    35,    79,
      71,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    37,    46,    36,
      16,    37,    79,    65,    71,    36,    63,    35,    79,    35,
      63,    36,    38,    71,    68,    38,    79,    69,    70,    36,
      36,     4,     3,    37,    35,    63,    68,    38,    36,    37,
      63,    38
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (p, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, p)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, p); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_state* p)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, p)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    parser_state* p;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (p);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_state* p)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, p)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    parser_state* p;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, p);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, parser_state* p)
#else
static void
yy_reduce_print (yyvsp, yyrule, p)
    YYSTYPE *yyvsp;
    int yyrule;
    parser_state* p;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , p);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, p); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, parser_state* p)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, p)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    parser_state* p;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (p);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (parser_state* p)
#else
int
yyparse (p)
    parser_state* p;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 653 "parse.y"
    { 
                      p->lval = (yyvsp[(1) - (1)].nd);  
                    }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 662 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                    }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 666 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                      node_array_add((yyval.nd), (yyvsp[(1) - (1)].nd));
                    }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 671 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (3)].nd);
                      node_array_add((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 676 "parse.y"
    {

                    }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 682 "parse.y"
    {
                      (yyval.nd) = node_let_new((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 686 "parse.y"
    {
                      (yyval.nd) = node_return_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 690 "parse.y"
    {
                      (yyval.nd) = node_import_new((yyvsp[(2) - (2)].id));
                    }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 694 "parse.y"
    {

                  }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 698 "parse.y"
    {

                  }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 702 "parse.y"
    {
                      (yyval.nd) = node_break_new();
                    }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 706 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 713 "parse.y"
    {
                        (yyval.nd) = node_ident_new((yyvsp[(1) - (1)].id));
                    }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 719 "parse.y"
    {
                      (yyval.nd) = node_op_new("+", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 723 "parse.y"
    {
                      (yyval.nd) = node_op_new("-", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 727 "parse.y"
    {
                      (yyval.nd) = node_op_new("*", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 731 "parse.y"
    {
                      (yyval.nd) = node_op_new("/", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 735 "parse.y"
    {
                      (yyval.nd) = node_op_new("%", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 739 "parse.y"
    {
                      (yyval.nd) = node_op_new("|", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 743 "parse.y"
    {
                      (yyval.nd) = node_op_new("&", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 747 "parse.y"
    {
                      (yyval.nd) = node_op_new(">", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 751 "parse.y"
    {
                      (yyval.nd) = node_op_new(">=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 755 "parse.y"
    {
                      (yyval.nd) = node_op_new("<", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 759 "parse.y"
    {
                      (yyval.nd) = node_op_new("<=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 763 "parse.y"
    {
                      (yyval.nd) = node_op_new("==", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 767 "parse.y"
    {
                      (yyval.nd) = node_op_new("==", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 771 "parse.y"
    {
                      (yyval.nd) = node_op_new("!=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 775 "parse.y"
    {
                      (yyval.nd) = node_value_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 779 "parse.y"
    {
                      (yyval.nd) = node_value_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 783 "parse.y"
    {
                      (yyval.nd) = node_op_new("!", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 787 "parse.y"
    {
                      (yyval.nd) = node_op_new("~", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 791 "parse.y"
    {
                      (yyval.nd) = node_op_new("&&", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 795 "parse.y"
    {
                      (yyval.nd) = node_op_new("||", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 799 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 805 "parse.y"
    {
                      (yyval.nd) = node_op_new("+", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 809 "parse.y"
    {
                      (yyval.nd) = node_op_new("-", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 813 "parse.y"
    {
                      (yyval.nd) = node_op_new("*", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 817 "parse.y"
    {
                      (yyval.nd) = node_op_new("/", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 821 "parse.y"
    {
                      (yyval.nd) = node_op_new("%", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 825 "parse.y"
    {
                      (yyval.nd) = node_op_new("|", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 829 "parse.y"
    {
                      (yyval.nd) = node_op_new("&", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 833 "parse.y"
    {
                      (yyval.nd) = node_op_new(">", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 837 "parse.y"
    {
                      (yyval.nd) = node_op_new(">=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 841 "parse.y"
    {
                      (yyval.nd) = node_op_new("<", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 845 "parse.y"
    {
                      (yyval.nd) = node_op_new("<=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 849 "parse.y"
    {
                      (yyval.nd) = node_op_new("==", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 853 "parse.y"
    {
                      (yyval.nd) = node_op_new("==", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 857 "parse.y"
    {
                      (yyval.nd) = node_op_new("!=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 861 "parse.y"
    {
                      (yyval.nd) = node_value_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 865 "parse.y"
    {
                      (yyval.nd) = node_value_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 869 "parse.y"
    {
                      (yyval.nd) = node_op_new("!", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 873 "parse.y"
    {
                      (yyval.nd) = node_op_new("~", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 877 "parse.y"
    {
                      (yyval.nd) = node_op_new("&&", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 881 "parse.y"
    {
                      (yyval.nd) = node_op_new("||", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 885 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 891 "parse.y"
    {
                      (yyval.nd) = NULL;
                    }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 895 "parse.y"
    {
                      (yyval.nd) = node_if_new((yyvsp[(5) - (9)].nd), (yyvsp[(8) - (9)].nd), NULL);
                    }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 901 "parse.y"
    {
                      (yyval.nd) = NULL;
                    }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 905 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(4) - (5)].nd);
                    }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 911 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                    }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 915 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 921 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                      node_array_add((yyval.nd), (yyvsp[(1) - (1)].nd));
                    }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 926 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (3)].nd);
                      node_array_add((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 939 "parse.y"
    {
                    (yyval.nd) = node_ident_new((yyvsp[(1) - (1)].id));
                  }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 943 "parse.y"
    {
                       (yyval.nd) = (yyvsp[(2) - (3)].nd);
                    }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 947 "parse.y"
    {
                      (yyval.nd) = node_array_of((yyvsp[(2) - (3)].nd));
                    }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 951 "parse.y"
    {
                      (yyval.nd) = node_array_of(NULL);
                    }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 955 "parse.y"
    {
                      (yyval.nd) = node_map_of((yyvsp[(2) - (3)].nd));
                    }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 959 "parse.y"
    {
                      (yyval.nd) = node_map_of(NULL);
                    }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 963 "parse.y"
    {
                      (yyval.nd) = node_if_new((yyvsp[(3) - (8)].nd), (yyvsp[(6) - (8)].nd), (yyvsp[(8) - (8)].nd));
                    }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 967 "parse.y"
    {
                     (yyval.nd) = node_if_new((yyvsp[(3) - (7)].nd), (yyvsp[(6) - (7)].nd), NULL);
                    }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 971 "parse.y"
    {
                      (yyval.nd) = node_null();
                    }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 975 "parse.y"
    {
                      (yyval.nd) = node_true();
                    }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 979 "parse.y"
    {
                      (yyval.nd) = node_false();
                    }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 985 "parse.y"
    {
                       (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 989 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, node_ident_new((yyvsp[(1) - (4)].id)), (yyvsp[(3) - (4)].nd), NULL);
                    }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 993 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, node_ident_new((yyvsp[(3) - (6)].id)), (yyvsp[(5) - (6)].nd), NULL);
                    }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 997 "parse.y"
    {
                      (yyval.nd) = node_call_new((yyvsp[(1) - (3)].nd), node_ident_new((yyvsp[(3) - (3)].id)), NULL, NULL);
                    }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 1004 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, NULL, NULL, (yyvsp[(1) - (1)].nd));
                    }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 1008 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, node_ident_new((yyvsp[(2) - (3)].id)), NULL, (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 1012 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, node_ident_new((yyvsp[(1) - (5)].id)), (yyvsp[(3) - (5)].nd), (yyvsp[(5) - (5)].nd));
                    }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 1016 "parse.y"
    {
                      (yyval.nd) = node_loop_new((yyvsp[(3) - (8)].nd), (yyvsp[(7) - (8)].nd));
                    }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 1020 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL,node_ident_new((yyvsp[(2) - (6)].id)), (yyvsp[(4) - (6)].nd), (yyvsp[(6) - (6)].nd));
                    }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 1024 "parse.y"
    {
                      (yyval.nd) = node_call_new((yyvsp[(1) - (7)].nd), node_ident_new((yyvsp[(3) - (7)].id)), (yyvsp[(5) - (7)].nd), (yyvsp[(7) - (7)].nd));
                    }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 1028 "parse.y"
    {
                      (yyval.nd) = node_call_new((yyvsp[(1) - (4)].nd), node_ident_new((yyvsp[(3) - (4)].id)), NULL, (yyvsp[(4) - (4)].nd));
                    }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 1034 "parse.y"
    {
                      (yyval.nd) = node_pair_new((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 1038 "parse.y"
    {
                      (yyval.nd) = node_pair_new(node_ident_new((yyvsp[(1) - (3)].id)), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 1044 "parse.y"
    {
                      (yyval.nd) = node_map_new();
                      node_array_add((yyval.nd), (yyvsp[(1) - (1)].nd));
                    }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 1049 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (3)].nd);
                      node_array_add((yyval.nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 1056 "parse.y"
    {
                      (yyval.nd) = NULL;
                    }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 1060 "parse.y"
    {
                       (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 1066 "parse.y"
    {
                      (yyval.nd) = node_block_new((yyvsp[(2) - (4)].nd), (yyvsp[(3) - (4)].nd));
                    }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 1070 "parse.y"
    {
                      (yyval.nd) = node_block_new(NULL, (yyvsp[(2) - (3)].nd));
                    }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 1076 "parse.y"
    {
                      (yyval.nd) = NULL;
                    }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 1080 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 1086 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                      node_array_add((yyval.nd), node_ident_new((yyvsp[(1) - (1)].id)));
                    }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 1091 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (3)].nd);
                      node_array_add((yyval.nd), node_ident_new((yyvsp[(3) - (3)].id)));
                    }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 1102 "parse.y"
    {
                    yyerrok;
                  }
    break;


/* Line 1792 of yacc.c  */
#line 3016 "parse.tab.c"
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (p, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (p, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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
		      yytoken, &yylval, p);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp, p);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (p, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, p);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, p);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 1113 "parse.y"
