//
// Created by ale on 18/10/16.
//

#include "Restaurante.h"
#include "Cocinero.h"
#include "Mesa.h"

Restaurante::Restaurante(int recepCant, int mozosCant, int mesasCant, const std::list<std::pair<std::string, int> > &menu)
        : recepCant(recepCant), mozosCant(mozosCant), mesasCant(mesasCant), menu(menu),
          caja("CMakeCache.txt", 'A'), cantLiving("/bin/bash", 'z'), dineroNoAbonado("Makefile", 'b'),
          escrituraLiving("/bin/bash", 0), generadorClientes(clientes),
          lockLecturaClientes(clientes.getFdLectura()), lockLecturaMesas(living.getFdLectura()) {}

void Restaurante::iniciarPersonal() {
    iniciarMozos();
    iniciarCocinero();
    iniciarRecepcionistas();
    iniciarGeneradorClientes();
    iniciarMesas();
}

void Restaurante::iniciarGeneradorClientes(){
    this->generadorClientes.start();
}

void Restaurante::abrirPuertas() {

}

void Restaurante::iniciarMozos() {
    for (int i = 0; i < mozosCant; i++) {
        std::string path = "/bin/bash";
        Semaforo *semaforo = new Semaforo(path,i);
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

    for (int i = 0; i < recepCant; i++){
        Recepcionista* recepcionista = new Recepcionista(this->clientes,this->lockLecturaClientes, this->escrituraLiving, this->living);
        recepcionista->start();
        this->recepcionistas.push_back(recepcionista);
    }

    //Inicializo en 0
    MemoriaCompartida2<int> cantClientesLiving;
    cantClientesLiving.crear("/bin/bash", 'z');
    cantClientesLiving.escribir(0);

    escrituraLiving.v();
}

void Restaurante::iniciarMesas() {
    for (unsigned i = 0; i < mesasCant; i++) {
        std::string path = "/bin/grep";
        Semaforo *semaforo = new Semaforo(path,i);
        Mesa* mesai = new Mesa(i,living,pipeMesas,lockLecturaMesas,*semaforo);
        mesai->start();
        semaforos.push_back(semaforo);
    }
}


Restaurante::~Restaurante() {
    for (std::list<Mozo*>::iterator it = mozos.begin(); it != mozos.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    for (std::list<Semaforo*>::iterator it = semaforos.begin(); it != semaforos.end(); ++it){
        (*it)->eliminar();
        delete (*it);
    }

    for (std::list<Recepcionista*>::iterator it = recepcionistas.begin(); it != recepcionistas.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    kill(this->generadorClientes.get_pid(), SIGINT);
    this->generadorClientes.stop();
    escrituraLiving.eliminar();
}
