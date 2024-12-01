#ifndef ICE_DESCRIPTOR_H
#define ICE_DESCRIPTOR_H
#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t ip;
    uint16_t port;
} ICE;

typedef struct{
    ICE stun;
    ICE turn;
}ICE_stun_response;

typedef struct{
    ICE private_ip;
    ICE public_ip;
    ICE turn;
    uint8_t connection_established;
}ICE_signalling;
#endif