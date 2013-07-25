// useful functions frequently quoted by other programs
#include <fstream>
#include <string.h>
#include <time.h>
using namespace::std;

int Grep(const char str[],const char filename[]){
  char *buf = new char[512];
  ifstream fin;
  fin.open(filename);
  if(!fin) return 0;
  while(!fin.eof()){
    fin.getline(buf,511);
    if(strstr(buf,str)){
      fin.close();
      return 1;
    }
  }

  fin.close();
  return 0;
};

void Waiting(int n){ // n in seconds
  time_t start_time, cur_time;
  time(&start_time);
  do{
    time(&cur_time);
  }
  while((cur_time - start_time) < n);
};
