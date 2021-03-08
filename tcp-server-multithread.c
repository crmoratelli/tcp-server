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
#include <pthread.h>

struct client_data{
    int sk;
    struct sockaddr_in *client_addr;
};

void * client_handle(void* cd){
    struct client_data *client = (struct client_data *)cd;
    char sendBuff[1024];
    time_t ticks; 

    memset(sendBuff, 0, sizeof(sendBuff)); 

    /* Imprime IP e porta do cliente. */
    printf("Received connection from %s:%d\n", inet_ntoa(client->client_addr->sin_addr), ntohs(client->client_addr->sin_port));
    fflush(stdout);

    sleep(1);    
 
    /* Pega data e hora do sistema. */
    ticks = time(NULL);
    snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        
    /* Envia resposta ao cliente. */
    send(client->sk, sendBuff, strlen(sendBuff)+1, 0);

    /* Fecha conexão com o cliente. */
    close(client->sk);

    free(client->client_addr);
    free(client);

    return NULL;
}

int main(int argc, char *argv[])
{
    int listenfd = 0;
    struct sockaddr_in serv_addr; 
    int addrlen;
    struct client_data *cd;
    pthread_t thr;

    /* Cria o Socket: SOCK_STREAM = TCP */
    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	perror("socket");
    	return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

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
        cd = (struct client_data *)malloc(sizeof(struct client_data));
        cd->client_addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        addrlen = sizeof(struct sockaddr_in);

		/* Aguarda a conexão */	
        cd->sk = accept(listenfd, (struct sockaddr*)cd->client_addr, (socklen_t*)&addrlen); 

        pthread_create(&thr, NULL, client_handle, (void *)cd);
        pthread_detach(thr);

     }
}

