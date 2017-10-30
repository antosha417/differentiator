#ifndef STR_MAX_SIZE
#define STR_MAX_SIZE 32
#endif

char* BUFFER = NULL;

//=============================================================================
/*
e::= [' ']*
N::= ['0'-'9']+
D::= eN(['.'',']N)?e
Id::= ['A'-'Z''a'-'z']+['A'-'Z''a'-'z''0'-'9']*
P::= (e'('eEe')'e)|D|Id
F::= e['sin''cos''ln''log']e'('Ee([',']eE)?')'|P
Ex::= F(['^']F)?
T::= Ex(['*''/'])Ex)*
E::= T(['+''-']T)*
*/

//=============================================================================
Node* GetG0(char* str);

Node* GetE();

Node* GetT();

Node* GetEx();

Node* GetF();

Node* GetP();

Node* GetId();

Node* GetD();

int GetN();

void Gete();

//=============================================================================

//=============================================================================

void Gete()
{
	while(*BUFFER == ' ')
	{
		BUFFER++;
	}
}

//=============================================================================

int GetN()
{
	Gete();

	int val = 0;

	while('0' <= *BUFFER && *BUFFER <= '9')
	{
		val = 10* val + *BUFFER - '0';
		BUFFER++;
		//printf("[%c] I'm GetN.\n", *BUFFER );
	}

	return val;
}

//=============================================================================

Node* GetD()
{
	Gete();

	double val = 0;
	int val1 = 0, val2 = 0;

	val1 = GetN();

	int ten = 10;
	
	if(*BUFFER == '.' || *BUFFER == ',')
	{
		BUFFER++;
		val2 = GetN();
		while(val2 % ten != val2)
		ten *= 10;
	}

	val = val1 + (double)val2 / ten;
	Node* node = CreateNode(NUMB, val, NULL, NULL, NULL);

	Gete();
	return node;
}


//=============================================================================

Node* GetId()
{
	Gete();

	int cr = 0;
	char* name = NULL;
	if('A' <= *BUFFER && *BUFFER <= 'Z' ||
	   'a' <= *BUFFER && *BUFFER <= 'z' ||
	   *BUFFER == '\\' || *BUFFER == '_' ||  *BUFFER == '{'||  *BUFFER == '}')
		while('A' <= *BUFFER && *BUFFER <= 'Z' ||
			  'a' <= *BUFFER && *BUFFER <= 'z' ||
			  '0' <= *BUFFER && *BUFFER <= '9' ||
			  *BUFFER == '\\' || *BUFFER == '_'|| 
			  *BUFFER == '{'||  *BUFFER == '}')
		{
			if(cr == 0)
			{
				name = (char*)calloc(STR_MAX_SIZE, sizeof(*name));
				name[cr] = *BUFFER;
				BUFFER++;
				cr++;
			}
			else if(cr < STR_MAX_SIZE)
			{
				name[cr] = *BUFFER;
				BUFFER++;
				cr++;
			}
			else
			{
				printf("(Syntax error): <%s>\n", BUFFER);
				exit(-256);	
			}
		}
	Node* node = CreateNode(VAR, 0, name, NULL, NULL);
	Gete();
	return node;
}

//=============================================================================

Node* GetP()
{
	Gete();

	if(*BUFFER == '(')
	{
		BUFFER++;
		//printf("[%c] I'm GetP.\n", *BUFFER );
		Gete();

		Node* node = GetE();
		
		Gete();

		if(*BUFFER != ')')
		{	
			printf("Syntax error: <%s>\n", BUFFER);
			exit(-256);
		}
		BUFFER++;

		Gete();
		//printf("[%c] I'm GetP.\n", *BUFFER );
		return node;
	}
	else if('0' <= *BUFFER && *BUFFER <= '9')
	{
	    return GetD();
	}
	else
	{
		return GetId();
	}
}

//=============================================================================

Node* GetF()
{
	Gete();

	int OpNumb = -1;

	if(*BUFFER == 's' && BUFFER[1] == 'i' && BUFFER[2] == 'n')
	{
		BUFFER += 3;
		OpNumb = SIN;
	}
	else if(*BUFFER == 'c' && BUFFER[1] == 'o' && BUFFER[2] == 's')
	{

		BUFFER += 3;
		OpNumb = COS;
	}
	else if(*BUFFER == 'l')
	{
		BUFFER++;
		if(BUFFER[0] == 'n')
		{
			BUFFER ++;
			OpNumb = LN;
		}
		if(BUFFER[0] == 'o' && BUFFER[1] == 'g')
		{
			BUFFER += 2;
			OpNumb = LOG;
		}
	}

	Gete();
	if(OpNumb != -1)
		if(*BUFFER == '(')
		{	
			BUFFER++;
			Gete();
			Node* node1 = GetE();
			Node* node2 = NULL;

			Gete();
			if(*BUFFER == ',' && OpNumb == LOG)
			{
				BUFFER++;
				Gete();
				node2 = GetE();
			}


			Node* node = CreateNode(OP, OpNumb, NULL, node1, node2);
			Gete();
			if(*BUFFER != ')')
			{	
				printf("Syntax error: <%s>\n", BUFFER);
				exit(1);
			}
			BUFFER++;
			//printf("Numb = %d\n", OpNumb );
			//DumpNode(node);
			return node;
		}
		else 
		{	
				printf("Syntax error: <%s>\n", BUFFER);
				exit(1);
		}
	else return GetP();
	
}

//=============================================================================

Node* GetEx()
{
	Gete();

	Node* node = GetF();

	while(*BUFFER == '^')
	{
		BUFFER++;
		Node* node2 = GetF();
		node = CreateNode(OP, POW, NULL, node, node2);
	}
	return node;
}

//=============================================================================

Node* GetT()
{
	Gete();

	Node* node = GetEx();

	while(*BUFFER == '*' || *BUFFER == '/')
	{
		char op = *BUFFER;
		BUFFER++;
		//printf("[%c] I'm GetT.\n", *BUFFER );

		Node* node2 = GetEx();

		if(op == '*')
			node = CreateNode(OP, MUL, NULL, node, node2);
		if(op == '/')
			node = CreateNode(OP, DIV, NULL, node, node2);		
	}

	return node;
}

//=============================================================================

Node* GetE()
{
	Gete();

	Node* node = GetT();
	while(*BUFFER == '-' || *BUFFER == '+')
	{
		char op = *BUFFER;
		BUFFER++;
		//printf("[%c] I'm GetE.\n", *BUFFER );

		Node* node2 = GetT();

		if(op == '+')
			node = CreateNode(OP, ADD, NULL, node, node2);
		if(op == '-')
			node = CreateNode(OP, SUB, NULL, node, node2);		
	}

	return node;
}

//=============================================================================

Node* GetG0(char* str)
{	
	BUFFER = str;

	Node* node = GetE();

	if(*BUFFER != '>')
	{
		printf("Syntax error: <%s>\n", BUFFER);
		exit(1);
	}
	return node;
}

//=============================================================================