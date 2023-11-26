#include "vm.h"
#include "debug.h"
#include "store.h"
#include "compiler.h"
#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_vm(VM* vm) {
    
}

void free_vm(VM* vm) {
    
}

static void to_json(char* dest, int opcode, int status, Value* value) {
    char value_str[1024];

    if (value != NULL) {
        switch (value->type) {
            case VT_INT:
                snprintf(value_str, sizeof(value_str), "%ld", AsInt(*value));
                break;
            case VT_FLOAT:
                snprintf(value_str, sizeof(value_str), "%f", AsFloat(*value));
                break;
            case VT_BOOL:
                snprintf(value_str, sizeof(value_str), "%s", AsBool(*value) == true ? "true" : "false");
                break;
            case VT_STRING:
                snprintf(value_str, sizeof(value_str), "%s", AsStr(*value));
                break;
        }
    }
    
    snprintf(dest, 4096, "{\"op\": %d, \"status\": %d, \"value\": \"%s\", \"type\": %d}", opcode, status, value_str, value->type);
}

static void exec_get(VM* vm, char* output_buffer) {
    #define ReadByte() (*vm->ip++)
    #define ReadConstant() (vm->chunk->constants.values[ReadByte()])

    Value key = ReadConstant();

    Value value;
    GetResult result = get_store(vm->store, &value, key);

    switch (result) {
        case GET_OK: {
            to_json(output_buffer, 0, GET_OK, &value);
            break;
        }
        case GET_NO_SUCH_KEY: {
            to_json(output_buffer, 0, GET_NO_SUCH_KEY, &key);
            break;
        }
    }

    #undef ReadByte
    #undef ReadConstant
}


static void exec_set(VM* vm, char* output_buffer) {
    #define ReadByte() (*vm->ip++)
    #define ReadConstant() (vm->chunk->constants.values[ReadByte()])

    Value key = ReadConstant();
    Value value = ReadConstant();

    SetResult result = set_store(vm->store, key, value);

    switch (result) {
        case SET_OK: {
            to_json(output_buffer, 1, SET_OK, &value);
            break;            
        }
        case SET_DUPLICATE_KEY: {
            to_json(output_buffer, 1, SET_DUPLICATE_KEY, &key);
            break;
        }
    }

    #undef ReadByte
    #undef ReadConstant
}

static InterpretResult run(VM* vm, char* output_buffer) {
    #define ReadByte() (*vm->ip++)

    for (;;) {
        uint8_t instruction = ReadByte();

        switch (instruction) {
            case OP_RETURN: {
                return INTERPRET_OK;
            }

            case OP_GET: {
                exec_get(vm, output_buffer);
                break;
            }

            case OP_SET: {
                exec_set(vm, output_buffer);
                break;
            }

            default:
                printf("Unknown instruction: %d\n", instruction);
                return INTERPRET_RUNTIME_ERR;
        }
    }

    return INTERPRET_OK;


    #undef ReadByte
}

InterpretResult interpret(VM* vm, Store* store, const char* source, char* output_buffer) {
    Chunk chunk;
    init_chunk(&chunk);

    if (!compile(source, &chunk)) {
        return INTERPRET_COMP_ERR;
    }

    vm->chunk = &chunk;
    vm->ip = vm->chunk->code;
    vm->store = store;

    InterpretResult result = run(vm, output_buffer);

    free_chunk(&chunk);
    return result;
}


