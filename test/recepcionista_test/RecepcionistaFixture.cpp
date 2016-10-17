//
// Created by ale on 31/08/16.
//

#include <gtest/gtest.h>
#include <Recepcionista.h>
#include <Pipe.h>

class RecepcionistaFixture : public ::testing::Test {

protected:
    virtual void TearDown() {

    }

    virtual void SetUp() {

    }

public:
    RecepcionistaFixture() : Test() {
        canal = new Pipe();
        recepcionista = new Recepcionista(*canal);
    }

    virtual ~RecepcionistaFixture() {
        delete recepcionista;
    }

    Pipe* canal;
    Recepcionista* recepcionista;

};


TEST_F(RecepcionistaFixture, mi_test){
    recepcionista->start(); //el proceso hijo quiere leer del canal

    // escritor
    //canal->setearModo(canal->ESCRITURA);
    std::string dato = "Hola mundo pipes!!";
    sleep(5);
    std::cout << "Escritor: holaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;
    canal->escribir(static_cast<const void *>(dato.c_str()), dato.size());

    std::cout << "Escritor: escribi el dato [" << dato << "] en el pipe" << std::endl;
    std::cout << "Escritor: fin del proceso" << std::endl;

    // espero a que termine el hijo
    //wait ( NULL );
    recepcionista->stop();

    canal->cerrar();
    EXPECT_EQ(1,1);
}