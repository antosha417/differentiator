
void Simplification(Node* This, Node* papent, int branch);

double Operation(double val1, double val2, double op);

void FindVar(Node* This, char** VarNames, int* cr);

Node* Differentiation(Node* This,const char* name);

//=============================================================================

Node* DiffADD(Node* This, const char* name);

Node* DiffSUB(Node* This, const char* name);

Node* DiffMUL(Node* This, const char* name);

Node* DiffDIV(Node* This, const char* name);

Node* DiffPOW(Node* This, const char* name);

Node* DiffLN(Node* This, const char* name);

Node* DiffSIN(Node* This, const char* name);

Node* DiffCOS(Node* This, const char* name);

Node* DiffLOG(Node* This, const char* name);

//=============================================================================

void Simplification(Node* This, Node* papent, int branch)
{
	assert(This);assert(branch > -1);
	
	Node* node = NULL;
	
	if(This->branch1 != NULL && This->branch2 != NULL)
	{
		Simplification(This->branch1, This, 1);
		Simplification(This->branch2, This, 2);
		

		if(This->type == OP)
		{
			if(This->branch1->type == NUMB && This->branch2->type == NUMB)
			{
				double val1 = This->branch1->val;
				double val2 = This->branch2->val;
				double val = Operation(val1, val2, This->val);
				node = CreateNode(NUMB, val, NULL, NULL, NULL);	
			}

			else if(This->branch1->type == NUMB)
		 	{
		 		if(This->branch1->val == 0)
		 		{
		 			if(This->val == ADD)
		 			{
		 				node = This->branch2;
		 			}

		 			else if(This->val == MUL || This->val == DIV)
		 			{
		 				node = CreateNode(NUMB, 0, NULL, NULL, NULL);
		 			}

		 		}
		 		else if(This->branch1->val == 1)
		 		{
		 			if(This->val == MUL)
		 			{
		 				node = This->branch2;
		 			}

		 			else if(This->val == POW)
		 			{
		 				node = This->branch1;
		 			}
		 		}
		 	}

		 	else if(This->branch2->type == NUMB)
		 	{
		 		if(This->branch2->val == 0)
		 		{
		 			if(This->val == ADD || This->val == SUB)
		 			{
		 				node = This->branch1;
		 			}

		 			else if(This->val == MUL)
		 			{
		 				node = This->branch2;
		 			}

		 			else if(This->val == DIV)
		 			{
		 				printf("Error: Division by zero is not allowed\n");
		 				exit(1);
		 			}

		 			else if(This->val == POW)
		 			{
		 				node = CreateNode(NUMB, 1, NULL, NULL, NULL);
		 			}
		 		}

		 		else if(This->branch2->val == 1)
		 		{
		 			if(This->val == MUL || This->val == DIV)
		 			{
		 				node = This->branch1;
		 			}

		 			else if(This->val == POW)
		 			{
		 				node = This->branch1;
		 			}
		 		}

		 	}
		}
	}


	else if(This->branch1 != NULL && This->branch2 == NULL)
	{
		Simplification(This->branch1, This, 1);
		
		if(This->type == OP)
			if(This->branch1->type == NUMB)
			{
				double val1 = This->branch1->val;
				double val = Operation(val1, 0, This->val);
				if(This->val <= POW)
					node = CreateNode(NUMB, val, NULL, NULL, NULL);

			}
			else if(This->branch1->type == VAR && 
			 strcmp(This->branch1->name, "e") == 0)
				if(This->val == LN)
				node = CreateNode(NUMB, 1, NULL, NULL, NULL);
	}


	if(node != NULL)
	{
		if(papent != NULL)
		{
			if(branch == 1)
				papent->branch1 = node;
			else if(branch == 2)
				papent->branch2 = node;
		}
		else
		{
			This->type = node->type;
			This->val = node->val;
			This->name = node->name;
			This->branch1 = node->branch1;
			This->branch2 = node->branch2;
		}
	}	
}

//=============================================================================

double Operation(double val1, double val2, double op)
{
	if(op == ADD)
		return val1 + val2;
	else if(op == SUB)
		return val1 - val2;
	else if(op == MUL)
		return val1 * val2;
	else if(op == DIV)
		return val1 / val2;
	/*else if(op == SIN)
		return sin(val1);
	else if(op == COS)
		return cos(val1);
	else if(op == LN)
		return log(val1);
	else if(op == LOG)
		return log(val2)/log(val1);*/
	else if(op == POW)
		return exp(log(val1)*val2);
}

//=============================================================================

void FindVar(Node* This, char** VarNames, int* cr)
{
	if(This->type == VAR)
	{
		int cr1 = 0;
		for (int i = 0; i < *cr; i++)
			if(strcmp(VarNames[i],This->name) == 0)
				cr1 = 1;
		if(strcmp(This->name, "{\\pi}") == 0 ||
		   strcmp(This->name, "e") == 0)
			cr1 = 1;
		if(cr1 == 0)
		{
			VarNames[*cr] = This->name;
			(*cr)++;
		}
	}
	if(This->branch1 != NULL)
		FindVar(This->branch1, VarNames, cr);
	if(This->branch2 != NULL)
		FindVar(This->branch2, VarNames, cr);
}

