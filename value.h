#ifndef _value_h
#define _value_h


#include "common.h"


// The type of the value
typedef enum {
    VT_INT,
    VT_FLOAT,
    VT_STRING,
    VT_BOOL
} ValueType;


// A struct that represents a value;
// The union is what contains the actual value.
// Notice that it's named 'as', you can think of this as a sort of cast.
// Ex: Value a; a.as.flt = 32.67;
typedef struct {
    ValueType type;
    union {
        int32_t integer;
        double flt;
        char* str;
        bool boolean;
    } as;
} Value;

// These macros help us tell if a value is of certain type

#define IsOfType(value, t) (value.type == t)
#define IsInt(value) IsOfType(value, VT_INT)
#define IsFloat(value) IsOfType(value, VT_FLOAT)
#define IsStr(value) IsOfType(value, VT_STRING)
#define IsBool(value) IsOfType(value, VT_BOOL)

// And these ones cast them, in combination with the above,
// we create a safe interface in interacting with the Value struct



#define AsInt(value) (value.as.integer)
#define AsFloat(value) (value.as.flt)
#define AsStr(value) (value.as.str)
#define AsBool(value) (value.as.boolean)


typedef struct {
    int count;
    int capacity;
    Value* values;
} ValueArray;

void init_value_array(ValueArray* value_arr);
void free_value_array(ValueArray* value_arr);
uint8_t write_value_array(ValueArray* value_arr, Value value);
void print_value(Value value);

#endif