#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <map>

using namespace std;

struct Symbol
{
    string name;
    string type;
    bool initialized;

    Symbol(
        const string &n,
        const string &t,
        bool init = false)
    {
        name = n;
        type = t;
        initialized = init;
    }
};

class SymbolTable
{
private:
    map<string, Symbol> variables;

public:
    bool define(
        const string &name,
        const string &type,
        bool initialized = false);

    bool update(
        const string &name);

    Symbol *lookup(
        const string &name);

    bool isDefined(
        const string &name);

    bool isInitialized(
        const string &name);

    void printSymbols();
};

#endif