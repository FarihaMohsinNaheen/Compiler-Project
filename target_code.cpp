#include "target_code.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// CONSTRUCTOR

TargetCode::TargetCode(
    const vector<string> &instructions)
{
    tac = instructions;
}

// CONVERT BANGLA DIGITS TO ENGLISH DIGITS

string convertBanglaDigits(
    const string &text)
{
    string result;

    size_t i = 0;

    while (i < text.size())
    {
        unsigned char ch =
            static_cast<unsigned char>(
                text[i]);

        // ASCII digit

        if (ch >= '0' && ch <= '9')
        {
            result += text[i];

            i++;

            continue;
        }

        // Bangla digit

        if (
            i + 2 < text.size() &&
            static_cast<unsigned char>(
                text[i]) == 0xE0 &&
            static_cast<unsigned char>(
                text[i + 1]) == 0xA7)
        {
            unsigned char digit =
                static_cast<unsigned char>(
                    text[i + 2]);

            if (
                digit >= 0xA6 &&
                digit <= 0xAF)
            {
                result +=
                    char(
                        '0' +
                        (digit - 0xA6));

                i += 3;

                continue;
            }
        }

        // Normal character

        result += text[i];

        i++;
    }

    return result;
}

// CONVERT ENGLISH NUMBERS TO BANGLA NUMBERS

string convertNumbersToBangla(
    const string &text)
{
    string result;

    static const string banglaDigits[] =
        {
            "০",
            "১",
            "২",
            "৩",
            "৪",
            "৫",
            "৬",
            "৭",
            "৮",
            "৯"};

    size_t i = 0;

    while (i < text.size())
    {
        char ch = text[i];

        if (
            ch >= '0' &&
            ch <= '9')
        {
            bool partOfIdentifier = false;

            if (i > 0)
            {
                char previous = text[i - 1];

                if (
                    (previous >= 'a' &&
                     previous <= 'z') ||

                    (previous >= 'A' &&
                     previous <= 'Z') ||

                    previous == '_')
                {
                    partOfIdentifier = true;
                }
            }

            if (partOfIdentifier)
            {
                result += ch;
            }
            else
            {
                result +=
                    banglaDigits[ch - '0'];
            }

            i++;

            continue;
        }

        result += ch;

        i++;
    }

    return result;
}

// GENERATE EXECUTABLE PYTHON

string TargetCode::generate()
{
    stringstream output;

    int indentation = 0;

    output
        << "# বাংলা Compiler generated Python code\n\n";

    for (const string &instruction : tac)
    {
        // PASS

        if (instruction == "PASS")
        {
            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output << "pass\n";

            continue;
        }

        // IF

        if (instruction.rfind("IF ", 0) == 0)
        {
            string condition =
                instruction.substr(3);

            // Convert Bangla numbers to
            // English numbers for Python.

            condition =
                convertBanglaDigits(condition);

            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output
                << "if "
                << condition
                << ":\n";

            indentation++;

            continue;
        }

        // ELSE

        if (instruction == "ELSE")
        {
            if (indentation > 0)
            {
                indentation--;
            }

            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output << "else:\n";

            indentation++;

            continue;
        }

        // ENDIF

        if (instruction == "ENDIF")
        {
            if (indentation > 0)
            {
                indentation--;
            }

            continue;
        }

        // WHILE

        if (instruction.rfind("WHILE ", 0) == 0)
        {
            string condition =
                instruction.substr(6);

            // Convert Bangla numbers to
            // English numbers for Python.

            condition =
                convertBanglaDigits(condition);

            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output
                << "while "
                << condition
                << ":\n";

            indentation++;

            continue;
        }

        // ENDWHILE

        if (instruction == "ENDWHILE")
        {
            if (indentation > 0)
            {
                indentation--;
            }

            continue;
        }

        // PRINT

        if (instruction.rfind("print ", 0) == 0)
        {
            string value =
                instruction.substr(6);

            value =
                convertBanglaDigits(value);

            // Convert Bangla boolean values.

            if (value == "সত্য")
            {
                value = "True";
            }
            else if (value == "মিথ্যা")
            {
                value = "False";
            }

            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output
                << "print("
                << value
                << ")\n";

            continue;
        }

        // NORMAL TAC

        string pythonInstruction =
            convertBanglaDigits(
                instruction);

        // Convert boolean values.

        size_t equalPosition =
            pythonInstruction.find(" = ");

        if (equalPosition != string::npos)
        {
            string rightSide =
                pythonInstruction.substr(
                    equalPosition + 3);

            if (rightSide == "সত্য")
            {
                pythonInstruction =
                    pythonInstruction.substr(
                        0,
                        equalPosition + 3) +
                    "True";
            }
            else if (rightSide == "মিথ্যা")
            {
                pythonInstruction =
                    pythonInstruction.substr(
                        0,
                        equalPosition + 3) +
                    "False";
            }
        }

        for (int i = 0;
             i < indentation;
             i++)
        {
            output << "    ";
        }

        output
            << pythonInstruction
            << "\n";
    }

    return output.str();
}

