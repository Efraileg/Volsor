

#include "selector.h"
#include "FrameBuilder.h"

void selector::mostrarMAC(unsigned char* mac){
        for(int i = 0; i<5; i++){
            printf("%x : ", mac[i]);
        }
        printf("%x\n", mac[5]);
    }

unsigned char selector::calcularBCE(unsigned char* datos, int Long){
        unsigned char BCE = datos[0];

        for(int i = 1; i<Long; i++){
            BCE = BCE xor datos[i];
        }
        return (BCE == 0 || BCE == 255) ? 1 : BCE;
    }

void selector::recibirConfirmacion(){
        
        apacket_t p;
        const unsigned char* data;
        unsigned char tipoEmisor;
        unsigned char* macActual;
    
        //Estructura if-else, pensada para determinar quien sera maestro y quien esclavo a la hora de almacenar la MAC
        //del otro y a quien esperar.
        if(estado == 1){
            tipoEmisor = 0x02;
            macActual = MAC_esclavo;
            printf("Esperando a encontrar la estacion esclava.\n");
        }
        else{
            tipoEmisor = 0x01;
            macActual = MAC_maestro;
            printf("Esperando respuesta de la estacion maestra.\n");
        }

        //Espera hasta recibir la confirmación de la otra estación para poder efectuar la comunicación.
        //Posteriormente, almacena la dirección MAC para usarla en los protocolos principales de envio de datos.
        while(1){
            p = ReceiveFrame(&interfaz);
            data = p.packet;

            if(data != nullptr){

                if(*(data+12) == tipo[0] && *(data+13) == tipoEmisor){
                    
                    for(int i = 0; i<6; i++){
                        macActual[i] = *(data+i+6);
                    }

                    break;
                }
            }
        }       

        printf("Estacion encontrada. La MAC es: ");
        mostrarMAC(macActual);
        cout<<"El tamaño de paquete datos recibido es: "<<p.header.len;

        

    }

