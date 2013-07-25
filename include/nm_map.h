#include <iostream>
#include <cmath>
#include <ctime>
#include <iomanip>

#include "nm.h"

using namespace std;

template <typename K, typename V>
class Map{
 private:
  static const int maxSize=100;
  int nCur,nKey;

  K **key;
  V *val;
  
 public:
  Map();
  Map(int n);
  ~Map();
  void Add(K [],V v);
  int Search(K []);
  void Clear();
  //inline const K* operator[](const int i) const;
  inline V & operator[](const int i);

};

template <typename K, typename V>
Map<K,V>::Map() : nCur(0), nKey(0), key(0), val(0) {}

template <typename K, typename V>
Map<K,V>::Map(int n){
  int i;
  nCur=0;
  nKey=n;
  key = new K*[maxSize];
  for(i=0;i<maxSize;i++){
    key[i] = new K[nKey];
  }
  val = new V[maxSize];
}

template <typename K, typename V>
Map<K,V>::~Map(){
  if (key != 0) {
    delete[] (key[0]);
    delete[] (key);
  }
  if (val !=0) {
    delete[] val;
  }
}

template <typename K, typename V>
void Map<K,V>::Add(K k[],V v){
  if(nCur>=maxSize) return;
  int i;
  for(i=0;i<nKey;i++){
    key[nCur][i]=k[i];
  }
  val[nCur]=v;
  nCur++;
  return;
}

template <typename K, typename V>
int Map<K,V>::Search(K k[]){
  int i,j,flag;

  for(i=0;i<nCur;i++){
    flag=1;
    for(j=0;j<nKey;j++){
      if(key[i][j]!=k[j]){
        flag=0;
        break;
      }
    }
    if(flag==1) return i;
  }
  return -1;
}

template <typename K, typename V>
void Map<K,V>::Clear(){
  nCur=0;
}

/*template <typename K, typename V>
inline const K* Map<K,V>::operator[](const int i) const{
  return key[i];
}
*/
template <typename K, typename V>
inline V & Map<K,V>::operator[](const int i){
  return val[i];
}
