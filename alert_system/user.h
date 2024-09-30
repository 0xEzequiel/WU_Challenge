#ifndef USER_H
#define USER_H

#include <vector>
#include <string>
#include <set> 
#include "alert.h"
using namespace std;

class User {
public:
    // Props de la clase
    int id;
    string nombre;
    set<int> temasDeInteres;
    vector<Alerta> alertasRecibidas;

    // Metodos de la clase
    // Constructor
    User(int id, const string& name) : id(id), nombre(name) {}

    // Metodo para agregar un tema de interés
    void add_temaDeInteres(int temaID) { temasDeInteres.insert(temaID); }

    // Metodo para recibir una alerta
    void recibirAlerta(const Alerta& alerta) { alertasRecibidas.push_back(alerta); }

    // Método para marcar una alerta como leída
    void marcarAlertaComoLeida(int alertaID) {
        for (auto& alerta : alertasRecibidas) {
            if (alerta.id == alertaID) {
                alerta.leida = true;
                cout << "Alerta marcada como leida." << endl;
                return;
            }
        }
        cout << "Alerta no encontrada!" << endl;
    }

    // Metodo para obtener alertas no leídas y no expiradas
    vector<Alerta> obtenerAlertasNoLeidasNoExpiradas() const { // agregado el const
        vector<Alerta> alertasNoLeidasNoExpiradas;
        time_t ahora = time(0);
        for (const auto& alerta: alertasRecibidas) {
            if (!alerta.leida && !alerta.alertaExpirada()) {
                alertasNoLeidasNoExpiradas.push_back(alerta);
            }
        }
    return alertasNoLeidasNoExpiradas;
}
};

#endif