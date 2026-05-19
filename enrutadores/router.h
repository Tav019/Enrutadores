#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <map>
#include <string>

using namespace std;

class Router {
public:
    map<string, int> links;
    map<string, int>::iterator links_iterador;

    Router();

    void agregarLink(string destino, int costo = -1);
    void modificarLink(string destino, int costo);
    void eliminarLink(string destino);

    bool estaEnlazado(string destino);
    bool estaEnlazado();

    void mostrarLinks(bool group = false);
};

#endif // ROUTER_H
