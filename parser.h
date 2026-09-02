#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

// PARSE TREE NODE

struct Node
{
    string name;
    vector<shared_ptr<Node>> children;

    Node(const string &nodeName)
    {
        name = nodeName;
    }
};

// PARSER CLASS

class Parser
{
private:
    vector<Token> tokens;
    size_t position;

    // Current token

    Token currentToken();

    // Move to next token

    void advance();

    // Check token

    bool check(TokenType type);

    // Match token

    bool match(TokenType type);

    // Consume token

    bool consume(
        TokenType type,
        const string &message);

    // Parser error

    void error(const string &message);

    // GRAMMAR FUNCTIONS

    shared_ptr<Node> program();

    shared_ptr<Node> statement();

    shared_ptr<Node> declaration();

    shared_ptr<Node> assignment();

    shared_ptr<Node> printStatement();

    shared_ptr<Node> ifStatement();

    shared_ptr<Node> whileStatement();

    shared_ptr<Node> block();

    // EXPRESSION FUNCTIONS
    shared_ptr<Node> expression();

    shared_ptr<Node> equality();

    shared_ptr<Node> comparison();

    shared_ptr<Node> term();

    shared_ptr<Node> factor();

    shared_ptr<Node> unary();

    shared_ptr<Node> primary();

public:
    Parser(const vector<Token> &inputTokens);

    // Start parsing

    shared_ptr<Node> parse();

    // Print parse tree

    void printTree(
        const shared_ptr<Node> &node,
        int level = 0);
};

#endif