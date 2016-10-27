//
// Created by nicolas on 20/10/16.
//

#ifndef RESTAURANTCONCURRENTE_MESA_H
#define RESTAURANTCONCURRENTE_MESA_H


#include <Pipe.h>
#include <Semaforo.h>
#include <MemoriaCompartida2.h>
#include <LockFd.h>
#include <list>
#include <vector>
#include "Forkeable.h"

typedef enum ESTADO_MESA {
    ESPERANDO_CLIENTE,
    CLIENTE_SENTADO,
    CLIENTE_ESPERA_PEDIDO,
    CLIENTE_COMIENDO,
    CLIENTE_ESPERA_CUENTA,
    APAGON_MESA
} e_mesa;

class Mesa : public Forkeable {
private:
    Pipe& living;  // clientes en el mismo living del restaurante
    Pipe& pedidos;  // un pipe de pedidos por mesa-cliente

    LockFd& lockLiving;  // Para leer gente del living de a una mesa por vez

    e_mesa estado;
    Semaforo* sEsperandoMozo;  // Mientras el mozo hace cosas me duermo

    int idCliente;  // id del cliente actual sentado en la mesa

    MemoriaCompartida2<int> cantClientesLiving;  // cantidad de clientes en el living compartido
    Semaforo& escrituraLiving;  // Para descontar gente del living cuando los saco


    int cuentaSesion;  // Cuenta de lo pedido por ahora en la sesion
    MemoriaCompartida2<int> dineroPerdido;  // De comidas que se ordenaron pero no se pagaron en el apagon
    Semaforo& semDineroPerdido;

    MemoriaCompartida2<int> cajaResto;
    Semaforo& semCajaRestaurante;

    std::vector<std::pair<std::string, int> > menu;

    Mesa(const Mesa& object);
    Mesa& operator=(const Mesa& object);

    virtual void run() override;
    void rutinaMesa();

    void esperandoCliente();
    void clienteSentado();
    void clienteEsperaPedido();
    void comer();
    void clienteEsperaCuenta();
    void apagonMesa();

    void avanzarEstado();

    int calcularRandom(int max);

    void pedirComida(std::ostringstream &stream);

public:
    Mesa (Pipe& living, Pipe& pedidos, LockFd& lockLiving, Semaforo* sEsperandoMozo, Semaforo& escrituraLiving,
          Semaforo& semCajaRestaurante, Semaforo& semDineroPerdido, std::vector<std::pair<std::string, int> > menu);


};


#endif //RESTAURANTCONCURRENTE_MESA_H
