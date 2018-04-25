#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>

#define STACK_SIZE 1048576
#define AMOUNT_OF_CPU_COMMANDS 52
#define STACK_POISON -999

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::map;

map <string, int> cmds;
map <int, int> cmd_types;
map <string, int> labels;

enum mode
{
	RR ,
	RI ,
	RM ,
	RI1,
	RM1
};

typedef union
{
	double d;
	int u[2];
} du;

struct data
{
	int value;
	string name;
	mode m;
};

enum registers
{
	r0 = 0,
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7,
	r8,
	r9,
	r10,
	r11,
	r12,
	r13,
	r14,
	r15
};

map <string, enum registers> reg_number;


enum code {
    HALT  	 =  0,
    SYSCALL  =  1,
    ADD  	 =  2,
    ADDI  	 =  3,
    SUB  	 =  4,
    SUBI  	 =  5,
    MUL  	 =  6,
    MULI  	 =  7,
    DIV  	 =  8,
    DIVI  	 =  9,
    LC  	 = 12,
    SHL  	 = 13,
    SHLI  	 = 14,
    SHR  	 = 15,
    SHRI  	 = 16,
    AND  	 = 17,
    ANDI  	 = 18,
    OR  	 = 19,
    ORI  	 = 20,
    XOR  	 = 21,
    XORI  	 = 22,
    NOT  	 = 23,
    MOV  	 = 24,
	ADDD  	 = 32,
    SUBD  	 = 33,
    MULD  	 = 34,
    DIVD  	 = 35,
    ITOD  	 = 36,
    DTOI  	 = 37,
    PUSH  	 = 38,
    POP  	 = 39,
    CALL  	 = 40,
    CALLI  	 = 41,
    RET  	 = 42,
    CMP  	 = 43,
    CMPI  	 = 44,
    CMPD  	 = 45,
    JMP  	 = 46,
    JNE  	 = 47,
    JEQ  	 = 48,
    JLE  	 = 49,
    JL  	 = 50,
    JGE  	 = 51,
    JG  	 = 52,
    LOAD  	 = 64,
    STORE  	 = 65,
    LOAD2  	 = 66,
    STORE2   = 67,
    LOADR  	 = 68,
    LOADR2   = 69,
    STORER   = 70,
    STORER2  = 71
};

data ops[] =
{
	{HALT   , "halt"   , RI },
	{SYSCALL, "syscall", RI },
	{ADD    , "add"    , RR },
	{ADDI   , "addi"   , RI },
	{SUB    , "sub"    , RR },
	{SUBI   , "subi"   , RI },
	{MUL    , "mul"    , RR },
	{MULI   , "muli"   , RI },
	{DIV    , "div"    , RR },
	{DIVI   , "divi"   , RI },
	{LC     , "lc"     , RI },
	{SHL    , "shl"    , RR },
	{SHLI   , "shli"   , RI },
	{SHR    , "shr"    , RR },
	{SHRI   , "shri"   , RI },
	{AND    , "and"    , RR },
	{ANDI   , "andi"   , RI },
	{OR     , "or"     , RR },
	{ORI    , "ori"    , RI },
	{XOR    , "xor"    , RR },
	{XORI   , "xori"   , RI },
	{NOT    , "not"    , RI },
	{MOV    , "mov"    , RR },
	{ADDD   , "addd"   , RR },
	{SUBD   , "subd"   , RR },
	{MULD   , "muld"   , RR },
	{DIVD   , "divd"   , RR },
	{ITOD   , "itod"   , RR },
	{DTOI   , "dtoi"   , RR },
	{PUSH   , "push"   , RI },
	{POP    , "pop"    , RI },
	{CALL   , "call"   , RR },
	{CALLI  , "calli"  , RM1},
	{RET    , "ret"    , RI1},
	{CMP    , "cmp"    , RR },
	{CMPI   , "cmpi"   , RI },
	{CMPD   , "cmpd"   , RR },
	{JMP    , "jmp"    , RM1},
	{JNE    , "jne"    , RM1},
	{JEQ    , "jeq"    , RM1},
	{JLE    , "jle"    , RM1},
	{JL     , "jl"     , RM1},
	{JGE    , "jge"    , RM1},
	{JG     , "jg"     , RM1},
	{LOAD   , "load"   , RM },
	{STORE  , "store"  , RM },
	{LOAD2  , "load2"  , RM },
	{STORE2 , "store2" , RM },
	{LOADR  , "loadr"  , RR },
	{LOADR2 , "storer" , RR },
	{STORER , "loadr2" , RM },
	{STORER2, "storer2", RR }
};

