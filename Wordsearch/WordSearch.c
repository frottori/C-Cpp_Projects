#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

char** INIT_GRID(int ROWS,int COLS,FILE *fp);
char** INIT_WORDS(int ROWS,FILE *fp);
int** INIT_UNUSED(int ROWS,int COLS);
void PRINT_GRID(char** arr,int ROWS,int COLS);
void BUBBLE_SORT(char** Words, int K);
int* Find_Word(char** Grid,char* Word,int M,int N,int K,int** Unused);
void Check_SecondChar(char** Grid,char* Word,int* letters,int M,int N,int a,int b,int** Unused);
int Direction(int l,int a);
void Print_Word(char* Word,int *letters);
void Print_UnusedLetters(int** Unused,char** Grid,int ROWS,int COLS);

int main(int argc,char **argv)
{
    if (argc!=2)
    {
        printf("Error... Too many or none parameters given");
        return 1;
    }

    int M,N,K,i;   
    char **Grid;   //Πίνακας των γραμμάτων
    char **Words;  //Πίνακας των λέξεων
    int *letters;  //Πίνακας για τις δυο συντεταγμένες x,y του πρώτου και τελευταίου γράμματος
    int **Unused;  //Πίνακας για τα αχρησιμοποιήτα γράμματα
    FILE *fp; 
    fp = fopen(argv[1],"r"); assert(fp!=NULL); 
    fscanf(fp,"%d,%d",&M,&N);      

    Grid=INIT_GRID(M,N,fp);
    fscanf(fp,"%d\n",&K); 
    Words=INIT_WORDS(K,fp);
    fclose(fp);
    Unused=INIT_UNUSED(M,N);
    BUBBLE_SORT(Words,K); 
    PRINT_GRID(Grid,M,N);
    
    for(i=0;i<K;i++)
    {
        letters=Find_Word(Grid,Words[i],M,N,K,Unused); 
        if(letters[0]!=-1)
            Print_Word(Words[i],letters);
        else
            printf("Word : %s\nError... Word not found\n\n",Words[i]);
    }
    free(letters);
    for(i=0;i<K;i++)
        free(Words[i]);
    free(Words);
    Print_UnusedLetters(Unused,Grid,M,N);
    
    for (i=0;i<M;i++)  
    {
        free(Grid[i]);
        free(Unused[i]);
    }
    free(Grid);
    free(Unused);
    return 0;
}

char** INIT_GRID(int ROWS,int COLS,FILE *fp)
{
    int i,j;
    char **arr;
    arr = (char**) malloc (ROWS*sizeof(char*)); assert(arr!=NULL);
    for (i=0;i<ROWS;i++)
    {
        arr[i]=(char*) malloc (COLS*sizeof(char)); assert(arr[i]!=NULL);
    }

    for (i=0;i<ROWS;i++)                           
        for(j=0;j<COLS;j++)
            fscanf(fp," %c ",&arr[i][j]); 
    return arr;
}

char** INIT_WORDS(int ROWS,FILE *fp)
{
    int i;
    char** arr;
    char* buf;
    buf = (char*) malloc (201*sizeof(char)); assert(buf!=NULL);
    arr = (char**) malloc (ROWS * sizeof(char*)); assert(arr!=NULL);
    for (i=0;i<ROWS;i++)
    {
        fscanf(fp,"%s ",buf);
        arr[i]= (char*) malloc ((strlen(buf)+1)*sizeof(char)); assert(arr[i]!=NULL);
        strcpy(arr[i],buf);
    }
    free(buf);
    return arr;
} 

int** INIT_UNUSED(int ROWS,int COLS)
{
    int i,j;
    int **arr;
    arr = (int**) malloc (ROWS * sizeof(int*)); assert(arr!=NULL);
    for (i=0;i<ROWS;i++)
    {
    arr[i]=(int*) malloc (COLS * sizeof(int)); assert(arr[i]!=NULL);
    }

    for (i=0;i<ROWS;i++)                           
        for(j=0;j<COLS;j++)
            arr[i][j]=0;
    return arr;
}

void BUBBLE_SORT(char **Words,int K) 
{
  int swap,i;
  char *tmp;
  do 
  {
    swap=0;
    for (i=0;i<K-1;i++) 
      if (strcmp(Words[i],Words[i+1]) > 0) 
      {
        tmp=Words[i];
        Words[i]=Words[i+1];
        Words[i+1]=tmp;
        swap=1;
      }
  } 
  while(swap);
}

