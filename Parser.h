#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <vector>
#include "UtilData.h"
#include <filesystem>

class Parser
{
public:
    Parser(std::string filename) {
        if(filename != "2")
            ReadFile(filename, filename);
        else
            ReadDirectory();
    }

    std::vector<struct Command> getCommands() {
        return this->commands;
    }

private:
    std::vector<struct Command> commands;

    int ReadFile(std::string filename, std::string entryFile) {
        //Opens the file
        
        //std::ifstream file("C:\\Users\\Alexandre\\source\\repos\\VM Translator P8\\BasicLoop.vm"); //Abs path for debug
        
        std::ifstream file(filename);

        if (file.is_open()) {
            std::string line;
            int lineCount = 0;
            while (std::getline(file, line)) { //Checks until it hits an \n
                //Clear all the comments and indentation and assign it to a string vector
                if (line.find("//") != std::string::npos)
                    line.erase(line.find("//"));

                if (line.empty()) //Ignores white lines
                    continue;

                //line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); //Removes whitespaces

                Command temp;
                this->tokenString(line, &temp);
                this->checkCommandType(&temp);
                this->assignAdressVariable(&temp, entryFile);

                commands.push_back(temp);
            }
            file.close();
            return 1;
        }
        else {
            std::cout << "Can't open the file, please try again!" << std::endl;
            return 0;
        }
    }

    void ReadDirectory() {
        std::string directory_path = std::filesystem::current_path().string(); // Get the current working directory
        std::vector<Command> allCommands;

        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            if (entry.path().extension() == ".vm") {
                std::string filename = entry.path().string();
                std::string entryFileName = entry.path().filename().replace_extension("").string();

                ReadFile(filename, entryFileName);
            }
        }
    }

    void tokenString(std::string str, struct Command* outCmd) {     
        // Vector of string to save tokens
        std::vector <std::string> tokens;

        // stringstream class check1
        std::stringstream check1(str);

        std::string intermediate;

        // Tokenizing w.r.t. space ' '
        for (int i = 0; i < 3; i++) {
            getline(check1, intermediate, ' ');

            outCmd->command[i] = intermediate;
        }
    }

    void assignAdressVariable(struct Command* com, std::string filename) {
        if (com->commandType == C_POP || com->commandType == C_PUSH) {
            if (com->command[1].compare("local") == 0)
                com->adressVariable = "@LCL";
            else if (com->command[1].compare("argument") == 0)
                com->adressVariable = "@ARG";
            else if (com->command[1].compare("temp") == 0)
                com->adressVariable = "@R5";
            else if (com->command[1].compare("this") == 0)
                com->adressVariable = "@THIS";
            else if (com->command[1].compare("that") == 0)
                com->adressVariable = "@THAT";
            else if (com->command[1].compare("pointer") == 0) {
                if (com->command[2].compare("0") == 0)
                    com->adressVariable = "@THIS";
                else
                    com->adressVariable = "@THAT";
            }
            else if (com->command[1].compare("static") == 0) {
                com->adressVariable = "@" + filename; //This is the staticBaseAddrss for the file that is being parsed
            }
            else {
                com->adressVariable = "@" + com->command[2]; //constant
            }
        }
    }

    void checkCommandType(struct Command *com) {
        auto it = commandTable.find(com->command[0]);
        if(it != commandTable.end()) {
            com->commandType = it->second;
        }
        return;
    }

};

