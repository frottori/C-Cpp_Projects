#ifndef STUDENT_H
#define STUDENT_H
#include "Course.h"
#include <vector>
using namespace std;

class Student 
{
  private:
	char* AM;
	string Name;
	unsigned int Term;
	vector<Course*> Subject;
	vector< pair<float, Course*> > Grades;
	
  public:
  	
	Student(const char*,string,unsigned int=1);
  	Student(const Student &);
  	~Student();
  	
  	void setAM(const char*);
  	char* getAM();
  	
  	void setName(string);
  	string getName();
  	
  	void setTerm(unsigned int);
  	unsigned int getTerm();
  	
  	void PrintSubject(ostream &); 
  	
  	void setGrades(float,Course *);
    float getGrades(Course *);
  	
  	Student operator =(const Student &);     //Ισοδύναμος Φοιτητή
  	void operator +=(Course*);   //Πρόσθεση Μαθήματος 
  	void operator++(int);      //Αύξηση Εξαμήνου
  	bool operator == (const Student &);        
  	bool operator != (const Student &);      
  	bool operator < (const Student &);      
  	bool operator <= (const Student &);       
  	bool operator > (const Student &);       
  	bool operator >= (const Student &);       
  	friend ostream& operator << (ostream &, const Student &);
};
#endif