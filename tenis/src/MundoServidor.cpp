// Mundo.cpp: implementation of the MundoServidor class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "MundoServidor.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include "glut.h"
#define MAX 50

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void handler(int);
MundoServidor::MundoServidor()
{
	Init();
}

MundoServidor::~MundoServidor()
{
	close(t);
	unlink(tuberia);	
	disparos1.DestruirContenido();
	disparos2.DestruirContenido();
	acabar=1;
	usleep(25000);
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(thConex, NULL);

	for(int i=0;i<conexiones.size();i++){
		conexiones[i].Close();
	}
	conex.Close();
	disparos1.DestruirContenido();
	disparos2.DestruirContenido();
/////////eliminamos las partes del bot
/////////servidor solo se comuica con logger
	/*if (munmap(datos2, bstat.st_size) == -1)
   	{
      	  close(fd);
       	  perror("Error un-mmapping the file");
    	}
	close(fd);*/
	
}

void MundoServidor::FinDeJuego()
{
	//usleep(250000);		
	disparos1.DestruirContenido();
	disparos2.DestruirContenido();
	esfera.velocidad.x=0;
	esfera.velocidad.y=0;
	jugador1.velocidad.y=0;
	OnDraw();
	
	
	
}

void MundoServidor::InitGL()
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
void MundoServidor::OnDraw()
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

void MundoServidor::OnTimer(int value)
{	
////////Comunicacion sockets////////////////////////////
	sprintf(msj,"%f %f %f %f %f %f %f %f %f %f %d %d", esfera.centro.x,esfera.centro.y, jugador1.x1,jugador1.y1,jugador1.x2,jugador1.y2,jugador2.x1,jugador2.y1,jugador2.x2,jugador2.y2, puntos1, puntos2);
	
	//for(int i=0;i<conexiones.size();i++)
	/*if (conexiones.size()>0){	
		if((conexiones[0].Send(msj,200))==-1){
			perror("Error al enviar mensaje en com\n");
		}
	}*/
//desconexiones sockets
	for (int i=conexiones.size()-1; i>=0; i--) {
  	       if (conexiones[i].Send(msj,200) <= 0) {
        	    conexiones.erase(conexiones.begin()+i); 
		    if (i < 2) {// Hay menos de dos clientes conectados
			puntos1=0;  // Se resetean los puntos a cero
			puntos2=0;
             	    }	   
               } 
         }
///////////////////////////////////////////////////////////////////////
	
	jugador1.Mueve(0.025f);
	jugador2.Mueve(0.025f);
	esfera.Mueve(0.025f);
	disparos1.Mueve(0.025f);
	disparos2.Mueve(0.025f);
	int i;

	//Interaccion paredes con esferas	
	for(i=0;i<paredes.size();i++)
	{
		//paredes[i].Rebota(esfera);
		paredes[i].Rebota(jugador1);
		paredes[i].Rebota(jugador2);
		paredes[i].Rebota(esfera);	
	}
	
	//Interaccion jugadores y puntuacion con esferas
	
	jugador1.Rebota(esfera);
	jugador2.Rebota(esfera);
	if(fondo_izq.Rebota(esfera))
	{
		esfera.centro.x=0;
		esfera.centro.y=rand()/(float)RAND_MAX;
		esfera.velocidad.x=2+2*rand()/(float)RAND_MAX;
		esfera.velocidad.y=2+2*rand()/(float)RAND_MAX;
		puntos1++;
		sprintf (infoJugador1, "Jugador 1 marca 1 punto,lleva un total de %d puntos\n", puntos1); 
		if((t = open(tuberia, O_WRONLY))==-1){
			perror("Error al abrir tuberia en tenis");	
		}				
		if(write(t,infoJugador1, sizeof(infoJugador1))<0)
		{
			perror("Error al escribir en tuberia en tenis\n");
		}
		
	}
	if(fondo_dcho.Rebota(esfera))
	{
		esfera.centro.x=0;
		esfera.centro.y=rand()/(float)RAND_MAX;
		esfera.velocidad.x=-2-2*rand()/(float)RAND_MAX;
		esfera.velocidad.y=-2-2*rand()/(float)RAND_MAX;
		puntos2++;
		sprintf (infoJugador2, "Jugador 2 marca 1 punto,lleva un total de %d puntos\n", puntos2); 
		if((t = open(tuberia, O_WRONLY))==-1){
			perror("Error al abrir tuberia en tenis");	
		}				
		if(write(t,infoJugador2, sizeof(infoJugador2))<0)
		{
			perror("Error al escribir en tuberia en tenis\n");
		}
	}
		
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
		
	/*for(int i=0;i<numeroE;i++){
		if (numeroE<4){	
			if(numeroE%2==0) esferas.sentido=false;
			else esfera.sentido=true;		
			esfera.ModificarTamanio();
			//Se comentan las dos siguientes lineas para implementacion del bot			
			//esferas.max=numeroE+1;			
			//esferas.agregar(new Esfera());		
		}	
	}*/
	/*
	//actualizacion de los datos para el bot
	datos1.esfera=esfera;
	datos1.raqueta1=jugador1;
	datos2->esfera=esfera;
	datos2->raqueta1=jugador1;	
	if (datos2->accion==1) OnKeyboardDown('w',1,1);
	if(datos2->accion==-1) OnKeyboardDown('s',1,1);
	*/


	//Fin de juego
	/*if (puntos1==5 || puntos2==5){
		 sprintf(msj,"%f %f %f %f %f %f %f %f %f %f %d %d", esfera.centro.x,esfera.centro.y, jugador1.x1,jugador1.y1,jugador1.x2,jugador1.y2,jugador2.x1,jugador2.y1,jugador2.x2,jugador2.y2, puntos1, puntos2);
		if((com.Send(msj,sizeof(msj)))==-1){
		perror("Error al enviar mensaje en com\n");
		}
		 finDeJuego= true;
	}*/

}

