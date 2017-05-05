/*!
 *  @mainpage Servidor
 *  @author Adelino Afonso Fernandes Avelino
 *  @author Irene Ginani Costa Pinheiro
 *  @date May, 2017
 *  @version 1.0
 *
 *  @file drive_forward_list.cpp
 *
 *  @brief Arquivo servidor
 *
 */

#include <cstdio>       //printf
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <vector>
#include <iostream>
#include <thread>
#include <stdlib.h> 
#include <classes.h>

#define MAXMSG 1024
#define PORTNUM 4320
 
using namespace std;


void TabuleiroInicial(vector<vector<Carta>> &cartas){
    Carta carta1(2,false);
    Carta carta2(1,false);
    Carta carta3(2,false);
    Carta carta4(1,false);
    Carta carta5(3,false);
    Carta carta6(4,false);
    Carta carta7(3,false);
    Carta carta8(4,false);

    vector<Carta> cartass;

    cartass.push_back(carta1);
    cartass.push_back(carta2);

    cartas.push_back(cartass);

    cartass.clear();

    cartass.push_back(carta3);
    cartass.push_back(carta4);

    cartas.push_back(cartass);

    cartass.clear();

    cartass.push_back(carta5);
    cartass.push_back(carta6);

    cartas.push_back(cartass);

    cartass.clear();

    cartass.push_back(carta7);
    cartass.push_back(carta8);

    cartas.push_back(cartass);

    cartass.clear();
}

void receberDados(int conexaoClienteId, char *msg,int &a){
    a = recv(conexaoClienteId,msg,MAXMSG,0);
}



int main(int argc, char *argv[])
{
    //variáveis do servidor
    struct sockaddr_in endereco;
    int socketId;

    vector<vector<Carta>> cartas;
    TabuleiroInicial(cartas);
    Tabuleiro tab(cartas);
    
    
    
    //variáveis relacionadas com as conexões clientes
    struct sockaddr_in enderecoCliente;
    socklen_t tamanhoEnderecoCliente = sizeof(struct sockaddr);
    int conexaoClienteId;
 
    //Permitir inserir o caractere de fim de msg \0
    char *msg = new char[MAXMSG+1];
    int byteslidos, mov;
 
    /*
     * Configurações do endereço
    */
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    //endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_addr.s_addr = inet_addr("127.0.0.1");
 
    /*
     * Criando o Socket
     *
     * PARAM1: AF_INET ou AF_INET6 (IPV4 ou IPV6)
     * PARAM2: SOCK_STREAM ou SOCK_DGRAM
     * PARAM3: protocolo (IP, UDP, TCP, etc). Valor 0 escolhe automaticamente
    */
    socketId = socket(AF_INET, SOCK_STREAM, 0);
 
    //Verificar erros
    if (socketId == -1)
    {
        printf("Falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }
 
    //Conectando o socket a uma porta. Executado apenas no lado servidor
    if ( bind (socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr)) == -1 )
    {
        printf("Falha ao executar bind()\n");
        exit(EXIT_FAILURE);
    }
 
    //Habilitando o servidor a receber conexoes do cliente
    if ( listen( socketId, 10 ) == -1)
    {
        printf("Falha ao executar listen()\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Servidor: esperando conexão do Jogador\n");
    //servidor ficar em um loop infinito
    while(1)
    {
 
        //Servidor fica bloqueado esperando uma conexão do cliente
        conexaoClienteId = accept( socketId,(struct sockaddr *) &enderecoCliente,&tamanhoEnderecoCliente );
 
        //printf("Servidor: recebeu conexão de %s\n", inet_ntoa(enderecoCliente.sin_addr));
 
        //Verificando erros
        if ( conexaoClienteId == -1)
        {
            printf("Falha ao executar accept()");
            exit(EXIT_FAILURE);
        }
 
        //receber uma msg do cliente
        //printf("Servidor vai ficar esperando uma mensagem\n");
        thread recebe(receberDados,conexaoClienteId,msg,ref(byteslidos));
        recebe.join();
        mov = atoi(msg);
        tab.movimenta(mov);
        
        if (byteslidos == -1)
        {
            printf("Falha ao executar recv()");
            exit(EXIT_FAILURE);
        }
        else if (byteslidos == 0)
        {
            printf("Cliente finalizou a conexão\n");
            exit(EXIT_SUCCESS);
        }
 
        //Inserir o caracter de fim de mensagem
 
        //printf("Servidor recebeu a seguinte msg do cliente: %d \n", mov);
 
        close(conexaoClienteId);
    }
 
    return 0;
}
