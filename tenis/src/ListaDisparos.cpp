/*
 * ListaDisparos.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: daaguirre
 */
#include "ListaDisparos.h"
#include "Plano.h"

ListaDisparos::ListaDisparos()
{
	numero=0;
	for (int i=0;i<max; i++)
		lista[i]=0;
}

ListaDisparos::~ListaDisparos(){}
bool ListaDisparos::agregar(Esfera *d)
{
	if(numero<max)
		lista[numero++]=d;
	else
		return false;
	return true;
}
void ListaDisparos::eliminar(int index){
	if((index<0)||(index>=numero))
		return;
	delete lista[index];
	numero--;
	for(int i=index;i<numero;i++)
		lista[i]=lista[i+1];
}

void ListaDisparos::Dibuja()
{
	for(int i=0;i<numero;i++)
		lista[i]->Dibuja();
}
void ListaDisparos::Mueve(float t)
{
	for(int i=0;i<numero;i++)
		lista[i]->Mueve(t);
}

void ListaDisparos::DestruirContenido()
{
	for(int i=0;i<numero;i++)
		delete lista[i];
	numero=0;
}

/*void ListaDisparos::ModificarTamanio()
{
	if (sentido==true){
		for(int i=0;i<numero;i++){
			if(lista[i]->radio>0.3)	{
				lista[i]->radio=(lista[i]->radio-0.3);
			}
		}
	}
	else {
		for(int i=0;i<numero;i++){
			if(lista[i]->radio<0.7){
				lista[i]->radio=(lista[i]->radio+0.3);
			}
		}	
	}	
}*/


