//TCP_UDP programming - client side
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


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

    struct sockaddr_in address; //internet socket address

    int Socket; //takes a non-negative integer from socket function

    char *message = malloc(sizeof(char*));
    char buffer[BUFSIZ];   //8192

    if((Socket = socket(AF_UNIX,SOCK_STREAM,0)) == -1){ // creating a socket

        perror("SOCKET");
        return EXIT_FAILURE;
    }

//addressing
    address.sin_family = AF_UNIX;   // local pc
    address.sin_port = htons(atoi(argumentVector)); // port imp.
//end of addressing

    if(connect(Socket, (struct  sockaddr*)&address, sizeof(address)) == -1){    // try to connect at given port

        perror("CONNECT");
        return EXIT_FAILURE;
    }

    fprintf(stdout,"\nConnected to the server at port %s\n",argumentVector);

    while (strcmp(message,"//exit") || strcmp(buffer,"//exit")){

        printf("\nclient: ");
        fgets(message, sizeof(message),stdin);
        send(Socket,message, strlen(message), 0);
        read(Socket,buffer,BUFSIZ);
        printf("\nserver: %s\n",buffer);
    }

    return EXIT_SUCCESS;

}//end of TransmissionControl

int UserDatagram(char const *argumentVector){

    struct sockaddr_in address; //internet socket address

    int addressLength = sizeof(address); //length of address
    int Socket; //takes a non-negative integer from socket function
    int n;      // end-of-string cursor

    char message[BUFSIZ];   //8192
    char buffer[BUFSIZ];   //8192

    //Clean buffers
    memset(message, '\0', sizeof(message));
    memset(buffer, '\0', sizeof(buffer));



    if((Socket = socket(AF_UNIX,SOCK_DGRAM,0)) == -1){ // creating a socket

        perror("SOCKET");
        return EXIT_FAILURE;
    }

    //addressing
    address.sin_family = AF_UNIX;   // local pc
    address.sin_port = htons(atoi(argumentVector)); // port imp.
    //end of addressing



    fprintf(stdout,"\nConnected to the server at port %s\t(I hope so)\n",argumentVector);

    while (strcmp(message,"//exit") || strcmp(buffer,"//exit")){

        printf("\nclient: ");
        fgets(message, sizeof(message),stdin);

        if(sendto(Socket, (const char *)message, strlen(message),MSG_CONFIRM, (const struct sockaddr *) &address,addressLength) < 0){
            fprintf(stderr,"\nCOULD NOT SEND\n");
            return EXIT_FAILURE;
        }

        if((n = recvfrom(Socket, (char *)buffer, BUFSIZ,MSG_WAITALL, ( struct sockaddr *) &address,(socklen_t*)&addressLength)) < 0){

            fprintf(stderr,"\nCOULD NOT RECEIVE\n");
            return EXIT_FAILURE;

        }

        buffer[n] = '\0';
        printf("\nserver: %s\n",buffer);
    }

    close(Socket);

    return EXIT_SUCCESS;
}//end of UserDatagram



//CLIENT-SIDE
