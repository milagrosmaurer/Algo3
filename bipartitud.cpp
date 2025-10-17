#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Idea: cuando llemos el grafo hacemos una lista de adyacencias para BFS y una matriz de adyacencias para el grafo complemento
// Recordemos que (u,v) pertenece al grafo complemento si (u,v) no pertenece al grafo original
// Correr BFS en el grafo original para tener el vector de distancias
// O puedo hacer una lista de aristas no??

// Cuando recorra la matriz de adyacencia no la quiero recorrer entera, al ser un grafo no dirigido es simetrica, solo quiero ver
// la diagonal de arriba

// To do esto me da TLE
// Idea 2: correr BFS, hacer la biparticion de nodos, los que estan a distancia par del vertice y los que estan a distancia impar
// iterar sobre alguno de esos dos conjuntos y contar para cada elemento de ese conjunto la diferencia que hay entre el conjunto de distinta paridad
// y la cant de vecinos que tiene en el grafo

// Idea 3 (y la que quedo)
// En el mismo BFS cuento cuantas aristas en nivel par hay, y cuantas aristas en nivel impar hay.
// Me fijo la cantidad de aristas totales entre esos dos conjuntos, osea #par * #impar y le resto la cantidad de aristas actuales que tiene, osea n-1 porque el grafo dado es arbol.

vector<vector<int>> solicitarDatos(){

    int n; // Numero de nodos
    cin >> n;

    vector<vector<int>> grafo(n);



    // Me sirve hacer un renombramiento de nodos, asi cuando indexo no me hago lio!!
    // El nodo que se llamaba u ahora pasa a llamarse u - 1

    for(int i = 0; i < n - 1; i++){ // n - 1 aristas, porque es un arbol!!

        int u,v;
        cin >> u >> v;

        grafo[u - 1].push_back(v - 1);
        grafo[v - 1].push_back(u - 1);

    }



    return grafo;
} // Esto esta ok

int inf = 1e9;

// Algoritmo del cormen

vector<int> BFS (vector<vector<int>>& grafo, int s){

    int n = grafo.size();
    vector<int> colores(n, -1); // -1 es color blanco
    vector<int> distancias(n, inf);
    //vector<int> padres(n,0);
    // El vector padres me da el arbol

    colores[s] = 0; // 0 es color gris
    distancias[s] = 0; // La distancia a si mismo es s
    //padres[s] = s;

    queue<int> cola({s});

    while( not cola.empty()){
        int u = cola.front();
        cola.pop();
        for(int i = 0; i < grafo[u].size(); i++){ // vecinos de u

            int v = grafo[u][i];

            if(colores[v] == -1){
                cola.push(v);
                colores[v] = 0;
                //padres[v] = u;
                distancias[v] = distancias[u] + 1;
            }

        }
        colores[u] = 1; // 1 representa el negro en colores
    }

    return distancias;

} // IMPORTANTE DE BFS: esta hecho para listas de adyacencias!!!
// Complejidad: O(m+n)

// Puedo hacer un BFS que solo cuente la cantidad de nodos de la biparticion

pair<int,int> BFScantidadDeNodosBiparticion (vector<vector<int>>& grafo, int s){

    int n = grafo.size();
    vector<int> colores(n, -1); // -1 es color blanco
    vector<int> distancias(n, inf);
    //vector<int> padres(n,0);
    // El vector padres me da el arbol

    int par = 1; // Nodo s;
    int impar = 0;

    colores[s] = 0; // 0 es color gris
    distancias[s] = 0; // La distancia a si mismo es s
    //padres[s] = s;

    queue<int> cola({s});

    while( not cola.empty()){
        int u = cola.front();
        cola.pop();
        for(int i = 0; i < grafo[u].size(); i++){ // vecinos de u

            int v = grafo[u][i];

            if(colores[v] == -1){
                cola.push(v);
                colores[v] = 0;
                //padres[v] = u;
                distancias[v] = distancias[u] + 1;
                if(distancias[v] % 2 == 0) par++;
                if(distancias[v] % 2 != 0) impar++;
            }

        }
        colores[u] = 1; // 1 representa el negro en colores
    }

    return make_pair(par,impar);

}




// armo la bipartitud, creo los dos conjuntos disjuntos de vertices, osea interseccion(V1,V2) = vacio
pair<vector<int>,vector<int>> conjuntosIgualParidad(vector<int>& distancias){

    vector<int> par;
    vector<int> impar;

    for(int i = 0; i < distancias.size(); i++){

        if(distancias[i] % 2 == 0) par.push_back(i);
        else impar.push_back(i);

    }

    return make_pair(par,impar);
}// Complejidad: O(n)



/*long long int cantAristasPuedoAgregar(vector<vector<int>>& grafo){

    vector<int> distancias = BFS(grafo,0); //O(n+m)

    pair<vector<int>,vector<int>> conjuntos = conjuntosIgualParidad(distancias);//O(n)

    vector<int> conjuntoPar = conjuntos.first;
    vector<int> conjuntoImpar = conjuntos.second;

    // esto me sirve porque solo necesito las aristas que 'cruzan' los conjuntos
    // pq si chequeo las que unen elementos del mismo conjunto, estoy haciendo chequeos innecesarios, porque no se va a poder

    long long int contador = 0;

    // Me daba TLE porque yo estaba viendo QUE aristas podia agregar, pero no hace falta eso!!
    // Necesitas la diferencia para todo u perteneciente a conjuntosPar, la dif entre conjuntosImpar.size() - grafo[u].size()

    for( int u : conjuntoPar){

        contador += conjuntoImpar.size() - grafo[u].size();
        // Como grafo es bipartito -> todos los elementos de grafo[u] tienen que estar contenidos en el conjunto de aristas a distancia impar
        // los que no estan en ese conjunto, es porque se pueden agregar, y no rompe con la biparticion, pues es una arista que une dos nodos con distinta paridad

    }

    return contador;
}*/


int main(){

    vector<vector<int>> grafo = solicitarDatos();

    //vector<int> distancias = BFS(grafo, 0);

    //pair<vector<int>,vector<int>> biparticion = conjuntosIgualParidad(distancias);

    //vector<int> par = biparticion.first;

    //vector<int> impar = biparticion.second;

    // Una vez que tengo la biparticion, la cantidad de aristas posibles entre esos dos conjuntos es multiplicar el tamano del conjunto,
    // por el tamano del otro conjunto, pues un nodo de un conjunto puede estar conectado a todos los otros nodos del conjunto
    // como el grafo que me dan es arbol y bipartito, tiene exactamente n - 1 aristas, con lo cual a todas esas posibles les resto las que ya tengo

    pair<int,int> bipartitud = BFScantidadDeNodosBiparticion(grafo,0);

    int par = bipartitud.first;
    int impar = bipartitud.second;

    long long res = par * impar - grafo.size() + 1;

    //long long res = (par.size() * impar.size()) - grafo.size() + 1;



    //long long int res = cantAristasPuedoAgregar(grafo);


    cout << res << endl;

    return 0;
}
