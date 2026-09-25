#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"
#include "symbol_table.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

class SemanticAnalyzer
{
private:
    SymbolTable symbolTable;

    vector<string> errors;

    void analyzeStatement(
        const shared_ptr<Node> &node);

    void analyzeDeclaration(
        const shared_ptr<Node> &node);

    void analyzeAssignment(
        const shared_ptr<Node> &node);

    void analyzePrint(
        const shared_ptr<Node> &node);

    void analyzeIf(
        const shared_ptr<Node> &node);

    void analyzeWhile(
        const shared_ptr<Node> &node);

    void analyzeBlock(
        const shared_ptr<Node> &node);

    string analyzeExpression(
        const shared_ptr<Node> &node);

    bool isNumber(
        const string &value);

public:
    void analyze(
        const shared_ptr<Node> &root);

    bool hasErrors();

    void printErrors();

    void printSymbolTable();
};

#endif