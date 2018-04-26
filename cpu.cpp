#include "fupm_cpu.cpp"
#include <iostream>

using namespace std;

int main()
{
	FUPM_CPU Test;
	Test.load_from_file("TEST.FUPMOS");
	Test.run();

	// Test.dump();
	return 0;
}