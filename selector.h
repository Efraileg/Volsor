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
#include "linkLayer.h"
#include <fstream>

#define ENQ 5
#define EOT 4
#define ACK 6
#define NACK 21
#define STX 2

#define default_protocolo 0x30;

using namespace std;



class selector{

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
        Construye un campo datos conformado por 3 bytes correspondientes a los bytes de control empleados
        en una trama de control de comunicación.
    */
    unsigned char* construirTramaControl(unsigned char  direccion, unsigned char  control, unsigned char  numero);

    /*
        Recibe como parametro los 3 bytes de una trama de control más los bytes necesarios para los datos propiamente dichos.
        Para ellos se necesita almacenar en un campo la longitud de los datos y al final de la trama el BCE o calculo de bytes
        para obtener posibles errores.
    */
    unsigned char* construirTramaDatos(unsigned char* tramaControl, int Long, unsigned char* datos, unsigned char BCE);
    
    /*
        Metodo que pasado un vector de unsigned char, calcula con una operación matemática un resultado de 1 byte para
        hallar errores.

    */
    unsigned char calcularBCE(unsigned char* datos, int Long);
    
    /*
        Metodo que muestra el campo datos de una trama recibida, funciona independientemente de su tamaño
        total de elementos, necesita previamente conversion a char desde unsigned char.
    */
    void mostrarTramas(char* payload, int size);

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

    char* filtrarTrama(char* const tramaRecibida, int longitud, int &nuevaLongitud, int filtroBajo, int filtroAlto);

    void ComunicacionProtocolo(unsigned char tipo, unsigned char *mac_destino);

    void mostrarTramasControl(unsigned char* trama, unsigned char BCEcalculado, unsigned char BCE, unsigned char recepcion);

    vector<string> cargarDatos(string nombreFichero);
    void volcarDatos(vector<char*> datosGuardados, vector<int> vSize, string nombreFichero);

    /*
        Método que extrae una interfaz de un vector de interfaces en función
        de un índice pedido por teclado al usuario, dicho índice debe ser menor
        al rango introducido como máximo (se sale del tamaño máximo)

        El método muestra por pantalla la dirección MAC de la interfaz seleccionada.
    */
    void seleccionarInterfaz(pcap_if *listaInterfaz, unsigned int max);
    

    /*
        Envia unos caracteres previamente convirtiendolos a un formato de protocolo ethernet para su envio y recepcion.
    */
    void EnviarTramas(char* car, int size, unsigned char* mac_dst);

    /*
        Metodo que recibe una trama ethernet de longitud variable.
        Presenta como salida la longitud de dicha cadena y ademas convierte de unsigned char a char
        para su posterior procesamiento de caracteres.
    */
    char* recibirTrama(int& longitud);

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
