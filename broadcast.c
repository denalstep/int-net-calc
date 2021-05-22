#include "header.h"
#include <ifaddrs.h>

void broadcast() {
    int sockfd;
    struct sockaddr_in addresses;
    struct hostent *he;
    int broadcast = 1;
    int numbytes;

    if ((he = gethostbyname("255.255.255.255")) == NULL) {
        perror("broadcast: gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("broadcast: socket");
        exit(1);
    }

    if ((setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast)) == -1) {
        perror("broadcast: setsockopt (SO_BROADCAST)");
        exit(1);
    }

    addresses.sin_family = AF_INET;
    addresses.sin_port = htons(SERVERPORTUI);
    addresses.sin_addr = *((struct in_addr*)he->h_addr);
    memset(addresses.sin_zero, '\0', sizeof addresses.sin_zero);

    char msg[20];
    char type[4];
    struct ifaddrs* server_data;
    getifaddrs(&server_data);
    while (server_data) {
        if (server_data->ifa_addr && server_data->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in* addr = (struct sockaddr_in *)server_data->ifa_addr;
            sprintf(type, "%s", server_data->ifa_name);
            if (type[0] == 'e' && type[2] == '0') {
                sprintf(msg, "%s", inet_ntoa(addr->sin_addr));
            }
        }
        server_data = server_data->ifa_next;
    }

    // printf("%s", msg);

    if ((numbytes = sendto(sockfd, &msg, sizeof msg, 0, (struct sockaddr*)&addresses, sizeof addresses)) == -1) {
        perror("broadcast: sendto");
        exit(1);
    }  

    printf("sent broadcast message to %s\n", inet_ntoa(addresses.sin_addr));

    close(sockfd);
}