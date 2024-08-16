#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

// Token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_INVALID,
    TOKEN_EOF
} TokenType;

// Keywords for the example
const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
    "bool", "catch", "class", "const_cast", "delete", "dynamic_cast", "explicit", 
    "export", "friend", "mutable", "namespace", "new", "operator", "private", 
    "protected", "public", "reinterpret_cast", "static_cast", "template", 
    "this", "throw", "try", "typeid", "using", "virtual", "wchar_t", NULL
};


// Function prototypes
void process_token(const char *token, TokenType type);
TokenType get_token_type(const char *token);
void skip_whitespace(FILE *input);
void skip_comment(FILE *input);
void lexer(FILE *input);

int main() {
    FILE *input = fopen("source_file.txt", "r");
    if (input == NULL) {
        perror("Error opening file");
        return 1;
    }

    lexer(input);

    fclose(input);
    return 0;
}

void process_token(const char *token, TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD:
            printf("Keyword: %s\n", token);
            break;
        case TOKEN_IDENTIFIER:
            printf("Identifier: %s\n", token);
            break;
        case TOKEN_NUMBER:
            printf("Number: %s\n", token);
            break;
        case TOKEN_OPERATOR:
            printf("Operator: %s\n", token);
            break;
        case TOKEN_PUNCTUATION:
            printf("Punctuation: %s\n", token);
            break;
        case TOKEN_INVALID:
            printf("Invalid token: %s\n", token);
            break;
        case TOKEN_EOF:
            printf("End of File\n");
            break;
    }
}

TokenType get_token_type(const char *token) {
    // Check for keywords
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            return TOKEN_KEYWORD;
        }
    }
    
    // Check for number
    int is_number = 1;
    for (int i = 0; token[i] != '\0'; i++) {
        if (!isdigit(token[i]) && token[i] != '.') {
            is_number = 0;
            break;
        }
    }
    if (is_number) {
        return TOKEN_NUMBER;
    }
    
    // Check for operators and punctuations
    if (strchr("+-*/=<>!;(),{}", token[0])) {
        return TOKEN_OPERATOR;
    }
    if (strchr("();,{}", token[0])) {
        return TOKEN_PUNCTUATION;
    }
    
    // Check for identifiers (basic check)
    if (isalpha(token[0]) || token[0] == '_') {
        for (int i = 1; token[i] != '\0'; i++) {
            if (!isalnum(token[i]) && token[i] != '_') {
                return TOKEN_INVALID;
            }
        }
        return TOKEN_IDENTIFIER;
    }
    
    return TOKEN_INVALID;
}

void skip_whitespace(FILE *input) {
    int ch;
    while (isspace(ch = fgetc(input)) && ch != EOF) {
        // Skip whitespace
    }
    ungetc(ch, input);
}

void skip_comment(FILE *input) {
    int ch;
    while ((ch = fgetc(input)) != EOF && ch != '\n') {
        // Skip comment characters
    }
}

void lexer(FILE *input) {
    char token[MAX_TOKEN_LENGTH];
    int index = 0;
    int ch;

    while ((ch = fgetc(input)) != EOF) {
        if (isspace(ch)) {
            continue;
        }
        if (ch == '/') {
            if ((ch = fgetc(input)) == '/') {
                skip_comment(input);
                continue;
            } else {
                ungetc(ch, input);
            }
        }
        if (isalpha(ch) || ch == '_') {
            token[index++] = ch;
            while (isalnum(ch = fgetc(input)) || ch == '_') {
                token[index++] = ch;
            }
            ungetc(ch, input);
            token[index] = '\0';
            index = 0;
            process_token(token, get_token_type(token));
        } else if (isdigit(ch)) {
            token[index++] = ch;
            while (isdigit(ch = fgetc(input)) || ch == '.') {
                token[index++] = ch;
            }
            ungetc(ch, input);
            token[index] = '\0';
            index = 0;
            process_token(token, get_token_type(token));
        } else {
            token[0] = ch;
            token[1] = '\0';
            process_token(token, get_token_type(token));
        }
    }

    process_token("", TOKEN_EOF);
}
