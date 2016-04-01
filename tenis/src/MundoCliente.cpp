// Mundo.cpp: implementation of the MundoCliente class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "MundoCliente.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "glut.h"
#define MAX 50
#define PORT 3550 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MundoCliente::MundoCliente()
{
	Init();
}

MundoCliente::~MundoCliente()
{
	disparos1.DestruirContenido();
	disparos2.DestruirContenido();	
	close(fd);	
	com.Close();
	if (munmap(datos2, bstat.st_size) == -1)
   	{
      	  close(fd);
       	  perror("Error un-mmapping the file");
    	}
}

void MundoCliente::FinDeJuego()
{	
	disparos1.DestruirContenido();
	disparos2.DestruirContenido();
	esfera.velocidad.x=0;
	esfera.velocidad.y=0;
	jugador1.velocidad.y=0;
	OnDraw();
	
}

void MundoCliente::InitGL()
{
	//Habilitamos las luces, la renderizacion y el color de los materiales
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 40.0, 800/600.0f, 0.1, 150);	
}

void print(char *mensaje, int x, int y, float r, float g, float b)
{
	glDisable (GL_LIGHTING);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT) );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glColor3f(r,g,b);
	glRasterPos3f(x, glutGet(GLUT_WINDOW_HEIGHT)-18-y, 0);
	int len = strlen (mensaje );
	for (int i = 0; i < len; i++) 
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, mensaje[i] );
		
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
}
void MundoCliente::OnDraw()
{
	//Borrado de la pantalla	
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Para definir el punto de vista
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	
	gluLookAt(0.0, 0, 17,  // posicion del ojo
		0.0, 0.0, 0.0,      // hacia que punto mira  (0,0,0) 
		0.0, 1.0, 0.0);      // definimos hacia arriba (eje Y)    

	/////////////////
	///////////
	//		AQUI EMPIEZA MI DIBUJO
	char cad[100];
	sprintf(cad,"Jugador1: %d",puntos1);
	print(cad,10,0,1,1,1);
	sprintf(cad,"Jugador2: %d",puntos2);
	print(cad,650,0,1,1,1);
	if(finDeJuego && puntos1>puntos2) 
	{
		sprintf(cad,"Gana Jugador1");
		print(cad,300,0,1,1,1);
	}
	if(finDeJuego && puntos2>puntos1) 
	{
		sprintf(cad,"Gana Jugador2");
		print(cad,300,0,1,1,1);
	}	
	int i;
	for(i=0;i<paredes.size();i++)
		paredes[i].Dibuja();

	fondo_izq.Dibuja();
	fondo_dcho.Dibuja();
	jugador1.Dibuja();
	jugador2.Dibuja();
	disparos1.Dibuja();
	disparos2.Dibuja();
	esfera.Dibuja();
	/////////////////
	///////////
	//		AQUI TERMINA MI DIBUJO
	////////////////////////////

	//Al final, cambiar el buffer
	glutSwapBuffers();
}

void MundoCliente::OnTimer(int value)
{	
	/////////lectura tuberia SC
//////////////////////////////////////////////////////
	com.Receive(msj2, 200);
	sscanf(msj2,"%f %f %f %f %f %f %f %f %f %f %d %d", &esfera.centro.x,&esfera.centro.y, &jugador1.x1,&jugador1.y1,&jugador1.x2,&jugador1.y2, &jugador2.x1,&jugador2.y1,&jugador2.x2,&jugador2.y2, &puntos1, &puntos2);


	/*jugador1.Mueve(0.025f);
	jugador2.Mueve(0.025f);
	esfera.Mueve(0.025f);*/
	disparos1.Mueve(0.025f);
	disparos2.Mueve(0.025f);
	int i;

	
		
	for(i=0;i<disparos2.numero;i++){
		if(jugador1.Rebota(*disparos2.lista[i])){
			disparos2.eliminar(i);
			jugador1.y1=jugador1.y1+0.4;
			jugador1.y2=jugador1.y2-0.4;
			if(jugador1.y1>jugador1.y2){
				jugador1.y1=jugador1.y1-0.4;
				jugador1.y2=jugador1.y2+0.4;			
			}
			return;
		}
	}
	for(i=0;i<disparos1.numero;i++){
		if(jugador2.Rebota(*disparos1.lista[i])){
			disparos1.eliminar(i);
			jugador2.y1=jugador2.y1+0.4;
			jugador2.y2=jugador2.y2-0.4;
			if(jugador2.y1>jugador2.y2){
				jugador2.y1=jugador2.y1-0.4;
				jugador2.y2=jugador2.y2+0.4;			
			}
			return;
		}
	}
	
		
	if((puntos1+puntos2)==0){
		esfera.sentido=true;
	}
	if((puntos1+puntos2)>0){
		if(((puntos1+puntos2)%2)==0){
			esfera.sentido=false;
			esfera.ModificarTamanio();			
		}	
		else{
			esfera.sentido=true;
			esfera.ModificarTamanio();		
		}
	}
		
/////////actualizacion de los datos para el bot
////////////////////////////////////////////////
	datos1.esfera=esfera;
	datos1.raqueta1=jugador1;
	datos2->esfera=esfera;
	datos2->raqueta1=jugador1;	
	//if (datos2->accion==1) OnKeyboardDown('w',1,1);
	//if(datos2->accion==-1) OnKeyboardDown('s',1,1);
	
	//Fin de juego
	//if (puntos1==5 || puntos2==5) finDeJuego= true;

}

