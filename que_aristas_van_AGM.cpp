#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

// KRUSKAL: uso Kruskal porque necesito los bosques, para saber cuando mi arista une dos componentes conexas distintas
// Si la arista u - v  tiene peso unico, y dsu.find(u) != dsu.find(v) -> pertenece a cualquier AGM
// Si la arista u - v tiene peso unico, y dsu.find(u) == dsu.find(v) (o #aristas arbol = n - 1)  -> no pertenece a ningun AGM
// Si me encuentro un grupo de aristas de igual peso -> armo un grafo G' con cada nodo representando una componente conexa del bosque que
// arma Kruskal, agrego las aristas con el mismo peso, corro DFS y puentes y tengo dos opciones:
// - Si es puente es porque une dos componentes conexas distintas -> pertenece a cualquier AGM
// - Si no es puente es porque hay un ciclo - > pertenece a por lo menos un AGM


// variables globales
int none = -1 , atLeastOne = 1, any = 0;

// Mi arista va a ser nodo u, nodo v, peso de la arista w, e index i que es el orden en que lei el input

struct edge{ // Estas van a ser las aristas de mi grafo
    edge(int u, int v, int w, int index): u(u),v(v),w(w),index(index){}
    int u;  // nodo
    int v;  // nodo
    int w;  // peso de la arista
    int index; // index es el orden en el que lei el input, me sirve para imprimir el output
};

pair<int,vector<edge>> solicitarDatos(){

    // Me guardo n porque lo necesito para crear el conjunto de dsu


    int n,m; // n nodos, m aristas
    cin >> n >> m;

    vector<edge> datos;

    for(int i = 0 ; i < m; i++){
        int u,v,w;

        cin >> u >> v >> w;

        edge arista = edge(u,v,w,i); // i momento en que lei el input

        datos.push_back(arista);
    }

    return make_pair(n,datos);
}

// Codigo de la catedra


class DisjointSet {

    vector<int> rank, parent;

public:

    DisjointSet(int n) {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        for(int i = 0; i < n + 1; i++){
            parent[i] = i;
        }
    }

    int findSet(int node){

        // En caso que nodo sea el representante
        if (node == parent[node]) return node;

        // Hago path compression
        return parent[node] = findSet(parent[node]);
    }

    void unionByRank(int u, int v) {
        int uRepresentative = findSet(u);
        int vRepresentative = findSet(v);

        // Si tienen el mismo representante, entonces pertenece al
        // mismo conjunto
        if (uRepresentative == vRepresentative) return;

        // Actualizamos el representante segun el caso del rank
        if (rank[uRepresentative] < rank[vRepresentative]) {
            parent[vRepresentative] = uRepresentative;
        } else if(rank[uRepresentative] > rank[vRepresentative]) {
            parent[uRepresentative] = vRepresentative;
        } else {
            parent[vRepresentative] = uRepresentative;
            rank[uRepresentative]++;
        }
    }
};

// Para encontrar puentes en mi grafo G'

// LINK DEL CODIGO: https://cp-algorithms.com/graph/bridge-searching.html

// nota: lo bueno de este algoritmo es que me detecta las aristas no puentes para multigrafos, cosa que el cubren de la practica, no sabia como cambiarlo


int timer = 0;
void dfs(int v, int p, vector<int>& low, vector<int>& tin, vector<bool>& visited, vector<vector<pair<int,int>>>& adj, vector<int>& estados) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    // tin es el tiempo de descubrimiento del nodo (como hace el del Cormen)
    bool parent_skipped = false; // con esto hace lo de los multigrafos
    // low es el menor de los tiempos de todos los hijos de v, si hay un ciclo , para todos los nodos que forman parte de ese ciclo, van a tener el mismo low
    for (pair<int,int> to : adj[v]) {
        int u = to.first;
        int index = to.second;
        if (u == p && !parent_skipped) {
            parent_skipped = true;
            continue;
        }

        if (visited[u]) {
            low[v] = min(low[v], tin[u]);
        } else {
            dfs(u, v,low,tin,visited,adj,estados);
            low[v] = min(low[v], low[u]);
            if (low[u] > tin[v]){
                estados[index] = any;
            }



        }
    }
}



// funcion para ordenar aristas segun peso
bool compararAristas(const edge& a, const edge& b){
    return a.w < b.w;
}

