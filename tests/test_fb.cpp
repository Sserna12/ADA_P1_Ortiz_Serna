#include "../src/fb_core.hpp"
#include "../src/fb_dictionary.hpp"
#include "../src/fb_instancias.hpp"

#include <iostream>
#include <vector>

using namespace std;

int main() {
    int fallos = 0;
    string hashAbc12 = "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";

    if (fb::sha256Hex("abc12") == hashAbc12) {
        cout << "FUNCIONA\n";
    } else {
        cout << "FALLO\n";
        fallos = fallos + 1;
    }

    {
        fb::ResultadoBusqueda r = fb::buscarPorFuerzaBrutaLongitudFija(hashAbc12, fb::ALFABETO_A2, 5);
        if (r.encontrada && r.password == "abc12") {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }
    }

    {
        fb::ResultadoBusqueda r = fb::buscarPorFuerzaBruta(hashAbc12, fb::ALFABETO_A2, 3, 5);
        if (r.encontrada && r.password == "abc12" && r.longitudEncontrada == 5) {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }
    }

    {
        string hashQueNoExiste(64, '0');
        fb::ResultadoBusqueda r = fb::buscarPorFuerzaBrutaLongitudFija(hashQueNoExiste, fb::ALFABETO_A1, 2);
        if (!r.encontrada && r.candidatosEvaluados == 26 * 26) {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }
    }

    {
        string hashPassword = fb::sha256Hex("password");
        fb::ResultadoDiccionario r = fb::buscarPorDiccionario(hashPassword, "resources/diccionario.txt");

        if (!r.errorLectura) {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }

        if (r.encontrada && r.password == "password") {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }
    }

    {
        string hashRaro = fb::sha256Hex("xkq7-noesta-en-la-lista-92");
        fb::ResultadoDiccionario r = fb::buscarPorDiccionario(hashRaro, "resources/diccionario.txt");
        if (!r.encontrada) {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }
    }

    {
        vector<string> apellidos1 = {"Ortiz", "Serna"};
        vector<string> apellidos2 = {"Serna", "Ortiz"};
        long long s1 = fb::calcularSemilla(apellidos1);
        long long s2 = fb::calcularSemilla(apellidos2);

        if (s1 == s2) {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }

        long long s3 = fb::calcularSemilla(apellidos1);
        if (s1 == s3) {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }
    }

    {
        vector<fb::InstanciaEquipo> instancias = fb::generarInstanciasEquipo(1105);
        int longitudesEsperadas[5] = {4, 4, 5, 5, 6};
        string alfabetosEsperados[5] = {"A1", "A2", "A1", "A2", "A1"};

        bool ok = true;
        if (instancias.size() != 5) {
            ok = false;
        }

        for (int i = 0; i < 5 && ok; i++) {
            if (instancias[i].longitud != longitudesEsperadas[i]) {
                ok = false;
            }
            if (instancias[i].alfabetoNombre != alfabetosEsperados[i]) {
                ok = false;
            }
            if ((int)instancias[i].password.size() != longitudesEsperadas[i]) {
                ok = false;
            }
            if (instancias[i].hashHex != fb::sha256Hex(instancias[i].password)) {
                ok = false;
            }
        }

        if (ok) {
            cout << "FUNCIONA\n";
        } else {
            cout << "FALLO\n";
            fallos = fallos + 1;
        }
    }

    cout << "\n";
    if (fallos == 0) {
        cout << "TODAS LAS PRUEBAS PASARON\n";
        return 0;
    } else {
        cout << "HAY PRUEBAS FALLIDAS (" << fallos << " fallo(s))\n";
        return 1;
    }
}
