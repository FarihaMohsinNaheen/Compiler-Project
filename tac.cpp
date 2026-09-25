#include "tac.h"

using namespace std;

// TAC COPY

string TACCopy::toString() const
{
    return dest + " = " + src;
}

// TAC BINARY OPERATION

string TACBinOp::toString() const
{
    return dest + " = " +
           left + " " +
           op + " " +
           right;
}

// TAC PRINT

string TACPrint::toString() const
{
    return "print " + src;
}

// CONSTRUCTOR

TACGenerator::TACGenerator()
{
    tempCount = 0;
}

// NEW TEMP

string TACGenerator::newTemp()
{
    string temp =
        "t" + to_string(tempCount);

    tempCount++;

    return temp;
}

// MAP OPERATOR

string TACGenerator::mapOperator(
    const string &op)
{
    if (op == "PLUS")
        return "+";

    if (op == "MINUS")
        return "-";

    if (op == "STAR")
        return "*";

    if (op == "SLASH")
        return "/";

    if (op == "EQUAL")
        return "==";

    if (op == "NOT_EQUAL")
        return "!=";

    if (op == "LESS")
        return "<";

    if (op == "LESS_EQUAL")
        return "<=";

    if (op == "GREATER")
        return ">";

    if (op == "GREATER_EQUAL")
        return ">=";

    return op;
}

// GENERATE EXPRESSION

string TACGenerator::generateExpression(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return "";
    }

    // TRUE

    if (node->name == "সত্য")
    {
        return "True";
    }

    // FALSE

    if (node->name == "মিথ্যা")
    {
        return "False";
    }

    // UNARY MINUS

    if (
        node->name == "MINUS" &&
        node->children.size() == 1)
    {
        string value =
            generateExpression(
                node->children[0]);

        string temp =
            newTemp();

        TACBinOp instruction(
            temp,
            "0",
            "-",
            value);

        instructions.push_back(
            instruction.toString());

        return temp;
    }

    // BINARY OPERATION

    if (
        node->name == "PLUS" ||
        node->name == "MINUS" ||
        node->name == "STAR" ||
        node->name == "SLASH" ||
        node->name == "EQUAL" ||
        node->name == "NOT_EQUAL" ||
        node->name == "LESS" ||
        node->name == "LESS_EQUAL" ||
        node->name == "GREATER" ||
        node->name == "GREATER_EQUAL")
    {
        if (node->children.size() < 2)
        {
            return "";
        }

        string left =
            generateExpression(
                node->children[0]);

        string right =
            generateExpression(
                node->children[1]);

        string temp =
            newTemp();

        TACBinOp instruction(
            temp,
            left,
            mapOperator(node->name),
            right);

        instructions.push_back(
            instruction.toString());

        return temp;
    }

    // NUMBER OR IDENTIFIER

    return node->name;
}

// GENERATE CONDITION

string TACGenerator::generateCondition(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return "";
    }

    // TRUE

    if (node->name == "সত্য")
    {
        return "True";
    }

    // FALSE

    if (node->name == "মিথ্যা")
    {
        return "False";
    }

    // COMPARISON

    if (
        node->children.size() == 2 &&
        (node->name == "EQUAL" ||
         node->name == "NOT_EQUAL" ||
         node->name == "LESS" ||
         node->name == "LESS_EQUAL" ||
         node->name == "GREATER" ||
         node->name == "GREATER_EQUAL"))
    {
        string left =
            generateCondition(
                node->children[0]);

        string right =
            generateCondition(
                node->children[1]);

        return "(" +
               left +
               " " +
               mapOperator(node->name) +
               " " +
               right +
               ")";
    }

    // ARITHMETIC CONDITION

    if (
        node->children.size() == 2 &&
        (node->name == "PLUS" ||
         node->name == "MINUS" ||
         node->name == "STAR" ||
         node->name == "SLASH"))
    {
        string left =
            generateCondition(
                node->children[0]);

        string right =
            generateCondition(
                node->children[1]);

        return "(" +
               left +
               " " +
               mapOperator(node->name) +
               " " +
               right +
               ")";
    }

    // UNARY MINUS

    if (
        node->name == "MINUS" &&
        node->children.size() == 1)
    {
        return "(-" +
               generateCondition(
                   node->children[0]) +
               ")";
    }

    // NUMBER OR VARIABLE

    return node->name;
}

