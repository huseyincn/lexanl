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
    if (strcasecmp(kelime, "break") || strcasecmp(kelime, "case") || strcasecmp(kelime, "char") ||
        strcasecmp(kelime, "const") || strcasecmp(kelime, "continue") || strcasecmp(kelime, "do") ||
        strcasecmp(kelime, "else") || strcasecmp(kelime, "enum") || strcasecmp(kelime, "float") ||
        strcasecmp(kelime, "for") || strcasecmp(kelime, "goto") || strcasecmp(kelime, "if") ||
        strcasecmp(kelime, "int") || strcasecmp(kelime, "long") || strcasecmp(kelime, "record") ||
        strcasecmp(kelime, "return") || strcasecmp(kelime, "static") || strcasecmp(kelime, "while")) { return true; }
    return false;
} // pass to tests

bool isInteger(char *kelime) {
    int i;
    int len = strlen(kelime);
    if (len > MAXINTCONSTSIZE) {
        printf("The size of integer is bigger than supported.\n");
        return false;
    }
    if (len == 0)
        return false;
    for (i = 0; i < len; i++) {
        if (kelime[i] != '0' && kelime[i] != '1' && kelime[i] != '2' && kelime[i] != '3' && kelime[i] != '4' &&
            kelime[i] != '5' && kelime[i] != '6' && kelime[i] != '7' && kelime[i] != '8' && kelime[i] != '9' ||
            kelime[i] == '-')
            return false;
    }
    return true;
} // this code pass the tests

bool isOperator(char *kelime) {
    if (!strcasecmp(kelime, "+") || !strcasecmp(kelime, "-") || !strcasecmp(kelime, "*") || !strcasecmp(kelime, "/") ||
        !strcasecmp(kelime, "++") || !strcasecmp(kelime, "--") || !strcasecmp(kelime, ":="))
        return true;
    return false;
} // this function pass the tests

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

// Extracts the SUBSTRING.
char *subString(char *str, int left, int right) {
    int i;
    char *subStr = (char *) malloc(
            sizeof(char) * (right - left + 2));

    for (i = left; i <= right; i++)
        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return (subStr);
}

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

void parse(FILE *fin, FILE *fout) {
    char ch, buffer[21] = {0};
    char *str_temp = NULL;
    int i = 0, index = 0;

    while ((ch = getc(fin)) != EOF) {

        if (isdigit(ch) && (isspace(whatsNext(fin) || isdigit(whatsNext(fin))))) {
            str_temp = (char *) malloc(MAXINTCONSTSIZE + 1);
            i = 0;
            str_temp[i] = ch;
            while ((ch = fgetc(fin) != EOF && i <= MAXINTCONSTSIZE)) {
                str_temp[++i] = ch;
                if (!isdigit(whatsNext(fin))) {
                    str_temp[++i] = '\0';
                    break;
                }
            }
            if (i > MAXINTCONSTSIZE)
                printf("The size of integer is bigger than supported.\n");
            fprintf(fout, "IntConst(%s)", str_temp);
            free(str_temp);


        } else if (isalnum(ch) || ch == '_') {
            buffer[index++] = ch;
            if (!isalnum(whatsNext(fin)) && whatsNext(fin) != '_') { // ((kelime[i] != '_' && !isalpha(kelime[i])) && !isdigit(kelime[i]))
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
                        break;
                    }
                }
                if (ch == EOF) {
                    printf("\nThe string constant in the code hasn't been ended by coder.");
                }
            } else if (get_bracket_name(ch) != NULL) {
                fprintf(fout, "%s\n", get_bracket_name(ch));
            } else if (ch == '/' && whatsNext(fin) == '*') {
                fgetc(fin); // asterisk geçme
                while ((ch = fgetc(fin) )!= EOF) {
                    if (ch == '*' && whatsNext(fin) == '/') {
                        fgetc(fin); // slash geçme
                        break;
                    }
                }
                if (ch == EOF) {
                    printf("\nThe comment in the code hasn't been ended by coder.\n");
                }
            } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ':') {
                if (whatsNext(fin) == '=' && ch == ':'){
                    fgetc(fin);
                    fprintf(fout, "Operator(:=)\n");}
                else if (whatsNext(fin) == '+' && ch == '+'){
                    fgetc(fin);
                    fprintf(fout, "Operator(++)\n");}
                else if (whatsNext(fin) == '-' && ch == '-'){
                    fgetc(fin);
                    fprintf(fout, "Operator(--)\n");}
                else
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

/*
  int left = 0, right = 0;
    int len = strlen(line);

    while (right <= len && left <= right) {
        while (((line[left] == '(' && line[left + 1] == '*') || isCommentNotEnded) && right + 1 < len) {
            isCommentNotEnded = true;
            if (line[right] != '*' || line[right + 1] != ')') {
                right++;
            }
            if (line[right] == '*' && line[right + 1] == ')') {
                isCommentNotEnded = false;
                return;
            }
        }

        while ((isStringNotEnded || line[left] == '"') && right <= len) {
            if (isStringNotEnded == false)
                printf("String Constant(");
            if (isStringNotEnded == true && line[right] == '"') {
                isStringNotEnded = false;
                printf("%s)\n", line);
                return;
            }
            isStringNotEnded = true;
            if (left == right || line[right] != '"') {
                right++;
                if (right > len)
                    printf("%s", line);
            }
        }




        if (line[right] != ' ')
            right++;
        else {

            if (left == right) {
                if (line[left] == ';')
                    fputs("EndOfLine\n", filex);

            }
            char *kontrol = subString(line, left, right);
            if(isIdentifier(kontrol))
                printf("Identifier(%s)",kontrol);
            else if (isOperator(kontrol))
                printf("Operator(%s)",kontrol);
            else if (isInteger(kontrol))
                printf("IntConst(%s)",kontrol);
            else if (isKeyword(kontrol))
                printf("Keyword(%s)",kontrol);


            left = right;
        }
        }
 */