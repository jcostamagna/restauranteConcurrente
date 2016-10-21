//
// Created by juan on 17/10/16.
//

#ifndef RESTAURANTCONCURRENTE_GENERADORCLIENTES_H
#define RESTAURANTCONCURRENTE_GENERADORCLIENTES_H


#include <Pipe.h>
#include "Forkeable.h"

class GeneradorClientes: public Forkeable {
private:
    int cantClientes;
    Pipe& clientes;

    GeneradorClientes(const GeneradorClientes& object);
    GeneradorClientes& operator=(const GeneradorClientes& object);

    virtual void run() override;
    void rutinaGenerador();

public:
    GeneradorClientes (Pipe& clientes);
};


#endif //RESTAURANTCONCURRENTE_GENERADORCLIENTES_H
