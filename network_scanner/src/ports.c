#include <stdlib.h> //for strtol and free
#include <stdint.h> //for U_INT type

#include "ports.h"

/*Helper function that check if a number was read and if its between 1 and 65535*/
static int check_and_read_number(const char *cursor, uint16_t *read_port, const char **end_read){

    char *end;  //for strtol (2Nd setting without const)
    long number = strtol(cursor, &end, 10);

    if(end == cursor){
            return -1;
    }

    if(number < 1 || number > 65535){
        return -1;
    }

    //Writing of résult
    *read_port = (uint16_t) number;
    *end_read = end;

    return 0;
}


/*The function take ports from the user, */
int parse_ports(const char *input, ports_array_t *list){

    list->ports = NULL;
    list->count = 0;

    if(input == NULL || input[0] == '\0'){
        return -1;
    }

    const char *cursor = input;//For travel in the ports input

    list->ports = malloc(sizeof(*list->ports)*UINT16_MAX); //We can put the max of ports possible (65535)
    if(list->ports == NULL){
        return -1;
    }

    uint16_t range_start=0, range_end=0; //if we have one port range_start = range_end


    /*we stop when we read all the input*/
    while(*cursor != '\0'){
        if(check_and_read_number(cursor, &range_start, &cursor) == -1){
            goto cleanup;
        }

        if(*cursor == '-'){
            cursor++; //We jump the -
            if(check_and_read_number(cursor, &range_end, &cursor) == -1){
                goto cleanup;
            }
            if(range_start > range_end){
                goto cleanup;
            }
        }
        else {
            range_end = range_start;
        }

        
            //We will add the ports to the list
        for(uint32_t port = range_start; port <= range_end; port++){
            if(list->count >= UINT16_MAX){ //for not writing outside the array
                goto cleanup;
             }
             list->ports[list->count] = (uint16_t)port;
            list->count++;
        }

        if(*cursor == ','){
            cursor++;
            if(*cursor == '\0'){ //In case we have for exemple 80, this is not good
                goto cleanup;
            }
        }

        // Only a comma can separate two entries: anything else (e.g. "80 90") is invalid
        else if (*cursor != '\0') {
            goto cleanup;
        }

    }

    return 0;
     
cleanup:
    free(list->ports);
    list->ports = NULL;
    list->count = 0;
    return -1;
}


void free_ports_array(ports_array_t *list){
    if(list != NULL){
        free(list->ports);
        list->ports = NULL;
        list->count = 0;
    }
}
