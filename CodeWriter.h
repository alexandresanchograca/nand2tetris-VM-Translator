#pragma once
#include "Parser.h"

class CodeWriter
{
private:
    std::vector<struct Command> commands;

    std::vector < std::string > outputStrings;
    unsigned long commandNum = 0;
    unsigned long labelNum = 0;
    std::vector <struct StaticVars> staticData;
    unsigned int staticCounter = 16;
public:
    CodeWriter(std::string outFilename, std::vector<struct Command>* commandList) {
        this->commands = *commandList;

        this->WriteInit();

        for (int i = 0; i < commands.size(); i++) {
            if (commands[i].commandType == C_ARITHMETIC) {
                outputStrings.push_back("//" + commands[i].command[0]);
                WriteArithmetic(commands[i].command[0]);
            }
            else if (commands[i].commandType == C_PUSH) {
                outputStrings.push_back("//" + commands[i].command[0] + " " + commands[i].command[1] + " " + commands[i].command[2]);
                WritePushPop(commands[i]);
            }
            else if (commands[i].commandType == C_POP) {
                outputStrings.push_back("//" + commands[i].command[0] + " " + commands[i].command[1] + " " + commands[i].command[2]);
                WritePushPop(commands[i]);
            }
            else if (commands[i].commandType == C_LABEL) {
                outputStrings.push_back("//" + commands[i].command[0] + " " + commands[i].command[1]);
                WriteLabel(commands[i]);
            }
            else if (commands[i].commandType == C_GOTO) {
                outputStrings.push_back("//" + commands[i].command[0] + " " + commands[i].command[1] + " " + commands[i].command[2]);
                WriteGoto(commands[i]);
            }
            else if (commands[i].commandType == C_IF) {
                outputStrings.push_back("//" + commands[i].command[0] + " " + commands[i].command[1] + " " + commands[i].command[2]);
               WriteIfGoto(commands[i]);
            }
            else if (commands[i].commandType == C_FUNCTION) {
                outputStrings.push_back("//" + commands[i].command[0] + " " + commands[i].command[1] + " " + commands[i].command[2]);
                WriteFunction(commands[i]);
            }
            else if (commands[i].commandType == C_CALL) {
                outputStrings.push_back("//" + commands[i].command[0] + " " + commands[i].command[1] + " " + commands[i].command[2]);
                WriteCall(commands[i]);
            }
            else if (commands[i].commandType == C_RETURN) {
                outputStrings.push_back("//" + commands[i].command[0] + " " + commands[i].command[1] + " " + commands[i].command[2]);
                WriteReturn(commands[i]);
            }
        }

        WriteFile(outFilename);
    }

