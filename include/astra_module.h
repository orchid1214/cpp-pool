#include <fstream>
#include <iomanip>
#include <cstring>
#include <math.h>
#include <stdlib.h>

using namespace::std;

class Input{
 public:
  int np;
  double qe; // nC
  double rc; // mm
  double Eref; // MeV
  string data;
  
  Input(){};
  Input(int n,double q,double r,double E,string dat);
  void Write(const char out[],ios_base::openmode mode=ios::out);
};

Input::Input(int n,double q,double r,double E,string dat){
  np=n;qe=q;rc=r;Eref=E;data=dat;
};

void Input::Write(const char out[],ios_base::openmode mode){
  ofstream fout;
  fout.open(out,mode);
  fout.setf(ios_base::boolalpha);
  fout<<boolalpha;
  fout<<"&INPUT"<<endl;
  fout<<" FNAME='"<<data<<"'"<<endl;
  fout<<" Add=FALSE,	N_add=0,"<<endl;
  fout<<" IPart="<<np<<endl;
  fout<<" Species='positrons'"<<endl;
  fout<<" Probe=F,	Noise_reduc=T,		Cathode=T"<<endl;
  fout<<" Q_total="<<qe<<endl;
  fout<<" "<<endl;
  fout<<" Ref_zpos=0.0E0"<<endl;
  fout<<" Ref_Ekin="<<Eref<<endl;
  fout<<" "<<endl;
  fout<<" Dist_z='g',     sig_clock=14.5E-3,	C_sig_clock=2.0"<<endl;
  //fout<<" Dist_z='u',     Lt=0.175"<<endl;
  fout<<" Dist_pz='g', 	  sig_Ekin=0,	cor_Ekin=0.0E0"<<endl;
  fout<<" "<<endl;
  fout<<" Dist_x='r',   Lx="<<rc<<endl;
  fout<<" Dist_px='g',  Nemit_x=.0E0,	cor_px=0.0E0"<<endl;
  fout<<" Dist_y='r',   Ly="<<rc<<endl;
  fout<<" Dist_py='g',  Nemit_y=.0E0,	cor_py=0.0E0"<<endl;
  fout<<"/"<<endl;
  fout<<endl;
  fout.close();
};

class Newrun{
 public:
  string title;
  string data;
  bool autophase;
  
  Newrun(){};
  Newrun(string tit,string dat,bool aut=true):title(tit),data(dat),autophase(aut){};
  void Write(const char out[],ios_base::openmode mode=ios::app){
    ofstream fout;
    fout.open(out,mode);
    fout.setf(ios_base::boolalpha);
    fout<<boolalpha;
    fout<<"&NEWRUN"<<endl;
    fout<<" Head='"<<title<<"'"<<endl;
    fout<<" RUN=1"<<endl;
    fout<<" Distribution='"<<data<<"'"<<endl;
    fout<<" Xoff=0.0,	Yoff=0.0,"<<endl;
    fout<<" TRACK_ALL=T,    Auto_phase="<<autophase<<endl;
    fout<<" H_max=0.01,     H_min=0.00"<<endl;
    fout<<"/"<<endl;
    fout<<endl;
    fout.close();
  };
};

class Cavity{
 public:
  int num;
  double *pos;  // m
  double *fmax; // MV/m
  double *freq; // GHz
  double *phi;  // deg
  string *data;
  bool LE;

  Cavity(){num=0;};
  Cavity(int n);
  int Size(){return num;};
  void Build(int i,double p,double fm,double fq,double ph,string dat,bool l=true);
  void Write(const char out[],ios_base::openmode mode=ios::app);
};

Cavity::Cavity(int n){
  num=n;
  pos=new double[num];
  fmax=new double[num];
  freq=new double[num];
  phi=new double[num];
  data=new string[num];
}

void Cavity::Build(int i,double p,double fm,double fq,double ph,string dat,bool l){
  pos[i]=p;fmax[i]=fm;freq[i]=fq;phi[i]=ph;data[i]=dat;LE=l;
}

void Cavity::Write(const char out[],ios_base::openmode mode){
  ofstream fout;
  fout.open(out,mode);
  fout.setf(ios_base::boolalpha);
  fout<<boolalpha;
  fout<<"&CAVITY"<<endl;
  fout<<" LEField="<<LE<<","<<endl;
  for(int i=0;i<num;i++){
    fout<<" File_Efield("<<i+1<<")='"<<data[i]<<"'"<<endl;
    fout<<" C_pos("<<i+1<<")="<<pos[i]<<endl;
    fout<<" Nue("<<i+1<<")="<<freq[i]<<endl;
    fout<<" MaxE("<<i+1<<")="<<fmax[i]<<endl;
    fout<<" Phi("<<i+1<<")="<<phi[i]<<endl;
    fout<<" C_smooth(1)=3, C_higher_order(1)=T"<<endl;
  }
  fout<<"/"<<endl;
  fout<<endl;
  fout.close();
}

