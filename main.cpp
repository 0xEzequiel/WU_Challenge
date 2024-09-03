#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "user.h" 
#include "topic.h"
#include "alert.h"
using namespace std;

// Vector dinamico para almacenar usuarios
vector<User> usuarios;

// Vector dinamico para almacenar los temas de interes
vector<Topic> temas;

// Vector dinámico para almacenar las alertas
vector<Alerta> alertas;

// Función para registrar un usuario
// Opcion 1
void registrarUsuario() {
    int id;
    string name;

    cout << "Ingrese el ID de usuario: ";
    cin >> id;
    cout << "Ingrese el nombre del usuario: ";
    cin >> name;

    User newUser(id, name);
    usuarios.push_back(newUser);
    cout << "Usuario registrado exitosamente." << endl;
}

// Función para registrar un tema
// Opcion 2
void registrarTopic() {
    int id;
    string name;

    cout << "Ingrese el ID del tema: ";
    cin >> id;
    cout << "Ingrese el nombre del tema: ";
    cin >> name;

    Topic newTopic(id, name);
    temas.push_back(newTopic);
    cout << "Tema registrado!."<< endl;
}

// Asignar tema de interes
// Opcion 3
void assignTopicToUser() {
    int userID;
    int temaID;
    cout<<"Ingrese el ID del tema: ";
    cin>>temaID;
    cout<<"Ingrese el ID del usuario: ";
    cin>>userID;

    // Buscar al usuario por su id
    auto it = find_if(usuarios.begin(), usuarios.end(), [userID](const User& user) { return user.id == userID; });

    if (it != usuarios.end()) { // Si el user existe
        // Agrego el tema a sus intereses
        it -> temasDeInteres.insert(temaID);
        cout << "Tema asignado al usuario!" << endl;
    } else { // ...sino, aviso que no fue encontrado
        cout << "Usuario no encontrado" << endl;
    }
}

// Función para enviar una alerta a todos los usuarios interesados en un tema
// Opcion 4
void enviarAlerta() {
    int temaID;
    int tipo;
    string mensaje;
    time_t fechaExpiracion;

    cout << "Ingrese el ID del tema: ";
    cin >> temaID;
    cout << "Ingrese el tipo de alerta (0 para Informativa, 1 para Urgente): ";
    cin >> tipo;
    cout << "Ingrese el mensaje de la alerta: ";
    
    cin.ignore(); // Ignorar el salto de línea

    getline(cin, mensaje);
    cout << "Ingrese la fecha de expiracion (segundos desde epoch): ";
    /* Esto fue una elección de diseño para simplificar la tarea, pero soy consciente de 
    lo poco user-friendly que resulta. Por supuesto que se puede implementar de otra manera, se
    podria crear una clase time y manejar la fecha de expiracion usando como solo los datos como 
    dia, mes y año. Elegí usar un tipo time_t para aprovechar la biblioteca ctime. */
    cin >> fechaExpiracion;

    static int ID_alerta = 1; // id único para cada alerta

    // Crear alerta
    Alerta nuevaAlerta(ID_alerta++, temaID, static_cast<tipoAlerta>(tipo), fechaExpiracion, mensaje); // casteo!

    // Me guardo la alerta
    alertas.push_back(nuevaAlerta);

    // Envio la alerta a todos los usuarios interesados en este topic de interes
    for (auto& user: usuarios) {
        if (user.temasDeInteres.count(temaID) > 0) {
            user.recibirAlerta(nuevaAlerta);  // Enviar la alerta al usuario
        }
    }

    cout << "Alerta enviada a todos los usuarios interesados" << endl;
}

// Función para marcar una alerta como leída
// Opción 8 
// Obs: decidí que sea opcion 8 y no 5 para que quede lo mas 1 a 1 posible con la consigna.
void marcarAlertaComoLeida() {
    int userId, alertaId;

    cout << "Ingrese el ID del usuario: ";
    cin >> userId;
    cout << "Ingrese el ID de la alerta: ";
    cin >> alertaId;

    // Buscar al usuario por su id
    auto it = find_if(usuarios.begin(), usuarios.end(), [userId](const User& user) { 
        return user.id == userId; 
    });

    if (it != usuarios.end()) {
        it -> marcarAlertaComoLeida(alertaId);  // Marcar la alerta como leída
    } else {
        cout<< "Usuario no encontrado." << endl;
    }
}

