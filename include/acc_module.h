#include <vector>
#include <limits>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <math.h>

using namespace::std;

typedef vector< vector<double> > Matrix;

// structure type
#define tDriftSpace   0
#define tHalfCell			1
#define tFullCell			2
#define tFirstHalf    3
#define tSecondHalf   4
#define tAsymmetric   5
#define tTotalLength 	100
#define tUnknown      1000

// relative position
// if rpos < 0, print (0,0);if abs(rpos) > 1, print (L,0) and (0,0)
#define pFirst    	 -1
#define pInbetween    0
#define pLast        	2
#define pStandalone  -2

// Acc_point
class Acc_point{
 private:
  double x;
  double y;

 public:
  Acc_point(double xx,double yy){ x=xx;y=yy;}
  friend ostream& operator<<(ostream& out,const Acc_point& p){
    out<<"&PO "<<"X="<<p.x<<" Y="<<p.y<<" &";
    return out;
  }
};

// Acc_arcend: NT=2
class Acc_arcend{
 private:
  double xc;
  double yc;
  double x;
  double y;

 public:
  Acc_arcend(double x0,double y0,double xx,double yy){ xc=x0;yc=y0;x=xx;y=yy;}
  friend ostream& operator<<(ostream& out,const Acc_arcend& p){
    out<<"&PO NT=2 "<<"X0="<<p.xc<<" Y0="<<p.yc<<" X="<<p.x<<" Y="<<p.y<<" &";
    return out;
  }
};

// Acc_conf
class Acc_conf{
 private:
  string title;
  int kprob;
  int icylin;
  double freq;
  double xdri;
  double ydri;
  double dx;
  double dy;

  bool scan;
  int nstep;
  double delfr;
 public:
  Acc_conf(){
    title="Here goes the title";kprob=1;icylin=1;freq=2856;xdri=ydri=0;dx=dy=0.05;
    scan=true;nstep=0;delfr=0;
  }
  void set_title(string str){ title=str; }
  void set_problem(int kpr,int icy){ kprob=kpr;icylin=icy; }
  void Set_frequency(double fr0){ freq=fr0; }
  void Set_drive(double xdr,double ydr){ xdri=xdr;ydri=ydr; }
  void Set_mesh(double dxx,double dyy){ dx=dxx;dy=dyy; }
  void Set_scan(int n,double df,double fr0){ scan=true;nstep=n;delfr=df;freq=fr0; }

  void Acc_write(string fname,ios_base::openmode mode=ios::out){
    ofstream fout;
    fout.open(fname.c_str(),mode);

    fout<<title<<endl<<endl;
    fout<<"&REG"<<endl;
    fout<<"KPROB="<<kprob<<" ICYLIN="<<icylin<<endl;
    if(scan==true&&nstep>0){
      fout<<"NSTEP="<<nstep<<endl;
      fout<<"DELFR="<<delfr<<endl;
    }
    fout<<"FREQ="<<freq<<endl;
    if(fabs(xdri)>0&&fabs(ydri)>0){
      fout<<"XDRI="<<xdri<<" YDRI="<<ydri<<endl;
    }
    fout<<"DX="<<dx<<" DY="<<dy<<endl;
    fout<<"&"<<endl<<endl;

    fout.close();
  }
};

// Acc_cavity
class Acc_cavity{
 protected:
  int    type; 
  int    rpos;

  double position;

  double length;
  double bore;
  double diameter;
  double corner;
  double septum;
 public:
 Acc_cavity(int t=tDriftSpace,int p=pStandalone,double pos=0,double len=0,double bor=0,double dia=0,double cor=0,double sep=0):
  type(t),rpos(p),position(pos),length(len),bore(bor),diameter(dia),corner(cor),septum(sep){if(type==tTotalLength) length=length-position;};
  void Acc_write(string fname,ios_base::openmode mode=ios::app);
};

