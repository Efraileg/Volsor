#include "FrameBuilder.h"






    unsigned char* FrameBuilder::recibirTrama(int &size){
        
        apacket_t p = ReceiveFrame(&interfaz);
        const unsigned char* data = p.packet;
        size = 0;
        unsigned int contador = 0;

        unsigned char* datos = nullptr;

        if(data != nullptr){
            if(*(data+12) == tipo[0] && *(data+13) == 0x00){        // Si es nuestro tipo de protocolo y es 0x00 (ya se ha conectado)

                int size = p.header.len-14;                 // Se ignora la macOrigen, macDestino, protocolo, etc.
            
                
                if(size == 46){                             // Ethernet rellena el campo de datos con 46 bytes por defecto, si se envian mas de 46 no hay ceros (el elemento de fin de campo de datos) que detectar.            
                    while(*(data+14+contador) != 0x00 && contador < 46){     // Si se han enviado menos de 46 bytes o menos tiene que haber algún cero puesto por defecto.
                        contador++;                                          // Se comprueba que se hagan 46 iteraciones máximo para este caso evitando un bucle indefinido.
                    }
                    if(contador == 0){
                        delete [] data;
                        return nullptr;
                    }
                }else{
                    contador = size;
                }

                datos = new unsigned char[contador];
                for(int i=0; i<contador; i++){       // Los 14 bytes son de la MAC_maestro, mac_destino, el protocolo... menos el campo de datos.
                    datos[i] = *(data+14+i);
                }

                
            }
        }


        size = contador;
        delete[] data;


        return datos;    
    }

    FrameBuilder::FrameBuilder(interface_t _interfaz){
        interfaz = _interfaz;
        //sm = StringMethod();
    }
    
    FrameBuilder::FrameBuilder(){}

    unsigned char* FrameBuilder::tramaControl(unsigned char direccion, unsigned char control, unsigned char numero){
        return new unsigned char[3]{direccion, control, numero};
        
    }

    unsigned char* FrameBuilder::tramaDatos(unsigned char direccion, unsigned char control, unsigned char numero, char* datos, int longitud, int BCE){

        int tam = 5 + longitud;

        unsigned char *tramaDatos = new unsigned char[tam];

        tramaDatos[0] = direccion;
        tramaDatos[1] = control;
        tramaDatos[2] = numero;
        tramaDatos[3] = longitud;
        tramaDatos[tam-1] = BCE;

        for(int i = 4; i<tam-1; i++){
            tramaDatos[i] = (unsigned char)datos[i-4];
        }

        return tramaDatos;
    }

    void FrameBuilder::enviarBroadcast(){
        unsigned char MAC_BroadCast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        tipo[1] = 0x01;
        unsigned char* tramaBroad = BuildHeader(interfaz.MACaddr, MAC_BroadCast, tipo);
        SendFrame(&interfaz, tramaBroad, 0);
        free(tramaBroad);
    }

    unsigned char* FrameBuilder::responderBroadcast(unsigned char* mac_dest){

    }

    void FrameBuilder::limpiarTrama(unsigned char* trama){
        delete[] trama;
    }

    void FrameBuilder::enviarTrama(unsigned char* trama, int size, unsigned char* mac_dest){

        unsigned char* tramaEnviar = BuildFrame(interfaz.MACaddr, mac_dest, tipo, trama);

        SendFrame(&interfaz, tramaEnviar, size);   // No usar sizeof(unsigned char) aunque sea igual a 1;

        free (tramaEnviar);
    }

    unsigned char* FrameBuilder::esperarTrama(int &size){
        bool condition = true;
        unsigned char* frame;
        while(condition){

            frame = recibirTrama(size);
            if(frame != nullptr){
                //Ya filtrada.
                condition = false;

            }
        }
        return frame;
    }

    unsigned char* FrameBuilder::comprobarTrama(int &size){

        return recibirTrama(size);
    }

    unsigned char* FrameBuilder::modificarTrama(unsigned char* trama){
        return nullptr;
    }

    void FrameBuilder::insertarEnTrama(unsigned char* trama, unsigned char* tramaInsertar, int posInsercion, int sizeTrama, int sizeTramaInsertar){

        //sm.insertarEnArray(trama, tramaInsertar, posInsercion);
    }

    char* FrameBuilder::filtrarTrama(unsigned char* tramaRecibida, int longitud, int &nuevaLongitud, int filtroBajo, int filtroAlto){
       
        nuevaLongitud = longitud - (filtroBajo + filtroAlto);

        char* datosFiltrados = new char(nuevaLongitud);

        for(int i = 0; i<nuevaLongitud; i++){
            datosFiltrados[i] = (char)tramaRecibida[i+filtroBajo];
        }
        return datosFiltrados;
    }

    void FrameBuilder::mostrarTramasControl(unsigned char* trama, unsigned char BCE, unsigned char BCEcalculado, unsigned char recepcion){
        
        printf("%c\t%c", recepcion, *(trama));
        switch(*(trama+1)){
            case 2:
                printf("\tSTX");
                break;
            case 4:
                printf("\tEOT");
                break;
            case 5:
                printf("\tENQ");
                break;
            case 6:
                printf("\tACK");
                break;
            case 21:
                printf("\tNACK");
        }

        printf("\t%c", *(trama+2));

        if(BCE != 0) //EXISTE BCE
        {
            printf("\t%d", BCE);
            if(BCEcalculado != 0){
                printf("\t%d", BCEcalculado);
            }
        }
        printf("\n");
    }

    void FrameBuilder::mostrarTramas(char* payload, int size)
    {
        if(payload != nullptr)
        {
            printf("Recibido\n");
            for(int i=0; i<size; i++)
            {
                printf("%c",*(payload+i));
            }
            printf("\n Longitud trama: %d",size);
        }
    }

    void FrameBuilder::asignarTipo(unsigned char tipo1, unsigned char tipo2){
        tipo[0] = tipo1;
        tipo[1] = tipo2;
    }

    FrameBuilder::~FrameBuilder(){

    }

