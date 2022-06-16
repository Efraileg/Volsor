
#include "linkLayer.h"

class FrameBuilder{


private:

    unsigned char* recibirTrama(int &size);

protected:
    interface_t interfaz;
    unsigned char tipo[2];
public:

    FrameBuilder(interface_t _interfaz, unsigned char _tipo[2]);

    unsigned char* tramaControl(unsigned char direccion, unsigned char control, unsigned char numero);
    unsigned char* tramaDatos(unsigned char direccion, unsigned char control, unsigned char numero, char* datos, int longitud, int BCE);

    void limpiarTrama(unsigned char* trama);

    void enviarTrama(unsigned char* trama, int size, unsigned char* mac_dest);

    unsigned char* esperarTrama(int &size);         //Espera.

    unsigned char* comprobarTrama(int &size);       //Continua.

    unsigned char* modificarTrama(unsigned char* trama);

    void mostrarTramasControl(unsigned char* trama, unsigned char BCE, unsigned char BCEcalculado, unsigned char recepcion);

    void mostrarTramas(char* payload, int size);



    ~FrameBuilder();
};