void Acc_cavity::Acc_write(string fname,ios_base::openmode mode){
  double x,y,x0,y0;
  ofstream fout;

  fout.open(fname.c_str(),mode);
  
  x=position;y=bore;
  if(rpos<0){
    fout<<Acc_point(x,0)<<endl;
    fout<<Acc_point(x,y)<<endl;
  }

  if(type==tDriftSpace||type==tTotalLength){
    // a drift space
    x=x+length;y=y;
    if(length>0) fout<<Acc_point(x,y)<<endl;
  }
  else{
    // first half of a cell
    if(type==tFullCell||type==tFirstHalf){
      x0=x;y0=y+septum/2;x=x+septum/2;y=y0;
      fout<<Acc_arcend(x0,y0,x-x0,y-y0)<<endl;
      x=x;y=diameter/2-corner;
      fout<<Acc_point(x,y)<<endl;
      x0=x+corner;y0=y;x=x0;y=y0+corner;
      if(corner>0) fout<<Acc_arcend(x0,y0,x-x0,y-y0)<<endl;
      x=x+(length-2*corner-septum)/2;y=y;
      fout<<Acc_point(x,y)<<endl;
    }
    else if(type==tHalfCell){
      x=x;y=diameter/2;
      fout<<Acc_point(x,y)<<endl;
    }
    // second half of a cell
    if(type==tFullCell||type==tSecondHalf||type==tHalfCell){
      x=x+(length-2*corner-septum)/2;y=diameter/2;
      fout<<Acc_point(x,y)<<endl;
      x0=x;y0=y-corner;x=x+corner;y=y0;
      if(corner>0) fout<<Acc_arcend(x0,y0,x-x0,y-y0)<<endl;
      x=x;y=septum/2+bore;
      fout<<Acc_point(x,y)<<endl;
      x0=x+septum/2;y0=y;x=x0;y=y-septum/2;
      if(septum/2>0) fout<<Acc_arcend(x0,y0,x-x0,y-y0)<<endl;
    }
  }

  if(abs(rpos)>1){
    fout<<Acc_point(x,0)<<endl;
    fout<<Acc_point(0,0)<<endl;
  }

  fout<<endl;
  fout.close();
};

int Acc_index(string str){
  if(!strcmp(str.c_str(),"DriftSpace")){
    return tDriftSpace;
  }
  else if(!strcmp(str.c_str(),"HalfCell")){
    return tHalfCell;
  }
  else if(!strcmp(str.c_str(),"FullCell")){
    return tFullCell;
  }
  else if(!strcmp(str.c_str(),"FirstHalf")){
    return tFirstHalf;
  }
  else if(!strcmp(str.c_str(),"SecondHalf")){
    return tSecondHalf;
  }
  else if(!strcmp(str.c_str(),"TotalLength")){
    return tTotalLength;
  }
  else{
    return tUnknown;
  }
};

void Acc_read2vector(string fname,Matrix &vv){
  int i,m,type,rpos;
  double *d;

  if(vv.size()==0) m=7;
  d=new double[m];

  char buf[512],key[32];
  ifstream fin;
  fin.open(fname.c_str());

  while(!fin.eof()){
    fin.getline(buf,511);
    while(!fin.eof()&&sscanf(buf,"%[^:]:%d",key,&rpos)<2){
      fin.getline(buf,511);
    }
    if(fin.eof()) break;

    type=Acc_index(key);
    d[0]=type;
    d[1]=rpos;

    i=2;
    switch(type){
    case tDriftSpace:case tTotalLength:
      while(i<4){
        fin.getline(buf,511);
        sscanf(buf,"%*s %lf",&d[i]);
        i++;
      }
      break;
    case tHalfCell:case tFullCell:case tFirstHalf:case tSecondHalf:
      while(i<m){
        fin.getline(buf,511);
        sscanf(buf,"%*s %lf",&d[i]);
        i++;
      }
      break;
    }
    vv.push_back(vector<double>(d,d+m));
  }
};

void Acc_build(string fname,Matrix &vv,double rr=1.0){
  int i,j,n=vv.size(),m=vv[0].size(),type,rpos;
  double pos=0;

  Acc_conf con;
  Acc_cavity c;

  con.Set_drive(3.5,3.8);
  con.Set_frequency(2855);
  con.Set_mesh(0.03,0.03);
  //con.Set_scan(100,1,2820);
  con.Acc_write(fname);

  for(i=0;i<n;i++){
    for(j=2;j<m;j++){
      vv[i][j]*=rr;
    }
  }

  for(i=0;i<n;i++){
    type=(int)(vv[i][0]);
    rpos=(int)(vv[i][1]);
    c=Acc_cavity(type,rpos,pos,vv[i][2],vv[i][3],vv[i][4],vv[i][5],vv[i][m-1]);
    c.Acc_write(fname.c_str());
    if(type==tHalfCell||type==tFirstHalf||type==tSecondHalf) pos+=vv[i][2]/2;
    else pos+=vv[i][2];
  }

};

void Acc_run_autofish(string fp){
  string cmd;
  cmd="autofish "+fp;
  system(cmd.c_str());
};

