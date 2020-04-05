%{
/* Declaration section */

#include <stdio.h>
#include <stdlib.h>

typedef enum _TokenType
{
    ID, BIN_INT, OCT_INT, DEC_INT, HEX_INT, DEC_REAL, HEX_FP, STRING, TYPE, VAR, LET,
    FUNC, IMPORT, NIL, WHILE, IF, ELSE, RETURN, SC, COMMA, LPAREN, RPAREN, LBRACE, RBRACE,
    LBRACKET, RBRACKET, ASSIGN, RELOP, LOGOP, BINOP, TRUE, FALSE, ARROW, COLON, COMMENT
} TokenType;

typedef enum _ErrorType
{
    ILLEGAL_CHAR,
    UNCLOSED_STRING,
    UNCLOSED_COMMENT,
    UNDEFINED_ESCAPE_SEQUENCE,
    NESTED_COMMENT
} ErrorType;

char buffer[1025] = "";
int bufferLen = 0;

int currCommentNumLines = 1;

void presentToken(TokenType tokenType);

void printTokenName(TokenType tokenType);

void printTokenValue(TokenType tokenType);

void printIntLexemeAsDecInt(unsigned int intLexemeBase);

void printHexStringToBuffer(char* hex);

void printCharToBuffer(char c);

void clearBuffer();

void exitWithError(ErrorType errorType);

void printError(ErrorType errorType);

%}


%option yylineno
%option noyywrap

letter			            [a-zA-Z]
identifier_char             [a-zA-Z0-9]
hex_dig                     [0-9a-fA-F]
dec_dig                     [0-9]
whitespace		            [ \t\r\n]
character                   [\x00-\xFF]
printable_char              [\x20-\x7E\x09\x0A\x0D]
printable_char_no_newline   [\x20-\x7E\x09]
escape_seq_printable_ascii  (0{0,4}([2-6]{hex_dig}|7[0-9a-eA-E]|(0?[9dDaA])))
dec_real                    ({dec_dig}+\.{dec_dig}*|{dec_dig}*\.{dec_dig}+)
dec_scientific_suffix       [eE][+-]{dec_dig}*
hex_scientific_suffix       [pP][+-]{dec_dig}*
or_logop                    \|\|
and_logop                   &&

%x STRING_SC
%x MULTI_LINE_COMMENT
%%

<STRING_SC>\\n                                          printCharToBuffer('\n');
<STRING_SC>\\r                                          printCharToBuffer('\r');
<STRING_SC>\\t                                          printCharToBuffer('\t');
<STRING_SC>\\\\                                         printCharToBuffer('\\');
<STRING_SC>\\\"                                         printCharToBuffer('\"');
<STRING_SC>\\u\{{escape_seq_printable_ascii}\}          printHexStringToBuffer(yytext+3);
<STRING_SC>\\{character}                                exitWithError(UNDEFINED_ESCAPE_SEQUENCE);
<STRING_SC>[\n\r]                                       exitWithError(UNCLOSED_STRING);
<STRING_SC><<EOF>>                                      exitWithError(UNCLOSED_STRING);
<STRING_SC>\"                                           presentToken(STRING); BEGIN(INITIAL);
<STRING_SC>{printable_char}                             printCharToBuffer(yytext[0]);
<STRING_SC>.                                            exitWithError(ILLEGAL_CHAR);
<MULTI_LINE_COMMENT>\*\/                                presentToken(COMMENT); BEGIN(INITIAL);
<MULTI_LINE_COMMENT>\/\*                                exitWithError(NESTED_COMMENT);
<MULTI_LINE_COMMENT>(\n|\r|\r\n)                        currCommentNumLines++;
<MULTI_LINE_COMMENT>{printable_char}                    ;
<MULTI_LINE_COMMENT>.                                   exitWithError(ILLEGAL_CHAR);
<MULTI_LINE_COMMENT><<EOF>>                             exitWithError(UNCLOSED_COMMENT);
\"                                                      BEGIN(STRING_SC);
\/\*                                                    BEGIN(MULTI_LINE_COMMENT);
\/\/{printable_char_no_newline}*                        presentToken(COMMENT);
0b[01]+								                    presentToken(BIN_INT);
0o[0-7]+                                                presentToken(OCT_INT);
{dec_dig}*                                              presentToken(DEC_INT);
0x{hex_dig}+                                            presentToken(HEX_INT);
0x{hex_dig}+{hex_scientific_suffix}               	    presentToken(HEX_FP);
{dec_real}{dec_scientific_suffix}?	                    presentToken(DEC_REAL);
(Int|UInt|Double|Float|Bool|String|Character)           presentToken(TYPE);
var                                                     presentToken(VAR);
let                                                     presentToken(LET);
func                                                    presentToken(FUNC);
import                                                  presentToken(IMPORT);
nil                                                     presentToken(NIL);
while                                                   presentToken(WHILE);
if                                                      presentToken(IF);
else                                                    presentToken(ELSE);
return                                                  presentToken(RETURN);
;                                                       presentToken(SC);
,                                                       presentToken(COMMA);
\(                                                      presentToken(LPAREN);
\)                                                      presentToken(RPAREN);
\{                                                      presentToken(LBRACE);
\}                                                      presentToken(RBRACE);
\[                                                      presentToken(LBRACKET);
\]                                                      presentToken(RBRACKET);
=                                                       presentToken(ASSIGN);
(==|!=|<|>|<=|>=)                                       presentToken(RELOP);
({or_logop}|{and_logop})                                presentToken(LOGOP);
(\+|\-|\*|\/|\%)                                        presentToken(BINOP);
true                                                    presentToken(TRUE);
false                                                   presentToken(FALSE);
->                                                      presentToken(ARROW);
:                                                       presentToken(COLON);
(_{identifier_char}+|{letter}{identifier_char}*)        presentToken(ID);
{whitespace}                                            ;
.			                                            exitWithError(ILLEGAL_CHAR);