// GENERATE DISPLAY CODE

string TargetCode::generateDisplay()
{
    string pythonCode =
        generate();

    stringstream output;

    int indentation = 0;

    output
        << "# বাংলা Compiler generated Python code\n\n";

    for (const string &instruction : tac)
    {

        // PASS

        if (instruction == "PASS")
        {
            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output << "pass\n";

            continue;
        }

        // IF

        if (instruction.rfind("IF ", 0) == 0)
        {
            string condition =
                instruction.substr(3);

            condition =
                convertNumbersToBangla(
                    convertBanglaDigits(condition));

            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output
                << "if "
                << condition
                << ":\n";

            indentation++;

            continue;
        }

        // ELSE
        if (instruction == "ELSE")
        {
            if (indentation > 0)
            {
                indentation--;
            }

            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output << "else:\n";

            indentation++;

            continue;
        }

        // ENDIF

        if (instruction == "ENDIF")
        {
            if (indentation > 0)
            {
                indentation--;
            }

            continue;
        }

        // WHILE

        if (instruction.rfind("WHILE ", 0) == 0)
        {
            string condition =
                instruction.substr(6);

            condition =
                convertNumbersToBangla(
                    convertBanglaDigits(condition));

            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output
                << "while "
                << condition
                << ":\n";

            indentation++;

            continue;
        }

        // ENDWHILE

        if (instruction == "ENDWHILE")
        {
            if (indentation > 0)
            {
                indentation--;
            }

            continue;
        }

        // PRINT

        if (instruction.rfind("print ", 0) == 0)
        {
            string value =
                instruction.substr(6);

            value =
                convertNumbersToBangla(
                    convertBanglaDigits(value));

            for (int i = 0;
                 i < indentation;
                 i++)
            {
                output << "    ";
            }

            output
                << "print("
                << value
                << ")\n";

            continue;
        }

        // NORMAL TAC

        string displayInstruction =
            convertNumbersToBangla(
                convertBanglaDigits(
                    instruction));

        // Restore temporary variable digits.

        static const string banglaDigits[] =
            {
                "০",
                "১",
                "২",
                "৩",
                "৪",
                "৫",
                "৬",
                "৭",
                "৮",
                "৯"};

        static const char englishDigits[] =
            {
                '0',
                '1',
                '2',
                '3',
                '4',
                '5',
                '6',
                '7',
                '8',
                '9'};

        for (int digit = 0;
             digit <= 9;
             digit++)
        {
            size_t position = 0;

            while (
                (position =
                     displayInstruction.find(
                         banglaDigits[digit],
                         position)) != string::npos)
            {
                if (position > 0)
                {
                    char previous =
                        displayInstruction[position - 1];

                    if (previous == 't')
                    {
                        displayInstruction.replace(
                            position,
                            banglaDigits[digit].size(),
                            string(
                                1,
                                englishDigits[digit]));

                        position += 1;

                        continue;
                    }
                }

                position +=
                    banglaDigits[digit].size();
            }
        }

        for (int i = 0;
             i < indentation;
             i++)
        {
            output << "    ";
        }

        output
            << displayInstruction
            << "\n";
    }

    return output.str();
}

// SAVE

bool TargetCode::save(
    const string &fileName)
{
    ofstream file(
        fileName,
        ios::binary);

    if (!file)
    {
        return false;
    }

    // IMPORTANT:
    // Save executable Python version.

    string pythonCode =
        generate();

    file << pythonCode;

    file.close();

    return true;
}

// RUN PYTHON

void TargetCode::run(
    const string &fileName)
{
    cout << "\n";
    cout << "PROGRAM OUTPUT\n";

    string command =
        "python \"" +
        fileName +
        "\"";

    int result =
        system(command.c_str());

    // Try Windows Python launcher

    if (result != 0)
    {
        cout
            << "\n'python' command failed. Trying 'py'...\n";

        string command2 =
            "py \"" +
            fileName +
            "\"";

        result =
            system(command2.c_str());
    }

    if (result != 0)
    {
        cout
            << "\nCould not execute output.py.\n";

        cout
            << "Try running: python output.py\n";
    }
}