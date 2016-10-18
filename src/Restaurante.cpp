//
// Created by ale on 18/10/16.
//

#include "Restaurante.h"
#include "Cocinero.h"

Restaurante::Restaurante(int recepCant, int mozosCant, int mesasCant, const std::map<std::string, int> &menu)
        : recepCant(recepCant), mozosCant(mozosCant), mesasCant(mesasCant), menu(menu), living(),
          caja("CMakeCache.txt", 'A'), cantLiving("Makefile", 'A') {}

void Restaurante::iniciarPersonal() {
    iniciarCocinero();
    iniciarMozos();
    iniciarRecepcionistas();
}

void Restaurante::abrirPuertas() {

}

void Restaurante::iniciarCocinero() {
    cocinero = new Cocinero();
    cocinero->start();
}

void Restaurante::iniciarMozos() {
    for (int i = 0; i < mozosCant; i++) {
        Mozo *mozo = new Mozo();
        mozo->start();
        mozos.insert(std::make_pair(mozo->get_pid(), mozo));
    }
}

void Restaurante::iniciarRecepcionistas() {
    for (int i = 0; i < recepCant; i++) {
        Recepcionista *recepcionista = new Recepcionista(living);
        recepcionista->start();
        recepcionistas.insert(std::make_pair(recepcionista->get_pid(), recepcionista));
    }
}


