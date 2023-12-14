// g++ echo_server.cpp openrecord.cpp data2d.cpp -std=c++11 -g -std=gnu++11 -pthread -I ./eigen-eigen-323c052e1731/
//g++ echo_server.cpp openrecord.cpp data2d.cpp plot.cpp -std=c++11 -g -std=gnu++11 -pthread -lboost_iostreams -lboost_system -lboost_filesystem -I ./eigen-eigen-323c052e1731/ -I ./gnuplot-iostream/
//g++ echo_server.cpp openrecord.cpp data2d.cpp plot.cpp stringManagement.cpp -std=c++11 -g -std=gnu++11 -pthread -lboost_iostreams -lboost_system -lboost_filesystem -I ./eigen-eigen-323c052e1731/ -I ./gnuplot-iostream/
#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <string>
#include<vector>

#include "openrecord.h"
#include "plot.h"

typedef websocketpp::server<websocketpp::config::asio> server;

//using namespace std;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

class fileData
{
	//OpenRecord fileR;
	int C_N00E;
	int C_V;
	int C_N90W;
	double theta;

	string namefileN00E;
	string namefileV;
	string namefileN90W;

	Data2D dN90W;
	Data2D dN00E;
	Data2D dV;
	Data2D dHor;
	double longitude;
	double latitude;

	int typeOfData;
	//fileFormat format;

	std::vector<Data2D> vecData2D;
	std::vector<string> vecNamefile;
	std::vector<string> vecName;


public:
	string date;
	string fileName;
	string clave;
	
	fileData(std::string str){
		OpenRecord fileR(str);
		//int C_N00E=2;
		//int C_V=1;
		//int C_N90W=3;
		//vector< double > x(fileR.C(C_N90W));
		//vector< double > y(fileR.C(C_N00E));
		//vector< double > z(fileR.C(C_V));
		vector< double > x(fileR.C(1));
		vector< double > y(fileR.C(2));
		vector< double > z(fileR.C(3));
		cout<<"Arreglos"<<endl;
		latitude=fileR.LatitudEstacion();
		longitude=fileR.LongitudEstacion();
		date=fileR.Date();
		fileName=fileR.FileName();
		clave=fileR.name();
		
		//cout<<"dV"<<endl;

		//dV.component="+V";
		dV.component=fileR.orientacion(1);
		dV.dt=fileR.dt(1);
		dV.name=fileR.name();
		dV.name_raw=fileR.name();
		//dV.dt=fileR.dt(C_V);
		dV.ti=0.0;
		dV.y=z;
		dV.max=max_element_abs( dV.y );
	//dV.max=*std::max_element(dV.y.begin(),dV.y.end());
	
		//cout<<"dN00E"<<endl;

		//dN00E.component="N00E";
		dN00E.component=fileR.orientacion(2);
		dN00E.dt=fileR.dt(2);
		dN00E.name=fileR.name();
		dN00E.name_raw=fileR.name();
		//dN00E.dt=fileR.dt(C_N00E);
		dN00E.ti=0.0;
		//dN00E.y.resize(y.size());
		dN00E.y=y;
		dN00E.max=max_element_abs( dN00E.y );

		//cout<<"dN90W"<<endl;
		//dN90W.component="N90W";
		dN90W.component=fileR.orientacion(3);
		dN90W.dt=fileR.dt(3);
		dN90W.name=fileR.name();
		dN90W.name_raw=fileR.name();
		//dN90W.dt=fileR.dt(C_N90W);
		dN90W.ti=0.0;
		//dN90W.y.resize(x.size());
		dN90W.y=x;
		dN90W.max=max_element_abs( dN90W.y );
	};
	Data2D &N00E(){return dN00E;}
	Data2D &V(){return dV;}
	Data2D &N90W(){return dN90W;}
	Data2D &HOR(){return dHor;}
	double Longitude(){return longitude;}
	double Latitude(){return latitude;}
	string Date(){return date;}
	string FileName(){return fileName;}
	string Clave(){return clave;}
	int Type(){return typeOfData;}
	//fileFormat Format(){return format;}

};

