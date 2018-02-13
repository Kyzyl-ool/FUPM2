#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

enum mode
{
	RR,
	RI,
	RM
};
struct data
{
	int value;
	string name;
	mode m;
};

data ops[] =
{
	{0, "halt", RI},
	{1, "syscall", RI},
	{2, "add", RR},
	{3, "addi", RI},
	{4, "sub", RR},
	{5, "subi", RI},
	{6, "mul", RR},
	{7, "muli", RI},
	{8, "div", RR},
	{9, "divi", RI},
	{12, "lc", RI},
	{13, "shl", RR},
	{14, "shli", RI},
	{15, "shr", RR},
	{16, "shri", RI},
	{17, "and", RR},
	{18, "andi", RI},
	{19, "or", RR},
	{20, "ori", RI},
	{21, "xor", RR},
	{22, "xori", RI},
	{23, "not", RI},
	{24, "mov", RR},
	{32, "addd", RR},
	{33, "subd", RR},
	{34, "muld", RR},
	{35, "divd", RR},
	{36, "itod", RR},
	{37, "dtoi", RR},
	{38, "push", RI},
	{39, "pop", RI},
	{40, "call", RR},
	{41, "calli", RM},
	{42, "ret", RI},
	{43, "cmp", RR},
	{44, "cmpi", RI},
	{45, "cmpd", RR},
	{46, "jmp", RM},
	{47, "jne", RM},
	{48, "jeq", RM},
	{49, "jle", RM},
	{50, "jl", RM},
	{51, "jge", RM},
	{52, "jg", RM},
	{64, "load", RM},
	{65, "store", RM},
	{66, "load2", RM},
	{67, "store2", RM},
	{68, "loadr", RR},
	{69, "storer", RR},
	{70, "loadr2", RM},
	{71, "storer2", RR}
};

int main()
{
	map<string, int> tab;
	for (int i = 0; i < sizeof ops / sizeof ops[0]; i++)
	{
		tab[ops[i].name] = ops[i].value;
	}
	string s;
	
	for (;;)
	{
		cin >> s;
		int c = tab[s];
		if (c == 0)
		{
			cout << "Not found or HALT" << endl;
		}
		else
		{
			cout << "code = " << c << endl;
		}
	}
	return 0;
}
