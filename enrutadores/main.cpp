#include <iostream>
#include <limits>
#include "network.h"

using namespace std;

void mostrarOpcionesDeRed();
void mostrarMenu();
int costo();
void getEnrutadores(Network& red, string& nameR, string& nameR2, int n=1);

int main() {
    Network red;
    int opcion;
    while (true) {
        mostrarOpcionesDeRed();
        cin >> opcion;
        if (opcion == 1){
            string fileName;
            cout << "Ingrese el nombre del archivo fuente: ";
            cin >> fileName;
            red.importarRed(fileName);

        }else if (opcion == 2){

            red.generarRed();

        }else if (opcion == 3){

            string name1,name2;
            int cost = 0;


            cout << "Ingrese dos enrutadores para crear red:"<<endl;
            cout << "Enrutador 1: ";
            cin >> name1;
            cout << "Enrutador 2: ";
            while (true){
                cin >> name2;
                if (name2 == name1){
                    cout<<"Los enrutadores no pueden ser iguales."<<endl;
                }
                else break;
            }
            cout << "Costo entre enrutadores: ";
            cost = costo();


            red.agregarRouter(name1);
            red.agregarRouter(name2);
            red.modificarLink(name1, name2, cost);

        }else {
            cout << "Saliendo del programa..."<<endl;
            break;

        }

        int opcion2;
        bool salir = false;
        while (!salir){
            mostrarMenu();
            cin >> opcion2;

            string nameR = "";
            string nameR2 = "";

            switch (opcion2) {
            case 1: {

                getEnrutadores(red,nameR,nameR2,2);

                int cost;
                cout << "Ingrese el costo del enlace: ";
                cost = costo();

                red.agregarRouter(nameR);
                red.agregarLink(nameR, nameR2, cost);

                break;
            }
            case 2: {
                string nameR;
                cout << "Ingrese el nombre del enrutador a eliminar: ";
                cin >> nameR;

                if (!red.existeRouter(nameR)){
                    cout<<"El enrutador no existe."<<endl;
                    break;
                }

                red.eliminarRouter(nameR);

                break;
            }
            case 3: {

                getEnrutadores(red,nameR,nameR2);

                int cost;
                cout <<"Ingrese el costo del enlace: ";
                cost = costo();

                red.agregarLink(nameR,nameR2,cost);

                break;
            }
            case 4: {

                getEnrutadores(red,nameR,nameR2);

                if (!red.existeLink(nameR,nameR2)){
                    cout << "Los enrutadores no están enlazados"<<endl;
                    break;
                }

                red.modificarLink(nameR,nameR2,-1);
                break;

            }
            case 5: {

                getEnrutadores(red,nameR,nameR2);

                int cost;
                cout<<"Ingrese el costo del enlace: ";
                cost = costo();

                red.modificarLink(nameR,nameR2,cost);

                break;

            }

            case 6: {

                getEnrutadores(red,nameR,nameR2);

                red.dijkstra(nameR,nameR2);
                break;

            }
            case 7: {

                red.mostrarTabla();

                break;

            }

            case 8: {
                string fileName;
                cout<<"Ingrese el nombre del archivo: ";
                cin>>fileName;

                red.guardarRed(fileName);

                break;
            }
            default:{
                salir = true;
                red.vaciarRed();
                cout << "Volviendo al menu principal..." << endl;
                break;
            }
            }
        }
    }

    return 0;
}

void mostrarOpcionesDeRed(){
    cout<<"\nMenu de creacion de red:" << endl;
    cout<<"(1) Importar red" << endl;
    cout<<"(2) Generar red" << endl;
    cout<<"(3) Crear red" << endl;
    cout<<"(#) Salir" << endl;
    cout<<"Seleccione una opcion: ";
}

void mostrarMenu() {
    cout << "\nMenu de opciones:" << endl;
    cout << "(1) Agregar enrutador" << endl;
    cout << "(2) Remover enrutador" << endl;
    cout << "(3) Agregar enlace" << endl;
    cout << "(4) Remover enlace" << endl;
    cout << "(5) Modificar enlace" << endl;
    cout << "(6) Calcular envio" << endl;
    cout << "(7) Ver tabla enrutamiento"<<endl;
    cout << "(8) Exportar red"<<endl;
    cout << "(#) Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int costo() {
    int number;
    while (true) {
        cout << "Ingresa el costo del enlace: ";
        cin >> number;

        if (cin.fail()) { // Verifica si la entrada no es válida
            cout << "Entrada invalida." << endl;
            cin.clear(); // Limpia el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar entrada inválida
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar caracteres sobrantes
            return number; // Entrada válida
        }
    }
}

void getEnrutadores(Network& red, string& nameR, string& nameR2,int n) {
    if (n == 1) {
        while (true) {
            cout << "Ingrese el enrutador de origen: ";
            cin >> nameR;

            if (red.existeRouter(nameR)) {
                break;
            } else {
                cout << "El enrutador no existe. Intente de nuevo." << endl;
            }
        }

        while (true) {
            cout << "Ingrese el enrutador de destino: ";
            cin >> nameR2;

            if (!red.existeRouter(nameR2)) {
                cout << "El enrutador no existe. Intente de nuevo." << endl;
            } else if (nameR == nameR2) {
                cout << "El enrutador de destino no puede ser el mismo que el de origen. Intente de nuevo." << endl;
            } else {
                break;
            }
        }
    }
    else {
        while (true) {
            cout << "Ingrese el nuevo enrutador: ";
            cin >> nameR;

            if (red.existeRouter(nameR)) {
                cout << "El enrutador ya existe. Intente de nuevo."<<endl;
            } else {
                break;
            }
        }

        while (true) {
            cout << "Ingrese el enrutador de destino: ";
            cin >> nameR2;

            if (!red.existeRouter(nameR2)) {
                cout << "El enrutador no existe. Intente de nuevo." << endl;
            } else if (nameR == nameR2) {
                cout << "El enrutador de destino no puede ser el mismo que el de origen. Intente de nuevo." << endl;
            } else {
                break;
            }
        }

    }
}