void selector::maestro(){

        //Construir MAC broadcast
        //Construir trama

        unsigned char MAC_BroadCast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        tipo[1] = 0x01;
        unsigned char* tramaBroad = BuildHeader(interfaz.MACaddr, MAC_BroadCast, tipo);
        SendFrame(&interfaz, tramaBroad, 0);

        //Metodo recibir confirmación, empleado en la busqueda del esclavo.
        recibirConfirmacion();

        printf("Seleccion de modo:\n\t[F1] - Envio de caracteres interactivos\n\t[F2] - Envio de un fichero\n\t[F3] - Protocolo paro y espera\n\t[F4] - Introduccion de errores durante el protocolo\n\t[ESC] - Salir\n");

       __fpurge(stdin);

        int funcion = 0;
        char car, *aux;
        int longitud;
        unsigned char* tramaEnviar;
        bool salir = false;
        bool recibido = false;
        while(!salir){
        
            while(funcion == 0){
                funcion = leerTeclaEspecial();
                if(funcion == 4){
                    //caracter no válido:
                    funcion = 0;
                }
                if(funcion == -1){
                    salir = true;
                }
                __fpurge(stdin);
            }

            tipo[1] = 0x00;     // Se indica comunicacion normal.
            if(funcion == 1){
                
                car = 0;

                //Se espera hasta introducir un ESC, en cuyo caso se regresa al menu.
                while(car != 27){
                    
                    aux = recibirTrama(longitud);

                    mostrarTramas(aux, longitud);

                    if(kbhit()){
                        car = getch();
                        if(car != 27){
                            EnviarTramas(&car, 1, MAC_esclavo);
                        }
                    }
                }
                //En caso de introducir teclas especiales como F1, al estar compuestas por varias partes, entre ellas el valor de ESC.
                //Se limpia el resto del buffer de teclado para evitar inconsistencias con el resto de opciones del menú.
                __fpurge(stdin);    
            }

            if(funcion == 2){       // Enviar fichero
                
                //Primero se leen los datos del fichero y se almacenan en un vector de string.
                //Posteriormente se enviará cada parte convertido en *char a una trama de datos
                cout<<" Se va a enviar el fichero"<<endl;
                vector<string> datosGuardados = cargarDatos("envio.txt");
                char* c;
                int l = 0;
                unsigned char* tramaEnviar;

                cout<<"Fichero enviandose con: "<<datosGuardados.size()<<" tramas"<<endl;
                for(int i = 0; i<datosGuardados.size(); i++){

                    l = datosGuardados[i].size();
                    c = new char[l];

                    datosGuardados[i].copy(c, l);
                    EnviarTramas(c, l, MAC_esclavo);

                    delete c;
                }
            }

            //Funcion de comunicacion de protocolo de paro y espera
            if(funcion == 3){
                //Seleccion o sondeo
                printf("\n\n Modo de funcionamiento F3 \n");
                printf("Selecciona seleccion 'R' o sondeo 'T'\n");

                unsigned char funcionamiento;
                cin>>funcionamiento;

                
                while(funcionamiento != 'R' && funcionamiento != 'T'){
                    printf("Caracter no valido:  Selecciona seleccion 'R' o sondeo 'T'\n");
                    cin>>funcionamiento;
                }

                printf("\n\n");
                recibido = false;
                //Reconstruir la trama ENQ en el caso de que se dañase al enviarse en ese caso se reenvia.

                while(!recibido){
                    //ENVIAR pregunta ¿Sondeo o Seleccion?
                    tramaEnviar = construirTramaControl(funcionamiento, ENQ, '0');
                    EnviarTramas((char*) tramaEnviar, 3, MAC_esclavo);
                    mostrarTramasControl(tramaEnviar, 0, 0, 'E');
                    delete tramaEnviar;

                    //ESPERAR RESPUESTA
                    
                    while(!recibido){
                        aux = recibirTrama(longitud);
                        if(aux != nullptr){
                            if(*(aux+1) == ACK){
                                recibido = true;
                            }
                            mostrarTramasControl((unsigned char*)aux, 0, 0, 'R');
                            printf("\n");
                        }   
                    }
                }

                // Metodo encargado de gobernar la comunicación, se encarga tanto de los envios de trama de dato, respuestas de
                // confirmacion y negacion y tramas de fin de comunicacion.
                ComunicacionProtocolo(funcionamiento, MAC_esclavo);

                
            }
            __fpurge(stdin);
            
            //Si se presiona ESC en cualquier modo de funcionamiento se imprimirá de nuevo el menu para visualizarlo nuevamente.
            //Si ya se presiono ESC en el propio menu, saldrá del código sin mostrar el menú por segunda vez seguida.
            if(funcion != -1){
                printf("Seleccion de modo:\n\t[F1] - Envio de caracteres interactivos\n\t[F2] - Envio de un fichero\n\t[F3] - Protocolo paro y espera\n\t[F4] - Introduccion de errores durante el protocolo\n\t[ESC] - Salir\n");
            }

            //Reseteo de funcion.
            funcion = 0;
        }
    }
    
