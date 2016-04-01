/*
 * ListaDisparos.h
 *
 *  Created on: Oct 26, 2015
 *      Author: daaguirre
 */

#ifndef INCLUDE_LISTADISPAROS_H_
#define INCLUDE_LISTADISPAROS_H_
#include "Esfera.h"
#define MAX_DISPAROS 10


class ListaDisparos{
public:
	ListaDisparos();
	virtual ~ListaDisparos();
	bool agregar(Esfera *d);
	//void eliminar();
	void eliminar(int index);
	void Dibuja();
	void Mueve(float t);
	void DestruirContenido();
	//void ModificarTamanio();

	int max;
	//bool sentido; //decide si se reduce o aumenta tamañ0 de esfera	
	Esfera *lista[MAX_DISPAROS];
	int numero;
};




#endif /* INCLUDE_LISTADISPAROS_H_ */
