
#include "StringMethod.h"

StringMethod::StringMethod(){

}

template <class TDD>
int StringMethod::getSizeArray(TDD* array){

    // return (*array == nullptr) ? 0 : ((*array == '\0') ? 0 : 1 + getSizeArray(array+1));

    if(array == nullptr){
        return 0;
    }

    int contador = 0;

    while(*array != '\0'){
        array++;
        contador++;
    }

    return contador;
}

template <class TDD>
void StringMethod::copiarArrays(TDD* arrayOrigen, TDD*& arrayDestino){

    if(arrayOrigen == nullptr){                 // No existe el array desde donde se copiará la información -> No se hace nada.
        return;
    }

    int sizeArrayOrigen = getSizeArray(arrayOrigen);

    delete[] arrayDestino;                      // Se elimina el array independientemente de su contenido. No hay problemas si el array está en nullptr.
    arrayDestino = new TDD(sizeArrayOrigen);    // Se reserva la memoria necesaria para acoger la información del arrayOrigen.
    
    for(int i=0; i<sizeArrayOrigen; i++){       // Se copia la información.
        arrayDestino[i] = arrayOrigen[i];
    }
}

template <class TDD>
void StringMethod::copiarArrays(TDD* arrayOrigen, TDD*& arrayDestino, int sizeArrayOrigen){

    if(arrayOrigen == nullptr){                 // No existe el array desde donde se copiará la información -> No se hace nada.
        return;
    }

    delete[] arrayDestino;                      // Se elimina el array independientemente de su contenido. No hay problemas si el array está en nullptr.
    arrayDestino = new TDD(sizeArrayOrigen);    // Se reserva la memoria necesaria para acoger la información del arrayOrigen.
    
    for(int i=0; i<sizeArrayOrigen; i++){       // Se copia la información.
        arrayDestino[i] = arrayOrigen[i];
    }
}

template <class TDD>
void StringMethod::concatenarArrays(TDD* arrayOrigen, TDD*& arrayDestino){

   // No existe el array desde donde se copiará la información -> No se hace nada.
   if(arrayOrigen == nullptr){
       return;
   }

    int sizeArrayOrigen = getSizeArray(arrayOrigen);
    int sizeArrayDestino = getSizeArray(arrayDestino);
    int totalSize = sizeArrayOrigen + sizeArrayDestino;

   // El array donde se insertará la información no está creado -> Se crea.
    if(arrayDestino == nullptr){
        
        arrayDestino = new TDD(totalSize);

        for(int i=0; i<totalSize; i++){
            arrayDestino[i] = arrayOrigen[i];
        }
    }
    
    else{
    	
		for(int i=sizeArrayDestino, j=0; i<sizeArrayOrigen; i++, j++){
			arrayDestino = new TDD;
			arrayDestino[i] = arrayOrigen[j];
		}
	}
}

template <class TDD>
void StringMethod::concatenarArrays(TDD* arrayOrigen, TDD*& arrayDestino, int sizeArrayOrigen, int sizeArrayDestino){

   // No existe el array desde donde se copiará la información -> No se hace nada.
   if(arrayOrigen == nullptr){
       return;
   }
	
    int totalSize = sizeArrayOrigen + sizeArrayDestino;

   // El array donde se insertará la información no está creado -> Se crea.
    if(arrayDestino == nullptr){
        
        arrayDestino = new TDD(totalSize);

        for(int i=0; i<totalSize; i++){
            arrayDestino[i] = arrayOrigen[i];
        }
    }
	
	else{
		
		for(int i=sizeArrayDestino, j=0; i<sizeArrayOrigen; i++, j++){
			arrayDestino = new TDD;
			arrayDestino[i] = arrayOrigen[j];
		}
			
	}
    
}

template <class TDD>
void StringMethod::insertarEnArray(TDD*& arrayDestino, TDD* characteres, int pos){

    // No existe el array desde donde se copiará la información -> No se hace nada.
    if(arrayOrigen == nullptr){
        return;
    }

    if(pos < 0 || pos > getSizeArray(arrayDestino)){
        return;
    }

    int sizeCharacters = getSizeArray(characteres);
    int sizeArrayDestino = getSizeArray(arrayDestino);
    int totalSize = sizeCharacters + sizeArrayDestino;

    // El array donde se insertará la información no está creado -> Se crea.
     if(arrayDestino == nullptr){
         
         arrayDestino = new TDD(totalSize);
         for(int i=0; i<totalSize; i++){
             arrayDestino[i] = characteres[i];
         }
     }

     else{
        
        TDD* aux = new TDD[totalSize];      // Tengo en cuenta el tamaño total del array + los caracteres a insertar.
        int i = 0;

        // Se copian los caracteres hasta la posicion "pos" del array
        for(int j=0; j<pos; i++, j++){
            aux[i] = arrayDestino[j];
        }

        // Se insertan los caracteres pasados por parametro ampliando el vector dinámicamente
        for(int j=0; j<sizeCharacters; i++, j++){
            aux[i] = characteres[j];
        }

        // Se insertan el resto de caracteres a partir de la posicion "pos" (los caracteres que faltan)
        for(int j=pos; j<sizeArrayDestino; i++, j++){
            aux[i] = arrayDestino[i];
        }

        copiarArrays(aux, arrayDestino);
     }
}

template <class TDD>
void StringMethod::insertarEnArray(TDD*& arrayDestino, TDD* characteres, int pos, int sizeArrayDestino, int sizeCharacters){

    // No existe el array desde donde se copiará la información -> No se hace nada.
    if(arrayOrigen == nullptr){
        return;
    }
	
    if(pos < 0 || pos > getSizeArray(arrayDestino)){
        return;
    }
	
    int totalSize = sizeCharacters + sizeArrayDestino;

    // El array donde se insertará la información no está creado -> Se crea.
     if(arrayDestino == nullptr){
         
         arrayDestino = new TDD(totalSize);
         for(int i=0; i<totalSize; i++){
             arrayDestino[i] = characteres[i];
         }
     }

     else{
        
        TDD* aux = new TDD[totalSize];      // Tengo en cuenta el tamaño total del array + los caracteres a insertar.
        int i = 0;

        // Se copian los caracteres hasta la posicion "pos" del array
        for(int j=0; j<pos; i++, j++){
            aux[i] = arrayDestino[j];
        }

        // Se insertan los caracteres pasados por parametro ampliando el vector dinámicamente
        for(int j=0; j<sizeCharacters; i++, j++){
            aux[i] = characteres[j];
        }

        // Se insertan el resto de caracteres a partir de la posicion "pos" (los caracteres que faltan)
        for(int j=pos; j<sizeArrayDestino; i++, j++){
            aux[i] = arrayDestino[i];
        }

        copiarArrays(aux, arrayDestino, totalSize);
     }
}

void StringMethod::copiarCharAString(string& cad, char* c)
{
    cad.copy(c, getSizeArray(c));
}

void StringMethod::copiarCharAString(string& cad, char* c, int size)
{
    cad.copy(c, size);
}

void StringMethod::copiarStringAChar(char*& c, string cad){

	int l = cad.size();

    delete[] c;                 // Se elimina el array independientemente de su contenido. No hay problemas si el array está en nullptr.
    c = new char(l);    		// Se reserva la memoria necesaria para acoger la información del string.
    
    for(int i=0; i<l; i++){     // Se copia la información.
        c[i] = cad[i];
    }
}