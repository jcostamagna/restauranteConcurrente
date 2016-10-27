//
// Created by juan on 15/10/16.
//
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <SIGINT_Handler.h>
#include "Recepcionista.h"
#include "lockFiles.h"
#include <Log.h>


Recepcionista::Recepcionista(Pipe &clientes, LockFd& lecturaPuerta, Semaforo& escrituraLiving,Pipe &living) :
                            puerta(clientes),living(living), mesaLibre(false), estado(ESPERANDO),
                            lecturaPuerta(lecturaPuerta), escrituraLiving(escrituraLiving),
                            cantClientesLiving(SM_CLIENTES_LIVING_FILE, SM_CLIENTES_LIVING_LETRA){}

void Recepcionista::run() {

    this->rutinaRecepcionista();

    this->puerta.cerrar();
    this->living.cerrar();
    std::stringstream ss;
    ss << "Termino el proceso recepcionista " << getpid() << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Termino el proceso recepcionista " << getpid() << std::endl;
}

void Recepcionista::rutinaRecepcionista() {
    while (sigint_handler.getGracefulQuit() == 0) {
        switch (estado) {
            case ESPERANDO:
                esperando();
                break;
            case APAGON_RECEPCIONISTA:
                apagon();
                break;
            default:
                esperando();
                break;
        }
    }
}

void Recepcionista::avanzarEstado() {
    switch (estado) {
        case ESPERANDO:
            estado = UBICANDO_EN_LIVING;
            break;
        case UBICANDO_EN_LIVING:
            estado = ESPERANDO;
            break;
        case APAGON_RECEPCIONISTA:
            estado = ESPERANDO;
            break;
        default:
            estado = ESPERANDO;
            break;
    }
}

void Recepcionista::esperando() {
    //const int BUFFSIZE = 8;

    // lector
    char buffer[BUFFSIZE];

    std::stringstream ss;
    ss << "Recepcionista: Esperando clientes..." << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Recepcionista: Esperando clientes..." << std::endl;
    ss.str("");


    //Leemos con un lock de lectura
    lecturaPuerta.tomarLock();
    ssize_t bytesLeidos = this->puerta.leer(static_cast<void *>(buffer), BUFFSIZE);
    lecturaPuerta.liberarLock();

    if (bytesLeidos <= 0) return; // vuelve a esperando()
    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos);

    std::cout << "Recepcionista: LLego el cliente [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe puerta"
              << std::endl;
    ss << "Recepcionista: LLego el cliente [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe puerta"
          << std::endl;
    Log::getInstance()->log(ss.str());
    ss.str("");


    escrituraLiving.p(); //semaforo para que la cantidad de gente en el living sea siempre valida (nadie puede sumar ni restar)
    std::cout << "Recepcionista: LLevo al cliente [" << mensaje << "] " << "al living"
              << std::endl;
    ss << "Recepcionista: Llevo al cliente [" << mensaje << "] " << "al living"
       << std::endl;
    Log::getInstance()->log(ss.str());
    ss.str("");

    //LLevo clientes al living, o mesa si hay disponible
    this->living.escribir(static_cast<const void *>(mensaje.c_str()), mensaje.size());

    // aumento en 1 la cantidad de clientes en el living
    int cantClientes = this->cantClientesLiving.leer();
    cantClientes ++;
    ss << "Recepcionista: Cantidad clientes en living [" << cantClientes << "] "<< std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Recepcionista: Cantidad clientes en living [" << cantClientes << "] "<< std::endl;

    ss.str("");
    this->cantClientesLiving.escribir(cantClientes);

    escrituraLiving.v();
}

void Recepcionista::apagon() {

}
