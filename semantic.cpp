#include "semantic.h"

#include <cctype>
#include <iostream>

using namespace std;

// CHECK NUMBER

bool SemanticAnalyzer::isNumber(
    const string &value)
{
    if (value.empty())
    {
        return false;
    }

    // ASCII digits

    bool asciiNumber = true;

    for (unsigned char ch : value)
    {
        if (!isdigit(ch))
        {
            asciiNumber = false;
            break;
        }
    }

    if (asciiNumber)
    {
        return true;
    }

    // Bangla digits

    size_t i = 0;

    while (i + 2 < value.size())
    {
        unsigned char b1 =
            static_cast<unsigned char>(
                value[i]);

        unsigned char b2 =
            static_cast<unsigned char>(
                value[i + 1]);

        unsigned char b3 =
            static_cast<unsigned char>(
                value[i + 2]);

        if (
            b1 != 0xE0 ||
            b2 != 0xA7 ||
            b3 < 0xA6 ||
            b3 > 0xAF)
        {
            return false;
        }

        i += 3;
    }

    return i == value.size();
}

// ANALYZE EXPRESSION

string SemanticAnalyzer::analyzeExpression(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return "unknown";
    }

    // NUMBER

    if (isNumber(node->name))
    {
        return "int";
    }

    // TRUE

    if (node->name == "সত্য")
    {
        return "bool";
    }

    // FALSE

    if (node->name == "মিথ্যা")
    {
        return "bool";
    }

    // IDENTIFIER

    if (node->children.empty())
    {
        Symbol *symbol =
            symbolTable.lookup(node->name);

        if (symbol == nullptr)
        {
            errors.push_back(
                "Variable '" +
                node->name +
                "' is not declared.");

            return "unknown";
        }

        if (!symbol->initialized)
        {
            errors.push_back(
                "Variable '" +
                node->name +
                "' is not initialized.");

            return symbol->type;
        }

        return symbol->type;
    }

    // UNARY MINUS

    if (
        node->name == "MINUS" &&
        node->children.size() == 1)
    {
        string type =
            analyzeExpression(
                node->children[0]);

        if (type != "int" &&
            type != "unknown")
        {
            errors.push_back(
                "Unary minus can only be used with integer.");
        }

        return "int";
    }

    // BINARY OPERATIONS

    if (node->children.size() >= 2)
    {
        string leftType =
            analyzeExpression(
                node->children[0]);

        string rightType =
            analyzeExpression(
                node->children[1]);

        // ARITHMETIC

        if (
            node->name == "PLUS" ||
            node->name == "MINUS" ||
            node->name == "STAR" ||
            node->name == "SLASH")
        {
            if (
                leftType != "int" &&
                leftType != "unknown")
            {
                errors.push_back(
                    "Left side of arithmetic operation must be integer.");
            }

            if (
                rightType != "int" &&
                rightType != "unknown")
            {
                errors.push_back(
                    "Right side of arithmetic operation must be integer.");
            }

            return "int";
        }

        // COMPARISON

        if (
            node->name == "LESS" ||
            node->name == "LESS_EQUAL" ||
            node->name == "GREATER" ||
            node->name == "GREATER_EQUAL")
        {
            if (
                leftType != "int" &&
                leftType != "unknown")
            {
                errors.push_back(
                    "Comparison requires integer operands.");
            }

            if (
                rightType != "int" &&
                rightType != "unknown")
            {
                errors.push_back(
                    "Comparison requires integer operands.");
            }

            return "bool";
        }

        // EQUALITY

        if (
            node->name == "EQUAL" ||
            node->name == "NOT_EQUAL")
        {
            if (
                leftType != "unknown" &&
                rightType != "unknown" &&
                leftType != rightType)
            {
                errors.push_back(
                    "Both sides of equality must have the same type.");
            }

            return "bool";
        }
    }

    return "unknown";
}

// DECLARATION

void SemanticAnalyzer::analyzeDeclaration(
    const shared_ptr<Node> &node)
{
    if (node == nullptr ||
        node->children.size() < 2)
    {
        return;
    }

    string type;

    if (node->children[0]->name == "পূর্ণ")
    {
        type = "int";
    }
    else if (
        node->children[0]->name ==
        "সত্যমিথ্যা")
    {
        type = "bool";
    }
    else
    {
        errors.push_back(
            "Unknown data type.");

        return;
    }

    string variable =
        node->children[1]->name;

    bool initialized = false;

    if (node->children.size() >= 3)
    {
        string valueType =
            analyzeExpression(
                node->children[2]);

        if (
            valueType != "unknown" &&
            valueType != type)
        {
            errors.push_back(
                "Type mismatch in declaration of '" +
                variable +
                "'. Expected " +
                type +
                ".");
        }
        else if (valueType == type)
        {
            initialized = true;
        }
    }

    if (!symbolTable.define(
            variable,
            type,
            initialized))
    {
        errors.push_back(
            "Variable '" +
            variable +
            "' is already declared.");
    }
}

