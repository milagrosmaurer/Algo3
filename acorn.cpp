//me funciona, pero me da time limit exceed
#include <iostream>
#include <vector>
#include <sstream>
#include <map>

// https://vjudge.net/problem/UVA-1231

using namespace std;

//entregable nro 3, le puse 4.1 porque me equivoque: PROGRAMACION DINAMICA BOTTOM UP
//mi input va a ser una matriz de tamaño arboles y niveles en el arbol, con cada bellota en arbol y nivel correspondiente

//veamos que onda el input y que nos dice
//1 c=numbers of datasets
//3 10 2 t=nros de arboles,h=altura,f=tiempo que tarda en viajar de un arbol a otro
//3 1 4 10 1er nro cuantas bellotas hay y los que le siguen en que altura estan
//6 3 5 7 8 9 9
//5 3 4 5 6 9
//0 no se que es este 0
//creo que no se puede pasar del arbol 1 al 3
//matriz del ejemplo
// 1 0 0    7 8 6
// 0 2 1    5 8 6
// 0 1 0    5 6 5
// 0 1 0    4 5 5  posicion (3,1) miro (4,1) (5,2) (5,0)
// 0 0 1    3 3 5
// 0 1 1    2 3 4
// 1 0 1    2 2 3
// 0 1 1    1 2 2
// 0 0 0    1 0 0
// 1 0 0    1 0 0



//la ultima fila de mi matriz recoleccion es igual a la ultima fila de mi matriz arbol
//puedo inicializar la matriz con arboles + 2 (en el ejemplo 5 columnas) para que no se me vaya de rango

pair<int,vector<vector<int>>> solicitarDatos(){
    //voy a hacer mi matriz arbol
    int arboles;
    int niveles;
    int tiempoEnAvanzar;

    string linea;
    getline(cin,linea);
    stringstream ss1(linea);

    ss1 >> arboles >> niveles >> tiempoEnAvanzar;

    vector<vector<int>> datos(niveles,vector<int>(arboles,0));

    /*getline(cin,linea);
    stringstream ss2(linea);*/

    for(int j = 0; j < arboles ; j++){
        getline(cin,linea); //lee la linea que le sigue a los datos anteriores
        stringstream ss(linea);

        int cantidadBellotas;
        ss >> cantidadBellotas; //es el primer elemento de la linea

        int elemento; //lee los siguientes elementos despues de cant de bellotas
        while(ss >> elemento){
            datos[elemento-1][j] = datos[elemento-1][j] + 1;
            //elemento + 1 porque en el ejemplo me dice en fila 1 y en realidad en mi matriz es la 0
        }
    }


    /*cout << "Matriz de datos:" << endl;
    for (int i = 0; i < niveles; ++i) {
        for (int j = 0; j < arboles; ++j) {
            cout << datos[i][j] << " ";
        }
        cout << endl;
    }*/


    return make_pair(tiempoEnAvanzar,datos);

}
vector<pair<int,vector<vector<int>>>> vectorMatrices(int casos){
    vector<pair<int,vector<vector<int>>>> vectorDatos;
    for(int i = 0; i<casos;i++){
        vectorDatos.push_back(solicitarDatos());
    }
    return vectorDatos;
}

int maximasBellotas(vector<vector<int>> arboles, int tiempoEnAvanzar) {
    int fila = arboles.size();
    int columna = arboles[0].size();
    int mejorCaso = 0;



    for(int i = fila - 1; i >= 0 ; i--){ //empiezo desde la anteultima fila asi la ultima fila me la deja tal cual esta
        //recorre filas -> empieza desde abajo. Algoritmo Bottom Up.
        for(int j = columna - 1; j >= 0; j--){ // recorre columnas y empieza desde la ultima   //columna = 2

           int cantBellotaActual = arboles[i][j];
           int mejorOpcion = cantBellotaActual; //cuando arranca la mejor opcion es donde estoy posicionado

           for(int k = 0; k < columna; k++){
               int opcion = 0;
               if(k == j){//solo me fijo el que esta abajo mio
                   if(i+1 <= fila -1){
                       opcion = cantBellotaActual + arboles[i+1][k];
                       if(opcion > mejorOpcion) mejorOpcion = opcion;
                   }else{
                       opcion = cantBellotaActual;
                       if(opcion > mejorOpcion) mejorOpcion = opcion;
                   }

               }else{ //me fijo la mejor opcion de los otros arboles

                   if(i + tiempoEnAvanzar <= fila - 1){
                       opcion = cantBellotaActual + arboles[i+tiempoEnAvanzar][k];
                       if(opcion > mejorOpcion) mejorOpcion = opcion;
                   }else{
                       opcion = cantBellotaActual;
                       if(opcion > mejorOpcion) mejorOpcion = opcion;
                   }

               }
           }

           arboles[i][j] = mejorOpcion;
           if(arboles[i][j] > mejorCaso) mejorCaso = arboles[i][j];
        }



    }

    return mejorCaso;

}
 // version sin un for, osea optimizada (esperemos)
int maxCantidadDeBellotas(vector<vector<int>> arboles, int tiempoEnAvanzar){
    int fila = arboles.size();
    int columna = arboles[0].size();
    int mejorCaso = 0;
    vector<int> mejorCasoDeCadaColumna(fila,0);

    for(int i = fila - 1; i >= 0 ; i--){

        for(int j = columna - 1; j>=0 ; j--){
            int cantBellotaActual = arboles[i][j];

            if( i + 1 <= fila - 1 ){
                if(i + tiempoEnAvanzar <=  fila - 1){
                    arboles[i][j] = max(cantBellotaActual + arboles[i+1][j],cantBellotaActual + mejorCasoDeCadaColumna[i + tiempoEnAvanzar] );
                    // puede ir para abajo en el mismo arbol o bajar tiempoEnAvanzar, pero me interesa solo la mejor opcion
                }else{
                    arboles[i][j] = cantBellotaActual + arboles[i+1][j];
                    // solo puede ir para abajo en el mismo arbol
                }
            }else{
                arboles[i][j] = cantBellotaActual;
                //no puede ir para abajo
            }
            if(arboles[i][j] > mejorCasoDeCadaColumna[i]) mejorCasoDeCadaColumna[i] = arboles[i][j];
            if(arboles[i][j] > mejorCaso) mejorCaso = arboles[i][j];
        }

    }
    return mejorCaso;
}

int main() {
    //vector<vector<int>> arbol = {{1,0,0},{0,2,1},{0,1,0},{0,1,0},{0,0,1},{0,1,1},{1,0,1},{0,1,1},{0,0,0},{1,0,0}};
    int casos;
    cin >> casos;
    cin.ignore();

    auto vectorDatos = vectorMatrices(casos);
    string linea;
    int cero;
    getline(cin,linea);
    stringstream ss3(linea);
    ss3 >> cero;

    for(int i = 0; i < casos;i++){
        int resultado = maximasBellotas(vectorDatos[i].second,vectorDatos[i].first);
        cout << resultado << endl;
    }

    return 0;
}




