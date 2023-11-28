#include "redis.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void repl() {
    init_redis();

    int id = create_instance();
    char* output_buffer = calloc(sizeof(char), 4096);

    for (;;) {
        char buf[1024] = {'\0'};
        printf("> ");
        fgets(buf, sizeof(buf), stdin);        
    
        if (strcmp(buf, "exit") == 0) {
            break;
        }

        buf[strlen(buf) - 1] = '\0';

        
        output_buffer = query(id, buf);
        
        printf("%s\n", output_buffer);
    }

    free(output_buffer);

}

int main(int argc, char* argv[]) {

    if (argc == 1) {
        repl();
    }


    return 0;
}