class Solenoid{
 public:
  int num;
  double *pos;  // m
  double *fmax; // T
  string *data;
  bool LB;

  Solenoid(){num=0;};
  Solenoid(int n);
  int Size(){return num;};
  void Build(int i,double p,double fm,string dat,bool l=true);
  void Write(const char out[],ios_base::openmode mode=ios::app);
};

Solenoid::Solenoid(int n){
  num=n;
  pos=new double[num];
  fmax=new double[num];
  data=new string[num];
}

void Solenoid::Build(int i,double p,double fm,string dat,bool l){
  pos[i]=p;fmax[i]=fm;data[i]=dat;LB=l;
}

  void Solenoid::Write(const char out[],ios_base::openmode mode){
  ofstream fout;
  fout.open(out,mode);
  fout.setf(ios_base::boolalpha);
  fout<<boolalpha;
  fout<<"&SOLENOID"<<endl;
  fout<<" LBField="<<LB<<","<<endl;
  for(int i=0;i<num;i++){
    fout<<" File_Bfield("<<i+1<<")='"<<data[i]<<"'"<<endl;
    fout<<" S_pos("<<i+1<<")="<<pos[i]<<endl;
    fout<<" MaxB("<<i+1<<")="<<fmax[i]<<endl;
    fout<<" S_smooth(1)=3, S_higher_order(1)=T"<<endl;
  }
  fout<<"/"<<endl;
  fout<<endl;
  fout.close();
}

class Output{
 public:
  double zstart;
  double zstop;

  int zemit;
  int zphase;

  int nscr;
  double *screen;

  Output(){};
  Output(double zsta,double zsto,int zemi=100,int zpha=10,int n=0,double v[]=0){
    zstart=zsta;zstop=zsto;zemit=zemi;zphase=zpha;nscr=n;screen=new double[n];
    for(int i=0;i<n;i++) screen[i]=v[i];
  }
  void Write(const char out[],ios_base::openmode mode=ios::app);
};

void Output::Write(const char out[],ios_base::openmode mode){
  ofstream fout;
  fout.open(out,mode);
  fout.setf(ios_base::boolalpha);
  fout<<boolalpha;
  fout<<"&OUTPUT"<<endl;
  fout<<" Lmagnetized=F"<<endl;
  fout<<" ZSTART="<<zstart<<",	  ZSTOP="<<zstop<<endl;
  fout<<" Zemit="<<zemit<<",      Zphase="<<zphase<<endl;
  for(int i=0;i<nscr;i++){
    fout<<" Screen("<<i+1<<")="<<screen[i]<<endl;
  }
  fout<<" RefS=T,        TrackS=T"<<endl;
  fout<<" EmitS=T,       PhaseS=T"<<endl;
  fout<<" TcheckS=T"<<endl;
  fout<<"/"<<endl;
  fout<<"\n"<<endl;
  fout.close();
};

class Charge{
 public:
  bool lspch;
  int Nrad;
  int Nlong;
 
  Charge(){lspch=true;Nrad=30;Nlong=100;};
  Charge(int nr,int nl,bool l=true);
  void Write(const char out[],ios_base::openmode mode=ios::app);
};

Charge::Charge(int nr,int nl,bool l){
  Nrad=nr;Nlong=nl;lspch=l;
};

void Charge::Write(const char out[],ios_base::openmode mode){
  ofstream fout;
  fout.open(out,mode);
  fout.setf(ios_base::boolalpha);
  fout<<boolalpha;
  fout<<"&CHARGE"<<endl;
  fout<<" LSPCH="<<lspch<<endl;
  fout<<" Lmirror=T"<<endl;
  fout<<" Nrad="<<Nrad<<endl;
	fout<<" Nlong_in="<<Nlong<<endl;
  fout<<" Cell_var=2.0"<<endl;
  fout<<" min_grid=0.0"<<endl;
  fout<<" Max_scale=0.1"<<endl;
  fout<<" Max_count=100"<<endl;
  fout<<"/"<<endl;
  fout<<endl;
  fout.close();
};