    void WriteArithmetic(std::string commandName) {

        if (commandName == "add") { //add the previous 2 values of the stack
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("D=M");
            outputStrings.push_back("@SP"); //SP --
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");

            outputStrings.push_back("M=M+D"); // M = X + Y
        }
        else if (commandName == "sub") {
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("D=M");
            outputStrings.push_back("@SP"); //SP --
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");

            outputStrings.push_back("M=M-D"); // M = X + Y
        }
        else if (commandName == "neg") {
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("M=!M");
            outputStrings.push_back("M=M+1");
        }
        else if (commandName == "eq") {
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("D=M");
            outputStrings.push_back("@SP"); //SP --
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");

            outputStrings.push_back("D=M-D"); //D=a-b a-b == 0? 
            outputStrings.push_back("@TRUE." + std::to_string(commandNum));
            outputStrings.push_back("D;JEQ");

            outputStrings.push_back("@SP"); //if false
            outputStrings.push_back("A=M");
            outputStrings.push_back("M=0"); //0 is false in the VM

            outputStrings.push_back("@CONTINUE." + std::to_string(commandNum));
            outputStrings.push_back("0;JMP");

            outputStrings.push_back("(TRUE." + std::to_string(commandNum) + ")");
            outputStrings.push_back("@SP");
            outputStrings.push_back("A=M");
            outputStrings.push_back("M=-1"); //-1 is true in the VM

            outputStrings.push_back("(CONTINUE." + std::to_string(commandNum) + ")");


            this->commandNum++;
        }
        else if (commandName == "gt") {
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("D=M");
            outputStrings.push_back("@SP"); //SP --
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");

            outputStrings.push_back("D=M-D"); //D=a-b a-b>0? 
            outputStrings.push_back("@TRUE." + std::to_string(commandNum));
            outputStrings.push_back("D;JGT");

            outputStrings.push_back("@SP"); //if false
            outputStrings.push_back("A=M");
            outputStrings.push_back("M=0"); //0 is false in the VM

            outputStrings.push_back("@CONTINUE." + std::to_string(commandNum));
            outputStrings.push_back("0;JMP");

            outputStrings.push_back("(TRUE." + std::to_string(commandNum) + ")");
            outputStrings.push_back("@SP");
            outputStrings.push_back("A=M");
            outputStrings.push_back("M=-1");

            outputStrings.push_back("(CONTINUE." + std::to_string(commandNum) + ")");

            this->commandNum++;
        }
        else if (commandName == "lt") {
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("D=M");
            outputStrings.push_back("@SP"); //SP --
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");

            outputStrings.push_back("D=D-M"); //D=b-a b-a>0? 
            outputStrings.push_back("@TRUE." + std::to_string(commandNum));
            outputStrings.push_back("D;JGT");

            outputStrings.push_back("@SP"); //if false
            outputStrings.push_back("A=M");
            outputStrings.push_back("M=0"); //0 is false in the VM

            outputStrings.push_back("@CONTINUE." + std::to_string(commandNum));
            outputStrings.push_back("0;JMP");

            outputStrings.push_back("(TRUE." + std::to_string(commandNum) + ")");
            outputStrings.push_back("@SP");
            outputStrings.push_back("A=M");
            outputStrings.push_back("M=-1");

            outputStrings.push_back("(CONTINUE." + std::to_string(commandNum) + ")");

            this->commandNum++;
        }
        else if (commandName == "and") { //Bitwise operation
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("D=M");
            outputStrings.push_back("@SP"); //SP --
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");

            outputStrings.push_back("M=M&D"); //Bitwise and
        }
        else if (commandName == "or") {
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("D=M");
            outputStrings.push_back("@SP"); //SP --
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");

            outputStrings.push_back("M=M|D"); //Bitwise and
        }
        else if (commandName == "not") {
            outputStrings.push_back("@SP"); //SP--
            outputStrings.push_back("M=M-1");
            outputStrings.push_back("A=M");
            outputStrings.push_back("M=!M"); //bitwise not
        }

        outputStrings.push_back("@SP"); //SP--
        outputStrings.push_back("M=M+1");
    }

