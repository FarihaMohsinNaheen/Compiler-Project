#include "parser.h"

#include <iostream>

using namespace std;

// CONSTRUCTOR
Parser::Parser(const vector<Token> &inputTokens)
{
    tokens = inputTokens;
    position = 0;
}

// CURRENT TOKEN

Token Parser::currentToken()
{
    if (position < tokens.size())
    {
        return tokens[position];
    }

    return {
        TokenType::END_OF_FILE,
        "EOF",
        -1};
}

// ADVANCE

void Parser::advance()
{
    if (position < tokens.size())
    {
        position++;
    }
}

// CHECK

bool Parser::check(TokenType type)
{
    return currentToken().type == type;
}

// MATCH

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }

    return false;
}

// CONSUME

bool Parser::consume(
    TokenType type,
    const string &message)
{
    if (check(type))
    {
        advance();
        return true;
    }

    error(message);

    return false;
}

// ERROR

void Parser::error(const string &message)
{
    cout << "Parser Error at line "
         << currentToken().line
         << ": "
         << message
         << endl;
}

// CREATE NODE

shared_ptr<Node> makeNode(const string &name)
{
    return make_shared<Node>(name);
}

// PROGRAM
// program to statement*

shared_ptr<Node> Parser::program()
{
    auto node = makeNode("Program");

    while (!check(TokenType::END_OF_FILE))
    {
        auto statementNode = statement();

        if (statementNode != nullptr)
        {
            node->children.push_back(statementNode);
        }
    }

    return node;
}

// STATEMENT

shared_ptr<Node> Parser::statement()
{
    // Declaration

    if (
        check(TokenType::INT_TYPE) ||
        check(TokenType::BOOL_TYPE))
    {
        return declaration();
    }

    // Print

    if (check(TokenType::PRINT))
    {
        return printStatement();
    }

    // If

    if (check(TokenType::IF))
    {
        return ifStatement();
    }

    // While

    if (check(TokenType::WHILE))
    {
        return whileStatement();
    }

    // Assignment

    if (check(TokenType::IDENTIFIER))
    {
        return assignment();
    }

    error(
        "Unexpected token: " +
        tokenName(currentToken().type));

    advance();

    return nullptr;
}

// DECLARATION
// পূর্ণ বয়স = ২০;
// সত্যমিথ্যা ছাত্র = সত্য;

shared_ptr<Node> Parser::declaration()
{
    auto node = makeNode("Declaration");

    // Data type

    if (check(TokenType::INT_TYPE))
    {
        node->children.push_back(
            makeNode("পূর্ণ"));

        advance();
    }
    else if (check(TokenType::BOOL_TYPE))
    {
        node->children.push_back(
            makeNode("সত্যমিথ্যা"));

        advance();
    }
    else
    {
        error("Data type expected.");

        return nullptr;
    }

    // Variable name

    if (!check(TokenType::IDENTIFIER))
    {
        error("Variable name expected.");

        return nullptr;
    }

    node->children.push_back(
        makeNode(currentToken().value));

    advance();

    // Optional assignment

    if (match(TokenType::ASSIGN))
    {
        auto value = expression();

        if (value != nullptr)
        {
            node->children.push_back(value);
        }
    }

    // Semicolon

    consume(
        TokenType::SEMICOLON,
        "';' expected.");

    return node;
}

// ASSIGNMENT
// বয়স = বয়স + ১;

shared_ptr<Node> Parser::assignment()
{
    auto node = makeNode("Assignment");

    // Variable name

    string variableName =
        currentToken().value;

    consume(
        TokenType::IDENTIFIER,
        "Variable name expected.");

    node->children.push_back(
        makeNode(variableName));

    // Equal sign

    consume(
        TokenType::ASSIGN,
        "'=' expected.");

    // Expression

    auto value = expression();

    if (value != nullptr)
    {
        node->children.push_back(value);
    }

    // Semicolon

    consume(
        TokenType::SEMICOLON,
        "';' expected.");

    return node;
}

// PRINT
// লিখো(বয়স);

shared_ptr<Node> Parser::printStatement()
{
    auto node = makeNode("Print");

    consume(
        TokenType::PRINT,
        "'লিখো' expected.");

    consume(
        TokenType::LEFT_PAREN,
        "'(' expected.");

    auto value = expression();

    if (value != nullptr)
    {
        node->children.push_back(value);
    }

    consume(
        TokenType::RIGHT_PAREN,
        "')' expected.");

    consume(
        TokenType::SEMICOLON,
        "';' expected.");

    return node;
}

// IF / else condition

shared_ptr<Node> Parser::ifStatement()
{
    auto node = makeNode("If");

    consume(
        TokenType::IF,
        "'শর্ত' expected.");

    consume(
        TokenType::LEFT_PAREN,
        "'(' expected.");

    // Condition

    auto condition = expression();

    if (condition != nullptr)
    {
        auto conditionNode =
            makeNode("Condition");

        conditionNode->children.push_back(
            condition);

        node->children.push_back(
            conditionNode);
    }

    consume(
        TokenType::RIGHT_PAREN,
        "')' expected.");

    // Then block

    auto thenBlock = block();

    if (thenBlock != nullptr)
    {
        auto thenNode =
            makeNode("Then");

        thenNode->children.push_back(
            thenBlock);

        node->children.push_back(
            thenNode);
    }

    // Else block

    if (match(TokenType::ELSE))
    {
        auto elseBlock = block();

        if (elseBlock != nullptr)
        {
            auto elseNode =
                makeNode("Else");

            elseNode->children.push_back(
                elseBlock);

            node->children.push_back(
                elseNode);
        }
    }

    return node;
}

