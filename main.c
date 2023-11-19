#include "debug.h"
#include "chunk.h"
#include "vm.h"
#include "value.h"
#include "store.h"

int main() {
    Chunk chunk;
    VM vm;
    Store store;

    
    init_vm(&vm);
    init_chunk(&chunk);
    init_store(&store);

    Value key;
    key.type = VT_STRING;
    key.as.str = "name";

    Value value;
    value.type = VT_STRING;
    value.as.str = "shahid";

    uint8_t key_idx = write_value_array(&chunk.constants, key);
    uint8_t value_idx = write_value_array(&chunk.constants, value);
    uint8_t get_key_idx = write_value_array(&chunk.constants, key);

    write_chunk(&chunk, OP_SET);
    write_chunk(&chunk, key_idx);
    write_chunk(&chunk, value_idx);

    write_chunk(&chunk, OP_GET);
    write_chunk(&chunk, get_key_idx);


    write_chunk(&chunk, OP_RETURN);

    // disassemble_chunk(&chunk, "Hello, chunk");

    interpret(&vm, &chunk, &store);

    free_chunk(&chunk);
    free_vm(&vm);
    free_store(&store);

    return 0;
}