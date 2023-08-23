%{

/* Declarations section */
#include <stdio.h>
#include "bison_objects.hpp"
#define YYSTYPE Node*
#include "parser.tab.hpp"
#include "hw3_output.hpp"




%}

%option yylineno
%option noyywrap


Void            "void"
Int             "int"
Byte            "byte"
B               "b"
Bool            "bool"
overRide        "override"
Const           "const"
And             "and"
Or              "or"
Not             "not"
True            "true"
False           "false"
Return          "return"
If              "if"
Else            "else"
While           "while"
Break           "break"
Continue        "continue"
Sc              ;
Comma           ,
Lparen          "("
Rparen          ")"
Lbrace          "{"
Rbrace          "}"
Assign          "="

Eq              "=="
Neq             "!="
Lt              "<"
Lte             "<="
Gt              ">"
Gte             ">="

Plus            "+"
Minus           "-"
Mul             "*"
Div             "/"

Comment         (\/\/)[^\r\n]*[\r|\n|\r\n]?
Id              [a-zA-Z][a-zA-Z0-9]*
Nonzero         [1-9][0-9]*
Zero            0
Num             {Nonzero}|{Zero}
String          (\"([^\n\r\"\\]|\\[rnt\"\\])+\")

Whitespace      [\r\t\n ]


%%



{Void}            { yylval = new Node(types::VOID); return VOID;}
{Int}             { yylval = new Node(types::INT); return INT; }
{Byte}            { yylval = new Node(types::BYTE); return BYTE; }
{B}               {return B;}
{Bool}            { yylval = new Node(types::BOOL); return BOOL;}
{overRide}        {yylval = new OverRide(false); return OVERRIDE;}
{Const}           {return CONST;}
{And}             {return AND;}
{Or}              {return OR;}
{Not}             {return NOT;}
{True}            { yylval = new Node(types::BOOL); return TRUE;}
{False}           { yylval = new Node(types::BOOL); return FALSE;}
{Return}          {return RETURN;}
{If}              {return IF;}
{Else}            {return ELSE;}
{While}           {return WHILE;}
{Break}           {return BREAK;}
{Continue}        {return CONTINUE;}
{Sc}              {return SC;}
{Comma}           {return COMMA;}
{Lparen}          {return LPAREN;}
{Rparen}          {return RPAREN;}
{Lbrace}          {return LBRACE;}
{Rbrace}          {return RBRACE;}
{Assign}          {return ASSIGN;}
{Eq}              {return EQ;}
{Neq}             {return NEQ;}
{Lt}              {return LT;}
{Lte}             {return LTE;}
{Gt}              {return GT;}
{Gte}             {return GTE;}
{Plus}            {return PLUS;}
{Minus}           {return MINUS;}
{Mul}             {return MUL;}
{Div}             {return DIV;}
{Id}              { yylval = new IDType(yytext); return ID;}
{Num}             { yylval = new Num(std::atoi(yytext)); return NUM;}
{Comment}         ;
{Whitespace}      ;
{String}          { yylval = new Node(types::STRING); return STRING;}
.                 {output::errorLex(yylineno); exit(0);}

%%


