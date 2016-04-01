// Esfera.cpp: implementation of the Esfera class.
//
//////////////////////////////////////////////////////////////////////

#include "Esfera.h"
#include "glut.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Esfera::Esfera()
{
	radio=0.5f;
	velocidad.x=3;
	velocidad.y=3;
}

Esfera::Esfera(float a1,float b1,float a2,float b2,float v)
{
	centro.x=(a1+a2)/2;
	centro.y=(b1+b2)/2;
	radio=0.1;
	velocidad.x=v;		
}

Esfera::~Esfera()
{

}



void Esfera::Dibuja()
{
	glColor3ub(255,255,0);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(centro.x,centro.y,0);
	glutSolidSphere(radio,15,15);
	glPopMatrix();
}

void Esfera::Mueve(float t)
{
	centro=centro+velocidad*t*2;
}

void Esfera::ModificarTamanio()
{
	if (sentido==true){
			if(radio>0.3)	{
				radio=(radio-0.3);
			}
		
	}
	else {
			if(radio<0.7){
				radio=(radio+0.3);
			}
			
	}	
}


