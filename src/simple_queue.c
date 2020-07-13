#include "simple_queue.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

Node* NewNode(void* data) {
    Node* object = malloc(sizeof(Node));
    object->data = data;
    object->next = NULL;
    object->prev = NULL;
    return object;
}

Simple_queue* NewSimpleQueue(int size) {
    Simple_queue* object = malloc(sizeof(Simple_queue));
    object->size = size;
    object->length = 0;
    object->head = NULL;
    object->tail = NULL;
    return object;
}

bool Pop(Simple_queue* self) {
    if (self->length == 0) {
        return false;
    }
    self->length--;
    if(self->length == 0) {
        self->head = NULL;
        self->tail = NULL;
        return true;
    }
    self->head = self->head->prev;
    return true;
}
bool Push(Simple_queue* self, void* data) {
    if (self->length == self->size) {
        return false;
    }
    if(self->length == 0) {
        self->length++;
        self->head = NewNode(data);
        self->tail = self->head;
        return true;
    }
    self->tail->prev = NewNode(data);
    if (!self->tail->prev) {
        return false;
    }
    self->length++;
    self->tail->prev->next = self->tail;
    self->tail = self->tail->prev;
    return true;
}
void* Top(Simple_queue* self) {
    return self->head->data;
}
