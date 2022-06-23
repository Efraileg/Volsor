//============================================================================
// ----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES -----------
// ---------------------------- CURSO 2021/22 --------------------------------
// ----------------------------- SESION2.CPP ---------------------------------
//============================================================================

#include <stdio_ext.h>
#include "selector.h"
#include <iostream>

//using namespace std;

int main()
{

  //Creación de la clase selectora.
  
  selector S;
  

  //metodo inicial.
  //Permite observar las interfaces disponibles y seleccionar una de ellas.
  S.mostrarInterfaces();

  //Una vez seleccionada una interfaz, escoger el grupo y tipo de funcionamiento que seguirá.
  S.establecerComunicacion();
  std::cout<<std::endl<<std::endl;
  return 0;
}