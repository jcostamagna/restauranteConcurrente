//
// Created by nicolas on 20/10/16.
//

#ifndef RESTAURANTCONCURRENTE_MESAS_H
#define RESTAURANTCONCURRENTE_MESAS_H


#include <Pipe.h>
#include "Mesa.h"
#include <list>

class Mesas {
private:
    Pipe& pLiving;
    Pipe& pMesas;
    Pipe pClienteAMesa;
    LockFd lockMesas;
    int cantidadMesas;

    std::list<Mesa*> mesas;
    std::list<Semaforo*> semaforos;

public:
    Mesas (Pipe& pLiving, Pipe& pMesas, int cantidadMesas);
};


#endif //RESTAURANTCONCURRENTE_MESAS_H