class FUPM_CPU
{
private:
	bool running;
	int  Registers[16];
	char Flags; // 3 - ne, 2 - e, 1 - g, 0 - l
	int  Stack[STACK_SIZE];
	
	
public:
	FUPM_CPU();
	// ~FUPM_CPU();
	void dump();
	void load_from_file(string filename);
	void run();
	
	void HALT		(registers r, int number);
	void SYSCALL	(registers r, int number);
	void ADD		(registers ri, registers ro, int number);
	void ADDI		(registers r, int number);
	void SUB		(registers ri, registers ro, int number);
	void SUBI		(registers r, int number);
	void MUL		(registers ri, registers ro);
	void MULI		(registers r, int number);
	void DIV		(registers ri, registers ro);
	void DIVI		(registers r, int number);
	void LC			(registers r, int number);
	void SHL		(registers ri, registers ro);
	void SHLI		(registers r, int number);
	void SHR		(registers ri, registers ro);
	void SHRI		(registers r, int number);
	void AND		(registers ri, registers ro);
	void ANDI		(registers r, int number);
	void OR			(registers ri, registers ro);
	void ORI		(registers r, int number);
	void XOR		(registers ri, registers ro);
	void XORI		(registers r, int number);
	void NOT		(registers r);
	void MOV		(registers ri, registers ro, int number);
	void ADDD		(registers ri, registers ro, int number);
	void SUBD		(registers ri, registers ro, int number);
	void MULD		(registers ri, registers ro, int number);
	void DIVD		(registers ri, registers ro, int number);
	void ITOD		(registers ri, registers ro, int number);
	void DTOI		(registers ri, registers ro, int number);
	void PUSH		(registers r, int number);
	void POP		(registers r, int number);
	void CALL		(registers ri, registers ro, int number);
	void CALLI		(registers r, unsigned int number);
	void RET		(int number);
	void CMP		(registers ri, registers ro, int number);
	void CMPI		(registers r, int number);
	void CMPD		(registers ri, registers ro, int number);
	void JMP		(unsigned int number);
	void JNE		(unsigned int number);
	void JEQ		(unsigned int number);
	void JLE		(unsigned int number);
	void JL			(unsigned int number);
	void JGE		(unsigned int number);
	void JG			(unsigned int number);
	void LOAD		(registers r, unsigned int number);
	void STORE		(registers r, unsigned int number);
	void LOAD2		(registers r, unsigned int number);
	void STORE2		(registers r, unsigned int number);
	void LOADR		(registers ri, registers ro, int number);
	void STORER		(registers ri, registers ro, int number);
	void LOADR2		(registers ri, registers ro, int number);
	void STORER2	(registers ri, registers ro, int number);
};

FUPM_CPU::FUPM_CPU():
running(false),
Flags(0)
{
	for (int i = 0; i < AMOUNT_OF_CPU_COMMANDS; ++i)
	{
		cmds[ops[i].name] = ops[i].value;
		cmd_types[ops[i].value] = ops[i].m;
	}

	reg_number["r0"] = r0;
	reg_number["r1"] = r1;
	reg_number["r2"] = r2;
	reg_number["r3"] = r3;
	reg_number["r4"] = r4;
	reg_number["r5"] = r5;
	reg_number["r6"] = r6;
	reg_number["r7"] = r7;
	reg_number["r8"] = r8;
	reg_number["r9"] = r9;
	reg_number["r10"] = r10;
	reg_number["r11"] = r11;
	reg_number["r12"] = r12;
	reg_number["r13"] = r13;
	reg_number["r14"] = r14;
	reg_number["r15"] = r15;

	for (int i = 0; i <= r15; i++) Registers[i] = 0;
	Registers[r14] = STACK_SIZE - 1;

	for (int i = 0; i < STACK_SIZE; i++)
	{
		Stack[i] = STACK_POISON;
	}
}

