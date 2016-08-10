/*

	a simple MXL iterator example
	written by Jared Bruni ( jared@lostsidedead.biz )

	under the GNU GPL

*/

#include<iostream>
#include<mxl.h>

int main(int argc, char **argv)
{
	mxlParser p; // instance of mxlParser class

	if(!p.parseMXL_file("demo.mxl")) // parse the demo mxl file
	{
		return EXIT_FAILURE; // return failure if parsing failed
	}

	mxlParser::iterator it(p); // declare iterator pass instance of mxlParser class to constructor

	for(it.start(); it != it.stop(); it++) // simple loop through container with iterator one step at a time
	{
		std::cout << "tag id: " << it->field_name << "\n"; 

		for(subtag tx = it->tag.begin(); tx != it->tag.end(); tx++) // subtag (actual fields) for statement to print out values
		{
			std::cout << tx->first << " = " << tx->second << "\n"; // actual stored values of the mxl tags
		}

	}

	return EXIT_SUCCESS; // success
}


