//
// Created by ale on 17/10/16.
//

#ifndef RESTAURANTCONCURRENTE_GRUPOCOMENSALES_H
#define RESTAURANTCONCURRENTE_GRUPOCOMENSALES_H

#include "Forkeable.h"

typedef enum ESTADO_GRUPO {
    ESPERANDO_EN_PUERTA,
    ESPERANDO_EN_LIVING,
    ESPERANDO_ORDENAR,
    ESPERANDO_MOZO_COMIDA,
    COMIENDO,
    ESPERANDO_PAGAR,  // espera que haya un mozo disponible para pagar
    PAGAR_RETIRARSE,
    APAGON
} e_grupo;

class GrupoComensales: public Forkeable {
private:
    bool vive;
    e_grupo estado;
    int cantidad;

    virtual void run() override;
    void rutinaComensal();
    void esperandoEnPuerta();
    void esperandoEnLiving();
    void esperandoOrdenar();
    void esperandoComida();
    void comiendo();
    void esperandoPagar();
    void pagarRetirarse();
    void apagon();

    void avanzarEstado();

    int calcularRandom (int tope);

public:
    GrupoComensales();

    virtual ~GrupoComensales();
    void sumarCantidad();
    int getCantidad();


};


#endif //RESTAURANTCONCURRENTE_GRUPOCOMENSALES_H