void FUPM_CPU::dump()
{
	cout << "DUMP: FUPM_CPU ("; if (running) cout << "running"; else cout << "not running"; cout << ")\n{\n	Stack (" << Registers[r14] << "):\n	{\n";
	
	for (int i = 0; i < STACK_SIZE; i++)
		if (Stack[i] != STACK_POISON)
			cout << "		Stack[" << i << "] = " << Stack[i] << endl;
	
	cout << "	}\n	Registers:\n	{\n";
	for (int i = 0; i <= r15; i++) cout << "		r" << i << " = " << Registers[i] << endl;
	cout << "	}\n\n";
	
	cout << "	Flags:\n	{\n";
	if (Flags & 0b1000)
		cout << "		ne";
	if (Flags & 0b0100)
		cout << "		e";
	if (Flags & 0b0010)
		cout << "		g";
	if (Flags & 0b0001)
		cout << "		l";
	cout << "	}\n";

	cout << "}\n\n";
}

void FUPM_CPU::HALT(registers r, int number)
{
	running = false;
}

void FUPM_CPU::SYSCALL(registers r, int number)
{
	switch (number)
	{
		case 100:
		{
			scanf("%d", &Registers[r]);
			break;
		}
		case 101:
		{
			du tmp;
			scanf("%lg", &tmp.d);
			
			Registers[r] = tmp.u[0];
			Registers[r+1] = tmp.u[1];
			break;
		}
		case 102:
		{
			printf("%d", Registers[r]);
			break;
		}
		case 103:
		{
			du tmp;
			tmp.u[0] = Registers[r];
			tmp.u[1] = Registers[r+1];
			printf("%lg", tmp.d);
			break;
		}
		case 105:
		{
			printf("%c", Registers[r]);
			break;
		}
		case 0:
		{
			running = false;
			break;
		}
		default:
		{
			assert(!"UNDEFINED COMMAND");
		}
	}
}

void FUPM_CPU::ADD		(registers ri, registers ro, int number)
{
	Registers[ri] += Registers[ro] + number;
}
void FUPM_CPU::ADDI		(registers r, int number)
{
	Registers[r] += number;
}
void FUPM_CPU::SUB		(registers ri, registers ro, int number)
{
	Registers[ri] -= Registers[ro] + number;
}
void FUPM_CPU::SUBI		(registers r, int number)
{
	Registers[r] -= number;
}
void FUPM_CPU::MUL		(registers ri, registers ro)
{
	long result = Registers[ri]*Registers[ro];
	Registers[ri] = result & 0x00000000FFFFFFFF;
	Registers[ri+1] = result & 0xFFFFFFFF00000000;
}

