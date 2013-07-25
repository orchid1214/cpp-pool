#include <cmath>
#include "Amoeba.h"
using namespace std;

Amoeba::Amoeba(){
  np = 0; mp = 0; op = 0;
  nfunc = 0;
  x = 0;  y = 0;
  centroid = 0;
  simplex = 0;
}

Amoeba::~Amoeba(){}

Amoeba::Amoeba(int n,int m){
  if(m==0) m=n+1;
  np = n; mp = m; op = 0; nfunc = 0;
  x = NMVec<double>(np);
  y = NMVec<double>(mp);
  centroid = NMVec<double>(np);
  simplex = NMMat<double>(mp,np);
}

int Amoeba::Size(){
  return np;
}

void Amoeba::Simplex(const double x0[]){
  int i,j;
  int m=simplex.nrows();
  int n=simplex.ncols();
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      simplex[i][j] = x0[j];
    }
    if(m>0) simplex[i][i-1]=x0[i-1]*0.8;
  }
}

void Amoeba::Simplex(const NMVec<double> &x0){
  int i,j;
  int m=simplex.nrows();
  int n=simplex.ncols();
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      simplex[i][j] = x0[j];
    }
    if(m>0) simplex[i][i-1]=x0[i-1]*0.8;
  }
}

void Amoeba::Simplex(const NMMat<double> &s0){
  int i,j;
  int m=simplex.nrows();
  int n=simplex.ncols();
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      simplex[i][j] = s0[i][j];
    }
  }
}

void Amoeba::Centroid(){
  int i,j;
  double sum;

  int m=simplex.nrows();
  int n=simplex.ncols();

  for (j=0;j<n;j++) {
    for (sum=0.0,i=0;i<m;i++)
      sum += simplex[i][j];
    centroid[j]=sum;
  }
}

void Amoeba::Solve(ObjFun func, const double ftol){

  const int NMAX=5000;
  const double TINY=1.0e-6;
  int i,ihi,ilo,inhi,j;
  double rtol,ysave,ytry;

  int m=simplex.nrows();
  int n=simplex.ncols();
  
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      x[j]=simplex[i][j];
    }
    y[i]=func(x);
    nfunc++;
  }

  Centroid(); // centroid/np is the centroid of the simplex
  for (;;) {
    ilo=0;
    ihi = y[0]>y[1] ? (inhi=1,0) : (inhi=0,1);
    for (i=0;i<m;i++) {
      if (y[i] <= y[ilo]) ilo=i;
      if (y[i] > y[ihi]) {
        inhi=ihi;
        ihi=i;
      } else if (y[i] > y[inhi] && i != ihi) inhi=i;
    }
    // stop iteration or not
    //rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo])+TINY);
    rtol=fabs(y[ihi]-y[ilo]);
    rtol=y[ilo];
    if (rtol < ftol) {
      //SWAP(y[0],y[ilo]);
      //for (i=0;i<n;i++) SWAP(simplex[0][i],simplex[ilo][i]);
      op = ilo;
      break;
    }
    if (nfunc >= NMAX) nmerror("NMAX exceeded");
    // continue
    ytry=Amotry(func,ihi,-1.0);   // reflection
    nfunc++;
    if (ytry <= y[ilo]){
      ytry=Amotry(func,ihi,2.0);  // expansion
      nfunc++;
    }
    else if (ytry >= y[inhi]) {
      ysave=y[ihi];
      ytry=Amotry(func,ihi,0.5);  // contraction
      nfunc++;
      if (ytry >= ysave) {
        for (i=0;i<m;i++) {
          if (i != ilo) {
            for (j=0;j<n;j++)
              simplex[i][j]=centroid[j]=0.5*(simplex[i][j]+simplex[ilo][j]); // reduction
            y[i]=func(centroid);
            nfunc++;
          }
        }
        Centroid();
      }
    }
  }
}

double Amoeba::Amotry(ObjFun func, const int ihi, const double fac){
  int j;
  double fac1,fac2,ytry;

  int n=simplex.ncols();
  NMVec<double> ptry(n);
  fac1=(1.0-fac)/n;
  fac2=fac1-fac;
  for (j=0;j<n;j++)
    ptry[j]=centroid[j]*fac1-simplex[ihi][j]*fac2;
  ytry=func(ptry);
  if (ytry < y[ihi]) {
    y[ihi]=ytry;
    for (j=0;j<n;j++) {
      centroid[j] += ptry[j]-simplex[ihi][j];
      simplex[ihi][j]=ptry[j];
    }
  }
  return ytry;
}

double* Amoeba::BestSolution(){ 
  double *b=new double[np]; 
  for(int i=0;i<np;i++){
    b[i]=simplex[op][i];
  }
  return b;
};

void Amoeba::BestSolution(NMVec<double> &b){ 
  b=NMVec<double>(np); 
  for(int i=0;i<np;i++){
    b[i]=simplex[op][i];
  }
};

