#ifndef STR_MAX_SIZE
#define STR_MAX_SIZE 32
#endif

#ifndef MAX
#define MAX 100000
#endif

//=============================================================================

struct Var
{
	char* name;
	double value;
	double sigma;
};

//=============================================================================

int ScanValues(Var* This, char* buffer);

char* ScanName(char** buffer);

double ScanDouble(char** buffer);

void InitVar(Node* This, char* buffer);

//=============================================================================

int ScanValues(Var* This, char* buffer)
{
	int cr = 0;

	while(*buffer != '\0')
	{
		if(*buffer == '[')
		{	
			buffer++;

			This[cr].name = ScanName(&buffer);

			This[cr].value = ScanDouble(&buffer);

			This[cr].sigma = ScanDouble(&buffer);

			cr++;
		}


		buffer++;
	}
	return cr;
}

char* ScanName(char** buffer)
{
	while(**buffer == ' ')
				(*buffer)++;

	char* name = (char*)calloc(STR_MAX_SIZE, sizeof(char));
	assert(name);

			int i = 0;
			while('A' <= **buffer && **buffer <= 'Z' ||
			  'a' <= **buffer && **buffer <= 'z' ||
			  '0' <= **buffer && **buffer <= '9' ||
			  **buffer == '\\' || **buffer == '_'||  **buffer == '{'||  **buffer == '}')
			{
				name[i] = **buffer;
				i++;
				(*buffer)++;
			}


	while(**buffer == ' ')
				(*buffer)++;
	return name;
}


double ScanDouble(char** buffer)
{
	while(**buffer == ' ')
				(*buffer)++;

	int val1 = 0, val2 = 0;

	while('0' <= **buffer && **buffer <= '9')
	{
		val1 = 10* val1 + **buffer - '0';
		(*buffer)++;
	}

	int ten = 10;
	
	if(**buffer == '.' || **buffer == ',')
	{
		(*buffer)++;

		while('0' <= **buffer && **buffer <= '9')
		{
			val2 = 10* val2 + **buffer - '0';
			(*buffer)++;
		}

		while(val2 % ten != val2)
		ten *= 10;
	}

	while(**buffer == ' ')
		(*buffer)++;
	double val = val1 + (double)val2 / ten;

	return val;
}

//=============================================================================

void InitVar(Node* This, int cr, Var* Values)
{
	if(This->type == VAR)
	{

		for(int i = 0; i < cr; i++)
			if(strcmp(This->name, Values[i].name) == 0)
			{
				This->val = Values[i].value;
			}

	}
	if(This->branch1 != NULL)
		InitVar(This->branch1, cr, Values);
	if(This->branch2 != NULL)
		InitVar(This->branch2, cr, Values);
}