void FUPM_CPU::MULI		(registers r, int number)
{
	long result = Registers[r]*number;
	Registers[r] = result & 0x00000000FFFFFFFF;
	Registers[r+1] = result & 0xFFFFFFFF00000000;
}
void FUPM_CPU::DIV		(registers ri, registers ro)
{
	long dividend = Registers[ri+1];
	dividend = dividend >> 32;
	dividend += Registers[ri];
	
	Registers[ri] = dividend / Registers[ro];
	Registers[ri+1] = dividend % Registers[ro];
}
void FUPM_CPU::DIVI		(registers r, int number)
{
	long dividend = Registers[r+1];
	dividend = dividend >> 32;
	dividend += Registers[r];
	
	Registers[r] = dividend / number;
	Registers[r+1] = dividend % number;
}
void FUPM_CPU::LC			(registers r, int number)
{
	Registers[r] = number;
}
void FUPM_CPU::SHL		(registers ri, registers ro)
{
	Registers[ri] <<= Registers[ro];
}
void FUPM_CPU::SHLI		(registers r, int number)
{
	Registers[r] <<= number;
}
void FUPM_CPU::SHR		(registers ri, registers ro)
{
	Registers[ri] >>= Registers[ro];
}
void FUPM_CPU::SHRI		(registers r, int number)
{
	Registers[r] >>= number;
}
void FUPM_CPU::AND		(registers ri, registers ro)
{
	Registers[ri] &= Registers[ro];
}
void FUPM_CPU::ANDI		(registers r, int number)
{
	Registers[r] &= number;
}
void FUPM_CPU::OR			(registers ri, registers ro)
{
	Registers[ri] |= Registers[ro];
}
void FUPM_CPU::ORI		(registers r, int number)
{
	Registers[r] |= number;
}
void FUPM_CPU::XOR		(registers ri, registers ro)
{
	Registers[ri] = Registers[ri] xor Registers[ro];
}
void FUPM_CPU::XORI		(registers r, int number)
{
	Registers[r] = Registers[r] xor number;
}
void FUPM_CPU::NOT		(registers r)
{
	Registers[r] = ~Registers[r];
}
void FUPM_CPU::MOV		(registers ri, registers ro, int number)
{
	Registers[ri] = Registers[ro] + number;
}
void FUPM_CPU::ADDD		(registers ri, registers ro, int number)
{
	du tmpi, tmpo;
	tmpi.u[0] = Registers[ri]; tmpi.u[1] = Registers[ri+1];
	tmpo.u[0] = Registers[ro]; tmpo.u[1] = Registers[ro+1];

	tmpi.d += tmpo.d + number;

	Registers[ri] = tmpi.u[0];
	Registers[ri+1] = tmpi.u[1];
}
void FUPM_CPU::SUBD		(registers ri, registers ro, int number)
{
	du tmpi, tmpo;
	tmpi.u[0] = Registers[ri]; tmpi.u[1] = Registers[ri+1];
	tmpo.u[0] = Registers[ro]; tmpo.u[1] = Registers[ro+1];

	tmpi.d -= tmpo.d + number;

	Registers[ri] = tmpi.u[0];
	Registers[ri+1] = tmpi.u[1];
}
void FUPM_CPU::MULD		(registers ri, registers ro, int number)
{
	du tmpi, tmpo;
	tmpi.u[0] = Registers[ri]; tmpi.u[1] = Registers[ri+1];
	tmpo.u[0] = Registers[ro]; tmpo.u[1] = Registers[ro+1];

	tmpi.d *= tmpo.d;
	tmpi.d += number;

	Registers[ri] = tmpi.u[0];
	Registers[ri+1] = tmpi.u[1];
}
void FUPM_CPU::DIVD		(registers ri, registers ro, int number)
{
	du tmpi, tmpo;
	tmpi.u[0] = Registers[ri]; tmpi.u[1] = Registers[ri+1];
	tmpo.u[0] = Registers[ro]; tmpo.u[1] = Registers[ro+1];

	tmpi.d /= tmpo.d;
	tmpi.d += number;

	Registers[ri] = tmpi.u[0];
	Registers[ri+1] = tmpi.u[1];
}
void FUPM_CPU::ITOD		(registers ri, registers ro, int number)
{
	double d = Registers[ro];
	du tmp; tmp.d = d;
	Registers[ri] = tmp.u[0];
	Registers[ri+1] = tmp.u[1];
}
void FUPM_CPU::DTOI		(registers ri, registers ro, int number)
{
	du tmp;
	tmp.u[0] = Registers[ro];
	tmp.u[1] = Registers[ro+1];

	Registers[ri] = tmp.d;
}
void FUPM_CPU::PUSH		(registers r, int number)
{
	Stack[Registers[r14]--] = Registers[r] + number;
}
void FUPM_CPU::POP		(registers r, int number)
{
	Registers[r] = Stack[++Registers[r14]] + number;
}
void FUPM_CPU::CALL		(registers ri, registers ro, int number)
{
	Stack[Registers[r14]--] = Registers[r15];
	Registers[r15] = Registers[ro] + number;
}
void FUPM_CPU::CALLI		(registers r, unsigned int number)
{
	Stack[Registers[r14]--] = Registers[r15];
	Registers[r15] = number;
}
void FUPM_CPU::RET		(int number)
{
	Registers[r14] += number;
	Registers[r15] = Stack[++Registers[r14]];
}

