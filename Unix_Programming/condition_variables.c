#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //mutex για την προστασία του flag 
//τα condition variables για κάθε κατάσταση
pthread_cond_t c1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t c2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t c3 = PTHREAD_COND_INITIALIZER;
int flag = 1;   //καθορίζει την τωρινή κατάσταση 

void *thread1(void *);
void *thread2(void *);
void *thread3(void *);

int main(int argc, char **argv) 
{
    pthread_t thr[3];

    //Δημιουργία των τριών thread για κάθε συνάρτηση
    pthread_create(&thr[0],NULL,thread1,NULL);
    pthread_create(&thr[1],NULL,thread2,NULL);
    pthread_create(&thr[2],NULL,thread3,NULL);

    for (int i=0;i<3;i++)
        pthread_join(thr[i],NULL); //το main thread περιμένει για τα thread να τελείωσουν 

    //Διαγραφή των condtition variables και της mutex
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&c1);
    pthread_cond_destroy(&c2);
    pthread_cond_destroy(&c3);
    return 0;
}

void *thread1(void *arg) 
{
    while (1) 
    {
        pthread_mutex_lock(&mutex); //κλείδωμα mutex για κρίσιμο τμήμα

        while (flag != 1)
            pthread_cond_wait(&c1, &mutex); //περιμένει στο conditon variable c1 μέχρι να ικανοποιήθει η τιμή της flag
        printf("one ");
        flag = 2;       //αλλαγή κατάστασης για το c2 
        pthread_cond_signal(&c2);   //ειδοποιει το c2 

        pthread_mutex_unlock(&mutex); //ελευθέρωση mutex
    }
}

void *thread2(void *arg) 
{
    while (1) 
    {
        pthread_mutex_lock(&mutex); //κλείδωμα mutex για κρίσιμο τμήμα

        while (flag != 2)
            pthread_cond_wait(&c2, &mutex);  //περιμένει στο conditon variable c2 μέχρι να ικανοποιήθει η τιμή της flag
        printf("two ");
        flag = 3;           //αλλαγή κατάστασης για το c3 
        pthread_cond_signal(&c3); //ειδοποιει το c3

        pthread_mutex_unlock(&mutex); //ελευθέρωση mutex
    }
}

void *thread3(void *arg) 
{
    while (1) 
    {
        pthread_mutex_lock(&mutex);  //κλείδωμα mutex για κρίσιμο τμήμα

        while (flag != 3)
            pthread_cond_wait(&c3, &mutex);  //περιμένει στο conditon variable c3 μέχρι να ικανοποιήθει η τιμή της flag
        printf("three ");
        flag = 1;               //αλλαγή κατάστασης για το c1
        pthread_cond_signal(&c1); //ειδοποιει το c1 (και πραγματοποιείται ο "κύκλος")

        pthread_mutex_unlock(&mutex);   //ελευθέρωση mutex
    }
}