// Grafo como lista de aristas
vector<int> queAristasVanEnUnAGM(vector<edge>& grafo,int n){

    int m = grafo.size(); // Cant aristas del grafo

    vector<vector<pair<int,int>>> ady(n + 1); // grafo G', ademas me lo guardo con sus respectivos index asi a medida
    // que voy encontrando puentes los cambio a any en la misma funcion de encontrar puentes

    vector<int> estados(m,none); //este va a ser mi output, inicializo todas como none
    // Si pertenecen al grafo G' dejan de ser none y pueden pasar a ser atleastone, any, dependiendo si es puente o no

    DisjointSet dsu(n);

    map<int,pair<int,int>> mapaRep; // Aca me guardo un contador con la arista
    map<int,int> mapaIndex; // Aca me guardo un contador con el orden del index del output
    // estos mapas son mas que nada para iterar sobre las aristas que me importan y poder encontralas mas rapido
    //vector<int> mapaCount(n + 1, -1);

    sort(grafo.begin(),grafo.end(), compararAristas);

    int i = 0; // Empiezo a iterar por las aristas en orden ascendente de pesos

    vector<bool> visited(n + 1, false);
    vector<int> tin(n + 1, -1);
    vector<int> low(n + 1, -1);
    //vector<bool> clasificada(m, false);

    while ( i < m ){

            int peso = grafo[i].w; // Me fijo si las aristas que le siguen son de este peso y voy agregando a G'

            // Esto lo hago SIEMPRE, SIN IMPORTAR QUE LA ARISTA SEA DE PESO UNICO

            int j = i;

            int count = 0; // contador para no perder datos





            while( j < m && grafo[j].w == peso){ // Encuentro aristas del mismo peso

                // armo grafo G' que una vertices de DISTINTAS COMPONENTES CONEXAS
                // corro dfs
                // me fijo si es puente
                // si es puente es any
                // si no es puente hay un ciclo puedo elegir entre cualquiera de las aristas del mismo peso que formen parte de ese ciclo

                // Primero reinicio las variables globales, asi si tengo que hacer dos veces G' con dos grafos distintos no me piso los resultados

                int u = grafo[j].u;
                int v = grafo[j].v;
                int index = grafo[j].index;

                int rep_u = dsu.findSet(u);
                int rep_v = dsu.findSet(v);

                if(rep_u != rep_v) { // Todas las que no entran aca ya estan etiquetadas como none antes
                    // Ahora, las que entran aca son las que pertenecen a mi grafo G' -> son o at least one o  son any, no hay ninguna none


                    ady[rep_v].push_back(make_pair(rep_u, index));
                    ady[rep_u].push_back(make_pair(rep_v, index)); // Armo el grafo por COMPONENTES CONEXAS
                    // me guardo el indice aca, porque con tarjan encontre la forma de ir clasificando las aristas a medida que


                    mapaRep[count] = {rep_v, rep_u};
                    mapaIndex[count] = index;
                    //mapaCount[rep_v] = count;
                    //mapaCount[rep_u] = count;

                    estados[index] = atLeastOne;

                    count++;

                //}



                }
                j++;

            }

            // Ya termine de armar G'

            i = j; // Vuelvo a actualizar i para que siga con sus iteraciones

            for(int k = 0 ; k < count ; k++){ // Ahora, con las aristas que agregue a G' me fijo si es o no es puente

                int q = (mapaRep.find(k) -> second).first;
                int r = (mapaRep.find(k) -> second).second;
                int index = mapaIndex.find(k) -> second; // esto al final no lo uso porque etiqueto en el mismo dfs, pero no lo borro por las dudas
                // Lo pongo como atleastone hasta demostrar que es puente
                //estados[index] = atLeastOne;

                if(!visited[q]){
                    dfs(q,-1,low,tin,visited,ady,estados);
                }

                dsu.unionByRank(q,r);
                ady[q].clear();
                ady[r].clear();
                //mapaCount[q] = -1;
                //mapaCount[r] = -1;
                // A partir de aca vuelvo a limpiar todos los vectores para dfs, lo hago asi porque con .assign me daba TLE
                // ademas todas las aristas que veo ya les hice sus respectivas modificaciones en estados con lo cual

                visited[q] = false;
                visited[r] = false;
                tin[q] = -1;
                tin[r] = -1;
                low[q] = -1;
                low[r] = -1;
            }


    }

    return estados;



}




int main() {
    pair<int, vector<edge>> datos = solicitarDatos();

    int n = datos.first;

    vector<edge> grafo = datos.second;

    vector<int> estados = queAristasVanEnUnAGM(grafo,n);

    for(int i = 0 ; i < grafo.size() ; i ++){

        if(estados[i] == none) cout << "none" << endl;
        if(estados[i] == atLeastOne) cout << "at least one" << endl;
        if(estados[i] == any) cout << "any" << endl;

    }




    return 0;
}