/* Observacion: no quisiera dejar pasar que en un contexto de ingeniería de software 
probablemente hubiera sido una mejor práctica usar un stack/pila para las alertas urgentes
y una queue/cola para las informativas. 
Esto hubiera requerido la implementación de dichos módulos, aunque probablemente sea overkill para efectos del challenge. */

// Función para obtener alertas no leídas y no expiradas de un usuario
void mostrarAlertasNoLeidasNoExpiradas() {
    int userId;
    cout << "Ingrese el ID del usuario: ";
    cin >> userId;

    auto it = find_if(usuarios.begin(),usuarios.end(),[userId](const User& user) { 
        return user.id == userId; 
    });

    if (it != usuarios.end()) {
        vector<Alerta> alertasNoLeidas = it -> obtenerAlertasNoLeidasNoExpiradas();

        // Ordenar alertas
        sort(alertasNoLeidas.begin(), alertasNoLeidas.end(), [](const Alerta& a1, const Alerta& a2) {
            if (a1.tipo != a2.tipo)
                return a1.tipo > a2.tipo; // Urgente primero (mayor valor)
            return a1.id > a2.id; // LIFO para Urgentes y FIFO para Informativas
        });

        // Mostrar alertas
        for (const auto& alerta : alertasNoLeidas) {
            cout << "ID: " << alerta.id << ", Mensaje: " << alerta.mensaje << ", Tipo: " << (alerta.tipo == tipoAlerta::Urgente ? "Urgente" : "Informativa") << endl;
            // Re-hecho usando logica ternaria
        }
    } else {
        cout << "Usuario no encontrado." << endl;
    }
}

// Funcion para obtener alertas no expiradas para un tema
void mostrarAlertasNoExpiradasPorTema() {
    int topicId;
    cout << "Ingrese el ID del tema: ";
    cin >> topicId;

    vector<Alerta> alertasNoExpiradasPorTema;
    for (const auto& alerta : alertas) {
        if (alerta.topic_ID == topicId && !alerta.alertaExpirada()) {
            alertasNoExpiradasPorTema.push_back(alerta);
        }
    }

    // Ordeno las alertas. 
    // Urgentes <- LIFO. Info <- FIFO.
    sort(alertasNoExpiradasPorTema.begin(), alertasNoExpiradasPorTema.end(), [](const Alerta& a1, const Alerta& a2) {
        if (a1.tipo != a2.tipo)
            return a1.tipo > a2.tipo; // urgente primero (mayor valor)
        return a1.id > a2.id; // Urgentes <- LIFO. Info <- FIFO.
    });

    // Mostrar alertas
    for (const auto& alerta: alertasNoExpiradasPorTema) {
        cout << "ID: " << alerta.id << ", Mensaje: " << alerta.mensaje << ", Tipo: " << (alerta.tipo == tipoAlerta::Urgente ? "Urgente" : "Informativa") 
             << ", Destinatario: " << (alerta.user_ID == -1 ? "Todos los usuarios" : "Usuario ID " + to_string(alerta.user_ID)) << endl;
    }
}

int main() {
    int opcion;

    do {
        cout << "Menú:" << endl;
        cout << "1. Registrar usuario" << endl;
        cout << "2. Registrar tema de interés" << endl;
        cout << "3. Asignar tema de interés a usuario" << endl;
        cout << "4. Enviar alerta sobre un tema" << endl;
        cout << "8. Marcar alerta como leída" << endl;
        cout << "9. Mostrar alertas no leídas y no expiradas de un usuario" << endl;
        cout << "10. Mostrar alertas no expiradas para un tema" << endl;
        cout << "404. Salir" << endl;
        cout << "Ingrese una opción: ";
        
        cin >> opcion;
        switch (opcion) {
            case 1:
                registrarUsuario();
                break;
            case 2: 
                registrarTopic();
                break;
            case 3: 
                assignTopicToUser();
            case 4:
                enviarAlerta();
                break;
            case 8:
                marcarAlertaComoLeida();
                break;
            case 9:
                mostrarAlertasNoLeidasNoExpiradas();
                break;
            case 10:
                mostrarAlertasNoExpiradasPorTema();
                break;
            case 404:
                cout << "Bye bye :)" << endl;
                break;
            default:
                cout << "Opción inválida." << endl;
        }

    } while (opcion != 404);

    return 0;
}