#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

// Divide and conquer
// La idea es hacer busqueda binaria sobre los posibles lugares de los establos
// Entonces, me fijo para cada n = 1...10^9 (maximos lugares para lo establos) si hay una forma de poner la cantidad C de vacas que necesito ubicar
// Si es asi, me muevo a la derecha, y si no a la izquierda, el maximo aparece cuando left + 1 == right
// X con xi, i = 0...10^9 es el arreglo en el cual voy a hacer la busqueda binaria

// La idea es que me quede complejidad O(Nlog(X))
//Osea que mi funcion de esPosible(cantVacas, establos, nroAIntentar) tiene que ser O(N)

vector<pair<int,vector<int>>> solicitarDatos(){

    int casos;
    cin >> casos;

    vector<pair<int,vector<int>>> datos;

    for(int i = 0; i < casos; i++){

        int N,C; //N cant de establos, C cant de vacas
        cin >> N >> C;

        vector<int> establos;

        for(int j = 0; j < N; j++){
            int posEstablo;
            cin >> posEstablo;
            establos.push_back(posEstablo);
        }

        sort(establos.begin(),establos.end()); // Los ordeno O(NlogN) << O(NlogX), asi es mas facil mi funcion de esPosible
        datos.push_back(make_pair(C,establos));
    }
    return datos;
}

//con esta funcion me voy a fijar, dado un numero si es posible ubicar las vacas en los establos teniendo en cuenta que
// la MINIMA distancia que se lleven debe ser ese numero
// TODO esto lo hice pensando en el ejemplo, quiza este

// Ejemplo

// 5 establos 3 vacas a ubicar
// establos = [1,2,4,8,9]
// Para que mi busqueda binaria funcione necesito que con nroAIntentar = 1,2,3 me de TRUE y que a partir de nroAIntentar = 4 en adelante FALSE
// Entonces mi bsuqueda binaria me va a devolver 3 que es el punto de corte en donde todos son true y a partir del 3 todos son false

bool esPosible (vector<int>& establos, int vacas, int nroAIntentar){ // Complejidad O(N), pues recorro una vez sola mis establos

    int i = 0; // Siempre empiezo en el primero, osea ubico una vaca en el primer establo
    int j = 1;
    bool res;
    int cantVacasUbicadas = 1; // Ya la ubico en el primer establo


    while(i < establos.size() && j < establos.size()) { // Asi no me paso de rango!!
        if (establos[j] - establos[i] >=  nroAIntentar) { // Si la distancia entre los establos es mayor o igual a la que estoy buscando entonces
            // i ahora es j, donde ubique mi ultima vaca
            i = j;
            j++; // itero sobre las posiciones que le siguen a j, asi i nunca es igual a j y no repito posiciones
            cantVacasUbicadas++;


        } else { // Si no lo cumple sigo iterando con el resto del arreglo a ver si hay alguna posicion que lo cumpla
            j++;
        }
    }


    //Si la cantidad de vacas ubicadas es mayor o igual a las que tenia para ubicar to do bien
    if(cantVacasUbicadas >= vacas ){
        res = true;
    }else{
        res = false;
    }

    return res;
}



//Busqueda binaria entre TO DO el rango de posiciones, de 0 a 10^9

// int va hasta 2^32, es por eso que no tengo que poner long long ni nada de eso

int busquedaBinaria(vector<int>& establos,int vacas){

    int low = 0;

    int high = 1e9; //hasta este numero puede estar mi solucion!!!
    // Fijarse que el peor caso es tener que ubicar dos vacas, en donde nos conviene ubicarlo en la pos 1 y pos 10^9 (si hay establos ahi ubicados)


    while(low + 1 != high){ // Corte

        int mid = (high + low) / 2;

        if(esPosible(establos,vacas,mid)){// Si es posible, me muevo a la derecha, porque estoy buscando un maximo!!!!
            //Chequear esta condicion es O(N)
            low = mid;
        }else{ // Si no, me muevo a la iquierda, pues todavia no lo encontre y es mas chico
            high = mid;
        }


    }

    return low;
} // Complejidad de busqueda binaria O(NlogX)



int main() {
    vector<pair<int,vector<int>>> datos = solicitarDatos();



    for(int i = 0; i < datos.size(); i++){

        vector<int> establosIesimo = datos[i].second;
        int vacas = datos[i].first;


        int res = busquedaBinaria(establosIesimo,vacas);
        cout << res << endl;
    }

    return 0;
}
