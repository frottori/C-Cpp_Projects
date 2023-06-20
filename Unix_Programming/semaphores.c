#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
void *thread1(void*);
void *thread2(void*);
void *thread3(void*);

sem_t s1,s2,s3;

int main(int argc, char **argv) 
{
  pthread_t thr[3];

  //Αρχικοποιήση σημαφόρων
  sem_init(&s1,0,0);  
  sem_init(&s2,0,0);
  sem_init(&s3,0,1);  

  //Δημιουργία των τριών thread για κάθε συνάρτηση
  pthread_create(&thr[0],NULL,thread1,NULL);
  pthread_create(&thr[1],NULL,thread2,NULL);
  pthread_create(&thr[2],NULL,thread3,NULL);

  for (int i=0;i<3;i++)
    pthread_join(thr[i],NULL);  //το main thread περιμένει για τα thread να τελείωσουν 

  //Ελευθέρωση μνήμης των σημαφόρων
  sem_destroy(&s1); 
  sem_destroy(&s2);
  sem_destroy(&s3);
  return 0;
}

void *thread1(void *arg) 
{
  while (1) 
  {
    sem_wait(&s3);  //σε 1 αρχικοποιημένος άρα θα τυπωθέι πρώτα το one (έπειτα γίνεται s3 = 0)
    printf("one ");
    sem_post(&s1);  //γινεται το s1 = 1
  }
}

void *thread2(void *arg) 
{
  while (1) 
  {
    sem_wait(&s1);  //μετά απο το thread1 αν είναι ενα τότε εκτελειται (έπειτα γίνεται s1=0)
    printf("two ");
    sem_post(&s2);  // γίνεται s2 = 1
  }
}

void *thread3(void *arg) 
{
  while (1) 
  {
    sem_wait(&s2); //μετά απο το thread2 αν είναι ένα τότε εκτελειται (έπειτα γίνεται s2=0)
    printf("three ");
    sem_post(&s3);  //γίνεται s3 = 1 (και πραγματοποιείται ο "κύκλος")
  }
}