#pragma once
#include <math.h>
#include "branch_and_bound_types.h"
#include "helper_functions.h"

void cover(index_t city_num, Header* headers, Set* node_set)
{
    Node* next = headers[city_num].down;
    while (next != NULL) {
        next->left->right = next->right;
        next->right->left = next->left;
        next = next->down;
    }
    
    Set* this_node = &((node_set->down)[city_num]);
    this_node->up->down = this_node->down;
    this_node->down->up = this_node->up;
}

void uncover(index_t city_num, Header* headers, Set* node_set)
{
    Set* this_node = &((node_set->down)[city_num]);
    this_node->up->down = this_node;
    this_node->down->up = this_node;
    
    Node* next = headers[city_num].down;
    while (next != NULL) {
        next->left->right = next;
        next->right->left = next;
        next = next->down;
    }
}

void branch_and_bound(Point* cities, const index_t num_cities)
{
    Header* headers = (Header*)malloc(sizeof(Header)*num_cities);
    Set* node_set = (Set*)malloc(sizeof(Set)*(num_cities + 1));
    Node* city_matrix = (Node*)malloc(sizeof(Node)*num_cities*(num_cities + 1));
    
    node_set[0].city_num = num_cities;
    node_set[0].up = NULL;
    node_set[0].down = &(node_set[1]);
    for (index_t i = 1; i <= num_cities; ++i) {
        node_set[i].city_num = i;
        node_set[i].up = &(node_set[i - 1]);
        node_set[i].down = (i == num_cities) ? NULL : &(node_set[i + 1]);
    }
    
    coord_t* temp_costs_array = (coord_t*)malloc(sizeof(coord_t)*num_cities);
    index_t* index_holder = (index_t*)malloc(sizeof(index_t)*num_cities);
    Node** prev_city_array = (Node**)malloc(sizeof(Node*)*num_cities);

    for (index_t i = 0; i < num_cities; ++i) {
        Point* city_ein = &(cities[i]);
        coord_t ein_x = city_ein->x;
        coord_t ein_y = city_ein->y;
        
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
            
            // sort
            index_t true_pos = 0;
            while (temp_costs_array[true_pos] <= dist && true_pos < j) {
                ++true_pos;
            }
            for (index_t pos = j; pos > true_pos; --pos) {
                temp_costs_array[pos] = temp_costs_array[pos - 1];
                index_holder[pos] = index_holder[pos - 1];
            }
            temp_costs_array[true_pos] = dist;
            index_holder[true_pos] = j;
        }
        
        Node* sub_matrix = city_matrix + i*(num_cities + 1);
        
        sub_matrix[0].cost = 0;
        sub_matrix[0].city_num = 0;
        sub_matrix[0].left = NULL;
        sub_matrix[0].right = &(sub_matrix[1]);
        for (index_t k = 0; k < num_cities; ++k) {
            Node* node = &(sub_matrix[k + 1]);
            node->cost = temp_costs_array[k];
            node->city_num = index_holder[k];
            node->left = node - 1;
            node->right = (k == num_cities - 1) ? NULL : node + 1;
            
            if (i == 0) {
                headers[index_holder[k]].down = node;
            } else {
                prev_city_array[index_holder[k]]->down = node;
                if (i == num_cities - 1) {
                    node->down = NULL;
                }
            }
            prev_city_array[index_holder[k]] = node;
        }
    }
    free(temp_costs_array);
    free(index_holder);
    free(prev_city_array);
    
#ifdef DEBUG
    // debug 1
    printf("Test #1\n");
    Node* pika = city_matrix + 1*(num_cities + 1);
    while (pika->right != NULL) {
        printf("%lf %d\n", pika->right->cost, pika->right->city_num);
        pika = pika->right;
    }
    
    // debug 2
    printf("\nTest #2\n");
    for (index_t i = 0; i < num_cities; ++i) {
        Node* pika = headers[i].down;
        printf("(%2d)\t", i);
        while (pika != NULL) {
            printf("%12lf %3d\t", pika->cost, pika->city_num);
            pika = pika->down;
        }
        printf("\n");
    }
#endif
    
    free(headers);
    free(node_set);
    free(city_matrix);
}