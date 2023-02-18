#include "lista.h"

struct Dir;
struct File;

typedef struct Dir{
	char *name; 
	// numele directorului
	struct Dir* parent; 
	// directorul parinte
	TLista children_files; 
	// lista de fisiere continute
	TLista children_dirs;
	// lista de directoare continute
} Dir;

Dir *initDir(char *name);
// initailizam un director cu numele name
void delDir(void *dir);
// dezalocam memoria ocupata de un director si de continutul acestuia
int compDir(const void *dirA,const void *dirB);
// comparam numele a doua directoare
void afisDir(void *dir, FILE *out);
// afisam numele unei director
typedef struct File {
	char *name;
	// numele fisierului
	struct Dir* parent;
	// directorul parinte
} File;

File *initFile(char *name);
// initializam un fisier cu numele name
void delFile(void *file);
// dezalocam memoria ocupata de un fisier
int compFile(const void *fileA, const void *fileB);
// comparam numele a 2 fisiere
void afisFile(void *file, FILE *out);
// afisam numele fisierului
typedef enum {_touch, _mkdir, _ls, _rm, _rmdir, 
			  _cd, _pwd, _stop, _tree, _mv, _err} Command;

Command getCommandType(char *op);
// obtine tipul operatiei dintr-un string

void touch (Dir* parent, char* name) ;
// creaza un fisier cu numele name in directorul parent
void mkdir (Dir* parent, char* name) ;
// creaza un director cu numele name in directorul parent
void ls (Dir* parent) ;
// afiseaza numele directoarelor si fisierelor din directorul parent
void rm (Dir* parent, char* name) ;
// sterge fisierul cu numele name din directorul parent
void rmdir (Dir* parent, char* name) ;
// sterge directorul cu numele name din directorul parent
void cd(Dir** target, char *name) ;
// schimba directorul target in directorul cu numele name
char *pwd (Dir* target) ;
// intoarce un string ce contine calea directorului curent
void stop (Dir* target) ;
// elibereaza memoria ocupata de sistemul de fisiere si opreste rularea programului
void tree (Dir* target, int level) ;
// afiseaza continutul directorului si a subdirectoarelor in mod recursiv
void mv(Dir* parent, char *oldname, char *newname) ;
// redenumeste fisierul/directorul cu numele oldname ls numele newname