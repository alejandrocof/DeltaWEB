#ifndef OPENRECORD_H
#define OPENRECORD_H

//#include <boost/algorithm/string.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include "data2d.h"
#include "stringManagement.hpp"

//using namespace std;
//using namespace boost;


class OpenRecord
{
    string fileName;
    string date;
    string clave;
    double latitudEstacion,longitudEstacion;
    double latitudEpicentro,longitudEpicentro;
    int tiempo_hEpicentro,tiempo_mEpicentro;
    double tiempo_sEpicentro;
    int tiempo_hPrimerMuestra,tiempo_mPrimerMuestra;
    double tiempo_sPrimerMuestra;
    string sC1,sC2,sC3;
    
    int totalMuestras1,totalMuestras2,totalMuestras3;
    std::array< double,3> intervaloMuestreo;
    std::array< std::vector< double >, 3 > vC;
    std::array< std::string, 3> vOrientacion;

public:
    OpenRecord(string);
    //void open(string);
    const vector< double > &C(int i){cout<<"regresando:"<<i-1<<endl;return vC[i-1];}
    const std::string &orientacion(int i){cout<<"regresando:"<<i-1<<endl;return vOrientacion[i-1];}
    double dt(int i){return intervaloMuestreo[i-1];}
    string name(){return clave;}
    double LatitudEstacion(){return latitudEstacion;}
    double LongitudEstacion(){return longitudEstacion;}
    string FileName(){return fileName;}
    string Date(){return date;}

};

#endif // OPENRECORD_H