    void WritePushPop(struct Command com) {
        if (com.commandType == C_PUSH) {
            if (com.command[1] == "constant") {
                outputStrings.push_back("@" + com.command[2]);
                outputStrings.push_back("D=A");
                outputStrings.push_back("@SP");
                outputStrings.push_back("A=M");
                outputStrings.push_back("M=D");
                outputStrings.push_back("@SP");
                outputStrings.push_back("M=M+1");
            }
            else if (com.command[1] == "pointer") {
                outputStrings.push_back(com.adressVariable);
                outputStrings.push_back("D=M");
                outputStrings.push_back("@SP");
                outputStrings.push_back("A=M");
                outputStrings.push_back("M=D");
                outputStrings.push_back("@SP");
                outputStrings.push_back("M=M+1");
            }
            else if (com.command[1] == "this" || com.command[1] == "that" || com.command[1] == "argument" || com.command[1] == "local") {
                outputStrings.push_back(com.adressVariable);
                outputStrings.push_back("D=M");
                outputStrings.push_back("@" + com.command[2]);
                outputStrings.push_back("A=D+A");
                outputStrings.push_back("D=M");

                outputStrings.push_back("@SP");
                outputStrings.push_back("A=M");
                outputStrings.push_back("M=D");
                outputStrings.push_back("@SP");
                outputStrings.push_back("M=M+1");
            }
            else if (com.command[1] == "static") {
                //com.adressVariable has the name of the file 
                
                //staticData.push_back(StaticVars())


                outputStrings.push_back(com.adressVariable + com.command[2]); //baseStaticAddrss
                outputStrings.push_back("D=A");
                outputStrings.push_back("D=M");

                outputStrings.push_back("@SP");
                outputStrings.push_back("A=M");
                outputStrings.push_back("M=D");

                outputStrings.push_back("@SP");
                outputStrings.push_back("M=M+1");
            }
            else if (com.command[1] == "temp") {
                outputStrings.push_back(com.adressVariable);
                outputStrings.push_back("D=A");
                outputStrings.push_back("@" + com.command[2]);
                outputStrings.push_back("A=D+A");
                outputStrings.push_back("D=M");

                outputStrings.push_back("@SP");
                outputStrings.push_back("A=M");
                outputStrings.push_back("M=D");
                outputStrings.push_back("@SP");
                outputStrings.push_back("M=M+1");
            }
            else {
                outputStrings.push_back(com.adressVariable);
                outputStrings.push_back("D=A");
                outputStrings.push_back("@" + com.command[2]);
                outputStrings.push_back("D=D+A");
                outputStrings.push_back("@SP");
                outputStrings.push_back("A=M");
                outputStrings.push_back("M=D");
                outputStrings.push_back("@SP");
                outputStrings.push_back("M=M+1");
            }
        }
        else { //POP
            if (com.command[1] == "pointer") {
                outputStrings.push_back("@SP"); //SP--
                outputStrings.push_back("M=M-1");

                outputStrings.push_back("A=M");
                outputStrings.push_back("D=M"); //D = *SP

                outputStrings.push_back(com.adressVariable);
                outputStrings.push_back("M=D"); //this or that = *SP 
            }
            else if (com.command[1] == "temp") {
                outputStrings.push_back(com.adressVariable);
                outputStrings.push_back("D=A");
                outputStrings.push_back("@" + com.command[2]);
                outputStrings.push_back("D=D+A");
                outputStrings.push_back("@R13"); //For addr storage
                outputStrings.push_back("M=D");  //addr = localBaseAddress + offset

                outputStrings.push_back("@SP"); //SP--
                outputStrings.push_back("M=M-1");

                outputStrings.push_back("A=M");
                outputStrings.push_back("D=M"); //D = *SP

                outputStrings.push_back("@R13"); //addr = localBaseAddress + offset
                outputStrings.push_back("A=M"); //*addr
                outputStrings.push_back("M=D"); //*addr = *SP 
            }
            else if (com.command[1] == "static") {
                outputStrings.push_back(com.adressVariable + com.command[2]); //baseStaticAddrss
                outputStrings.push_back("D=A");

                outputStrings.push_back("@R13"); //For addr storage
                outputStrings.push_back("M=D");  //addr = localBaseAddress + offset

                outputStrings.push_back("@SP"); //SP--
                outputStrings.push_back("M=M-1");

                outputStrings.push_back("A=M");
                outputStrings.push_back("D=M"); //D = *SP

                outputStrings.push_back("@R13"); //addr = localBaseAddress + offset
                outputStrings.push_back("A=M"); //*addr
                outputStrings.push_back("M=D"); //*addr = *SP 
            }
            else if (com.command[1] != "constant") {
                outputStrings.push_back(com.adressVariable);
                outputStrings.push_back("D=M");
                outputStrings.push_back("@" + com.command[2]);
                outputStrings.push_back("D=D+A");
                outputStrings.push_back("@R13"); //For addr storage
                outputStrings.push_back("M=D");  //addr = localBaseAddress + offset

                outputStrings.push_back("@SP"); //SP--
                outputStrings.push_back("M=M-1");

                outputStrings.push_back("A=M");
                outputStrings.push_back("D=M"); //D = *SP

                outputStrings.push_back("@R13"); //addr = localBaseAddress + offset
                outputStrings.push_back("A=M"); //*addr
                outputStrings.push_back("M=D"); //*addr = *SP 
            }
        }
    }

    void WriteLabel(struct Command com) {
        outputStrings.push_back("(" + com.command[1] + ")");
        labelNum++;
    }

