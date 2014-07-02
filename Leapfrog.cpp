#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
using namespace std;
typedef vector < vector <double> > mat;
class Body {
public:
  double m;
  vector <double> r;
  vector <double> v;
  vector <double> a;
  void set_r(vector <double>);
  void set_v(vector <double>);
  void set_a(vector <double>);
  void set_m(double);
};
void Body::set_r(vector <double> x) {
  r = x;
}
void Body::set_v(vector <double> x) {
  v = x;
}
void Body::set_a(vector <double> x) {
  a = x;
}
void Body::set_m(double x) {
  m = x;
}
void print_vec(vector <double> vec) {
  for (int i = 0; i < vec.size(); i++) {
    if (i =! 0) cout << ",";
    cout << vec[i];
  }
  cout << endl;
}
void print_mat(mat m) {
  for (int i = 0; i < m.size(); i++) print_vec(m[i]);
}
double random(int max, int min, int precision) { 
  min *= pow(10,precision);
  max *= pow(10,precision);
  double result = rand() % (max - min) + min;
  result /= pow(10,precision);
  return result;
}
int main() {
  cout.setf(ios::fixed);
  cout.precision(4);
  int n;
  cin >> n;
  Body System[n];
  double m = 0;
  double r_max = 1000;
  double r_min = 0;
  double v_max = 10;
  double v_min = -10;
  double m_max = 1000;
  double m_min = 100;
  vector <double> r(3,0);
  vector <double> v(3,0);
  vector <double> a(3,0);
  vector <double> x(3);
  vector <double> x0(3);
  for (int i = 0; i < 3; i++) {
    x0[i] = r_min;
    x[i] = r_max;
  }
  Cube Root;
  Root.set_x0(x0);
  Root.set_x(x);
  // SETTING POSITIONS AND VELOCITIES RANDOMLY
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 3; j++) {
      r[j] = random(r_max,r_min,4);
      v[j] = random(v_max,v_min,4);
    }
    m = random(m_max,m_min,4);
    System[i].set_r(r);
    System[i].set_v(v);
    System[i].set_a(a);
    System[i].set_m(m);
  }
}
