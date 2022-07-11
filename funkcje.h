#pragma once
#include <vector>
#include <stdexcept>
#include "Globals.h"
#ifndef FUNKCJE_H
namespace zwykla
{
	bool wyznacznik(std::vector< std::vector <long double> > macierz,int n);
    bool symetryczna(std::vector< vector <long double> > a,int n);
    void symmatrix(int n, std::vector< std::vector <long double> > a ,std::vector <long double> b, std::vector <long double> &x);

    bool trimatrix(int n, std::vector <long double> &a ,std::vector <long double> &b, 
    std::vector <long double> &c,std::vector <long double> &d);
}

namespace przedzial
{
	using namespace interval_arithmetic;

    bool trimatrix(int n, std::vector <Interval <long double>> &a ,std::vector <Interval <long double>> &b, 
    std::vector <Interval <long double>> &c,std::vector <Interval <long double>> &d);
    
    bool wyznacznik(std::vector <std::vector <Interval <long double>>> macierz,int n);
    bool symetryczna(std::vector <std::vector <Interval <long double>>> a,int n);
    void symmatrix(int n, std::vector <std::vector <Interval <long double>>> a,
    std::vector <Interval <long double>> b, std::vector <Interval <long double>> &x);
}
#endif // !funkcje.h