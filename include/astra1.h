#include <fstream>
#include <iomanip>
#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

#include "misc.h"
#include "astra_module.h"

using namespace::std;

void * StopAstra(void *arg);

class Astra{
 public:
  Input input;

  Newrun newrun;
  Charge charge;
  Cavity cavity;
  Solenoid solenoid;

  Output output;

  void SetInput(int n,double q,double E,string dat){input=Input(n,q,E,dat);};

  void SetNewrun(string tit,string dat,bool aut){newrun=Newrun(tit,dat,aut);};  
  void SetCharge(bool l,int nr,int nl){charge=Charge(nr,nl,l);};

  void InitCavity(int n){cavity=Cavity(n);};
  void SetCavity(int i,double p,double fm,double fr,double ph,string dat){cavity.Build(i,p,fm,fr,ph,dat);}
  void InitSolenoid(int n){solenoid=Solenoid(n);};
  void SetSolenoid(int i,double p,double fm,string dat){solenoid.Build(i,p,fm,dat);}

  void SetOutput(double zsta,double zsto,int zemi=100,int zpha=10,int n=0,double v[]=0){output=Output(zsta,zsto,zemi,zpha,n,v);}

  void WriteAll(const char out[]="_ast.in");

  void RunAstra(const char in[]="_ast.in");
  void Generator(const char in[]="_gen.in");
  void SaveAstra(const char dir[]);

};

void Astra::WriteAll(const char out[]){
  input.Write("_gen.in");

  newrun.Write(out);
  charge.Write(out);
  if(cavity.Size()>0) cavity.Write(out);
  if(solenoid.Size()>0) solenoid.Write(out);
  output.Write(out);
}

void Astra::RunAstra(const char in[]){

  char cmd[128];
  pthread_t pid;
  pthread_create(&pid,NULL,StopAstra,(void *)(1));
  sprintf(cmd,"astra %s > _ast.dat",in);
  system(cmd);
  return;
}

void Astra::Generator(const char in[]){

  char cmd[128];
  pthread_t pid;
  pthread_create(&pid,NULL,StopAstra,(void *)(0));
  sprintf(cmd,"generator %s > _gen.dat",in);
  system(cmd);
  return;
}

void Astra::SaveAstra(const char dir[]){
  char cmd[256];
  sprintf(cmd,"mkdir %s",dir);
  system(cmd);

  sprintf(cmd,"move _* %s/",dir);
  system(cmd);
}

void * StopAstra(void *arg){
  int *flag = (int *)arg;
  while(1){
    if(flag==0){
      Waiting(10);
      if(Grep("Fortran Pause","_gen.dat")){
        system("taskkill /im generator.exe /f");
        return NULL;
      }
      cout<<"Waiting to kill generator"<<endl;
    }
    else{
      Waiting(10);
      if(Grep("Fortran Pause","_ast.dat")){
        system("taskkill /im Astra.exe /f");
        return NULL;
      }
      cout<<"Waiting to kill astra"<<endl;
    }
  }  
}