void MundoServidor::OnKeyboardDown(unsigned char key, int x, int y)
{
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

void* hilo_comandos1(void* d)
{
      MundoServidor* p=(MundoServidor*) d;
      p->RecibeComandosJugador1();
}
void* hilo_comandos2(void* d)
{
      MundoServidor* p=(MundoServidor*) d;
      p->RecibeComandosJugador2();
}

void* hilo_conex(void* f)
{
      MundoServidor* p=(MundoServidor*) f;
      p->GestionaConexiones();
}
/*void* espectadores(void* d)
{
      MundoServidor* p=(MundoServidor*) d;
      p->RecibeComandosJugador1();
}*/
void MundoServidor::RecibeComandosJugador1()
{
     while (acabar==0) {
            usleep(25000);
	    if(conexiones.size()>0){
		    if(conexiones[0].Receive(jug1,200)==-1 ){
			perror("Error al recibir mensaje en thread jug 1\n Jugador 1 se fue\n");
			continue;
		    }
		    unsigned char key;
		    sscanf(jug1,"%c",&key);
		    if(key=='s')jugador1.velocidad.y=-4;
		    if(key=='w')jugador1.velocidad.y=4;
		    //if(key=='t')kill(getpid(),SIGUSR1);
	     }
    }
}
void MundoServidor::RecibeComandosJugador2()
{
     while (acabar==0) {
            usleep(25000);
	    if(conexiones.size()>1){
		    if(conexiones[1].Receive(jug2,200)==-1){
			perror("Error al recibir mensaje en thread jug 2\n jugador2 se fue\n");
			continue;
		    }
		    unsigned char key;
		    sscanf(jug2,"%c",&key);
		    if(key=='l')jugador2.velocidad.y=-4;
		    if(key=='o')jugador2.velocidad.y=4;
		    //if(key=='t')kill(getpid(),SIGUSR1);
	     }
     }
}

void MundoServidor::GestionaConexiones(){
	Socket com;
	int size; //numero de clientes conectados	 
	while (acabar==0) {
            usleep(10);
            //conex acepta al cliente y devuelve el socket de comunicacion
	    conexiones.push_back(conex.Accept());	
	    size = conexiones.size();
	    if(size==1) {
		printf("NUEVO CLIENTE CONECTADO, Jugador1\n");
	    }
	    else {
		if(size==2){
			 printf("NUEVO CLIENTE CONECTADO, Jugador2\n");
	    	}
	    
	   	 else printf("NUEVO CLIENTE CONECTADO,Espectador\n");
	    }
	    if(conexiones.size()>2){
		for (int i=conexiones.size()-1; i>=2; i--) {
		    if(conexiones[i].Receive( msj_Espec,200)==-1){
			perror("Error al recibir mensaje en thread Espectadores\n Un espectador se fue\n");
			continue;
		    }
		}
	    }
	    
	}
      
}

void MundoServidor::Init()
{
	acabar=0;
	
////////Tuberias mkfifo///////////////////////////////////////
/////////////////////////////////////////////////////////////
	
	tuberia="/tmp/logger";	
	
	disparos1.max=10;
	disparos2.max=10;
	esfera.sentido=false;
	

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

///////sockets///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////	
	conexiones.reserve(10); //reservamos memoria para 10 clientes	
	conex.InitServer((char*)"127.0.0.1", PORT);//enlaza conex con una IP y un puerto
	
	
	
//////////Creación del thread/////////
	pthread_create(&th1, NULL, hilo_comandos1,this);	
	pthread_create(&th2, NULL, hilo_comandos2,this);
	//pthread_create(&th3, NULL, espectadores,this);	
	pthread_create(&thConex,NULL,hilo_conex,this);

///////señales!!!////////////////////////////////
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);	
	act.sa_handler = handler;
	act.sa_flags = 0;
	
	if(sigaction(SIGINT, &act, NULL)==-1){
		perror("Error: SIGINT"); 
	}
	if (sigaction(SIGTERM, &act, NULL) == -1) {
        	perror("Error: SIGTERM"); 
   	 }
	if (sigaction(SIGPIPE, &act, NULL) == -1) {
        	perror("Error: SIGPIPE"); 
   	 }
	if (sigaction(SIGUSR1, &act, NULL) == -1) {
        	perror("Error: SIGUSR1"); 
   	 }

////////este es el servidor!!
	/*///////Proyeccion en    memoria///////////////////////////////////
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
	printf("tamanio fichero %ld ",bstat.st_size);
	//Proyeccion del fichero
	if((org=mmap((caddr_t) 0,bstat.st_size, PROT_READ |  PROT_WRITE ,MAP_SHARED, fd, 0))==MAP_FAILED){ 
    		perror("Error en la proyeccion del fichero"); close(fd);
    	}
	else printf("mem proyectada\n");
	close(fd);
	datos2=static_cast<DatosMemCompartida*>(org);
*/
	

}
void handler(int sig)
{
	switch(sig){
		case SIGPIPE:
			printf("He recibido señal SIGPIPE\n");
			//exit(1);
		case SIGTERM:
			printf("Señal SIGTERM recibida, fin de ejecucion\n");
			exit(1);
		case SIGINT:
			printf("Programa terminado por señal SIGINT\n");
			exit(1);
		case SIGUSR1:
			printf("He recibido señal SIGUSR1\n");
			exit(0);

	}
}
