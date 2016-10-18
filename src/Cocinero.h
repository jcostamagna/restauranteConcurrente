//
// Created by ale on 17/10/16.
//

#ifndef RESTAURANTCONCURRENTE_COCINERO_H
#define RESTAURANTCONCURRENTE_COCINERO_H

#include "Forkeable.h"

typedef enum ESTADO_COCINERO {
    ESPERANDO_PEDIDO, //esperando que un mozo le de un pedido de comida
    ENTREGANDO_PEDIDO, //entrega la comida y hace...
    APAGON_COCINERO
} e_cocinero;

class Cocinero: public Forkeable {
private:
    bool vive;
    e_cocinero estado;

private:
    virtual void run() override;
    void rutinaCocinero();

    void esperandoPedido();
    void entregandoPedido();
    void apagon();

public:
    Cocinero();
    virtual ~Cocinero();


};


#endif //RESTAURANTCONCURRENTE_COCINERO_H
