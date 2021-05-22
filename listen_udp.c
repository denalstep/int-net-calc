#include "header.h"

char* get_serv_addr() {
        int sockfd; 
        struct addrinfo hints, *servinfo, *p;
        int rv;
        int numbytes;
        struct sockaddr_storage their_addr; 
        char addr_str[20];
        socklen_t addr_len;
        char s[INET_ADDRSTRLEN];
        
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET; 
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE; 
        
        if ((rv = getaddrinfo(NULL, SERVERPORTU, &hints, &servinfo)) != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                exit(1);
        }
        
        for (p = servinfo; p != NULL; p = p->ai_next) {
                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                        perror("listener: socket");
                        continue;
                }
                
                if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                        close(sockfd);
                        perror("listener: bind");
                        continue;
                }
                
                break;
        }
        
        if (p == NULL) {
                fprintf(stderr, "listener: failed to bind socket\n");
                exit(2);
        }
        
        freeaddrinfo(servinfo);
        
        addr_len = sizeof their_addr;

        int yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
                perror("setsockopt");
                exit(1);
        }

        if ((numbytes = recvfrom(sockfd, addr_str, 19, 0, (struct sockaddr*)&their_addr, &addr_len)) == -1) {
                perror("listen_udp: recvfrom");
                exit(1);
        } // receive from server and put its info to serv_info
        
        addr_str[numbytes] = '\0';
        // printf("%lu, %s", sizeof(addr_str), addr_str); // print address of server received
        close(sockfd);
        
        int size = -1;
        for (int i = 0; i < sizeof(addr_str); ++i) {
            if (addr_str[i] != '\0') {
                size++;
            }
        }
        
        char* fin_addr_str = (char*) calloc(size, sizeof(char));
        memcpy(fin_addr_str, addr_str, size * sizeof(char));
        // printf("%lu, %s", sizeof(fin_addr_str), fin_addr_str);
        return fin_addr_str;
}