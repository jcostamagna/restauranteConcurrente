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
#include "Gerente.h"

class Restaurante {

    int recepCant, mozosCant, mesasCant, clientesCant;
    std::vector<std::pair<std::string, int> > menu;

    Cocinero *cocinero;
    Gerente *gerente;
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
    Semaforo semCajaRestaurante;

    MemoriaCompartida2<int> cantClientesLiving;  // Cantidad de clientes en el living
    Semaforo escrituraLiving;  // Para que solo escriban en el cantLiving de a uno por vez

    MemoriaCompartida2<int> dineroPerdido;  // De comidas que se ordenaron pero no se pagaron en el apagon
    Semaforo semDineroPerdido;
//    MemoriaCompartida2<int> dineroNoAbonado;  // De comidas que se ordenaron pero no se pagaron toavia




    GeneradorClientes generadorClientes;  // Genera clientes y los mete en el pipe puerta

    LockFd lockLecturaClientes;  // lock de lectura de la puerta (pipe clientes)
    LockFd lockLecturaLiving;
    LockFd lockLecturaMesas;    // lock de lectura de mesas (pipe pedidos mesa)

    bool apagon;

    MemoriaCompartida2<bool> tirarPedidosDeMesas;  //se utiliza junto con lockLecturaMesas

    void iniciarMozos();

    void iniciarCocinero();

    void iniciarRecepcionistas();

    void iniciarMesas();

    void iniciarGeneradorClientes();

public:
    Restaurante(int recepCant, int mozosCant, int mesasCant, int clientesCant, std::vector<std::pair<std::string, int>> menu);

    void iniciarPersonal();

    void apagonRestaurante();

    void vaciar_living();

    virtual ~Restaurante();

    void iniciarGerente();


    void limpiar_mesas();


    void limpiar_cocina();
};


#endif //RESTAURANTCONCURRENTE_RESTAURANTE_H