void selector::esclavo(){

        tipo[1] = 0x02;
        //Metodo para establecer la comunicación con la estación maestra.
        recibirConfirmacion();

        unsigned char* tramaRespuesta= BuildHeader(interfaz.MACaddr, MAC_maestro, tipo);
        SendFrame(&interfaz, tramaRespuesta, 0);

        printf("Seleccion de modo: \n\t[F1] - Envio de caracter interactivo \n\t[F3] - Trama\n\t[F4] - Introduccion de errores durante el protocolo\n\t[ESC] - Salir\n");

        tipo[1] = 0x00;
        char car = 'A';
        unsigned char* enviarTramas;
        int longitud;
        char* aux;
        int funcionamiento = 0;
        bool salir = false;

        //Bucle encargado de gobernar el menu y todas las diferentes opciones, es una modificación del bucle de maestro.
        //pues esclavo necesita estar mostrando todo lo que le llegue independientemente de elegir alguna opción.
        __fpurge(stdin);
        while(!salir){
            
            aux = recibirTrama(longitud);
            
            mostrarTramas(aux, longitud);

            //Estado base, no se toman opciones del meno
            if(funcionamiento == 0){
                funcionamiento = leerTeclaEspecial();
                __fpurge(stdin);
            }
            
            //F2 y F4 no son validas, F4 solo será válida en el protocolo de paro y espera cuando a esclavo le toque enviar errores.
            if(funcionamiento == 2 || funcionamiento == 4){
                funcionamiento = 0;
            }

            //Envio interactivo de caracteres.
            if(funcionamiento == 1){
                    if(car != 27){
                        
                        if(kbhit()){
                            car = getch();
                            if(car != 27){
                                EnviarTramas(&car, 1, MAC_maestro);  // La primera MAC que cojo es la origen y a la que tengo que enviar los datos.
                            }
                        }   
                    }else{
                        funcionamiento = 0;
                        printf("Seleccion de modo: \n\t[F1] - Envio de caracter interactivo \n\t[F3] - Trama\n\t[F4] - Introduccion de errores durante el protocolo\n\t[ESC] - Salir\n");

                    }
                }
            
            //Protocolo de paro y espera.
            if(funcionamiento == 3){

                printf("\n\n Modo de funcionamiento F3 \n\n\n\n");
                

     
                bool recibirControlCorrecta = false;
                unsigned char seleccionSondeo;
                bool aceptable = false;
                unsigned char devolverControl;

                //Esperar recibir trama maestro y determinar si es tipo R o T
                while(!recibirControlCorrecta){

                    aux = recibirTrama(longitud);
                    if(aux != nullptr){
                            
                            //No es pedido pero podría darse el caso de recibir una ENQ erronea
                        aceptable = ((*(aux) == 'R' || *(aux) == 'T') && *(aux + 1) == ENQ);
  
                        if(aceptable){
                            recibirControlCorrecta = true;
                            devolverControl = ACK;

                            seleccionSondeo = *(aux);

                            printf("\n\n\n");
                            mostrarTramasControl((unsigned char*) aux, 0, 0, 'R');
                        }else{
                            devolverControl = NACK;
                        }

                        //Responder a mestro su ENQ.
                        enviarTramas = construirTramaControl(*(aux),devolverControl, *(aux+2));
                        EnviarTramas((char*)enviarTramas,3,MAC_maestro);
                        mostrarTramasControl(enviarTramas, 0, 0, 'E');
                           
                    }

                }

                printf("\n");

                //Igual que en maestro, se establece la comunicación.
                ComunicacionProtocolo(seleccionSondeo, MAC_maestro);

                __fpurge(stdin);  
                funcionamiento = 0;
                printf("Seleccion de modo: \n\t[F1] - Envio de caracter interactivo \n\t[F3] - Trama\n\t[F4] - Introduccion de errores durante el protocolo\n\t[ESC] - Salir\n");

            }
            
            
            //Limpiado del buffer restante y determinar si salir al presionar ESC en el propio menu.
            salir = (funcionamiento == -1);
            __fpurge(stdin);
        }
    }

char* selector::filtrarTrama(char* const tramaRecibida, int longitud, int &nuevaLongitud, int filtroBajo, int filtroAlto){
    nuevaLongitud = longitud - (filtroBajo + filtroAlto);
    return (tramaRecibida+filtroBajo);
}