string plot(string name, string date, Data2D &ax, Data2D &ay, Data2D &az){

	cout<<"Plot"<<endl;
	Plot pXY;
	pXY.name="Acel";
	pXY.xlabel="Time (s)";
	pXY.ylabel="Acceleration (cm/s^2)";
	pXY.title=name+" Acel "+date;
	pXY.units="cm/s^2";
	pXY.multipleDeployments=true;
	double amax=std::max(std::max(ax.max,ay.max),az.max);
	double l=log10(amax);
	double m;
	if(l<0.0) m=pow(10.0,fabs(floor(l)));
	else m=pow(10.0,ceil(l));
	pXY.ymin=-ceil(amax*m)/m;
	pXY.ymax=ceil(amax*m)/m;
	//cout<<"Plot push"<<endl;
	pXY.push_dat(ay);
	pXY.push_dat(az);
	pXY.push_dat(ax);

	//cout<<"Plot draw1"<<endl;
	string nameFileSVG="./imagenes/"+pXY.name+".svg";
	pXY.draw(nameFileSVG);
	//cout<<"Plot draw2"<<endl;
	
	return "../imagenes/"+pXY.name+".svg";
}

Data2D ax,ay,az;
string fileName;
string date;

string openAccelerogram(const string str){
	fileData ff(str);
	//cout<<"Bandpass ax"<<endl;
	//Data2D ax=Bandpass(taper(ff.N90W()),0.05,15);
	//cout<<"Bandpass ay"<<endl;
	//Data2D ay=Bandpass(taper(ff.N00E()),0.05,15);
	//cout<<"Bandpass az"<<endl;
	//Data2D az=Bandpass(taper(ff.V()),0.05,15);
	ax=ff.N90W();
	ay=ff.N00E();
	az=ff.V();
	fileName=ff.fileName;
	date=ff.Date();
	string fileNameSVG=plot(fileName, date, ax, ay, az);
	return "{\"F\":\""+fileNameSVG+"\"}";
}


