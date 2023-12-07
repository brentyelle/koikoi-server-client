//server.c
extern "C" {
#include "csapp.h"
}
#include <cstdlib>
#include "serv-playgame.hpp"

typedef struct {
    int cfd;                // connection file descriptor
    char hostn[MAXLINE];    // host name
    char portn[MAXLINE];    // port name
} ClientInfo;

// void *thread(void *vargp);

int main(int argc, char**argv) {
    int listenfd;
    ClientInfo *clientptr;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    
    printf("Initializing server...\n");
    listenfd = Open_listenfd(argv[1]);
    printf("Server ready to receive connection.\n\n");
    
    while (1) {
        //dynamically allocate memory and create connection to client
        clientlen = sizeof(struct sockaddr_storage);
        clientptr = new ClientInfo;
        clientptr->cfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientlen);

        // print info of who we've connected to
        Getnameinfo( (struct sockaddr *) &clientaddr, clientlen, clientptr->hostn, MAXLINE, clientptr->portn, MAXLINE, 0);
        printf("Connected to (%s, %s).\n", clientptr->hostn, clientptr->portn);
        
        // make a thread to deal with this new client
        serviceKoiKoi(clientptr->cfd);
        
        //then wait for a new client
        printf("Connection to (%s, %s) closed.\n", clientptr->hostn, clientptr->portn);
        close(clientptr->cfd);
        delete clientptr;
        
    }
    return 0;
}

/* ================================================================
   SERVICE THREAD
   ================================================================ */
/*
void *thread(void *vargp) {
    int userchoice;  //holds user-input choice
    //get info from the clientptr object passed here
    ClientInfo *thisClient = (ClientInfo*)vargp;
    int connfd = thisClient->cfd;
    
    //run in "detached" mode so that we self-reap once we hit "return NULL"
    pthread_detach(pthread_self());
    
    // BEGIN SERVICE
    do {
        userchoice = PromptUserChoice(connfd);
        //if (userchoice == 1) {      //if the user chooses 1, then we're adding a new record
        //    AddRecordService(connfd);
        //} 
        //else
        if (userchoice == 1) { //if the user chooses 2, then we're reading for a record
            ReadRecordService(connfd);
        }
    } while (userchoice != 2); //if the user chooses 3, then we're closing the connection

    // END SERVICE
    PrintGoodbye(connfd);
    printf("Connection to (%s, %s) closed.\n", thisClient->hostn, thisClient->portn);
    close(connfd);
    
    //free the dynamically-allocated memory to avoid a leak
    free(vargp);
    
    return NULL;
}
*/