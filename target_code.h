#ifndef TARGET_CODE_H
#define TARGET_CODE_H

#include <string>
#include <vector>

using namespace std;

class TargetCode
{
private:
    vector<string> tac;

public:
    TargetCode(
        const vector<string> &instructions);

    // Generate executable Python code
    string generate();

    // Generate display version with Bangla numbers
    string generateDisplay();

    bool save(
        const string &fileName);

    void run(
        const string &fileName);
};

#endif