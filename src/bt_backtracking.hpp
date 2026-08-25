#ifndef BT_BACKTRACKING_HPP
#define BT_BACKTRACKING_HPP

#include <string>
#include <cstdint>

struct Politica {
    int n;

    int minLower;
    int minUpper;
    int minDigit;
    int minSymbol;

    bool sinRepetidosConsecutivos;
};

struct MetricasBT {
    std::uint64_t nodosVisitados = 0;
    std::uint64_t nodosPodados = 0;
    std::uint64_t soluciones = 0;

    double tiempoMs = 0.0;
};

struct EstadoBT {
    std::string prefijo;

    int lower = 0;
    int upper = 0;
    int digit = 0;
    int symbol = 0;
};

bool esMinuscula(char c);
bool esMayuscula(char c);
bool esDigito(char c);
bool esSimbolo(char c);

bool estadoFactible(
    const EstadoBT& estado,
    const Politica& politica
);

bool solucionValida(
    const EstadoBT& estado,
    const Politica& politica
);

#endif