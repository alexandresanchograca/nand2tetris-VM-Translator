// VM Translator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CodeWriter.h"

int main()
{
    std::string filename;
    std::string outFile;
    std::vector < std::string > parsedStrings; //Storing the file input in memory
    std::vector<std::string> translatedAssembly; //Storing the  translated assembly code

    std::cout << "Type the file name that you want to translate to assembly or enter [2] for translating the whole directory): ";
    std::cin >> filename;

    std::cout << "Converting " << filename << " to assembly code for our hack computer..." << std::endl;

    Parser parseFile(filename);

    auto commandArray = parseFile.getCommands();

    std::cout << "Type the output file name: ";
    std::cin >> outFile;

    CodeWriter writeFile(outFile, &commandArray);

    std::cout << "Finished sucessfully, please check the output file." << std::endl;
    return 1;
}