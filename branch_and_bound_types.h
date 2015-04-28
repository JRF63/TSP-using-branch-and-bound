#pragma once
#include <stdint.h>

typedef double coord_t;
typedef uint32_t index_t;

typedef struct
{
    coord_t x, y;
} Point;

typedef struct node
{
    coord_t cost;
    index_t city_num;
    struct node* left;
    struct node* right;
    struct node* down;
} Node;

typedef struct
{
    Node* down;
} Header;

typedef struct set
{
    index_t city_num;
    struct set* up;
    struct set* down;
} Set;