//
// Created by ale on 18/10/16.
//

#include "Restaurante.h"
#include "Cocinero.h"

Restaurante::Restaurante(int recepCant, int mozosCant, int mesasCant, const std::map<std::string, int> &menu)
        : recepCant(recepCant), mozosCant(mozosCant), mesasCant(mesasCant), menu(menu), living(),
          caja("CMakeCache.txt", 'A'), cantLiving("Makefile", 'A') {}

void Restaurante::iniciarPersonal() {
    iniciarMozos();
    iniciarCocinero();
    iniciarRecepcionistas();
}

void Restaurante::abrirPuertas() {

}

void Restaurante::iniciarMozos() {
    for (int i = 0; i < mozosCant; i++) {
        std::string path = "main" + std::to_string(i) + ".cc";
        Semaforo *semaforo = new Semaforo(path,0);
        Mozo* mozoi = new Mozo(i,pipeMesas,pipeECocinero,pipeLCocinero,*semaforo);
        mozoi->start();
        mozos.push_back(mozoi);
        semaforos.push_back(semaforo);
        mozosMap.insert(std::make_pair(mozoi->get_pid(), mozoi));
    }
}

void Restaurante::iniciarCocinero() {
    cocinero = new Cocinero(pipeECocinero,pipeLCocinero,semaforos);
    cocinero->start();
}

void Restaurante::iniciarRecepcionistas() {
/*    for (int i = 0; i < recepCant; i++) {
        Recepcionista *recepcionista = new Recepcionista(living);
        recepcionista->start();
        recepcionistas.insert(std::make_pair(recepcionista->get_pid(), recepcionista));
    }
    */
}


Restaurante::~Restaurante() {
    for (std::list<Mozo*>::iterator it = mozos.begin(); it != mozos.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    for (std::list<Semaforo*>::iterator it = semaforos.begin(); it != semaforos.end(); ++it){
        delete (*it);
    }
}
