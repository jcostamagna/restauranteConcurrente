//
// Created by ale on 15/10/16.
//

#include "Log.h"

#include <iostream>
#include <sstream>
#include <cstring>

Log* Log:: instance = NULL;
LockFile* Log:: lockFile = NULL;

Log::Log() {}


Log* Log::getInstance () {

    if ( instance == NULL )
        instance = new Log ();

    return instance;
}

void Log::destruir() {
    if ( instance != NULL ) {
        delete lockFile;
        delete ( instance );
        instance = NULL;
    }
}

bool Log::setearArchivo(const char *archivo) {
    lockFile = new LockFile(archivo); //abre el archivo y setea el lock
    if (lockFile)
        return true;
    return false;
}

void Log::log(std::string &msg) {
    log(msg.c_str());
}

void Log::log(const char *msg) {
    std::stringstream ss;
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char* time = asctime(timeinfo);
    time[strlen(time) - 1] = 0; //le saco el \n

    ss << std::string(time) << "\t";
    ss << "[" << getpid() << "]" << "\t\t";
    ss << msg << std::endl;

    lockFile->tomarLock();
    lockFile->escribir(ss.str().c_str(),ss.str().size());
    lockFile->liberarLock();
}
