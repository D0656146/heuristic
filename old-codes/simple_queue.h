/**
 * A simple queue in C for tabu search
 */
#ifndef SIMPLE_QUEUE_H_
#define SIMPLE_QUEUE_H_

#include <stdbool.h>

typedef struct Node
{
    void* data;
    struct Node* prev;
    struct Node* next;
}Node;

Node* NewNode(void* data);

typedef struct Simple_queue {
    int size;
    int length;
    struct Node* head;
    struct Node* tail;
} Simple_queue;

Simple_queue* NewSimpleQueue(int size);
bool Pop(Simple_queue* self);
bool Push(Simple_queue* self, void* data);
void* Top(Simple_queue* self);

#endif  // SIMPLE_QUEUE_H_
