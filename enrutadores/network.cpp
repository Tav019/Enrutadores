
#include "network.h"
using namespace std;

Network::Network(){
}

bool Network::existeRouter(string r){
    if(tablaEnrutamiento.find(r) != tablaEnrutamiento.end()) return true;
    else return false;
}

bool Network::existeLink(string r1, string r2){

    if (tablaEnrutamiento.find(r1) == tablaEnrutamiento.end()) return false;
    if (tablaEnrutamiento.find(r2) == tablaEnrutamiento.end()) return false;

    if(tablaEnrutamiento[r1].estaEnlazado(r2) && tablaEnrutamiento[r2].estaEnlazado(r1)) return true;
    else return false;

}

void Network::agregarRouter(string name){

    if(tablaEnrutamiento.find(name) == tablaEnrutamiento.end()){

        Router r;

        routers.insert(pair<int, string>(numRouters, name)); //gets added to the list of routers
        numRouters++;

        for(int i = 0; i<numRouters; i++){ //create links with all routers

            if(name == routers[i])
                r.agregarLink(routers[i], 0);
            else
                r.agregarLink(routers[i]);
        }

        tablaEnrutamiento.insert(pair<string, Router>(name, r)); //add new router to routing table

        for(tablaEnrutamiento_iterador=tablaEnrutamiento.begin(); tablaEnrutamiento_iterador != tablaEnrutamiento.end(); tablaEnrutamiento_iterador++) //adding a link with "name" to all existing routers
            tablaEnrutamiento_iterador->second.agregarLink(name);
    }
}

void Network::eliminarRouter(string name){

    tablaEnrutamiento.erase(name); //delete router from routing table

    for(tablaEnrutamiento_iterador = tablaEnrutamiento.begin(); tablaEnrutamiento_iterador != tablaEnrutamiento.end(); tablaEnrutamiento_iterador++) //deleting all links with 'name' from all existing routers
        tablaEnrutamiento_iterador->second.eliminarLink(name);

    if(tablaEnrutamiento.size()>1){ // Creates a new "numRouters" that doesn't include "name"
        map<int, string>temp;
        int temp_index = 0;

        for(int i = 0; i<numRouters; i++)
            if(routers[i] != name){
                temp.insert(pair<int, string>(temp_index, routers[i]));
                temp_index++;
            }
        routers = temp;
        numRouters = temp_index;
    }
    else this->vaciarRed();
}

void Network::mostrarTabla(){

    cout<<"\t";

    for(tablaEnrutamiento_iterador=tablaEnrutamiento.begin();tablaEnrutamiento_iterador != tablaEnrutamiento.end(); tablaEnrutamiento_iterador++)
        cout<<tablaEnrutamiento_iterador->first<<'\t';

    cout<<endl;

    for(tablaEnrutamiento_iterador=tablaEnrutamiento.begin();tablaEnrutamiento_iterador != tablaEnrutamiento.end(); tablaEnrutamiento_iterador++){
        cout<<tablaEnrutamiento_iterador->first<<'\t';
        tablaEnrutamiento_iterador->second.mostrarLinks(true);
        cout<<endl;
    }
}
//Encuentra el enrutador con menor distancia que no este en sptSe
int Network::minDistance(int *dist, bool *sptSet){
    int min = INT_MAX, min_index;

    for (int v = 0; v < numRouters; v++){
        if (sptSet[v] == false && dist[v] <= min && dist[v] != -1){
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}

void Network::mostrarRuta(int *parent, int j) {

    if (parent[j] == - 1) return;

    mostrarRuta(parent, parent[j]);//recursividad
    cout<<routers[j]<<' ';
}

void Network::mostrarSolucion(int *dist,  int *parent, int origin, int destiny){

    if(dist[destiny] < INT_MAX && dist[destiny] > 0){

        printf("Conexion\tCosto\tCamino");

        cout<<'\n'<<routers[origin]<<" -> "<<routers[destiny]<<"\t\t"<<dist[destiny]<<"\t"<<routers[origin]<<' ';

        mostrarRuta(parent, destiny);
    }
    else cout<<"No existe ningun camino entre "<<routers[origin]<<" y "<<routers[destiny]<<endl;
}

// Rellena la matriz con los costos de los enlaces
void Network::llenarGrafo(int **graph){

    string key1, key2;

    for(int i = 0; i < numRouters; i++){

        key1 = routers[i];

        for(int j = 0; j < numRouters; j++){

            key2 = routers[j];

            graph[i][j] = tablaEnrutamiento[key1].links[key2];

        }
    }
}

void Network::eliminarGrafo(int **graph){

    for(int i = 0; i < numRouters; ++i) {
        delete[] graph[i];
    }
    delete[] graph;
}

int Network::getRouterIndex(string router){

    for(int k = 0; k < numRouters; k++){
        if(routers[k] == router){
            return k;
        }
    }

    return -1;
}

void Network::dijkstra(string r1, string r2){

    int origin = this->getRouterIndex(r1);
    int destiny = this->getRouterIndex(r2);

    int** graph = new int*[numRouters];

    for (int i = 0; i < numRouters; ++i){
        graph[i] = new int[numRouters];
    }

    llenarGrafo(graph);

    int dist[numRouters];//distancia minima desde el origen hasta router i
    bool sptSet[numRouters];//verifica si el nodo está incluido en el conjunto de caminos más cortos
    int parent[numRouters];//almacena la ruta

    for (int i = 0; i < numRouters; i++) {

        dist[i] = INT_MAX;
        parent[origin] = -1;
        sptSet[i] = false;
    }

    dist[origin] = 0;

    for (int count = 0; count < numRouters - 1; count++) {

        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < numRouters; v++)
            //v no ha sido procesado, existe enlace directo, comparacion de distancias, no existe
            if (!sptSet[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v] && graph[u][v] != -1) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }

    mostrarSolucion(dist, parent, origin, destiny);

    eliminarGrafo(graph);
}