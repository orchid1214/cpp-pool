#ifndef _NM_math_H_
#define _NM_math_H_

#include <cstdlib>
#include <string>
#include <cmath>
#include <complex>
#include <iostream>
using namespace std;

template<class T>
inline const T SQR(const T a) {return a*a;}

template<class T>
inline const T MAX(const T &a, const T &b) {return b > a ? (b) : (a);}

inline float MAX(const double &a, const float &b) {return b > a ? (b) : float(a);}
inline float MAX(const float &a, const double &b) {return b > a ? float(b) : (a);}

template<class T>
inline const T MIN(const T &a, const T &b) {return b < a ? (b) : (a);}

inline float MIN(const double &a, const float &b) {return b < a ? (b) : float(a);}
inline float MIN(const float &a, const double &b) {return b < a ? float(b) : (a);}

template<class T>
inline const T SIGN(const T &a, const T &b) {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

inline float SIGN(const float &a, const double &b) {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}
inline float SIGN(const double &a, const float &b) {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

template<class T>
inline void SWAP(T &a, T &b) {T dum=a; a=b; b=dum;}

//Overloaded complex operations to handle mixed float and double
inline const complex<float> operator+(const double &a,
				      const complex<float> &b) { return float(a)+b; }

inline const complex<float> operator+(const complex<float> &a,
				      const double &b) { return a+float(b); }

inline const complex<float> operator-(const double &a,
				      const complex<float> &b) { return float(a)-b; }

inline const complex<float> operator-(const complex<float> &a,
				      const double &b) { return a-float(b); }

inline const complex<float> operator*(const double &a,
				      const complex<float> &b) { return float(a)*b; }

inline const complex<float> operator*(const complex<float> &a,
				      const double &b) { return a*float(b); }

inline const complex<float> operator/(const double &a,
				      const complex<float> &b) { return float(a)/b; }

inline const complex<float> operator/(const complex<float> &a,
				      const double &b) { return a/float(b); }

//some compilers choke on pow(float,double) in single precision. also atan2

inline float pow (float x, double y) {return pow(double(x),y);}
inline float pow (double x, float y) {return pow(x,double(y));}
inline float atan2 (float x, double y) {return atan2(double(x),y);}
inline float atan2 (double x, float y) {return atan2(x,double(y));}

#endif /* _NM_H_ */
