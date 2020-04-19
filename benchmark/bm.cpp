#include <bits/stdc++.h>
using namespace std;
int main()
{
    double tgt[22]={209320,143721,512361,12528,27513,10178,17976,102378,2097170,247926,983128,986008,961324,976965,971266,603910,2026626,3455242,57986,340047,20557,22327};
    string s;
    ifstream arq("bm.txt");
    ofstream arq2("bm_avg.txt");
    for (int i=0;i<22;i++){
        getline(arq,s);
        arq2<<s;
        double custo=0,tempo=0;
        for (int j=0;j<10;j++){
            getline(arq,s);
            stringstream r;
            double t;
            r<<s.substr(7,s.size()-7);
            r>>t;
            tempo+=t;
            getline(arq,s);
            stringstream p;
            double c;
            p<<s.substr(7,s.size()-7);
            p>>c;
            custo+=c;
            getline(arq,s);
        }
       double pct = ( (custo/10 - tgt[i]) / tgt[i] ) * 100;
       arq2<<"\nTempo: "<<tempo/10<<"\nCusto: "<<custo/10<<endl<<pct<<"%"<<endl<<endl;

    }
    arq.close(); 
    arq2.close();
    return 0;
}

