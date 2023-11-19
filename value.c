#include "value.h"

#include <stdlib.h>
#include <stdio.h>

void init_value_array(ValueArray* value_arr) {
    value_arr->capacity = 0;
    value_arr->count = 0;
    value_arr->values = NULL;
}

void free_value_array(ValueArray* value_arr) {
    free(value_arr->values);
    init_value_array(value_arr);
}

uint8_t write_value_array(ValueArray* value_arr, Value value) {
    if (value_arr->count + 1 > value_arr->capacity) {
        int new_capacity = value_arr->capacity < 8 ? 8 : value_arr->capacity * 2;
        value_arr->values = (Value*)realloc(value_arr->values, sizeof(Value) * new_capacity);
        value_arr->capacity = new_capacity;
    }

    value_arr->values[value_arr->count] = value;
    value_arr->count++;

    return value_arr->count - 1;
}

void print_value(Value value) {
    switch (value.type) {
        case VT_INT:
            printf("%d", AsInt(value));
            break;
        case VT_FLOAT:
            printf("%f", AsFloat(value));
            break;
        case VT_BOOL:
            printf("%s", AsBool(value) == true ? "true" : "false");
            break;
        case VT_STRING:
            printf("%s", AsStr(value));
            break;
    }
}