//
// Created by juan on 15/10/16.
//

#ifndef RESTAURANTCONCURRENTE_RECEPCIONISTA_H
#define RESTAURANTCONCURRENTE_RECEPCIONISTA_H


#include <Pipe.h>

class Recepcionista {
private:
    Pipe& clientes;
    int pid;
    void rutinaRecepcionista();
    Recepcionista(const Recepcionista& object);
    Recepcionista& operator=(const Recepcionista& object);

public:
    Recepcionista (Pipe& clientes): clientes(clientes) {}
    void start();
    int getPid();
    void stop();
    //~Parser ();
};


#endif //RESTAURANTCONCURRENTE_RECEPCIONISTA_H
