#ifndef NETWORK_H
#define NETWORK_H
#include "router.h"
#include <fstream>
//#include <cstdlib>
#include <limits.h>

using namespace std;

class Network {
private:
    map<string, Router> tablaEnrutamiento;
    map<string, Router>::iterator tablaEnrutamiento_iterador;

    int numRouters = 0;
    map<int, string> routers;

private:
    int minDistance(int *dist, bool *sptSet);
    void mostrarRuta(int *parent, int j);
    void mostrarSolucion(int *dist, int *parent, int origin, int destiny);
    void llenarGrafo(int **graph);
    void eliminarGrafo(int **graph);
    int getRouterIndex(string router);

public:

    Network();

    bool existeRouter(string r);
    bool existeLink(string r1, string r2);

    void agregarLink(string r1, string r2, int cost);
    void eliminarLink(string r1, string r2);
    void modificarLink(string r1, string r2, int cost);

    void agregarRouter(string name);
    void eliminarRouter(string name);

    void mostrarTabla();

    void dijkstra(string r1, string r2);

    void generarRed();
    void importarRed(string file_name);
    void guardarRed(string file_name);
    void vaciarRed();
};

#endif // NETWORK_H