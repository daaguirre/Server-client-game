// Esfera.h: interface for the Esfera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ESFERA_H__8D520BAF_8208_423B_BD91_29F6687FB9C3__INCLUDED_)
#define AFX_ESFERA_H__8D520BAF_8208_423B_BD91_29F6687FB9C3__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector2D.h"

class Esfera  
{
public:	
	Esfera();
	Esfera(float a1,float b1,float a2,float b2,float v);
	virtual ~Esfera();
		
	Vector2D centro;
	Vector2D velocidad;
	float radio;
	bool sentido;

	void Mueve(float t);
	void Dibuja();
	void ModificarTamanio();
};

#endif // !defined(AFX_ESFERA_H__8D520BAF_8208_423B_BD91_29F6687FB9C3__INCLUDED_)
