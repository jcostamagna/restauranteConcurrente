//
// Created by ale on 18/10/16.
//

#include <sstream>
#include <iomanip>
#include "Restaurante.h"
#include "Log.h"
#include "lockFiles.h"

Restaurante::Restaurante(int recepCant, int mozosCant, int mesasCant, int clientesCant, std::vector<std::pair<std::string, int>> menu)
        : recepCant(recepCant), mozosCant(mozosCant), mesasCant(mesasCant), clientesCant(clientesCant), menu(menu),
          caja(SM_CAJA_FILE, SM_CAJA_LETRA), semCajaRestaurante(SEM_CAJA_FILE,SEM_CAJA_LETRA,0),
          cantClientesLiving(SM_CLIENTES_LIVING_FILE, SM_CLIENTES_LIVING_LETRA),
          escrituraLiving(SEM_CLIENTES_LIVING_FILE, SEM_CLIENTES_LIVING_LETRA, 0),
          dineroPerdido(SM_DINERO_PERDIDO_FILE, SM_DINERO_PERDIDO_LETRA),
          semDineroPerdido("/bin/cp",'k',0) , generadorClientes(puerta, clientesCant),
          lockLecturaClientes(puerta.getFdLectura()), lockLecturaLiving(living.getFdLectura()),
          lockLecturaMesas(pipePedidosMesas.getFdLectura()), limpiador(NULL) {}

void Restaurante::iniciarPersonal() {
    caja.escribir(0);
    semCajaRestaurante.v();

    dineroPerdido.escribir(0);
    semDineroPerdido.v();

    cantClientesLiving.escribir(0);
    escrituraLiving.v();

    iniciarGerente();
    iniciarMesas();
    iniciarMozos();
    iniciarCocinero();
    iniciarRecepcionistas();
    iniciarGeneradorClientes();

}

void Restaurante::iniciarGeneradorClientes(){
    this->generadorClientes.start();
}


void Restaurante::iniciarMesas() {
    for (int i = 0; i < mesasCant; i++){
        std::string path = "/bin/grep";
        char sem = (char)'a'+i;
        Semaforo *semParaMozos = new Semaforo(path, sem,0);
        Mesa* mesa = new Mesa(this->living, this->pipePedidosMesas, this->lockLecturaLiving, semParaMozos,  this->escrituraLiving, semCajaRestaurante, semDineroPerdido, menu);
        mesa->start();
        //this->semaforosMesas[mesa->get_pid()] = semaforoConCocinero;
        this->semaforosMesas.insert(std::make_pair(mesa->get_pid(), semParaMozos));
        this->mesas.push_back(mesa);
    }
}

void Restaurante::iniciarMozos() {
    for (int i = 0; i < mozosCant; i++) {
        std::string path = "/bin/chmod";
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

}

void Restaurante::iniciarGerente() {
    gerente = new Gerente(escrituraLiving,semCajaRestaurante,semDineroPerdido);
    gerente->start();
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

    for (std::list<Mesa*>::iterator it = mesas.begin(); it != mesas.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    for (std::map<int, Semaforo*>::iterator it = semaforosMesas.begin(); it != semaforosMesas.end(); ++it){
        (*it).second->eliminar();
        delete (*it).second;
    }

    for (std::list<Recepcionista*>::iterator it = recepcionistas.begin(); it != recepcionistas.end(); ++it){
        kill((*it)->get_pid(), SIGINT);
        (*it)->stop();
        delete (*it);
    }

    kill(this->cocinero->get_pid(), SIGINT);
    this->cocinero->stop();

    kill(this->gerente->get_pid(), SIGINT);
    this->gerente->stop();


    kill(this->generadorClientes.get_pid(), SIGINT);
    this->generadorClientes.stop();
    escrituraLiving.eliminar();
    semDineroPerdido.eliminar();
    semCajaRestaurante.eliminar();
    //semaforosCocineroMozos eliminarlos!!
}

void Restaurante::apagonRestaurante() {
    vaciar_living();
    limpiar_mesas();

    for (std::list<Mesa*>::iterator it = mesas.begin(); it != mesas.end(); ++it){
        kill((*it)->get_pid(),SIGCONT);
    }

    for (std::list<Mozo*>::iterator it = mozos.begin(); it != mozos.end(); ++it){
        kill((*it)->get_pid(),SIGCONT);
    }

    kill(cocinero->get_pid(),SIGCONT);

    kill(generadorClientes.get_pid(),SIGCONT);


}

void Restaurante::vaciar_living() {
    char buffer[BUFFSIZE];

    escrituraLiving.p();
    this->lockLecturaLiving.tomarLock();
    int cantClientes = this->cantClientesLiving.leer();

    std::stringstream ss;
    ss.str("");
    ss << "APAGON - LIVING: Cantidad de clientes al momento del apagon: " << cantClientes << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "APAGON - LIVING: Cantidad de clientes al momento del apagon: " << cantClientes << std::endl;

    for(int i=0; i<cantClientes; i++) {
        ssize_t bytesLeidos = this->living.leer(static_cast<void *>(buffer), BUFFSIZE);

        if (bytesLeidos <= 0) return;
        std::string mensaje = buffer;
        mensaje.resize(bytesLeidos);
        std::string::size_type sz;

        std::stringstream ss1;
        int idCliente;
        for (unsigned int i = 0; i < PID_LENGHT && i < mensaje.size(); ++i) {
            ss1 << mensaje.at(i);
        }
        ss1 >> idCliente;

        ss.str("");
        ss << "APAGON - LIVING: El cliente [" << idCliente << "]  se va!" << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "APAGON - LIVING: El cliente [" << idCliente << "]  se va!" << std::endl;
    }

    this->cantClientesLiving.escribir(0);
    this->lockLecturaLiving.liberarLock();
    escrituraLiving.v();
}

void Restaurante::limpiar_mesas() {
    if (limpiador)
        delete limpiador;

    limpiador = new LimpiadorMesas(pipePedidosMesas,lockLecturaMesas);
    limpiador->start();

    std::stringstream stream;
    stream << std::setfill(LIMPIAR_PEDIDOS) << std::setw(BUFFSIZE) << LIMPIAR_PEDIDOS;

    pipePedidosMesas.escribir(stream.str().c_str(), BUFFSIZE);


}
