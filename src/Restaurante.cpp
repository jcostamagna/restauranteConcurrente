//
// Created by ale on 18/10/16.
//

#include "Restaurante.h"

Restaurante::Restaurante(int recepCant, int mozosCant, int mesasCant, int clientesCant, std::vector<std::pair<std::string, int>> menu)
        : recepCant(recepCant), mozosCant(mozosCant), mesasCant(mesasCant), clientesCant(clientesCant), menu(menu),
          caja("CMakeCache.txt", 'A'), cantLiving("CMakeCache.txt", 'h'), dineroNoAbonado("Makefile", 'b'),
          escrituraLiving("CMakeCache.txt", 'z', 0), generadorClientes(puerta, clientesCant),
          lockLecturaClientes(puerta.getFdLectura()), lockLecturaMesas(pipePedidosMesas.getFdLectura()) {}

void Restaurante::iniciarPersonal() {
    iniciarMesas();
    iniciarMozos();
    iniciarCocinero();
    iniciarRecepcionistas();
    iniciarGeneradorClientes();
}

void Restaurante::iniciarGeneradorClientes(){
    this->generadorClientes.start();
}

void Restaurante::abrirPuertas() {
}

void Restaurante::iniciarMozos() {
    for (int i = 0; i < mozosCant; i++) {
        std::string path = "/bin/bash";
        char sem = (char)'a'+i;
        Semaforo *semaforo = new Semaforo(path, sem,0); // attentos, cada uno tiene un semaforoConCocinero distinto
        Mozo* mozoi = new Mozo(i,pipePedidosMesas,lockLecturaMesas,pipeECocinero,*semaforo, this->semaforosMesas);
        mozoi->start();
        mozos.push_back(mozoi);
        semaforosCocineroMozos.push_back(semaforo);
        mozosMap.insert(std::make_pair(mozoi->get_pid(), mozoi));
    }
}

void Restaurante::iniciarCocinero() {
    cocinero = new Cocinero(pipeECocinero,semaforosCocineroMozos);
    cocinero->start();
}

void Restaurante::iniciarRecepcionistas() {

    for (int i = 0; i < recepCant; i++){
        Recepcionista* recepcionista = new Recepcionista(this->puerta,this->lockLecturaClientes, this->escrituraLiving, this->living);
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
    for (int i = 0; i < mesasCant; i++){
        std::string path = "/bin/grep";
        char sem = (char)'a'+i;
        Semaforo *semaforo = new Semaforo(path, sem,0);
        Mesa* mesa = new Mesa(this->living, this->pipePedidosMesas, this->lockLecturaClientes, semaforo,  this->escrituraLiving, menu);
        mesa->start();
        //this->semaforosMesas[mesa->get_pid()] = semaforoConCocinero;
        this->semaforosMesas.insert(std::make_pair(mesa->get_pid(), semaforo));
        this->mesas.push_back(mesa);
    }
}


Restaurante::~Restaurante() {
    for (std::list<Mozo*>::iterator it = mozos.begin(); it != mozos.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    for (std::list<Semaforo*>::iterator it = semaforosCocineroMozos.begin(); it != semaforosCocineroMozos.end(); ++it){
        (*it)->eliminar();
        delete (*it);
    }

    for (std::map<int, Semaforo*>::iterator it = semaforosMesas.begin(); it != semaforosMesas.end(); ++it){
        (*it).second->eliminar();
        delete (*it).second;
    }

    for (std::list<Mesa*>::iterator it = mesas.begin(); it != mesas.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    for (std::list<Recepcionista*>::iterator it = recepcionistas.begin(); it != recepcionistas.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    kill(this->cocinero->get_pid(), SIGINT);
    this->cocinero->stop();


    kill(this->generadorClientes.get_pid(), SIGINT);
    this->generadorClientes.stop();
    escrituraLiving.eliminar();
}

