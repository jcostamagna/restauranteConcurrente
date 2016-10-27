//
// Created by ale on 27/10/16.
//

#include "Gerente.h"
#include "Log.h"
#include "lockFiles.h"

Gerente::Gerente(Semaforo &semClientesLiving, Semaforo &semCajaRestaurante, Semaforo &semDineroPerdido) :
        semClientesLiving(semClientesLiving), cantClientesLiving(SM_CLIENTES_LIVING_FILE, SM_CLIENTES_LIVING_LETRA),
        semCajaRestaurante(semCajaRestaurante), cajaRestaurante(SM_CAJA_FILE, SM_CAJA_LETRA),
        dineroPerdido(SM_DINERO_PERDIDO_FILE, SM_DINERO_PERDIDO_LETRA), semDineroPerdido(semDineroPerdido) {}


Gerente::~Gerente() {

}

void Gerente::run() {
    rutinaGerente();

    std::cout << "Termino el proceso gerente " << getpid() << std::endl;
}

void Gerente::rutinaGerente() {
    while (sigint_handler.getGracefulQuit() == 0) {

        consultarDatos();
        sleep(5);
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

    ss << "\t\t Cantidad de dinero en la caja del restaurante: \t" << cajaRestaurante.leer() << " pesos" << std::endl;

    semCajaRestaurante.v();
}

void Gerente::consultarDineroPerdido(std::stringstream &ss) {
    semDineroPerdido.p();

    ss << "\t\t Cantidad de dinero perdido por cortes de luz: \t" << dineroPerdido.leer() << " pesos" << std::endl;

    semDineroPerdido.v();
}