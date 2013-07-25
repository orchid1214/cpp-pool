#ifndef _NM_H_
#define _NM_H_

#include <cstdlib>
#include <string>
#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

inline void nmerror(const string error_text)
// Numerical Models standard error handler
{
  cerr << "Numerical Models run-time error..." << endl;
  cerr << error_text << endl;
  cerr << "...now exiting to system..." << endl;
  exit(1);
}

// Define vectors
template <class T>
class NMVec {
 private:
  int ndim;	// size of array. upper index is ndim-1
  T *v;
 public:
  NMVec();
  explicit NMVec(int n);		// Zero-based array
  NMVec(const T &a, int n);	// Initialize to constant value
  NMVec(const T *a, int n);	// Initialize to array
  NMVec(const NMVec &rhs);	// Copy constructor
  NMVec & operator=(const NMVec &rhs);	//assignment
  NMVec & operator=(const T &a);	//assign a to every element
  inline T & operator[](const int i);	//i'th element
  inline const T & operator[](const int i) const;
  inline int size() const;
  ~NMVec();
};

template <class T>
NMVec<T>::NMVec() : ndim(0), v(0) {}

template <class T>
NMVec<T>::NMVec(int n) : ndim(n), v(new T[n]) {}

template <class T>
NMVec<T>::NMVec(const T& a, int n) : ndim(n), v(new T[n])
{
  for(int i=0; i<n; i++)
    v[i] = a;
}

template <class T>
NMVec<T>::NMVec(const T *a, int n) : ndim(n), v(new T[n])
{
  for(int i=0; i<n; i++)
    v[i] = *a++;
}

template <class T>
NMVec<T>::NMVec(const NMVec<T> &rhs) : ndim(rhs.ndim), v(new T[ndim])
{
  for(int i=0; i<ndim; i++)
    v[i] = rhs[i];
}

template <class T>
NMVec<T> & NMVec<T>::operator=(const NMVec<T> &rhs)
  // postcondition: normal assignment via copying has been performed;
  //		if vector and rhs were different sizes, vector
  //		has been resized to match the size of rhs
{
  if (this != &rhs)
    {
      if (ndim != rhs.ndim) {
	if (v != 0) delete [] (v);
	ndim=rhs.ndim;
	v= new T[ndim];
      }
      for (int i=0; i<ndim; i++)
	v[i]=rhs[i];
    }
  return *this;
}

  template <class T>
  NMVec<T> & NMVec<T>::operator=(const T &a)	//assign a to every element
  {
    for (int i=0; i<ndim; i++)
      v[i]=a;
    return *this;
  }

    template <class T>
    inline T & NMVec<T>::operator[](const int i)	//subscripting
    {
      return v[i];
    }

template <class T>
inline const T & NMVec<T>::operator[](const int i) const	//subscripting
{
  return v[i];
}

template <class T>
inline int NMVec<T>::size() const
{
  return ndim;
}

template <class T>
NMVec<T>::~NMVec()
{
  if (v != 0)
    delete[] (v);
}

// Define matrix
template <class T>
class NMMat {
 private:
  int nrow;
  int ncol;
  T **v;
 public:
  NMMat();
  NMMat(int n, int m);			// Zero-based array
  NMMat(const T &a, int n, int m);	//Initialize to constant
  NMMat(const T *a, int n, int m);	// Initialize to array
  NMMat(const NMMat &rhs);		// Copy constructor
  NMMat & operator=(const NMMat &rhs);	//assignment
  NMMat & operator=(const T &a);		//assign a to every element
  inline T* operator[](const int i);	//subscripting: pointer to row i
  inline const T* operator[](const int i) const;
  inline int nrows() const;
  inline int ncols() const;
  ~NMMat();
};

template <class T>
NMMat<T>::NMMat() : nrow(0), ncol(0), v(0) {}

template <class T>
NMMat<T>::NMMat(int n, int m) : nrow(n), ncol(m), v(new T*[n])
{
  v[0] = new T[m*n];
  for (int i=1; i< n; i++)
    v[i] = v[i-1] + m;
}

template <class T>
NMMat<T>::NMMat(const T &a, int n, int m) : nrow(n), ncol(m), v(new T*[n])
{
  int i,j;
  v[0] = new T[m*n];
  for (i=1; i< n; i++)
    v[i] = v[i-1] + m;
  for (i=0; i< n; i++)
    for (j=0; j<m; j++)
      v[i][j] = a;
}