void Acc_run_sf7(string fp){
  string cmd;
  cmd="sf7 "+fp;
  system(cmd.c_str());
};

double Acc_find_FR(string fp=""){
  char ext[10];
  string cmd,str;
  double FR;
  ifstream fin;

  sscanf(fp.c_str(),"%*[^.].%s",ext);

  if(strcmp(fp.c_str(),"")==0||strcmp(ext,"TXT")==0||strcmp(ext,"txt")==0){
    cmd="awk '{print $3 }' "+fp+" | tail -n +35 | head -n -1 | awk 'BEGIN{min=0;max=0}{if(min>$1) min=$1;if(max<$1) max=$1}END{print -max/min}' > tmp.dat";
  }
  else if(strcmp(ext,"SFO")==0||strcmp(ext,"sfo")==0){
    cmd="awk '{print $2 }' "+fp+" | head -n 694 | tail -n +216 | awk 'BEGIN{m1=0;m2=0;flag=1}{if($1<5e5) flag=2;if(m1<$1&&flag==1) m1=$1;if(m2<$1&&flag==2) m2=$1;}END{print m1/m2}' > tmp.dat";
  }

  //cmd="./find_FR.sh "+fp;
  system(cmd.c_str());

  fin.open("tmp.dat");
  fin>>str;
  sscanf(str.c_str(),"%lf",&FR);
  fin.close();
  remove("tmp.dat");
  
  return FR;
};

double Acc_find_frequency(string fp){
  char buf[256];
  string cmd;
  double freq;
  ifstream fin;

  cmd="grep \"Frequency\" "+fp+" | grep \"=\" > tmp.dat";
  system(cmd.c_str());

  fin.open("tmp.dat");
  fin.getline(buf,255);
  sscanf(buf,"%*[^=]=%lf",&freq);
  fin.close();
  remove("tmp.dat");

  return freq;
};

void Acc_adjust_FR(Matrix &vv,double fr0=1.0){
  int i,j,n,m,ff;
  double d0,d1,fr,freq,eps=2e-3;
  string fname="tmp.af";
  n=vv.size();
  m=vv[0].size();
  for(i=0;i<n;i++){
    if((int)(vv[i][0])==tFullCell){ ff=i;break;}}
  d0=d1=vv[ff][4];fr=fr0;
  do{
    for(i=0;i<n;i++){
      for(j=0;j<m;j++){
        //cout<<setw(10)<<vv[i][j];
      }
      //cout<<endl;
    }

    if(fr>=fr0) d1=d0*(1-0.002*(1-fr0/fr)); else d1=d0*(1+0.002*(1-fr/fr0));
    vv[ff][4]=d1;
    Acc_build(fname,vv);
    Acc_run_autofish(fname);
    Acc_run_sf7("tmp.in7");
    fr=Acc_find_FR("OUTSF7.TXT");
    d0=vv[ff][4];
    cout<<setw(10)<<"d1 = "<<setw(10)<<d1<<setw(10)<<"fr = "<<setw(1)<<fr<<endl;
    //break;
  }while(fabs(fr-fr0)>eps);

  freq=Acc_find_frequency("TMP.SFO");
  cout<<freq<<endl;
  Acc_build(fname,vv,freq/2856);
  Acc_run_autofish(fname);
  Acc_run_sf7("tmp.in7");
  
};

void Acc_adjust_frequency(Matrix &vv,int type=tFirstHalf,const double f0=2856){
  int i,j,n,m,ff;
  double d0,d1,freq,eps=2e-3;
  string fname="tmp.af";
  n=vv.size();
  m=vv[0].size();
  for(i=0;i<n;i++){
    if((int)(vv[i][0])==type){ ff=i;break;}
  }
  d0=d1=vv[ff][4];freq=f0;
  do{
    for(i=0;i<n;i++){
      for(j=0;j<m;j++){
        //cout<<setw(10)<<vv[i][j];
      }
      //cout<<endl;
    }

    d1+=(freq-f0)/f0*d0*0.8;
    vv[ff][4]=vv[ff+1][4]=d1;
    Acc_build(fname,vv);
    Acc_run_autofish(fname);
    freq=Acc_find_frequency("TMP.SFO");
    d0=vv[ff][4];
    cout.precision(9);
    cout<<setw(10)<<"d1 = "<<setw(10)<<d1<<setw(10)<<"freq = "<<setw(1)<<freq<<endl;
    //break;
  }while(fabs(freq-f0)>eps);

  Acc_run_sf7("tmp.in7");
  
};

