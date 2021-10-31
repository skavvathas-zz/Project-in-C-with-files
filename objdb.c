#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"objdb.h"

void make_null(char array[255][255]){
	int i, j;

	for(i = 0; i < 255; i++){
		for(j = 0; j < 255; j++){
			array[i][j] = '\0';
		}
	}
}

int object_find(FILE **fptr_base, char *objname){
	char buffer[255][255]={"\0"}, c, help;
	int i = 0, j = 0;
	long int b;
	
	if(fseek(*fptr_base,9,SEEK_SET) != 0){
		fprintf(stderr,"\nfread fail in line 21");
		perror("\nError 202");
		if(ferror(*fptr_base) != 0){
			fprintf(stderr,"\nError from ferror\n");
		}
	}
	if(fread(&c,sizeof(char),1,*fptr_base) != 1){
		fprintf(stderr,"\nfread fail in line 28.OK\n");
		perror("\nError 209");
		if(ferror(*fptr_base) != 0){
			fprintf(stderr,"\nError from ferror\n");
		}
	}
	else{
		buffer[0][j++] = c;
	}
	if(feof(*fptr_base) != 0){//file reach end
		return(0);
	}
		
	if(ferror(*fptr_base) != 0){
		fprintf(stderr,"\nError from ferror\n");
		perror("\nError 43");
	}
	do{
		do{
			if(fread(&help,sizeof(char),1,*fptr_base) != 1){
				fprintf(stderr,"\nfread fail in line 48\n");
				perror("\nError 232");
				if(ferror(*fptr_base) != 0){
					fprintf(stderr,"\nError from ferror\n");
				}
			}
			if(help != '!'){
				buffer[i][j] = help;
				j++;
			}
		}while(help != '!');

		if(strcmp(buffer[i],objname) == 0){
			return(1);
		}
	
		if(fread(&b,sizeof(long int),1,*fptr_base) != 1){
			fprintf(stderr,"\nfread fail in line 65\n");
			perror("\nError 246");
			if(ferror(*fptr_base) != 0){
			fprintf(stderr,"\nError from ferror\n");
			}
		}

		if(fseek(*fptr_base,b,SEEK_CUR) != 0){
			fprintf(stderr,"\nfread fail in line 73\n");
			perror("\nError 253 ");
			if(ferror(*fptr_base) != 0){
				fprintf(stderr,"\nError from ferror\n");
			}
		}
		if(fread(&help,sizeof(char),1,*fptr_base) != 1){
			if(feof(*fptr_base) != 0){//file reach end
				return(0);
			}
			else{
				i++;
				j = 0;
				buffer[i][j++] = help;
				break;
			}
			if(ferror(*fptr_base) != 0){
				fprintf(stderr,"\nError from ferror\n");
			}
			fprintf(stderr,"\nfread fail in line 92\n");
			perror("\nError 93");
		}
		i++;
		j = 0;
		buffer[i][j++] = help;
		
	}while(feof(*fptr_base) == 0);
	
	return(0);
}

void print_open(int back, char *basename){

	if(back == -1){
		printf("\nError opening %s.\n", basename);
	}
	if(back == 2){
		printf("\nInvalid db file %s.\n", basename);
	}
}

void print_import(int back, char *fname, char *objname){

	if(back == 1){
		printf("\nFile %s not found.\n", fname);
	}
	if(back == 2){
		printf("\nNo open db file.\n");
	}
	if(back == 3){
		printf("\nObject %s already in db.\n", objname);
	}
}

void print_find(int back, char array[255][255]){
	int i, j = 0;

	if(back == -1){
		printf("\nNo open db file.\n");
	}
	if(back == 1){
		printf("\n##");
		for(i = 0; (array[i][j] != '\0') && (i < 255) ; i++){
			printf("\n%s", array[i]);
		}
		printf("\n");
		make_null(array);		
	}
	if(back == 2){
		printf("\n##");
		for(i = 0; (array[i][j] != '\0') && (i < 255); i++){
			printf("\n%s\n", array[i]);
		}
		printf("\n");
		make_null(array);
	}
}

