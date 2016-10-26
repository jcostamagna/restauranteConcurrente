//
// Created by ale on 18/10/16.
//

#ifndef RESTAURANTCONCURRENTE_RESTAURANTE_H
#define RESTAURANTCONCURRENTE_RESTAURANTE_H


#include <map>
#include <MemoriaCompartida2.h>
#include "Cocinero.h"
#include "Recepcionista.h"
#include "Mozo.h"
#include "GeneradorClientes.h"
#include "Mesa.h"

class Restaurante {

    int recepCant, mozosCant, mesasCant, clientesCant;
    std::vector<std::pair<std::string, int> > menu;

    Cocinero *cocinero;
    std::map<pid_t, Mozo *> mozosMap;

    Pipe living; //clientes en el living, mesas que los sacan
    Pipe puerta;  // clientes que entran por la puerta, recepcionistas que los atienden
    Pipe pipePedidosMesas; // clientes en mesas escriben sus pedidos, los mozos leen los pedidos
    Pipe pipeECocinero;  // Pipe escritura cocinero de pedidos de comidas

    std::list<Mozo*> mozos;
    std::list<Semaforo*> semaforosCocineroMozos;
    std::map<int,Semaforo*> semaforosMesas;  // Un semaforoConCocinero por mesa. Se bloquean cuando esperan al mozo. El mozo las desbloquea
    std::list<Recepcionista*> recepcionistas;
    std::list<Mesa*> mesas;

    MemoriaCompartida2<int> caja;
    MemoriaCompartida2<int> cantLiving;  // Cantidad de clientes en el living
    MemoriaCompartida2<int> dineroNoAbonado;  // De comidas que se ordenaron pero no se pagaron

    Semaforo escrituraLiving;  // Para que solo escriban en el cantLiving de a uno por vez

    GeneradorClientes generadorClientes;  // Genera clientes y los mete en el pipe puerta

    LockFd lockLecturaClientes;  // lock de lectura de la puerta (pipe clientes)
    LockFd lockLecturaLiving;
    LockFd lockLecturaMesas;    // lock de lectura de mesas (pipe pedidos mesa)

    bool apagon;

    void iniciarMozos();

    void iniciarCocinero();

    void iniciarRecepcionistas();

    void iniciarMesas();

    void iniciarGeneradorClientes();

public:
    Restaurante(int recepCant, int mozosCant, int mesasCant, int clientesCant, std::vector<std::pair<std::string, int>> menu);

    void iniciarPersonal();

    void abrirPuertas();

    void apagonRestaurante();

    void vaciar_living();

    virtual ~Restaurante();

};


#endif //RESTAURANTCONCURRENTE_RESTAURANTE_H
