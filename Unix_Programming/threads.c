#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>

void *func(void *arg);
pthread_mutex_t mutex; //mutex για τον αποκλεισμό της κρίσιμης περιοχής
int N, **A, p;
int total_max;   //τελικό μέγιστο πίνακα

typedef struct ThreadArgs   // δομή για τα δύο ορίσματα της συνάρτησης των thread
{
    int s_row;    //από ποια γραμμή θα αρχίσει κάθε thread
    int e_row;    //σε ποια γραμμή θα τελειώσει κάθε thread
} ThreadArgs;

int main(int argc, char **argv)
{
    clock_t start_time, end_time;
    start_time = clock(); //καταγραφή του χρόνου της CPU στην αρχή του προγράμματος

    int i, j;
    srand(time(NULL));
    printf("Give N: ");
    scanf("%d", &N);
    do
    {
        printf("Give p: ");
        scanf("%d", &p);
    } while(N%p != 0);  //έλεγχος για αν p ακέραιο πολλαπλάσιο του Ν
   
    //Δυναμική δέσμευση πίνακα με τα thread
    pthread_t *thread = (pthread_t *)malloc(p * sizeof(pthread_t)); 
    assert(thread != NULL); //η assert διαβεβαιώνει ότι έγινε σωστή δέσμευση και αν όχι τερματίζει το πρόγραμμα

    //Δυναμική δέσμευση δισδιάστατου πίνακα
    A=(int **) malloc (N * sizeof(int *)); assert(A != NULL);
    for (i=0; i<N; i++)
    {
        A[i]=(int *) malloc (N * sizeof(int)); assert(A[i] != NULL);
    }
    // Αρχικοποιήση mutex
    pthread_mutex_init(&mutex, NULL);

    //Αρχικοποίηση πίνακα με random 3ψήφιες ακέραιες τιμές (θετικές και αρνητικές)
    printf("Array:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
        {
            A[i][j]=rand() % (1999) -999;
            printf("%4d ", A[i][j]);
        }
        printf("\n");
    }

    total_max = A[0][0]; //αρχικοποίηση total_max στο πρώτο στοιχείο του πίνακα

    //Δέσμευση πίνακα για τα ορίσματα των p thread 
    ThreadArgs *thr_args = malloc(p * sizeof(ThreadArgs)); assert(thr_args != NULL); 

    int local_rows = N/p;         // o αριθμός των γραμμών για κάθε thread
    int start_row = 0;           //για τον υπολογισμό της αρχικής γραμμής του κάθε thread (αρχικοποιήση σε 0)

    for (i=0; i<p; i++) 
    {
        thr_args[i].s_row = start_row;              //αρχή του thread[i]
        thr_args[i].e_row = start_row + local_rows; //τελός του thread[i]

        //δημιουργία του thread[i] ορίσμα το thr_args[i] που περιέχει το s_row και το e_row
        pthread_create(&thread[i], NULL,func, (void*) &thr_args[i]);

        start_row = thr_args[i].e_row;        //ενημέρωση της αρχής του επόμενου thread απο το τέλος του προηγούμενου thread 
    }

    for (i=0; i<p; i++)
        pthread_join(thread[i], NULL); // το main thread περιμένει να τελειώσουν τα thread που δημιούργηε πριν συνεχίσει την εκτέλεση του

    printf("Total max: %d\n", total_max);  //αφου τελείωσε ο υπολογισμός από ολα τα thread τυπώνεται ο μέγιστος
    pthread_mutex_destroy(&mutex); // καταστροφή του mutex

    //Αποδέσμευση μνήμης της malloc
    free(thread);
    free(thr_args);
    for (i=0;i<N;i++)
        free(A[i]);

    end_time = clock(); //καταγραφή του χρόνου της CPU στο τέλος του προγράμματος
    double exec_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;  //υπολογισμός χρόνου εκτέλεσης
    printf("Execution time: %.5lf secs\n", exec_time);

    return 0;
}

void *func(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;  //typecasting void* του ορίσματος σε τύπου struct ThreadArgs*
    int local_max = A[args->s_row][0];      //αρχικοποιήση local_max στο πρώτο στοιχείο της αρχικής γραμμής του
    int i, j;

    //Υπολογισμός local max για τις γραμμές του thread
    for (i = args->s_row; i < args->e_row; i++)
        for (j = 0; j < N; j++)
            if (A[i][j] > local_max)
                local_max = A[i][j];
    //κρίσιμη περιοχή
    pthread_mutex_lock(&mutex); 
    if (local_max > total_max)
        total_max = local_max;      //ενημέρωση total_max
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}