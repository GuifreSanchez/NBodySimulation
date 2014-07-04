#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <fstream>
using namespace std;
double d = 3, p = 4, G = 10;
// Defining body class
ofstream results("results.csv");
ofstream energy("energy.txt");
ofstream init("init.csv");
class Body {
public:
  vector <double> r;
  vector <double> v;
  vector <double> f;
  double m;
};
// Function for random numbers given a maximum and a minimum. Precision says how many decimals you want.
double random(int max, int min, int precision) {
  precision = pow(10,precision);
  max *= precision;
  min *= precision;
  double result = rand() % (max - min) + min;
  result /= precision;
  return result;
}
// Function for relative vector
vector <double> rel_pos(Body b1, Body b2) {
  vector <double> result(d,0);
  for (int i = 0; i < d; i++) result[i] = b2.r[i] - b1.r[i];
  return result;
}
// Function for vector magnitude
double m_vec(vector <double> a) {
  double m = 0;
  for (int i = 0; i < d; i++) m += (a[i] * a[i]);
  return sqrt(m);
}
vector <double> force(Body b1, Body b2) {
  vector <double> F(d,0);
  // e Needed to soft the system, force getting values close to infinity when distance approx. 0.
  double e = 100;
  double d3 = pow(m_vec(rel_pos(b1,b2)),3);
  for (int i = 0; i < d; i++) F[i] = G * b1.m * b2.m * rel_pos(b1,b2)[i] / (d3 + e);
  return F;
}
vector <double> s_vec(vector <double> a, vector <double> b) {
  vector <double> result(d,0);
  for (int i = 0; i < d; i++) result[i] = a[i] + b[i];
  return result;
}
vector <double> p_vec(vector <double> a, double n) {
  vector <double> result(d,0);
  for (int i = 0; i < d; i++) result[i] = a[i] * n;
  return result;
}
void print_vec(vector <double> a, char opt) {
  for (int i = 0; i < a.size(); i++) {
    if (i != 0) {
      if (opt == 'r') results << ",";
      if (opt == 's') init    << ",";
    }
    if (opt == 'r') results << a[i];
    if (opt == 's') init    << a[i];
  }
  if (opt == 'r') results << endl;
  if (opt == 's') init    << endl;
}
void print_system(Body System[], double n, char opt, char file) {
  for (int i = 0; i < n; i++) {
    if (opt == 'r') print_vec(System[i].r,file);
    if (opt == 'v') print_vec(System[i].v,file);
    if (opt == 'f') print_vec(System[i].f,file);
  }
} 
double E_pot(Body System[], double n) {
  double E = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) E -= G * System[i].m * System[j].m / m_vec(rel_pos(System[i],System[j]));
  }
  return E;
}
double E_kin(Body System[], double n) {
  double E = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < d; j++) E += 0.5 * System[i].m * System[i].v[j] * System[i].v[j];
  }
  return E;
}
vector <double> empty(vector <double> a) {
  for (int i = 0; i < d; i++) a[i] = 0;
  return a;
}
int main() {
  int n;
  results.setf(ios::fixed);
  results.precision(p);
  energy.setf(ios::fixed);
  energy.precision(p);
  init.setf(ios::fixed);
  init.precision(p);
  cout.setf(ios::fixed);
  cout.precision(p);
  cin >> n;
  Body System[n];
  double dt = 0.0001;
  double r_max = 100, r_min = -100, v_max = 2, v_min = -2, m_max = 700, m_min = 200;
  int count = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < d; j++) {
      System[i].r.push_back(random(r_max,r_min,p));
      System[i].v.push_back(random(v_max,v_min,p));
      System[i].f.push_back(0);
    }
    System[i].m = random(m_max,m_min,p);
  }
  double E_tot0 = E_kin(System,n) + E_pot(System,n);
  energy << "===================================================" << endl;
  energy << "Initial Kinetic Energy        : " << E_kin(System,n) << endl;
  energy << "Initial Potential Energy      : " << E_pot(System,n) << endl;
  energy << "Total Energy of the System (i): " << E_tot0 << endl;
  // LEAPFROG ALGORITHM
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      // Trying to get lower complexity < O(N^2)
      System[i].f = s_vec(System[i].f,force(System[i],System[j]));
      System[j].f = s_vec(System[j].f,force(System[j],System[i]));
    } 
  }
  print_system(System,n,'r','s');
  for (int t = 0; t < 100000000; t++) {
    for (int i = 0; i < n; i++) {
      if (t % 99 == 0) print_system(System,n,'r','r');
      System[i].v = s_vec(System[i].v,p_vec(System[i].f,dt * 0.5 / System[i].m));
      System[i].r = s_vec(System[i].r,p_vec(System[i].v,dt));
      System[i].f = empty(System[i].f);
      for (int j = i + 1; j < n; j++) {
	System[i].f = s_vec(System[i].f,force(System[i],System[j]));
	System[j].f = s_vec(System[j].f,force(System[j],System[i]));
      } 
      System[i].v = s_vec(System[i].v,p_vec(System[i].f,dt * 0.5 / System[i].m));
    }
  }
  double E_tot = E_kin(System,n) + E_pot(System,n);
  energy << "---------------------------------------------------" << endl;
  energy << "Final Kinetic   Energy        : " << E_kin(System,n) << endl;
  energy << "Final Potential Energy        : " << E_pot(System,n) << endl;
  energy << "Total Energy of the System (f): " << E_tot << endl;
  energy << "===================================================" << endl;
  double error = (E_tot - E_tot0) / E_tot0 * 100;
  if (error < 0) error *= -1;
  energy << "Energy Error (%) => " << error                       << endl;
  energy << "===================================================" << endl;
}
