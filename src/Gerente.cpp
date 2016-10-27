//
// Created by ale on 27/10/16.
//

#include "Gerente.h"
#include "Log.h"

Gerente::Gerente(Semaforo &semClientesLiving, Semaforo &semCajaRestaurante, Semaforo &semDineroPerdido) :
        semClientesLiving(semClientesLiving), cantClientesLiving("/bin/bash", 'z'), semCajaRestaurante(semCajaRestaurante),
        cajaRestaurante("/bin/cat", 'A'), dineroPerdido("/bin/tar", 'b'), semDineroPerdido(semDineroPerdido) {}


Gerente::~Gerente() {

}

void Gerente::run() {
    rutinaGerente();

    std::cout << "Termino el proceso gerente " << getpid() << std::endl;
}

void Gerente::rutinaGerente() {
    while (sigint_handler.getGracefulQuit() == 0) {
        sleep(5);
        consultarDatos();
    }
}

void Gerente::consultarDatos() {
    std::stringstream ss;
    ss << "Gerente(" << getpid() << ")" << std::endl;

    consultarClientesLiving(ss);
    consultarCajaResto(ss);
    consultarDineroPerdido(ss);

    Log::getInstance()->log(ss.str());
    std::cout << ss.str();
}

void Gerente::consultarClientesLiving(std::stringstream &ss) {
    semClientesLiving.p();

    ss << "\t\t Cantidad de clientes en el living: \t" << cantClientesLiving.leer() << std::endl;

    semClientesLiving.v();
}

void Gerente::consultarCajaResto(std::stringstream &ss) {
    semCajaRestaurante.p();

    ss << "\t\t Cantidad de dinero en la caja del restaurante: \t" << cajaRestaurante.leer() << std::endl;

    semCajaRestaurante.v();
}

void Gerente::consultarDineroPerdido(std::stringstream &ss) {
    semDineroPerdido.p();

    ss << "\t\t Cantidad de dinero perdido por cortes de luz: \t" << dineroPerdido.leer() << std::endl;

    semDineroPerdido.v();
}