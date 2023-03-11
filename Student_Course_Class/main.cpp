#include <iostream>
#include <fstream> 
#include "Student.h"
#include "Course.h"

int main(int argc,char**argv)
{
	Course *s1;                          //Δυναμικός Πίνακας 1ου εξαμήνου
    s1=new Course [6]; 
	s1->setATR("ICE-1001","Μαθηματική Ανάλυση I",1);
    (s1+1)->setATR("ICE-1002","Γραμμική Άλγεβρα",1);
    (s1+2)->setATR("ICE-1003","Εισαγωγή στην επιστήμη των Υπολογιστών",1);
    (s1+3)->setATR("ICE-1004","Προγραμματισμός Υπολογιστών",1);
    (s1+4)->setATR("ICE-1005","Διακριτά Μαθηματικά",1);
    (s1+5)->setATR("ICE-1006","Φυσική",1);
    
    Course *s2;                         //Δυναμικός Πίνακας 2ου εξαμήνου
    s2=new Course [6];
    s2->setATR("ICE-2001","Σχεδίαση και Ανάλυση Αλγορίθμων",2);
    (s2+1)->setATR("ICE-2002","Πιθανότητες και Στατιστική",2);
    (s2+2)->setATR("ICE-2003","Μαθηματική Ανάλυση II",2);
    (s2+3)->setATR("ICE-2004","Αντικειμενοστρεφής Προγραμματισμός",2);
    (s2+4)->setATR("ICE-2005","Ψηφιακή Σχεδίαση",2);
    (s2+5)->setATR("ICE-2006","Θεωρία Κυκλωμάτων",2);
	
	Student st1("21390021","Ευφροσύνη Βάρσου",1);     //Δημιουργία 1ου Φοιτητή
	cout<<"1oς Φοιτητής : "<< st1<<endl;
	Student st2("21390021","Αλέξανδρος Δημητρίου",1);   //Δημιουργία 2ου Φοιτητή
	cout<<"2oς Φοιτητής : "<< st2<<endl<<endl;
	
	
	cout<<"Αύξηση του εξάμηνου του Φοιτητή 1 :";
	st1++;
	cout<< st1<<endl;
	cout<<"Αύξηση του εξάμηνου του Φοιτητή 2:";
	st2++;
	cout<< st2<<endl<<endl;
	                                                //Δήλωση Μαθημάτων για τον Φοιτητή 1
	cout << "Για τον Φοιτητή :"<<endl; 
    cout << st1<<endl<<endl; 
    cout<< "Διαθέσιμα μαθήματα τρέχοντος εξαμήνου"<<endl<<endl;
    
    
    if (st1.getTerm() == 2)
	   	  for (int i=1;i<=6;i++)
	   	    cout<<i<<".  "<<s2[i-1]<<endl;
	   	    cout<<endl;
	int i=0;
       do
	   {
		cout<< "Πόσα μαθήματα θες να δηλώσεις;(1-6) :";
		cin >> i;
	   } while (i<=0 || i>6);
		int *ch;
		ch = new int [i];
		
		for(int n=0;n<i;n++)
		    {
		    	
		    	do
		    	{
				cout<< n+1 << "η επιλογή :";
		    	cin >>ch[n];
		        } while (ch[n]<=0 || ch[n]>6);
		    		if (ch[n]>0 && ch[n]<=6)
		    	st1 += s2+ch[n]-1;
		    }
		    delete[] ch;
		
	   cout<<endl;
	   cout<< "Δηλωμένα μαθήματα τρέχοντος εξαμήνου"<<endl<<endl;
	   st1.PrintSubject(cout);
	   
	   st1.setGrades(10,s1);               //Αρχικοποιήση βαθμών προηγούμενου εξαμήνου
	   st1.setGrades(4,s1+1);
	   st1.setGrades(3,s1+2);
	   st1.setGrades(2,s1+3);
	   st1.setGrades(6,s1+4);
	   st1.setGrades(7,s1+5);
	   
	   cout<<endl << "Βαθμοί επιτυχίας προηγούμενων εξαμήνων"<<endl<<endl;
	   cout << st1;
       	                 
	   
	   Student st3("2139031","Ελένη Κυρτάτου",2);
	   cout <<endl<<endl<<"Φοιτητής 3 :" <<st3;
	   Student st4("2139037","Μαρία Φουσέκη",1);
	   cout <<endl<<"Φοιτητής 4 :"<< st4<<endl;
	   if (st3>st4)
	      cout<< "Ο Φοιτητής 3 έχει μεγαλύτερο εξάμηνο απο τον Φοιτητή 4"<< endl;
	   if (st3<st4)
	      cout<< "Ο Φοιτητής 3 έχει μικρότερο εξάμηνο απο τον Φοιτητή 4"<< endl;
	   if (st3==st4)
	    cout<< "Ο Φοιτητής 3 έχει το ίδιο εξάμηνο με τον Φοιτητή 4"<< endl;
	   if (st3!=st4)
	    cout<< "Ο Φοιτητής 3 έχει διαφορετικό εξάμηνο με τον Φοιτητή 4"<< endl<<endl;
	   
        st2 += s2;                         //Δήλωση Μαθημάτων Φοιτητή 2
        st2 += s2 + 2;
        st2 += s2 + 3;
	              
	   st2.setGrades(7,s1);                //Καταχώρηση Βαθμών Φοιτητή 2
	   st2.setGrades(9,s1+1);
	   st2.setGrades(3,s1+2);
	   st2.setGrades(4,s1+3);
	   st2.setGrades(7,s1+4);
	   st2.setGrades(5,s1+5);
	   
	    st3 += s2 + 1;                      //Δήλωση Μαθημάτων Φοιτητή 3
        st3 += s2 + 4;
        st3 += s2 + 5;
        st3 += s2 + 2;
	              
	   st3.setGrades(4,s1);                 //Καταχώρηση Βαθμών Φοιτητή 3
	   st3.setGrades(7,s1+1);
	   st3.setGrades(9,s1+2);
	   st3.setGrades(10,s1+3);
	   st3.setGrades(6,s1+4);
	   st3.setGrades(4,s1+5);
	   
	    st4 += s2 ;                   //Δήλωση Μαθημάτων Φοιτητή 4
        st4 += s2 + 1;
        st4 += s2 + 2;
        st4 += s2 + 3;
        st4 += s2 + 4;
	              
	   st4.setGrades(10,s1);                //Καταχώρηση Βαθμών Φοιτητή 4
	   st4.setGrades(9,s1+1);
	   st4.setGrades(7,s1+2);
	   st4.setGrades(5,s1+3);
	   st4.setGrades(3,s1+4);
	   st4.setGrades(2,s1+5);
	   
	   cout<<"Αντιγραφή Φοιτητή 3 σε Φοιτητή 4"<<endl;
	   st3=st4;
	   cout<< st3;
	   
	   //Αποθήκευση Μαθημάτων και Φοιτητών σε Αρχεία 
	   
	   ofstream F1 ("Course.txt",ios::out);
	  
	   F1 << "1ο εξάμηνο"<<endl;
	   for(int j=1;j<=6;j++)
	     F1 <<j<<".  "<<s1[j-1]<<endl;
	   	    F1<<endl;
	   F1 << "2ο εξάμηνο"<<endl;
	   for(int j=1;j<=6;j++)
	     F1 <<j<<".  "<<s2[j-1]<<endl;
	   	    F1<<endl;
	   	    
	   ofstream F2 ("Student.txt",ios::out);
	   
	    F2 << "Βαθμοί επιτυχίας προηγούμενων εξαμήνων για Φοιτητή 1:"<<endl;
	    F2 << st1<<endl<<endl;
	    F2<< "Δηλωμένα μαθήματα τρέχοντος εξαμήνου για Φοιτητή 1 :"<<endl<<endl;
	      st1.PrintSubject(F2);
	      
	    F2 <<endl << "Βαθμοί επιτυχίας προηγούμενων εξαμήνων για Φοιτητή 2:"<<endl;
	    F2 << st2<<endl<<endl;
	    F2<< "Δηλωμένα μαθήματα τρέχοντος εξαμήνου για Φοιτητή 2 :"<<endl<<endl;
	      st2.PrintSubject(F2);
	    
	    F2 <<endl<< "Βαθμοί επιτυχίας προηγούμενων εξαμήνων για Φοιτητή 3:"<<endl;
	    F2 << st3<<endl<<endl;
	    F2<< "Δηλωμένα μαθήματα τρέχοντος εξαμήνου για Φοιτητή 3 :"<<endl<<endl;
	      st3.PrintSubject(F2);
	      
	      F2 <<endl<< "Βαθμοί επιτυχίας προηγούμενων εξαμήνων για Φοιτητή 4:"<<endl;
	    F2 << st4<<endl<<endl;
	    F2<< "Δηλωμένα μαθήματα τρέχοντος εξαμήνου για Φοιτητή 4 :"<<endl<<endl;
	      st4.PrintSubject(F2);
	
	   F1.close();
	   F2.close();
	   delete[] s1;
	   delete[] s2;
	return 0;
}