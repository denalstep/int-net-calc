#include "header.h"
#include "broadcast.c"

typedef struct thread_data {
      int socket;
      int thread_num;
      char func[20];
      double left_x;
      double right_x;
} thread_data;

void* thread_socket(void* arg) {
      thread_data* data = (thread_data*) arg;
      int n = 0;
      // printf("%d, %lf, %lf\n", data->thread_num, data->left_x, data->right_x);

      n = send(data->socket, data, sizeof(thread_data), 0);
      if (n != sizeof(thread_data)) {
            perror("server: send");
            exit(1);
      }
      // printf("%d\n", n);
      // printf("%lu\n", sizeof(thread_data));
      double* num_dots = (double*) calloc(1, sizeof(double));

      struct pollfd pfds;
      pfds.fd = data->socket;
      pfds.events = POLLIN;
      int num_events = poll(&pfds, 1, POLL_TIME);
      if (num_events == 0) {
            printf("server: poll timed out\n");
      }
      else {
            int pollin_happened = pfds.revents & POLLIN;
            if (pollin_happened) {
                  recv(data->socket, num_dots, sizeof(double), 0);
            }
            else {
                  printf("server: poll, unexpected event occured %d\n", pfds.revents);
            }
      }
      close(data->socket);
      pthread_exit(num_dots);
}

int main() {
      thread_data data;
      printf("Enter your function: ");
      scanf("%s", data.func);
      printf("Enter integration segment (x0, x1): ");
      scanf("%lf %lf", &data.left_x, &data.right_x);

      int sockfd, new_sockfd; 
      struct addrinfo hints, *servinfo, *p;
      struct sockaddr_storage clients_addr;
      socklen_t sin_size;
      struct sigaction sa;
      int yes = 1;
      char s[INET_ADDRSTRLEN];
      int rv;     

      memset(&hints, 0, sizeof hints);
      hints.ai_family = AF_INET;  
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = AI_PASSIVE;

      if ((rv = getaddrinfo(NULL, SERVERPORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "server: getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
      }

      broadcast();

      for (p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                  perror("server: socket");
                  continue;
            }
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                  perror("server: setsockopt");
                  exit(1);
            }
            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                  close(sockfd);
                  perror("server: bind");
                  continue;
            }
            break;
      }
      freeaddrinfo(servinfo);
      
      if (p == NULL) {
            fprintf(stderr, "server: failed to bind\n");
            exit(1);
      }
      
      if (listen(sockfd, BACKLOG) == -1) {
            perror("server: listen");
            exit(1);
      }

      // waiting for connections
      int i = 0;
      pthread_t tid[BACKLOG];
      double result = 0;
      while (1) {
            sin_size = sizeof clients_addr;
            data.thread_num = i;
            // printf("i: %i\n", i);
            data.socket = accept(sockfd, (struct sockaddr*)&clients_addr, &sin_size);
            if (data.socket == -1) {
                  perror("server: accept");
                  continue;
            }
            if (pthread_create(&tid[i++], NULL, thread_socket, &data) != 0) {
                  fprintf(stderr, "server: pthread_create\n");
            }
            if (i == BACKLOG) {
                  i = 0;
                  while (i < BACKLOG) {
                        double* temp;
                        pthread_join(tid[i++], (void**)&temp);
                        result += *temp;
                        free(temp);
                  }
                  break;
            }
      }

      printf("Result is: %lf\n", (result / BACKLOG));

      return 0;
}