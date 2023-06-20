#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>                  
#define SOCKET_PATH "socket"

int main(int argc,char** argv)
{
    char buf[1024];
    printf("Give a sequence of numbers :");   //η ακολουθία αριθμών θα διαχωρίζεται με οριοθέτη το κενό μέχρι να πατήσει ο χρήστης enter
    scanf("%[^\n]", buf);                     // %[^\n] για να μπορώ να έχω κενά ανάμεσα στην ακολουθία των ακεραίων

    while(strcmp(buf,"exit") != 0)  //exit τιμή τερματισμού
    {
        //Δημιουργία stream socket 
        int sock = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock == -1) 
        {
            perror("socket");
            exit(1);
        }

        //Ορισμός διεύθυνσης του socket του server
        struct sockaddr_un server;
        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, SOCKET_PATH);

        // Σύνδεση του socket στην διεύθυνση του server
        if (connect(sock, (struct sockaddr *) &server, sizeof(server)) == -1) 
        {
            perror("connect");
            exit(1);
        }
        
        if (write(sock, buf, strlen(buf)) < 0)          //γράφει στο socket του server την ακολουθία ακεραίων
            perror("writing on stream socket");

        char ans[256];  
        if(read(sock, ans, sizeof(ans)) < 0)  //διαβασμα απο το socket του server της απάντησης
        {
            perror("read");
            exit(1);
        }
        puts(ans);  //τύπωμα απάντησης του server

        bzero(buf,sizeof(buf));         //αρχικοποιώ τον buf σε 0 για την επόμενη ακολουθία  
        printf("Give a sequence of numbers (or type exit) :"); 
        while (getchar() != '\n');         //καθαρίζω τον output buffer γιατί πατάω enter απο την προηγούμενη scanf και περνιέται στον buf 
        scanf("%[^\n]", buf);                                          
        close(sock); //κλείσιμο του socket  
    }
}