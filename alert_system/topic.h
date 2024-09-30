#ifndef TOPIC_H
#define TOPIC_H

#include <string>
using namespace std;

class Topic {
public:
    // Propiedades de la clase
    int id;
    string nombre;

    // Constructor
    Topic(int id, const string& name) : id(id), nombre(name) {}
};

#endif