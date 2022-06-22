#include "LectorEntrada.h"
#include "kbhit.h"


#define ESC 27
#define FEspecial 79
#define F1 80
#define F2 81
#define F3 82
#define F4 83


LectorEntrada::LectorEntrada(){

};



int LectorEntrada::funcionEspecial(){

    int funcion = 0;                        //No se ha presionado tecla.
    char car;
    if(kbhit()){
        car = getch();
        if(car == ESC){
            if(kbhit()){
                car = getch();
                if(car == FEspecial){
                    car = getch();
                    funcion = car-FEspecial;       //[80,81,82,83] - 79 = [1,2,3,4]
                }
            }   
            else{
                funcion = -1;               //Se ha presionado ESC
            }
        }
        else{
            switch(car){

                case 'A':
                    funcion = 5;
                    break;
                case 'B':
                    funcion = 6;
                    break;
                case 'C':
                    funcion = 7;
                    break;
                case 'D':
                    funcion = 8;
                    break;
            }
        }
    }
    return funcion;
};

void LectorEntrada::asignarTecla(char _key){
    key = _key;
};

bool LectorEntrada::teclaMarcada(){
    if(kbhit()){
        if(getch() == key){
            return true;
        }
    }
    return false;
};

LectorEntrada::~LectorEntrada(){
    
};