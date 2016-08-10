
#include<iostream>
#include<fstream>
#include<mxl.h>


int main(int argc, char **argv)
{

	mxlParser p;

	std::fstream file;

	file.open("demo.mxl", std::ios::in |  std::ios::binary );

	file.seekg(0,std::ios::end);

	int pos = file.tellg();

	file.seekg(0, std::ios::beg);

	char *buf = new char [ pos + 1 ];


	file.read((char*)buf, pos);

	buf[pos] = 0;

	file.close();


	p.parseMXL(buf);

	if(argc == 2 && strcmp(argv[1], "--html") == 0)
		p.printHTML_byIndex(std::cout);
	else
		p.printMXL_byIndex(std::cout);

	delete [] buf;

	return 0;

}



