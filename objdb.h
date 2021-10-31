#ifndef _OBJDB_H
#define _OBJDB_H

void make_null(char array[255][255]);
int object_find(FILE **fptr_base, char *objname);
void print_open(int back, char *basename);
void print_import(int back, char *fname, char *objname);
void print_find(int back, char array[255][255]);
void print_export(int back, char *objname, char *fname);
void print_delete(int back, char *objname);
void print_close(int back);
int open(FILE **fptr_base, char *basename,int *base_close);
int import(FILE **fptr_base, char *basename, char *fname, char *objname,int *base_close);
int find(FILE **fptr_base, char *basename, char *objname,char array[255][255], int *base_close);
int export(FILE **fptr_base,char *basename, char *objname, char *fname, int *base_close);
int delete(FILE **fptr_base, char *basename, char *objname, int *base_close);
int close_(FILE **fptr_base, int *base_close);
void quit(FILE **fptr_base, int *base_close);

#endif
