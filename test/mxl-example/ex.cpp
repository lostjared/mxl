/*


	MXL Example - Simple Phone Number Database

	written by jared


*/

#include<iostream>
#include<string>
#include<mxl.h>
#include<map>
#include<algorithm>


// this function creates a new tag within the mxl internal map
static void new_phone_tag(std::string name, std::string phone, mxlParser &p)
{


	p.create_tag(name); 
	p.getTags()[name].tag["phone"] = phone;
	
}


// this function allows the user to add a new tag or update a exisiting tag
// it then calls new_phone_tag to add the tag
static void print_options(mxlParser &p)
{

	std::cout << "Enter name: ";
	std::string name;
	getline(std::cin, name);
	fflush(stdin);
	std::cout << "Enter number: ";
	std::string phonenum;
	getline(std::cin, phonenum);
	new_phone_tag(name, phonenum, p);
	
}


// this function displays all the phone numbers
// currently within the database
// it simply loops through and uses the by index versues by name
// indexing of the numbers.
static void display_all(mxlParser &p)
{

	unsigned int i;

	for(i = 0; i < p.getkeybyindex_size(); i++)
	{

		tag_body &b = p.getkeybyindex(i);
		std::cout << b.field_name << " number is " << b.tag["phone"] << "\n";
	}


}


// this searchs the map for a name if it exists it prints the phone number
// if not it does not print the phone number instead prints a error message
static void display_find(mxlParser &p)
{

	std::string user_id;	
	std::cout << "Enter user name (case sensitive): ";
	getline(std::cin, user_id);

	fulltag t = p.getTags().find(user_id);

	if(t == p.getTags().end())
	{
		std::cout << "could not find name in database\n";
		return;
	}

	std::cout << " Phone Number for " <<  t->second.field_name << " is " << t->second.tag["phone"] << "\n";

}
// this function is called when the user is done using the program
// and outputs the file in mxl format for later use
// this is just for if they added new tags while the program
// was running
static void exit_save(mxlParser &p)
{

	
	std::cout  << "exit file name: ";
	std::string gline;
	getline(std::cin, gline);
	std::fstream file;
	file.open(gline.c_str(), std::ios::out);
	p.printMXL_byIndex(file);
	file.close();

}
// the main function
// where it all happens

int main(int argc, char **argv)
{


	std::cout << "Welcome to the libmxl example enter filename of db to load: ";
	std::string filename;
	getline(std::cin, filename);
	mxlParser p;
	p.parseMXL_file(filename);

	
	char c = 'y';
	while(c == 'y')
	{
		std::cout << "Options are :\n1 dispaly all numbers\n2 search\n3 enter number\n4 exit saving changes\n5 exit discarding changes\n";
		int op;
		std::cout << "enter option: ";
		std::string op_s;
		getline(std::cin, op_s);
		op = atoi(op_s.c_str());

			switch(op) 
			{
			case 1:
				display_all(p);
			break;

			case 2:
				display_find(p);
			break;

			case 3:
				print_options(p);
			break;
			case 4:
				exit_save(p);
				exit(EXIT_SUCCESS);
			break;
			case 5:
				exit(EXIT_SUCCESS);
			break;
			}


	}
	
	return EXIT_SUCCESS;
}


