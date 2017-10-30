#include "mylib.h"
#include "tree.h"
#include "Differentiation.h"
#include "varinit.h"
#include "report.h"
#include "recursive_descent.h"

#ifndef MAX
#define MAX 100000
#endif

#define INPUT_FILE_NAME "data.txt"

//=============================================================================

char* ReadDataToBufer(const char* FileName);

char* GetFormula(char* buffer);

void Free(Var* Values, int NumbOfInitVar);

//=============================================================================

int main(int argc, char const *argv[])
{
	HELLO(Recursive Descent);

	char* buffer = ReadDataToBufer(INPUT_FILE_NAME);

	char* formula = GetFormula(buffer);

	Node* phrase = GetG0(formula);

	char* VarNames[MAX] = {};

	int* NumbOfVar = (int*)calloc(1, sizeof(int));

	FindVar(phrase, VarNames, NumbOfVar);

	Var Values [*NumbOfVar] = {};

	free(buffer);

	buffer = ReadDataToBufer(INPUT_FILE_NAME);

	int NumbOfInitVar = ScanValues(Values, buffer);

	InitVar(phrase, NumbOfInitVar, Values);

	MadeReport(phrase, VarNames, NumbOfVar, NumbOfInitVar, Values);


	Free(Values, NumbOfInitVar);
	free(NumbOfVar);
	free(buffer);
	DeleteTree(phrase);


	system("pdflatex report.tex");
	
	system("evince report.pdf&"); 


	return 0;
}

//=============================================================================

char* ReadDataToBufer(const char* FileName)
{
	FILE* data = fopen(FileName, "r");
	assert(data);


	fseek(data, 0, SEEK_END);
	int len = ftell(data);
	fseek(data, 0, SEEK_SET);

	char* buffer = (char*) calloc (len, sizeof(*buffer));
	fread(buffer, len, sizeof(*buffer), data);
	fclose(data);
	return buffer;
}

//=============================================================================

char* GetFormula(char* buffer)
{
	char* formula = NULL;

	while(*buffer != '\0')
	{
		if(*buffer == '<')
		{
			*buffer = '\0';
			formula = buffer + 1;
		}
		
		if(*buffer == '>')
			return formula;
		buffer++;
	}
	return NULL;
}
 
void Free(Var* Values, int NumbOfInitVar)
{
	for(int i = 0; i< NumbOfInitVar; i++)
		free(Values[i].name);
}