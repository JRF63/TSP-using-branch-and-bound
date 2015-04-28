#pragma once
#include <math.h>
#include "branch_and_bound_types.h"
#include "helper_functions.h"

// void cover(index_t city_num, Header* headers, Set* node_set)
// {
//     Node* next = headers[city_num].down;
//     while (next != NULL) {
//         next->left->right = next->right;
//         next->right->left = next->left;
//         next = next->down;
//     }
//     
//     Set* this_node = &((node_set->down)[city_num]);
//     this_node->up->down = this_node->down;
//     this_node->down->up = this_node->up;
// }
// 
// void uncover(index_t city_num, Header* headers, Set* node_set)
// {
//     Set* this_node = &((node_set->down)[city_num]);
//     this_node->up->down = this_node;
//     this_node->down->up = this_node;
//     
//     Node* next = headers[city_num].down;
//     while (next != NULL) {
//         next->left->right = next;
//         next->right->left = next;
//         next = next->down;
//     }
// }

void branch_and_bound(Point* cities, const index_t num_cities)
{       
    Header* headers = (Header*)malloc(sizeof(Header)*(num_cities + 1));
    Node* city_matrix = (Node*)malloc(sizeof(Node)*(num_cities + 1)*num_cities);
    
    headers[0].right = NULL; // di kailangan?
    headers[0].up = NULL;
    headers[0].down = &(headers[1]);
    
    
    for (index_t i = 0; i < num_cities; ++i) {
        Point* city_ein = &(cities[i]);
        coord_t ein_x = city_ein->x;
        coord_t ein_y = city_ein->y;
        
        Node* sub_matrix = city_matrix + i*(num_cities + 1);
        Node* start = &(sub_matrix[0]);
        start->cost = 0;
        start->city_num = num_cities;
        start->left = NULL;
        start->right = NULL;
        
        headers[i + 1].right = start;
        headers[i + 1].up = &(headers[i]);
        headers[i + 1].down = (i == num_cities - 1) ? NULL : &(headers[i + 2]);
        
        for (index_t j = 0; j < num_cities; ++j) {
            coord_t dist;
            if (i == j) {
                dist = HUGE_VAL;
            } else {
                Point* city_zwei = &(cities[j]);
                coord_t dist_x = ein_x - city_zwei->x;
                coord_t dist_y = ein_y - city_zwei->y;
                dist = sqrt(dist_x*dist_x + dist_y*dist_y);
            }
            
            Node* node = &(sub_matrix[j + 1]);
            node->cost = dist;
            node->city_num = j;
            
            Node* prev = &(sub_matrix[0]);
            while (prev->right != NULL && prev->right->cost <= dist) {
                prev = prev->right;
            }
            
            if (prev->right != NULL) {
                prev->right->left = node;
            }
            node->left = prev;
            node->right = prev->right;
            prev->right = node;
        }
    }
    
#ifdef DEBUG
    // debug 1
    printf("Test #1\n");
    for (index_t i = 0; i < num_cities; ++i) {
        printf("(%2d)\t", i);
        Node* pika = headers[i + 1].right;
        while (pika->right != NULL) {
            printf("%12lf %3d\t", pika->right->cost, pika->right->city_num);
            pika = pika->right;
        }
        printf("\n");
    }
    
    // debug 2
    printf("\nTest #2\n");
    for (index_t i = 0; i < num_cities; ++i) {
        printf("(%2d)\t", i);
        for (index_t j = 1; j <= num_cities; ++j) {
            #define IDX(i, j) ((i)*(num_cities + 1) + (j))
            Node* pika = &(city_matrix[IDX(i, j)]);
            printf("%12lf %3d\t", pika->cost, pika->city_num);
        }
        printf("\n");
    }
#endif
    
    free(headers);
    free(city_matrix);
}