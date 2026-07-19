#include "tree_sitter/parser.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

enum TokenType {
    TRY,
};

static bool is_identifier_start(int32_t character) {
    return character == '_' || (character >= 'a' && character <= 'z') ||
           (character >= 'A' && character <= 'Z') || character >= 0x80;
}

static void advance(TSLexer *lexer) {
    lexer->advance(lexer, false);
}

static void skip(TSLexer *lexer) {
    lexer->advance(lexer, true);
}

static bool skip_separator(TSLexer *lexer) {
    bool separated = false;

    for (;;) {
        while (lexer->lookahead == ' ' || lexer->lookahead == '\t' ||
               lexer->lookahead == '\r' || lexer->lookahead == '\f') {
            separated = true;
            skip(lexer);
        }

        if (lexer->lookahead != '/') {
            return separated;
        }
        skip(lexer);
        if (lexer->lookahead != '*') {
            return false;
        }

        separated = true;
        skip(lexer);
        for (;;) {
            if (lexer->lookahead == 0 || lexer->lookahead == '\n') {
                return false;
            }
            if (lexer->lookahead == '*') {
                skip(lexer);
                if (lexer->lookahead == '/') {
                    skip(lexer);
                    break;
                }
                continue;
            }
            skip(lexer);
        }
    }
}

bool tree_sitter_go_external_scanner_scan(void *payload, TSLexer *lexer,
                                          const bool *valid_symbols) {
    (void)payload;

    if (!valid_symbols[TRY]) {
        return false;
    }

    while (lexer->lookahead == ' ' || lexer->lookahead == '\t' ||
           lexer->lookahead == '\n' || lexer->lookahead == '\r' ||
           lexer->lookahead == '\f') {
        skip(lexer);
    }

    if (lexer->lookahead != 't') {
        return false;
    }

    advance(lexer);
    if (lexer->lookahead != 'r') {
        return false;
    }
    advance(lexer);
    if (lexer->lookahead != 'y') {
        return false;
    }
    advance(lexer);
    lexer->mark_end(lexer);

    if (!skip_separator(lexer) || !is_identifier_start(lexer->lookahead)) {
        return false;
    }

    lexer->result_symbol = TRY;
    return true;
}

unsigned tree_sitter_go_external_scanner_serialize(void *payload, char *buffer) {
    (void)payload;
    (void)buffer;
    return 0;
}

void tree_sitter_go_external_scanner_deserialize(void *payload,
                                                 const char *buffer,
                                                 unsigned length) {
    (void)payload;
    (void)buffer;
    (void)length;
}

void *tree_sitter_go_external_scanner_create(void) {
    return malloc(1);
}

void tree_sitter_go_external_scanner_destroy(void *payload) {
    free(payload);
}
