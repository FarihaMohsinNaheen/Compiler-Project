#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

using namespace std;

// TOKEN TYPES

enum class TokenType
{
    END_OF_FILE,
    INVALID,

    IDENTIFIER,
    NUMBER,

    INT_TYPE,    // পূর্ণ
    BOOL_TYPE,   // সত্যমিথ্যা
    TRUE_VALUE,  // সত্য
    FALSE_VALUE, // মিথ্যা

    PRINT, // লিখো
    IF,    // শর্ত
    ELSE,  // অন্যথা
    WHILE, // চলমান

    PLUS,
    MINUS,
    STAR,
    SLASH,

    ASSIGN,
    EQUAL,
    NOT_EQUAL,

    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,

    LEFT_PAREN,
    RIGHT_PAREN,

    LEFT_BRACE,
    RIGHT_BRACE,

    SEMICOLON
};

// TOKEN

struct Token
{
    TokenType type;
    string value;
    int line;
};

// TOKEN NAME

string tokenName(TokenType type);

// LEXER CLASS

class Lexer
{
private:
    string source;
    size_t position;
    int line;

    char currentChar();

    void advance();

    bool isIdentifierCharacter(unsigned char ch);

    TokenType checkKeyword(const string &word);

    void skipSpacesAndComments();

public:
    Lexer(const string &program);

    vector<Token> tokenize();
};

#endif