
enum TypesOfNode
{
	OP,
	NUMB,
	VAR
};

//=============================================================================

enum TypesOfOperations
{
	ADD,
	SUB,
	MUL,
	DIV,
	POW,
	SIN,
	COS,
	LN,
	LOG
};

//=============================================================================

struct Node
{
	int type;
	double val;
	char* name;
	Node* branch1;
	Node* branch2;
};

//=============================================================================

Node* CreateNode(int type, double val, char* name, Node* branch1, Node* branch2);

void DeleteNode(Node* This);

void DeleteTree(Node* This);

void DumpTree(Node* This);

void DumpNode(Node* This);

void DumpTreeToTex(Node* This,FILE* output);

void DumpNodeToTex(Node* This,FILE* output);

//=============================================================================

Node* CreateNode(int type, double val, char* name, Node* branch1, Node* branch2)
{
	Node* This = (Node*)calloc(1, sizeof(*This));
	assert(This);
	This->type = type;
	This->val = val;
	This->name = name;
	This->branch1 = branch1;
	This->branch2 = branch2;
}

//=============================================================================

void DeleteNode(Node* This)
{
	assert(This);
	if(This->type == VAR && This->name != NULL)
		free(This->name);
	This->branch1 = This->branch2 = NULL;
	free(This);
	This = NULL;
}

//=============================================================================

void DeleteTree(Node* This)
{	
	assert(This);
	if(This->branch1 != NULL)
		DeleteTree(This->branch1);
	if(This->branch2 != NULL)
		DeleteTree(This->branch2);
	if(This->branch1 == NULL && This->branch2 == NULL)
		DeleteNode(This);
}

//=============================================================================

void DumpTree(Node* This)
{	
	assert(This);
	if(This->branch1 != NULL && This->branch2 != NULL)
	{
		DumpTree(This->branch1);
		DumpNode(This);
		DumpTree(This->branch2);
	}
	else if(This->branch1 == NULL && This->branch2 == NULL)
		DumpNode(This);
}

//=============================================================================

#define CASE(x, code)	\
	case x:				\
	printf( #code );	\
	break;

void DumpNode(Node* This)
{
	printf("NODE\n{\nType = ");
	if(This->type == OP)
	{
		printf("OP\n");
		printf("Val = ");
		switch((int)This->val)
		{
			CASE(ADD, +);
			CASE(SUB, -);
			CASE(MUL, *);
			CASE(DIV, /);
			CASE(POW, ^);
			CASE(SIN, sin);
			CASE(COS, cos);
			CASE(LN, ln);
			CASE(LOG, log);
		}
		printf("\n");
	}
	else if(This->type == NUMB)
	{
		printf("NUMB\n");
		printf("Val = %lg\n", This->val);
	}
	else if(This->type == VAR)
	{
		printf("VAR\n");
		printf("Name = %s\n", This->name);
	}
	printf("}\n");
}

//=============================================================================

void DumpTreeToTex(Node* This,FILE* output)
{
	if(This->type == NUMB)
		fprintf(output, "%lg\n", This->val);
	else if(This->type == VAR)
		fprintf(output, "%s\n", This->name);
	else if(This->type == OP)
	{
		if(This->val == ADD)
		{
			fprintf(output, "{");

			DumpTreeToTex(This->branch1, output);

			fprintf(output, "} + {");

			DumpTreeToTex(This->branch2, output);
			
			fprintf(output, "}");
		}
		else if(This->val == SUB)
		{
			fprintf(output, "{");

			DumpTreeToTex(This->branch1, output);

			fprintf(output, "} - {");

			DumpTreeToTex(This->branch2, output);
			
			fprintf(output, "}");
		}
		else if(This->val == MUL)
		{
			fprintf(output, "{");

			bool BreaketWasOpen = false;
			if(This->branch1->val <= SUB && This->branch1->type == OP)
			{
				fprintf(output, "(");
				BreaketWasOpen = true;
			}
			DumpTreeToTex(This->branch1, output);
			
			if(BreaketWasOpen)
			{
				fprintf(output, ")");
				BreaketWasOpen = false;
			}

			fprintf(output, "} \\cdot {");

			if(This->branch2->val <= SUB && This->branch2->type == OP)
			{
				fprintf(output, "(");
				BreaketWasOpen = true;
			}

			DumpTreeToTex(This->branch2, output);

			if(BreaketWasOpen)
			{
				fprintf(output, ")");
				BreaketWasOpen = false;
			}
			
			fprintf(output, "}");
		}
		else if(This->val == DIV)
		{
			fprintf(output, "\\frac {");

			DumpTreeToTex(This->branch1, output);

			fprintf(output, "}{");

			DumpTreeToTex(This->branch2, output);

			fprintf(output, "}");
		}
		else if(This->val == POW)
		{
			fprintf(output, "{");

			bool BreaketWasOpen = false;
			if(This->branch1->type <= OP)
			{
				fprintf(output, "(");
				BreaketWasOpen = true;
			}

			DumpTreeToTex(This->branch1, output);
			
			if(BreaketWasOpen)
			{
				fprintf(output, ")");
				BreaketWasOpen = false;
			}			

			fprintf(output, "} ^ {");

			if(This->branch1->type <= OP)
			{
				fprintf(output, "(");
				BreaketWasOpen = true;
			}

			DumpTreeToTex(This->branch2, output);

			if(BreaketWasOpen)
			{
				fprintf(output, ")");
				BreaketWasOpen = false;
			}
			
			fprintf(output, "}");
		}
		else if(This->val == SIN)									
		{															
			fprintf(output, "\\sin{");								
			bool BreaketWasOpen = false;							
			if(This->branch1->type == OP && This->branch1->val <= POW)
			{														
				fprintf(output, "(");								
				BreaketWasOpen = true;								
			}														
			DumpTreeToTex(This->branch1, output);					
						if(BreaketWasOpen)					
			{												
				fprintf(output, ")");						
				BreaketWasOpen = false;						
			}												
			fprintf(output, "}");								
		}
		else if(This->val == COS)									
		{															
			fprintf(output, "\\cos{");								
			bool BreaketWasOpen = false;							
			if(This->branch1->type == OP && This->branch1->val <= POW)
			{														
				fprintf(output, "(");								
				BreaketWasOpen = true;								
			}														
			DumpTreeToTex(This->branch1, output);					
						if(BreaketWasOpen)					
			{												
				fprintf(output, ")");						
				BreaketWasOpen = false;						
			}												
			fprintf(output, "}");								
		}
		else if(This->val == LN)									
		{															
			fprintf(output, "\\ln{");								
			bool BreaketWasOpen = false;							
			if(This->branch1->type == OP && This->branch1->val <= POW)
			{														
				fprintf(output, "(");								
				BreaketWasOpen = true;								
			}														
			DumpTreeToTex(This->branch1, output);					
						if(BreaketWasOpen)					
			{												
				fprintf(output, ")");						
				BreaketWasOpen = false;						
			}												
			fprintf(output, "}");								
		}
		else if(This->val == LOG)
		{
			fprintf(output, "\\log_{(");
			DumpTreeToTex(This->branch1, output);
			fprintf(output, ")}");
			fprintf(output, "{");
			DumpTreeToTex(This->branch2, output);
			fprintf(output, "}");

		}
	}

}

//=============================================================================