#include <iostream>
#include <vector>
#include <tuple>
#include <queue>

using namespace std;

int INF = 102; // la maxima capacidad de una arista puede ser 100

//Problema de Flujo


// todo CADA AULA TIENE QUE TERMINAR CON B_i ESTUDIANTES!!!
// quiero ver si es posible reorganizar a los alumnos en las aulas, para que cumplan
// con la restricciones, es posible cuando se saturan todas las aristas de i -> t, esto significa que
// se pudo reubicar a todos los alumnos cumpliendo con la capacidad de las aulas, osea que el Fmax = cant alumnos a reubicar (todos)

// mi red va a ser una red de 2 capas, osea voy a duplicar la cantidad de aulas

// En mi grafo voy a poner las siguientes aristas con las siguientes capacidades
// La capa de aulas la duplico, entonces para cada nodo aula tengo i e i'

// s -> i fuente a aula numero i, c(s -> i) = ai, cantidad de alumnos que tiene cada aula
// i -> t aula numero i a sumidero, c(i -> t) = bi, cantidade de alumnos que puede tener cada aula
// i -> j' si se puede ir de aula i a j con c(i -> j') = ai, pues a lo sumo puedo mover la cantidad de alumnos que se encuentran en ese aula
// j -> i' si se puede ir de aula j a i con c(j -> i') = aj
// i -> i' el alumno se puede quedar en el aula en el que se encuentra con c(i -> i') = ai

// nodo 0 = s
// nodo 1...n = 1er capa
// nodo n+1 .. 2n = 2da capa
// nodo 2n + 1 = t

// i -> i' entonces i -> n + i

// cant nodos de mi red = 2n + 2


// Como lleno mi matriz capacidades???


tuple<int,int,vector<vector<int>>,vector<vector<int>>> solicitarDatos(){

    int n,m; // n cant de aulas, m pares de movimientos que se pueden hacer entre aulas

    cin >> n >> m;

    int cantAlumnos = 0; // aca voy a sumar todos los a_i

    int cantLugares = 0; // aca sumo la capacidad de cada aula

    // puedo empezar a buscar la solucion del problema <=> cantAlumnos == cantLugares
    // creo matriz de capacidades:

    vector<vector<int>> capacidades(2 * n + 2,vector<int>(2 * n + 2, 0));

    int s = 0; // fuente
    int t = capacidades.size() - 1; // sumidero

    // creo lista de adyacencias para BFS

    vector<vector<int>> ady(2 * n + 2);

    // leo la cantidad de alumnos que hay en cada aula y "creo" arista s -> i para todo i de 1 a n

    // notar que si ai o bi es igual a 0 , esa arista no la tengo que poner en mi lista de adyacencias, porque por ahi no puede pasar flujo

    for (int i = 0; i < n; ++i) {

        int a_i;
        cin >> a_i; // ai PUEDE SER 0, EN ESE CASO NO PONGO LA ARISTA EN EL GRAFO ADY

        cantAlumnos += a_i;
        capacidades[s][i + 1] = a_i;

        // ademas agrego la arista i -> i' con capacidad a_i que indica que puedo quedarme en el aula en el cual  estoy

        capacidades[i + 1][i + 1 + n] = a_i;

        if(a_i != 0){
            ady[i + 1].push_back(s);
            ady[s].push_back(i + 1);
            ady[i + 1].push_back(i + 1 + n);
            ady[i + 1 + n].push_back(i + 1);
        }

    }

    // leo las capacidades de cada aula y "creo" las aristas i' -> t con capacidad bi

    for (int i = 0; i < n; ++i) {

        int b_i;
        cin >> b_i;

        cantLugares += b_i;
        capacidades[i + 1 + n][t] = b_i;

        if(b_i != 0){
            ady[i + 1 + n].push_back(t);
            ady[t].push_back(i + 1 + n);
        }

    }

    // ahora las aristas i -> j' que permiten intercambio entre las aulas i y j con capacidad ai

    for (int i = 0; i < m; ++i) {

        int p , q;
        cin >> p >> q;

        capacidades[p][q + n] = capacidades[s][p];
        capacidades[q][p + n] = capacidades[s][q];

        // me estaba faltando agregar aca las aristas bidireccionales!!!

        if(capacidades[s][p] != 0){
            ady[p].push_back(q + n);
            ady[q + n].push_back(p);
        }
        if(capacidades[s][q] != 0){
            ady[q].push_back(p + n);
            ady[p + n].push_back(q);
        }

    }

    return make_tuple(cantAlumnos,cantLugares,capacidades,ady);

}

