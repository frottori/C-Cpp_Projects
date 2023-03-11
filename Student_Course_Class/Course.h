#ifndef COURSE_H
#define COURSE_H

using namespace std;

class Course
{
  private :
	string ID;
	string Name;
	unsigned int Term;
	
  public:
  	Course(string,string,unsigned int);
  	Course();
  	
  	void setID(string);
  	string getID();
  	
  	void setName(string);
  	string getName();
  	
  	void setTerm(unsigned int);
  	unsigned int getTerm();
	void setATR(string,string,unsigned int);
	
	friend ostream& operator << (ostream &, const Course &);
};
#endif


