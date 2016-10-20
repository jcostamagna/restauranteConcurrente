//
// Created by juan on 15/10/16.
//
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <SIGINT_Handler.h>
#include <SignalHandler.h>
#include "Recepcionista.h"


Recepcionista::Recepcionista(Pipe &clientes, LockFd& lecturaPuerta, Semaforo& escrituraLiving,Pipe &living, Pipe &clientesAMesa) :
                            clientes(clientes),living(living), clientesAMesa(clientesAMesa), mesaLibre(false),
                                 estado(ESPERANDO), lecturaPuerta(lecturaPuerta), escrituraLiving(escrituraLiving), cantClientesLiving("/bin/bash", 'z'){}

void Recepcionista::run() {

    //static const std::string NOMBRE = "/bin/bash";



    //Creo Memoria Compartida
    //this->cantClientesLiving.crear(NOMBRE, 'z');

    this->rutinaRecepcionista();



    //this->cantClientesLiving.liberar();
    this->clientes.cerrar();
    this->clientesAMesa.cerrar();
    this->living.cerrar();
    std::cout << "Termino el proceso " << getpid() << std::endl;

}

void Recepcionista::rutinaRecepcionista() {
    while (sigint_handler.getGracefulQuit() == 0) {
        switch (estado) {
            case ESPERANDO:
                esperando();
                break;
            case UBICANDO_EN_LIVING:
                ubicandoEnLiving();
                break;
            case UBICANDO_EN_MESA:
                ubicandoEnMesa();
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
        //case ESPERANDO; si estaba ESPERANDO lo define la rutina si va a UBICANDO EN EL LIVING O A UBICANDO EN MESA
        case UBICANDO_EN_LIVING:
            estado = ESPERANDO;
            break;
        case UBICANDO_EN_MESA:
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
    static const int BUFFSIZE = 26;

    // lector
    char buffer[BUFFSIZE];

    std::cout << "Lector: esperando para leer..." << std::endl;


    //Por ahora dejamos con un lock de lectura, tal vez no es necesario o se usa semaforo
    lecturaPuerta.tomarLock();
    ssize_t bytesLeidos = this->clientes.leer(static_cast<void *>(buffer), BUFFSIZE);
    lecturaPuerta.liberarLock();

    if (bytesLeidos <= 0) return;
    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos);

    std::cout << "Lector: lei el dato [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe y soy el hijo "
              << getpid() << std::endl;

    if (mesaLibre) {
        this->clientesAMesa.escribir(static_cast<const void *>(mensaje.c_str()), mensaje.size());
        return;
    }

    this->living.escribir(static_cast<const void *>(mensaje.c_str()), mensaje.size());

    escrituraLiving.p();


    int cantClientes = this->cantClientesLiving.leer();
    std::cout << "Lector: Cantidad clientes[" << cantClientes << "] "<< getpid() << std::endl;
    cantClientes ++;
    this->cantClientesLiving.escribir(cantClientes);

    escrituraLiving.v();
    //std::cout << "Lector: fin del proceso" << std::endl;

}

void Recepcionista::ubicandoEnLiving() {

}

void Recepcionista::ubicandoEnMesa() {

}

void Recepcionista::apagon() {

}
