#include <iostream>
#include <vector>
#include <sstream>



using namespace std;

// https://vjudge.net/problem/UVA-11790

// Idea: si estoy parada en posicion i fijarme en mi vector mem desde j = 0 hasta i-1, en caso de que alturas[j] > alturas[i]
// el maximo entre agregar el ancho del edificio i a mem[j] o no agregarlo, osea mem[i] , el mejor caso ahsta el momento
// si mem[i] es mejor que el que tengo guardado como mejor caso, mem[i] pasa a ser mejor caso (to do esto es en caso creciente, en decreciente es analogo)

//Esta idea la saque del ejercicio de maximo largo de subsecuencia y la adapte a este ejercicio.

struct skyline { //esto me sirvio mucho al momento de impresion sino me pasaba que me imprimia mientras se resolvia el ejercicio
    skyline(int e, vector<int> an,vector<int> al): edificios(e),anchos(an),alturas(al){}
    int edificios;
    vector<int> anchos;
    vector<int> alturas;
};

vector<skyline> solicitarDatos(int casos){

    vector<skyline> datos;

    for (int i = 0; i < casos; ++i) {
        vector<int> anchos;
        vector<int> alturas;
        int edificios;
        cin >> edificios;
        cin.ignore();

        string linea;
        getline(cin,linea);
        stringstream ss1(linea); //ss1 manipula cadena de caracteres
        int temp;
        while(ss1 >> temp){ //se hace mientras sea posible extraer valores de la cadena de caraceteres
            alturas.push_back(temp);
        }

        getline(cin,linea);
        stringstream ss2(linea); //ss2 tambien, a diferencia de que ambas son independiente
        while(ss2 >> temp){
            anchos.push_back(temp);
        }
        datos.push_back(skyline(edificios,anchos,alturas));
    }
    return datos;
}



int skylineCreciente(vector<int> anchos, vector<int> alturas,int edificios){
    int mejorCaso = 0;
    vector<int> mem(edificios,0); //inicializo mi estructura de memoizacion como un vector de largo cantidad de edificos con ceros

    for(int i = 0;i<edificios;i++){
        mem[i] = anchos[i]; // siempre existe la posibilidad de que se tenga que empezar de mas adelante, en este caso por eso hacemos mem[i] = anchos[i]
         for(int j = 0; j<i;j++){
            if(alturas[j]<alturas[i]){ //como estoy en i y me quiero fijar los de atras, veo el caso en el que atras es mas chico, y el de adelante osea i es mas alto
                mem[i]=max(mem[i],mem[j]+anchos[i]); //maximo entre agregar ancho[i] o no agregarlo
            }
        }
        if(mem[i]>mejorCaso){
            mejorCaso = mem[i]; //actualizacion de mejor caso
        }
    }
    return mejorCaso;
}

int skylineDecreciente(vector<int> anchos, vector<int> alturas,int edificios){ //analogo a creciente pero cambiando condicion de if
    int mejorCaso = 0;
    vector<int> mem(edificios,0);
    for(int i = 0;i<edificios;i++){
        mem[i] = anchos[i];

        for(int j = 0; j<i;j++){
            if(alturas[j]>alturas[i]){
                mem[i]=max(mem[i],mem[j]+anchos[i]);
            }
        }
        if(mem[i]>mejorCaso){
            mejorCaso =mem[i];
        }
    }
    return mejorCaso;
}


int main(){
    int casos;
    cin >> casos;
    cin.ignore();

    vector<skyline> datos = solicitarDatos(casos);

    for(int i = 0;i<casos;i++){
        skyline actual = datos[i];

        int resultadoCreciente = skylineCreciente(actual.anchos,actual.alturas,actual.edificios);
        int resultadoDecreciente = skylineDecreciente(actual.anchos,actual.alturas,actual.edificios);
        if(i<casos-1 ) {
            if (resultadoCreciente >= resultadoDecreciente) {
                cout << "Case " << i + 1 << "." << " Increasing " << "(" << resultadoCreciente << ")." << " Decreasing "
                     << "(" << resultadoDecreciente << ")." << endl;
            } else {
                cout << "Case " << i + 1 << "." << " Decreasing " << "(" << resultadoDecreciente << ")."
                     << " Increasing " << "(" << resultadoCreciente << ")." << endl;
            }
        }else{
            if (resultadoCreciente >= resultadoDecreciente) {
                cout << "Case " << i + 1 << "." << " Increasing " << "(" << resultadoCreciente << ")." << " Decreasing "
                     << "(" << resultadoDecreciente << ")." << endl;
            } else {
                cout << "Case " << i + 1 << "." << " Decreasing " << "(" << resultadoDecreciente << ")."
                     << " Increasing " << "(" << resultadoCreciente << ")." << endl;
            }
        }

    }

    return 0;
}












