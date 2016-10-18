//
// Created by nicolas on 17/10/16.
//

#include <gtest/gtest.h>
#include <list>
#include <Mozo.h>

class MozoRealizaPedidoComida : public ::testing::Test {

protected:
    virtual void TearDown() {

    }

    virtual void SetUp() {
    }

public:
    MozoRealizaPedidoComida() : Test() {
        pipeMesas = new Pipe();
        pipeECocinero = new Pipe();
        pipeLCocinero = new Pipe();
    }

    virtual ~MozoRealizaPedidoComida() {
        for (std::list<Mozo*>::iterator it = mozos.begin(); it != mozos.end(); ++it){
            kill((*it)->get_pid(), SIGINT);
            (*it)->stop();
            delete (*it);
        }

        for (std::list<Semaforo*>::iterator it = semaforos.begin(); it != semaforos.end(); ++it){
            delete (*it);
        }
    }

    Pipe* pipeMesas;
    Pipe* pipeECocinero;
    Pipe* pipeLCocinero;

    std::list<Mozo*> mozos;
    std::list<Semaforo*> semaforos;

};


/*
TEST_F(MozoRealizaPedidoComida, mi_test2){
    mozo->start();

    // escritor
    std::string dato = "comida";
    sleep(5);
    pipeMesas->escribir(static_cast<const void *>(dato.c_str()), dato.size());

    std::cout << "Pedido: escribi el dato [" << dato << "] en el pipe" << std::endl;
    std::cout << "Pedido: fin del proceso" << std::endl;

    mozo->stop();

    pipeMesas->cerrar();
    EXPECT_EQ(1,1);
}*/

TEST_F(MozoRealizaPedidoComida, mi_test3) {
    //CREO LOS MOZOS
    static const int CANTMOZOS = 4;
    for(int i=0; i<CANTMOZOS; i++){
        std::string path = "main" + std::to_string(i) + ".cc";
        Semaforo *semaforo_ = new Semaforo(path,0);
        Mozo* mozoi = new Mozo(i,*pipeMesas,*pipeECocinero,*pipeLCocinero,*semaforo_);
        mozoi->start();
        mozos.push_back(mozoi);
        semaforos.push_back(semaforo_);
    }

    static const int BUFFSIZE = 12;
    char buffer[BUFFSIZE];

    static const int CANTPEDIDOS = 4;

    // PEDIDOS
    for(int i=0; i<CANTPEDIDOS; i++){
        std::string dato = "comida      ";
        pipeMesas->escribir(static_cast<const void *>(dato.c_str()), dato.size());
    }

    //COCINERO
    for(int i=0; i<CANTPEDIDOS; i++) {
        std::cout << "Cocinero: esperando para leer..." << std::endl;
        ssize_t bytesLeidos = pipeECocinero->leer ( static_cast<void*>(buffer),BUFFSIZE);
        std::string mensaje = buffer;
        mensaje.resize ( bytesLeidos );
        std::cout << "Cocinero: Leo al mozo ->" << mensaje << "<-" << std::endl;

        int N = std::stoi(mensaje);
        std::cout << "Cocinero: Pongo en verde el semaforo ->" << N << "<-" << std::endl;
        if (semaforos.size() > (unsigned)N)
        {
            std::list<Semaforo*>::iterator it = semaforos.begin();
            std::advance(it, N);
            (*it)->v();
        }

        std::string dato = "Pedido listo";
        std::cout << "Cocinero: Escribo en mozo: " << dato << std::endl;
        pipeLCocinero->escribir(static_cast<const void *>(dato.c_str()), dato.size());
    }
}