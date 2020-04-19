#include "readData.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <limits>
#include <chrono>
#include <bits/stdc++.h>

using namespace std;

double ** matrizAdj; // matriz de adjacencia
int dimension; // quantidade total de vertices

struct CL{
    int no;
    double custo;
};

bool cmp (CL a, CL b){
    return a.custo<b.custo;
}

void construction(vector<int> &s1){
    vector<int> candidatos;
    s1={1};
    for (int i=2;i<=dimension;i++){
        candidatos.push_back(i);
    }

    int tam=0,adicionar=dimension-1;
    while (tam<adicionar){
        CL lista[adicionar-tam];
        for (int j=0;j<adicionar-tam;j++){
            lista[j].custo=matrizAdj[s1[tam]][candidatos[j]];
            lista[j].no=j;
        }
        sort(lista,lista+adicionar-tam,cmp);

        int a=round(((rand() % 26)/100.0)*(adicionar-tam));
        if (a==0) a=1;
        
        int e=rand()%a;

        s1.push_back(candidatos[lista[e].no]);
        candidatos.erase(candidatos.begin()+lista[e].no);
        tam++;
    }
    s1.push_back(1);
}

void reOptPreProcessing(vector<int> &s1, vector<vector<vector<long double>>> &reOpt) {
  
    reOpt[2][0][0]=0;
    for (int i=1;i<=dimension;i++){
        reOpt[0][i][i]=0;
        reOpt[1][i][i]=0;
        reOpt[2][i][i]=1;
    }
    for (int t=2;t<=dimension+1;t++){
        for (int i=0,j;i<dimension-t+2;i++){

            j=i+t-1;
            reOpt[0][i][j]=reOpt[0][i][j-1]+matrizAdj[s1[j-1]][s1[j]];
            reOpt[0][j][i]=reOpt[0][i][j];
            
            reOpt[1][i][j]=reOpt[1][i][j-1]+reOpt[0][i][j];
            reOpt[1][j][i]=reOpt[1][j-1][i]+matrizAdj[s1[j]][s1[j-1]]*(j-i);

            reOpt[2][i][j]=reOpt[2][i][j-1]+reOpt[2][j][j];
            reOpt[2][j][i]=reOpt[2][i][j];
        
        }
    }
}

int Swap(vector<int> &s1, double &custo1, vector<vector<vector<long double>>> &reOpt){

    double d=0,t=0,t0=0;
    int x,y;
    for (int i=1;i<dimension-1;i++){

        t0=-reOpt[2][i][dimension]*matrizAdj[s1[i]][s1[i-1]];

        for (int j=i+1;j<dimension;j++){

            t= t0+ reOpt[2][i][dimension]*matrizAdj[s1[i-1]][s1[j]]+
            reOpt[2][j+1][dimension]*(matrizAdj[s1[j+1]][s1[i]]-matrizAdj[s1[j+1]][s1[j]]);

            if (j!=i+1) {
                t+= reOpt[2][i+1][dimension]*(matrizAdj[s1[i+1]][s1[j]]-matrizAdj[s1[i]][s1[i+1]])+
                reOpt[2][j][dimension]*(matrizAdj[s1[j-1]][s1[i]]-matrizAdj[s1[j-1]][s1[j]]);
            }

            if (t<d) {
                d=t;
                x=i;
                y=j;
            }
        }
    }
    
    if (d<0){
        
        swap(s1[x],s1[y]);
        return 0;
    }
    
    return 1;
}

int Opt_2(vector<int> &s1, double &custo1, vector<vector<vector<long double>>> &reOpt){
    
    double d=0,t=0,t0=0;
    int x,y;
    for (int i=1;i<dimension-4;i++){
        
        t0=-matrizAdj[s1[i]][s1[i-1]]*reOpt[2][i][dimension];

        for (int j=i+3;j<dimension;j++){

            t= t0+ reOpt[1][j][i]-reOpt[1][i][j]+
            reOpt[2][i][dimension]*matrizAdj[s1[i-1]][s1[j]]+
            reOpt[2][j+1][dimension]*(matrizAdj[s1[i]][s1[j+1]]-matrizAdj[s1[j]][s1[j+1]]);

            if (t<d){
                d=t;
                x=i;
                y=j+1;
            }
        }
    }

    if(d<0){
    
        reverse(s1.begin()+x,s1.begin()+y);
        return 0;
    }

    return 1;
}

