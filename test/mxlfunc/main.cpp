/*

	MXL Function Example
	written by Jared Bruni
	this program is released under the conditions of the GNU GPL

*/

#include<iostream>
#include<mxl.h>

// error exit, this is called to abort the program, since its just a simple example
void error_exit(const char *str)
{
	perror(str);
	exit(-1);

}

// this is the callback function that is pointed to by the MXL file
void *print_string(void *ptr)
{

	std::map<std::string,std::string> *tag = (std::map<std::string, std::string> *) ptr;
	std::cout << tag->operator[]("text") << "\n";



}


// these two arrays are very important
// the first is the mxl function array, contains the functions that you want to have called back from the MXL file
mxlfunc func_ptrs[] = { print_string, 0 };

// second is the MXL internal name of the tag, its just the name of the tag you described with a @ symbol before it
// to seperate them from the normal tags
char *func_array[] = { "@print_tag", 0 };

// this is a simple tag look up function, youll prob want to write your own
int tag_lookup(const char *str)
{
	int i;

	for(i = 0; func_array[i] != 0; i++)
	{

		if(strcmp(str, func_array[i]) == 0)
			return i;

	}

	return -1;

}



// the main function

int main(int argc, char **argv)
{

	mxlParser p; // make a instance of the mxlParser

	if(p.parseMXL_file("func.mxl")) // parse from a file rather than from string
	{

		std::cout << "successfully parsed func.mxl \n";

		tag_body tag;

		tag = p.getTags()["OutputTag"];

		// call a function tag

		std::string func_ptr = tag.tag["print"]; // we put the name of the function in a item called print

		int func_id =  0;

		if((func_id = tag_lookup(func_ptr.c_str())) == -1) // call are look up function
		{
			std::cout << "function tag not present\n";
			error_exit(func_ptr.c_str());
		}

		func_ptrs[func_id]((void*)&tag.tag); // call the function from the func_ptr table

		std::cout << "Successfully called function tag";
	}
	else 
	{
		error_exit(" could not parse mxl file ");
	}


	return 0;
}

// - Jared
