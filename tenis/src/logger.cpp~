
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define    MAX_MSJ     200
int main()
{
	int t;	
	const char *tuberia = "/tmp/logger";
	char msj[MAX_MSJ];
	if (mkfifo(tuberia,0777)<0){
		perror("Error al crear la tuberia\n");
		return -1;
	}
	else printf("tuberia creada\n");
	//abre la tuberia	
	if((t = open(tuberia,O_RDONLY))==-1)
	{
		perror("Error al abrir la tuberia\n");
		return -1;
	}
	else printf("Tuberia abierta\n");
	char* buffer;
	//lee el mensaje desde la tuberia y lo almacena
	while(1){		
		if(read(t,msj,sizeof(msj))<0){
			perror("Error al leer ensaje\n");
			return -1;
		}
		else{
			if(strcmp(buffer,msj)==0) break;	      
			printf("%s",msj);
			strcpy(buffer,msj);
		}
		
	}
       //cerramos la tuberia 
	close(t);
	//borramos tuberia
	unlink(tuberia);
	
	return 0;	
	
}



