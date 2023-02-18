#include "folder.h"
#include <string.h>

Dir *initDir(char *name){
	// initializam un director cu numele name
	Dir *newDir = (Dir *)malloc(sizeof(Dir));
	if(newDir == NULL)
		return NULL;
	newDir->name = (char *)malloc(strlen(name) + 1);
	if(newDir->name == NULL)
	{
		free(newDir);
		return NULL;
	}
	strcpy(newDir->name, name);
	newDir->parent = NULL;
	newDir->children_dirs = NULL;
	newDir->children_files = NULL;
	return newDir;
}

void delDir(void *dir){
	// dezalocam memoria ocupata de director, de fisierele din el si de 
	// subdirectoare in mod recursiv
	Dir *auxDir = (Dir *)dir;
	free(auxDir->name);
	ResetL(&(auxDir->children_dirs), delDir);
	ResetL(&(auxDir->children_files), delFile);
	free(auxDir);
}

int compDir(const void *dirA,const void *dirB){
	// comparam numele a 2 directoare
	return strcmp(((Dir *)dirA)->name, ((Dir *)dirB)->name);
}

void afisDir(void *dir, FILE *out){
	// afisam numele directorului
	fprintf(out, "%s", ((Dir *)dir)->name);
}

File *initFile(char *name)
{	// initializam un fisier cu numele name
	File *newFile = (File *)malloc(sizeof(File));
    if(newFile == NULL)
		return NULL;
	newFile->name = (char *)malloc(strlen(name) + 1);
	if(newFile->name == NULL)
	{
		free(newFile);
		return NULL;
	}
	strcpy(newFile->name, name);
	newFile->parent = NULL;
	return newFile;
}

void delFile(void *file){
	// eliberam memoria ocupata de fisier
	File *auxFile = (File *)file;
	free(auxFile->name);
	free(auxFile);
}

int compFile(const void *fileA, const void *fileB){
	// comparam numele fisierelor
	return strcmp(((File *)fileA)->name, ((File *)fileB)->name);
}

void afisFile(void *file, FILE *out){
	// afisam numele fisierului
	fprintf(out, "%s", ((File *)file)->name);
}


Command getCommandType(char *op)
{
    // intoarce tipul comandei
    if(strcmp(op, "touch") == 0) 
        return _touch;
	if(strcmp(op, "mkdir") == 0) 
        return _mkdir;
	if(strcmp(op, "ls") == 0) 
        return _ls;
	if(strcmp(op, "rm") == 0) 
        return _rm;
	if(strcmp(op, "rmdir") == 0) 
        return _rmdir;
	if(strcmp(op, "cd") == 0) 
        return _cd;
	if(strcmp(op, "pwd") == 0) 
        return _pwd;
	if(strcmp(op, "stop") == 0) 
        return _stop;
	if(strcmp(op, "tree") == 0) 
        return _tree;
	if(strcmp(op, "mv") == 0) 
        return _mv;
	return _err;
    
}

void touch (Dir* parent, char* name) {
	// creaza un fisier cu numele name in directorul parent
	File *newFile = initFile(name);
	// initializeaza un fisier cu numele dat
    Dir *tempDir = initDir(name);
    // un Dir temporar, creat pentru a putea gasi directorul cu numele dat
	int isDuplicate = (FindL(parent->children_files, (void *)newFile, compFile) != NULL 
                    || FindL(parent->children_dirs, (void *)tempDir, compDir));
	// verificam daca exista un director/fisier cu acelasi nume
    delDir((void *)tempDir);
	if(isDuplicate)
	{   // daca da, nu mai cream un fisier nou
		printf("File already exists\n");
        delFile((void *)newFile);
		return;
	}
	InsertEndL(&(parent->children_files), (void *)newFile);
	// inseram fisierul in lista de fisiere a directorului
    newFile->parent = parent;
}

void mkdir (Dir* parent, char* name) {
	// creaza un director cu numele name in directorul parent
	Dir *newDir = initDir(name);
	// initializeaza un director cu numele dat
    File *tempFile = initFile(name);
    // un File temporar, creat pentru a putea gasi fisierul cu numele dat
	int isDuplicate = (FindL(parent->children_dirs, (void *)newDir, compDir) != NULL 
                    || FindL(parent->children_files, (void *)tempFile, compFile) != NULL);
	// verificam daca exista un director/fisier cu acelasi nume
    delFile((void *)tempFile);
    // verificam daca exista un director sau fisier cu numele name
	if(isDuplicate)
	{	// daca da, nu mai cream un director nou
		printf("Directory already exists\n");
        delDir((void *)newDir);
		return;
	}
	InsertEndL(&(parent->children_dirs), (void *)newDir);
	// inseram directorul in lista de directoare a directorului
    newDir->parent = parent;
}

void ls (Dir* parent) {
	// afiseaza numele directoarelor si fisierelor din directorul parent
	AfisareL(parent->children_dirs, afisDir, stdout);
	AfisareL(parent->children_files, afisFile, stdout);
}

