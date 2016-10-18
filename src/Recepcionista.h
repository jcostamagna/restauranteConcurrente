//
// Created by juan on 15/10/16.
//

#ifndef RESTAURANTCONCURRENTE_RECEPCIONISTA_H
#define RESTAURANTCONCURRENTE_RECEPCIONISTA_H


#include <Pipe.h>
#include <LockFd.h>
#include "Forkeable.h"

typedef enum ESTADO_RECEPCIONISTA {
    ESPERANDO,
    UBICANDO_EN_LIVING,
    UBICANDO_EN_MESA,
    APAGON
} e_recepcionista;

class Recepcionista: public Forkeable {
private:
    Pipe& clientes;
    bool vive;
    e_recepcionista estado;
    LockFd& lecturaPuerta;



    Recepcionista(const Recepcionista& object);
    Recepcionista& operator=(const Recepcionista& object);

    virtual void run() override;
    void rutinaRecepcionista();

    void esperando();
    void ubicandoEnLiving();
    void ubicandoEnMesa();
    void apagon();

    void avanzarEstado();

public:
    Recepcionista (Pipe& clientes, LockFd& lecturaPuerta);


};


#endif //RESTAURANTCONCURRENTE_RECEPCIONISTA_H
