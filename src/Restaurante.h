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

    int recepCant, mozosCant, mesasCant;
    //std::map<std::string, int> menu;
    std::list<std::pair<std::string, int> > menu;

    Cocinero *cocinero;
    std::map<pid_t, Mozo *> mozosMap;

    Pipe living; //clientes en el living
    Pipe clientes;  // clientes que entran por la puerta
    Pipe pipeMesas; // clientes en mesas esperando por pedido
    Pipe pipeECocinero;  // Pipe escritura cocinero de comidas cocinadas
    Pipe pipeLCocinero;  // Pipe lectura de cocinero de pedidos

    std::list<Mozo*> mozos;
    std::list<Semaforo*> semaforos;
    std::map<int,Semaforo*> semaforosMesas;
    std::list<Recepcionista*> recepcionistas;
    std::list<Mesa*> mesas;

    MemoriaCompartida2<int> caja;
    MemoriaCompartida2<int> cantLiving;
    MemoriaCompartida2<int> dineroNoAbonado;

    Semaforo escrituraLiving;

    GeneradorClientes generadorClientes;

    LockFd lockLecturaClientes;  // lock de lectura de la puerta (pipe clientes)

    void iniciarMozos();

    void iniciarCocinero();

    void iniciarRecepcionistas();

    void iniciarMesas();

    void iniciarGeneradorClientes();

public:
    Restaurante(int recepCant, int mozosCant, int mesasCant, const std::list<std::pair<std::string, int> > &menu);

    void iniciarPersonal();

    void abrirPuertas();

    virtual ~Restaurante();





};


#endif //RESTAURANTCONCURRENTE_RESTAURANTE_H