template <class T>
NMMat<T>::NMMat(const T *a, int n, int m) : nrow(n), ncol(m), v(new T*[n])
{
  int i,j;
  v[0] = new T[m*n];
  for (i=1; i< n; i++)
    v[i] = v[i-1] + m;
  for (i=0; i< n; i++)
    for (j=0; j<m; j++)
      v[i][j] = *a++;
}

template <class T>
NMMat<T>::NMMat(const NMMat &rhs) : nrow(rhs.nrow), ncol(rhs.ncol), v(new T*[nrow])
{
  int i,j;
  v[0] = new T[ncol*nrow];
  for (i=1; i< nrow; i++)
    v[i] = v[i-1] + ncol;
  for (i=0; i< nrow; i++)
    for (j=0; j<ncol; j++)
      v[i][j] = rhs[i][j];
}

template <class T>
NMMat<T> & NMMat<T>::operator=(const NMMat<T> &rhs)
  // postcondition: normal assignment via copying has been performed;
  //		if matrix and rhs were different sizes, matrix
  //		has been resized to match the size of rhs
{
  if (this != &rhs) {
    int i,j;
    if (nrow != rhs.nrow || ncol != rhs.ncol) {
      if (v != 0) {
	delete[] (v[0]);
	delete[] (v);
      }
      nrow=rhs.nrow;
      ncol=rhs.ncol;
      v = new T*[nrow];
      v[0] = new T[ncol*nrow];
    }
    for (i=1; i< nrow; i++)
      v[i] = v[i-1] + ncol;
    for (i=0; i< nrow; i++)
      for (j=0; j<ncol; j++)
	v[i][j] = rhs[i][j];
  }
  return *this;
}

  template <class T>
  NMMat<T> & NMMat<T>::operator=(const T &a)	//assign a to every element
  {
    for (int i=0; i< nrow; i++)
      for (int j=0; j<ncol; j++)
	v[i][j] = a;
    return *this;
  }

    template <class T>
    inline T* NMMat<T>::operator[](const int i)	//subscripting: pointer to row i
    {
      return v[i];
    }

template <class T>
inline const T* NMMat<T>::operator[](const int i) const
{
  return v[i];
}

template <class T>
inline int NMMat<T>::nrows() const
{
  return nrow;
}

template <class T>
inline int NMMat<T>::ncols() const
{
  return ncol;
}

template <class T>
NMMat<T>::~NMMat()
{
  if (v != 0) {
    delete[] (v[0]);
    delete[] (v);
  }
}

// Define 3d matrix
template <class T>
class NMMat3d {
 private:
  int nn;
  int mm;
  int kk;
  T ***v;
 public:
  NMMat3d();
  NMMat3d(int n, int m, int k);
  inline T** operator[](const int i);	//subscripting: pointer to row i
  inline const T* const * operator[](const int i) const;
  inline int dim1() const;
  inline int dim2() const;
  inline int dim3() const;
  ~NMMat3d();
};

template <class T>
NMMat3d<T>::NMMat3d(): nn(0), mm(0), kk(0), v(0) {}

template <class T>
NMMat3d<T>::NMMat3d(int n, int m, int k) : nn(n), mm(m), kk(k), v(new T**[n])
{
  int i,j;
  v[0] = new T*[n*m];
  v[0][0] = new T[n*m*k];
  for(j=1; j<m; j++)
    v[0][j] = v[0][j-1] + k;
  for(i=1; i<n; i++) {
    v[i] = v[i-1] + m;
    v[i][0] = v[i-1][0] + m*k;
    for(j=1; j<m; j++)
      v[i][j] = v[i][j-1] + k;
  }
}

template <class T>
inline T** NMMat3d<T>::operator[](const int i) //subscripting: pointer to row i
{
  return v[i];
}

template <class T>
inline const T* const * NMMat3d<T>::operator[](const int i) const
{
  return v[i];
}

template <class T>
inline int NMMat3d<T>::dim1() const
{
  return nn;
}

template <class T>
inline int NMMat3d<T>::dim2() const
{
  return mm;
}

template <class T>
inline int NMMat3d<T>::dim3() const
{
  return kk;
}

template <class T>
NMMat3d<T>::~NMMat3d()
{
  if (v != 0) {
    delete[] (v[0][0]);
    delete[] (v[0]);
    delete[] (v);
  }
}

#endif /* _NM_H_ */
