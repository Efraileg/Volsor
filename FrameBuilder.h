
#include "linkLayer.h"
#include <string.h>
#include <iostream>

class FrameBuilder{

private:
    
    //StringMethod sm;
    unsigned char* recibirTrama(int &size);

protected:

    interface_t* interfaz;
    unsigned char tipo[2];

public:

    FrameBuilder(interface_t *_interfaz);
    FrameBuilder();

    unsigned char* tramaControl(unsigned char direccion, unsigned char control, unsigned char numero);
    unsigned char* tramaDatos(unsigned char direccion, unsigned char control, unsigned char numero, char* datos, int longitud, int BCE);

    void enviarBroadcast();
    unsigned char* responderBroadcast(unsigned char* mac_dest);

    void limpiarTrama(unsigned char* trama);

    void enviarTrama(unsigned char* trama, int size, unsigned char* mac_dest);

    unsigned char* esperarTrama(int &size);         //Espera.

    unsigned char* comprobarTrama(int &size);       //Continua.

    unsigned char* modificarTrama(unsigned char* trama);

    void mostrarTramasControl(unsigned char* trama, unsigned char BCE, unsigned char BCEcalculado, unsigned char recepcion);

    void mostrarTramas(char* payload, int size);

    void insertarEnTrama(unsigned char* trama, unsigned char* tramaInsertar, int posInsercion, int size1, int size2);

    std::string filtrarTrama(unsigned char* tramaRecibida, int longitud, int &nuevaLongitud, int filtroBajo, int filtroAlto);
   

    void asignarTipo(unsigned char tipo1, unsigned char tipo2);

    ~FrameBuilder();
};