#include <iostream>
#include "demo.h"

using namespace::std;

void hello(){
   cout << "Hello World!";
}

int factorial(int n){
  int i,r=1;
  for(i=1;i<=n;i++){
    r=r*i;
  }
  return r;
}
