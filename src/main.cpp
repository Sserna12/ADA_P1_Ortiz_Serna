#include "bt_backtracking.hpp"

#include <iomanip>
#include <iostream>
#include <string>

int main() {
    const std::string alfabetoPrueba = "abAB12!";

    Politica politica;
    politica.n = 4;
    politica.minLower = 1;
    politica.minUpper = 1;
    politica.minDigit = 1;
    politica.minSymbol = 1;
    politica.sinRepetidosConsecutivos = true;

    MetricasBT conPoda =
        ejecutarConPoda(politica, alfabetoPrueba);

    MetricasBT sinPoda =
        ejecutarSinPoda(politica, alfabetoPrueba);

    double reduccion = 0.0;

    if (sinPoda.nodosVisitados > 0) {
        reduccion =
            (static_cast<double>(
                sinPoda.nodosVisitados -
                conPoda.nodosVisitados
            ) /
            static_cast<double>(
                sinPoda.nodosVisitados
            )) * 100.0;
    }

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "=== PRUEBA DE BACKTRACKING ===\n\n";

    std::cout << "Con poda:\n";
    std::cout << "Nodos visitados: "
              << conPoda.nodosVisitados << "\n";
    std::cout << "Nodos podados: "
              << conPoda.nodosPodados << "\n";
    std::cout << "Soluciones: "
              << conPoda.soluciones << "\n";
    std::cout << "Tiempo: "
              << conPoda.tiempoMs << " ms\n\n";

    std::cout << "Sin poda:\n";
    std::cout << "Nodos visitados: "
              << sinPoda.nodosVisitados << "\n";
    std::cout << "Soluciones: "
              << sinPoda.soluciones << "\n";
    std::cout << "Tiempo: "
              << sinPoda.tiempoMs << " ms\n\n";

    std::cout << "Reduccion de nodos: "
              << reduccion << "%\n";

    if (conPoda.soluciones == sinPoda.soluciones) {
        std::cout
            << "Verificacion: ambas versiones coinciden.\n";
    } else {
        std::cout
            << "ERROR: las versiones no coinciden.\n";
        return 1;
    }

    return 0;
}