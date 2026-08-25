#include "bt_backtracking.hpp"

#include <algorithm>
#include <chrono>


// =========================================================
// CLASIFICACION DE CARACTERES
// =========================================================

bool esMinuscula(char c) {
    return c >= 'a' && c <= 'z';
}

bool esMayuscula(char c) {
    return c >= 'A' && c <= 'Z';
}

bool esDigito(char c) {
    return c >= '0' && c <= '9';
}

bool esSimbolo(char c) {
    return c == '!' ||
           c == '@' ||
           c == '#' ||
           c == '$' ||
           c == '%';
}


// =========================================================
// MANEJO DEL ESTADO
// =========================================================

static void agregarCaracter(
    EstadoBT& estado,
    char c
) {
    estado.prefijo.push_back(c);

    if (esMinuscula(c)) {
        estado.lower++;
    } else if (esMayuscula(c)) {
        estado.upper++;
    } else if (esDigito(c)) {
        estado.digit++;
    } else if (esSimbolo(c)) {
        estado.symbol++;
    }
}


static void quitarCaracter(
    EstadoBT& estado,
    char c
) {
    if (esMinuscula(c)) {
        estado.lower--;
    } else if (esMayuscula(c)) {
        estado.upper--;
    } else if (esDigito(c)) {
        estado.digit--;
    } else if (esSimbolo(c)) {
        estado.symbol--;
    }

    estado.prefijo.pop_back();
}


// =========================================================
// CONTROL DEL LIMITE DE EXPLORACION
// =========================================================

static bool limiteAlcanzado(
    const MetricasBT& metricas,
    const LimitesBT& limites
) {
    return limites.maxNodos > 0 &&
           metricas.nodosVisitados >= limites.maxNodos;
}


// =========================================================
// VALIDACION DE SOLUCION COMPLETA
// =========================================================

bool solucionValida(
    const EstadoBT& estado,
    const Politica& politica
) {
    if (
        static_cast<int>(estado.prefijo.size())
        != politica.n
    ) {
        return false;
    }

    if (estado.lower < politica.minLower) {
        return false;
    }

    if (estado.upper < politica.minUpper) {
        return false;
    }

    if (estado.digit < politica.minDigit) {
        return false;
    }

    if (estado.symbol < politica.minSymbol) {
        return false;
    }

    if (politica.sinRepetidosConsecutivos) {
        for (
            std::size_t i = 1;
            i < estado.prefijo.size();
            ++i
        ) {
            if (
                estado.prefijo[i]
                == estado.prefijo[i - 1]
            ) {
                return false;
            }
        }
    }

    return true;
}


// =========================================================
// FACTIBILIDAD DEL ESTADO PARCIAL
// =========================================================

bool estadoFactible(
    const EstadoBT& estado,
    const Politica& politica
) {
    int longitudActual =
        static_cast<int>(estado.prefijo.size());

    if (longitudActual > politica.n) {
        return false;
    }

    int restantes =
        politica.n - longitudActual;

    int faltanLower =
        std::max(
            0,
            politica.minLower - estado.lower
        );

    int faltanUpper =
        std::max(
            0,
            politica.minUpper - estado.upper
        );

    int faltanDigit =
        std::max(
            0,
            politica.minDigit - estado.digit
        );

    int faltanSymbol =
        std::max(
            0,
            politica.minSymbol - estado.symbol
        );

    int faltantes =
        faltanLower +
        faltanUpper +
        faltanDigit +
        faltanSymbol;

    return faltantes <= restantes;
}


// =========================================================
// BACKTRACKING CON PODA
// =========================================================

void backtrackingConPoda(
    EstadoBT& estado,
    const Politica& politica,
    const std::string& alfabeto,
    MetricasBT& metricas,
    const LimitesBT& limites
) {
    if (limiteAlcanzado(metricas, limites)) {
        metricas.interrumpido = true;
        return;
    }

    metricas.nodosVisitados++;

    // Si el propio estado actual ya es infactible,
    // no tiene sentido generar ninguno de sus hijos.
    if (!estadoFactible(estado, politica)) {
        metricas.nodosPodados++;
        return;
    }

    // Caso base: cadena completa.
    if (
        static_cast<int>(estado.prefijo.size())
        == politica.n
    ) {
        if (solucionValida(estado, politica)) {
            metricas.soluciones++;
        }

        return;
    }

    for (char c : alfabeto) {
        if (metricas.interrumpido) {
            return;
        }

        // Poda local:
        // no permitir caracteres identicos consecutivos.
        if (
            politica.sinRepetidosConsecutivos &&
            !estado.prefijo.empty() &&
            estado.prefijo.back() == c
        ) {
            metricas.nodosPodados++;
            continue;
        }

        agregarCaracter(estado, c);

        if (estadoFactible(estado, politica)) {
            backtrackingConPoda(
                estado,
                politica,
                alfabeto,
                metricas,
                limites
            );
        } else {
            metricas.nodosPodados++;
        }

        quitarCaracter(estado, c);
    }
}


// =========================================================
// EXPLORACION EXHAUSTIVA SIN PODA
// =========================================================

void exploracionSinPoda(
    EstadoBT& estado,
    const Politica& politica,
    const std::string& alfabeto,
    MetricasBT& metricas,
    const LimitesBT& limites
) {
    if (limiteAlcanzado(metricas, limites)) {
        metricas.interrumpido = true;
        return;
    }

    metricas.nodosVisitados++;

    if (
        static_cast<int>(estado.prefijo.size())
        == politica.n
    ) {
        if (solucionValida(estado, politica)) {
            metricas.soluciones++;
        }

        return;
    }

    for (char c : alfabeto) {
        if (metricas.interrumpido) {
            return;
        }

        agregarCaracter(estado, c);

        exploracionSinPoda(
            estado,
            politica,
            alfabeto,
            metricas,
            limites
        );

        quitarCaracter(estado, c);
    }
}


// =========================================================
// EJECUCION CON PODA
// =========================================================

MetricasBT ejecutarConPoda(
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites
) {
    EstadoBT estado;
    MetricasBT metricas;

    auto inicio =
        std::chrono::high_resolution_clock::now();

    backtrackingConPoda(
        estado,
        politica,
        alfabeto,
        metricas,
        limites
    );

    auto fin =
        std::chrono::high_resolution_clock::now();

    metricas.tiempoMs =
        std::chrono::duration<double, std::milli>(
            fin - inicio
        ).count();

    return metricas;
}


// =========================================================
// EJECUCION SIN PODA
// =========================================================

MetricasBT ejecutarSinPoda(
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites
) {
    EstadoBT estado;
    MetricasBT metricas;

    auto inicio =
        std::chrono::high_resolution_clock::now();

    exploracionSinPoda(
        estado,
        politica,
        alfabeto,
        metricas,
        limites
    );

    auto fin =
        std::chrono::high_resolution_clock::now();

    metricas.tiempoMs =
        std::chrono::duration<double, std::milli>(
            fin - inicio
        ).count();

    return metricas;
}
