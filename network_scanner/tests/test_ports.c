/*
This is a program with his proper main. He call parse_ports with differents entry.
He check the result and give a summary

u can do make test
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "ports.h"

static int error_count=0;

static void check_function(bool condition, const char *test_name){

    if(!condition){
        printf("FAIL_%s\n", test_name);
        error_count++;
    }
}

static bool test_valid_input(const char *input,const uint16_t *expected_array, size_t expected_count){

    ports_array_t list;
    bool result = true;

    if(parse_ports(input, &list) == -1){
        result = false;
    }

    if(list.count != expected_count){
        result = false;
    }

    else {
        for(size_t i=0; i < list.count; i++){
            if (list.ports[i] != expected_array[i]){
                result=false;
                break;
            }
        }
    }

    free_ports_array(&list);

    return result;
}


static void run_valid_cases(void){
    const uint16_t expected_single[] = {80};
    check_function(test_valid_input("80", expected_single, 1), "single port");

    const uint16_t expected_list[] = {22, 80};
    check_function(test_valid_input("22,80", expected_list, 2), "comma separated list");

    const uint16_t expected_range[] = {1000, 1001, 1002};
    check_function(test_valid_input("1000-1002", expected_range, 3), "port range");
}

bool test_invalid_input(const char *input){

    ports_array_t list;
    bool result = true;

    if(parse_ports(input, &list) != -1){
        result = false;
    }
    if(list.ports != NULL || list.count != 0){
        result = false;
    }

    free_ports_array(&list);

    return result;
}

static void run_invalid_cases(void){
    check_function(test_invalid_input("abc"), "letters");
    check_function(test_invalid_input("0"), "port zero");
    check_function(test_invalid_input("70000"), "port too large");
    check_function(test_invalid_input("100-50"), "reversed range");
    check_function(test_invalid_input("80,"), "trailing comma");
    check_function(test_invalid_input("80 90"), "space instead of comma");
    check_function(test_invalid_input(""), "empty string");
}

int main(void){

    run_valid_cases();
    run_invalid_cases();

    printf("Le nombre d'échecs est %d\n", error_count);

    if(error_count > 0){
        return 1;
    }

    return 0;
}



