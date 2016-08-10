#include<iostream>
#include<mxl.h>



int main(int argc, char **argv)
{


	mxlParser p;

	if(!p.parseMXL_file("demo.mxl"))
	{

		std::cerr << "Error proccessing mxl file\n";
		return EXIT_FAILURE;

	}


	mxlArray varray;

	p.fetchArray(varray);

	for(unsigned int i = 0; i < varray.size(); i++)
	{

		std::cout << "Key: " <<  varray[i].first << "\n";
		
		for(subtag tag = p.begin_mxl(i, varray); tag != p.end_mxl(i, varray); tag++)
		{
			std::cout << tag->first << " = " << tag->second << "\n";
		}

	}
	return EXIT_SUCCESS;
}
