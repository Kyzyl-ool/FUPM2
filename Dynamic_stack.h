#include <stdio.h>
#define DYNAMIC_STACK_DEFAULT_SIZE 16


typedef int dynamic_stack_t;
typedef struct dynamic_stack
{
	dynamic_stack_t* data;
	int current;
	int amount_of_allocated_blocks;
} dynamic_stack;

typedef enum dynamic_stack_error_codes
{
	DYNAMIC_STACK_OK,
	DYNAMIC_STACK_NO_MEMORY,
	DYNAMIC_STACK_OVERFLOW,
	DYNAMIC_STACK_FULL,
	DYNAMIC_STACK_POP_FROM_EMPTY,
	DYNAMIC_STACK_OK_BUT_ONE_FREE_BLOCK,
	
} dynamic_stack_error_code;


dynamic_stack* dynamic_stack_Construct();
int dynamic_stack_Destroy(dynamic_stack* ds);

int dynamic_stack_Dump(dynamic_stack* ds, FILE* fout, const char* dynamic_stack_name);
int dynamic_stack_check(dynamic_stack* ds);
const char* dynamic_stack_meaning_of_error_code(int error_code);
void dynamic_stack_error_print(int error_code);

int dynamic_stack_Push(dynamic_stack* ds, dynamic_stack_t value);
dynamic_stack_t dynamic_stack_Pop(dynamic_stack* ds);

int is_full(dynamic_stack* ds);
int dynamic_stack_expand(dynamic_stack* ds);
int dynamic_stack_reduce(dynamic_stack* ds);
