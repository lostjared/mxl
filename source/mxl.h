/*

	MXL - a simple language for creating tables of data 
	stored in C++ containers

	written by Jared Bruni ( jared@lostsidedead.biz )

	under the GNU GPL

*/

#ifndef __MXL__H__
#define __MXL__H__

#include<masc++.h>
#include<iostream>
#include<string>
#include<map>
#include<fstream>
#include<vector>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>


	typedef void *(*mxlfunc)(void *);
	
	class tag_body
	{
	public:
		tag_body();
		std::map<std::string,std::string> tag;
		int tags_present;
		std::string field_name;
	};

	typedef std::map<std::string, tag_body>::iterator fulltag;
	typedef std::vector<std::pair<std::string, tag_body> > mxlArray;
	typedef std::map<std::string,std::string>::iterator subtag;

	template<typename Container>
	class mxl_iterator {
	Container container;
	unsigned int offset;
	public:
		tag_body members;
		mxl_iterator();
		mxl_iterator(Container &c);
		Container &operator=(Container &c);
		Container &operator=(mxl_iterator<Container> &c);
		Container &operator++();
		Container &operator++(int);
		Container &operator--();
		Container &operator--(int);
		tag_body &operator*();
		tag_body *operator->();
		bool operator!=(mxl_iterator<Container> &c);
		bool operator!=(int x);
		bool operator==(mxl_iterator<Container> &c);
		bool operator==(int x);

		bool operator<(mxl_iterator<Container> &c);
		bool operator<(int x);
		bool operator>(mxl_iterator<Container> &c);
		void assign(mxl_iterator<Container> &c);
		void lookup();
		bool locateTag(std::string str);
		void start() { offset = 0; }
		int  stop()  { return container.size(); }

	};
	


	class mxlParser {


	public:

		typedef mxl_iterator<mxlParser> iterator;

		mxlParser();
		mxlParser(std::string str);
		int parseMXL(std::string str);
		int parseMXL_file(std::string file_name);

		void printAscii(std::ostream& out);
		void printHTML(std::ostream& out);
		void printMXL(std::ostream &out);
		void printMXL_byIndex(std::ostream &out);
		void printHTML_byIndex(std::ostream &out);
		

		void clear();


		std::string getstringfromkey(std::string tag, std::string key);
		std::string operator()(std::string tag, std::string key);

		std::map<std::string, std::string>::iterator begin_key(std::string tag);
		std::map<std::string, std::string>::iterator end_key(std::string tag);
		tag_body &getkeybyindex(const unsigned int i);
		tag_body &operator[](int i) { return tags[tag_set[i]]; }
		unsigned int getkeybyindex_size();
		std::map<std::string, tag_body>& getTags() { return tags; }
		bool create_tag(std::string);
		bool fetchArray(mxlArray &v);
		subtag begin_mxl(int index, mxlArray &v);
		subtag end_mxl(int index, mxlArray &v);
		const unsigned int size() const { return tag_set.size(); } 

		std::vector< std::string > tag_set;

	protected:

		int grabTag(std::string name, mascToken *token,std::string str);
		int getNextToken(mascToken *token, std::string str);

		std::map<std::string, tag_body> tags;
	
		int tag_count;

	};

	

	template<typename Container>
	mxl_iterator<Container>::mxl_iterator()
	{
		offset = 0;
	}
	
	template<typename Container>
	mxl_iterator<Container>::mxl_iterator(Container &c)
	{
	
		offset = 0;
		container = c;

	}

	template<typename Container>
	Container &mxl_iterator<Container>::operator=(Container &c)
	{
		container = c;
		return *this;
	}


	template<typename Container>
	void mxl_iterator<Container>::assign(mxl_iterator<Container> &c)
	{

		c = c.container;
		offset = c.offset;
	}

	template<typename Container>
	Container &mxl_iterator<Container>::operator=(mxl_iterator<Container> &c)
	{

		assign(c);
		return *this;

	}

	template<typename Container>
	Container &mxl_iterator<Container>::operator++()
	{
		if(offset < container.size()) offset++;
		lookup();
		return *this;
	}

	template<typename Container>
	Container &mxl_iterator<Container>::operator--()
	{
		if(offset > 0) offset--;
		lookup();
	}

	template<typename Container>
	Container &mxl_iterator<Container>::operator++(int)
	{
		if(offset < container.size()) ++offset;
		lookup();
	}

	template<typename Container>
	Container &mxl_iterator<Container>::operator--(int)
	{
		if(offset > 0) --offset;
		lookup();
	}

	template<typename Container>
	void mxl_iterator<Container>::lookup()
	{

		if(offset < container.size())
		members = container[offset];

	}

	template<typename Container>
	tag_body &mxl_iterator<Container>::operator*()
	{
		if(offset < container.size())
		members = container[offset];
		return members;

	}

	
	template<typename Container>
	tag_body *mxl_iterator<Container>::operator->()
	{
		if(offset < container.size())
		members = container[offset];
		return &members;

	}


	template<typename Container>
	bool mxl_iterator<Container>::operator!=(mxl_iterator<Container> &c)
	{

		if(offset != c.offset) return true;

		return false;
	}
	
	template<typename Container>
	bool mxl_iterator<Container>::operator==(mxl_iterator<Container> &c)
	{
		if(offset == c.offset)
		return true;

		return false;
	}

	template<typename Container>
	bool mxl_iterator<Container>::operator<(mxl_iterator<Container> &c)
	{
		if(offset < c.offset)
		return true;

		return false;
	}


	template<typename Container>
	bool mxl_iterator<Container>::operator>(mxl_iterator<Container> &c)
	{
		if(offset > c.offset)
		return true;

		return false;
	}

	template<typename Container>
	bool mxl_iterator<Container>::locateTag(std::string name)
	{

		unsigned int i = 0;

		bool found = false;

		for(i = 0; i < container.tag_set.size(); i++)
		{

			if(container.tag_set[i] == name) 
			{

				found = true;
				break;

			}

		} 


		if(found == false) return false;

		offset = i;
		lookup();
		return found;
	}

	template<typename Container>
	bool mxl_iterator<Container>::operator<(int x)
	{
		if(offset < x) return true;
		return false;
	}

	template<typename Container>
	bool mxl_iterator<Container>::operator==(int x)
	{
		if(offset == x) return true;
		return false;
	}

	template<typename Container>
	bool mxl_iterator<Container>::operator!=(int x)
	{
		if(offset != x) return true;
		return false;
	}

#endif





