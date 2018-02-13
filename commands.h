enum code {
    HALT = 0,
    SYSCALL = 1,
    ADD = 2,
    ADDI = 3,
    SUB = 4,
    SUBI = 5,
    MUL = 6,
    MULI = 7,
    DIV = 8,
    DIVI = 9,
    LC = 12,
    SHL = 13,
    SHLI = 14,
    SHR = 15,
    SHRI = 16,
    AND = 17,
    ANDI = 18,
    OR = 19,
    ORI = 20,
    XOR = 21,
    XORI = 22,
    NOT = 23,
    MOV = 24,
	ADDD = 32,
    SUBD = 33,
    MULD = 34,
    DIVD = 35,
    ITOD = 36,
    DTOI = 37,
    PUSH = 38,
    POP = 39,
    CALL = 40,
    CALLI = 41,
    RET = 42,
    CMP = 43,
    CMPI = 44,
    CMPD = 45,
    JMP = 46,
    JNE = 47,
    JEQ = 48,
    JLE = 49,
    JL = 50,
    JGE = 51,
    JG = 52,
    LOAD = 64,
    STORE = 65,
    LOAD2 = 66,
    STORE2 = 67,
    LOADR = 68,
    LOADR2 = 69,
    STORER = 70,
    STORER2 = 71
};

int halt();
int syscall;
int add();
int addi();
int sub();
int subi();
int mul();
int muli();
int div();
int divi();
int lc();
int shl();
int shli();
int shr();
int shri();
int and();
int andi();
int or();
int ori();
int xor();
int xori();
int not();
int mov();
int addd();
int subd();
int muld();
int divd();
int itod();
int dtoi();
int push();
int pop();
int call();
int calli();
int ret();
int cmp();
int cmpi();
int cmpd();
int jmp();
int jne();
int jeq();
int jle();
int jl();
int jge();
int jg();
int load();
int store();
int load2();
int store2();
int loadr();
int storer();
int loadr2();
int storer2();