void selector::ComunicacionProtocolo(unsigned char tipoComunicacion, unsigned char *mac_destino){
    bool emisorOreceptor;

    FrameBuilder FB(interfaz, tipo);

    // True = emisor
    // False = receptor
    if(tipoComunicacion == 'R'){
        
        emisorOreceptor = (estado == 1);
        
    }else{

        emisorOreceptor = (estado == 2);
    }

    char *c = nullptr, *aux = nullptr, caracter;
    unsigned char *tramaEnviar, *tramaControl;
    unsigned char numTrama = '0', BCE, devolverControl;
    int Error, ErrorAcum = 0;

    bool ultimaTrama = false, recibido = false, respondido = false, lecturaFallos;
    int i = 0, l = 0, longitud = 0;

    if(emisorOreceptor){
        //ES EMISOR

        //Se carga en un vector string el fichero de datos.
         vector<string> datosGuardados = cargarDatos("EProtoc.txt");    


        //Repetir hasta enviar la ultima trama del fichero.
        while(!ultimaTrama){
                

            l = datosGuardados[i].size();

            c = new char[l];
            datosGuardados[i].copy(c, l);

            i++;
            if(i ==  datosGuardados.size()){
                ultimaTrama = true;
            }


            //Esperar hasta que el receptor nos haya respondido de forma correcta con ACK.
            while(!respondido){
                
                Error = 3;  //LeerTeclaEspecial.


                tramaControl = construirTramaControl(tipoComunicacion, STX, numTrama); //CAMBIAR 0 o 1
                BCE = calcularBCE((unsigned char*)c, l);
                tramaEnviar = construirTramaDatos(tramaControl, l, (unsigned char*)c, BCE);

                if(ErrorAcum > 0){
                    printf("\nINTRODUCCIENDO ERROR\n");
                    *(tramaEnviar+4) = 'ç';   
                    ErrorAcum--;
                }
                
                
                
               

                mostrarTramasControl(tramaEnviar, BCE, 0, 'E');
                EnviarTramas((char*)tramaEnviar, 5 + l,mac_destino);
                delete tramaEnviar;




                while(!respondido){                     // A la espera de que me responda.
                    aux = recibirTrama(longitud);
                    if(aux != nullptr){
                        if(*(aux+1) == ACK){
                            mostrarTramasControl((unsigned char*) aux, 0, 0, 'R');
                            respondido = true;
                        }
                        else if (*(aux+1) == NACK){
                            mostrarTramasControl((unsigned char*) aux, 0, 0, 'R');
                            break;
                        }
                    }
                }
            }

            numTrama = (numTrama == '0') ? '1' : '0';   //Alternar entro 0 o 1
            delete c;
            respondido = false;
        }

        numTrama = '1';         
        printf("\n");
        while(!recibido){
            
            numTrama = (numTrama == '0') ? '1' : '0';   //Alternar entre 0 o 1

            tramaEnviar = construirTramaControl(tipoComunicacion, EOT, numTrama);
            mostrarTramasControl(tramaEnviar, 0, 0, 'E');

            EnviarTramas((char*)tramaEnviar,3,mac_destino);
            delete tramaEnviar;

            recibido = false;
            while(!recibido){
                aux = recibirTrama(longitud);
                if(aux != nullptr){
                    if(*(aux+1) == ACK){
                        recibido = true;
                        mostrarTramasControl((unsigned char*) aux, 0, 0, 'R');
                    }
                    else if(*(aux+1) == NACK){
                        mostrarTramasControl((unsigned char*) aux, 0, 0, 'R');
                        break;
                    }
                }
            }
        }
    }else{      // receptor de la comunicacion
        bool recibirFinal = false;
        int tamDatos = 0, nuevaLongitud = 0;
        unsigned char BCEcalculado = ' ';
        char* tramaFiltrada = nullptr;

        vector<char*> datosGuardados;
        vector<int> vSize;

        while(!recibirFinal){

            aux = recibirTrama(longitud);
            if(aux != nullptr){

                if(*(aux+1)==STX) { //Debe ser STX
                    tamDatos = (unsigned char) *(aux + 3);  // Casting a unsigned char transformando el posible valor negativo a positivo (-1 => FF = 255).
                    BCEcalculado = calcularBCE((unsigned char*)aux+4, tamDatos);
                    mostrarTramasControl((unsigned char*)aux, BCEcalculado, (unsigned char)aux[longitud-1], 'R');
                    if(BCEcalculado == (unsigned char) *(aux+longitud-1)){                   
                        tramaFiltrada = filtrarTrama(aux, longitud, nuevaLongitud, 4, 1);

                        datosGuardados.push_back(tramaFiltrada);      // Datos a guardar en el fichero
                        vSize.push_back(nuevaLongitud);

                        devolverControl = ACK;
                    }else{ 
                        devolverControl = NACK;
                    }

                    tramaEnviar = construirTramaControl(tipoComunicacion, devolverControl, *(aux+2));
                    EnviarTramas((char*)tramaEnviar,3,mac_destino);
                    mostrarTramasControl(tramaEnviar, 0, 0, 'E');

                    delete tramaEnviar;

                }else if(*(aux+1)==EOT){
                    unsigned char respuestaConfirmada = ACK; 
                    recibirFinal = true;
                    printf("\n");
                    mostrarTramasControl((unsigned char*) aux, 0, 0, 'R');

                    if(estado == 1){        // Modo maestro, esto se encarga de determinar si el maestro acepta cerrar la
                        int opc = 0;        // comunicacion en sondeo (T)

                        __fpurge(stdin);
                        cout<<"Acepta el cierre de la comunicacion: \n [1] Si. \n [2] No.\nOpcion: ";
                        while(opc < 1 || opc > 2){
                            if(kbhit()){
                                opc= getch()-48;
                                
                            }
                        }
                        __fpurge(stdin);

                        if(opc == 1){
                            respuestaConfirmada = ACK; 
                             
                        }

                        else if (opc == 2){
                            respuestaConfirmada = NACK;
                            recibirFinal = false;
                        }
                        printf("\n");
                    }

                    //Enviar respuesta de cierre.
                    tramaEnviar = construirTramaControl(tipoComunicacion, respuestaConfirmada, *(aux+2));

                    EnviarTramas((char*)tramaEnviar,3,mac_destino);
                    mostrarTramasControl(tramaEnviar, 0, 0, 'E');

                    delete tramaEnviar;
                }          
            }
        }
        
        //Guardar en fichero.
        volcarDatos(datosGuardados, vSize, "RProtoc.txt");
    }
}
  
