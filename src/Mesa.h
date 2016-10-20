//
// Created by nicolas on 20/10/16.
//

#ifndef RESTAURANTCONCURRENTE_MESA_H
#define RESTAURANTCONCURRENTE_MESA_H


#include <Pipe.h>
#include <Semaforo.h>
#include <MemoriaCompartida2.h>
#include <LockFd.h>
#include "Forkeable.h"

typedef enum ESTADO_MESA {
    ESPERANDO_CLIENTE,
    CLIENTE_SENTADO,
    CLIENTE_ESPERA_PEDIDO,
    CLIENTE_ESPERA_CUENTA,
    CLIENTE_SE_VA,
    APAGON_MESA
} e_mesa;

class Mesa : public Forkeable {
private:
    Pipe& living;
    Pipe& pedidos;

    LockFd& lockLiving;

    e_mesa estado;
    Semaforo* sEsperandoMozo;
    Semaforo& escrituraLiving;
    int idCliente;

    MemoriaCompartida2<int> cantClientesLiving;
    int cuenta;

    Mesa(const Mesa& object);
    Mesa& operator=(const Mesa& object);

    virtual void run() override;
    void rutinaMesa();

    void esperandoCliente();
    void clienteSentado();
    void clienteEsperaPedido();
    void clienteEsperaCuenta();
    void apagon();

    void avanzarEstado();

public:
    Mesa (Pipe& living, Pipe& pedidos, LockFd& lockLiving, Semaforo* sEsperandoMozo, Semaforo& escrituraLiving);
};


#endif //RESTAURANTCONCURRENTE_MESA_H
