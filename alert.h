#ifndef ALERTA_H
#define ALERTA_H

#include <string>
#include <ctime> 
using namespace std;

enum class tipoAlerta { 
    Informativa, 
    Urgente
};

class Alerta {
public:
    // Props de la clase
    int id;
    int topic_ID;
    tipoAlerta tipo;
    time_t fechaExpiracion;
    string mensaje;
    int user_ID;  // -1 indica que es para todos los users
    bool leida; 

    // Metodos de la clase
    // Constructor
    Alerta(int id, int tema_ID, tipoAlerta tipo, time_t fechaExpiracion, const string& mensaje, int usuario_ID = -1)
        : id(id), topic_ID(tema_ID), tipo(tipo), fechaExpiracion(fechaExpiracion), mensaje(mensaje), user_ID(usuario_ID), leida(false) {}

    // Metodo para verificar si la alerta expiró
    bool alertaExpirada() const {
        time_t ahora = time(0);
        return difftime(fechaExpiracion,ahora)<=0;
    }
};

#endif