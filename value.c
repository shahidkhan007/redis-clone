#include "value.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_value_array(ValueArray* value_arr) {
    value_arr->capacity = 0;
    value_arr->count = 0;
    value_arr->values = NULL;
}

void free_value_array(ValueArray* value_arr) {
    for (int i = 0; i < value_arr->count; i++) {
        Value v = value_arr->values[i];

        if (IsStr(v)) {
            free(v.as.str);
        }
    }
    free(value_arr->values);
    init_value_array(value_arr);
}

uint8_t write_value_array(ValueArray* value_arr, Value value) {
    if (value_arr->count + 1 > value_arr->capacity) {
        int new_capacity = value_arr->capacity < 8 ? 8 : value_arr->capacity * 2;
        value_arr->values = (Value*)realloc(value_arr->values, sizeof(Value) * new_capacity);
        value_arr->capacity = new_capacity;
    }

    if (IsStr(value)) {
        char* mem = (char*)calloc(sizeof(char), strlen(AsStr(value)));
        strncpy(mem, AsStr(value), strlen(AsStr(value)));
        value.as.str = mem;
    }

    value_arr->values[value_arr->count] = value;
    value_arr->count++;

    return value_arr->count - 1;
}

void print_value(Value value, char* output_buffer) {
    switch (value.type) {
        case VT_INT:
            snprintf(output_buffer, 4096, "%ld", AsInt(value));
            // printf("%d", AsInt(value));
            break;
        case VT_FLOAT:
            snprintf(output_buffer, 4096, "%f", AsFloat(value));
            // printf("%f", AsFloat(value));
            break;
        case VT_BOOL:
            snprintf(output_buffer, 4096, "%s", AsBool(value) == true ? "true" : "false");
            // printf("%s", AsBool(value) == true ? "true" : "false");
            break;
        case VT_STRING:
            snprintf(output_buffer, 4096, "%s", AsStr(value));
            // printf("%s", AsStr(value));
            break;
    }
}