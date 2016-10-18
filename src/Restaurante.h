//
// Created by ale on 18/10/16.
//

#ifndef RESTAURANTCONCURRENTE_RESTAURANTE_H
#define RESTAURANTCONCURRENTE_RESTAURANTE_H


#include <map>
#include "Cocinero.h"
#include "Recepcionista.h"
#include "Mozo.h"

class Restaurante {

    int recepCant, mozosCant, mesasCant;
    std::map<std::string, int> menu;

    Cocinero *cocinero;
    std::map<pid_t, Recepcionista*> recepcionistas;
    std::map<pid_t, Mozo*> mozos;


    Pipe living;

    void iniciarCocinero();
    void iniciarMozos();
    void iniciarRecepcionistas();

public:
    Restaurante(int recepCant, int mozosCant, int mesasCant, const std::map<std::string, int> &menu);

    void iniciarPersonal();
    void abrirPuertas();



};


#endif //RESTAURANTCONCURRENTE_RESTAURANTE_H
