//
// Created by ale on 31/08/16.
//

#include <gtest/gtest.h>
#include <Recepcionista.h>

class RecepcionistaFixture : public ::testing::Test {

protected:
    virtual void TearDown() {

    }

    virtual void SetUp() {

    }

public:
    RecepcionistaFixture() : Test() {
        canal = new Pipe();
        //recepcionista = new Recepcionista(*pipeMesas);
    }

    virtual ~RecepcionistaFixture() {
        //delete recepcionista;
        delete canal;
    }

    Pipe* canal;
    //Recepcionista* recepcionista;

};


TEST_F(RecepcionistaFixture, mi_test){
    /*recepcionista->start();

    // escritor
    std::string dato = std::string("Numero 9");
    sleep(5);
    pipeMesas->escribir(static_cast<const void *>(dato.c_str()), dato.size());

    std::cout << "Escritor: escribi el dato [" << dato << "] en el pipe" << std::endl;
    std::cout << "Escritor: fin del proceso" << std::endl;

    // espero a que termine el hijo
    //wait ( NULL );
    kill(recepcionista->get_pid(), SIGINT);
    recepcionista->stop();
    */
    canal->cerrar();
    EXPECT_EQ(1,1);
}