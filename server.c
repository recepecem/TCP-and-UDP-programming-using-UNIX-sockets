//TCP_UDP programming - Server Side
#include <unistd.h> // for read()
#include <stdio.h>  // for perror(), fprintf(), BUFSIZ, stdout, stderr
#include <sys/socket.h> // for bind(), listen(), accept(), send(), socket(), setsockopt()
#include <stdlib.h> // for malloc(), system(), EXIT_FAILURE
#include <netinet/in.h> //for htons(), STRUCT sockaddr_in
#include <string.h> //for strcmp(), strlen()


int TransmissionControl(char const *argumentVector); //tcp
int UserDatagram(char const *argumentVector);       //udp

int main(int argc, char const *argv[]) {

    if(argc != 3){  // checks if you gave all the parameters.

        fprintf(stdout,"\nusage:\n\t\t%s [port] [protocol]\n",argv[0]);
        fprintf(stdout,"example usage:\n\t\t%s 5000 tcp\n\n",argv[0]);
        return EXIT_FAILURE;
    }

    system("clear");

    if(!(strcmp(argv[2],"tcp"))){
        if(TransmissionControl(argv[1])){

            fprintf(stderr,"\nAn Error Occurred while using TCP.\n");
            return EXIT_FAILURE;
        }
    }

    if(!(strcmp(argv[2],"udp"))){
        if(UserDatagram(argv[1])){

            fprintf(stderr,"\nAn Error Occurred while using UDP.\n");
            return EXIT_FAILURE;
        }
    }

    fprintf(stdout,"\nUnknown Protocol\n");

    return EXIT_SUCCESS;
}//end of main


int TransmissionControl(char const *argumentVector){

    // variable implementations
    struct sockaddr_in address;  // internet socket address

    char buffer[BUFSIZ]; // 8192
    char *message = malloc(sizeof(char*)*8192);

    int sfd; //socket file descriptor. Takes non-negative integer from socket function
    int Socket;  // takes a non-negative integer from accept function
    int addressLength = sizeof(address); //length of address

    //end of variable implementations

    if((sfd = socket(AF_UNIX,SOCK_STREAM,0)) == -1){ // creating a socket

        perror("SOCKET");
        return EXIT_FAILURE;
    }

//addressing
    address.sin_family = AF_UNIX; // local pc
    address.sin_addr.s_addr = INADDR_ANY; // to accept any incoming messages
    address.sin_port = htons(atoi(argumentVector)); // port  imp.
//end of addressing


    if(bind(sfd,(struct  sockaddr *)&address, sizeof(address)) == -1){   // attaching socket to the port

        perror("BIND");
        return EXIT_FAILURE;
    }


    if(listen(sfd,100) == -1){   // listen up to 100 clients
        perror("LISTEN");
        return EXIT_FAILURE;
    }

    fprintf(stdout,"\nOnline at port %s\n",argumentVector);


    if((Socket = accept(sfd, (struct sockaddr*)&address, (socklen_t*)&addressLength)) == -1 ){
        perror("ACCEPT");
        return EXIT_FAILURE;
    }


    while (strcmp(message,"//exit") || strcmp(buffer,"//exit")){  //actual communication in order.

        read(Socket, buffer, BUFSIZ);
        printf("\n\nclient: %s\nserver: ", buffer);
        fgets(message, sizeof(message),stdin);
        send(Socket, message, strlen(message), 0);
    }

    return EXIT_SUCCESS;
}//end of TransmissionControl


int UserDatagram(char const *argumentVector) {

    // variable implementations

    struct sockaddr_in address;  // internet socket address

    char message[BUFSIZ];   //8192
    char buffer[BUFSIZ];   //8192

    //Clean buffers
    memset(message, '\0', sizeof(message));
    memset(buffer, '\0', sizeof(buffer));

    int sfd; //socket file descriptor. Takes non-negative integer from socket function
    int addressLength = sizeof(address); //length of address
    int n;  // end-of-string cursor
    //end of variable implementations


    if ((sfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) { // creating a socket

        perror("SOCKET");
        return EXIT_FAILURE;
    }


    //addressing
    address.sin_family = AF_UNIX; // local pc
    address.sin_addr.s_addr = INADDR_ANY; // to accept any incoming messages
    address.sin_port = htons(atoi(argumentVector)); // port  imp.
    //end of addressing


    if (bind(sfd, (struct sockaddr *) &address, sizeof(address)) == -1) {   // attaching socket to the port

        perror("BIND");
        return EXIT_FAILURE;
    }

    fprintf(stdout,"\nOnline at port %s\n",argumentVector);

    while (strcmp(message, "//exit") || strcmp(buffer, "//exit")) {

        if((n = recvfrom(sfd, (char *) buffer, BUFSIZ, MSG_WAITALL, (struct sockaddr *) &address,(socklen_t *) &addressLength)) < 0){
            fprintf(stderr,"\nCOULD NOT RECEIVE\n");
            return EXIT_FAILURE;
        }

        buffer[n] = '\0';
        printf("\n\nclient: %s\nserver: ", buffer);
        
        fgets(message, sizeof(message),stdin);

        if(sendto(sfd, (const char *) message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &address,addressLength) < 0){
            fprintf(stderr,"\nCOULD NOT SEND\n");
            return EXIT_FAILURE;
        }
    }

    close(sfd);
    
    return EXIT_SUCCESS;
}//end of UserDatagram

//SERVER_SIDE