void MundoCliente::OnKeyboardDown(unsigned char key, int x, int y)
{
	//envio de teclas a servidor por socket
	sprintf(msj,"%c \n",key);
	com.Send(msj,200);
	if(key=='t') exit(0);
	switch(key)
	{
//	case 'a':jugador1.velocidad.x=-1;break;
//	case 'd':jugador1.velocidad.x=1;break;
//	case 's':jugador1.velocidad.y=-4;break;
//	case 'w':jugador1.velocidad.y=4;break;
//	case 'l':jugador2.velocidad.y=-4;break;
//	case 'o':jugador2.velocidad.y=4;break;
	case '1':disparos1.agregar(new Esfera(jugador1.x1,jugador1.y1,jugador1.x2,jugador1.y2,2));break;
	case '0':disparos2.agregar(new Esfera(jugador2.x1,jugador2.y1,jugador2.x2,jugador2.y2,-2));break;
	}
}

void MundoCliente::Init()
{
	
	//tuberia="/tmp/logger";		
	
	disparos1.max=10;
	disparos2.max=10;
	//esferas.max=1;
	numeroE=0;
	esfera.sentido=false;
//////////////////////////////////////////////////////
/////////Sockets//////////////////////////////////////
	//const char *IP = "127.0.0.1";	
	//printf("Ingrese nombre del cliente\n");
 	//scanf("%s",msj);
 	
	com.Connect((char*)"127.0.0.1", PORT);//enlaza com con una IP y un puerto
	//com.Send(msj,sizeof(msj));//envia nombre de cliente a servidor
	


///////////////////////////////////////	
	finDeJuego=false;
	Plano p;
//pared inferior
	p.x1=-7;p.y1=-5;
	p.x2=7;p.y2=-5;
	paredes.push_back(p);

//superior
	p.x1=-7;p.y1=5;
	p.x2=7;p.y2=5;
	paredes.push_back(p);

	fondo_izq.r=0;
	fondo_izq.x1=-7;fondo_izq.y1=-5;
	fondo_izq.x2=-7;fondo_izq.y2=5;

	fondo_dcho.r=0;
	fondo_dcho.x1=7;fondo_dcho.y1=-5;
	fondo_dcho.x2=7;fondo_dcho.y2=5;

	//a la izq
	jugador1.g=0;
	jugador1.x1=-6;jugador1.y1=-1;
	jugador1.x2=-6;jugador1.y2=1;

	//a la dcha
	jugador2.g=0;
	jugador2.x1=6;jugador2.y1=-1;
	jugador2.x2=6;jugador2.y2=1;

	///////Proyeccion en memoria///////////////////////////////////
/////////////////////////////////////////////////////////////
	datos1.esfera=esfera;
	datos1.raqueta1=jugador1;		
	void *org;
	if((fd=open("/tmp/bot.txt", O_CREAT|O_TRUNC|O_RDWR,0666))<0)
		perror("Error al abrir bot en bot.cpp");
	write(fd,&datos1, sizeof(datos1));	
	if (fstat(fd, &bstat)<0) {
    		perror("Error en fstat del fichero"); close(fd);
  	}
	//printf("tamanio fichero %ld ",bstat.st_size);
	//Proyeccion del fichero
	if((org=mmap((caddr_t) 0,bstat.st_size, PROT_READ |  PROT_WRITE ,MAP_SHARED, fd, 0))==MAP_FAILED){ 
    		perror("Error en la proyeccion del fichero"); close(fd);
    	}
	else printf("mem proyectada\n");
	close(fd);
	datos2=static_cast<DatosMemCompartida*>(org);
	



}
