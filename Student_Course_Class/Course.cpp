#include <string.h>
#include <string>
#include <iostream>
#include "Course.h"

using namespace std;

Course :: Course(string p,string s,unsigned int j)
{
	ID = p;	
	Name = s;
	Term=j;	 	
}

Course :: Course ()
{
	
}

void Course :: setID(string s)
{
    ID= s;
}

string Course :: getID()
{
	return ID;
}

void Course::setName(string s)
{
	Name = s;
}

string Course:: getName()
{
	return Name;
}

void Course:: setTerm(unsigned int s)
{
	Term=s;
}
unsigned int Course:: getTerm()
{
	return Term;
}

void Course :: setATR(string s,string a,unsigned int j)
{
	ID=s;
	Name = a;
	Term=j;
}


ostream& operator << (ostream &out, const Course &c)
{
	out << c.ID<<" ";
	out << c.Name<<" ";
	out << c.Term<<" ";
    return out;
}


