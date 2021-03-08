/* Adaptado de https://www.thegeekstuff.com/2011/12/c-socket-programming/?utm_source=feedburner */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    struct sockaddr_in client_addr; 
    unsigned int addrlen = sizeof(client_addr);

    char sendBuff[1025];
    time_t ticks; 

    /* Cria o Socket: SOCK_STREAM = TCP */
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	perror("socket");
    	return 1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(sendBuff, 0, sizeof(sendBuff)); 

	/* Configura servidor para receber conexoes de qualquer endereço:
	 * INADDR_ANY e ouvir na porta 5000 */ 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

	/* Associa o socket a estrutura sockaddr_in */
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
    	perror("bind");
    	return 1;
    }

	/* Inicia a escuta na porta */
    listen(listenfd, 10); 

    while(1) {
		/* Aguarda a conexão */	
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &addrlen); 

		/* Imprime IP e porta do cliente. */
        printf("Received connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		/* Pega data e hora do sistema. */
        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        
        /* Envia resposta ao cliente. */
        send(connfd, sendBuff, strlen(sendBuff)+1, 0);

		/* Fecha conexão com o cliente. */
        close(connfd);

     }
}
