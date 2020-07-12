#ifndef SIMPLE_CLOSURE_H_
#define SIMPLE_CLOSURE_H_

// 不知道如何當函式指標傳入

typedef struct ClosureInt {
    void** parameters;
    int (*call)(struct ClosureInt* self, void** parameters);
} ClosureInt;

typedef struct ClosureDouble {
    void** parameters;
    double (*call)(struct ClosureDouble* self, void** parameters);
} ClosureDouble;

typedef struct ClosureArray {
    void** parameters;
    void* (*call)(struct ClosureArray* self, void** parameters);
} ClosureArray;

#endif  // SIMPLE_CLOSURE_H_
