#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

using namespace std;

// https://vjudge.net/problem/UVA-1235

vector<vector<vector<int>>> solicitarDatos(){
    //Vector de vectores para guardar las claves a las que tengo que llegar
    int casos;
    cin>> casos;
    vector<vector<vector<int>>> todasLasClaves;

    for(int j = 0;j<casos;j++){
        vector<vector<int>> clavesDelIesimoCaso;
        int claves;
        cin >> claves; // Cantidad de claves en el caso i
        for(int k = 0;k<claves;k++){
            vector<int> numeroAVector(4); //Siempre el vector va a ser de 4 cifras, pues las claves son de 4 cifras
            int numero;
            cin >> numero;
            for(int i = 3; i >= 0; i--){
                numeroAVector[i] = numero % 10;
                numero /= 10;
            }
            clavesDelIesimoCaso.push_back(numeroAVector);
        }
        todasLasClaves.push_back(clavesDelIesimoCaso);

    }

    return todasLasClaves;
}


int distancia(vector<int>& x,vector<int>& y){
    int res = 0;
    for(int i = 0;i < x.size();i++){ // Los vectores siempre van a ser de longitud 4, porque las claves son de 4 digitos
        if(abs(x[i]-y[i]) <= 5){ // Es el criterio para ir de un numero a otro
            res += abs(x[i] - y[i]);
        }else{
            res += 10 - max(x[i],y[i]) + min(x[i],y[i]);
        }
    }
    return res;
}

int buscarMinimo(vector<int>& costos){
    int inf = numeric_limits<int> :: max();
    int indiceMinimo = -1; //Lo inicio indefinido, -1 nunca puede ser un indice
    int minimo = inf; //El minimo lo inicializo en el mas infinito
    for(int i = 0; i<costos.size();i++){
        if(costos[i]< minimo){
            minimo = costos[i];
            indiceMinimo = i;
        }
    }
    return indiceMinimo;
} //Nota esta funcion me devuelve -1 si el vector es tiene al inf en todos sus valores, esto me generaba problemas en prim




//Funcion que me dice por que numero de nodo empezar, me guardo el nro de nodo y lo que me cuesta pasar del 0000 a la clave del nodo
pair<int,int> indiceNodoCero(vector<vector<int>>& datos){
    vector<int> nodoCero = {0,0,0,0};
    int menorDistAlCero;
    int v ; //Lo inicializo indefinido
    for(int i = 0;i<datos.size();i++){ //O(m)
        int distAlCero = distancia(nodoCero,datos[i]);
        if(menorDistAlCero>distAlCero){
            v = i;
            menorDistAlCero = distAlCero;
        }
    }
    return make_pair(v,menorDistAlCero);
}


//Hacer prim. Como es un grafo denso lo puedo hacer con la implementacion O(n^2)
//Algoritmo visto en la teorica
vector<int> prim(vector<vector<int>>& datos,int r){ // v es el nodo donde se inicia el recorrido
    int inf = numeric_limits<int>::max();
    vector<int> tree(datos.size(),inf);
    vector<int> padres(datos.size(),inf);
    vector<int> costos(datos.size(),inf);
    padres[r] = r;
    tree[r] = r;

    for(int i = 0;i<datos.size();i++){
        if(i != r){ //Como es un grafo completo, todos los demas son vecinos excepto si mismo
            costos[i] = distancia(datos[r],datos[i]);
            padres[i] = r;
        }
    }

    for(int i = 0;i<datos.size();i++){ //Tengo que agregar n-1 aristastas para que sea un arbol
        int w = buscarMinimo(costos);
        if(w == -1) break; // Si el vector costos tiene todos los valores infinitos, es porque el arbol ya esta, entonces sale del for
        int v = padres[w];
        tree[w] = v;
        costos[w] = inf;
        for(int j = 0; j<datos.size();j++){
            if((j != w) && (tree[j] == inf)){
                if(distancia(datos[j],datos[w]) < costos[j]){
                    costos[j] = distancia(datos[j],datos[w]);
                    padres[j] = w;
                }
            }
        }
    }
    return tree;

}


int main() {

    vector<vector<vector<int>>> datos = solicitarDatos();

    for(int i = 0;i<datos.size();i++){
        // Para cada matriz, osea para cada caso, busco el primer nodo y aplico prim
        vector<vector<int>> casoIesimo = datos[i];
        pair<int,int> primerNodo = indiceNodoCero(casoIesimo);


        vector<int> primCasoUno = prim(casoIesimo,primerNodo.first);

        //Calculo las distancias de la posicion i con su padre en el arbol
        //La suma de todos estos me va a dar el minimo ya que es un arbol generador minimo
        int res = primerNodo.second;
        for(int j = 0;j<casoIesimo.size();j++){
            res += distancia(casoIesimo[primCasoUno[j]],casoIesimo[j]);
        }
        cout <<res<<endl;
    }

    return 0;
}
