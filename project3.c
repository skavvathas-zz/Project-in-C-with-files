#include<stdio.h>
#include<stdlib.h>
#include"objdb.h"

int main(int argc, char *argv[]){
	char command;
	char *basename, *fname, *objname;
	int back, base_close = 0;
	FILE *fptr_base;
	char array[255][255];
	
	make_null(array);

	basename = (char *)malloc(225);
	fname = (char *)malloc(225);
	objname = (char *)malloc(225);

	do{
		scanf(" %c", &command);
		switch(command){
			case 'o':
				scanf(" %s", basename);
				back = open(&fptr_base, basename, &base_close);
				print_open(back,basename);
				break;
			case 'i':
				scanf(" %s %s", fname, objname);
				back = import(&fptr_base, basename, fname, objname, &base_close);
				print_import(back,fname,objname);
				break;
			case 'f':
				scanf(" %s", objname);
				back = find(&fptr_base, basename, objname, array, &base_close);
				print_find(back,array);
				break;
			case 'e':
				scanf(" %s %s", objname, fname);
				back = export(&fptr_base, basename, objname, fname, &base_close);
				print_export(back, objname, fname);
				break; 
			case 'd':
				scanf(" %s", objname);
				back = delete(&fptr_base, basename, objname, &base_close);
				print_delete(back, objname);
				break;
			case 'c':
				back = close_(&fptr_base, &base_close);
				print_close(back);
				break;
			case 'q':
				quit(&fptr_base, &base_close);
				break;
		}
	}while(command != 'q');


	free(basename);
	free(fname);
	free(objname);

	return(0);
}