// ASSIGNMENT

void SemanticAnalyzer::analyzeAssignment(
    const shared_ptr<Node> &node)
{
    if (node == nullptr ||
        node->children.size() < 2)
    {
        return;
    }

    string variable =
        node->children[0]->name;

    Symbol *symbol =
        symbolTable.lookup(variable);

    if (symbol == nullptr)
    {
        errors.push_back(
            "Variable '" +
            variable +
            "' is not declared.");

        analyzeExpression(
            node->children[1]);

        return;
    }

    string valueType =
        analyzeExpression(
            node->children[1]);

    if (
        valueType != "unknown" &&
        valueType != symbol->type)
    {
        errors.push_back(
            "Type mismatch in assignment to '" +
            variable +
            "'.");
    }
    else if (valueType == symbol->type)
    {
        symbolTable.update(variable);
    }
}

// PRINT

void SemanticAnalyzer::analyzePrint(
    const shared_ptr<Node> &node)
{
    if (node == nullptr ||
        node->children.empty())
    {
        return;
    }

    analyzeExpression(
        node->children[0]);
}

// IF

void SemanticAnalyzer::analyzeIf(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return;
    }

    for (const auto &child :
         node->children)
    {
        if (child->name == "Condition")
        {
            if (!child->children.empty())
            {
                string type =
                    analyzeExpression(
                        child->children[0]);

                if (
                    type != "bool" &&
                    type != "unknown")
                {
                    errors.push_back(
                        "IF condition must be boolean.");
                }
            }
        }
        else if (
            child->name == "Then" ||
            child->name == "Else")
        {
            if (!child->children.empty())
            {
                analyzeBlock(
                    child->children[0]);
            }
        }
    }
}

// WHILE

void SemanticAnalyzer::analyzeWhile(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return;
    }

    for (const auto &child :
         node->children)
    {
        if (child->name == "Condition")
        {
            if (!child->children.empty())
            {
                string type =
                    analyzeExpression(
                        child->children[0]);

                if (
                    type != "bool" &&
                    type != "unknown")
                {
                    errors.push_back(
                        "WHILE condition must be boolean.");
                }
            }
        }
        else if (child->name == "Body")
        {
            if (!child->children.empty())
            {
                analyzeBlock(
                    child->children[0]);
            }
        }
    }
}

// BLOCK

void SemanticAnalyzer::analyzeBlock(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return;
    }

    for (const auto &child :
         node->children)
    {
        analyzeStatement(child);
    }
}

// STATEMENT

void SemanticAnalyzer::analyzeStatement(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->name == "Declaration")
    {
        analyzeDeclaration(node);
    }
    else if (node->name == "Assignment")
    {
        analyzeAssignment(node);
    }
    else if (node->name == "Print")
    {
        analyzePrint(node);
    }
    else if (node->name == "If")
    {
        analyzeIf(node);
    }
    else if (node->name == "While")
    {
        analyzeWhile(node);
    }
    else if (node->name == "Block")
    {
        analyzeBlock(node);
    }
}

// ANALYZE

void SemanticAnalyzer::analyze(
    const shared_ptr<Node> &root)
{
    errors.clear();

    if (root == nullptr)
    {
        errors.push_back(
            "Parse tree is empty.");

        return;
    }

    for (const auto &child :
         root->children)
    {
        analyzeStatement(child);
    }
}

// HAS ERRORS

bool SemanticAnalyzer::hasErrors()
{
    return !errors.empty();
}

// PRINT ERRORS

void SemanticAnalyzer::printErrors()
{
    if (errors.empty())
    {
        cout << "\nSEMANTIC ANALYSIS\n";
        cout << "No semantic errors found.\n";
        return;
    }

    cout << "\nSEMANTIC ERRORS\n";

    for (const string &error : errors)
    {
        cout << error << "\n";
    }
}

// PRINT SYMBOL TABLE

void SemanticAnalyzer::printSymbolTable()
{
    symbolTable.printSymbols();
}