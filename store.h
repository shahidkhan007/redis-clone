#ifndef _store_h
#define _store_h

#include "value.h"

// This module exposes an interface on top of which the VM will operate,
// how it's actually implemented is none of VM's business. That's why
// at first we'll start with the simplest implementation, a dynamic array.
// Later, We'll will change this to a Hashmap


// This struct will represent the store, for now
typedef struct {
    ValueArray keys;
    ValueArray values;
} Store;


// This tells us wheather a retrieval failed or not, and if so, what happended
typedef enum {
    GET_OK,
    GET_NO_SUCH_KEY
} GetResult;


// Same here but for inserts
typedef enum {
    SET_OK,
    SET_DUPLICATE_KEY
} SetResult;


// This is the API exposed, as long as we don't make changes to this,
// the actual storage mechanism doesn't matter.
// If we do make changes to this, we'll also have to make changes in the VM

void init_store(Store* store);
void free_store(Store* store);
GetResult get_store(Store* store, Value* dest, Value key);
SetResult set_store(Store* store, Value key, Value value);

#endif