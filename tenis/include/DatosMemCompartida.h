#ifndef INCLUDE_DATOSMEMCOMPARTIDA_H_
#define INCLUDE_DATOSMEMCOMPARTIDA_H_
#pragma once 

#include "Esfera.h"
#include "Raqueta.h"
#include "Plano.h"

class DatosMemCompartida
{       
public:         
      Esfera esfera;
      Raqueta raqueta1;
      int accion; //1 arriba, 0 nada, -1 abajo
};

#endif
