#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_branch_and_bound.h"

int main(int argc, char* argv[])
{
    index_t num_cities;
    Point* cities;
    num_cities = parser(argv[1], &cities);
    
    branch_and_bound(cities, num_cities);
    
    free(cities);
}