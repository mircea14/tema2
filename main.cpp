#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <algorithm>
#define pb push_back

using namespace std;

int compar(const void *a, const void *b)
{
    int x = *((int*)a);
    int y = *((int*)b);
    if (x-y>0) return 1;
    if (x-y<0) return -1;

    return 0;
}

class Automat
{

protected:

    vector<int> stari;
    string alfabet;
    int stareInit;
    vector<int> stariFinale;

public:

    Automat(){stareInit = -1;}
    vector<int> getStari(){return stari;}
    string getAlfabet(){return alfabet;}
    int getStareInit(){return stareInit;}
    vector<int> getStariFinale(){return stariFinale;}
    void setStareInit(int stare){stareInit = stare;}
    void citire(ifstream&,Automat&);

};

class Nfa: public Automat
{

    vector<int>** tranzitii;

public:

    Nfa():Automat(){tranzitii = NULL;}
    Nfa(int,int);
    vector<int>** getTranzitii(){return tranzitii;}
    friend ifstream& operator>>(ifstream&,Nfa&);
    friend ostream& operator<<(ostream&, Nfa&);

};

class Dfa: public Automat
{

    int **tranzitii;

public:

    Dfa():Automat(){tranzitii=NULL;}
    Dfa(int,int);
    Dfa(vector<int>**,int,int);
    int** getTranzitii(){return tranzitii;}

};

void Automat::citire(ifstream& fin,Automat& a)
{

    int nrStari,nrCuvAlfabet,nrStariFinale;
    fin>>nrStari>>nrCuvAlfabet>>nrStariFinale;
    for(int i=0;i<nrStari;i++)
    {
        int x;
        fin>>x;
        a.stari.pb(x);
    }
    string linie;
    for(int i=0;i<nrCuvAlfabet;i++)
    {
        fin>>linie;
        a.alfabet.pb(linie[0]);
    }
    fin>>a.stareInit;
    for(int i=0;i<nrStariFinale;i++)
    {
        int x;
        fin>>x;
        a.stariFinale.pb(x);
    }

}

Nfa::Nfa(int nrStari,int nrCuvAlfabet):Automat()
{
    tranzitii = new vector<int>*[nrStari];
    for (int i=0;i<nrStari;i++)
    {
        tranzitii[i] = new vector<int>[nrCuvAlfabet+1];
    }

}

ifstream& operator>>(ifstream& fin,Nfa& l)
{

    int nrTranzitii;
    l.citire(fin,l);
    fin>>nrTranzitii;
    l.tranzitii = new vector<int>*[l.stari.size()];
    for (int i=0;i<l.stari.size();i++)
    {
        l.tranzitii[i] = new vector<int>[l.alfabet.size()+1];
    }

    for (int i=0;i<nrTranzitii;i++)
    {
        int x,y;
        char c;
        fin>>x>>c>>y;
        int z = l.alfabet.find(c);
        l.tranzitii[x][z+1].pb(y);
    }

    for (int i=0;i<l.stari.size();i++)
    {
        l.tranzitii[i][0].pb(i);
        for (int j=1;j<=l.alfabet.size();j++)
            std::sort(l.tranzitii[i][j].begin(),l.tranzitii[i][j].end());
    }

    return fin;

}



ostream& operator<<(ostream& out, Nfa& l)
{

    for (int i=0;i<l.stari.size();i++)
    {
        out<<l.stari[i]<<" ";
    }
    out<<endl<<l.alfabet<<endl<<l.stareInit<<endl;
    for(int i=0;i<l.stariFinale.size();i++)
    {
        out<<l.stariFinale[i]<<" ";
    }
    out<<endl;
    for(int i=0;i<l.stari.size();i++)
    {

        for(int j=0;j<l.alfabet.size()+1;j++)
        {

            for(int z=0;z<l.tranzitii[i][j].size();z++)
                out<<l.tranzitii[i][j][z]<<" ";
            out<<" | ";

        }
        out<<endl;
    }
    out<<endl;

    return out;

}

Dfa::Dfa(int nrStari,int nrCuvAlfabet)
{

    tranzitii = new int*[nrStari];
    for(int i=0;i<nrCuvAlfabet;i++)
        tranzitii[i] = new int;

}

Dfa::Dfa(vector<int>** a,int nrStari,int nrCuvAlfabet)
{

    tranzitii = new int*[nrStari];
    for(int i=0;i<nrCuvAlfabet;i++)
        tranzitii[i] = new int;

}

bool cautaElementInVector(int x,vector<int>a)
{

    for(int i=0;i<a.size();i++)
        if(x==a[i])
            return true;
    return false;

}

void reuniuneVectori(vector<int> &a,vector<int> b)
{

    for(int i=0;i<b.size();i++)
    {
        if(!cautaElementInVector(b[i],a)) a.pb(b[i]);
    }
    sort(a.begin(),a.end());

}