int N_reinsertion(vector<int> &s1, double &custo1, vector<vector<vector<long double>>> &reOpt, int n){

    double d=0,t=0,t0=0;
    int x,y,r[n],b=0;
    if (n==3) b=1;
    for (int i=1;i<dimension-n+1;i++){
        
        t0=-reOpt[2][i][dimension]*matrizAdj[s1[i]][s1[i-1]]-
        reOpt[2][i+n][dimension]*matrizAdj[s1[i+n-1]][s1[i+n]];

        for (int j=0;j<dimension;j++){

            if (j>=i-n-1 && j<i+n+b+1){
                j=i+n+b;
                continue;
            }
            
            if (i>j){
                t= t0+ reOpt[0][i][i+n-1]*reOpt[2][j+1][i-1]-n*reOpt[0][j+1][i-1]+
                matrizAdj[s1[j+1]][s1[i+n-1]]*(reOpt[2][j+1][dimension]-n)+
                reOpt[2][i+n][dimension]*matrizAdj[s1[i-1]][s1[i+n]]+
                reOpt[2][j+1][dimension]*(matrizAdj[s1[j]][s1[i]]-matrizAdj[s1[j]][s1[j+1]]);
            }
            else {
                t= t0+ reOpt[0][i+n][j]*n-reOpt[0][i][i+n-1]*reOpt[2][i+n][j]+                
                matrizAdj[s1[j]][s1[i]]*(reOpt[2][j+1][dimension]+n)+
                reOpt[2][i][dimension]*matrizAdj[s1[i-1]][s1[i+n]]+
                reOpt[2][j+1][dimension]*(matrizAdj[s1[j+1]][s1[i+n-1]]-matrizAdj[s1[j]][s1[j+1]]);
            }

            if (t<d){
                d=t;
                x=i;
                y=j+1;
            }
        }
    }
    
    if (d<0){
        
        for (int i=0;i<n;i++){
            r[i]=s1[x+i];
        }

        if (y-1>x) y-=n;
        
        s1.erase(s1.begin()+x,s1.begin()+x+n);
        s1.insert(s1.begin()+y,r,r+n);
        return 0;
    }
    
    return 1;
}

void rvnd(vector<int> &s1, double &custo1, vector<vector<vector<long double>>> &reOpt){
    
    vector<int> v={1,2,3,4,5};
    while (!v.empty()){ 
        int e=rand()%v.size(),c=1;

        switch(v[e]){
            case 1:
                c = Swap(s1,custo1,reOpt);
                break;
            case 2:
                c = Opt_2(s1,custo1,reOpt);
                break;
            case 3:
                c = N_reinsertion(s1,custo1,reOpt,1);
                break;
            case 4:
                c = N_reinsertion(s1,custo1,reOpt,2);
                break;
            case 5:
                c = N_reinsertion(s1,custo1,reOpt,3);
                break;
        }

        if (c==1) v.erase(v.begin()+e);
        else {
            v={1,2,3,4,5};
            reOptPreProcessing(s1,reOpt);
            custo1=reOpt[1][0][dimension];
        }
    }
}

void perturb(vector<int> &s1){
    
    int t=dimension/10;
    if (dimension<=30) t=3;
    int r[4];
    r[0]=rand()%(dimension-5);
    r[1]=rand()%(min(dimension-5-r[0],t-2))+r[0]+2;
    r[2]=rand()%(dimension-3-r[1])+r[1]+1;
    r[3]=rand()%(min(dimension-2-r[2],t-2))+r[2]+2;

    vector<int> sub1(s1.begin()+r[0]+1,s1.begin()+r[1]+1);
    vector<int> sub2(s1.begin()+r[2]+1,s1.begin()+r[3]+1);
    
    s1.erase(s1.begin()+r[0]+1,s1.begin()+r[1]+1);
    s1.erase(s1.begin()+r[2]+1-sub1.size(),s1.begin()+r[3]+1-sub1.size());
    s1.insert(s1.begin()+r[0]+1,sub2.begin(),sub2.end());
    s1.insert(s1.begin()+r[2]+1-sub1.size()+sub2.size(),sub1.begin(),sub1.end());

}

int main(int argc, char** argv)
{
	int imx=10,ils;
	double custo_min=numeric_limits<double>::max(),custo1=0,custo2=0;
	vector<int> bestS,s1,s2;
    bestS.reserve(110);
    s1.reserve(110);
    s2.reserve(110);
    srand (time(NULL));

    readData(argc, argv, &dimension, &matrizAdj);

    auto start = chrono::system_clock::now();

    vector<vector<vector<long double>>> reOpt(3,vector<vector<long double>>(dimension+1,vector<long double>(dimension+1, 0)));

    if (dimension>=100) ils=100;
    else ils=dimension+1;

    for (int m=0;m<imx;m++){
        
        construction(s1);
        reOptPreProcessing(s1,reOpt);
        custo1=reOpt[1][0][dimension];

        s2=s1;
        custo2=custo1;
        int n=0;
        while (n<ils){
            
            rvnd(s1,custo1,reOpt);

            if (custo1<custo2){
                s2=s1;
                custo2=custo1;
                n=-1;
            }
            else {
                s1=s2;
                custo1=custo2;
            }
            
            perturb(s1);
            reOptPreProcessing(s1,reOpt);
            custo1=reOpt[1][0][dimension];
            
            n++;
        }
        if (custo2<custo_min){
            custo_min=custo2;
            bestS=s2;
        }
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> time=end-start;
    
    /*printf("Solution: ");
    for (int i=0;i<dimension+1;i++){
        printf("%i ",bestS[i]);
    }
    printf("\n\n");*/

    printf("Tempo: %lf\nCusto: %lf\n\n",time,custo_min);
    
    return 0;
}
