#include <gtest/gtest.h>
#include "../user.h"
#include "../alert.h"
#include "../topic.h"
using namespace std;

TEST(UserTests, registrarUsuario) {
    User user(1, "Ezequiel");
    ASSERT_EQ(user.id, 1);
    ASSERT_EQ(user.nombre, "Ezequiel");
}

TEST(UserTests, marcarAlertaComoLeida) {
    User user(1, "Bill Evans");
    Alerta alerta(1, 1, tipoAlerta::Urgente, time(0) + 3600, "Escribile algo a Debby!");
    user.recibirAlerta(alerta);
    user.marcarAlertaComoLeida(1);
    ASSERT_EQ(user.alertasRecibidas[0].leida, true);
}

TEST(TopicTests, registrarTema) {
    Topic topic(1, "Jazz");
    ASSERT_EQ(topic.id, 1);
    ASSERT_EQ(topic.nombre, "Jazz");
}

TEST(AlertTests, enviarAlerta) {
    User user(1, "Herbie");
    Topic topic(1, "from: Miles");
    Alerta alerta(1, 1, tipoAlerta::Urgente, time(0) + 3600, "Don't play the butter notes!");
    user.add_temaDeInteres(1);
    user.recibirAlerta(alerta);
    ASSERT_EQ(user.alertasRecibidas.size(), 1);
    ASSERT_EQ(user.alertasRecibidas[0].id, 1);
}

TEST(AlertTests, getAlertasNoExp) {
    User user(1, "Ezequiel");
    Topic topic(1, "CompSci");
    Alerta alerta(1,1,tipoAlerta::Urgente, time(0) + 3600, "Debuggear!");
    user.add_temaDeInteres(1);
    user.recibirAlerta(alerta);
    auto alertasNoLeidas = user.obtenerAlertasNoLeidasNoExpiradas();
    ASSERT_EQ(alertasNoLeidas.size(), 1);
    ASSERT_EQ(alertasNoLeidas[0].id, 1);
}

TEST(FunctionalTests, getAlertasNoExpPorUsuario) {
    User user(1, "Chick");
    Topic topic(1, "Comida coreana");
    Alerta alerta1(1, 1, tipoAlerta::Urgente, time(0) + 3600, "Alerta vigente");
    Alerta alerta2(2, 1, tipoAlerta::Urgente, time(0) - 3600, "Alerta expirada"); 

    user.add_temaDeInteres(1);
    user.recibirAlerta(alerta1);
    user.recibirAlerta(alerta2);

    vector<Alerta> alertasNoLeidasNoExpiradas = user.obtenerAlertasNoLeidasNoExpiradas();
    ASSERT_EQ(alertasNoLeidasNoExpiradas.size(), 1);
    ASSERT_EQ(alertasNoLeidasNoExpiradas[0].id, 1);
}

TEST(FunctionalTests, getAlertasNoExpPorTema) {
    User user(1, "Chet");
    Topic topic(1, "Bakery");
    Alerta alerta1(1, 1, tipoAlerta::Urgente, time(0) + 3600, "Alerta vigente");
    Alerta alerta2(2, 1, tipoAlerta::Urgente, time(0) - 3600, "Alerta expirada");

    user.add_temaDeInteres(1);
    user.recibirAlerta(alerta1);
    user.recibirAlerta(alerta2);

    vector<Alerta> alertasNoExpiradasPorTema;
    time_t ahora = time(0);
    for (const auto& alerta : user.alertasRecibidas) {
        if (alerta.topic_ID == 1 && !alerta.alertaExpirada()) {
            alertasNoExpiradasPorTema.push_back(alerta);
        }
    }

    ASSERT_EQ(alertasNoExpiradasPorTema.size(), 1);
    ASSERT_EQ(alertasNoExpiradasPorTema[0].id, 1);
}

// Correr tests :)
int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}