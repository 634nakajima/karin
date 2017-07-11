#include "SocketManager.h"

SocketManager::SocketManager() {
    active = 0;
    startServer();
}

SocketManager::~SocketManager() {
    stopServer();
}

void SocketManager::startServer() {
    int result;
    
    if (!active) {
        active = 1;
        result = pthread_create(&thread, NULL, threadFunction, (void *)this);
    }
    
}

void SocketManager::stopServer() {
    int result;
    
    if (active) {
        active = 0;
        result = pthread_join( thread, NULL );
    }
}

void SocketManager::broadcast(void *d, int size) {
    int sock;
    struct sockaddr_in addr;
    int yes = 1;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6340);
    addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));
    sendto(sock, d, size, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);
}

void SocketManager::sendData(void *d, int size, char *ip) {
    int sock;
    struct sockaddr_in addr;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6340);
    addr.sin_addr.s_addr = inet_addr(ip);

    sendto(sock, d, size, 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);
}

void *SocketManager::threadFunction(void *data) {
    SocketManager *socketManager = (SocketManager *)data;
    int sock, sock0, val;
    struct sockaddr_in addr, senderinfo;
    
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6340);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock0, 5);

    while (socketManager->active) {
        int n;
        int data[256];
        socklen_t addrlen;
        addrlen = sizeof(senderinfo);
        val = 1;
        ioctl(sock, FIONBIO, &val);
        
        sock = accept(sock0, (struct sockaddr *)&senderinfo, &addrlen);
        n = read(sock, data, sizeof(data));
        
        if(n == -1) {
            usleep(1000);
            continue;
        }
        socketManager->callback(data, n);
    }
    close(sock0);
    pthread_exit(NULL);
}
