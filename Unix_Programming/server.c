#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define N 5
#define SOCKET_PATH "socket"
void* client_handler(int* arg);
int sum_numbers(char* num,int *cnt);

int main(int argc,char** argv) 
{
    //Δημιουργία stream socket
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);   
    if (sock == -1) 
    {
        perror("socket");
        exit(1);
    }

    //Ορισμός διεύθυνσης του socket 
    struct sockaddr_un server;
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, SOCKET_PATH);

    //Δέσμευση του socket στην διεύθυνησ που ορίσαμε
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1) 
    {
        perror("bind");
        exit(1);
    }

    //Αναμονή για N εύρεση clients
    if (listen(sock, N) == -1) 
    {
        perror("listen");
        exit(1);
    }

    printf("Server is listening...\n");
    int cnt;
    while (1) 
    {
        // Αποδοχή σύνδεσης με client
        int client_sock = accept(sock, NULL, NULL);
        if (client_sock == -1) 
        {
            perror("accept");
            exit(1);
        }

        // Δημιουργία thread που εξυπηρετεί ταυτόχρονα clients
        pthread_t thread;
        if (pthread_create(&thread, NULL, (void *) client_handler, (void*) &client_sock) != 0) 
        {
            perror("pthread_create");
            exit(1);
        }
    }

    close(sock);
    unlink(SOCKET_PATH);
    return 0;
}

void* client_handler(int* arg) 
{
    int client_sock = *arg; 
    char buffer[1024];
    bzero(buffer,sizeof(buffer));   //αρχικοποιήση buffer εισόδου με 0
    ssize_t num_bytes = read(client_sock, buffer, sizeof(buffer)); //Δίαβασμα ακολουθίας ακεραίων Ν απο client
    if (num_bytes == -1) 
    {
        perror("read");
        exit(1);
    }
    int cnt;    
    int sum = sum_numbers(buffer,&cnt);  //υπολογισμός αθροίσματος
    float mo = (float) sum / cnt;  //υπολογισμός μέσου όρου
    printf("Average : %.2f\n",mo);
   
    if(mo > 20)
    {
        char str[256];
        sprintf(str, "Average : %.2f\nSequence OK", mo); //τύπωνει το formatted string στο str
        if(write(client_sock,str,256 * sizeof(char)) < 0)     //γράφει στο socket του client 
            perror("writing on socket");
    }
    else
    {
        if(write(client_sock,"Check Failed",13 * sizeof(char)) < 0) //γράφει στο socket του client 
            perror("writing on socket");
    }
    close(client_sock);         
    pthread_exit(NULL); 
}

int sum_numbers(char* num,int *cnt) 
{
    int sum = 0;
    char* tok = strtok(num," ");    //εκχωρούνται στο tok οι χαρακτήρες πρίν το space άρα διαχωρίζω τα ψηφία του πρώτου αριθμού
    *cnt = 0;                       //για εύρεση του πλήθους αριθμών που έδωσε ο χρήστης

    while (tok != NULL) //αν επιστρέψει NULL τότε άδειασε ο num και δεν υπάρχουν άλλοι αριθμοί
    {
        sum += atoi(tok);   //μετατροπή του κάθε αριθμού σε ακέραιο και πρόσθεση του στο sum
        (*cnt)++;           
        tok = strtok(NULL," ");  //διαχωρίζει τους υπόλοιπους αριθμούς με deleimeter το space (Null γιατί συνεχίζει απο την προγούμενη ακολουθία)
    }
    return sum;
}