void FUPM_CPU::CMP		(registers ri, registers ro, int number)
{
	if (Registers[ri] == Registers[ro])
		Flags = 0b0100;
	else if (Registers[ri] > Registers[ro])
		Flags = 0b1010;
	else
		Flags = 0b1001;
}
void FUPM_CPU::CMPI		(registers r, int number)
{
	if (Registers[r] == number)
		Flags = 0b0100;
	else if (Registers[r] > number)
		Flags = 0b1010;
	else
		Flags = 0b1001;
}
void FUPM_CPU::CMPD		(registers ri, registers ro, int number)
{
	du tmp1, tmp2;

	tmp1.u[0] = Registers[ri]; tmp1.u[1] = Registers[ri+1];
	tmp2.u[0] = Registers[ro]; tmp2.u[1] = Registers[ro+1];

	if (tmp1.d == tmp2.d)
		Flags = 0b0100;
	else if (tmp1.d > tmp2.d)
		Flags = 0b1010;
	else
		Flags = 0b1001;
}
void FUPM_CPU::JMP		(unsigned int number)
{
	Registers[r15] = number;
}
void FUPM_CPU::JNE		(unsigned int number)
{
	if (Flags & 0b1000)
		Registers[r15] = number;
}
void FUPM_CPU::JEQ		(unsigned int number)
{
	if (Flags & 0b0100)
		Registers[r15] = number;
}
void FUPM_CPU::JLE		(unsigned int number)
{
	if (Flags & 0b0101)
		Registers[r15] = number;
}
void FUPM_CPU::JL			(unsigned int number)
{
	if (Flags & 0b0001)
		Registers[r15] = number;
}
void FUPM_CPU::JGE		(unsigned int number)
{
	if (Flags & 0b0110)
		Registers[r15] = number;
}
void FUPM_CPU::JG			(unsigned int number)
{
	if (Flags & 0b0010)
		Registers[r15] = number;
}
void FUPM_CPU::LOAD		(registers r, unsigned int number)
{
	Registers[r] = Stack[number];
}
void FUPM_CPU::STORE		(registers r, unsigned int number)
{
	Stack[number] = Registers[r];
}
void FUPM_CPU::LOAD2		(registers r, unsigned int number)
{
	Registers[r] = Stack[number];
	Registers[r+1] = Stack[number+1];
}
void FUPM_CPU::STORE2		(registers r, unsigned int number)
{
	Stack[number] = Registers[r];
	Stack[number+1] = Registers[r+1];
}
void FUPM_CPU::LOADR		(registers ri, registers ro, int number)
{
	Registers[ri] = Stack[Registers[ro] + number];
}
void FUPM_CPU::STORER		(registers ri, registers ro, int number)
{
	Stack[Registers[r0]+number] = Registers[ri];
}
void FUPM_CPU::LOADR2		(registers ri, registers ro, int number)
{
	Registers[ri] = Stack[Registers[ro] + number];
	Registers[ri+1] = Stack[Registers[ro] + number + 1];
}
void FUPM_CPU::STORER2	(registers ri, registers ro, int number)
{
	Stack[Registers[ro]] = Registers[ri] + number;
	Stack[Registers[ro]+1] = Registers[ri+1] + number;
}

