//
// Created by ale on 16/10/16.
//

#ifndef RESTAURANTCONCURRENTE_MOZO_H
#define RESTAURANTCONCURRENTE_MOZO_H


#include <Pipe.h>
#include <LockFd.h>
#include <Semaforo.h>
#include <map>
#include <MemoriaCompartida2.h>
#include "Forkeable.h"

typedef enum ESTADO_MOZO {
    RECIBIENDO_ORDEN, //Esperando que alguna mesa tenga un pedido y le diga cual es (ordenar comida, ordenar cuenta)
    ESPERANDO_COMIDA,
    ENTREGANDO_COMIDA,
    ENTREGANDO_CUENTA,
    APAGON_MOZO
} e_mozo;

class Mozo : public Forkeable {
private:
    int id;
    Pipe& pedidos;  // Pipe de pedidos de todas las mesas y todos los mozos
    Pipe& eCocinero;  // Mozo escribe pedidos en el pipe, cocinero los lee
    Pipe& lCocinero;  // Cocinero escribe
    bool vive;
    e_mozo estado;

    Semaforo& semaforoConCocinero;  // Me bloqueo cuando espero que el cocinero cocine la comida
    const std::map<int, Semaforo*> &semaforosMesas; // Para que las mesas puedan continuar cuando les devuelvo su pedido

    MemoriaCompartida2<int> cuenta;
    int idMesa;

    Mozo(const Mozo& object);
    Mozo& operator=(const Mozo& object);

    virtual void run() override;

    void rutinaMozo();

    void pedirComida();

    void recibiendoOrden();

    void entregandoComida();

    void entregandoCuenta();

    void apagon();

    void avanzarEstado();

public:
    Mozo(int id,Pipe& pedidos, Pipe& escrCocinero, Pipe& lectCocinero, Semaforo& semaforo,const std::map<int, Semaforo*> &semaforosMesas);

    virtual ~Mozo();


    void esperarComida();
};


#endif //RESTAURANTCONCURRENTE_MOZO_H
