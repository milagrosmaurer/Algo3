#include <iostream>
#include <string>
#include <vector>
#include <tuple>

// https://vjudge.net/problem/CodeForces-1215C

using namespace std;

pair<int,int> cantidadErrores(string uno, string dos){ //cuenta la cantidad de "bloques" que hay que cambiar
    int cantErroresA = 0;
    int cantErroresB = 0;

    for(int i = 0;i<uno.size();i++){
        if(uno[i] == 'a' && dos[i] == 'b') cantErroresA ++;
        if(uno[i] == 'b' && dos[i] == 'a') cantErroresB ++;
    }
    return make_pair(cantErroresA,cantErroresB);
}

void cambiar(string& uno,string& dos,int i,int j){ //funcion que hace el swap
    char cambioUno = uno[i];
    uno[i] = dos[j];
    dos[j] = cambioUno;
}

// Tengo cuatro tipos de cambios para hacer
// Cuando es error del tipo a y erroresA>1 entonces busco otro error del tipo a y hago el cambio
// Cuando es error del tipo a y erroresA = 1 entonces cambio consigo mismo y busco error del tipo b
// Cuando es error del tipo b y erroresB>1 entonces busco otro error del tipo b y hago el cambio
// Cuando es error del tipo b y erroresB = 1 entonces cambio consigo mismo y busco error del tipo a

// Error del tipo a: uno[i] == 'a' && dos[i] == 'b'
// Error del tipo b: uno[i] == 'b' && dos[i] == 'a'

pair<int,vector<pair<int,int>>> cambiarLetrasAux(string& uno, string& dos, int cantLetras){
    pair<int,int> errores = cantidadErrores(uno,dos);
    int erroresA = errores.first;
    int erroresB = errores.second;
    vector<pair<int,int>> movimientos;

    if((erroresA + erroresB) % 2 != 0) return make_pair(-1,movimientos); // Si la suma de la cantidad de errores es impar, no se pueden igualar los dos strings

    for(int i = 0; i<cantLetras ; i++){
        if(uno[i] == 'a' && dos[i] == 'b' && erroresA>1){ //Primer caso
            bool encontro = false;
            int j = i + 1;
            while(j<cantLetras && !encontro){
                if(uno[j] == 'a' && dos[j] == 'b'){
                    encontro = true;
                    cambiar(uno,dos,i,j);
                    movimientos.push_back(make_pair(i+1,j+1));
                    erroresA -= 2;
                }
                j++;
            }
        }
        if(uno[i] == 'a' && dos[i] == 'b' && erroresA == 1){ //Segundo caso
            bool encontro = false;
            int j = i + 1;
            while(j<cantLetras && !encontro){
                if(uno[j] == 'b' && dos[j] == 'a'){
                    encontro = true;
                    cambiar(uno,dos,i,i); // Intercambio consigo mismo
                    movimientos.push_back(make_pair(i+1,i+1));
                    cambiar(uno,dos,i,j);
                    movimientos.push_back(make_pair(i+1,j+1));
                    erroresA --;
                    erroresB --;
                }
                j++;
            }
        }
        if(uno[i] == 'b' && dos[i] == 'a' && erroresB > 1){ // Tercer caso
            bool encontro = false;
            int j = i + 1;
            while(j < cantLetras && !encontro){
                if(uno[j] == 'b' && dos[j] == 'a'){
                    encontro = true;
                    cambiar(uno,dos,i,j);
                    movimientos.push_back(make_pair(i+1,j+1));
                    erroresB -= 2;
                }
                j++;
            }
        }
        if(uno[i] == 'b' && dos[i] == 'a' && erroresB == 1){ //Cuarto caso
            bool encontro = false;
            int j = i+1;
            while(j<cantLetras && !encontro){
                if(uno[j] == 'a' && dos[j] == 'b'){
                    encontro = true;
                    cambiar(uno,dos,i,i);
                    movimientos.push_back(make_pair(i+1,i+1));
                    cambiar(uno,dos,i,j);
                    movimientos.push_back(make_pair(i+1,j+1));
                    erroresA--;
                    erroresB --;
                }
                j++;
            }
        }
    }
    return make_pair(movimientos.size(),movimientos);
}




int main() {
        int cantLetras;
        string uno, dos;

        cin >> cantLetras >> uno >> dos;

        pair<int,vector<pair<int,int>>> resultado = cambiarLetrasAux(uno, dos, cantLetras);
        if(resultado.first == -1){
            cout << -1 <<endl;
        }else{
            cout << resultado.first << endl;
            for (const auto& par : resultado.second) {
                cout << par.first << " " << par.second << endl;
            }
        }

        return 0;
}