vector<string> selector::cargarDatos(string nombreFichero){
        
        ifstream file;
        string cad = "", aux = "", key = "Trama";
        vector<string> v;

        file.open(nombreFichero);

        if(file.is_open()){
            while(!file.eof()){
                getline(file, aux);
                cad += aux;
            }

            file.close();   
        }

        int index = 0;
        bool band = true;
        while(band){
            
            index = cad.length();

            if(index > 254){
                index = 254;  
            }else{
                band = false;
            }

            aux = cad.substr(0, index);
            if(index == 254){
                cad = cad.substr(index, cad.length());
            }
            
            v.push_back(aux);

        }
        
        return v;
    }

void selector::volcarDatos(vector<char*> datosGuardados, vector<int> vSize, string nombreFichero){

    ofstream file;
    file.open(nombreFichero);
    
    if(file.is_open()){
        
        file.clear();

        for(int i=0; i<datosGuardados.size(); i++)
        {
            file.write(datosGuardados[i], vSize[i]);
        }

        file.close();
    }
}

void selector::seleccionarInterfaz(pcap_if *listaInterfaz, unsigned int max){
        
        unsigned int interface;
        std::cout<<"Seleccione interfaz: ";
        std::cin>>interface;

        if(interface < max){
            std::cout<<"\nInterfaz elegida:" ;

            interface_t* aux = new interface_t;

            setDeviceName(aux, listaInterfaz[interface].name);
            std::cout<<aux->deviceName<<std::endl;
            std::cout<<"La MAC es: ";

            GetMACAdapter(aux);

            interfaz = *aux;

            mostrarMAC(aux->MACaddr);   
            printf("\n");     
        }


    }
   
