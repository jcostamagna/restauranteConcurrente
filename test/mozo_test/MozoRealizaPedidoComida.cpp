//
// Created by nicolas on 17/10/16.
//

#include <gtest/gtest.h>
#include <Mozo.h>

class MozoRealizaPedidoComida : public ::testing::Test {

protected:
    virtual void TearDown() {

    }

    virtual void SetUp() {

    }

public:
    MozoRealizaPedidoComida() : Test() {
        canal = new Pipe();
        mozo = new Mozo(*canal);
    }

    virtual ~MozoRealizaPedidoComida() {
        delete mozo;
    }

    Pipe* canal;
    Mozo* mozo;

};



TEST_F(MozoRealizaPedidoComida, mi_test2){
    mozo->start();

    // escritor
    std::string dato = "comida";
    sleep(5);
    canal->escribir(static_cast<const void *>(dato.c_str()), dato.size());

    std::cout << "Pedido: escribi el dato [" << dato << "] en el pipe" << std::endl;
    std::cout << "Pedido: fin del proceso" << std::endl;

    mozo->stop();

    canal->cerrar();
    EXPECT_EQ(1,1);
}