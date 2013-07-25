#ifndef _Amoeba_H_
#define _Amoeba_H_

#include "nm.h"
#include "nm_math.h"

using namespace std;

typedef double (*ObjFun)(const NMVec<double> &);
//typedef double (*ObjFun)(const double []);

class Amoeba
{
 public:
  Amoeba();
  Amoeba(int n,int m=0);
  ~Amoeba();

  int Size();
  void Centroid();

  void Simplex(const double x0[]);
  void Simplex(const NMVec<double> &x0);
  void Simplex(const NMMat<double> &s0);

  double Amotry(ObjFun func, const int ihi, const double fac);
  void Solve(ObjFun func,const double ftol);

  // return the results.  
  int Nfunc(){ return nfunc; };
  double BestObjective(){ return y[op]; };
  double* BestSolution();
  void BestSolution(NMVec<double> &);
  
 protected:
  int np;
  int mp;
  int op;
  int nfunc;

  NMVec<double> x;
  NMVec<double> y;

  NMVec<double> centroid;
  NMMat<double> simplex;

};

#endif /* _Amoeba_H_ */
