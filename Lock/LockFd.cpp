//
// Created by juan on 18/10/16.
//

#include "LockFd.h"

#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>

LockFd :: LockFd ( int fd ) {

    this->fl.l_type = F_RDLCK;
    this->fl.l_whence = SEEK_SET;
    this->fl.l_start = 0;
    this->fl.l_len = 0;
    //this->fd = open ( this->nombre.c_str(),O_CREAT|O_WRONLY,0777 );
    this->fd = fd;
    if (fd <= 0){
        std::string mensaje = std::string("Error en open() del Lock (crear): ") + std::string(strerror(errno));
        std::cerr<<mensaje<<std::endl;
        throw mensaje;
    }
}

int LockFd :: tomarLock () {
    this->fl.l_type = F_RDLCK;
    return fcntl ( this->fd,F_SETLKW,&(this->fl) );
}

int LockFd :: liberarLock () {
    this->fl.l_type = F_UNLCK;
    return fcntl ( this->fd,F_SETLK,&(this->fl) );
}

ssize_t LockFd :: escribir ( const void* buffer,const ssize_t buffsize ) const {
    lseek ( this->fd,0,SEEK_END );
    return write ( this->fd,buffer,buffsize );
}

LockFd :: ~LockFd () {
    //close ( this->fd );
}
