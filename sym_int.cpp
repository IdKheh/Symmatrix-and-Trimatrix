#include <iostream>
#include <vector>
#include "funkcje.h"

using namespace std;
using namespace interval_arithmetic;

bool przedzial::wyznacznik(vector <vector <Interval <long double>>> macierz,int n)
{
    Interval<long double> det=Interval<long double>(1, 1);
    vector< vector<Interval <long double>>> macierz_prim (n,vector< Interval <long double>> (n,Interval<long double>(0, 0)));
    vector< vector<Interval <long double>>> L (n,vector<Interval <long double>> (n,Interval<long double>(0, 0)));
    vector< vector<Interval <long double>>> U (n,vector<Interval <long double>> (n,Interval<long double>(0, 0)));

    for(int k=0;k<n-1;k++)
    {
        for(int i=k;i<n;i++)
            if((macierz[k][k].a<macierz[i][k].a)&&(macierz[k][k].b<macierz[i][k].b))
                for(int j=0;j<n;j++)
                    swap(macierz[k][j],macierz[i][j]);

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                macierz_prim[i][j]=macierz[i][j];

        for(int i=k+1;i<n;i++)
        {
            Interval<long double> Gauss=macierz[i][k]/macierz[k][k];
            for(int j=k;j<n;j++)
            {
                if(i==k) macierz_prim[i][j]=macierz[i][j];
                else if((j==k)&&(i!=k)) macierz_prim[i][j]=Gauss; 
                else macierz_prim[i][j]=macierz[i][j]-Gauss*macierz[k][j];
            }
        }
        for(int i=k+1;i<n;i++)
            for(int j=0;j<n;j++)
                macierz[i][j]=macierz_prim[i][j];
    }

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if(i>j) L[i][j]=macierz_prim[i][j];
            else if(i==j)
            {
                L[i][i]=Interval<long double>(1, 1);
                U[i][i]=macierz_prim[i][i];
            }
            else U[i][j]=macierz_prim[i][j];
        }
    for(int i=0;i<n;i++)
        det=det*U[i][i];

    if((det.a!=0)&&(det.b!=0)) return true;
    return false;
}

bool przedzial::symetryczna(vector <vector <Interval <long double>>> a,int n)
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if((a[i][j].a!=a[j][i].a)||(a[i][j].b!=a[i][j].b)) return false;
    return true;   
}

void przedzial::symmatrix(int n, vector <vector <Interval <long double>>> a,vector <Interval <long double>> b, vector <Interval <long double>> &x)
{
    vector< vector<Interval <long double>>> macierz (n,vector<Interval <long double>> (n,Interval <long double>(0,0)));
    vector< vector<Interval <long double>>> L (n,vector<Interval <long double>> (n,Interval <long double>(0,0)));
    vector< vector<Interval <long double>>> U (n,vector<Interval <long double>> (n,Interval <long double>(0,0)));
    vector <Interval <long double>> y(n,Interval <long double>(0,0));

    for(int k=0;k<n-1;k++)
    {
        for(int i=k;i<n;i++)
            if((a[k][k].a<a[i][k].a)&&(a[k][k].b<a[i][k].b))
                for(int j=0;j<n;j++)
                    swap(a[k][j],a[i][j]);

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                macierz[i][j]=a[i][j];

        for(int i=k+1;i<n;i++)
        {
            Interval <long double> Gauss=a[i][k]/a[k][k];
            for(int j=k;j<n;j++)
            {
                if(i==k) macierz[i][j]=a[i][j];
                else if((j==k)&&(i!=k)) macierz[i][j]=Gauss; 
                else macierz[i][j]=a[i][j]-Gauss*a[k][j];
            }
        }
        for(int i=k+1;i<n;i++)
            for(int j=0;j<n;j++)
                a[i][j]=macierz[i][j];
    }

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if(i>j) L[i][j]=macierz[i][j];
            else if(i==j)
            {
                L[i][i]=Interval <long double> (1,1);
                U[i][i]=macierz[i][i];
            }
            else U[i][j]=macierz[i][j];
        }

    Interval <long double> suma;
    for(int i=0;i<n;i++)
    {
        suma=Interval <long double> (0,0);
        for(int j=0;j<=i-1;j++)
        {
            suma=suma+L[i][j]*y[j];
        }
        y[i]=b[i]-suma;
    }
    
    for(int i=n-1;i>=0;i--)
    {
        suma=Interval <long double> (0,0);
        for(int j=i+1;((j<n)&&(j>=0));j++)
        {
            suma=suma+L[j][i]*x[j];
        }
        x[i]=y[i]/U[i][i]-suma;
    }
}



    //vector< vector<Interval <long double>>> a (n,vector<Interval <long double>> (n,Interval <long double>(0,0)));
    //vector <Interval <long double>> b (n,Interval <long double>(0,0)); vector <Interval <long double>> x (n,Interval <long double>(0,0)); 
    //if((n>=1)&&(wyznacznik(a,n))&&(symetryczna(a,n))) symmatrix(n,a,b,x);