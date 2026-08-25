#ifndef BT_BACKTRACKING_HPP
#define BT_BACKTRACKING_HPP

#include <cstdint>
#include <string>

struct Politica {
    int n = 0;

    int minLower = 0;
    int minUpper = 0;
    int minDigit = 0;
    int minSymbol = 0;

    bool sinRepetidosConsecutivos = false;
};

struct EstadoBT {
    std::string prefijo;

    int lower = 0;
    int upper = 0;
    int digit = 0;
    int symbol = 0;
};

struct MetricasBT {
    std::uint64_t nodosVisitados = 0;
    std::uint64_t nodosPodados = 0;
    std::uint64_t soluciones = 0;

    double tiempoMs = 0.0;

    bool interrumpido = false;
};

struct LimitesBT {
    // 0 significa sin limite.
    std::uint64_t maxNodos = 0;
};

bool esMinuscula(char c);
bool esMayuscula(char c);
bool esDigito(char c);
bool esSimbolo(char c);

bool solucionValida(
    const EstadoBT& estado,
    const Politica& politica
);

bool estadoFactible(
    const EstadoBT& estado,
    const Politica& politica
);

void backtrackingConPoda(
    EstadoBT& estado,
    const Politica& politica,
    const std::string& alfabeto,
    MetricasBT& metricas,
    const LimitesBT& limites
);

void exploracionSinPoda(
    EstadoBT& estado,
    const Politica& politica,
    const std::string& alfabeto,
    MetricasBT& metricas,
    const LimitesBT& limites
);

MetricasBT ejecutarConPoda(
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites = {}
);

MetricasBT ejecutarSinPoda(
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites = {}
);

#endif
