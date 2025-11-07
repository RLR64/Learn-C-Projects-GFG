/*
Lexical Analyzer

Features:
- Tokenizes a C-like input file
- Multi-character operators (==, !=, <=, >=, ++, --, +=, -=, etc.)
- Floating-point number recognition (simple)
- Skips comments (single-line and multi-line)
- Writes tokens to 'tokens.txt' and prints to console

For Linux/macOS:
Replace fopen_s, strcpy_s, etc., with fopen, strcpy, if needed.

Code Structure:
Includes
Constants & Macros  
Struct Definitions
Global Variables
Function Declarations
Driver Code (main)
Function Definitions

Build:
clang -std=c17 -Wall -Wextra -Werror -g -O0 lexical_analyzer.c -o lexer.exe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constants & Macros
#define MAX_TOKEN_TEXT 128
#define MAX_TOKENS 2048
#define TOKEN_TYPE_LENGTH 32
#define OUTPUT_TOKEN_FILE "tokens.txt"

// Token Structure
typedef struct Token {
char text[MAX_TOKEN_TEXT];
char type[TOKEN_TYPE_LENGTH];
} token_t;

// Keyword Table
static const char *keywords[] = {
"auto","break","case","char","const","continue","default","do","double",
"else","enum","extern","float","for","goto","if","inline","int","long",
"register","restrict","return","short","signed","sizeof","static",
"struct","switch","typedef","union","unsigned","void","volatile","while",
"main"
};
static const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// Function Declarations
static int isKeyword(const char *word);
static void pushToken(token_t *tokens, int *token_count, const char *text, const char *type);
static void lexicalAnalysis(const char *filename, token_t *tokens, int *token_count);
static void skipSingleLineComment(FILE *f);
static void skipMultiLineComment(FILE *f);
static int readOperator(FILE *f, int first, char *out_buffer, size_t out_size);
static int readNumber(FILE *f, int first, char *out_buffer, size_t out_size);
static void writeTokensToFile(const char *filename, token_t *tokens, int token_count);


// Driver Code
int main(int argc, char **argv) {
// Accept source filename either from argv or interactively
char filename[512];
if (argc >= 2) {
    strncpy_s(filename, sizeof(filename), argv[1], _TRUNCATE);
} else {
    printf("Lexical Analyzer - Final\n");
    printf("Enter source filename: ");
    if (scanf_s("%511s", filename, (unsigned)_countof(filename)) != 1) {
        fprintf(stderr, "No filename provided. Exiting.\n");
        return EXIT_FAILURE;
    }
}

token_t tokens[MAX_TOKENS];
int token_count = 0;

lexicalAnalysis(filename, tokens, &token_count);

// Print tokens to console
printf("-------------------------------------\n");
printf(" Lexical Analysis Result (tokens found: %d)\n", token_count);
printf("-------------------------------------\n");
for (int i = 0; i < token_count; ++i) {
    printf("%-20s -> %s\n", tokens[i].text, tokens[i].type);
}
printf("-------------------------------------\n");

// Write tokens to file
writeTokensToFile(OUTPUT_TOKEN_FILE, tokens, token_count);
printf("Tokens saved to '%s'\n", OUTPUT_TOKEN_FILE);

return EXIT_SUCCESS;
}

// Function Declarations
// Check if word is a keyword
static int isKeyword(const char *word) {
for (int i = 0; i < keyword_count; ++i) {
    if (strcmp(word, keywords[i]) == 0) return 1;
}
return 0;
}

// Push a token safely into the token array
static void pushToken(token_t *tokens, int *token_count, const char *text, const char *type) {
if (*token_count >= MAX_TOKENS) {
    fprintf(stderr, "Warning: token buffer full, dropping token '%s'\n", text);
    return;
}
strncpy_s(tokens[*token_count].text, MAX_TOKEN_TEXT, text, _TRUNCATE);
strncpy_s(tokens[*token_count].type, TOKEN_TYPE_LENGTH, type, _TRUNCATE);
(*token_count)++;
}

// Skip single-line comment: consume until newline or EOF
static void skipSingleLineComment(FILE *f) {
int c;
while ((c = fgetc(f)) != EOF) {
    if (c == '\n') break;
}
}

// Skip multi-line comment: consume until "*/" or EOF
static void skipMultiLineComment(FILE *f) {
int prev = 0, curr;
while ((curr = fgetc(f)) != EOF) {
    if (prev == '*' && curr == '/') break;
    prev = curr;
}
}

// Read operator: supports multi-character operators.
// Returns 1 on success, 0 otherwise. Fills out_buffer.
// (caller should ensure out_buffer has enough space)
static int readOperator(FILE *f, int first, char *out_buffer, size_t out_size) {
// Multi-character operator candidates
int second = fgetc(f);
if (second == EOF) {
    // only first
    if (out_size) {
        out_buffer[0] = (char)first;
        out_buffer[1] = '\0';
    }
    return 1;
}

// common combos to recognize
int matched = 0;
if (first == '=' && second == '=') matched = 1;
else if (first == '!' && second == '=') matched = 1;
else if (first == '<' && second == '=') matched = 1;
else if (first == '>' && second == '=') matched = 1;
else if (first == '+' && second == '+') matched = 1;
else if (first == '-' && second == '-') matched = 1;
else if (first == '+' && second == '=') matched = 1;
else if (first == '-' && second == '=') matched = 1;
else if (first == '&' && second == '&') matched = 1;
else if (first == '|' && second == '|') matched = 1;
else if (first == '/' && second == '=') matched = 1;
else if (first == '*' && second == '=') matched = 1;

if (matched) {
    if (out_size) {
        out_buffer[0] = (char)first;
        out_buffer[1] = (char)second;
        out_buffer[2] = '\0';
    }
    return 1;
}

// not a multi-character operator — push second back and return single
ungetc(second, f);
if (out_size) {
    out_buffer[0] = (char)first;
    out_buffer[1] = '\0';
}
return 1;
}

