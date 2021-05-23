#include <iostream>
#include <fstream>

#include "../../lib/utest.h"

//#define _FILE_OUTPUT

int main()
{
#ifdef _FILE_OUTPUT
	std::ofstream ofile("utest.log");
#else
	std::ostream& ofile = std::cout;
#endif
	utest::RUN_ALL(ofile);

	ofile.flush();
#ifdef _FILE_OUTPUT
	ofile.close();
#endif
	return 0;
}