void FUPM_CPU::load_from_file(string filename)
{
	std::ifstream fin(filename);
	assert("FILE NOT EXISTS" && fin);
	string tmp, main_label;

	int count = 0;
	while (!fin.eof())
	{
		fin >> tmp;
		if (tmp.find(':') != -1)
		{
			tmp.pop_back();
			labels[tmp] = count+1;
			fin >> tmp;
		}
		if (tmp.find("end ") != -1) break;


		switch (cmd_types[cmds[tmp]])
		{
			case RI:
			{
				count++;
				fin >> tmp;
				fin >> tmp;
				break;
			}
			case RR:
			{
				count++;
				fin >> tmp;
				fin >> tmp;
				fin >> tmp;
				break;
			}
			case RM:
			{
				count++;
				fin >> tmp;
				fin >> tmp;
				break;
			}
			case RI1:
			{
				count++;
				fin >> tmp;
				break;
			}
			case RM1:
			{
				count++;
				fin >> tmp;
				break;
			}
			default: assert(!"FATAL ERROR");
		}
	}
	fin.close();
	fin.open(filename);
	

	if (labels["main"]-1 != -1)
	{
		Registers[r15] = labels["main"]-1;
		main_label = "main";
	}
	else
	{
		main_label = labels.begin()->first;
	}


	count = 0;
	while(true)
	{
		fin >> tmp;
		if (tmp.find(':') != -1)
		{
			fin >> tmp;
		}
		else if (tmp.find("end") != -1)
		{
			fin >> tmp;
			if (tmp.find(main_label) != -1)
			{
				Stack[count++] = 0;
				break;
			}
			else
				assert(0);
		}

		switch (cmd_types[cmds[tmp]])
		{
			case RI:
			{
				Stack[count] = (cmds[tmp] << 24);
				fin >> tmp;
				Stack[count] |= (reg_number[tmp] << 20);
				fin >> tmp;
				Stack[count++] |= std::stoi(tmp);
				break;
			}
			case RR:
			{
				Stack[count] = (cmds[tmp] << 24);
				fin >> tmp;
				Stack[count] |= (reg_number[tmp] << 20);
				fin >> tmp;
				Stack[count] |= (reg_number[tmp] << 16);
				fin >> tmp;
				Stack[count++] |= std::stoi(tmp);
				break;
			}
			case RM:
			{
				Stack[count] = (cmds[tmp] << 24);
				fin >> tmp;
				Stack[count] |= (reg_number[tmp] << 20);
				fin >> tmp;
				Stack[count++] |= std::stoi(tmp);
				break;
			}
			case RI1:
			{
				Stack[count] = (cmds[tmp] << 24);
				fin >> tmp;
				if (labels[tmp]-1 != -1)
				{
					Stack[count++] |= ((labels[tmp]-1) << 24);
				}
				else
				{
					Stack[count++] += (std::stoi(tmp) << 24);
				}
				break;
			}
			case RM1:
			{
				Stack[count] = (cmds[tmp] << 24);
				fin >> tmp;
				if (labels[tmp]-1 != -1)
				{
					Stack[count++] |= ((labels[tmp]-1) << 24);
				}
				else
				{
					Stack[count++] += (std::stoi(tmp) << 24);
				}
				break;
			}
			default: assert(!"FATAL ERROR");
		}

	}
	fin.close();
}

int cmd_to_cmd_number(int cmd)
{
	return ((cmd & (0xFF << 24)) >> 24);
}

registers cmd_take_register_number1(int cmd)
{
	return (registers)((cmd & (0b1111 << 20)) >> 20);
}

registers cmd_take_register_number2(int cmd)
{
	return (registers)((cmd & (0b1111 << 16)) >> 16);
}

