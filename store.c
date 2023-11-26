#include "store.h"
#include "value.h"

#include <string.h>

void init_store(Store* store) {
    init_value_array(&store->keys);
    init_value_array(&store->values);
}

void free_store(Store* store) {
    free_value_array(&store->keys);
    free_value_array(&store->values);
}

static bool are_equal(Value v1, Value v2) {
    if (v1.type == v2.type) {
        switch (v1.type) {
            case VT_INT: {
                return v1.as.integer == v2.as.integer;
            }
            case VT_FLOAT: {
                return v1.as.flt == v2.as.flt;
            }
            case VT_BOOL: {
                return v1.as.boolean = v2.as.boolean;
            }
            case VT_STRING: {
                return strcmp(v1.as.str, v2.as.str) == 0;
            }

        }
    }

    return false;
}

static Value* search_value(Store* store, Value key) {
    for (int i = 0; i < store->keys.count; i++) {
        Value target_key = store->keys.values[i];
        
        if (are_equal(key, target_key)) {
            return &store->values.values[i];
        }        
    }

    return NULL;
}

SetResult set_store(Store* store, Value key, Value value) {
    if (search_value(store, key) != NULL) {
        return SET_DUPLICATE_KEY;
    }

    write_value_array(&store->keys, key);
    write_value_array(&store->values, value);

    return SET_OK;
}

GetResult get_store(Store* store, Value* dest, Value key) {
    Value* result = search_value(store, key);
    
    if (result == NULL) {
        return GET_NO_SUCH_KEY;
    }

    *dest = *result;
    return GET_OK;
}
