#ifndef MAX
#define MAX 100000
#endif

//=============================================================================

void MadeReport(Node* phrase, char* VarNames[MAX],int* NumbOfVar, int NumbOfInitVar, Var* Values);

double Operat(double val1, double val2, double op);

double Simpl(Node* This, Node* papent, int branch);

//=============================================================================

void MadeReport(Node* phrase, char* VarNames[MAX],int* NumbOfVar, int NumbOfInitVar, Var* Values)
{
	FILE* output = fopen("report.tex", "w");

	fprintf(output, "\\documentclass[14pt,a4paper]{scrartcl}\n\
					 \\usepackage[utf8]{inputenc}\n\
					 \\usepackage[english,russian]{babel}\n\
					 \\usepackage{indentfirst}\n\
					 \\usepackage{misccorr}\n\
					 \\usepackage{graphicx}\n\\usepackage{amsmath}\n\
					 \\begin{document}\n");
	fprintf(output, "\\begin{center}\\begin{large}{\\bfНахождение погрешности исследуемой величины заданной формулой.}\\end{large}\\end{center}\n\n");
	
	fprintf(output, "Исходная формула:\n\n $$F(");
	for (int i = 0; i < *NumbOfVar; i++)
	{
		if(i != 0)
			fprintf(output, ",");
		fprintf(output, "%s", VarNames[i]);
	}
	fprintf(output, ") = ");


	DumpTreeToTex(phrase, output);
	fprintf(output, "$$\n\n");
	fprintf(output, "Для вычисления погрешности используем формулу: \n\n");
	fprintf(output, "$$\\sigma _{F} = \\sqrt{");

	for (int i = 0; i < *NumbOfVar; i++)
	{
		if(i != 0)
			fprintf(output, "+");
		fprintf(output, "\\Bigl(\\frac{\\partial F}{\\partial %s}\\Bigl)^2\\cdot{\\sigma _%s}^2", VarNames[i], VarNames[i]);
	}

	fprintf(output, "}$$\n\n");
	fprintf(output, "Найдём частную производную по каждoму из параметров:\n\n");

	double MeasurementError = 0;

	double FuncValue = Simpl(phrase, NULL, 0);

	for (int i = 0; i < *NumbOfVar; i++)
	{
		Node* dp = NULL;	

		fprintf(output, "$$\\frac{\\partial F}{\\partial %s}=", VarNames[i]);

		dp = Differentiation(phrase, VarNames[i]);
		

		Simplification(dp, NULL, 0);

		DumpTreeToTex(dp, output);
		fprintf(output, "$$");

		if(dp->branch1 != NULL && dp->branch2 != NULL)
		{
			fprintf(output, "Подставляя значения получим:  \n");
			fprintf(output, "$(\\frac{\\partial F}{\\partial %s})^2\\cdot{\\sigma_%s}^2= ", VarNames[i], VarNames[i]);

			double res = 1;
			for (int j = 0; j < NumbOfInitVar; j++)
				if(strcmp(VarNames[i], Values[j].name) == 0)
					res = Values[j].sigma*Values[j].sigma;
			double temp = Simpl(dp, NULL, 0);
			res *= temp*temp;
			MeasurementError += res;
			fprintf(output, "%lg", res);
		
			fprintf(output, "$");
		}
	}

	fprintf(output, "\n\nОкончательно получаем:");

	fprintf(output, "$$\\sigma _{F} = %lg$$", sqrt(MeasurementError));

	fprintf(output, "Тогда искомая величина принимет вид:\n");

	fprintf(output, "$$ F = %lg \\pm %lg$$",FuncValue ,sqrt(MeasurementError));

	fprintf(output, "\n\\end{document}\n");

	fclose(output);
}

//=============================================================================

double Simpl(Node* This, Node* papent, int branch)
{
	assert(This);assert(branch > -1);
	
	double res = 0;
	
	if(This->branch1 != NULL && This->branch2 != NULL)
	{
		double br1 = Simpl(This->branch1, This, 1);
		double br2 = Simpl(This->branch2, This, 2);
		

		if(This->type == OP)
		{
			res = Operat(br1, br2, This->val);
		}
	}
	else if(This->branch1 != NULL && This->branch2 == NULL)
	{
		double br1 = Simpl(This->branch1, This, 1);
		
		if(This->type == OP)
		{
			res = Operat(br1, 0, This->val);
		}
	}
	else if(This->branch1 == NULL && This->branch2 == NULL)
	{
		res = This->val;
	}
	return res;	
}

//=============================================================================

double Operat(double val1, double val2, double op)
{
	if(op == ADD)
		return val1 + val2;
	else if(op == SUB)
		return val1 - val2;
	else if(op == MUL)
		return val1 * val2;
	else if(op == DIV)
	{	
		if(val2 != 0)
			return val1 / val2;
		else 
			return 0;
	}
	else if(op == SIN)
		return sin(val1);
	else if(op == COS)
		return cos(val1);
	else if(op == LN)
		return log(val1);
	else if(op == LOG)
	{
		if(val1 > 0 && val2 > 0 && val1 != 1)
			return log(val2)/log(val1);
		else
			return 0;
	}
	else if(op == POW)
	{
		if(val1 > 0)
			return exp(log(val1)*val2);
		else
			return 0;
	}
	else
		return -256;
}

//=============================================================================
