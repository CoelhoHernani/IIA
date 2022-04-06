// ProcuraEngine.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include "knDamas.h"
#include "TProcuraConstrutiva.h"
int main(int argc, char* argv[])
{
	knDamas knDamas;
	

	TRand::srand(20);
	knDamas.TesteManual("knDamas",20);

	return 0;
}
