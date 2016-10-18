//
// Created by juan on 18/10/16.
//

#ifndef RESTAURANTCONCURRENTE_LOCKFD_H
#define RESTAURANTCONCURRENTE_LOCKFD_H


#include <unistd.h>
#include <fcntl.h>
#include <string>

class LockFd {

private:
    struct flock fl;
    int fd;


    LockFd(const LockFd& object);
    LockFd& operator=(const LockFd& object);

public:
    LockFd ( int fd );
    ~LockFd();

    int tomarLock ();
    int liberarLock ();
    ssize_t escribir ( const void* buffer,const ssize_t buffsize ) const;

};


#endif //RESTAURANTCONCURRENTE_LOCKFD_H
