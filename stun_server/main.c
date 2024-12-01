// Creator : Rudy

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "./sockutils/sockutils.h"
// tambahkan untuk sleep function
#include <unistd.h>
#define PORT 5080
#define BUFFER_SIZE 1024

int main() {
    // buat ipv4 udp socket
    int server_fd = createUDPIpv4Socket();
    if(server_fd < 0){
        return -1;
    }
    // buat ipv4 address
    struct sockaddr_in* server_addr = createIpv4Address("", PORT);
    if(server_addr == NULL){
        return -1;
    }
    // bind socket ke address
    if(bind(server_fd, (struct sockaddr*)server_addr, sizeof(struct sockaddr_in)) < 0){
        perror("[SERVER][BIND]: Bind failed");
        return -1;
    }else{
        printf("[SERVER][BIND]: Bind success\n");
    }
    // jalankan process loop
    process_loop(server_fd);
    // tutup socket
    printf("[SERV][SOCKET]: Closing socket\n");
    close(server_fd);
    return 0;

}
