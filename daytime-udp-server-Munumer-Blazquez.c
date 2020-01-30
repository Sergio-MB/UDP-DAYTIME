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
#include <signal.h>
#define SIZEBUFF 50


int main(int argc, char *argv[]) {
	
	struct sockaddr_in server;  	//Estructura del servidor
	struct sockaddr_in client;		//Estructura del cliente
	char hostNameBuffer[SIZEBUFF];	//Buffer cadena enviada
	char buff[SIZEBUFF];			//Buff para leer del fichero
	char buffer[SIZEBUFF];			//Buffer mensaje recibido
	FILE *fich;						//Fichero para extraer la fecha

//Comprobamos si el numero de argumentos es inferior a 2 (sin recibir puerto)
	if (argc < 2 || (argc == 2 && (strcmp("-p",argv[1]) == 0 ))){
//Creamos la entidad servidor para localizar su puerto 	
		struct servent *server;
//Asignamos la entidad servidor al servicio daytime
		server = getservbyname("daytime","udp");
//Comprobamos error en la obtencion del puerto
		if(server == NULL){
			printf("Error al obtener el puerto por defecto\n");
			perror("getservbyname\n");
			exit(EXIT_FAILURE);
		}
//Asignamos el puerto obtenido a la estructura del cliente
		client.sin_port = server->s_port;
	}
//Comprobamos si nos ofrecen un puerto especifico
	else if( argc == 3 && (strcmp("-p",argv[1]) == 0 )){	
		int puerto;
		sscanf(argv[2],"%d", &puerto);
//Asignamos el puerto obtenido a la estructura del cliente
		client.sin_port = htons(puerto);
	}else{
//Sino estamos en los casos comprobados anteriormente lanzamos error
		printf("\nError de sintaxis\n");
		printf("--> ./programa [-p Puerto]\n");
		exit(EXIT_FAILURE);
	}
//Creamos descriptor socket y definimos su tipo, comprobando el exito de la operacion
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket()");
		exit(EXIT_FAILURE);
	}
//Completamos la estructura del cliente, dando familia y una direccion bien conocida cualquiera
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htons(INADDR_ANY);
//Enlazamos socket con la estructura del cliente, comprobando error en la operacion
	if(bind(sockfd, (struct sockaddr*) &client, sizeof(client)) < 0){
		perror("bind()");
		exit(EXIT_FAILURE);
	}

//Creamos la variable "len" con el valor del tamaño de la estructura del servidor
	socklen_t len = sizeof(server);
//Comenzamos el bucle "while" que soportará el servicio
//Estaremos "a la escucha" esperando el mensaje del cliente
	while(1){
//Recibimos del cliente el mensaje, comprobando si falla
		if(recvfrom( sockfd,buffer, SIZEBUFF, 0 ,
			(struct sockaddr*) &server, &len)< 0 ){
		
			perror("recvfrom()");
			exit(EXIT_FAILURE);
		}
//Almacenamos el valor "Nombre del host" en el buffer correspondiente
//Comprobando exito en la operacion
	if(gethostname(hostNameBuffer,SIZEBUFF) < 0){
		perror("gethostname");
		exit(EXIT_FAILURE);
	}
			
//Capturamos la hora utilizando una llamada a system, con el comando date
//Lo almacenamos en un fichero, para ahora extraer la cadena
		system("date > /tmp/tt.txt");
//Abrimos fichero modo lectura
		fich = fopen("/tmp/tt.txt","r");
//Capturamos la cadena del fichero en el buffer correspondiente
//Comprobando error en la operacion
		if (fgets(buff,SIZEBUFF,fich)==NULL) {
			printf("Error en system(), en fopen(), o en fgets()\n");
			exit(EXIT_FAILURE);
		}	
//Concatenamos HostName y la fecha con el formato apropiado		
		strcat(hostNameBuffer,": ");
		strcat(hostNameBuffer,buff);
//Enviamos al cliente comprobando error en el envio
		if(sendto(sockfd, hostNameBuffer,SIZEBUFF, 0,
			(struct sockaddr*) &server, sizeof(server)) < 0){
			
			perror("sendto()");
			exit(EXIT_FAILURE);
		}
		
	}
//FIN
	exit(EXIT_SUCCESS);
}
