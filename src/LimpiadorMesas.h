//
// Created by ale on 27/10/16.
//

#ifndef RESTAURANTCONCURRENTE_LIMPIADORMESAS_H
#define RESTAURANTCONCURRENTE_LIMPIADORMESAS_H


#include <Pipe.h>
#include <LockFd.h>
#include "Forkeable.h"

#define LIMPIAR_PEDIDOS "LIMPIAR_PEDIDOS"

class LimpiadorMesas: public Forkeable {

    Pipe& pedidos;  // Pipe de pedidos de todas las mesas y todos los mozos
    LockFd& lockLecturaMesas;


    virtual void run() override;
    void rutinaLimpiador();

public:
    LimpiadorMesas(Pipe &pedidos, LockFd &lockLecturaMesas);

    virtual ~LimpiadorMesas();

};


#endif //RESTAURANTCONCURRENTE_LIMPIADORMESAS_H
