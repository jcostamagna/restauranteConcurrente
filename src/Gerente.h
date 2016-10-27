//
// Created by ale on 27/10/16.
//

#ifndef RESTAURANTCONCURRENTE_GERENTE_H
#define RESTAURANTCONCURRENTE_GERENTE_H


#include <Semaforo.h>
#include <MemoriaCompartida2.h>
#include "Forkeable.h"

/*
 * Periodicamente, el Gerente del restaurant consulta:
 * a) La cantidad de dinero en la caja.
 * b) La cantidad de dinero que el restaurant perdio de facturar debido a los cortes electricos.
 * c) La cantidad de grupos de personas esperando en el living.
 */


class Gerente: public Forkeable {
    Semaforo& semClientesLiving;  // Para descontar gente del living cuando los saco
    MemoriaCompartida2<int> cantClientesLiving;  // cantidad de clientes en el living compartido

    MemoriaCompartida2<int> cajaRestaurante;
    Semaforo& semCajaRestaurante;

    MemoriaCompartida2<int> dineroPerdido;  // De comidas que se ordenaron pero no se pagaron
    Semaforo& semDineroPerdido;

    void consultarDatos();
    void consultarClientesLiving(std::stringstream &ss);
    void consultarCajaResto(std::stringstream &ss);
    void consultarDineroPerdido(std::stringstream &ss);

    virtual void run() override;
    void rutinaGerente();

public:
    Gerente(Semaforo &semClientesLiving, const MemoriaCompartida2<int> &cantClientesLiving,
            const MemoriaCompartida2<int> &cajaRestaurante, Semaforo &semCajaRestaurante,
            const MemoriaCompartida2<int> &dineroPerdido, Semaforo &semDineroPerdido);

    virtual ~Gerente();
};


#endif //RESTAURANTCONCURRENTE_GERENTE_H
