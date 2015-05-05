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
} Node;

typedef struct header
{
    Node* right;
    struct header* up;
    struct header* down;
} Header;

typedef struct path
{
    index_t city_num;
    struct path* prev;
} Path;