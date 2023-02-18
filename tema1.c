#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "folder.h"
#define MAX_INPUT_LINE_SIZE 300


int main () {
	
    char tokens[3] = " \n\r";
    
	Dir *currentDirectory = initDir("home");
    // initializam directorul home
    char *command = (char *)malloc(MAX_INPUT_LINE_SIZE);
	while(fgets(command, MAX_INPUT_LINE_SIZE - 1, stdin) != NULL)
	{   // citim o linie
        char *p = strtok(command, tokens);
		char *name;
		char *path;
        switch(getCommandType(p)){
            // vedem ce comanda trebuie executata
            case _touch:
                p = strtok(NULL, tokens);
                name = p;
                touch(currentDirectory, name);
            break;
			case _mkdir:
                p = strtok(NULL, tokens);
                name = p;
                mkdir(currentDirectory, name);
            break;
			case _ls:
                ls(currentDirectory);
            break;
			case _rm:
                p = strtok(NULL, tokens);
                name = p;
                rm(currentDirectory, name);
            break;
			case _rmdir:
                p = strtok(NULL, tokens);
                name = p;
                rmdir(currentDirectory, name);
            break;
			case _cd:
                p = strtok(NULL, tokens);
                name = p;
                cd(&currentDirectory, name);
            break;
			case _pwd:
				path = pwd(currentDirectory);
				printf("%s\n", path);
				free(path);
            break;
			case _stop:
                // eliberam memoria
	            free(command);
                stop(currentDirectory);
            break;
			case _tree:
                tree(currentDirectory, 0);
            break;
			case _mv:
                p = strtok(NULL, tokens);
                char *oldname = p;
				p = strtok(NULL, tokens);
                char *newname = p;
                mv(currentDirectory, oldname, newname);
            break;
            default:
            break;
        }
	}
	return 0;
}
