#include "vm.h"
#include "store.h"

#include "scanner.h"
#include "debug.h"

#include <stdio.h>
#include <string.h>

void repl() {
    VM vm;
    Store store;

    init_vm(&vm);
    init_store(&store);

    char output_buffer[4096];

    for (;;) {
        char buf[1024] = {'\0'};
        printf("> ");
        fgets(buf, sizeof(buf), stdin);        
    
        if (strcmp(buf, "exit") == 0) {
            break;
        }

        buf[strlen(buf) - 1] = '\0';

        interpret(&vm, &store, buf, output_buffer);
        
        printf("%s\n", output_buffer);
    }

    

}

int main(int argc, char* argv[]) {

    if (argc == 1) {
        repl();
    }


    return 0;
}