#include <iostream>
#include <unordered_map>

#define C_ARITHMETIC 0x1
#define C_PUSH 0x2
#define C_POP 0x3
#define C_LABEL 0x4
#define C_GOTO 0x5
#define C_IF 0x6
#define C_CALL 0x7
#define C_FUNCTION 0x8
#define C_RETURN 0x9

struct Command{
	std::string command[3];
	int commandType;
	std::string adressVariable;
};

std::unordered_map<std::string, int> commandTable = {
	{"add", C_ARITHMETIC},
	{"sub", C_ARITHMETIC},
	{"neg", C_ARITHMETIC},
	{"eq", C_ARITHMETIC},
	{"gt", C_ARITHMETIC},
	{"lt", C_ARITHMETIC},
	{"and", C_ARITHMETIC},
	{"or", C_ARITHMETIC},
	{"not", C_ARITHMETIC},
	{"pop", C_POP},
	{"push", C_PUSH},
	{"label", C_LABEL},
	{"goto", C_GOTO},
	{"if-goto", C_IF},
	{"function", C_FUNCTION},
	{"call", C_CALL},
	{"return", C_RETURN},
};