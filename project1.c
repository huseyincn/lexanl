#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


bool isStringNotEnded = false; // dosyadan satır satır okuma işlemi yaptığımdan bu iki değişkeni global variable olarak kullanıyorum
bool isCommentNotEnded = false;



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
    if (len > 10) {
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
    if (len > 20) {
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

void parse(char *line, FILE *filex) {
    int left = 0, right = 0;
    int len = strlen(line);

    while (right <= len && left <= right) {
        if (line[left] == '(' && line[left + 1] == '*' || isCommentNotEnded) {
            isCommentNotEnded == true;
            while (line[right] != '*' && line[right + 1] != ')') {
                right++;
            }
            if(line[right] == '*' && line[right + 1] == ')')
                isCommentNotEnded==false;
        }

        if(line[left]=='"' || isStringNotEnded){
            if(left==right && isStringNotEnded==false)
                right++;
            isStringNotEnded==true;
            while (line[right]!='"')
                right++;
            // string ekleme yaz birden buyuk satirli string olursa right leften kucuk olanilir
            // birden fazla satırlı string atamalarında strcat kullan
        }
        if (line[right] != ' ')
            right++;
        else {

            if (left == right) {
                if (line[left] == ';')
                    fputs("EndOfLine\n", filex);
                if (line[left] == '(')
                    fputs("LeftPar\n", filex);
                if (line[left] == ')')
                    fputs("RightPar\n", filex);
                if (line[left] == '[')
                    fputs("LeftSquareBracket", filex);
                if (line[left] == ']')
                    fputs("RightSquareBracket", filex);
                if (line[left] == '{')
                    fputs("LeftCurlyBracket\n", filex);
                if (line[left] == '}')
                    fputs("RightCurlyBracket", filex);
            }

            char *kontrol = subString(line, left, right);


            left = right;
        }
    }
}

int main() {
    FILE *fpsi;
    FILE *flex;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fpsi = fopen("code.psi", "r");
    flex = fopen("code.lex", "w+");

    while ((read = getline(&line, &len, fpsi)) != -1) {
        printf("%s", line);


        // parse line // write to



    }
    if (line) // free the memory
        free(line);

    if (isCommentNotEnded)
        printf("\nThe comment in the code hasn't been ended by coder.\n");

    if (isStringNotEnded)
        printf("The string constant in the code hasn't been ended by coder.");

    fclose(flex);
    fclose(fpsi);
    return 0;
}