void FUPM_CPU::run()
{
	registers r, ro, ri;
	int number;
	unsigned int unumber;
	int cmd; int count = 0;
	running = true;
	
	while (running)
	{
		cmd = Stack[count++];
		// cout << count;
		// cout << cmd << endl;

		// cout << "cmd: " << cmd_to_cmd_number(cmd) << endl;
		// cout << "register: " << cmd_take_register_number1(cmd) << endl;
		// cout << "number: " << (cmd & 0x000FFFFF) << endl << endl;

		switch (cmd_types[cmd_to_cmd_number(cmd)])
		{
			case RI:
			{
				r = cmd_take_register_number1(cmd);
				number = cmd & 0x000FFFFF;
				unumber = cmd & 0x000FFFFF;
				break;
			}
			case RR:
			{
				ri = cmd_take_register_number1(cmd);
				ro = cmd_take_register_number2(cmd);
				number = cmd & 0x0000FFFF;
				unumber = cmd & 0x0000FFFF;
				break;
			}
			case RM:
			{
				r = cmd_take_register_number1(cmd);
				unumber = cmd & 0x000FFFFF;
				number = cmd & 0x000FFFFF;
				break;
			}
			case RI1:
			{
				unumber = cmd & 0x000FFFFF;
				number = cmd & 0x000FFFFF;
				break;
			}
			case RM1:
			{
				unumber = cmd & 0x000FFFFF;
				number = cmd & 0x000FFFFF;
				break;
			}
			default: assert(!"FATAL ERROR");
		}

		// cout << cmd_to_cmd_number(cmd) << endl;

		switch (cmd_to_cmd_number(cmd))
		{
			case 0:	    { HALT(r, number); break;}        
			case 1:	    { SYSCALL(r, number); break;}        
			case 2:	    { ADD(ri, ro, number); break;}        
			case 3:	    { ADDI(r, number); break;}        
			case 4:	    { SUB(ri, ro, number); break;}        
			case 5:	    { SUBI(r, number); break;}        
			case 6:	    { MUL(ri, ro); break;}        
			case 7:	    { MULI(r, number); break;}        
			case 8:	    { DIV(ri, ro); break;}        
			case 9:	    { DIVI(r, number); break;}        
			case 12:	{ LC(r, number); break;}        
			case 13:	{ SHL(ri, ro); break;}       
			case 14:	{ SHLI(r, number); break;}       
			case 15:	{ SHR(ri, ro); break;}       
			case 16:	{ SHRI(r, number); break;}       
			case 17:	{ AND(ri, ro); break;}       
			case 18:	{ ANDI(r, number); break;}       
			case 19:	{ OR(ri, ro); break;}       
			case 20:	{ ORI(r, number); break;}       
			case 21:	{ XOR(ri, ro); break;}       
			case 22:	{ XORI(r, number); break;}       
			case 23:	{ NOT(r); break;}       
			case 24:	{ MOV(ri, ro, number); break;}       
			case 32:	{ ADDD(ri, ro, number); break;}       
			case 33:	{ SUBD(ri, ro, number); break;}       
			case 34:	{ MULD(ri, ro, number); break;}       
			case 35:	{ DIVD(ri, ro, number); break;}       
			case 36:	{ ITOD(ri, ro, number); break;}       
			case 37:	{ DTOI(ri, ro, number); break;}       
			case 38:	{ PUSH(r, number); break;}       
			case 39:	{ POP(r, number); break;}       
			case 40:	{ CALL(ri, ro, number); break;}       
			case 41:	{ CALLI(r, unumber); break;}       
			case 42:	{ RET(number); break;}       
			case 43:	{ CMP(ri, ro, number); break;}       
			case 44:	{ CMPI(r, number); break;}       
			case 45:	{ CMPD(ri, ro, number); break;}       
			case 46:	{ JMP(unumber); break;}       
			case 47:	{ JNE(unumber); break;}       
			case 48:	{ JEQ(unumber); break;}       
			case 49:	{ JLE(unumber); break;}       
			case 50:	{ JL(unumber); break;}       
			case 51:	{ JGE(unumber); break;}       
			case 52:	{ JG(unumber); break;}       
			case 64:	{ LOAD(r, unumber); break;}       
			case 65:	{ STORE(r, unumber); break;}       
			case 66:	{ LOAD2(r, unumber); break;}       
			case 67:	{ STORE2(r, unumber); break;}       
			case 68:	{ LOADR(ri, ro, number); break;}       
			case 69:	{ LOADR2(ri, ro, number); break;}       
			case 70:	{ STORER(ri, ro, number); break;}       
			case 71:	{ STORER2(ri, ro, number); break;}     
			default: assert(!"UNKNOWN COMMAND");  
		}
	}
}