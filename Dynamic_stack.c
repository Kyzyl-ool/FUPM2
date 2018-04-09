#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Dynamic_stack.h"

#define DEBUG
#define POISON -1999

dynamic_stack* dynamic_stack_Construct()
{
	dynamic_stack* ds = (dynamic_stack* )calloc(1, sizeof(dynamic_stack));
	if (!ds)
		return NULL;
	
	ds->current = 0;
	ds->amount_of_allocated_blocks = 1;
	ds->data = (dynamic_stack_t* )calloc(DYNAMIC_STACK_DEFAULT_SIZE, sizeof(dynamic_stack_t));
	for (int i = 0; i < ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE; i++)
		ds->data[i] = POISON;
	
	return ds;
}

int dynamic_stack_Destroy(dynamic_stack* ds)
{
	#ifdef DEBUG
	assert(ds);
	#endif
	if (ds->data)
		free(ds->data);
	free(ds);

	return DYNAMIC_STACK_OK;
}

int dynamic_stack_Dump(dynamic_stack* ds, FILE* fout, const char* dynamic_stack_name)
{
	fprintf(fout, "Dynamic stack \"%s\"[%s] dump:\n{\n", dynamic_stack_name, dynamic_stack_meaning_of_error_code(dynamic_stack_check(ds)));
	fprintf(fout, "	current: %d\n	blocks: %d\n\n", ds->current, ds->amount_of_allocated_blocks);
	for (int i = 0; i < ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE; i++)
		if (ds->data[i] != POISON)
			fprintf(fout, "	data[%d]:	%d\n", i, ds->data[i]);
		else
			fprintf(fout, "	data[%d]:	POISON\n", i);
	fprintf(fout, "}\n\n");
	
	return 0;
}

int dynamic_stack_Push(dynamic_stack* ds, dynamic_stack_t value)
{
	#ifdef DEBUG
	assert(ds);
	assert(ds->data);
	#endif
	
	ds->data[ds->current] = value;
	ds->current++;
	
	return dynamic_stack_check(ds);
}

dynamic_stack_t dynamic_stack_Pop(dynamic_stack* ds)
{
	#ifdef DEBUG
	assert(ds);
	assert(ds->data);
	#endif
	if (ds->current < 1)
	{
		#ifdef DEBUG
		dynamic_stack_error_print(DYNAMIC_STACK_POP_FROM_EMPTY);
		assert(0);
		#endif
		return POISON;
	}
	ds->current--;
	dynamic_stack_t tmp = ds->data[ds->current];
	ds->data[ds->current] = POISON;
	return tmp;
}

int is_full(dynamic_stack* ds)
{
	if (ds->current == ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE)
		return DYNAMIC_STACK_FULL;
	else if (ds->current > ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE)
		return DYNAMIC_STACK_OVERFLOW;
	else if (ds->current < ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE && ds->current >= (ds->amount_of_allocated_blocks - 1)*DYNAMIC_STACK_DEFAULT_SIZE)
		return DYNAMIC_STACK_OK;
	else
		return DYNAMIC_STACK_OK_BUT_ONE_FREE_BLOCK;
}

int dynamic_stack_check(dynamic_stack* ds)
{
	switch (is_full(ds))
	{
		case DYNAMIC_STACK_FULL:
		{
			dynamic_stack_expand(ds);
			break;
		}
		case DYNAMIC_STACK_OK:
		{
			break;
		}
		case DYNAMIC_STACK_OVERFLOW:
		{
			#ifdef DEBUG
			dynamic_stack_error_print(DYNAMIC_STACK_OVERFLOW);
			assert(0);
			#endif
			return DYNAMIC_STACK_OVERFLOW;
			break;
		}
		case DYNAMIC_STACK_OK_BUT_ONE_FREE_BLOCK:
		{
			dynamic_stack_reduce(ds);
			break;
		}
		default: assert(0);
	}
	
	return DYNAMIC_STACK_OK;
}

int dynamic_stack_expand(dynamic_stack* ds)
{
	dynamic_stack_t* tmp = malloc(ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE*sizeof(dynamic_stack_t));
	for (int i = 0; i < ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE; i++)
		tmp[i] = ds->data[i];
	free(ds->data);
	ds->amount_of_allocated_blocks++;
	ds->data = (dynamic_stack_t* )malloc(ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE*sizeof(dynamic_stack_t));
	for (int i = 0; i < (ds->amount_of_allocated_blocks - 1)*DYNAMIC_STACK_DEFAULT_SIZE; i++)
		ds->data[i] = tmp[i];
	for (int i = (ds->amount_of_allocated_blocks - 1)*DYNAMIC_STACK_DEFAULT_SIZE; i < ds->amount_of_allocated_blocks*DYNAMIC_STACK_DEFAULT_SIZE; i++)
		ds->data[i] = POISON;
	return dynamic_stack_check(ds);
}

const char* dynamic_stack_meaning_of_error_code(int error_code)
{
	switch (error_code)
	{
		#define _RET_CODE(code) case code: return #code
		
		_RET_CODE(DYNAMIC_STACK_OK);
		_RET_CODE(DYNAMIC_STACK_NO_MEMORY);
		_RET_CODE(DYNAMIC_STACK_OVERFLOW);
		_RET_CODE(DYNAMIC_STACK_POP_FROM_EMPTY);
		_RET_CODE(DYNAMIC_STACK_FULL);
		_RET_CODE(DYNAMIC_STACK_OK_BUT_ONE_FREE_BLOCK);
		default: return "DYNAMIC_STACK_UNDEFINED_ERROR";
		
		#undef _RET_CODE
	}
}

void dynamic_stack_error_print(int error_code)
{
	printf("%s\n", dynamic_stack_meaning_of_error_code(error_code));
}

int dynamic_stack_reduce(dynamic_stack* ds)
{
	#ifdef DEBUG
	assert(ds);
	assert(ds->data);
	#endif
	dynamic_stack_t* tmp = (dynamic_stack_t* )malloc((ds->amount_of_allocated_blocks - 1)*DYNAMIC_STACK_DEFAULT_SIZE*sizeof(dynamic_stack_t));
	for (int i = 0; i < (ds->amount_of_allocated_blocks - 1)*DYNAMIC_STACK_DEFAULT_SIZE; i++)
		tmp[i] = ds->data[i];
	
	free(ds->data);
	ds->amount_of_allocated_blocks--;
	ds->data = tmp;
	
	return dynamic_stack_check(ds);
}
