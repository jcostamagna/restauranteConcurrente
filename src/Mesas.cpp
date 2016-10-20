//
// Created by nicolas on 20/10/16.
//

#include "Mesas.h"

Mesas::Mesas (Pipe& pLiving, Pipe& pMesas, int cantidadMesas) :
        pLiving(pLiving), pMesas(pMesas), lockMesas(pMesas.getFdLectura()), cantidadMesas(cantidadMesas) {
    for (int i = 0; i < cantidadMesas; i++) {
        std::string path = "/bin/grep";
        Semaforo *semaforo = new Semaforo(path,i);
        Mesa* mesai = new Mesa(pLiving,pClienteAMesa,lockMesas,*semaforo);
        mesai->start();
        mesas.push_back(mesai);
        semaforos.push_back(semaforo);
    }
}