// Read number token -> supports simple floating point (one dot).
// Returns 1 and fills out_buffer; caller gets stream positioned after
// the last digit/dot. If non-number start, behavior is undefined.
static int readNumber(FILE *f, int first, char *out_buffer, size_t out_size) {
size_t idx = 0;
int c = first;
int seen_dot = 0;

// store first char
if (idx + 1 < out_size) out_buffer[idx++] = (char)c;
else { out_buffer[0] = '\0'; return 0; }

while (1) {
    int next = fgetc(f);
    if (next == EOF) break;

    if (next == '.') {
        if (seen_dot) { // second dot -> stop and push back
            ungetc(next, f);
            break;
        } else {
            seen_dot = 1;
            if (idx + 1 < out_size) out_buffer[idx++] = '.';
            else break;
            continue;
        }
    }

    if (isdigit(next)) {
        if (idx + 1 < out_size) out_buffer[idx++] = (char)next;
        else { /* truncated */ }
    } else {
        // end of number
        ungetc(next, f);
        break;
    }
}

// null terminate
if (idx < out_size) out_buffer[idx] = '\0';
else out_buffer[out_size - 1] = '\0';
return 1;
}

// Lexical analysis main loop
static void lexicalAnalysis(const char *filename, token_t *tokens, int *token_count) {
FILE *source_file = NULL;
errno_t err = fopen_s(&source_file, filename, "r");
if (err != 0 || !source_file) {
    fprintf(stderr, "Error: cannot open source file '%s'\n", filename);
    return;
}

*token_count = 0;

int raw;
char buffer[MAX_TOKEN_TEXT];

while ((raw = fgetc(source_file)) != EOF) {
    char ch = (char)raw;

    // Skip whitespace
    if (isspace((unsigned char)ch)) continue;

    // Handle comments or divide operator start
    if (ch == '/') {
        int next = fgetc(source_file);
        if (next == '/') {
            // single-line comment
            skipSingleLineComment(source_file);
            continue;
        } else if (next == '*') {
            // multi-line comment
            skipMultiLineComment(source_file);
            continue;
        } else {
            // could be operator /= or just /
            ungetc(next, source_file);
            readOperator(source_file, ch, buffer, sizeof(buffer));
            pushToken(tokens, token_count, buffer, "OPERATOR");
            continue;
        }
    }

    // Identifiers or keywords (start with letter or underscore)
    if (isalpha((unsigned char)ch) || ch == '_') {
        size_t idx = 0;
        buffer[idx++] = ch;
        while (1) {
            int nxt = fgetc(source_file);
            if (nxt == EOF) break;
            if (isalnum((unsigned char)nxt) || nxt == '_') {
                if (idx + 1 < sizeof(buffer)) buffer[idx++] = (char)nxt;
                else { /* truncate if overflow */ }
            } else {
                ungetc(nxt, source_file);
                break;
            }
        }
        buffer[idx] = '\0';
        if (isKeyword(buffer)) pushToken(tokens, token_count, buffer, "KEYWORD");
        else pushToken(tokens, token_count, buffer, "IDENTIFIER");
        continue;
    }

    // Numbers (integer or float). Start with digit
    if (isdigit((unsigned char)ch)) {
        if (readNumber(source_file, ch, buffer, sizeof(buffer)))
            pushToken(tokens, token_count, buffer, "NUMBER");
        continue;
    }

    // Operators (single or multi char) and punctuation/delimiters
    if (strchr("+-*=<>!&|%^", ch)) {
        readOperator(source_file, ch, buffer, sizeof(buffer));
        pushToken(tokens, token_count, buffer, "OPERATOR");
        continue;
    }

    // Punctuation / delimiters
    if (strchr(";:,(){}[].", ch)) {
        char s[2] = { ch, '\0' };
        pushToken(tokens, token_count, s, "DELIMITER");
        continue;
    }

    // Anything else -> assign an unknown token
    {
        char s[2] = { ch, '\0' };
        pushToken(tokens, token_count, s, "UNKNOWN");
    }
}

fclose(source_file);
}

// Write tokens to text file
static void writeTokensToFile(const char *filename, token_t *tokens, int token_count) {
FILE *out = NULL;
errno_t err = fopen_s(&out, filename, "w");
if (err != 0 || !out) {
    fprintf(stderr, "Error: cannot open token output file '%s'\n", filename);
    return;
}

fprintf(out, "%-20s | %s\n", "TOKEN", "TYPE");
fprintf(out, "-------------------------------------\n");
for (int i = 0; i < token_count; ++i) {
    fprintf(out, "%-20s | %s\n", tokens[i].text, tokens[i].type);
}
fclose(out);
}
