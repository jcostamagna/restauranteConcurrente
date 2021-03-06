//
// Created by nicolas on 17/10/16.
//
/*
#include <gtest/gtest.h>
#include <list>
#include <Mozo.h>
#include <Cocinero.h>

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

        for (std::list<Semaforo*>::iterator it = semaforosCocineroMozos.begin(); it != semaforosCocineroMozos.end(); ++it){
            delete (*it);
        }
    }

    Pipe* pipeMesas;
    Pipe* pipeECocinero;
    Pipe* pipeLCocinero;

    std::list<Mozo*> mozos;
    std::list<Semaforo*> semaforosCocineroMozos;

};


TEST_F(MozoRealizaPedidoComida, mi_test1) {
    //CREO LOS MOZOS
    static const int CANTMOZOS = 4;
    for(int i=0; i<CANTMOZOS; i++){
        std::string path = "/bin/bash";
        Semaforo *semaforo_ = new Semaforo(path,i);
        Mozo* mozoi = new Mozo(i,*pipeMesas,*pipeECocinero,*pipeLCocinero,*semaforo_);
        mozoi->start();
        mozos.push_back(mozoi);
        semaforosCocineroMozos.push_back(semaforo_);
    }

    static const int CANTPEDIDOS = 4;

    // PEDIDOS
    for(int i=0; i<CANTPEDIDOS; i++){
        std::string dato = "comida      ";
        pipeMesas->escribir(static_cast<const void *>(dato.c_str()), dato.size());
    }

    //COCINERO
    Cocinero* cocinero = new Cocinero(*pipeECocinero,*pipeLCocinero,semaforosCocineroMozos);
    cocinero->start();
}
 */