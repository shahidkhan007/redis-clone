#include "vm.h"
#include "store.h"

#include <stdio.h>

void init_vm(VM* vm) {
    // Right now, there's nothing here but later we will
}

void free_vm(VM* vm) {
    // Same here
}

static void exec_get(VM* vm) {
    #define ReadByte() (*vm->ip++)
    #define ReadConstant() (vm->chunk->constants.values[ReadByte()])

    Value key = ReadConstant();

    Value value;
    GetResult result = get_store(vm->store, &value, key);

    switch (result) {
        case GET_OK: {
            print_value(value);
            printf("\n");
            break;
        }
        case GET_NO_SUCH_KEY: {
            printf("No such key: ");
            print_value(key);
            printf("\n");
            break;
        }
    }

    #undef ReadByte
    #undef ReadConstant
}


static void exec_set(VM* vm) {
    #define ReadByte() (*vm->ip++)
    #define ReadConstant() (vm->chunk->constants.values[ReadByte()])

    Value key = ReadConstant();
    Value value = ReadConstant();

    SetResult result = set_store(vm->store, key, value);

    switch (result) {
        case SET_OK: {
            print_value(key);
            printf(" => ");
            print_value(value);
            printf("\n");
            break;            
        }
        case SET_DUPLICATE_KEY: {
            printf("A value for the key '");
            print_value(key);
            printf("' already exists.\n");
            break;
        }
    }

    #undef ReadByte
    #undef ReadConstant
}

static InterpretResult run(VM* vm) {
    #define ReadByte() (*vm->ip++)

    for (;;) {
        uint8_t instruction = ReadByte();
        InterpretResult result;

        switch (instruction) {
            case OP_RETURN: {
                // printf("Exiting gracfully.\n");
                return INTERPRET_OK;
            }

            case OP_GET: {
                exec_get(vm);
                break;
            }

            case OP_SET: {
                exec_set(vm);
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

InterpretResult interpret(VM* vm, Chunk* chunk, Store* store) {
    vm->chunk = chunk;
    vm->ip = vm->chunk->code;
    vm->store = store;
    return run(vm);

}

