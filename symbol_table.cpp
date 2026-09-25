#include "symbol_table.h"

#include <iostream>

using namespace std;

// DEFINE

bool SymbolTable::define(
    const string &name,
    const string &type,
    bool initialized)
{
    if (variables.find(name) != variables.end())
    {
        return false;
    }

    variables.emplace(
        name,
        Symbol(
            name,
            type,
            initialized));

    return true;
}

// UPDATE

bool SymbolTable::update(
    const string &name)
{
    auto it = variables.find(name);

    if (it == variables.end())
    {
        return false;
    }

    it->second.initialized = true;

    return true;
}

// LOOKUP

Symbol *SymbolTable::lookup(
    const string &name)
{
    auto it = variables.find(name);

    if (it == variables.end())
    {
        return nullptr;
    }

    return &(it->second);
}

// IS DEFINED

bool SymbolTable::isDefined(
    const string &name)
{
    return variables.find(name) != variables.end();
}

// IS INITIALIZED

bool SymbolTable::isInitialized(
    const string &name)
{
    auto it = variables.find(name);

    if (it == variables.end())
    {
        return false;
    }

    return it->second.initialized;
}

// PRINT SYMBOL TABLE

void SymbolTable::printSymbols()
{
    cout << "\n";
    cout << "SYMBOL TABLE\n";
    cout << "---------------------------------------------\n";
    cout << "Name\t\tType\t\tStatus\n";
    cout << "---------------------------------------------\n";

    for (const auto &item : variables)
    {
        const Symbol &symbol = item.second;

        cout << symbol.name
             << "\t\t"
             << symbol.type
             << "\t\t"
             << (symbol.initialized
                     ? "initialized"
                     : "uninitialized")
             << "\n";
    }

    cout << "---------------------------------------------\n";
}