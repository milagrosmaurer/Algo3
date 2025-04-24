#include <iostream>
#include <vector>
#include <string>

using namespace std;

// https://vjudge.net/problem/CodeForces-559B

//entregable 4 divide and conquer
//importante: los dos strings tienen la misma longitud

// EJEMPLO: aaba
//abaa
// ab aa
// aa ba

//a la funcion tendria que pasarle 4 parametros

bool equivalenciaAux (string a, string b , int lowA, int highA , int lowB, int highB){

    bool respuesta = false;

    if((lowA + 1 == highA) && (lowB + 1 == highB)){
        if(((a[lowA] == b[lowB]) && (a[highA] == a[highB])) || ((a[lowA] == b[highB]) && (a[highA] == b[lowB]))){
            respuesta = true;
        }
    }else{
        int midA = (lowA + highA + 1)/2;
        int midB = (lowB + highB + 1)/2;
        if((equivalenciaAux(a, b, lowA, midA - 1, lowB, midB - 1) && equivalenciaAux(a, b, midA, highA, midB, highB)) ||
                (equivalenciaAux(a, b, lowA , midA - 1, midB, highB) && equivalenciaAux(a, b, midA, highA, lowB, midB - 1))){
            respuesta = true;
        }
    }


    return respuesta;

}

bool equivalencia(string a, string b){
    if(a.size() % 2 != 0){
        if( a == b) return true;
        if( a != b) return false;
    }else{
        return equivalenciaAux(a,b,0,a.size() - 1,0,b.size() - 1);
    }

}



int main() {
    string a;
    string b;
    cin >> a >> b;
    bool resultado = equivalencia(a,b);

    if(resultado== true){
        cout << "YES" << endl;
    }else{
        cout<< "NO" << endl;
    }

    return 0;

}

