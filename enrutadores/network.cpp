
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

        routers.insert(pair<int, string>(numRouters, name));
        numRouters++;

        for(int i = 0; i<numRouters; i++){

            if(name == routers[i])
                r.agregarLink(routers[i], 0);
            else
                r.agregarLink(routers[i]);
        }

        tablaEnrutamiento.insert(pair<string, Router>(name, r));

        for(tablaEnrutamiento_iterador=tablaEnrutamiento.begin(); tablaEnrutamiento_iterador != tablaEnrutamiento.end(); tablaEnrutamiento_iterador++) //adding a link with "name" to all existing routers
            tablaEnrutamiento_iterador->second.agregarLink(name);
    }
}

void Network::eliminarRouter(string name){

    tablaEnrutamiento.erase(name);

    for(tablaEnrutamiento_iterador = tablaEnrutamiento.begin(); tablaEnrutamiento_iterador != tablaEnrutamiento.end(); tablaEnrutamiento_iterador++) //deleting all links with 'name' from all existing routers
        tablaEnrutamiento_iterador->second.eliminarLink(name);

    if(tablaEnrutamiento.size()>1){
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

void Network::agregarLink(string r1, string r2, int cost){

    tablaEnrutamiento[r1].agregarLink(r2, cost);
    tablaEnrutamiento[r2].agregarLink(r1, cost);
}

void Network::eliminarLink(string r1, string r2){

    tablaEnrutamiento[r1].eliminarLink(r2);
    tablaEnrutamiento[r2].eliminarLink(r1);
}

void Network::modificarLink(string r1, string r2, int cost){

    tablaEnrutamiento[r1].modificarLink(r2, cost);
    tablaEnrutamiento[r2].modificarLink(r1, cost);
}


void Network::generarRed(){

    short name_code, gen_routers = 2 + (rand() % (20 - 2 + 1));
    string name;
    int numLinks, r1, r2, c, prev_links = 0;

    while(gen_routers > numRouters){
        //numero aleatorio de routers
        name_code = 65 + (rand() % (90 - 65 + 1));
        name = char(name_code);

        if(!existeRouter(name)){
            agregarRouter(name);
        }
    }

    for(int i = 0; i < numRouters; i++){
        //Debe tener al menos un link
        if(!tablaEnrutamiento[routers[i]].estaEnlazado()){

            while(true){

                name_code = rand() % numRouters;

                if(name_code != i){

                    c = 1 + (rand() % (100));

                    agregarLink(routers[i], routers[name_code], c);
                    prev_links++;

                    break;
                }
            }
        }
    }

    numLinks = (((numRouters-1) * (numRouters))/2) - prev_links;

    if(numLinks > 0)
        numLinks = 1 + (rand() % (numLinks));

    while(numLinks > 0){
        //numero aleatorio de links
        r1 = rand() % (numRouters);
        r2 = rand() % (numRouters);

        if(r1 != r2){

            if(existeLink(routers[r1], routers[r2]) == false){

                c = 1 + (rand() % (100));

                modificarLink(routers[r1], routers[r2], c);

                numLinks--;
            }
        }
    }
}

void Network::importarRed(string file_name){

    fstream file (file_name,  fstream::in | fstream::binary);

    if(file.is_open()){

        string temp, r1, r2;
        int cost;
        short counter = 1;

        while (file >> temp){

            if(counter == 1){ //router 1
                r1 = temp;
                agregarRouter(r1);
            }
            else if (counter == 2){ //router 2
                r2 = temp;
                agregarRouter(r2);
            }
            else if (counter == 3){ //cost
                cost = stoi(temp);

                tablaEnrutamiento[r1].modificarLink(r2, cost);
                tablaEnrutamiento[r2].modificarLink(r1, cost);

                counter = 0;
            }

            counter++;
        }
        file.close();
        cout << "Red importada.";
    }
    else {
        cout<<"Error importando la red."<<endl;
    }
}

void Network::guardarRed(string file_name){

    fstream file (file_name,  fstream::out | fstream::binary);

    if(file.is_open()){

        string data;
        string key1, key2;
        int cost;

        for(int i = 0; i < numRouters; i++){

            key1 = routers[i];

            for(int j = 0; j < numRouters; j++){

                key2 = routers[j];

                cost = tablaEnrutamiento[key1].links[key2];

                if(cost>0){
                    data += key1;
                    data += ' ';
                    data += key2;
                    data += ' ';
                    data += to_string(cost);
                    data += "\n";
                }
            }
        }

        file.write(data.c_str(), data.length());

        file.close();
        cout<<"Red exportada correctamente"<<endl;
    }
    else {
        cout<<"No se puedo abrir el archivo."<<endl;
    }
}

void Network::vaciarRed(){
    routers.clear();
    tablaEnrutamiento.clear();
    numRouters = 0;
}