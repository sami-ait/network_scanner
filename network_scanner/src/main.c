#include <stdio.h>
#include <arpa/inet.h> //for inet_pton 
#include <netinet/in.h> //for sockaddr_in

#include "scanner.h"
#include "ports.h"

#define TEST_PORT_OPEN 8000
#define TEST_PORT_CLOSED 8001
#define TIMEOUT_MS 1000


static const char *port_state_to_string(port_state_t port_state){
    switch(port_state){
        case PORT_STATE_OPEN:
            return "open";
        case PORT_STATE_CLOSED:
            return "close";
        case PORT_STATE_FILTERED:
            return "filtered";
        default:
            return "error";
    }
}

int main(void){

    struct sockaddr_in address = {
        .sin_family = AF_INET,
    };

    port_state_t state;
    const uint16_t ports_array[2] = {TEST_PORT_OPEN, TEST_PORT_CLOSED};

    ports_array_t list;

    //the function converts the caracter string into a network address
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) !=1 ){
        return 1;
    }

    for(int i=0; i < 2; i++){
        state = check_port_state((struct sockaddr *)&address, sizeof(address), ports_array[i], TIMEOUT_MS);
        printf("le port est %u l'etat est %s\n", ports_array[i], port_state_to_string(state));
    }


    if(parse_ports("abc,0,100-50", &list) == 0){
        printf("Le nombre de ports est : %zu\n", list.count);
        for(size_t i = 0; i < list.count; i++){
            printf("Port : %u\n", list.ports[i]);
        }
    }
    else {
        printf("Invalid port specification\n");
    }
    
    free_ports_array(&list);


    return 0;
}