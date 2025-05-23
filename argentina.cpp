#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <algorithm>

// https://vjudge.net/problem/UVA-11804

using namespace std;

int sumaAtacantes(const vector<tuple<string, int, int>>& atacantes) {
    int suma = 0;
    for (const auto& jugador : atacantes) {
        suma += get<1>(jugador);
    }
    return suma;
}

int sumaDefensores(const vector<tuple<string, int, int>>& defensores) {
    int suma = 0;
    for (const auto& jugador : defensores) {
        suma += get<2>(jugador);
    }
    return suma;
}

bool estaEnAtaque(tuple<string,int,int> jugador,vector<tuple<string,int,int>>& ataque){
    for(int i = 0; i<ataque.size();i++){
        if(get<0>(ataque[i]) == get<0>(jugador)){
            return true;
        }
    }
    return false;
}

vector<tuple<string,int,int>> atacantes(const vector<tuple<string,int,int>>& jugadores, vector<tuple<string,int,int>>& ataque,vector<tuple<string,int,int>>& mejorAtaque,int i){
    if (i == jugadores.size()) {
        return mejorAtaque;
    }

    if (ataque.size() < 5) {
        ataque.push_back(jugadores[i]); // agrego el jugador al ataque
        if (sumaAtacantes(mejorAtaque) < sumaAtacantes(ataque)) { //agregando a este atacante, el puntaje de atacantes es mejor
            mejorAtaque = ataque;
        }
        else if (sumaAtacantes(mejorAtaque) == sumaAtacantes(ataque) && sumaDefensores(mejorAtaque) > sumaDefensores(ataque)) { //en caso de empate de habilidad atacante me quedo con el que le da menos habilidad defensiva a los atacantes
            mejorAtaque = ataque;
        }
        auto resultadoAtaque = atacantes(jugadores, ataque, mejorAtaque,i + 1);
        ataque.pop_back();
        mejorAtaque = resultadoAtaque;
    }
    return atacantes(jugadores,ataque,mejorAtaque,i+1);
}

pair<vector<tuple<string,int,int>>,vector<tuple<string,int,int>>> formacion(vector<tuple<string,int,int>> const& jugadores){ // a los jugadores que no estan como ataque los pongo en defensa
    vector<tuple<string,int,int>> ataque,mejorAtaque,defensa;
    vector<tuple<string,int,int>> mejoresAtacantes = atacantes(jugadores,ataque,mejorAtaque,0);
    for(int i = 0; i<jugadores.size();i++){
        if(!estaEnAtaque(jugadores[i],mejoresAtacantes)){
            defensa.push_back(jugadores[i]);
        }
    }
    return make_pair(mejoresAtacantes,defensa);
}


int main(){
    int casos;
    cin>>casos;
    cin.ignore();

    for(int i =0;i<casos;i++){
        vector<tuple<string,int,int>> jugadores;
        for(int j=0;j<10;j++){
            string nombre;
            int ataque,defensa;
            cin >> nombre >> ataque >> defensa;
            jugadores.push_back(make_tuple(nombre,ataque,defensa));
        }
        std::sort(jugadores.begin(),jugadores.end()); //ordeno por orden lexicografico
        auto resultado = formacion(jugadores);
        auto ataqueFinal = get<0>(resultado);
        auto defensaFinal = get<1>(resultado);

        cout << "Case " << i+1<< ":"<<endl ;
        cout << "(";
        for (size_t i = 0; i < ataqueFinal.size(); ++i) {
            cout <<  get<0>(ataqueFinal[i]) ;
            if (i != ataqueFinal.size() - 1)
                cout << ", ";
        }
        cout << ")" <<endl;


        cout << "(";
        for (size_t i = 0; i < defensaFinal.size(); ++i) {
            cout << get<0>(defensaFinal[i]);
            if (i != defensaFinal.size() - 1)
                cout << ", ";
        }
        cout << ")" << endl;
    }
    return 0;
}












