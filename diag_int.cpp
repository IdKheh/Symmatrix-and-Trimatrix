#include <iostream>
#include <vector>
#include "funkcje.h"

using namespace std;
using namespace interval_arithmetic;

bool przedzial::trimatrix(int n, vector <Interval <long double>> &a ,vector <Interval <long double>> &b, vector <Interval <long double>> &c,vector <Interval <long double>> &d)
{
    vector < Interval <long double> > x(n, Interval<long double>(0, 0));
    vector < Interval <long double> > y(n, Interval<long double>(0, 0));
    vector < Interval <long double> > u(n, Interval<long double>(0, 0));

    y[0]=d[0]/a[0];
    u[0]=b[0]/a[0];

    for(int i=1;i<n;i++)
    {
        Interval<long double>J(a[i]-u[i-1]*c[i]);
        if(J.a == 0 && J.b == 0) return false;

        u[i]=b[i]/(a[i]-u[i-1]*c[i]);
    }
    for(int i=1;i<n;i++)
    {
        Interval<long double>J(a[i]-u[i-1]*c[i]);
        if(J.a == 0 && J.b == 0) return false;
        y[i]=(d[i]-c[i]*y[i-1])/(a[i]-u[i-1]*c[i]);
    }

    for(int i=n-1;i>=0;i--)
    {
        Interval<long double>J(a[i]-u[i-1]*c[i]);
        if(J.a == 0 && J.b == 0) return false;

        if(i==n-1) x[i]=y[i];
        else x[i]=y[i]-u[i]*x[i+1];
    }
    for(int i=0;i<n;i++)
        d[i]=x[i];
    return true;
}
//if((n>=1)&&trimatrix(n,a,b,c,d)) trimatrix(n,a,b,c,d);
