#pragma once
#include <string>

namespace fb {

struct Alfabeto {
    std::string nombre;
    std::string simbolos;
};

extern const Alfabeto ALFABETO_A1;
extern const Alfabeto ALFABETO_A2;

const Alfabeto& alfabetoPorNombre(const std::string& nombre);

std::string sha256Hex(const std::string& texto);

struct ResultadoBusqueda {
    bool encontrada = false;
    std::string password;
    unsigned long long candidatosEvaluados = 0;
    double tiempoMs = 0.0;
    int longitudEncontrada = -1;
};

ResultadoBusqueda buscarPorFuerzaBrutaLongitudFija(const std::string& hashObjetivoHex, const Alfabeto& alfabeto, int n);

ResultadoBusqueda buscarPorFuerzaBruta(const std::string& hashObjetivoHex, const Alfabeto& alfabeto, int longitudMin, int longitudMax);

}