void print_export(int back, char *objname, char *fname){

	if(back == -1){
		printf("\nNo open db file.\n");
	}
	if(back == 1){
		printf("\nObject %s not in db.\n", objname);
	}
	if(back == 2){
		printf("\nCannot open file %s.\n", fname);
	}
}

void print_delete(int back, char *objname){

	if(back == 1){
		printf("\nObject %s not in db.\n", objname);
	}
	if(back == -1){
		printf("\nNo open db file.\n");
	}
}

void print_close(int back){

	if(back == 2){
		printf("\nNo open db file.\n");
	}
}

int open(FILE **fptr_base, char *basename, int *base_close){
	char buffer[] = {"!magicN\n"};
	char array[8] = {"\0"};

	if(*base_close == -1){
		fclose(*fptr_base);
	}

	*fptr_base = fopen(basename,"rb+");
	if(*fptr_base == NULL){//the file of base doesn't exists
		
		close_(fptr_base,base_close);
		*fptr_base = fopen(basename, "wb");
		if(*fptr_base == NULL){
			return(-1);
		}
		if(fwrite(buffer,sizeof(buffer),1,*fptr_base) != 1){//write the magic number in the start of the file
			printf("\nfwrite fail in line 25\n"); 
			perror("\nError  ");
			if(ferror(*fptr_base) != 0){
				fprintf(stderr,"\nError from ferror\n");
			}     
		}
		*base_close = -1;//base_close=-1 -> db file is open
		return(0);
	}
	else{//the file of base exists
		
		if(*fptr_base == NULL){
			return(-1);
		}
		if(fread(array,sizeof(array),1,*fptr_base) != 1){
			fprintf(stderr,"\nfseek fail in line 37\n");
			perror("\nError ");
			if(ferror(*fptr_base) != 0){
				fprintf(stderr,"\nError from ferror\n");
			}
		}
		if(strcmp(array,buffer) != 0){
			return(2);//Invalid db file <dbname>
		}
		*base_close = -1;//base_close=-1 -> db file is open
		return(0);
	}

	return(0);
}

