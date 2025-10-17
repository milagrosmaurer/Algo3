#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Black Or White

// La subsecuencia pintada de negro tiene que ser ASCENDENTE
// La subsecuencia pintada de blanco tiene que de DESCENDENTE
// La cantidad de numeros sin pintar tiene que ser MINIMA

// Para cada numero de mi secuencia tengo 3 opciones: pintarlo de negro, de blanco, o dejarlo sin pintar

// Mi idea es hacer un algortimo parecido al de skyline creciente, entonces vamos a hacer una estructura de memoizacion de NxNxN
// Y guardarme en M[i][ultN][ultB] donde i es la iteracion en la que me encuentro, ultN es el ultimo que pinte de negro, y ultB el ultimo que pinte de blanco

// Otra cosa que se me ocurre es: encontrar la subsecuencia creciente mas larga (NO FUNCIONA ESTO)
// y encontrar la subsecuencia decresciente mas larga
// Y el resultado vendria a ser: la longitud de ambos - la suma de la longitud de ambas subsecuencias.
// Tendria que tener en cuenta el hecho de que se pueden compartir terminos, como en el siguiente ejemplo:

// 2,4,6,8,10,9,7,5,3,1. Ejemplo de secuencia que comparten terminos
// 2 4 6 8 10
// 10 9 7 5 3 1



vector<vector<int>> solicitarDatos(){

    vector<vector<int>> datos; // Aca voy a almacenar todas las secuencias de mi test

    int x;

    while( cin >> x && x != -1){ //Los lee hasta que aparezca un -1

        vector<int> secuencia(x);


        for(int i = 0; i < x; i++){ // Lee cada int de la linea y lo agrega al vector secuencias
            int numero;
            cin >> numero;
            secuencia[i] = numero;
        }

        datos.push_back(secuencia);

    }
    return datos;

}

int inf = 1e9;

//Me guie como programar este problema con el de Mi Buenos Aires Crecido de turno tarde subido al Campus

int blackOrWhite(vector<int>& secuencia,int i, int ultN, int ultB,vector<vector<vector<int>>>& memo){

    if(i == secuencia.size()){
        return 0;
    }else{

        int indexN = ultN + 1; //Asi no ingreso a memo[i][-1][-1] :/
        int indexB = ultB + 1;

        if(memo[i][indexN][indexB] != -1) return memo[i][indexN][indexB]; //me fijo si ya esta memorizado

        int ultimoGrande = 0;
        int ultimoChico = inf; //Aca me estaba fallando, porque nunca podia empezar a pintar alguno de blanco, ya que la condicion era ultimoChico > actual
        // y ultimoChico = 0, entonces nunca entraba a esa guarda
        if(ultN != -1) ultimoGrande = secuencia[ultN];
        if(ultB != -1) ultimoChico = secuencia[ultB];



        int actual = secuencia[i];

        if(ultimoGrande < actual && ultimoChico > actual){ // Tengo 3 opciones

            memo[i][indexN][indexB] = min(blackOrWhite(secuencia,i+1,i,ultB,memo),
                                          min(blackOrWhite(secuencia,i+1,ultN,i,memo),
                                              blackOrWhite(secuencia,i+1,ultN,ultB,memo) + 1));

        }else if(ultimoGrande < actual && ultimoChico <= actual){ //Tengo dos opciones

            memo[i][indexN][indexB] = min(blackOrWhite(secuencia,i+1,i,ultB,memo),
                                          blackOrWhite(secuencia,i+1,ultN,ultB,memo) + 1);

        }else if( ultimoGrande >= actual && ultimoChico > actual){ // Tengo dos opciones

            memo[i][indexN][indexB] = min(blackOrWhite(secuencia,i+1,ultN,i,memo),
                                          blackOrWhite(secuencia,i+1,ultN,ultB,memo) + 1);

        }else{ // Solo me queda como opcion no pintar
            memo[i][indexN][indexB] = blackOrWhite(secuencia,i+1,ultN,ultB,memo) + 1;
        }

        return memo[i][indexN][indexB];

    }
}





int main() {
    vector<vector<int>> datos = solicitarDatos();

    for(int i = 0; i < datos.size(); i++){

        vector<int> casoIesimo = datos[i];
        int N = casoIesimo.size();

        vector<vector<vector<int>>> memo(N,vector<vector<int>>(N + 1,vector<int>(N + 1,-1)));

        int res = blackOrWhite(casoIesimo,0,-1,-1,memo);

        cout << res << endl;

    }

    return 0;
}
