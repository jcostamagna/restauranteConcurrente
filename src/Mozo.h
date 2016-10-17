//
// Created by ale on 16/10/16.
//

#ifndef RESTAURANTCONCURRENTE_MOZO_H
#define RESTAURANTCONCURRENTE_MOZO_H


#include "Forkeable.h"
typedef enum ESTADO_MOZO {
    RECIBIENDO_ORDEN, //Esperando que alguna mesa tenga un pedido y le diga cual es (ordenar comida, ordenar cuenta)
    ESPERANDO_COMIDA,
    ENTREGANDO_COMIDA,
    ENTREGANDO_CUENTA,
    APAGON} e_mozo;

class Mozo: public Forkeable {
private:
    bool vive;
    e_mozo estado;

    virtual void run() override;
    void rutinaMozo();

    void esperandoComida();
    void recibiendoOrden();
    void entregandoComida();
    void entregandoCuenta();
    void apagon();

    void avanzarEstado();

public:
    Mozo();
    virtual ~Mozo();



};


#endif //RESTAURANTCONCURRENTE_MOZO_H