int import(FILE **fptr_base, char *basename, char *fname, char *objname, int *base_close){
	FILE *fptr2;
	char buffer[512] = {"\0"}, array[255][255] ;
	long int bytes;
	int i, back;

	if(*base_close == -1){
		fptr2 = fopen(fname,"rb");
		if(fptr2 == NULL){
			return(1);
		}
		else{
			make_null(array);
			find(fptr_base,basename,objname,array,base_close);
			for(i = 0; (array[i][0] != '\0') && (i < 255); i++){
				if(strcmp(array[i],objname) == 0){
					return(3);
				}
			}
			close_(fptr_base,base_close);
			*fptr_base = fopen(basename,"rb+");
			*base_close = -1;
			if(fseek(fptr2,0L,SEEK_END) != 0){
				fprintf(stderr,"\nfseek fail in line 255\n");
				perror("\nError ");
				if(ferror(*fptr_base) != 0){
					fprintf(stderr,"\nError from ferror\n");
				}
			}
			bytes = ftell(fptr2); //return the bytes of file
			rewind(fptr2);//return the pointer in the start of the file
			if(fseek(*fptr_base,0,SEEK_END) != 0){
				fprintf(stderr,"\nfseek fail in line 265\n");
				perror("\nError ");
				if(ferror(*fptr_base) != 0){
					fprintf(stderr,"\nError from ferror\n");
				}
			}
			if(fwrite(objname,1,strlen(objname),*fptr_base) != strlen(objname)){
				fprintf(stderr,"\nfwrite fail in line 272\n");
				perror("\nError ");
				if(ferror(*fptr_base) != 0){
					fprintf(stderr,"\nError from ferror\n");
				}
			}
			if(fwrite("!",sizeof(char),1,*fptr_base) != 1){
				fprintf(stderr,"\nfwrite fail in line 279\n");
				perror("\nError ");
				if(ferror(*fptr_base) != 0){
					fprintf(stderr,"\nError from ferror\n");
				}
			}
			if(fwrite(&bytes,sizeof(long int),1,*fptr_base) != 1){
				fprintf(stderr,"\nfwrite fail in line 286\n");
				perror("\nError ");
				if(ferror(*fptr_base) != 0){
					fprintf(stderr,"\nError from ferror\n");
				}
			}
				
			if(bytes > 512){//if the file is bigger than 512 bytes
				for(i = 0; bytes > 512; i++){
					back = fread(buffer,1,512,fptr2);
					if(feof(*fptr_base) != 0){
						break;
					}
					if(back != 512){
						fprintf(stderr,"\nfread fail in line 297\n");
						perror("\nError ");
						if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
						}
					}
					bytes = bytes - back;
					if(fwrite(buffer,1,512,*fptr_base) != 512){
						fprintf(stderr,"\nfwrite fail in line 305\n");
						perror("\nError ");
						if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
						}
					}
					
				}
			}
			if(bytes == 512){//if the file is 512 bytes
				if(fread(buffer,1,512,fptr2) != 512){
					fprintf(stderr,"\nfread fail in line 316\n");
					perror("\nError ");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}
				if(fwrite(buffer,1,bytes,*fptr_base) != bytes){
					fprintf(stderr,"\nfwrite fail in line 323\n");
					perror("\nError ");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}

			}
			if(bytes < 512){//if the file is smaller than 512 bytes
				if(fread(buffer,1,bytes,fptr2) != bytes){
					fprintf(stderr,"\nfread fail in line 333\n");
					perror("\nError ");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}
				if(fwrite(buffer,1,bytes,*fptr_base) != bytes){
					fprintf(stderr,"\nfwrite fail in line 340\n");
					perror("\nError ");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}
			}
			
			fclose(fptr2);
			return(0);
		}
	}
	else{
		return(2);//No open db file
	}
	return(0);
}
int find(FILE **fptr_base, char *basename, char *objname, char array[255][255], int *base_close){
	int i = 0, j = 0, find = 0, l = 0;
	long int b;
	char buffer [225][255] = {"\0"}, help, *ptr, c;

	if(*base_close == 1){
		return(-1);
	}
	else{
		close_(fptr_base, base_close);
		*fptr_base = fopen(basename,"rb");
		*base_close = -1;
		if(fseek(*fptr_base,0,SEEK_SET) != 0){
			fprintf(stderr,"\nfread fail in line 370");
			perror("\nError 371");
			if(ferror(*fptr_base) != 0){
				fprintf(stderr,"\nError from ferror\n");
			}
		}
		if(fseek(*fptr_base,9,SEEK_SET) != 0){
			fprintf(stderr,"\nfread fail in line 377");
			perror("\nError 378");
			if(ferror(*fptr_base) != 0){
				fprintf(stderr,"\nError from ferror\n");
			}
		}
		if(fread(&c,sizeof(char),1,*fptr_base) != 1){
			fprintf(stderr,"\nfread fail in line 384.OK\n");
			perror("\nError 385");
			if(ferror(*fptr_base) != 0){
				fprintf(stderr,"\nError from ferror\n");
			}
		}
		if(feof(*fptr_base) != 0){//file reach end
			return(1);
		}
		
		if(ferror(*fptr_base) != 0){
			fprintf(stderr,"\nError from ferror\n");
			perror("\nError 396");
		}
		
		if(!strcmp(objname,"*")){
			buffer[0][0] = c;
			i++;
			do{
				do{
					if(fread(&help,sizeof(char),1,*fptr_base) != 1){
						if(feof(*fptr_base) != 0){//file reach end
							return(1);
						}
						fprintf(stderr,"\nfread fail in line 406\n");
						perror("\nError 407");
						if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
						}
					}
					if(help != '!'){
						buffer[l][i++] = help;
					}
				}while(help != '!');
				if(j < 255){
					strcpy(array[j],buffer[l]);
					j++;
					l++;
				}
				if(fread(&b,sizeof(long int),1,*fptr_base) != 1){
					fprintf(stderr,"\nfread fail in line 422\n");
					perror("\nError 423");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}
				if(fseek(*fptr_base,b,SEEK_CUR) != 0){
					fprintf(stderr,"\nfread fail in line 429\n");
					perror("\nError 430");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}

				if(fread(&help,sizeof(char),1,*fptr_base) != 1){
					if(feof(*fptr_base) != 0){//file reach end
						return(1);
					}
					else{
						i = 0;
						buffer[l][i++] = help;
						break;
					}
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
					fprintf(stderr,"\nfread fail in line 448\n");
					perror("\nError 449");
				}
				i = 0;
				buffer[l][i++] = help;
				

			}while(feof(*fptr_base) == 0);

			return(1);
		}
		else{
			buffer[0][0] = c;
			i++;
			do{
				do{
					if(fread(&help,sizeof(char),1,*fptr_base) != 1){
						if(feof(*fptr_base) != 0){//file reach end
							return(1);
						}
						fprintf(stderr,"\nfread fail in line 465\n");
						perror("\nError 466");
						if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
						}
					}
					if(help != '!'){
						buffer[l][i++] = help;
					}
				}while(help != '!');

				ptr = strstr(buffer[l],objname);

				if(ptr != NULL){	
					if(j < 255){
						strcpy(array[j],buffer[l]);
						j++;
						l++;
					}
					find = 1;//the objname found
				}
				else{
					l++;
				}

				if(fread(&b,sizeof(long int),1,*fptr_base) != 1){
					fprintf(stderr,"\nfread fail in line 495\n");	
					perror("\nError 326");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}
				if(fseek(*fptr_base,b,SEEK_CUR) != 0){
					fprintf(stderr,"\nfread fail in line 503\n");
					perror("\nError 504");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}
				if(fread(&help,sizeof(char),1,*fptr_base) != 1){
					if(feof(*fptr_base) != 0){//file reach end
						return(1);
					}
					else{	
						i = 0;
						buffer[l][i++] = help;
						break;
					}
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
					fprintf(stderr,"\nfread fail in line 520\n");
					perror("\nError 521");
				}

				i = 0;
				buffer[l][i++] = help;
			
			}while(feof(*fptr_base) == 0);

			if(find == 0){//objname doesn't exists in the file which is open
				return(0);
			}
			else{
				return(2);	
			}
		}
	}	

	return(0);
}				

