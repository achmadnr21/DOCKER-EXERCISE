#ifndef SOCKETUTIL_SOCKETUTIL_H
#define SOCKETUTIL_SOCKETUTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdint.h>
#include "../ice_descriptor/ice_descriptor.h"


int createUDPIpv4Socket(){
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        perror("Socket creation failed");
        return -1;
    }
    return fd;
}

struct sockaddr_in* createIpv4Address(const char* ip, int port){
    struct sockaddr_in* addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if(strlen(ip)==0){
        addr->sin_addr.s_addr = INADDR_ANY;
        return addr;
    }
    inet_pton(AF_INET, ip, &addr->sin_addr.s_addr);
    
    return addr;
}
typedef struct{
    int fd;
    struct sockaddr_in addr;
}ClientConnection;

// create mutex
pthread_mutex_t mtx_STUN_response_thread = PTHREAD_MUTEX_INITIALIZER;

int turn_port = 0;
uint32_t turn_ip = 16777343;

void* STUN_response_thread(void* arg){
    // argumen berupa ClientConnection
    ClientConnection* conn = (ClientConnection*)arg;
    // lock mutex
    pthread_mutex_lock(&mtx_STUN_response_thread);
    ICE_stun_response response = {
        .stun = {
            .ip = conn->addr.sin_addr.s_addr,
            .port = ntohs(conn->addr.sin_port)
        },
        .turn = {
            .ip = turn_ip,
            .port = turn_port
        }
    };
    // unlock mutex
    pthread_mutex_unlock(&mtx_STUN_response_thread);
    // send response to client
    if(sendto(conn->fd, &response, sizeof(ICE_stun_response), 0, (struct sockaddr*)&conn->addr, sizeof(struct sockaddr_in)) < 0){
        perror("[SERVER][SENDTO][ERROR]: Send failed");
        free(conn);
        return NULL;
    }
    printf("[SERVER][SENDTO][SUCCESS]: Response sent\n");
    free(conn);
    return NULL;
    // send response to client
    
    
}



void process_loop(int server_fd){

    while(1){
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(struct sockaddr_in);
        // peek the first packet to get client address
        int len = recvfrom(server_fd, NULL, 0, 0, (struct sockaddr*)&client_addr, &addr_len);
        if(len < 0){
            perror("[SERVER][RECVFROM][ERROR]: Receive failed");
            continue;
        }
        printf("[SERVER][RECVFROM][SUCCESS]: from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        pthread_t thread;
        ClientConnection* conn = (ClientConnection*)malloc(sizeof(ClientConnection));
        conn->fd = server_fd;
        conn->addr = client_addr;
        pthread_create(&thread, NULL, STUN_response_thread, conn);
        pthread_detach(thread);
        
    }

}


#endif