//=============================================================================

#define DIFF(x)							\
	if(This->val == x)					\
	{									\
		return Diff ## x(This, name);	\
	}

Node* Differentiation(Node* This,const char* name)
{
	assert(This);

	Node* node = NULL;

	if(This->type == NUMB || (This->type == VAR && strcmp(This->name, name) != 0))
	{
		node = CreateNode(NUMB, 0, NULL, NULL, NULL);
	}
	else if(This->type == VAR)
	{
		if(strcmp(This->name, "e") != 0 && strcmp(This->name, "{\\pi}") != 0)
			node = CreateNode(NUMB, 1, NULL, NULL, NULL);
		else
			node = CreateNode(NUMB, 0, NULL, NULL, NULL);
	}
	else if(This->type == OP)
	{
		DIFF(ADD);
		DIFF(SUB);
		DIFF(MUL);
		DIFF(DIV);
		DIFF(POW);
		DIFF(SIN);
		DIFF(COS);
		DIFF(LN);
		DIFF(LOG);
	}
	return node;
}

Node* DiffADD(Node* This, const char* name)
{
	Node* diff1 = Differentiation(This->branch1, name);
	Node* diff2 = Differentiation(This->branch2, name);
	return CreateNode(OP, ADD, NULL, diff1, diff2);
}

Node* DiffSUB(Node* This, const char* name)
{
	Node* diff1 = Differentiation(This->branch1, name);
	Node* diff2 = Differentiation(This->branch2, name);
	return CreateNode(OP, SUB, NULL, diff1, diff2);
}


Node* DiffMUL(Node* This, const char* name)
{
	Node* diff1 = Differentiation(This->branch1, name);
	Node* diff2 = Differentiation(This->branch2, name);

	Node* node1 = CreateNode(OP, MUL, NULL, diff1, This->branch2);
	Node* node2 = CreateNode(OP, MUL, NULL, diff2, This->branch1);

	return CreateNode(OP, ADD, NULL, node1, node2);
}

Node* DiffDIV(Node* This, const char* name)
{
	Node* diff1 = Differentiation(This->branch1, name);
	Node* diff2 = Differentiation(This->branch2, name);

	Node* node1 = CreateNode(OP, MUL, NULL, diff1, This->branch2);
	Node* node2 = CreateNode(OP, MUL, NULL, diff2, This->branch1);

	Node* up = CreateNode(OP, SUB, NULL, node1, node2);

	Node* two = CreateNode(NUMB, 2, NULL, NULL, NULL);

	Node* down = CreateNode(OP, POW, NULL, This->branch2, two);

	return CreateNode(OP, DIV, NULL, up, down);
}

Node* DiffPOW(Node* This, const char* name)
{
	Node* node1 = CreateNode(OP, LN, NULL,This->branch1, NULL);

	Node* diff1 = Differentiation(node1, name);

	Node* node2 = CreateNode(OP, MUL, NULL, diff1, This->branch2);

	Node* diff2 = Differentiation(This->branch2, name);

	Node* node3 = CreateNode(OP, MUL, NULL, node1, diff2);

	Node* node4 = CreateNode(OP, ADD, NULL, node2, node3);

	return CreateNode(OP, MUL, NULL, This, node4);
}


Node* DiffLN(Node* This, const char* name)
{
	Node* one = CreateNode(NUMB, 1, NULL, NULL, NULL);

	Node* node1 = CreateNode(OP, DIV, NULL, one ,This->branch1);

	Node* diff1 = Differentiation(This->branch1, name);
	
	return CreateNode(OP, MUL, NULL, node1, diff1);
}

Node* DiffSIN(Node* This, const char* name)
{

	Node* diff1 = Differentiation(This->branch1, name);

	Node* node1 = CreateNode(OP, COS, NULL, This->branch1, NULL);

	return CreateNode(OP, MUL, NULL, node1, diff1);
}

Node* DiffCOS(Node* This, const char* name)
{
	Node* diff1 = Differentiation(This->branch1, name);

	Node* node1 = CreateNode(OP, SIN, NULL, This->branch1, NULL);

	Node* MinusOne = CreateNode(NUMB, -1, NULL, NULL, NULL);

	Node* node2 = CreateNode(OP, MUL, NULL, MinusOne, node1);

	return CreateNode(OP, MUL, NULL, node2, diff1);
}


Node* DiffLOG(Node* This, const char* name)
{

	
	Node* node3 = CreateNode(OP, LN, NULL,This->branch1, NULL);

	Node* node4 = CreateNode(OP, LN, NULL,This->branch2, NULL);


	Node* diff1 = Differentiation(node3, name);
	Node* diff2 = Differentiation(node4, name);

	Node* node1 = CreateNode(OP, MUL, NULL, diff1, node4);
	Node* node2 = CreateNode(OP, MUL, NULL, diff2, node3);

	Node* up = CreateNode(OP, SUB, NULL, node1, node2);

	Node* two = CreateNode(NUMB, 2, NULL, NULL, NULL);

	Node* down = CreateNode(OP, POW, NULL, node4, two);

	return CreateNode(OP, DIV, NULL, up, down);
}