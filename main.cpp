#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "tac.h"
#include "target_code.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

// READ FILE

string readFile(
    const string &fileName)
{
    ifstream file(
        fileName,
        ios::binary);

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
    cout
        << "          Bangla Compiler\n";

    // READ SOURCE

    string sourceCode =
        readFile("banglatext.bng");

    if (sourceCode.empty())
    {
        cout
            << "Error: banglatext.bng file পাওয়া যায়নি।\n";

        return 1;
    }

    cout
        << "Source file loaded successfully.\n";

    // LEXER

    Lexer lexer(sourceCode);

    vector<Token> tokens =
        lexer.tokenize();

    cout << "\n";
    cout << "TOKENS\n";

    for (const Token &token : tokens)
    {
        cout
            << "< "
            << tokenName(token.type)
            << " , "
            << token.value
            << " , line "
            << token.line
            << " >\n";
    }

    // PARSER

    Parser parser(tokens);

    shared_ptr<Node> parseTree =
        parser.parse();

    cout << "\n";
    cout << "PARSE TREE\n";

    parser.printTree(parseTree);

    // SEMANTIC ANALYSIS

    SemanticAnalyzer semantic;

    semantic.analyze(parseTree);

    semantic.printSymbolTable();

    semantic.printErrors();

    if (semantic.hasErrors())
    {
        cout
            << "\nCompilation stopped because of semantic errors.\n";

        return 1;
    }

    // TAC

    TACGenerator tacGenerator;

    vector<string> tac =
        tacGenerator.generate(
            parseTree);

    cout << "\n";
    cout << "THREE ADDRESS CODE\n";

    for (const string &instruction : tac)
    {
        cout << instruction << "\n";
    }

    // TARGET CODE

    TargetCode targetCode(tac);

    string outputFile =
        "output.py";

    if (!targetCode.save(outputFile))
    {
        cout
            << "\nError: Could not create output.py\n";

        return 1;
    }

    cout << "\n";
    cout << "TARGET PYTHON CODE\n";

    cout
        << targetCode.generateDisplay();

    cout
        << "\nPython target code generated: "
        << outputFile
        << "\n";

    // RUN PYTHON

    targetCode.run(outputFile);

    cout << "\n";
    cout << "Compilation completed.\n";

    return 0;
}