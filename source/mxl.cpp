/*

	MXL - a simple language for creating tables of data 
	stored in C++ containers

	written by Jared Bruni ( jared@lostsidedead.biz )

	under the GNU GPL

*/

#include "mxl.h"
#include<string.h>




tag_body::tag_body()

{
}



mxlParser::mxlParser()
{

	tag_count = 0;

}


mxlParser::mxlParser(std::string str)
{

	tag_count = 0;
	parseMXL(str);

}

std::string mxlParser::operator()(std::string tag, std::string key)
{

	std::string value;
	value = tags[tag].tag[key];
	return value;

}
std::map<std::string, std::string>::iterator mxlParser::begin_key(std::string tag) {
	return tags[tag].tag.begin();
}



std::map<std::string, std::string>::iterator mxlParser::end_key(std::string tag)
{
	return tags[tag].tag.end();
}

std::string mxlParser::getstringfromkey(std::string tag, std::string key)
{

	return this->operator()(tag,key);


}

void mxlParser::clear()
{

	if(!tags.empty())
	{

		tags.erase(tags.begin(), tags.end());
		tag_count = 0;


	}

}

int mxlParser::parseMXL(std::string str)
{

	mascToken token;

	mascInit(&token);

	token.source_length = str.length();

	int rt_value = 0;

	while( (rt_value = mascLex(&token, str.c_str())) != MID_NULL )
	{

		if(rt_value == MID_SKIP) continue;
			

		if(strcmp(token.token, "tag") == 0)
		{


			if(!grabTag("", &token, str))
			{


				std::cout << "error  proccessing tag\n";
				return 0;	

			}

		}		
	
		mascTokenReset(&token);
	}
	
	free(token.token);
	return 1;

}


int mxlParser::getNextToken(mascToken *token, std::string str)
{
	mascTokenReset(token);
	int rt;
	while ( (rt = mascLex(token, str.c_str()) ) == MID_SKIP ) mascTokenReset(token) ;
	return rt;
}

int mxlParser::grabTag(std::string name, mascToken *token, std::string str)
{
	getNextToken(token,str);
	
	std::string id = token->token;
	
	name += id;
	
	tags[name].field_name = name;
	tags[name].tags_present = ++tag_count;
	tag_set.push_back( name );
	
	bool active = true;
	
	getNextToken(token,str);
	if(strcmp(token->token, "{") != 0)
	{

			std::cout << "tag missing opening identifier...instead found" << token->token << "\n";
			return 0;

	}

	getNextToken(token, str);

	int  rz =   1;	

	
	while( active == true && rz != MID_NULL )
	{

		std::string strX = token->token;
		if(strX == "tag")
		{

			std::string temp_name = name;
			temp_name += ".";
			int rt = grabTag(temp_name, token, str);
				
			if(rt == 0) return 0;

			rz = getNextToken(token,str);
			continue;

		}

		if(strX == "}") return 1;

		rz =  getNextToken(token, str);
		
		std::string eq = token->token;

		if(strcmp(token->token, "=") != 0 && strcmp(token->token, "==") != 0)
		{
			std::cout << "name: " << name << " subtag missing equal sign instead found: " << token->token << "\n";
			return 0;
		}

		rz = getNextToken(token,str);
		if(strcmp(eq.c_str(), "==") == 0)
		tags[name].tag[strX] = std::string("@") + std::string(token->token);
		else tags[name].tag[strX] = token->token;

		rz = getNextToken(token,str);

		if(strcmp(token->token, "}") == 0)
				break;
	}

	return 1;

}

void mxlParser::printAscii(std::ostream& out)
{

	std::map<std::string, std::string>::iterator vi;
	std::map<std::string, tag_body>::iterator p;


	for(p = tags.begin(); p!=tags.end(); p++)
	{

		out << "[" << p->first  << "] identifier\n";
		for(vi = p->second.tag.begin(); vi != p->second.tag.end(); vi++)
		{
			out << vi->first <<  ": " << vi->second <<  "\n";

		}

	}

}


void mxlParser::printMXL_byIndex(std::ostream &out)
{

	int i;

	for(i = 0; i < tag_set.size(); i++)
	{

		tag_body &body = getkeybyindex(i);

		out << "tag \"" << tag_set[i] << "\"\n{\n\n";
		std::map<std::string,std::string>::iterator it;

		for(it= body.tag.begin(); it != body.tag.end();  it++)
		{
			out << it->first  << " = " << "\""<<it->second << "\"\n";

		}
		out << "\n}\n";
	}

}


