#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
//=============================================================================

#ifndef TODO
#define TODO 
#endif

#define ASSERT_OK(type, what)										\
if(!type ## Ok(what))	 											\
	{																\
		type ## Dump(what, #what);									\
		printf(#type " Failed in %s, %d.\n",__FILE__, __LINE__ );	\
		abort();													\
	}


#define HELLO(x)\
	printf("\n\n#---- " #x " %s %s ----#\n\n", __DATE__, __TIME__); 


#define COMMENT(x)\
	printf("\n\n" "#" #x "\n");

//=============================================================================