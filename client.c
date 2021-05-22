#include "header.h"
#include "int.cpp"
#include "listen_udp.c"

typedef struct thread_data {
      int socket;
      int thread_num;
      char func[20];
      double left_x;
      double right_x;
} thread_data;

int main() {
      int sockfd, numbytes;
      struct addrinfo hints, *servinfo, *p;
      int rv;
      char s[INET_ADDRSTRLEN];

      char* addr = get_serv_addr();
      // printf("%lu, %s", sizeof(addr), addr);
      char a[15];
      a[0] = '\0';
      for (int i = 0; i < 14; ++i) {
            a[i+1] = addr[i];
            // printf("%c", a[i]);
      }

      memset(&hints, 0, sizeof hints);
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;

      if ((rv = getaddrinfo(a, SERVERPORT, &hints, &servinfo)) == -1) {
            fprintf(stderr, "client: getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
      }

      sleep(1);
      for (p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                  perror("client: socket");
                  continue;
            }
            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                  close(sockfd);
                  perror("client: connect");
                  continue;
            }
            break;
      }

      if (p == NULL) {
            fprintf(stderr, "client: failed to connect\n");
            return 2;
      }
      freeaddrinfo(servinfo);

      thread_data* rec_data = (thread_data*) calloc(1, sizeof(thread_data));
      memset(rec_data, '\0', sizeof(thread_data));
      if ((numbytes = recv(sockfd, rec_data, sizeof(thread_data), 0)) == -1) {
            perror("client: recv");
            exit(1);
      }
      free(rec_data);

      // printf("%d\n", numbytes);
      // printf("Thread num: %i\n", rec_data->thread_num);
      // printf("%s, %lf, %lf", rec_data->func, rec_data->left_x, rec_data->right_x);

      double num_dots = count_int(NUM_OF_DOTS, rec_data->left_x, rec_data->right_x, rec_data->func);
      printf("Res: %lf\n", num_dots);
      if (send(sockfd, &num_dots, sizeof(double), 0) == -1) {
            perror("client: send");
            exit(1);
      }
      close(sockfd);

      return 0;   
}