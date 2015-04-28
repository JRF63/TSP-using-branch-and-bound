#pragma once
#include <stdbool.h>
#include "branch_and_bound_types.h"

#ifdef EXACTLY_ON_SEGMENT
#include <math.h>
#endif

inline coord_t direction(Point* p_i, Point* p_j, Point* p_k)
{
    const coord_t x_1 = p_k->x - p_i->x;
    const coord_t y_1 = p_k->y - p_i->y;
    const coord_t x_2 = p_j->x - p_i->x;
    const coord_t y_2 = p_j->y - p_i->y;
    return x_1 * y_2 - x_2 * y_1;
}

// #ifdef EXACTLY_ON_SEGMENT
// inline bool on_segment(Point p_i, Point p_j, Point p_k)
// {
//     const coord_t x_i = p_i.x;
//     const coord_t y_i = p_i.y;
//     const coord_t x_j = p_j.x;
//     const coord_t y_j = p_j.y;
//     const coord_t x_k = p_k.x;
//     const coord_t y_k = p_k.y;
//     if (fmin(x_i, x_j) <= x_k && x_k <= fmax(x_i, x_j) &&
//         fmin(y_i, y_j) <= y_k && y_k <= fmin(y_i, y_j)) {
//         return true;
//     } else {
//         return false;
//     }
// }
// #endif

inline bool segments_intersect(Point* p_1, Point* p_2, Point* p_3, Point* p_4)
{
    const coord_t d_1 = direction(p_3, p_4, p_1);
    const coord_t d_2 = direction(p_3, p_4, p_2);
    const coord_t d_3 = direction(p_1, p_2, p_3);
    const coord_t d_4 = direction(p_1, p_2, p_4);
    if (((d_1 > 0 && d_2 < 0) || (d_1 < 0 && d_2 > 0)) &&
        ((d_3 > 0 && d_4 < 0) || (d_3 < 0 && d_4 > 0))) {
        return true;
    }
// #ifdef EXACTLY_ON_SEGMENT
//     else if (d_1 == 0 && on_segment(p_3, p_4, p_1)) {
//         return true;
//     } else if (d_2 == 0 && on_segment(p_3, p_4, p_2)) {
//         return true;
//     } else if (d_3 == 0 && on_segment(p_1, p_2, p_3)) {
//         return true;
//     } else if (d_4 == 0 && on_segment(p_1, p_2, p_4)) {
//         return true;
//     }
// #endif
    else {
        return false;
    }
}

index_t parser(char* filename, Point** cities)
{
    index_t num_cities;
    coord_t x, y;
    
    FILE* input = fopen(filename, "r");
    
    fscanf(input, "%d\n", &num_cities);
    *cities = (Point*)malloc(sizeof(Point)*num_cities);
    
    for (index_t i = 0; i < num_cities; ++i) {
        fscanf(input, "%lf %lf\n", &x, &y);
        (*cities)[i].x = x;
        (*cities)[i].y = y;
    }
    
    fclose(input);
    return num_cities;
}