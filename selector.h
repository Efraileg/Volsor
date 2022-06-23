/*
* --- AUTORES ---
* Óscar Baños Campos
* Eduardo Fraile González
* Grupos: L8
*/


#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <errno.h>
#include <vector>
#include <fstream>



#include "LectorEntrada.h"
#include "FrameBuilder.h"

#define ENQ 5
#define EOT 4
#define ACK 6
#define NACK 21
#define STX 2

#define default_protocolo 0x30;

using namespace std;



class selector{

    FrameBuilder frameB;
    LectorEntrada lector;
    
    unsigned char* MAC_esclavo;
    unsigned char* MAC_maestro;
    unsigned char tipo[2] = {0x00, 0x00};
    unsigned char grupo;
    unsigned char protocoloPropio;
    interface_t interfaz;
    int estado;


    /*
        Funcion de lectura de caracter especial.

        Lee por teclado si se ha presionado una tecla y determina si se ha presionado las teclas especiales ESC, F1, F2, F3, F4.

        Si se ha presionado otra tecla o directamente no se ha presionado devuelve 0.


        Return:
            0 -> Nada se ha presionado o ha sido presionada cualquier tecla diferente a las siguientes.
            1 -> F1
            2 -> F2
            3 -> F3
            4 -> F4
            -1 -> ESC
    */

    /*
        Metodo que muestra una MAC generica en formato hexadecimal.
    */
    void mostrarMAC(unsigned char* mac);
    
    /*
        Metodo que pasado un vector de unsigned char, calcula con una operación matemática un resultado de 1 byte para
        hallar errores.

    */
    unsigned char calcularBCE(unsigned char* datos, int Long);

    /*
    
        Metodo que establece la comunicacion entre esclavo y maestro, se ha condensado
        en un solo metodo pues había gran similitud entre ambos codigos.

    */
    void recibirConfirmacion();
    

    /*
        Interfaz maestra de la comunicacion
    */
    void maestro();


    /*
        Interfaz esclava de la comunicacion
    */
    void esclavo();


  /*
    Metodo que lee el fichero de envio y lo guarda en cadenas de String de maximo 254 caracteres.
    Posteriormente un metodo se encargara de convertir los string a *char para el envio por trama ethernet.
  */

    void ComunicacionProtocolo(unsigned char tipo, unsigned char *mac_destino);

   

    vector<string> cargarDatos(string nombreFichero);

    void volcarDatos(vector<string> datosGuardados, string nombreFichero);

    /*
        Método que extrae una interfaz de un vector de interfaces en función
        de un índice pedido por teclado al usuario, dicho índice debe ser menor
        al rango introducido como máximo (se sale del tamaño máximo)

        El método muestra por pantalla la dirección MAC de la interfaz seleccionada.
    */
    void seleccionarInterfaz(pcap_if *listaInterfaz, unsigned int max);

    public:
  
    /*
        Constructor por defecto de la clase selector.
        El objetivo es inicializar los elementos básicos para establecer una comunicación.
    */
    selector();


    /*
    Método que almacena y muestra en un vector las interfaces de un lista para poder ser empleadas posteriormente
    
    Se recorre la lista mientras se muestran los elementos y se contabiliza el total.
    Tras esto, se reserva un espacio en memoria para guardar los elementos previamente contabilizados

    Se llama al metodo selecionarInterfaz antes de acabar.
    */

    void mostrarInterfaces();


    /*
        Metodo intermedio que es empleado para abrir y cerrar el puerto de comunicacion tras seleccionar el estado
        de funcionamiento de la interfaz seleccionada.
    */
    void establecerComunicacion();

};