    void WriteGoto(struct Command com) {
        outputStrings.push_back("@" + com.command[1]);
        outputStrings.push_back("0;JMP");
    }

    void WriteIfGoto(struct Command com) {
        //If (Value != 0)
        Command push0{ "push", "constant", "0", C_PUSH, "@0" };
        WritePushPop(push0);

        outputStrings.push_back("@SP"); //SP--
        outputStrings.push_back("M=M-1");
        outputStrings.push_back("A=M");
        outputStrings.push_back("D=M");
        outputStrings.push_back("@SP"); //SP --
        outputStrings.push_back("M=M-1");
        outputStrings.push_back("A=M");

        outputStrings.push_back("D=M-D"); //D=a-b a-b == 0? 


        outputStrings.push_back("@" + com.command[1]);

        outputStrings.push_back("D;JNE");

        outputStrings.push_back("@SP"); //if false
        outputStrings.push_back("A=M");
        outputStrings.push_back("M=0"); //0 is false in the VM

        outputStrings.push_back("@CONTINUE." + std::to_string(commandNum));
        outputStrings.push_back("0;JMP");

        outputStrings.push_back("(TRUE." + std::to_string(commandNum) + ")");
        outputStrings.push_back("@SP");
        outputStrings.push_back("A=M");
        outputStrings.push_back("M=-1"); //-1 is true in the VM

        outputStrings.push_back("(CONTINUE." + std::to_string(commandNum) + ")");

        this->commandNum++;
    }

    void WriteCall(struct Command com) {
        //Pushamos o endereço de retorno para a stack para depois podermos voltar a ele
        outputStrings.push_back("@" + com.command[1] + "$RETURN_LABEL." + std::to_string(labelNum));
        outputStrings.push_back("D=A");
        outputStrings.push_back("@SP");
        outputStrings.push_back("A=M");
        outputStrings.push_back("M=D");
        outputStrings.push_back("@SP");
        outputStrings.push_back("M=M+1");

        //Guardamos os valores dos ponteiros dos segmentos de memória 
        //*SP[3] = memSegmentBasePtr
        Command pushLclPtr{ "push", "pointer", "LCL", C_PUSH, "@LCL" };
        WritePushPop(pushLclPtr);

        Command pushArgPtr{ "push", "pointer", "ARG", C_PUSH, "@ARG" };
        WritePushPop(pushArgPtr);

        Command pushThisPtr{ "push", "pointer", "THIS", C_PUSH, "@THIS" };
        WritePushPop(pushThisPtr);

        Command pushThatPtr{ "push", "pointer", "THAT", C_PUSH, "@THAT" };
        WritePushPop(pushThatPtr);

        //Reposicionamos o ptr de ARG para apontar para o valor dos argumentos passados
        //Sabemos que foram pushados para os stack 4 ponteiros de segmentos de memória
        //1 ponteiro do endereço de retorno da função e um numero de argumentos
        //sendo assim reposicionamos o ARG em SP - (4+1) - nArgs
        outputStrings.push_back("@" + com.command[2]);
        outputStrings.push_back("D=A");
        outputStrings.push_back("@5"); //number of basePtrs saved to the stack
        outputStrings.push_back("D=A+D"); //numBasePtrs + numArgs
        outputStrings.push_back("@SP");
        outputStrings.push_back("D=M-D");
        outputStrings.push_back("@ARG");
        outputStrings.push_back("M=D");

        //Setting the LCL ptr
        //LCL = SP 
        outputStrings.push_back("@SP");
        outputStrings.push_back("D=M");
        outputStrings.push_back("@LCL");
        outputStrings.push_back("M=D");

        //goto called function
        outputStrings.push_back("@" + com.command[1]);
        outputStrings.push_back("0;JMP");

        outputStrings.push_back("(" + com.command[1] + "$RETURN_LABEL." + std::to_string(labelNum) + ")");
        labelNum++;
    }

