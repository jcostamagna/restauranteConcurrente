//
// Created by ale on 17/10/16.
//

#ifndef RESTAURANTCONCURRENTE_COCINERO_H
#define RESTAURANTCONCURRENTE_COCINERO_H

#include <Pipe.h>
#include <Semaforo.h>
#include <list>
#include "Forkeable.h"

typedef enum ESTADO_COCINERO {
    ESPERANDO_PEDIDO, //esperando que un mozo le de un pedido de comida
    ENTREGANDO_PEDIDO, //entrega la comida y hace...
    APAGON_COCINERO
} e_cocinero;

class Cocinero: public Forkeable {
private:
    Pipe& eCocinero;
    Pipe& lCocinero;
    std::list<Semaforo*> semaforos;
    bool vive;
    e_cocinero estado;

private:
    virtual void run() override;
    void rutinaCocinero();

    void esperandoPedido();
    void entregandoPedido();
    void apagon();
    void avanzarEstado();

public:
    Cocinero(Pipe& escrCocinero, Pipe& lectCocinero, std::list<Semaforo *>& semaforos);
    virtual ~Cocinero();
};


#endif //RESTAURANTCONCURRENTE_COCINERO_H
