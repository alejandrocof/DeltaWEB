#ifndef DATA2D_H
#define DATA2D_H

#include <vector>
#include <string>
#include <iostream>

#include <unsupported/Eigen/FFT>


using namespace std;

class Data2D;
class DataFou;

Data2D smoothing(const Data2D& X, int Fs=5);
Data2D taper(const Data2D& A, double R=6.0);
Data2D Lowpass( const Data2D &A, double Fmax,int NPolos=4);
Data2D Highpass( const Data2D &A, double Fmin,int NPolos=4);
Data2D Bandpass( const Data2D &A, double Fmin, double Fmax,int NPolos=4);

class Data2D
{
public:
    string name_raw;
    string name;
    string nameFile;
    string date;
    string component;
    double ti;
    double dt;
    vector< double > y;
    double max;


    Data2D();
    //Data2D(const Data2D &);

    friend DataFou fou(const Data2D & X);
    friend Data2D abs(const DataFou & X);
    friend Data2D smoothing(const Data2D&, int);
    friend Data2D taper(const Data2D &A, double);
    friend Data2D ResponseSpectrum(Data2D& A,int NT,double Tmin,double Tmax,double z);
    friend double ResponseSpectrum(Data2D& A,double T,double z);
    friend Data2D Derive( const Data2D &A);
    friend Data2D Integrate( const Data2D &A);
    friend Data2D Integrate2( const Data2D &A);
    friend Data2D Lowpass( const Data2D &A, double Fmax,int NPolos);
    friend Data2D Highpass( const Data2D &A, double Fmin,int NPolos);
    friend Data2D Bandpass( const Data2D &A, double Fmin, double Fmax,int NPolos);
    friend Data2D IntegrateTrapecio( const Data2D &A);
};

class DataFou
{
public:

    string name_raw;
    string name;
    string component;
    double fi;
    double df;
    vector< std::complex<double> > Y;

    DataFou();
    //DataFou(const DataFou &);

    friend DataFou fou(const Data2D& X);
    friend Data2D abs(const DataFou& X);
};

//Data2D Smoothing(Data2D x, int Fs);

template <class T>
T max_element_abs ( std::vector<T> &A )
{
  auto first=A.begin();
  auto last=A.end();
  if (first==last) return std::abs(*last);

  auto largest = first;

  while (++first!=last)
      if (std::abs(*largest)<std::abs(*first))
      largest=first;
  return std::abs(*largest);
}

#endif // DATA2D_H