int export(FILE **fptr_base,char *basename, char *objname, char *fname, int *base_close){
	char array[255][255], buffer[512]={"\0"};
	int back, bytes, i, find_ = 0;
	FILE *fptr2;

	if(*base_close == 1){
		return(-1);//No open db file
	}
	else{
		
		make_null(array);
		find(fptr_base,basename,objname,array,base_close);
		for(i = 0; (array[i][0] != '\0') && (i < 255); i++){
			if(strcmp(array[i],objname) == 0){
				find_ = 1;
			}
		}
		if(find_ == 0){
			return(1);//Object not in db file
		}
		else{
			close_(fptr_base,base_close);
			*fptr_base = fopen(basename,"rb+");
			*base_close = -1;

			if(object_find(fptr_base,objname) == 1){
				if(fread(&bytes,sizeof(long int),1,*fptr_base) != 1){
					fprintf(stderr,"\nfread fail in line 569\n");
					perror("\nError 570");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}
				fptr2 = fopen(fname,"rb");
				if(fptr2 == NULL){
					fptr2 = fopen(fname,"wb");
					if(fptr2 == NULL){
						return(2);//Cannot open fname file
					}
				}
				
				if(bytes > 512){
					for(i = 0; bytes > 512; i++){
						back = fread(buffer,1,512,*fptr_base);
						if(back != 512){ 
							fprintf(stderr,"\nfread fail in line 587\n");
							perror("\nError 588 ");
							if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
							}
						}
						bytes = bytes - back;
						if(fwrite(buffer,1,512,fptr2) != 512){
							fprintf(stderr,"\nfwrite fail in line 595\n");//💲️
							perror("\nError 596 ");
							if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
							}
						}
					}
				}
				if(bytes == 512){
					if(fread(buffer,1,512,*fptr_base) != 512){
						fprintf(stderr,"\nfread fail in line 605\n");
						perror("\nError 606");
						if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
						}
					}
					
					if(fwrite(buffer,1,512,fptr2) != 512){
						fprintf(stderr,"\nfwrite fail in line 613\n");
						perror("\nError 614");
						if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
						}
					}
				}
				if(bytes < 512){
					if(fread(buffer,1,bytes,*fptr_base) != bytes){
						fprintf(stderr,"\nfread fail in line 622\n");
						perror("\nError 623 ");
						if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
						}
				
					}
					if(fwrite(buffer,1,bytes,fptr2) != bytes){
						fprintf(stderr,"\nfwrite fail in line 630\n");
						perror("\nError 631");
						if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
						}
					}
				}
					
				fclose(fptr2);
			}
		}
	}
	
	return(0);
}

