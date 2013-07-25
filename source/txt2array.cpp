#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "txt2array.h"

using namespace::std;

int isSpaceLine(char *str){
  char c;
  if(sscanf(str,"%^[ ]",&c)) return 1;
  else return 0;
}

int isThereDigit(char *str){
  char c;
  if(sscanf(str,"%^[0-9]",&c)) return 1;
  else return 0;
}

double GetColMin(const char in[],int flag,int st,int end){
  int n;
  double r,*array;
  array = txt2array(&n,in,flag);
  if(end==0){
    end=n;
  }
  r=array[st];
  for(int i=st+1;i<end;i++){
    if(r>array[i])
      r=array[i];
  }
  return r;
}

double GetColMin(const char in[],const char col[],int st,int end){
  int n;
  double r,*array;
  array = txt2array(&n,in,col);
  if(end==0){
    end=n;
  }
  r=array[st];
  for(int i=st+1;i<end;i++){
    if(r>array[i])
      r=array[i];
  }
  return r;
}

double GetColMax(const char in[],int flag,int st,int end){
  int n;
  double r,*array;
  array = txt2array(&n,in,flag);
  if(end==0){
    end=n;
  }
  r=array[st];
  for(int i=st+1;i<end;i++){
    if(r<array[i])
      r=array[i];
  }
  return r;
}

double GetColMax(const char in[],const char col[],int st,int end){
  int n;
  double r,*array;
  array = txt2array(&n,in,col);
  if(end==0){
    end=n;
  }
  r=array[st];
  for(int i=st+1;i<end;i++){
    if(r<array[i])
      r=array[i];
  }
  return r;
}

double* txt2array(int *len,const char in[256],const char col[64],double coe,double offset){	

  double *data;
  ifstream fin;
  fin.open(in);

  char buf[501];
  char delims[10] = " ,;:\t";
  char *pointer;
  char *nullPointer = NULL;

  int i,j,flag,cur,st,end;
			
  cur = 0;
  while(fin.getline(buf,500)&&isSpaceLine(buf)==1){ cur++; }

  flag = 0;
  pointer = strtok(buf,delims);

  do{
    if(strcmp(pointer,col)==0){
      break;
    }
    else{
      flag++;
    }
  }while((pointer=strtok(nullPointer,delims)));

  st = end = cur;
  while(fin.getline(buf,500)){
    cur++;
    if(isSpaceLine(buf)==0){ end++; }
  }
  *len = end-st;

  data = new double[*len];

  fin.clear();
  fin.seekg(0);

  while(fin.getline(buf,500)&&isSpaceLine(buf)==1);

  i = 0;
  while(fin.getline(buf,500)){
    if(isSpaceLine(buf)==1){ break; }
    pointer = strtok(buf,delims);
    j = 0;
    do{
      if(j==flag){
	data[i]=atof(pointer)*coe+offset;
	i++;
      }
      j++;
    }while((pointer=strtok(nullPointer,delims)));
  }

  fin.close();
  return data;
}

double* txt2array(int *len,const char in[256],int flag,double coe,double offset){	

  double *data;
  ifstream fin;
  fin.open(in);

  char buf[501];
  char delims[10] = " ,;:\t";
  char *pointer;
  char *nullPointer = NULL;

  int i,j,st,end;
			
  st = 0;
  end = st;
  while(fin.getline(buf,500)){
    if(strncmp(buf,"          ",10)==0||strlen(buf)==0) break;
    end++;
  }
  *len = end-st;

  data = new double[*len];

  fin.close();
  fin.open(in);
  while(st--){
    fin.getline(buf,501);
  }
  
  i = 0;
  while(fin.getline(buf,500)){
    if(strncmp(buf,"     ",5)==0||strlen(buf)==0) break;
    pointer = strtok(buf,delims);
    j = 0;
    do{
      if(j==flag){
	data[i]=atof(pointer)*coe+offset;
	i++;
      }
      j++;
    }while((pointer=strtok(nullPointer,delims)));
  }

  fin.close();
  return data;
}
