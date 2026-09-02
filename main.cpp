#include "lexer.h"
#include "parser.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

// READ FILE

string readFile(const string &fileName)
{
    ifstream file(fileName, ios::binary);

    if (!file)
    {
        return "";
    }

    return string(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>());
}

// MAIN

int main()
{
    cout << "          Bangla Compiler\n";

    // READ SOURCE FILE

    string sourceCode = readFile("banglatext.bng");

    if (sourceCode.empty())
    {
        cout << "Error: banglatext.bng file পাওয়া যায়নি।\n";

        return 1;
    }

    cout << "Source file loaded successfully.\n\n";

    // LEXER

    Lexer lexer(sourceCode);

    vector<Token> tokens = lexer.tokenize();

    // PRINT TOKENS

    cout << " TOKENS\n\n";

    for (const Token &token : tokens)
    {
        cout << "< "
             << tokenName(token.type)
             << " , "
             << token.value
             << " , line "
             << token.line
             << " >\n";
    }

    // PARSER

    Parser parser(tokens);

    shared_ptr<Node> parseTree = parser.parse();

    // PRINT PARSE TREE

    cout << "\n";
    cout << " PARSE TREE\n\n";

    parser.printTree(parseTree);

    cout << "\n";

    cout << "Compilation completed.\n";

    return 0;
}