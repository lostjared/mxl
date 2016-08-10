
#include<iostream>
#include<fstream>
#include<vector>
#include<mxl.h>
#include<map>
#include<string>
#include<sstream>

void custom_printHTML(mxlParser &p, std::ostream &out);



int main(int argc, char **argv)
{


	char *buf = 0;

	try
	{

		mxlParser p;

		std::fstream file;

		file.open("demo.mxl", std::ios::in |  std::ios::binary );

		file.seekg(0,std::ios::end);

		int pos = file.tellg();

		file.seekg(0, std::ios::beg);

		buf = new char [ pos + 1 ];


		file.read((char*)buf, pos);

		buf[pos] = 0;

		file.close();


		p.parseMXL(buf);
		custom_printHTML(p, std::cout);

	


		delete [] buf;

	} 
	catch (...)
	{

		if(buf != 0) delete [] buf;


	}

	return 0;

}



void custom_printHTML(mxlParser &p, std::ostream &out)
{


	out << "<HTML><TITLE>FreeES Information</TITLE><BODY><TABLE border=1 width=640  height=480><br/>";


	std::map<std::string, std::string>::iterator i;
	
	int e_count = 0;


	for(e_count = 0; e_count < 3; e_count++)
	{


		out << "<TR><TH>";
		std::ostringstream stream;

		stream << "entry"<<(e_count+1);
		for(i=p.begin_key(stream.str()); i!=p.end_key(stream.str()); i++)
		{

			out << "<b>" << i->second << "</b>"  <<"<br/>";

		}

		out<<"</TR></TH>";
	}

	out << "</TABLE></BODY></HTML>";

}