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

class Restaurante {

    int recepCant, mozosCant, mesasCant;
    //std::map<std::string, int> menu;
    std::list<std::pair<std::string, int> > menu;

    Cocinero *cocinero;
    std::map<pid_t, Mozo *> mozosMap;

    Pipe living;
    Pipe clientes;
    Pipe pipeMesas;
    Pipe pipeECocinero;
    Pipe pipeLCocinero;

    std::list<Mozo*> mozos;
    std::list<Semaforo*> semaforos;
    std::list<Recepcionista*> recepcionistas;

    MemoriaCompartida2<int> caja;
    MemoriaCompartida2<int> cantLiving;
    MemoriaCompartida2<int> dineroNoAbonado;

    Semaforo escrituraLiving;

    GeneradorClientes generadorClientes;

    LockFd lockLecturaClientes;

    void iniciarMozos();

    void iniciarCocinero();

    void iniciarRecepcionistas();

public:
    Restaurante(int recepCant, int mozosCant, int mesasCant, const std::list<std::pair<std::string, int> > &menu);

    void iniciarPersonal();

    void abrirPuertas();

    virtual ~Restaurante();


    void iniciarGeneradorClientes();
};


#endif //RESTAURANTCONCURRENTE_RESTAURANTE_H
