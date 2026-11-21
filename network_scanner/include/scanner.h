#ifndef SCANNER_H
#define SCANNER_H
 
#include <stdint.h> //for uint16_t type
#include <sys/socket.h> // for sockaddr and socklen

typedef enum {
    PORT_STATE_OPEN,  //FAIRE COMMENTAIRE POUR CHAQUE ETAT
    PORT_STATE_CLOSED,
    PORT_STATE_FILTERED,
    PORT_STATE_ERROR
} port_state_t;


/*
This function returns the port state, the timeout on miliseconds, address not changed
return PORT_STATE_ERROR on a local error case
*/
port_state_t check_port_state(const struct sockaddr *address, socklen_t addrlen, 
    uint16_t port, int timeout);

#endif