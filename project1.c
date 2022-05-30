#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAXINTCONSTSIZE 10
#define MAXIDENTIFIERSIZE 20

//      break,case,char,const,continue,do,else,enum,float,for,goto,if,int,
//      long,record,return,static,while
bool isKeyword(char *kelime) {
    if (strcasecmp(kelime, "break") == 0 || strcasecmp(kelime, "case") == 0 || strcasecmp(kelime, "char") == 0 ||
        strcasecmp(kelime, "const") == 0 || strcasecmp(kelime, "continue") == 0 || strcasecmp(kelime, "do") == 0 ||
        strcasecmp(kelime, "else") == 0 || strcasecmp(kelime, "enum") == 0 || strcasecmp(kelime, "float") == 0 ||
        strcasecmp(kelime, "for") == 0 || strcasecmp(kelime, "goto") == 0 || strcasecmp(kelime, "if") == 0 ||
        strcasecmp(kelime, "int") == 0 || strcasecmp(kelime, "long") == 0 || strcasecmp(kelime, "record") == 0 ||
        strcasecmp(kelime, "return") == 0 || strcasecmp(kelime, "static") == 0 ||
        strcasecmp(kelime, "while") == 0) { return true; }
    return false;
} // pass to tests

bool isIdentifier(char *kelime) {
    int i;
    int len = strlen(kelime);
    if (len > MAXIDENTIFIERSIZE) {
        printf("The size of identifier is bigger than supported.\n");
        return false;
    }
    if (!isalpha(kelime[0]))
        return false;
    if (len > 1) {
        for (i = 1; i < len; ++i) {
            if ((kelime[i] != '_' && !isalpha(kelime[i])) && !isdigit(kelime[i]))
                return false;
        }
    }
    return true;
} // this fucntion passed all the tests

char *get_bracket_name(char ch) {
    if (ch == '(')
        return "LeftPar";
    else if (ch == ')')
        return "RightPar";
    else if (ch == '[')
        return "LeftSquareBracket";
    else if (ch == ']')
        return "RightSquareBracket";
    else if (ch == '{')
        return "LeftCurlyBracket";
    else if (ch == '}')
        return "RightCurlyBracket";
    else
        return NULL;
}

char whatsNext(FILE *stream) {
    char c;
    c = fgetc(stream);
    ungetc(c, stream);
    return c;
}

bool isDelimiter(char c) {
    if (c == ';' || c == ')' || c == '}' || c == ']' || c == '+' || c == '-' || c == '*' || c == '/' ||
        c == ':') { return true; }
    return false;
}

void parse(FILE *fin, FILE *fout) {
    char ch, buffer[21] = {0};
    char *str_temp = NULL;
    int i = 0, index = 0;

    while ((ch = getc(fin)) != EOF) {

        if (isdigit(ch) && (isdigit(whatsNext(fin)) || isDelimiter(whatsNext(fin)) || isspace(whatsNext(fin)))) {
            str_temp = (char *) malloc(MAXINTCONSTSIZE + 1);
            i = 0;
            str_temp[i] = ch;
            while (isdigit(whatsNext(fin)) && (ch = fgetc(fin)) != EOF && i <= MAXINTCONSTSIZE) {
                str_temp[++i] = ch;
            }
            str_temp[++i] = '\0';
            if (i > MAXINTCONSTSIZE)
                printf("The size of integer is bigger than supported.\n");
            fprintf(fout, "IntConst(%s)\n", str_temp);
            free(str_temp);
        } else if (isalnum(ch) || ch == '_') {
            buffer[index++] = ch;
            if (!isalnum(whatsNext(fin)) && whatsNext(fin) != '_') {
                buffer[index] = '\0';
                index = 0;
                if (isKeyword(buffer))
                    fprintf(fout, "Keyword(%s)\n", buffer);
                else if (isIdentifier(buffer))
                    fprintf(fout, "Identifier(%s)\n", buffer);
            }
        } else {
            if (ch == '"') {
                str_temp = (char *) malloc(200);
                i = 0;
                while ((ch = fgetc(fin)) != EOF) {
                    str_temp[i++] = ch;
                    if (whatsNext(fin) == '"') {
                        fgetc(fin); // diğer double quote silindi
                        str_temp[i] = '\0';
                        fprintf(fout, "StrConst(%s)\n", str_temp);
                        break;
                    }
                }
                if (ch == EOF) {
                    fprintf(fout, "The string constant in the code hasn't been ended by coder.\n");
                }
            } else if (ch == '(' && whatsNext(fin) == '*') {
                fgetc(fin); // asterisk geçme
                while ((ch = fgetc(fin)) != EOF) {
                    if (ch == '*' && whatsNext(fin) == ')') {
                        fgetc(fin); // slash geçme
                        break;
                    }
                }
                if (ch == EOF) {
                    fprintf(fout, "The comment in the code hasn't been ended by coder.\n");
                }
            } else if (get_bracket_name(ch) != NULL) {
                fprintf(fout, "%s\n", get_bracket_name(ch));
            } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ':') {
                if (whatsNext(fin) == '=' && ch == ':') {
                    fgetc(fin);
                    fprintf(fout, "Operator(:=)\n");
                } else if (whatsNext(fin) == '+' && ch == '+') {
                    fgetc(fin);
                    fprintf(fout, "Operator(++)\n");
                } else if (whatsNext(fin) == '-' && ch == '-') {
                    fgetc(fin);
                    fprintf(fout, "Operator(--)\n");
                } else
                    fprintf(fout, "Operator(%c)\n", ch);
            } else if (ch == ';')
                fprintf(fout, "EndOfLine\n");
        }
    }
    fclose(fin);
    fclose(fout);
}

int main() {
    FILE *fpsi;
    FILE *flex;
    fpsi = fopen("code.psi", "r");
    flex = fopen("code.lex", "w+");

    if (fpsi == NULL || flex == NULL) {
        printf("Error while opening the file\n");
        exit(1);
    }
    parse(fpsi, flex);
    return 0;
}