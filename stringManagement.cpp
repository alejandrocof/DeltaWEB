#include "stringManagement.hpp"

void splitStrings(std::vector <std::string> &fields, std::string line, char c ){

	size_t found=line.find(c);
	size_t pos=0;
	fields.clear();
	
	while(found<line.length()){
		fields.push_back( line.substr (pos, found-pos));
		pos=found+1;
		found=line.find(c,pos);
	}
	
	fields.push_back( line.substr (pos));
	//size_t find(const std::string& my_string, size_t pos = 0);
}

std::string extract(const std::string& input, char p1, char p2) {
    std::string resultado;
    bool dentroDeParentesis = false;

    for (char caracter : input) {
        if (caracter == p1) {
            dentroDeParentesis = true;
        } else if (caracter == p2 && dentroDeParentesis) {
            dentroDeParentesis = false;
        } else if (dentroDeParentesis) {
            resultado += caracter;
        }
    }
    return resultado;
}
