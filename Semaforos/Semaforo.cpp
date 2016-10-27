#include "Semaforo.h"
#include <string.h>
#include <iostream>



Semaforo :: Semaforo ( const std::string& nombre,const char character,const int valorInicial ):valorInicial(valorInicial) {
	key_t clave = ftok ( nombre.c_str(),character );
    if (clave <= 0){
        std::string mensaje = std::string("Error en ftok() (crear) Semaforo: ") + std::string(strerror(errno));
        std::cerr<<mensaje<<std::endl;
        throw mensaje;
    }
	this->id = semget ( clave,1,0666 | IPC_CREAT );
	if (this->id < 0){
		std::string mensaje = std::string("Error en semget() (crear): ") + std::string(strerror(errno));
		std::cerr<<mensaje<<std::endl;
		throw mensaje;
	}
	this->inicializar ();
}

Semaforo::~Semaforo() {
}

int Semaforo :: inicializar () const {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = this->valorInicial;
	int resultado = semctl ( this->id,0,SETVAL,init );
	if (resultado < 0){
		std::string mensaje = std::string("Error en semctl(): ") + std::string(strerror(errno));
		std::cerr<<mensaje<<std::endl;
		throw mensaje;
	}
	return resultado;
}

int Semaforo :: p () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = -1;	// restar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	if (resultado < 0){
        int errsv = errno;
		std::string mensaje = std::string("Error en semop() (p): ") + std::string(strerror(errno));
		std::cerr<<mensaje<<std::endl;

        if (errsv == EINTR)
            throw APAGON_MATA_SEMAFORO;
        throw mensaje;
	}
	return resultado;
}

int Semaforo :: v () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = 1;	// sumar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	if (resultado < 0){
		std::string mensaje = std::string("Error en semop() (v): ") + std::string(strerror(errno));
		std::cerr<<mensaje<<std::endl;
		throw mensaje;
	}
	return resultado;
}

void Semaforo :: eliminar () const {
	if (semctl ( this->id,0,IPC_RMID ) < 0){
		std::string mensaje = std::string("Error en semctl() (eliminar): ") + std::string(strerror(errno));
		std::cerr<<mensaje<<std::endl;
		throw mensaje;
	}
}
