#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>                                                                                                               
#include <stdlib.h>                                                                                                              
#include <string.h>                                                                                                              
#include <unistd.h>                                                                                                              
#include <sys/socket.h>                                                                                                          
#include <arpa/inet.h>                                                                                                           
#include <netinet/in.h>                                                                                                          
#include <pthread.h>                                                                                                             
#include <sys/types.h>                                                                                                           
#include <sys/stat.h>                                                                                                            
#include <fcntl.h>                                                                                                               
#include <sys/sendfile.h>                                                                                                        
#include <signal.h>
#define SIZE 1024
#define ERR_EXIT(x) do                  \
                {                       \
                    perror(x);        \
                }                       \
                while(0);               \
















