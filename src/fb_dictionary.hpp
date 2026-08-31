#pragma once
#include <string>

namespace fb {

struct ResultadoDiccionario {
    bool encontrada = false;
    bool errorLectura = false;
    std::string password;
    unsigned long long candidatosEvaluados = 0;
    double tiempoMs = 0.0;
};

ResultadoDiccionario buscarPorDiccionario(const std::string& hashObjetivoHex, const std::string& rutaDiccionario);

}