void rm (Dir* parent, char* name) {
	// sterge fisierul cu numele name din directorul parent
	File *tempFile = initFile(name); 
	// un File temporar, creat pentru a putea gasi fisierul cu numele dat
	void *file = Find_ExtrL(&(parent->children_files), tempFile, compFile);
	if(file == NULL)
		// daca nu gaseste fisierul dat
		printf("Could not find the file\n");
	else
		delFile(file);
	delFile((void *)tempFile);
}

void rmdir (Dir* parent, char* name) {
	// sterge directorul cu numele name din directorul parent
	Dir *tempDir = initDir(name); 
	// un Dir temporar, creat pentru a putea gasi directorul cu numele dat
	void *dir = Find_ExtrL(&(parent->children_dirs), tempDir, compDir);
	if(dir == NULL)
		printf("Could not find the dir\n");
	else
		delDir(dir);
	delDir((void *)tempDir);

}

void cd(Dir** target, char *name) {
	// schimba directorul target in directorul cu numele name
    if(strcmp(name, "..") == 0)
    {	// schimba directorul curent in parintele sau
        if((*target)->parent != NULL)
            *target = (*target)->parent;
        return;
    }
    Dir *tempDir = initDir(name); 
	// un Dir temporar, creat pentru a putea gasi directorul cu numele dat
    void *dir = FindL((*target)->children_dirs, tempDir, compDir);
	delDir((void *)tempDir);
	if(dir == NULL)
		// daca nu gaseste directorul dat
		printf("No directories found!\n");
	else
		*target = dir;
	
}

void tree (Dir* target, int level) {
	// afiseaza continutul directorului si a subdirectoarelor in mod recursiv
    TLista L = target->children_dirs;
    for (; L != NULL; L = L->urm)       
    {	// afisam subdirectoarele si continutul lor
        for(int i = 0; i < level; i++)
			// adaugam o deplasare la dreapta de level ori
            printf("    ");
		afisDir(L->info, stdout);   
        printf("\n");  
		// afiseaza continutul subdirectorului la un nivel in plus la dreapta
        tree((Dir *)L->info, level + 1);
	}      
    L = target->children_files;
    for (; L != NULL; L = L->urm)       
    {	// afisam fisierele
        for(int i = 0; i < level; i++)
			// adaugam o deplasare la dreapta de level ori
            printf("    ");
		afisFile(L->info, stdout);     
		printf("\n");
	}      
}

char *pwd (Dir* target) {
	// intoarce un string ce contine calea directorului curent
    if(target == NULL)
    {
        return NULL;
    }
    if(target != NULL){
        char *path = pwd(target->parent);
		// gaseste calea directorului parinte
        if(path == NULL){
			// cazul in care intoarcem calea directorului home
			// alocam memorie pentru inceputul caii
            path = malloc(strlen(target->name) + 2);
            path[0] = '\0';
        }
        else{
			// alocam memorie pentru urmatorul nume in cale
            char *aux = realloc(path, strlen(path) + strlen(target->name) + 2);
            path = aux;
        }
		// adaugam numele directorului la sfarsitul caii
        strcat(path, "/");
        strcat(path, target->name);
        return path;
    }
}

void stop (Dir* target) {
	// elibereaza memoria ocupata de sistemul de fisiere si 
	// opreste rularea programului
	while(target->parent != NULL)
		// gaseste directorul home
		cd(&target, "..");
	delDir(target);
	exit(0);
}

void mv(Dir* parent, char *oldname, char *newname) {
	// redenumeste fisierul/directorul cu numele oldname ls numele newname
    Dir *tempDir = initDir(newname);
	// un Dir temporar, creat pentru a putea gasi directorul cu numele newname
    File *tempFile = initFile(newname);
	// un File temporar, creat pentru a putea gasi fisierul cu numele newname
    int isDuplicate = (FindL(parent->children_dirs, (void *)tempDir, compDir) != NULL 
                    || FindL(parent->children_files, (void *)tempFile, compFile) != NULL);
    delDir(tempDir);
    delFile(tempFile);
    if(isDuplicate){
		// daca gasim un fisier/director
        printf("File/Director already exists\n");
		return;
    }
    tempDir = initDir(oldname); 
    // un Dir temporar, creat pentru a putea gasi directorul cu numele dat
    void *dir = Find_ExtrL(&(parent->children_dirs), tempDir, compDir);
    delDir(tempDir);
	if(dir != NULL){
		// daca gasim un director cu numele oldname
		// il redenumim si il punem la sfarsitul listei
        strcpy(((Dir *)dir)->name, newname);
        InsertEndL(&(parent->children_dirs), dir);
        return;
    }
	tempFile = initFile(oldname); 
    // un Dir temporar, creat pentru a putea gasi directorul cu numele dat
    void *file = Find_ExtrL(&(parent->children_files), tempFile, compFile);
    delFile(tempFile);
	if(file != NULL){
		// daca gasim un fisier cu numele oldname
		// il redenumim si il punem la sfarsitul listei
        strcpy(((File *)file)->name, newname);
        InsertEndL(&(parent->children_files), file);
        return;
    }
	// daca nu gasim un fisier/director cu numele oldname
    printf("File/Director not found\n");
}