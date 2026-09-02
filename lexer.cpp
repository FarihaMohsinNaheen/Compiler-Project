#include "lexer.h"

#include <cctype>
#include <iostream>

using namespace std;

// TOKEN NAME

string tokenName(TokenType type)
{
    switch (type)
    {
    case TokenType::END_OF_FILE:
        return "EOF";

    case TokenType::INVALID:
        return "INVALID";

    case TokenType::IDENTIFIER:
        return "IDENTIFIER";

    case TokenType::NUMBER:
        return "NUMBER";

    case TokenType::INT_TYPE:
        return "INT_TYPE";

    case TokenType::BOOL_TYPE:
        return "BOOL_TYPE";

    case TokenType::TRUE_VALUE:
        return "TRUE";

    case TokenType::FALSE_VALUE:
        return "FALSE";

    case TokenType::PRINT:
        return "PRINT";

    case TokenType::IF:
        return "IF";

    case TokenType::ELSE:
        return "ELSE";

    case TokenType::WHILE:
        return "WHILE";

    case TokenType::PLUS:
        return "PLUS";

    case TokenType::MINUS:
        return "MINUS";

    case TokenType::STAR:
        return "STAR";

    case TokenType::SLASH:
        return "SLASH";

    case TokenType::ASSIGN:
        return "ASSIGN";

    case TokenType::EQUAL:
        return "EQUAL";

    case TokenType::NOT_EQUAL:
        return "NOT_EQUAL";

    case TokenType::LESS:
        return "LESS";

    case TokenType::LESS_EQUAL:
        return "LESS_EQUAL";

    case TokenType::GREATER:
        return "GREATER";

    case TokenType::GREATER_EQUAL:
        return "GREATER_EQUAL";

    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";

    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";

    case TokenType::LEFT_BRACE:
        return "LEFT_BRACE";

    case TokenType::RIGHT_BRACE:
        return "RIGHT_BRACE";

    case TokenType::SEMICOLON:
        return "SEMICOLON";
    }

    return "UNKNOWN";
}

// CONSTRUCTOR

Lexer::Lexer(const string &program)
{
    source = program;
    position = 0;
    line = 1;
}

// CURRENT CHARACTER

char Lexer::currentChar()
{
    if (position >= source.size())
    {
        return '\0';
    }

    return source[position];
}

// ADVANCE

void Lexer::advance()
{
    if (currentChar() == '\n')
    {
        line++;
    }

    position++;
}

// IDENTIFIER CHARACTER

bool Lexer::isIdentifierCharacter(unsigned char ch)
{
    return isalnum(ch) || ch == '_' || ch >= 128;
}

// CHECK KEYWORD

TokenType Lexer::checkKeyword(const string &word)
{
    if (word == "পূর্ণ")
    {
        return TokenType::INT_TYPE;
    }

    if (word == "সত্যমিথ্যা")
    {
        return TokenType::BOOL_TYPE;
    }

    if (word == "সত্য")
    {
        return TokenType::TRUE_VALUE;
    }

    if (word == "মিথ্যা")
    {
        return TokenType::FALSE_VALUE;
    }

    if (word == "লিখো")
    {
        return TokenType::PRINT;
    }

    if (word == "শর্ত")
    {
        return TokenType::IF;
    }

    if (word == "অন্যথা")
    {
        return TokenType::ELSE;
    }

    if (word == "চলমান")
    {
        return TokenType::WHILE;
    }

    return TokenType::IDENTIFIER;
}

// SKIP SPACES AND COMMENTS

void Lexer::skipSpacesAndComments()
{
    while (true)
    {
        while (isspace(
            static_cast<unsigned char>(currentChar())))
        {
            advance();
        }

        // Single line comment
        // Example:
        // ~ এটি একটি মন্তব্য

        if (currentChar() == '~')
        {
            while (
                currentChar() != '\n' &&
                currentChar() != '\0')
            {
                advance();
            }
        }
        else
        {
            break;
        }
    }
}

// TOKENIZE