bool vectoriEgali(vector<int> a,vector<int> b)
{

    if(a.size()==b.size())
    {
        sort(a.begin(),a.end());
        sort(b.begin(),b.end());
        for (int i=0;i<a.size();i++)
            if(a[i]!=b[i])
                return false;
        return true;
    }

    return false;

}

void dfs(vector<int> nfa[30][30],Nfa lnfa,vector<int> &stiva)
{

    for (int i=1;i<=lnfa.getAlfabet().size();i++)
    {
        if(!(lnfa.getTranzitii()[stiva.back()][i].empty()) && lnfa.getAlfabet()[i-1] != '&')
        {
            reuniuneVectori(nfa[stiva.back()][i],lnfa.getTranzitii()[stiva.back()][i]);
            for(int j=stiva.size()-2;j>=0;j--)
                reuniuneVectori(nfa[stiva[j]][i],nfa[stiva.back()][i]);
        }
        else
        {
            if(lnfa.getAlfabet()[i-1] == '&')
            {
                for(int k=0;k<lnfa.getTranzitii()[stiva.back()][i].size();k++)
                {
                    stiva.pb(lnfa.getTranzitii()[stiva.back()][i][k]);
                    dfs(nfa,lnfa,stiva);
                    stiva.pop_back();
                }
            }
        }
    }

}

void transNfa(vector<int> nfa[30][30],Nfa lnfa)
{
    for (int i=0;i<lnfa.getStari().size();i++)
        nfa[i][0].pb(i);

    vector<int> stiva;
    for(int i=0;i<lnfa.getStari().size();i++)
    {
        stiva.pb(lnfa.getStari()[i]);
        dfs(nfa,lnfa,stiva);
        if(!stiva.empty())
            stiva.pop_back();

    }
}

int transDfa(vector<int> dfa[30][30],Nfa lnfa,vector<int> nfa[30][30])
{

    int nrSt=1;
    dfa[0][0].pb(0);
    for(int i=1;i<lnfa.getAlfabet().size();i++)
    {
        reuniuneVectori(dfa[0][i],nfa[0][i]);
        if (nfa[0][i].size()>0)
        {
            reuniuneVectori(dfa[i][0],nfa[0][i]);
            nrSt++;
        }
    }

    for(int i=1;i<nrSt;i++)
    {
        for(int j=1;j<lnfa.getAlfabet().size();j++)
        {
            for(int k=0;k<dfa[i][0].size();k++)
            {
                reuniuneVectori(dfa[i][j],nfa[dfa[i][0][k]][j]);
            }
            bool ok=false;
            for(int z=0;z<nrSt;z++)
            {
                if(vectoriEgali(dfa[i][j],dfa[z][0])) ok = true;
            }

            if(!ok && dfa[i][j].size()>0)
            {
                reuniuneVectori(dfa[nrSt][0],dfa[i][j]);
                nrSt++;
            }

        }

    }

    return nrSt;

}

int main()
{

    Nfa l;
    ifstream f("date.in");
    f>>l;
    bool iniFin = false;
    vector<int> nfa[30][30];
    transNfa(nfa,l);
    vector<int> dfa[30][30];
    int nrSt;
    nrSt = transDfa(dfa,l,nfa);
    cout<<"Multimea de stari: ";
    for(int i=0;i<nrSt;i++) cout<<i<<" ";
    char *alfabet = new char[l.getAlfabet().size()+1];
    strcpy(alfabet,l.getAlfabet().c_str());
    alfabet[l.getAlfabet().size()-1]='\0';
    cout<<endl<<"Alfabet: "<<alfabet<<endl;
    cout<<"Starea initiala este: 0"<<endl;
    cout<<"Stari finale: ";
    vector<int> fin;
    for(int i=0;i<l.getStariFinale().size();i++)
        for(int j=0;j<nrSt;j++)
        {
            if(cautaElementInVector(l.getStariFinale()[i],dfa[j][0]) && !cautaElementInVector(j,fin))
            {
                fin.pb(j);
                if(cautaElementInVector(0,dfa[j][0]))
                    iniFin = true;
            }
        }
    if (iniFin)
        fin.pb(0);
    sort(fin.begin(),fin.end());
    for(int i=0;i<fin.size();i++)
        cout<<fin[i]<<" ";
    cout<<endl;
    for(int i=0;i<nrSt;i++)
    {
        for(int j=0;j<l.getAlfabet().size();j++)
        {
            for(int z=0;z<dfa[i][j].size();z++)
                cout<<dfa[i][j][z]<<" ";
            cout<<" | ";
        }
        cout<<endl;
    }
    cout<<endl;

    return 0;

}