void selector::EnviarTramas(char* car, int size, unsigned char* mac_dst){
        unsigned char* dato;
        dato = (unsigned char*) car;

        unsigned char* trama = BuildFrame(interfaz.MACaddr, mac_dst, tipo, dato);
        SendFrame(&interfaz, trama, size);   // No usar sizeof(unsigned char) aunque sea igual a 1;

        free (trama);
    }

char* selector::recibirTrama(int& longitud){
        
        apacket_t p = ReceiveFrame(&interfaz);
        const unsigned char* data = p.packet;
        longitud = 0;
        unsigned int contador = 0;

        char* datos = nullptr;

        if(data != nullptr){
            if(*(data+12) == tipo[0] && *(data+13) == 0x00){        // Si es nuestro tipo de protocolo y es 0x00 (ya se ha conectado)

                int size = p.header.len-14;                 // Se ignora la macOrigen, macDestino, protocolo, etc.
                
                
                if(size == 46){                             // Ethernet rellena el campo de datos con 46 bytes por defecto, si se envian mas de 46 no hay ceros (el elemento de fin de campo de datos) que detectar.            
                    while(*(data+14+contador) != 0x00 && contador < 46){     // Si se han enviado menos de 46 bytes o menos tiene que haber algún cero puesto por defecto.
                        contador++;                                          // Se comprueba que se hagan 46 iteraciones máximo para este caso evitando un bucle indefinido.
                    }
                    if(contador == 0){
                        return nullptr;
                    }
                }else{
                    contador = size;
                }

                datos = new char[contador];
                for(int i=0; i<contador; i++){       // Los 14 bytes son de la MAC_maestro, mac_destino, el protocolo... menos el campo de datos.
                    datos[i] = *(data+14+i);
                }

                
            }
        }


        longitud = contador;
        return datos;    
    }

selector::selector(){
        protocoloPropio = default_protocolo;
        grupo = 0x00;   //valor por defecto, no influye pues se va a modificar.
        MAC_esclavo = new unsigned char [6];
        MAC_maestro  = new unsigned char [6];
        estado = 0;     //valor por defecto, no influye pues se va a modificar.
    };

void selector::mostrarInterfaces(){
        pcap_if *sig;
        pcap_if *aux;

       
        sig = GetAvailAdapters(); 
        aux = sig;

        unsigned int i = 0;

        while(sig != nullptr){
            std::cout<<"["<<i<<"]"<<sig->name<<std::endl;
            i++;
            sig = sig->next;
        }
        if(i > 0){
            pcap_if *arrayInterface = new pcap_if[i];
            sig = aux;
            for(int j=0; j<i; j++){
                arrayInterface[j] = *sig;
                sig = sig->next;
            }

            seleccionarInterfaz(arrayInterface, i);
        }
    }

void selector::establecerComunicacion(){
        printf("Introduzca el numero de grupo:\n");
        
        std::cin>>grupo;
        grupo -= 48;                            //Pasar de 'char' a 'decimal'
        tipo[0] = protocoloPropio + grupo;

        printf("Seleccione el modo de la estacion:\n\t[1] Modo Maestra\n\t[2] Modo Esclava\n");
    
        std::cin>>estado;

        __fpurge(stdin);

        //Abrir el puerto.
        int Puerto=OpenAdapter(&interfaz);
        
        if(Puerto != 0){
            printf("Error al abrir el puerto");
            getch();
        }
        
        else{
            switch(estado){
            case 1:
                //Soy maestro
                maestro();
                break;
            case 2:
                //Soy esclavo
                esclavo();
                break;
            default:
                break;
                //No hago nada/No quiero comunicar
            }

            
        }

        //Cerrar el puerto.
        CloseAdapter(&interfaz);    
    }