void mxlParser::printHTML_byIndex(std::ostream &out)
{

	out << "<HTML><TITLE>MXL OUTPUT</TITLE><BR/><BR/><br>";

	out <<  "<body><font style=\"font-family:monospace;font-size=12px;color:rgb(0,0,200);\"><table border=4 width=640  height=480><br/>";
	
	std::map<std::string, std::string>::iterator vi;
	unsigned int i = 0;


	for(i = 0; i < tag_set.size(); i++)
	{

		tag_body &b = getkeybyindex(i);

		out << "<tr><th>";

		out << "<p align=\"left\">";

		out << "<b>" << b.field_name  << "</b>    tag <br/>";
		for(vi = b.tag.begin(); vi != b.tag.end(); vi++)
		{
			std::cout << vi->first <<  " = " << vi->second <<  "<br/>";
		}

		out << "</p>";
		out << "</tr></th>";
	}

	out << "</TABLE></FONT></BODY></HTML>";

}

void mxlParser::printMXL(std::ostream &out)
{
	std::map<std::string, std::string>::iterator etag_it;
	std::map<std::string, tag_body>::iterator p;

	for(p = tags.begin(); p != tags.end(); p++)
	{

		out << "tag \"" << p->first << "\"" << "{\n\n";
		for(etag_it = p->second.tag.begin(); etag_it != p->second.tag.end(); etag_it++)
		{

			out << etag_it->first << " = " << "\"" << etag_it->second << "\"\n";
		}
		out << "\n\n}\n\n";
	}


}

void mxlParser::printHTML(std::ostream &out)
{


	out << "<HTML><TITLE>MXL OUTPUT</TITLE><BR/><BR/><br>";

	out <<  "<body><font style=\"font-family:monospace;font-size=12px;color:rgb(0,0,200);\"><table border=4 width=640  height=480><br/>";
	
	std::map<std::string, std::string>::iterator vi;
	std::map<std::string, tag_body>::iterator p;


	for(p = tags.begin(); p!=tags.end(); p++)
	{
		out << "<tr><th>";

		out << "<p align=\"left\">";

		out << "<b>" << p->first  << "</b>    tag <br/>";
		for(vi = p->second.tag.begin(); vi != p->second.tag.end(); vi++)
		{
			out << vi->first <<  " = " << vi->second <<  "<br/>";
		}

		out << "</p>";

		out << "</tr></th>";
	}

	out << "</TABLE></FONT></BODY></HTML>";

}


int mxlParser::parseMXL_file(std::string file_name)
{
	std::fstream file;
	file.open(file_name.c_str(), std::ios::in | std::ios::binary);
	if(!file.is_open()) return 0;
	file.seekg(0, std::ios::end);
	size_t len = file.tellg();
	file.seekg(0, std::ios::beg);
	char *temp = new char [ len + 1 ];
	file.read((char*)temp, len );
	temp[len] = 0;
	std::string buf = temp;
	delete [] temp;
	file.close();
	return parseMXL(buf);

}

tag_body &mxlParser::getkeybyindex(const unsigned int i)
{
	return tags[tag_set[i]];
}

unsigned int mxlParser::getkeybyindex_size()
{

	return tag_set.size();

}

bool mxlParser::create_tag(std::string name)
{

	if(tags[name].field_name == "")
	tag_set.push_back(name);
	else return false;

	tags[name].field_name = name;
	tags[name].tags_present = ++tag_count;
	return true;

}

bool mxlParser::fetchArray(mxlArray &v)
{


	if(!v.empty()) v.erase(v.begin(), v.end());


	for(unsigned int i = 0; i < tag_set.size(); i++)
	{

		tag_body &body = getkeybyindex(i);
		std::pair<std::string, tag_body> ins(body.field_name, body);
		v.push_back(ins);

	}


	return true;
}


subtag mxlParser::begin_mxl(int index, mxlArray &v)
{
	return v[index].second.tag.begin();

}
subtag mxlParser::end_mxl(int index, mxlArray &v)
{
	return v[index].second.tag.end();
}

