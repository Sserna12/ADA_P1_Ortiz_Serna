#include "fb_dictionary.hpp"
#include "fb_core.hpp"

#include <cctype>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

namespace fb {

static string aMinusculas(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

ResultadoDiccionario buscarPorDiccionario(const string& hashObjetivoHex, const string& rutaDiccionario) {
    ResultadoDiccionario r;
    string objetivo = aMinusculas(hashObjetivoHex);

    ifstream archivo(rutaDiccionario);
    if (!archivo) {
        r.errorLectura = true;
        return r;
    }

    auto inicio = steady_clock::now();

    string linea;
    while (getline(archivo, linea)) {
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }
        if (linea.empty()) {
            continue;
        }

        r.candidatosEvaluados = r.candidatosEvaluados + 1;
        if (sha256Hex(linea) == objetivo) {
            r.encontrada = true;
            r.password = linea;
            break;
        }
    }

    auto fin = steady_clock::now();
    r.tiempoMs = duration<double, milli>(fin - inicio).count();
    return r;
}

}
