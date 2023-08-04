#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    int serverSocket, clientSocket;
    sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    char buffer[BUFFER_SIZE];

    // Criando o socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Erro ao criar o socket." << std::endl;
        return 1;
    }

    // Configurando o endereço do servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Associando o socket a um endereço
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erro ao associar o socket a um endereço." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Iniciando a escuta por conexões
    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Erro ao iniciar a escuta por conexões." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Aguardando conexões..." << std::endl;

    // Aceitando uma conexão de cliente
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        std::cerr << "Erro ao aceitar conexão." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Cliente conectado." << std::endl;

    while (true) {
        // Recebendo dados do cliente
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0) {
            std::cerr << "Conexão encerrada pelo cliente." << std::endl;
            break;
        }

        std::cout << "Mensagem recebida: " << buffer << std::endl;

        // Enviando uma resposta para o cliente
        const char* response = "Mensagem recebida com sucesso!";
        send(clientSocket, response, strlen(response), 0);
    }

    // Fechando os sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