string operationsStack(const string str){
	if(fileName.length()>0){
		cout<<"fileName: "<<fileName<<endl;
		cout<<"date: "<<date<<endl;
		//cout<<"str: "<<str<<endl;
		std::stringstream data(str);
		std::string filtro,text,opciones;
		
		Data2D temp_ax=ax;
		Data2D temp_ay=ay;
		Data2D temp_az=az;
		string temp_fileName(fileName);
		
		while(data>>filtro>>text>>opciones){
		
			std::map<std::string,std::string> mapVar;
			std::cout<<"filtro:"<<filtro<<" opciones"<<opciones<<std::endl;
			std::string op=extract(opciones,'{','}');
			std::cout<<op<<std::endl;
			std::vector <std::string> fields;
			splitStrings( fields, op, ',' );
			for (auto field : fields) {
				std::vector <std::string> var;
				std::cout << "field:"<<field << std::endl;
				splitStrings( var, field, ':' );
				string nameVar=extract(var[0],'"','"');
				string valVar=var[1];
				mapVar[nameVar]=valVar;
				std::cout<<"mapVar:"<<nameVar<<" = "<<valVar<<std::endl;
				
				
			}
			
			std::cout<<"Busca filtro:"<<std::endl;
			
			if(filtro=="Taper"){
				temp_fileName=filtro+">"+temp_fileName;
				temp_ax=taper(temp_ax, std::stod(mapVar["R"]) );
				temp_ay=taper(temp_ay, std::stod(mapVar["R"]) );
				temp_az=taper(temp_az, std::stod(mapVar["R"]) );
			}
			
			if(filtro=="Bandpass"){
				temp_fileName=filtro+">"+temp_fileName;
				temp_ax=Bandpass(temp_ax, std::stod(mapVar["Fmin"]), std::stod(mapVar["Fmax"]), (int)std::stof(mapVar["NPolos"]) );
				temp_ay=Bandpass(temp_ay, std::stod(mapVar["Fmin"]), std::stod(mapVar["Fmax"]), (int)std::stof(mapVar["NPolos"]) );
				temp_az=Bandpass(temp_az, std::stod(mapVar["Fmin"]), std::stod(mapVar["Fmax"]), (int)std::stof(mapVar["NPolos"]) );
			}
			
			if(filtro=="Lowpass"){
				temp_fileName=filtro+">"+temp_fileName;
				temp_ax=Lowpass(temp_ax, std::stod(mapVar["Fmax"]), (int)std::stof(mapVar["NPolos"]) );
				temp_ay=Lowpass(temp_ay, std::stod(mapVar["Fmax"]), (int)std::stof(mapVar["NPolos"]) );
				temp_az=Lowpass(temp_az, std::stod(mapVar["Fmax"]), (int)std::stof(mapVar["NPolos"]) );
			}
			
			if(filtro=="Highpass"){
				temp_fileName=filtro+">"+temp_fileName;
				temp_ax=Highpass(temp_ax, std::stod(mapVar["Fmin"]), (int)std::stof(mapVar["NPolos"]) );
				temp_ay=Highpass(temp_ay, std::stod(mapVar["Fmin"]), (int)std::stof(mapVar["NPolos"]) );
				temp_az=Highpass(temp_az, std::stod(mapVar["Fmin"]), (int)std::stof(mapVar["NPolos"]) );
			}
			
			
			//std::cout<<"R="<<std::stod(mapVar["r"])<<std::endl;
			//std::cout<<"Fmin="<<std::stod(mapVar["Fmin"])<<std::endl;
			//double num_double = std::stod(str);
			
			std::cout<<"----------"<<std::endl;
		}
		string fileNameSVG=plot(temp_fileName,date, temp_ax, temp_ay, temp_az);
		return "{\"F\":\""+fileNameSVG+"\"}";
	}
	else{
		return "{\"F\":\"espera.gif\"}";
	}
}


string acelerograma(const char k, const string str){

	//std::cout<<"key2:"<<k<<" : "<<str<<std::endl;
	string answer; //respuesta
	switch(k){
			case 'F':
				answer=openAccelerogram(str);
			break;
			case 'O':
				answer=operationsStack(str);
			break;
	}
	return answer;
}
// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	//std::cout << "on_message called with hdl: " << hdl.lock().get()
	//		  << " and message: " << msg->get_payload()
	//		  << std::endl;

		std::string command=msg->get_payload();
		std::string res;
		char k1=command.c_str()[0];
		char k2=command.c_str()[1];
		std::string substr=command.substr (2); 
		
		//std::cout<<"key1:"<<k1<<" : "<<substr<<std::endl;
		
		switch(k1){
			case 'A':
				res=acelerograma(k2,substr);
			break;
			//default:
				
		};
		
		//cout<<"key2:"<<k2<<endl;
		
	// check for a special command to instruct the server to stop listening so
	// it can be cleanly exited.
		if (msg->get_payload() == "stop-listening") {
			s->stop_listening();
			return;
		}

		try {
			//s->send(hdl, msg->get_payload(), msg->get_opcode());
			s->send(hdl, res, msg->get_opcode());
		} catch (websocketpp::exception const & e) {
			std::cout << "Echo failed because: "
				  << "(" << e.what() << ")" << std::endl;
		}
}


int main() {
	// Create a server endpoint
	server echo_server;

	try {
		// Set logging settings
		echo_server.set_access_channels(websocketpp::log::alevel::all);
		echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
		echo_server.set_reuse_addr(true);

		// Initialize Asio
		echo_server.init_asio();

		// Register our message handler
		echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));

		// Listen on port 9002
		echo_server.listen(9002);

		// Start the server accept loop
		echo_server.start_accept();

		// Start the ASIO io_service run loop
		echo_server.run();
	} catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	} catch (...) {
		std::cout << "other exception" << std::endl;
	}
}
