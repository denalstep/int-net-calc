#include "header.h"

double random_num(double min, double max) {
      double range = max - min;
      double div = RAND_MAX / range;
      double rn = min + ((double) rand() / div);
      return rn;
}

double func(double x) {
      return x*x*x;
}

int count_dots(int n, double x0, double x1, double y0, double y1) {
      srand(time(NULL));
      if (x0 >= x1 || y0 >= y1) {
            perror("client: invalid domain");
            exit(1);
      }
      int counter = 0;
      for (int i = 0; i < n; i++) {
            double x = random_num(x0,x1);
            double y = random_num(y0,y1);
            int flag = (y <= func(x));
            //printf("%d: (%lf,%lf) ---- %lf ---- %d\n", i+1, x, y, func(x), flag);
            if (flag) {
                  counter++;
            }
      }
      return counter;
}

