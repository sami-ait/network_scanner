#include "scanner.h"

#include <sys/socket.h> //for socket manipulations
#include <fcntl.h> // file descriptor control
#include <poll.h> // poll() system call and pollfd structure
#include <unistd.h> //Posix system calls
#include <errno.h> //for errno
#include <string.h> //For memcpy use
#include <netinet/in.h> //For struct sockaddr_in sockaddr_in6 and htons

port_state_t check_port_state(const struct sockaddr *address, socklen_t addrlen, 
uint16_t port, int timeout){

    
    struct sockaddr_storage address_copy = {0};
    port_state_t result = PORT_STATE_ERROR;
    int sockfd = -1; //Ask for connexion

    // View the generic storage as an IPv4 address to set the port
    struct sockaddr_in *addr_v4 = (struct sockaddr_in *)&address_copy;
     // View the generic storage as an IPv6 address to set the port
    struct sockaddr_in6 *addr_v6 = (struct sockaddr_in6 *)&address_copy;

    struct pollfd fds; //Used by poll() to wait for the non-blcking connect to complete
    

    if(addrlen > (socklen_t)sizeof(address_copy)){
        result = PORT_STATE_ERROR;
        goto cleanup;
    }

    memcpy(&address_copy, address, addrlen);

    //if IPV4
    if(address->sa_family == AF_INET){
        addr_v4->sin_port = htons(port);
    }

    else if (address->sa_family == AF_INET6){
         addr_v6->sin6_port = htons(port);
    }
    else {
        goto cleanup;
    }

    sockfd = socket(address->sa_family, SOCK_STREAM, 0);
    if(sockfd == -1){
        goto cleanup;
    }

    

    //We make the socket in non-blocking mode so that connect() returns immediately and we apply our own timeout with poll()
    int current_flags = fcntl(sockfd, F_GETFL);
    if(current_flags == -1){
        goto cleanup;
    }
    if(fcntl(sockfd, F_SETFL, current_flags | O_NONBLOCK) == -1){
        goto cleanup;
    }

    int connect_result = connect(sockfd, (struct sockaddr *)&address_copy, addrlen);

  //for getsockopt 
    int so_error = 0;
    socklen_t len_so_error = sizeof(so_error);

    //start connexion
    if(connect_result == 0){
        result = PORT_STATE_OPEN;
        goto cleanup;
    } 

    else{
        if(errno == EINPROGRESS){
            fds.fd = sockfd;
            fds.events = POLLOUT;
            int poll_result = poll(&fds, 1, timeout);
    
            //wait with poll the result with a timeout
            if(poll_result == 0){
                result = PORT_STATE_FILTERED;
                goto cleanup;
            }
            else if(poll_result == -1){
                result = PORT_STATE_ERROR;
                goto cleanup;
            }
            else if (poll_result > 0){
                //read the result
                if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len_so_error) == -1){
                    goto cleanup;
                }
                result = error_to_port_state(so_error);
            }

        }
        else {
            result = error_to_port_state(errno);   //If connect fails
            goto cleanup;
        }
    }
    

//we make label for clean
cleanup: 
    if(sockfd != -1){
        close(sockfd);
    }
    return result;
}

/*Return a state port who correspond to a error code
for exemple he receuvre 0 or ECCONREFUSED, the function return open or closed or filtered or error */
static port_state_t error_to_port_state(int error_code){
    switch(error_code){
        case 0:
            return PORT_STATE_OPEN;
        case ECONNREFUSED:
            return PORT_STATE_CLOSED;
        case ETIMEDOUT:
        case ENETUNREACH:
        case EHOSTUNREACH:
            return PORT_STATE_FILTERED;
        default :
            return PORT_STATE_ERROR;
    }
}
