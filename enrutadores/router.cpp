#include "router.h"
using namespace std;

Router::Router(){

}

void Router::agregarLink(string destino, int costo) {
    if(links.find(destino) == links.end()) // if the link doesn't exist
        links.insert(pair<string, int>(destino, costo));
    else if (links[destino] == -1)
        links[destino] = costo;
}

void Router::modificarLink(string destino, int costo) {
    if(links.find(destino) != links.end())
        links[destino] = costo;
}

void Router::eliminarLink(string destino) {
    if(links.find(destino) != links.end())
        links.erase(destino);
}

bool Router::estaEnlazado(string destino) {
    if(links[destino] != -1) return true;

    else return false;
}

bool Router::estaEnlazado() {
    for(links_iterador = links.begin(); links_iterador != links.end(); links_iterador++)
        if(links_iterador->second != -1 && links_iterador->second != 0)
            return true;

    return false;
}


void Router::mostrarLinks(bool group) {
    if(group){
        for(links_iterador = links.begin(); links_iterador != links.end(); links_iterador++)

            if(links_iterador->second != -1)
                cout<<links_iterador->second<<'\t';
            else
                cout<<'-'<<'\t';
        cout<<endl;
    }
    else{
        for(links_iterador = links.begin(); links_iterador != links.end(); links_iterador++)

            if(links_iterador->second != 0){

                if(links_iterador->second != -1)
                    cout<<links_iterador->first<<": "<<links_iterador->second<<'\t';
                else
                    cout<<links_iterador->first<<": "<<'-'<<'\t';
            }
        cout<<endl;
    }
}