#include "header.h"
#include "parser.cpp"
#include <iostream>
#include <string>
using std::string;


double random_num(double min, double max) {
      double range = max - min;
      double div = RAND_MAX / range;
      double rn = min + ((double) rand() / div);
      return rn;
}

double min(double x0, double x1, const string& str) {
      string str_post = InfixToPostfix(str);
      double min = 100000000;
      for (double i = x0; i < x1; i += 0.1) {
            double temp = postfixEval(str_post, i);
            min = (temp < min) ? temp : min;
      }
      return min;
}

double max(double x0, double x1, const string& str) {
      string str_post = InfixToPostfix(str);
      double max = -1000000000;
      for (double i = x0; i < x1; i += 0.1) {
            double temp = postfixEval(str_post, i);
            max = (temp > max) ? temp : max;
      }
      return max;
}

double count_int(int n, double x0, double x1, const string& str) {
      string str_post = InfixToPostfix(str);
      int y0 = min(x0, x1, str);
      int y1 = max(x0, x1, str);
      srand(time(NULL));
      // if (x0 <= x1 || y0 <= y1) {
      //       perror("client: invalid domain");
      //       exit(1);
      // }
      int counter = 0;
      for (int i = 0; i < n; i++) {
            double x = random_num(x0,x1);
            double y = random_num(y0,y1);
            int flag = (y <= postfixEval(str_post, x));
            //printf("%d: (%lf,%lf) ---- %lf ---- %d\n", i+1, x, y, func(x), flag);
            if (flag) {
                  counter++;
            }
      }
      double ans = abs(y1-y0) * abs(x1-x0) * (double) counter / n;
      return ans;
}