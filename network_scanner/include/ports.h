#ifndef PORTS_H
#define PORTS_H

#include <stdint.h> //for uint16_t type
#include <stddef.h> //for size_t (a counter have to not be negative)

//Ports list
typedef struct {

    uint16_t *ports; //list of ports
    size_t count; //count how much ports there is on the array
} ports_array_t;


/*Parsing function return 0 if its valid or -1 if the string is not valid
And from a string there is a list of ports
Exemple "80" -> [80]
"1000-1005" -> [1000-1001-1002 1002 1003 1004 1005]
but if we have "abc" or "700000" is not valid
so we return 2 things, the validity and the list of ports
*/
int parse_ports(const char *input, ports_array_t *list);

//Free memory function
void free_ports_array(ports_array_t *list);




#endif 