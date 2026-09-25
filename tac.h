#ifndef TAC_H
#define TAC_H

#include "parser.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

// TAC COPY

class TACCopy
{
private:
    string dest;
    string src;

public:
    TACCopy(
        const string &d,
        const string &s)
    {
        dest = d;
        src = s;
    }

    string toString() const;
};

// TAC BINARY OPERATION

class TACBinOp
{
private:
    string dest;
    string left;
    string op;
    string right;

public:
    TACBinOp(
        const string &d,
        const string &l,
        const string &o,
        const string &r)
    {
        dest = d;
        left = l;
        op = o;
        right = r;
    }

    string toString() const;
};

// TAC Print

class TACPrint
{
private:
    string src;

public:
    TACPrint(
        const string &s)
    {
        src = s;
    }

    string toString() const;
};

// TAC Generator

class TACGenerator
{
private:
    vector<string> instructions;

    int tempCount;

    string newTemp();

    string mapOperator(
        const string &op);

    string convertNumber(
        const string &value);

    string generateExpression(
        const shared_ptr<Node> &node);

    string generateCondition(
        const shared_ptr<Node> &node);

    void generateStatement(
        const shared_ptr<Node> &node);

    void generateBlock(
        const shared_ptr<Node> &node);

    void generateIf(
        const shared_ptr<Node> &node);

    void generateWhile(
        const shared_ptr<Node> &node);

public:
    TACGenerator();

    vector<string> generate(
        const shared_ptr<Node> &root);
};

#endif