// GENERATE BLOCK

void TACGenerator::generateBlock(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->children.empty())
    {
        instructions.push_back("PASS");

        return;
    }

    for (const auto &child :
         node->children)
    {
        generateStatement(child);
    }
}

// GENERATE IF

void TACGenerator::generateIf(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return;
    }

    shared_ptr<Node> condition = nullptr;
    shared_ptr<Node> thenBlock = nullptr;
    shared_ptr<Node> elseBlock = nullptr;

    for (const auto &child :
         node->children)
    {
        if (child->name == "Condition")
        {
            if (!child->children.empty())
            {
                condition =
                    child->children[0];
            }
        }
        else if (child->name == "Then")
        {
            if (!child->children.empty())
            {
                thenBlock =
                    child->children[0];
            }
        }
        else if (child->name == "Else")
        {
            if (!child->children.empty())
            {
                elseBlock =
                    child->children[0];
            }
        }
    }

    if (condition == nullptr)
    {
        return;
    }

    // IF

    instructions.push_back(
        "IF " +
        generateCondition(condition));

    // THEN

    generateBlock(thenBlock);

    // ELSE

    if (elseBlock != nullptr)
    {
        instructions.push_back("ELSE");

        generateBlock(elseBlock);
    }

    // END IF

    instructions.push_back("ENDIF");
}

// GENERATE WHILE

void TACGenerator::generateWhile(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return;
    }

    shared_ptr<Node> condition = nullptr;
    shared_ptr<Node> body = nullptr;

    for (const auto &child :
         node->children)
    {
        if (child->name == "Condition")
        {
            if (!child->children.empty())
            {
                condition =
                    child->children[0];
            }
        }
        else if (child->name == "Body")
        {
            if (!child->children.empty())
            {
                body =
                    child->children[0];
            }
        }
    }

    if (condition == nullptr)
    {
        return;
    }

    // WHILE

    instructions.push_back(
        "WHILE " +
        generateCondition(condition));

    // BODY

    generateBlock(body);

    // END WHILE

    instructions.push_back(
        "ENDWHILE");
}

// GENERATE STATEMENT

void TACGenerator::generateStatement(
    const shared_ptr<Node> &node)
{
    if (node == nullptr)
    {
        return;
    }

    // DECLARATION

    if (node->name == "Declaration")
    {
        if (node->children.size() >= 3)
        {
            string variable =
                node->children[1]->name;

            string value =
                generateExpression(
                    node->children[2]);

            TACCopy instruction(
                variable,
                value);

            instructions.push_back(
                instruction.toString());
        }
        else
        {
            instructions.push_back("PASS");
        }
    }

    // ASSIGNMENT

    else if (node->name == "Assignment")
    {
        if (node->children.size() >= 2)
        {
            string variable =
                node->children[0]->name;

            string value =
                generateExpression(
                    node->children[1]);

            TACCopy instruction(
                variable,
                value);

            instructions.push_back(
                instruction.toString());
        }
    }

    // PRINT

    else if (node->name == "Print")
    {
        if (!node->children.empty())
        {
            string value =
                generateExpression(
                    node->children[0]);

            TACPrint instruction(value);

            instructions.push_back(
                instruction.toString());
        }
    }

    // IF

    else if (node->name == "If")
    {
        generateIf(node);
    }

    // WHILE

    else if (node->name == "While")
    {
        generateWhile(node);
    }

    // BLOCK

    else if (node->name == "Block")
    {
        generateBlock(node);
    }
}

// GENERATE

vector<string> TACGenerator::generate(
    const shared_ptr<Node> &root)
{
    instructions.clear();

    tempCount = 0;

    if (root == nullptr)
    {
        return instructions;
    }

    for (const auto &child :
         root->children)
    {
        generateStatement(child);
    }

    return instructions;
}