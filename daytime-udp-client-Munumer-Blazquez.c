// Practica tema 5, Muñumer Blázquez Sergio
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <string.h>
#define MAX 512

int main(int argc, char *argv[]) {
//Creamos el buffer que contendra la respuesta del servidor
	char buffer[MAX];
//Creación de la estructura del servidor
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
//Comprobamos si el Num de Args es válido, lanzamos error
	if (argc<2) {
		printf("Numero de argumentos inválido\n");
		exit(EXIT_FAILURE);
	}
//Asigno el segundo argumento de programa a la estructura "serveraddr" (como direccion)
	inet_aton(argv[1], &servaddr.sin_addr);
//Creamos la entidad servidor para localizar su puerto en caso de necesitarlo
	struct servent *server;
//Asignamos la entidad servidor al servicio en concreto que necesitamos
	server=getservbyname("daytime","udp");
//Comprobamos el caso de que no nos dan puerto
	if(argc==2 || (argc==3 && strcmp("-p",argv[2])==0)){
//Buscamos el puerto por defecto
	
		servaddr.sin_port = server->s_port;
//Comprobamos fallo
		if (server == NULL){
			printf("Error al obtener puerto\n");
			perror("\nserv");
			exit(EXIT_FAILURE);
		}
	}
//Comprobamos el caso de que nos dan un puerto concreto
	else if (argc==4 && (strcmp("-p",argv[2])==0)){
//Asignamos el puerto dado por el usuario a la estructura del servidor
		int puerto;
		sscanf(argv[3],"%d", &puerto);
		servaddr.sin_port = htons(puerto);
	}else{
//Sino estamos en ninguno de los casos anteriores, lanzamos error
		printf("\nError de sintaxis\n");
		printf("--> ./programa DireccionIP [-p Puerto]\n");
		exit(EXIT_FAILURE);
	}


//Creamos descriptor socket y definimos su tipo
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//Comprobamos la operacion, si falla, salimos
        perror("\nsocket()");
        exit(EXIT_FAILURE);
    }

//Creacion de la estructura relacionada con el cliente
	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_port=0;
	client.sin_addr.s_addr= INADDR_ANY;

   
//Enlazamos socket del cliente con su "sockaddr_in"
	if (bind(sockfd, (struct sockaddr*) &client, sizeof(client)) < 0){
		printf("Fallo al enlazar el socket\n");
		perror("\nbind()");
		exit (EXIT_FAILURE);
	}
//Enviamos al servidor cadena aleatoria, comprobando si falla el envio
	char *cadena = "cadena";
	if(sendto(sockfd, cadena, sizeof(cadena),0, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
		printf("Fallo en el envio\n");
		perror("\nsendto()");
		exit(EXIT_FAILURE);
	}

//Recibimos del servidor comprobando si falla
	socklen_t len = sizeof(servaddr);
	if(recvfrom(sockfd, buffer, MAX, 0,(struct sockaddr*)&client,&len) < 0){
		perror("\nrecvfrom()");
		exit(EXIT_FAILURE);
	}

//Imprimimos la cadena dada por el servidor
	printf("%s\n",buffer);
//Cerrando el socket
	exit (EXIT_SUCCESS);
	close(sockfd);
//FIN 
    return 0;
}