vector<Token> Lexer::tokenize()
{
    vector<Token> tokens;

    while (true)
    {
        skipSpacesAndComments();

        int tokenLine = line;

        char ch = currentChar();

        // END OF FILE

        if (ch == '\0')
        {
            tokens.push_back({TokenType::END_OF_FILE,
                              "EOF",
                              tokenLine});

            break;
        }

        // ENGLISH NUMBER

        if (isdigit(
                static_cast<unsigned char>(ch)))
        {
            string value;

            while (
                isdigit(
                    static_cast<unsigned char>(
                        currentChar())))
            {
                value += currentChar();

                advance();
            }

            tokens.push_back({TokenType::NUMBER,
                              value,
                              tokenLine});

            continue;
        }

        // BANGLA NUMBER
        //
        // ০ ১ ২ ৩ ৪ ৫ ৬ ৭ ৮ ৯

        if (
            static_cast<unsigned char>(ch) >= 0xE0)
        {
            string value;

            size_t start = position;

            while (
                position + 2 < source.size() &&

                static_cast<unsigned char>(
                    source[position]) == 0xE0 &&

                static_cast<unsigned char>(
                    source[position + 1]) == 0xA7 &&

                static_cast<unsigned char>(
                    source[position + 2]) >= 0xA6 &&

                static_cast<unsigned char>(
                    source[position + 2]) <= 0xAF)
            {
                value += source[position];
                value += source[position + 1];
                value += source[position + 2];

                position += 3;
            }

            if (position != start)
            {
                tokens.push_back({TokenType::NUMBER,
                                  value,
                                  tokenLine});

                continue;
            }
        }

        // IDENTIFIER OR KEYWORD

        if (
            isIdentifierCharacter(
                static_cast<unsigned char>(ch)))
        {
            string value;

            while (
                isIdentifierCharacter(
                    static_cast<unsigned char>(
                        currentChar())))
            {
                value += currentChar();

                advance();
            }

            tokens.push_back({checkKeyword(value),
                              value,
                              tokenLine});

            continue;
        }

        // MOVE TO NEXT CHARACTER

        advance();

        // OPERATORS AND SYMBOLS

        switch (ch)
        {
        case '+':

            tokens.push_back({TokenType::PLUS,
                              "+",
                              tokenLine});

            break;

        case '-':

            tokens.push_back({TokenType::MINUS,
                              "-",
                              tokenLine});

            break;

        case '*':

            tokens.push_back({TokenType::STAR,
                              "*",
                              tokenLine});

            break;

        case '/':

            tokens.push_back({TokenType::SLASH,
                              "/",
                              tokenLine});

            break;

        case '(':

            tokens.push_back({TokenType::LEFT_PAREN,
                              "(",
                              tokenLine});

            break;

        case ')':

            tokens.push_back({TokenType::RIGHT_PAREN,
                              ")",
                              tokenLine});

            break;

        case '{':

            tokens.push_back({TokenType::LEFT_BRACE,
                              "{",
                              tokenLine});

            break;

        case '}':

            tokens.push_back({TokenType::RIGHT_BRACE,
                              "}",
                              tokenLine});

            break;

        case ';':

            tokens.push_back({TokenType::SEMICOLON,
                              ";",
                              tokenLine});

            break;

            // ASSIGNMENT OR EQUAL

        case '=':

            if (currentChar() == '=')
            {
                advance();

                tokens.push_back({TokenType::EQUAL,
                                  "==",
                                  tokenLine});
            }
            else
            {
                tokens.push_back({TokenType::ASSIGN,
                                  "=",
                                  tokenLine});
            }

            break;

            // NOT EQUAL

        case '!':

            if (currentChar() == '=')
            {
                advance();

                tokens.push_back({TokenType::NOT_EQUAL,
                                  "!=",
                                  tokenLine});
            }
            else
            {
                tokens.push_back({TokenType::INVALID,
                                  "!",
                                  tokenLine});
            }

            break;

            // LESS OR LESS EQUAL

        case '<':

            if (currentChar() == '=')
            {
                advance();

                tokens.push_back({TokenType::LESS_EQUAL,
                                  "<=",
                                  tokenLine});
            }
            else
            {
                tokens.push_back({TokenType::LESS,
                                  "<",
                                  tokenLine});
            }

            break;

            // GREATER OR GREATER EQUAL

        case '>':

            if (currentChar() == '=')
            {
                advance();

                tokens.push_back({TokenType::GREATER_EQUAL,
                                  ">=",
                                  tokenLine});
            }
            else
            {
                tokens.push_back({TokenType::GREATER,
                                  ">",
                                  tokenLine});
            }

            break;

            // INVALID CHARACTER

        default:

            tokens.push_back({TokenType::INVALID,
                              string(1, ch),
                              tokenLine});

            break;
        }
    }

    return tokens;
}