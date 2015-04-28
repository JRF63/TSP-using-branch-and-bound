#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_branch_and_bound.h"

int main(int argc, char* argv[])
{
    index_t num_cities;
    Point* cities;
    num_cities = parser(argv[1], &cities);
    
    coord_t best;
    index_t* solution = (index_t*)calloc(num_cities, sizeof(index_t));
    best = branch_and_bound(cities, num_cities, solution);
    
    printf("Cost of minimum tour: %lf\n", best);
    for (index_t i = 0; i < num_cities; ++i) {
        if (i == num_cities - 1) {
            printf("%d -> %d\n", solution[i], solution[0]);
        } else {
            printf("%d -> ", solution[i]);
        }
    }
    
    free(cities);
    free(solution);
}