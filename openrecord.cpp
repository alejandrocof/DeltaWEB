#include "openrecord.h"


OpenRecord::OpenRecord(std::string str)
{
	std::stringstream ss(str);
        int iLine=1;
        std::string line;
        while ( iLine<110 && getline (ss,line) ){
//            cout<<iLine<<endl;
            std::vector <std::string> fields;
            //std::vector <std::string> fields2;
            switch(iLine){
            case 9:
                //split( fields, line, is_any_of( ":" ) ); ///cambiar is_any_of
                splitStrings( fields, line, ':' );
                std::stringstream(fields[1])>>fileName;
                cout<<"NOMBRE DEL ARCHIVO:"<<fields[1]<<std::endl;
                break;
            case 17:
                //split( fields, line, is_any_of( ":" ) );
                splitStrings( fields, line, ':' );
                std::stringstream(fields[1])>>clave;
                cout<<"CLAVE DE LA ESTACION:"<<clave<<std::endl;
                break;
            case 23:
                //split( fields, line, is_any_of( ":" ) );
                //split( fields, fields[1], is_any_of( " " ) );
                splitStrings( fields, line, ':' );
                splitStrings( fields, fields[1], ' ' );
                std::stringstream(fields[1])>>latitudEstacion;
                std::cout<<"COORDENADAS DE LA ESTACION:"<<std::endl;
                std::cout<<"\t"<<latitudEstacion<<endl;
                break;
            case 24:
                //split( fields, line, is_any_of( ":" ) );
                //split( fields, fields[1], is_any_of( " " ) );
                splitStrings( fields, line, ':' );
                splitStrings( fields, fields[1], ' ' );
                std::stringstream(fields[1])>>longitudEstacion;
                std::cout<<"\t"<<longitudEstacion<<std::endl;
                break;
            case 37:
                //split( fields, line, is_any_of( "/" ) );
                splitStrings( fields, line, '/' );
                std::stringstream(fields[1])>>sC1;
                std::stringstream(fields[2])>>sC2;
                std::stringstream(fields[3])>>sC3;
                vOrientacion[0]=sC1;
                vOrientacion[1]=sC2;
                vOrientacion[2]=sC3;
                std::cout<<"ORIENTACION:"<<sC1<<" "<<sC2<<" "<<sC3<<std::endl;
                break;
            case 47:
                //split( fields, line, is_any_of( "/" ) );
                splitStrings( fields, line, '/' );
                std::stringstream(fields[1])>>intervaloMuestreo[0];
                std::stringstream(fields[2])>>intervaloMuestreo[1];
                std::stringstream(fields[3])>>intervaloMuestreo[2];
                std::cout<<"INTERVALO DE MUESTREO:"<<intervaloMuestreo[0]<<" "<<intervaloMuestreo[1]<<" "<<intervaloMuestreo[2]<<std::endl;
                break;
            case 57:
                //split( fields, line, is_any_of( ":" ) );
                splitStrings( fields, line, ':' );
                std::stringstream(fields[1])>>date;
                std::cout<<"FECHA DEL SISMO:"<<date<<std::endl;
                break;
            case 58:
                //split( fields, line, is_any_of( ":" ) );
                splitStrings( fields, line, ':' );
                std::stringstream(fields[1])>>tiempo_hEpicentro;
                std::stringstream(fields[2])>>tiempo_mEpicentro;
                std::stringstream(fields[3])>>tiempo_sEpicentro;
                std::cout<<"HORA EPICENTRO:"
                    <<tiempo_hEpicentro<<":"
                    <<tiempo_mEpicentro<<":"
                    <<tiempo_sEpicentro<<":"<<std::endl;
                break;
            case 60:
                //split( fields, line, is_any_of( ":" ) );
                //split( fields, fields[1], is_any_of( " " ) );
                splitStrings( fields, line, ':' );
                splitStrings( fields, fields[1], ' ' );
                std::stringstream(fields[1])>>latitudEpicentro;
                std::cout<<"COORDENADAS DEL EPICENTRO:"<<std::endl;
                std::cout<<"\t"<<latitudEpicentro<<std::endl;
                break;
            case 61:
                //split( fields, line, is_any_of( ":" ) );
                //split( fields, fields[1], is_any_of( " " ) );
                splitStrings( fields, line, ':' );
                splitStrings( fields, fields[1], ' ' );
                std::stringstream(fields[1])>>longitudEpicentro;
                std::cout<<"\t"<<longitudEpicentro<<std::endl;
                break;
//                Algunos archivos no incluyen el tiempo y al procesar el split se genera un error
//            case 68:
//                split( fields, line, is_any_of( ":" ) );
//                stringstream(fields[1])>>tiempo_hPrimerMuestra;
//                stringstream(fields[2])>>tiempo_mPrimerMuestra;
//                stringstream(fields[3])>>tiempo_sPrimerMuestra;
//                cout<<"HORA DE LA PRIMERA MUESTRA (GMT):"
//                    <<tiempo_hPrimerMuestra<<":"
//                    <<tiempo_mPrimerMuestra<<":"
//                    <<tiempo_sPrimerMuestra<<":"<<endl;
//                break;
            case 72:
                //split( fields, line, is_any_of( "/" ) );
                splitStrings( fields, line, '/' );
                std::stringstream(fields[1])>>totalMuestras1;
                std::stringstream(fields[2])>>totalMuestras2;
                std::stringstream(fields[3])>>totalMuestras3;
                std::cout<<"NUM. TOTAL DE MUESTRAS:"
                    <<totalMuestras1<<" "
                    <<totalMuestras2<<" "
                    <<totalMuestras3<<std::endl;
                break;
            }
            iLine++;
        }
        std::cout<<"Termina leer lineas"<<std::endl;
        vC[0].resize(totalMuestras1);
        vC[1].resize(totalMuestras2);
        vC[2].resize(totalMuestras3);
        int iData=0;
        while ( std::getline (ss,line) ){
            std::stringstream sline(line);
            if(iData<totalMuestras1)sline>>vC[0][iData];
            if(iData<totalMuestras2)sline>>vC[1][iData];
            if(iData<totalMuestras3)sline>>vC[2][iData];
            iData++;
        }

}

