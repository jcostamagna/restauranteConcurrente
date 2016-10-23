//
// Created by ale on 15/10/16.
//

#include "Log.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <chrono>
#include <sys/time.h>

Log *Log::instance = NULL;
LockFile *Log::lockFile = NULL;

Log::Log() {}


Log *Log::getInstance() {

    if (instance == NULL)
        instance = new Log();

    return instance;
}

void Log::destruir() {
    if (instance != NULL) {
        delete lockFile;
        delete (instance);
        instance = NULL;
    }
}

bool Log::setearArchivo(const char *archivo) {
    lockFile = new LockFile(archivo); //abre el archivo y setea el lock
    if (lockFile)
        return true;
    return false;
}

void Log::log(std::string msg) {
    log(msg.c_str());
}

void Log::log(const char *msg) {
    std::stringstream ss;

    char buf[64];
    setActualTime(buf);

    ss << std::string(buf) << "\t";
    ss << "[" << getpid() << "]" << "\t\t";
    ss << msg << std::endl;

    lockFile->tomarLock();
    lockFile->escribir(ss.str().c_str(), ss.str().size());
    lockFile->liberarLock();
}

void Log::setActualTime(char *buf) {
    struct timeval tv;
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64];

    gettimeofday(&tv, NULL);
    nowtime = tv.tv_sec;
    nowtm = localtime(&nowtime);
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
    snprintf(buf, sizeof buf, "%s.%06ld", tmbuf, tv.tv_usec);
}


