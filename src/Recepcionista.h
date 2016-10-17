//
// Created by juan on 15/10/16.
//

#ifndef RESTAURANTCONCURRENTE_RECEPCIONISTA_H
#define RESTAURANTCONCURRENTE_RECEPCIONISTA_H


#include <Pipe.h>
#include "Forkeable.h"

class Recepcionista: public Forkeable {
private:
    Pipe& clientes;

    void rutinaRecepcionista();
    Recepcionista(const Recepcionista& object);
    Recepcionista& operator=(const Recepcionista& object);

    virtual void run() override;

public:
    Recepcionista (Pipe& clientes): clientes(clientes) {}
};


#endif //RESTAURANTCONCURRENTE_RECEPCIONISTA_H