// WHILE

shared_ptr<Node> Parser::whileStatement()
{
    auto node = makeNode("While");

    consume(
        TokenType::WHILE,
        "'চলমান' expected.");

    consume(
        TokenType::LEFT_PAREN,
        "'(' expected.");

    // Condition

    auto condition = expression();

    if (condition != nullptr)
    {
        auto conditionNode =
            makeNode("Condition");

        conditionNode->children.push_back(
            condition);

        node->children.push_back(
            conditionNode);
    }

    consume(
        TokenType::RIGHT_PAREN,
        "')' expected.");

    // Body

    auto body = block();

    if (body != nullptr)
    {
        auto bodyNode =
            makeNode("Body");

        bodyNode->children.push_back(
            body);

        node->children.push_back(
            bodyNode);
    }

    return node;
}

// BLOCK

shared_ptr<Node> Parser::block()
{
    auto node = makeNode("Block");

    consume(
        TokenType::LEFT_BRACE,
        "'{' expected.");

    while (
        !check(TokenType::RIGHT_BRACE) &&
        !check(TokenType::END_OF_FILE))
    {
        auto statementNode = statement();

        if (statementNode != nullptr)
        {
            node->children.push_back(
                statementNode);
        }
    }

    consume(
        TokenType::RIGHT_BRACE,
        "'}' expected.");

    return node;
}

// EXPRESSION
// expression to equality

shared_ptr<Node> Parser::expression()
{
    return equality();
}

// EQUALITY
// equality to comparison
//             ((== | !=) comparison)*

shared_ptr<Node> Parser::equality()
{
    auto left = comparison();

    while (
        check(TokenType::EQUAL) ||
        check(TokenType::NOT_EQUAL))
    {
        TokenType operation =
            currentToken().type;

        advance();

        auto right = comparison();

        auto operatorNode =
            makeNode(tokenName(operation));

        operatorNode->children.push_back(left);
        operatorNode->children.push_back(right);

        left = operatorNode;
    }

    return left;
}

// COMPARISON

// comparison to term
//              ((< | <= | > | >=) term)*

shared_ptr<Node> Parser::comparison()
{
    auto left = term();

    while (
        check(TokenType::LESS) ||
        check(TokenType::LESS_EQUAL) ||
        check(TokenType::GREATER) ||
        check(TokenType::GREATER_EQUAL))
    {
        TokenType operation =
            currentToken().type;

        advance();

        auto right = term();

        auto operatorNode =
            makeNode(tokenName(operation));

        operatorNode->children.push_back(left);
        operatorNode->children.push_back(right);

        left = operatorNode;
    }

    return left;
}

// TERM
// term to factor
//        ((+ | -) factor)*

shared_ptr<Node> Parser::term()
{
    auto left = factor();

    while (
        check(TokenType::PLUS) ||
        check(TokenType::MINUS))
    {
        TokenType operation =
            currentToken().type;

        advance();

        auto right = factor();

        auto operatorNode =
            makeNode(tokenName(operation));

        operatorNode->children.push_back(left);
        operatorNode->children.push_back(right);

        left = operatorNode;
    }

    return left;
}

// FACTOR
// factor to unary
//          ((* | /) unary)*

shared_ptr<Node> Parser::factor()
{
    auto left = unary();

    while (
        check(TokenType::STAR) ||
        check(TokenType::SLASH))
    {
        TokenType operation =
            currentToken().type;

        advance();

        auto right = unary();

        auto operatorNode =
            makeNode(tokenName(operation));

        operatorNode->children.push_back(left);
        operatorNode->children.push_back(right);

        left = operatorNode;
    }

    return left;
}

// UNARY

shared_ptr<Node> Parser::unary()
{
    if (match(TokenType::MINUS))
    {
        auto node = makeNode("MINUS");

        auto value = unary();

        if (value != nullptr)
        {
            node->children.push_back(value);
        }

        return node;
    }

    return primary();
}

// PRIMARY

shared_ptr<Node> Parser::primary()
{
    // Number

    if (check(TokenType::NUMBER))
    {
        auto node =
            makeNode(currentToken().value);

        advance();

        return node;
    }

    // Identifier

    if (check(TokenType::IDENTIFIER))
    {
        auto node =
            makeNode(currentToken().value);

        advance();

        return node;
    }

    // সত্য

    if (check(TokenType::TRUE_VALUE))
    {
        auto node =
            makeNode("সত্য");

        advance();

        return node;
    }

    // মিথ্যা

    if (check(TokenType::FALSE_VALUE))
    {
        auto node =
            makeNode("মিথ্যা");

        advance();

        return node;
    }

    // Parentheses

    if (match(TokenType::LEFT_PAREN))
    {
        auto node = expression();

        consume(
            TokenType::RIGHT_PAREN,
            "')' expected.");

        return node;
    }

    error("Expression expected.");

    advance();

    return nullptr;
}

// PARSE

shared_ptr<Node> Parser::parse()
{
    return program();
}

// PRINT TREE

void Parser::printTree(
    const shared_ptr<Node> &node,
    int level)
{
    if (node == nullptr)
    {
        return;
    }

    // Print indentation

    for (int i = 0; i < level; i++)
    {
        cout << "    ";
    }

    cout << node->name << endl;

    // Print children

    for (const auto &child : node->children)
    {
        printTree(
            child,
            level + 1);
    }
}