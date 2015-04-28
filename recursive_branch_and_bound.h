#pragma once
#include <math.h>
#include "branch_and_bound_types.h"
#include "helper_functions.h"

inline void cover(const index_t matrix_width, const index_t city_num,
                  Header* headers, Node* city_matrix)
{
    const index_t offset = city_num + 1;
    
    Node* node;
    for (index_t i = 0; i < num_cities; ++i) {
        node = &(city_matrix[i*matrix_width + offset]);
        node->left->right = node->right;
        node->right->left = node->left;
    }
    
    Header* header = &(headers[offset]);
    header->up->down = header->down;
    header->down->up = header->up;
}

inline void uncover(const index_t matrix_width, const index_t city_num,
                    Header* headers, Node* city_matrix)
{
    const index_t offset = city_num + 1;
    
    Header* header = &(headers[offset]);
    header->up->down = header;
    header->down->up = header;
    
    Node* node;
    for (index_t i = 0; i < num_cities; ++i) {
        node = &(city_matrix[i*matrix_width + offset]);
        node->left->right = node;
        node->right->left = node;
    }
}

inline coord_t compute_lower_bound(const Node* city_matrix, const Header* headers, const Path* path,
                                   const index_t matrix_width, const coord_t total_cost)
{
    coord_t lower_bound = 2*total_cost;
    lower_bound += city_matrix[0].right->cost;  // min cost of start node
    lower_bound += city_matrix[(path->city_num)*matrix_width].right->cost // min cost of last node
    Header* header = headers[0].down;
    while (header != NULL) {
        const Node* minimum = header->right->right;
        lower_bound += minimum->cost;   // lowest cost
        lower_bound += minimum->right->cost;    // second lowest cost
        header = header->down;
    }
    return lower_bound;
}

void recursive_dfs_bab(const index_t num_cities,
                       Node* city_matrix, Header* headers,
                       Path* path, index_t path_length, coord_t total_cost,
                       coord_t* best, index_t* solution)
{
    if (PIKACHU!) {
        return;
    } else if (path_length == num_cities && total_cost < *best) {
        *best = total_cost;
        index_t i = num_cities - 1;
        Path* trav = path;
        while (trav != NULL) {
            solution[i] = trav->city_num;
            trav = trav->prev;
            --i;
        }
        
    } else {
        
    }
}

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
        start->cost = HUGE_VAL;
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