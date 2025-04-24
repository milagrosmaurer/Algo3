#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

//Entregable 5: Algortimos Greedy

// https://vjudge.net/problem/SPOJ-GERGOVIA

vector<pair<long,vector<long>>> solicitarDatos (){ //funcion que solicita mis datos y guarda cada vecindario en un vector
    vector<pair<long,vector<long>>> datos;
    long casas;
    do{

    string linea;
    getline(cin,linea);
    stringstream ss(linea);

    ss >> casas; //cantidad de casas de mi vecindario
    if(casas != 0){

        vector<long> vecindario;

        getline(cin,linea);
        stringstream ss1(linea);
        long elemento;

        while(ss1>>elemento){
            vecindario.push_back(elemento);
        }
        datos.push_back(make_pair(casas,vecindario));
    }

    }while(casas != 0);
    return datos;
}

//Idea: comprarle/vender a mi vecino

long comercioDeVino(long casas, vector<long> vecindario){
    long trabajoTotal = 0;
    for(long i = 1; i < casas; i++){
        vecindario[i] += vecindario[i-1];
        trabajoTotal += abs(vecindario[i-1]);
    }

    return trabajoTotal;

}

int main() {

    vector<pair<long,vector<long>>> datos = solicitarDatos();
    long casos = datos.size();

    for(int i = 0; i<casos;i++){
        long resultado = comercioDeVino(datos[i].first,datos[i].second);
        cout << resultado <<endl;
    }

    return 0;
}
