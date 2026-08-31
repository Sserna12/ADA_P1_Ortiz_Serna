#include "fb_core.hpp"
#include "third_party/picosha2.h"

#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

namespace fb {

const Alfabeto ALFABETO_A1{"A1", "abcdefghijklmnopqrstuvwxyz"};
const Alfabeto ALFABETO_A2{"A2", "abcdefghijklmnopqrstuvwxyz0123456789"};

const Alfabeto& alfabetoPorNombre(const string& nombre) {
    string n = nombre;
    for (int i = 0; i < n.size(); i++) {
        n[i] = tolower(n[i]);
    }

    if (n == "a1") return ALFABETO_A1;
    if (n == "a2") return ALFABETO_A2;

    cout << "Alfabeto desconocido: '" << nombre << "' (use a1 o a2)" << endl;
    exit(1);
}

string sha256Hex(const string& texto) {
    return picosha2::hash256_hex_string(texto);
}

static string aMinusculas(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

ResultadoBusqueda buscarPorFuerzaBrutaLongitudFija(const string& hashObjetivoHex, const Alfabeto& alfabeto, int n) {
    ResultadoBusqueda r;
    string objetivo = aMinusculas(hashObjetivoHex);
    int base = alfabeto.simbolos.size();

    auto inicio = steady_clock::now();

    if (n == 0) {
        r.candidatosEvaluados = 1;
        if (sha256Hex("") == objetivo) {
            r.encontrada = true;
            r.password = "";
            r.longitudEncontrada = 0;
        }
    } else {
        vector<int> indices(n, 0);
        string candidato(n, alfabeto.simbolos[0]);
        bool agotado = false;

        while (!agotado) {
            for (int i = 0; i < n; i++) {
                candidato[i] = alfabeto.simbolos[indices[i]];
            }
            r.candidatosEvaluados = r.candidatosEvaluados + 1;

            if (sha256Hex(candidato) == objetivo) {
                r.encontrada = true;
                r.password = candidato;
                r.longitudEncontrada = n;
                break;
            }

            int pos = n - 1;
            while (pos >= 0) {
                indices[pos] = indices[pos] + 1;
                if (indices[pos] < base) {
                    break;
                }
                indices[pos] = 0;
                pos = pos - 1;
            }
            if (pos < 0) {
                agotado = true;
            }
        }
    }

    auto fin = steady_clock::now();
    r.tiempoMs = duration<double, milli>(fin - inicio).count();
    return r;
}

ResultadoBusqueda buscarPorFuerzaBruta(const string& hashObjetivoHex, const Alfabeto& alfabeto, int longitudMin, int longitudMax) {
    ResultadoBusqueda total;
    auto inicio = steady_clock::now();

    for (int n = longitudMin; n <= longitudMax; n++) {
        ResultadoBusqueda parcial = buscarPorFuerzaBrutaLongitudFija(hashObjetivoHex, alfabeto, n);
        total.candidatosEvaluados = total.candidatosEvaluados + parcial.candidatosEvaluados;
        if (parcial.encontrada) {
            total.encontrada = true;
            total.password = parcial.password;
            total.longitudEncontrada = parcial.longitudEncontrada;
            break;
        }
    }

    auto fin = steady_clock::now();
    total.tiempoMs = duration<double, milli>(fin - inicio).count();
    return total;
}

}
