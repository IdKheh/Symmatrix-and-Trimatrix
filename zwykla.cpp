#include <iostream>
#include <vector>
#include "funkcje.h"
using namespace std;


bool zwykla::trimatrix(int n, vector <long double> &a ,vector <long double> &b, vector <long double> &c,vector <long double> &d)
{
    vector <long double> x(n,0);
    vector <long double> u(n,0);
    vector <long double> y(n,0);

    y[0]=d[0]/a[0];
    u[0]=b[0]/a[0];

    for(int i=1;i<n;i++)
    {
        if((a[i]-u[i-1]*c[i])==0) return false;
        u[i]=b[i]/(a[i]-u[i-1]*c[i]);
    }
    for(int i=1;i<n;i++)
    {
        if((a[i]-u[i-1]*c[i])==0) return false;
        y[i]=(d[i]-c[i]*y[i-1])/(a[i]-u[i-1]*c[i]);
    }


    for(int i=n-1;i>=0;i--)
    {
        if(a[i]-u[i-1]*c[i]==0) return false;
        if(i==n-1) x[i]=y[i];
        else x[i]=y[i]-u[i]*x[i+1];
    }
    for(int i=0;i<n;i++)
        d[i]=x[i];
    return true;
}

bool zwykla::wyznacznik(vector< vector <long double> > macierz,int n)
{
    double long det=1;
    vector< vector<long double>> macierz_prim (n,vector<long double> (n,0));
    vector< vector<long double>> L (n,vector<long double> (n,0));
    vector< vector<long double>> U (n,vector<long double> (n,0));

    for(int k=0;k<n-1;k++)
    {
        for(int i=k;i<n;i++)
            if(macierz[k][k]<macierz[i][k])
                for(int j=0;j<n;j++)
                    swap(macierz[k][j],macierz[i][j]);

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                macierz_prim[i][j]=macierz[i][j];

        for(int i=k+1;i<n;i++)
        {
            double long Gauss=macierz[i][k]/macierz[k][k];
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
                L[i][i]=1;
                U[i][i]=macierz_prim[i][i];
            }
            else U[i][j]=macierz_prim[i][j];
        }
    for(int i=0;i<n;i++)
        det*=U[i][i];

    if(det!=0) return true;
}

bool zwykla::symetryczna(vector< vector <long double> > a,int n)
{
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(a[i][j]!=a[j][i]) return false;
    return true;   
}

void zwykla::symmatrix(int n, vector< vector <long double> > a ,vector <long double> b, vector <long double> &x)
{
    vector< vector<long double>> macierz (n,vector<long double> (n,0));
    vector< vector<long double>> L (n,vector<long double> (n,0));
    vector< vector<long double>> U (n,vector<long double> (n,0));
    vector <long double> y(n,0);

    for(int k=0;k<n-1;k++)
    {
        for(int i=k;i<n;i++)
            if(a[k][k]<a[i][k])
                for(int j=0;j<n;j++)
                    swap(a[k][j],a[i][j]);

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                macierz[i][j]=a[i][j];

        for(int i=k+1;i<n;i++)
        {
            double long Gauss=a[i][k]/a[k][k];
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
                L[i][i]=1;
                U[i][i]=macierz[i][i];
            }
            else U[i][j]=macierz[i][j];
        }

    long double suma;
    for(int i=0;i<n;i++)
    {
        suma=0;
        for(int j=0;j<=i-1;j++)
        {
            suma+=L[i][j]*y[j];
        }
        y[i]=b[i]-suma;
    }
    
    for(int i=n-1;i>=0;i--)
    {
        suma=0;
        for(int j=i+1;((j<n)&&(j>=0));j++)
        {
            suma+=L[j][i]*x[j];
        }
        x[i]=y[i]/U[i][i]-suma;
    }
}