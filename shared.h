#pragma once

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>

struct file_header{
    char file_name[256];
    ssize_t file_size;
};