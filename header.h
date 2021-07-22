#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <poll.h>

#define SERVERPORTUI 3229
#define SERVERPORTI 3228
#define SERVERPORTU "3229"
#define SERVERPORT "3228"

#define POLL_TIME 60000

#define BACKLOG 3
#define NUM_OF_DOTS 10000000