    void WriteFunction(struct Command com) {
        //if (com.command[1].compare("Sys.init") == 0) {
        //    //Writes the bootstrap code
        //    //SP = 256 ou 261?
        //    outputStrings.push_back("@261");
        //    outputStrings.push_back("D=A");
        //    outputStrings.push_back("@SP");
        //    outputStrings.push_back("M=D");
        //}
        outputStrings.push_back("(" + com.command[1] + ")");


        for (int i = 0; i < atoi(com.command[2].c_str()); i++) {
            Command push0{ "push", "constant", "0", C_PUSH, "@0" };
            WritePushPop(push0);
        }
    }

    void WriteReturn(struct Command com) {
        //Restauramos os ponteiros de segmentos de memoria
        //Movemos LCL para R13
        outputStrings.push_back("@LCL"); 
        outputStrings.push_back("D=M");
        outputStrings.push_back("@R13");
        outputStrings.push_back("M=D");

        //Copiamos o endereço de return para R14
        //retAddr = *(endFrame - 5) ou *(LCL - 5)
        outputStrings.push_back("@5");
        outputStrings.push_back("D=A");

        outputStrings.push_back("@R13");
        outputStrings.push_back("A=M-D");
        outputStrings.push_back("D=M"); //*(endFrame - 5)

        outputStrings.push_back("@R14");
        outputStrings.push_back("M=D"); //R14 = *(LCL - 5)


        //Copiamos o valor de retorno para o valor do ARG[0]
        // ARG[0] = *SP
        //OU pop argument 0
        outputStrings.push_back("@SP");
        outputStrings.push_back("A=M-1");
        outputStrings.push_back("D=M");
        outputStrings.push_back("@ARG");
        outputStrings.push_back("A=M");
        outputStrings.push_back("M=D");

        //Restauramos o StackPointer para a posição antes de chamada
        //SP = ARG + 1
        outputStrings.push_back("@ARG");
        outputStrings.push_back("D=M+1");
        outputStrings.push_back("@SP");
        outputStrings.push_back("M=D");

        //Restauramos os restantes segmentos de memória
        //Sabemos que segundo o standard acima do LCL[0] ta o THAT
        outputStrings.push_back("@LCL");
        outputStrings.push_back("A=M-1");
        outputStrings.push_back("D=M");
        outputStrings.push_back("@THAT");
        outputStrings.push_back("M=D");

        outputStrings.push_back("@2");
        outputStrings.push_back("D=A");
        outputStrings.push_back("@LCL");
        outputStrings.push_back("A=M-D");
        outputStrings.push_back("D=M");
        outputStrings.push_back("@THIS");
        outputStrings.push_back("M=D");

        outputStrings.push_back("@3");
        outputStrings.push_back("D=A");
        outputStrings.push_back("@LCL");
        outputStrings.push_back("A=M-D");
        outputStrings.push_back("D=M");
        outputStrings.push_back("@ARG");
        outputStrings.push_back("M=D");

        outputStrings.push_back("@4");
        outputStrings.push_back("D=A");
        outputStrings.push_back("@LCL");
        outputStrings.push_back("A=M-D");
        outputStrings.push_back("D=M");
        outputStrings.push_back("@LCL");
        outputStrings.push_back("M=D");

        //Goto the return adress
        outputStrings.push_back("@R14");
        outputStrings.push_back("A=M");
        outputStrings.push_back("0;JMP");
    }

    void WriteInit() {
        //Writes the bootstrap code
        //SP = 256 ou 261?
        outputStrings.push_back("@256");
        outputStrings.push_back("D=A");
        outputStrings.push_back("@SP");
        outputStrings.push_back("M=D");
        outputStrings.push_back("(Sys.init)");

        Command initC{ "call", "Sys.init", "0", C_CALL, "@0" };
        WriteCall(initC);
    }

    int WriteFile(std::string outFilename) {

        outFilename = outFilename.substr(0, outFilename.find_first_of('.')) + ".asm";
        std::cout << "Generating file " << outFilename << std::endl;

        std::ofstream outputFile(outFilename);

        if (outputFile.is_open()) {
            for (int i = 0; i < outputStrings.size(); i++) {
                outputFile << outputStrings.at(i) + "\n";
            }
            outputFile.close();
            return 1;
        }
        else {
            std::cout << "Unable to write to file!";
            outputFile.close();
            return 0;
        }
    }
};