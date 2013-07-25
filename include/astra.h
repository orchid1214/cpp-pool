#include <fstream>
#include <iomanip>
#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

#include "misc.h"

using namespace::std;

void RunAstra(const char in[]="ast.in");
void Generator(const char in[]="gen.in");
void * StopAstra(void *arg);

void Move2Dir(const char prefix[],const char dir[]);

void RunAstra(const char in[]){
  char cmd[128];
  pthread_t pid;
  pthread_create(&pid,NULL,StopAstra,(void *)(1));
  sprintf(cmd,"astra %s > ast.log",in);
  system(cmd);
  return;
}

void Generator(const char in[]){
  char cmd[128];
  pthread_t pid;
  pthread_create(&pid,NULL,StopAstra,(void *)(0));
  sprintf(cmd,"generator %s > gen.log",in);
  system(cmd);
  return;
}

void * StopAstra(void *arg){
  int *flag = (int *)arg;
  while(1){
    if(flag==0){
      Waiting(5);
      if(Grep("Fortran Pause","gen.log")){
        system("taskkill /im generator.exe /f");
	return NULL;
      }
      cout<<"Waiting to kill generator"<<endl;
    }
    else{
      Waiting(10);
      if(Grep("Fortran Pause","ast.log")){
        system("taskkill /im Astra.exe /f");
	return NULL;
      }
      cout<<"Waiting to kill astra"<<endl;
    }
  }  
}

void Move2Dir(const char prefix[],const char dir[]){
  char cmd[256];
  sprintf(cmd,"if [ ! -d %s ];then mkdir %s; fi",dir,dir);
  //sprintf(cmd,"mkdir %s",dir);
  system(cmd);
  sprintf(cmd,"mv %s* %s/",prefix,dir);
  system(cmd);
}