void PRINT_GRID(char** arr,int ROWS,int COLS)
{
    int i,j;
    for(i=0;i<2*COLS-1;i++)
        printf("-");
    printf("WORDSEARCH");
    for(i=0;i<2*COLS-1;i++)
        printf("-");
    printf("\n\n");

    for (i=0;i<ROWS;i++)    
    {          
        printf("\t");              
        for(j=0;j<COLS;j++) 
            printf("[%c]",arr[i][j]);
        printf("\n");
    }
    printf("\n");

    for(i=0;i<4*COLS+8;i++)
        printf("-");
    printf("\n\n");
}

int* Find_Word(char** Grid,char* Word,int M,int N,int K,int** Unused)
{
    int* letters; 
    int flag,i,j,a,b;
    letters=(int*) malloc (4*sizeof(int)); assert(letters!=NULL); 
    for(i=0;i<4;i++)
        letters[i]=-1;

    for(i=0;i<M;i++)
        for(j=0;j<N;j++)
        {
            flag=0;
            if (Grid[i][j]==Word[0])   //Εύρεση πρώτου χαρακτήρα 
            {
                a=i;             
                b=j;        
                flag=1;                 
            }
            if(flag)  
            {
              Check_SecondChar(Grid,Word,letters,M,N,a,b,Unused);
              if (letters[2]!=-1)  
              {
                letters[0]=a; 
                letters[1]=b;
                Unused[a][b]=1; 
                return letters;   
              }  
            }   
        }     
   return letters;
}

void Check_SecondChar(char** Grid,char* Word,int* letters,int M,int N,int a,int b,int** Unused)  
{
    int l,k,x,y,dx,dy,ch,cnt=1;
    for(l=a-1;l<=a+1;l++)      //Έλεγχος περιμετρικά του πρώτου γράμματος 
		for(k=b-1;k<=b+1;k++)
	  	    if(l>=0 && k>=0 && l<M && k<N)  //Έλεγχος αν είναι εντός ορίων πίνακα
                if(Grid[l][k]==Word[1])  //Εύρεση δεύτερου χαρακτήρα 
                { 
                    dy=Direction(l,a); //ορισμός κατεύθυνσης y σύμφωνα με δεύτερο γράμμα
                    dx=Direction(k,b); //ορισμός κατεύθυνσης x σύμφωνα με δεύτερο γράμμα
                    y=l+dy;
                    x=k+dx;
                    for(ch=2;ch<strlen(Word);ch++) //Εύρεση των υπόλοιπων χαρακτήρων
                    {  
                       if(x<0 || x>=N || y<0 || y>=M) //Αν επόμενο γράμμα βγει εκτός ορίων 
                           continue;  
                        if(Grid[y][x]==Word[ch])  
                        {
                            x+=dx;
                            y+=dy; 
                            cnt++;
                        } 
                    }   
                    if(cnt==strlen(Word)-1) //Όλοι οι χαρακτήρες είναι ίδιοι άρα βρήκα λέξη
                    {
                        letters[2]=y-dy;          
                        letters[3]=x-dx;       
                        Unused[l][k]=1;
                        for(ch=2;ch<strlen(Word);ch++)
                        {
                            x-=dx;
                            y-=dy;
                            Unused[y][x]=1; 
                        }
                        return; 
                    }
                } 
}

int Direction(int l,int a)
{
    int d;
    if(l==a-1)
       d=-1;
    if(l==a)
        d=0;
    if(l==a+1)
       d=1;
    return d;
}

void Print_Word(char* Word,int *letters)
{
    printf("Word : %s\n",Word);
    printf("First letter: ");
    printf("Row : %d Column : %d\n",++letters[0],++letters[1]);
    printf("Last letter : ");
    printf("Row : %d Column : %d\n\n",++letters[2],++letters[3]);
}

void Print_UnusedLetters(int** Unused,char** Grid,int ROWS,int COLS)
{
    int i,j;
    printf("The unused letters are : ");
        for(i=0;i<ROWS;i++)
            for(j=0;j<COLS;j++)
                if(!Unused[i][j])
                    printf("%c",Grid[i][j]);
    printf("\n");
}