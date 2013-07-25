//This code manipulates one collum of data from a text file, including reading them into an array, and getting the maximum or minimum.
//The first line of .txt file should be variables and the following lines are values of variables. 

char* parse(char *str);

int isSpaceLine(char *str);
int isThereDigit(char *str);

double* txt2array(int *len,const char in[],const char col[],double coe=1,double offset=0);
double* txt2array(int *len,const char in[],int flag,double coe=1,double offset=0);

double GetColMin(const char in[],int flag,int st=0,int end=0);
double GetColMax(const char in[],int flag,int st=0,int end=0);

double GetColMin(const char in[],const char col[],int st=0,int end=0);
double GetColMax(const char in[],const char col[],int st=0,int end=0);
