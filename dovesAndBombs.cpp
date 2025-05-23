#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

// https://vjudge.net/problem/UVA-10765

// La idea de este ejercicio es buscar puntos de articulaciones
// Un vertice v de un grafo G es un punto de articulacion si G-v tiene mas componentes conexas que G

// Nota: muchos algoritmos los saque de practicas, teoricas, etc. Especialmente use la clase practica de recorridos, donde buscamos
// la cantidad de aristas puentes que hay. Este es un problema mas o menos analogo pero con puntos de articulacion.

// Solicito los datos de mi input, armo la lista de adyacencias para cada caso.

vector<pair<int, vector<vector<int>>>> solicitarDatos() { // Armo la lista de adyacencias, esto me hace el grafo
    int vertices, estaciones;
    vector<pair<int, vector<vector<int>>>> datos;
    cin >> vertices >> estaciones;
    while (vertices != 0 && estaciones != 0) {

        vector<vector<int>> aristas(vertices);

        int origen, destino;
        while (cin >> origen >> destino && (origen != -1 && destino != -1)) {
            aristas[origen].push_back(destino);
            aristas[destino].push_back(origen);
        }
        datos.push_back(make_pair(estaciones, aristas));
        cin >> vertices >> estaciones;
    }

    return datos;
}

//Creo que puedo no usar el vector de padres
//Algoritmo sacado de la practica
void dfsBackedges(vector<vector<int>> &adyacencias, int v,int p, vector<int>& padres, vector<int>& estado,
                  vector<vector<int>>& treeEdges,vector<int>& backsInf, vector<int>& backsSup){
    // Estado[v] = -1 es no lo vi, = 0 lo estoy viendo, = 1 ya lo vi
    estado[v] = 0;
    for(int u:adyacencias[v]){
        if(estado[u] == -1){
            padres[u] = v;
            treeEdges[v].push_back(u);
            dfsBackedges(adyacencias,u,v,padres,estado,treeEdges,backsInf,backsSup);
        }else if(u!=p && estado[u] == 1){
            backsInf[u] ++;
            backsSup[v] ++;
        }
    }
    estado[v] = 1;
}
//Algortimo sacado de la practica
int cubren(int v,int p,vector<int>& memo,vector<int>& backsInf, vector<int>& backsSup,vector<vector<int>>& treeEdges){
    if(memo[v] != -1) return memo[v];
    int res = 0;
    res += backsInf[v];
    res -= backsSup[v];
    for(int hijo:treeEdges[v]){ //Calculo cubren de los hijos
        if(p!=hijo){
            res += cubren(hijo,v,memo,backsInf,backsSup,treeEdges);
        }


    }
    memo[v] = res;
    return res;
}

vector<pair<int,int>> doveAndBombs(vector<vector<int>>& grafo){
    int n = grafo.size();
    vector<int> padres(n,-1);
    padres[0] = 0;
    vector<int> estado(n,-1);
    vector<vector<int>> treeEdges(n);
    vector<int> backsInf(n,0);
    vector<int> backsSup(n,0);
    vector<int> memo(n,-1);
    dfsBackedges(grafo,0,-1,padres,estado,treeEdges,backsInf,backsSup);
    vector<int> vectorCubren(n,0);

    for(int i = 0;i<n;i++){
        vectorCubren[i] = cubren(i,-1,memo,backsInf,backsSup,treeEdges);
    }
    //Aca ya tengo to do para arrancar, me faltaria contar las componentes conexas
    // NOTA: algo que me costo darme cuenta es que los nodos con cubren != de 0 pueden ser puntos de articulacion

    vector<int> compConexas(n,0);
    compConexas[0] = treeEdges[0].size();
    for(int i = 1; i<n ; i++){
        if(vectorCubren[i] == 0 && treeEdges[i].size()>0){
            compConexas[i] = treeEdges[i].size() + 1;
        }
        if(treeEdges[i].size() == 0){ //Es el caso de que i sea una hoja
            compConexas[i] = 1;
        }
        if(vectorCubren[i] > 0 && treeEdges[i].size()>0){
            //compConexas[i] = treeEdges[i].size() + 1 - vectorCubren[i];
            for(int j = 0;j<treeEdges[i].size();j++){
                if(vectorCubren[treeEdges[i][j]] == 0) compConexas[i]++;
            }
            compConexas[i]++;

        }
    }

    vector<pair<int,int>> definitivo(n);

    for(int i = 0 ; i<n ; i++){
        definitivo[i] = make_pair(i,compConexas[i]);
    }
    return definitivo;
}




int main() {
    vector<pair<int, vector<vector<int>>>> datos = solicitarDatos();

    for(int i = 0;i<datos.size();i++){
        vector<vector<int>> grafo = datos[i].second;
        int casoOutput = datos[i].first;
        int k = 0;
        vector<pair<int,int>> resultado = doveAndBombs(grafo);
        auto comp = [] (const pair<int,int>& a, const pair<int,int>& b){
            return a.second > b.second;
        };
        // Necesito que sea un algoritmo estable!!!
        stable_sort(resultado.begin(),resultado.end(),comp);
        while(k<casoOutput){
            cout << resultado[k].first << " " << resultado[k].second<< endl;
            k++;
        }
        cout<<"\n";
    }

    return 0;
}
