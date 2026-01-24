#include <iostream>
using namespace std;
int i,j,jopt,k,J;
long long int bg,bgopt,m,n,SET[5000][2],MAXDIM,DIM,BN,B,H,HV,OPP,OPPBEST=100000000;
double rc[5000];

int gd(int a,int b){
    int r=a%b;
    while(r){
        a=b;b=r;r=a%b;
    }
    return b;
}
void qs(int left, int right)
{
	int i,j;
	long long int y,z,t;
	double x,r,s;
	i=left;j=right;
	x=rc[(left+right)/2];
	do{	while((rc[i]>x)&&(i<right))i++;
		while((x>rc[j])&&(j>left))j--;
		if(i<=j){
			y=SET[i][0];z=SET[i][1];r=rc[i];
			SET[i][0]=SET[j][0];SET[i][1]=SET[j][1];rc[i]=rc[j];
			SET[j][0]=y;SET[j][1]=z;rc[j]=r;
			i++;j--;
		};
	} while (i<=j);
	if(left<j)qs(left,j);
	if(i<right)qs(i,right);
}
long long int nwopp(){
    long long int hvv=0;long long int ans=0;
    for(int k=1;k<=J;k++){
        ans+=SET[k][0]*(4*hvv+2+2*SET[k][1]);
        hvv+=SET[k][1];
    }
    ans+=2*hvv+1;
    return ans;
}

int main(){

for(bg=1;bg<=30;bg++){
J=0;B=H=0;
    for(m=1;m<=249;m++){
        for(n=1;n<=bg;n++){
            if(gd(m,n)==1){
                SET[++J][0]=m;SET[J][1]=n;rc[J]=(double)n/(double)m;B+=m;H+=n;
            }
        }
    }
    qs(1,J);
    OPP=nwopp();
    while(J>=250){
        MAXDIM=0;HV=0;BN=B-SET[1][0];
        for(j=1;j<=J;j++){
            DIM=(4*HV+2)*SET[j][0]+(4*BN+2)*SET[j][1]+2*SET[j][0]*SET[j][1];
            if(DIM>=MAXDIM){
                MAXDIM=DIM;jopt=j;
            }
            HV+=SET[j][1];BN-=SET[j+1][0];
        }
        OPP-=MAXDIM;
        B-=SET[jopt][0];H-=SET[jopt][1];
        if(jopt<J){
            for(k=jopt;k<J;k++){
                SET[k][0]=SET[k+1][0];SET[k][1]=SET[k+1][1];rc[k]=rc[k+1];
            }
        }
        J--;
    }
    if(OPP<OPPBEST){
        OPPBEST=OPP;bgopt=bg;
    }
}
printf("%d\n",bgopt);
printf("%d\n",OPPBEST);
return 0;
}