int delete(FILE **fptr_base, char *basename, char *objname, int *base_close){
	char array[255][255], *buffer;
	//FILE *fptr;
	int i, b, len, find_ = 0;
	long int bytes_old, bytes1, bytes_el;//bytes_el is the bytes which must copy to buffer

	if(*base_close == 1){
		return(-1);//No open db file
	}
	else{
		close_(fptr_base,base_close);
		*fptr_base = fopen(basename,"rb+");
		*base_close = -1;
		make_null(array);
		find(fptr_base,basename,objname,array,base_close);
		for(i = 0; (array[i][0] != '\0') && (i < 255); i++){
			if(strcmp(array[i],objname) == 0){
				find_ = 1;
			}
		}
		if(find_ == 0){
			return(1);//Object not in db file
		}
		else{
			fseek(*fptr_base,0,SEEK_END);//*fptr_base points to end
			bytes_old = ftell(*fptr_base);//how many bytes is the file
			rewind(*fptr_base);//*fptr_base points to start
			if(object_find(fptr_base,objname) == 1){
				len = strlen(objname);
				fseek(*fptr_base,-len-1,SEEK_CUR);
				bytes1 = ftell(*fptr_base);//count how many bytes is from start of file to where objname is
				fseek(*fptr_base,len+1,SEEK_CUR);
			
				if(fread(&b,sizeof(long int),1,*fptr_base) != 1){
					fprintf(stderr,"\nfread fail in line 679\n");
					perror("\nError 680");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}
				}
				if(fseek(*fptr_base,b,SEEK_CUR) != 0){
					fprintf(stderr,"\nfseek fail in line 687\n");
					perror("\nError 688");
					if(ferror(*fptr_base) != 0){
						fprintf(stderr,"\nError from ferror\n");
					}	
				}
				bytes_el = bytes_old - bytes1 - b - len - 9;
				
				buffer = (char *)calloc(bytes_el,sizeof(char));
				if(bytes_el <= 0){
					truncate(basename,bytes1);//truncate the file to bytes1 bytes
					free(buffer);
				}
				else{
					if(fread(buffer,sizeof(char),bytes_el,*fptr_base) != bytes_el){//copy to buffer the file after the object with name objname
						fprintf(stderr,"\nfread fail in line 689\n");
						perror("\nError 690");
						if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
						}
					}
					fclose(*fptr_base);

					truncate(basename,bytes1);//truncate the file to bytes1 bytes
				
					*fptr_base = fopen(basename,"rb+");
					if(*fptr_base == NULL){
						return(2);
					}
					if(fseek(*fptr_base,0,SEEK_END) != 0){
						fprintf(stderr,"\nfseek fail in line 704\n");
						perror("\nError 705 ");
						if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
						}	
					}
					if(fwrite(buffer,sizeof(char),bytes_el,*fptr_base) != bytes_el){
						fprintf(stderr,"\nfwrite fail in line 711\n");
						perror("\nError 712");
						if(ferror(*fptr_base) != 0){
							fprintf(stderr,"\nError from ferror\n");
						}	
					}

					free(buffer);
				}
			}
		}
	}
	
	return(0);
}

int close_(FILE **fptr_base, int *base_close){

	if(*base_close == -1){
		fclose(*fptr_base);
		*base_close = 1;
	}
	else{
		return(2);//No open db file
	}
	return(0);
}

void quit(FILE **fptr_base, int *base_close){
	
	if(*base_close == 1){
		close_(fptr_base, base_close);
	}
}

