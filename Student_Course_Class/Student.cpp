#include <string.h>
#include <string>
#include <iostream>
#include <iterator>
#include "Student.h"
#include "Course.h"


Student :: Student(const char* p,string s,unsigned int j)
{
	int i,l;
	l=strlen(p);
	if (l)
	{
		AM = new char [l];
		strcpy(AM,p);
	}	
	Name = s;
	Term=j;	 
    
}

Student :: Student (const Student &st)
{
	Term = st.Term;
	Name=st.Name;
	AM=new char[strlen(st.AM)];
	strcpy(AM,st.AM);	
}


Student :: ~Student()
{
	delete[] AM;	
}

void Student :: setAM(const char* s)
{
    strcpy(AM,s);
}
char* Student :: getAM()
{
	return AM;
}
void Student::setName(string s)
{
	Name = s;
}
string Student:: getName()
{
	return Name;
}
void Student:: setTerm(unsigned int s)
{
	Term=s;
}
unsigned int Student:: getTerm()
{
	return Term;
}


void Student :: PrintSubject(ostream & out)
{

    for (int i=0; i<Subject.size(); i++)
    out << *Subject[i] << endl;
}

void Student :: setGrades(float g,Course *x)
{
	if (g>=5)       //αν πέρασε
	Grades.push_back(make_pair(g,x));
}

float Student :: getGrades(Course * x)
{
	int a=0;
	for (int i=0;i<Grades.size(); i++)
	       if (Grades[i].second==x)
	              a=Grades[i].first;      
	return a;
}



Student Student :: operator =(const Student & st)
{

    if(this==&st)
    return *this;
	delete[] AM;;
	Term = st.Term;
	Name=st.Name;
	AM=new char[strlen(st.AM)];
	strcpy(AM,st.AM);
	Subject=st.Subject;
	Grades=st.Grades;
    return *this;
}

void Student :: operator +=(Course *x)
{
	Subject.push_back(x);
}


void Student:: operator++(int)
{
	Term = Term + 1;
}

bool Student :: operator == (const Student & st)
{
	if (Term==st.Term)
	    return true;
	else 
	    return false;
}

bool Student :: operator != (const Student &st)
{
	if (Term!=st.Term)
	    return true;
	else 
	    return false;	
}

bool Student :: operator < (const Student &st)
{
	if (Term<st.Term)
	    return true;
	else 
	    return false;
}  
      
bool Student :: operator <= (const Student &st)    
{
	if (Term<=st.Term)
	    return true;
	else 
	    return false;
	
}   
bool Student :: operator > (const Student &st)
{
	if (Term>st.Term)
	    return true;
	else 
	    return false;
}
bool Student :: operator >= (const Student & st)
{
	if (Term>=st.Term)
	    return true;
	else 
	    return false;
}

ostream& operator << (ostream &out, const Student &c)
{

	out << c.AM<<" ";
	out << c.Name<<" ";
	out << c.Term<<" ";
	if (!c.Grades.empty())
	{  out <<endl<<endl;
	float mo=0;
	for (int i=0;i<c.Grades.size(); i++)
	  {
	    out << *c.Grades[i].second << " Βαθμός :" << c.Grades[i].first<<endl;
	    mo += c.Grades[i].first;
      }
      out <<endl <<"Μεσος όρος : " << mo/c.Grades.size();
    }
    return out;
}