%%

void presentToken(TokenType tokenType)
{
    printf("%d ", yylineno);
	printTokenName(tokenType);
    printf(" ");
	printTokenValue(tokenType);
    printf("\n");
}

void printTokenName(TokenType tokenType)
{
    switch (tokenType)
    {
        case ID:            printf("ID");       break;
        case BIN_INT:       printf("BIN_INT");  break;
        case OCT_INT:       printf("OCT_INT");  break;
        case DEC_INT:       printf("DEC_INT");  break;
        case HEX_INT:       printf("HEX_INT");  break;
        case DEC_REAL:      printf("DEC_REAL"); break;
        case HEX_FP:        printf("HEX_FP");   break;
        case STRING:        printf("STRING");   break;
        case TYPE:          printf("TYPE");     break;
        case VAR:           printf("VAR");      break;
        case LET:           printf("LET"); 	    break;
        case FUNC:          printf("FUNC");     break;
        case IMPORT:        printf("IMPORT");   break;
        case NIL:           printf("NIL");      break;
        case WHILE:         printf("WHILE");    break;
        case IF:            printf("IF");       break;
        case ELSE:          printf("ELSE");     break;
        case RETURN:        printf("RETURN");   break;
        case SC:            printf("SC");       break;
        case COMMA:         printf("COMMA");    break;
        case LPAREN:        printf("LPAREN");   break;
        case RPAREN:        printf("RPAREN");   break;
        case LBRACE:        printf("LBRACE");   break;
        case RBRACE:        printf("RBRACE");   break;
        case LBRACKET:      printf("LBRACKET"); break;
        case RBRACKET:      printf("RBRACKET"); break;
        case ASSIGN:        printf("ASSIGN");   break;
        case RELOP:         printf("RELOP");    break;
        case LOGOP:         printf("LOGOP");    break;
        case BINOP:         printf("BINOP");    break;
        case TRUE:          printf("TRUE");     break;
        case FALSE:         printf("FALSE");    break;
        case ARROW:         printf("ARROW");    break;
        case COLON:         printf("COLON");    break;
        case COMMENT:       printf("COMMENT");  break;
        default:                                break;
    }
}

void printTokenValue(TokenType tokenType)
{
    switch (tokenType)
    {
        case BIN_INT:
            printIntLexemeAsDecInt(2);
            break;
        
        case OCT_INT:
            printIntLexemeAsDecInt(8);
            break;
        
        case DEC_INT:
            printIntLexemeAsDecInt(10);
            break;
        
        case HEX_INT:
            printIntLexemeAsDecInt(16);
            break;
        
        case STRING:
            printf("%s", buffer);
            clearBuffer();
            break;
        
        case COMMENT:
            printf("%d", currCommentNumLines);
            currCommentNumLines = 1;
            break;
            
        default:
            ECHO;
            break;
    }
}

void printIntLexemeAsDecInt(unsigned int intLexemeBase)
{
    int intOffset = (intLexemeBase == 10 ? 0 : 2);
    
    printf("%lu", strtol(yytext + intOffset, NULL, intLexemeBase));
}

void printCharToBuffer(char c)
{
    bufferLen += sprintf(buffer + bufferLen, "%c", c);
}

void printHexStringToBuffer(char* hex)
{
    int asciiDecimalValue = strtol(hex, NULL, 16);
    printCharToBuffer((char)asciiDecimalValue);
}

void clearBuffer()
{
    buffer[0] = '\0';
    bufferLen = 0;
}

void exitWithError(ErrorType errorType)
{
    printError(errorType);
    exit(0);
}

void printError(ErrorType errorType)
{
    switch (errorType)
    {
        case ILLEGAL_CHAR:
            printf("Error %c\n", yytext[0]);
            break;
            
        case UNCLOSED_STRING:
            printf("Error unclosed string\n");
            break;
            
        case UNCLOSED_COMMENT:
            printf("Error unclosed comment\n");
            break;
            
        case UNDEFINED_ESCAPE_SEQUENCE:
            printf("Error undefined escape sequence %c\n", yytext[1]);
            break;
            
        case NESTED_COMMENT:
            printf("Warning nested comment\n");
            break;
            
        default:
            break;
    }
}

//eli's repo try