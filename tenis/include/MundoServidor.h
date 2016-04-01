// Mundo.h: interface for the CMundo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_)
#define AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_

#include <vector>
//#include "Plano.h"
#include "glut.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Esfera.h"
#include "Raqueta.h"
#include "ListaDisparos.h"
#include "DatosMemCompartida.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <vector>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include "Socket.h"

#define PORT 3550 



class MundoServidor  
{
public:
	void Init();
	MundoServidor();
	virtual ~MundoServidor();	
	
	void InitGL();	
	void OnKeyboardDown(unsigned char key, int x, int y);
	void OnTimer(int value);
	void OnDraw();
	void FinDeJuego();
	void RecibeComandosJugador1();	
	void RecibeComandosJugador2();	
	void Espectadores();	
	//void handler(int);

	Esfera esfera;//contenedor de esferas para las pelotas de juego
	//Esfera esfera;	
	std::vector<Plano> paredes;
	Plano fondo_izq;
	Plano fondo_dcho;
	Raqueta jugador1;
	Raqueta jugador2;
	ListaDisparos disparos1;
	ListaDisparos disparos2;
	DatosMemCompartida datos1;
	DatosMemCompartida *datos2;
   	void GestionaConexiones();
	
	struct stat bstat;
	struct sigaction act;
	int puntos1;
	int puntos2;
	int numeroE;
	char infoJugador1[200];
	char infoJugador2[200];
	const char *tuberia;
	int t;//identificador mkfifo
	bool finDeJuego;
	Socket conex;//socket para conexion con cliente 
	Socket com;//socket para conexion con cliente
	int fd; //identificador socket conex
	int fd2;//id socket com
	char jug1[200];
	char jug2[200]; 
	char msj[200];//se envian datos a cliente por socket
	char msj_Espec[200];//para manejo de los espectadores
	pthread_t th1;//comandos jugador 1
	pthread_t th2;//comandos jugador 2
	pthread_t th3;//hilo paramanejar espectadores
	std::vector<Socket> conexiones;
	pthread_t thConex;
	int acabar; //para la destruccion de los hilos

};

#endif // !defined(AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_)