// ALGORITMO EDMONS Y KARP SACADO DE: https://cp-algorithms.com/graph/edmonds_karp.html#edmonds-karp-algorithm

//vector<vector<int>> capacity; // matriz de capacidades capacidades[i][j'] capacidad de arista i -> j'
//vector<vector<int>> adj; // lista de adyacencias, grafo no dirigido, poruqe es para la red residual

int bfs(int s, int t, vector<int>& parent,vector<vector<int>>& capacity, vector<vector<int>>& adj) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next]) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

pair<int,vector<vector<int>>> maxflow(int s, int t,vector<vector<int>>& capacity, vector<vector<int>>& adj) {

    int n = capacity.size();
    int flow = 0;
    vector<int> parent(n);
    int new_flow;

    while (new_flow = bfs(s, t, parent,capacity,adj)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }

    return make_pair(flow,capacity);
}


// como reconstruyo el flujo:
// quiero el flujo que pasa por las aristas i > j'

// tengo la matrices de capacidades, donde las posiciones [i][j'] es lo que puedo seguir pasando de flujo por ahi
// => si para esas posiciones, hago la matriz original - la matriz de capacidades luego de correr el algortimo de flujo maximo
// asi es como obtengo el flujo que pasa por i -> j'

// la matriz capacidades es de 2n + 2 x 2n + 2 quiero la submatriz [1...n][n + 1...2n]

void obtenerFlujo(vector<vector<int>>& capacidadesIniciales, vector<vector<int>>& capacidadesFinales){

    int n = (capacidadesIniciales.size() - 2) / 2;

    for(int i = 1; i <= n; i++){
        for(int j = n + 1; j <= 2 *n; j++){

            capacidadesIniciales[i][j] -= capacidadesFinales[i][j];

        }
    }
}


int main(){

    tuple<int,int,vector<vector<int>>,vector<vector<int>>> datos = solicitarDatos();

    int cantAlumnos = get<0>(datos);

    int cantLugares = get<1>(datos);

    vector<vector<int>> capacidades = get<2>(datos);

    vector<vector<int>> ady = get<3>(datos);

    vector<vector<int>> capacidadesIniciales = capacidades;

    int s = 0;
    int t = capacidades.size() - 1;
    int n = (capacidades.size() - 2) / 2;

    pair<int,vector<vector<int>>> edmonsKarp = maxflow(s,t,capacidades,ady);

    int flujoMaximo = edmonsKarp.first;
    vector<vector<int>> capacidadesFinales = edmonsKarp.second;

    obtenerFlujo(capacidadesIniciales,capacidadesFinales);

    if(cantLugares == cantAlumnos){ // me pide que las aulas esten llenas!!!
        if(cantAlumnos == flujoMaximo){ // si el flujo maximo == a la cantidad de alumnos que quiero reubicar es que se pueden reubicar todos los alumnos!

            cout << "YES" << endl;

            for (int i = 1; i <= n ; ++i) {
                for (int j = n + 1; j <= 2*n ; ++j) {

                    cout << capacidadesIniciales[i][j] << ' ';

                }

                cout << endl;
            }

        }else{
            cout << "NO"<< endl;
        }
    }else{
        cout << "NO" << endl;

    }

    return 0;
}
