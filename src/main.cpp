#include "bt_backtracking.hpp"

#include <iomanip>
#include <iostream>
#include <string>

void mostrarMetricas(
    const std::string& nombre,
    const MetricasBT& metricas
) {
    std::cout << nombre << ":\n";
    std::cout << "  Nodos visitados: "
              << metricas.nodosVisitados << "\n";
    std::cout << "  Nodos podados: "
              << metricas.nodosPodados << "\n";
    std::cout << "  Soluciones: "
              << metricas.soluciones << "\n";
    std::cout << "  Tiempo: "
              << metricas.tiempoMs << " ms\n";
}

int main(int argc, char* argv[]) {
    const std::string alfabetoCompleto =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%";

    const std::string alfabetoPrueba =
        "abAB12!";

    if (argc != 2) {
        std::cout
            << "Uso:\n"
            << "  ./ada_p1 prueba\n"
            << "  ./ada_p1 referencia\n"
            << "  ./ada_p1 equipo_n8\n"
            << "  ./ada_p1 equipo_n6\n"
            << "  ./ada_p1 equipo_n10\n"
            << "  ./ada_p1 relajada_n8\n"
            << "  ./ada_p1 sin_restricciones_n6\n";

        return 0;
    }

    std::string opcion = argv[1];

    Politica politica;
    std::string alfabeto;

    if (opcion == "prueba") {
        politica = {4, 1, 1, 1, 1, true};
        alfabeto = alfabetoPrueba;

    } else if (opcion == "referencia") {
        politica = {6, 2, 1, 1, 1, true};
        alfabeto = alfabetoCompleto;

    } else if (opcion == "equipo_n8") {
        politica = {8, 3, 2, 2, 1, true};
        alfabeto = alfabetoCompleto;

    } else if (opcion == "equipo_n6") {
        politica = {6, 3, 2, 2, 1, true};
        alfabeto = alfabetoCompleto;

    } else if (opcion == "equipo_n10") {
        politica = {10, 3, 2, 2, 1, true};
        alfabeto = alfabetoCompleto;

    } else if (opcion == "relajada_n8") {
        politica = {8, 1, 0, 0, 0, true};
        alfabeto = alfabetoCompleto;

    } else if (opcion == "sin_restricciones_n6") {
        politica = {6, 0, 0, 0, 0, false};
        alfabeto = alfabetoCompleto;

    } else {
        std::cerr << "Opcion no valida.\n";
        return 1;
    }

    std::cout << std::fixed << std::setprecision(3);

    std::cout
        << "\n====================================\n"
        << opcion
        << "\n====================================\n";

    MetricasBT conPoda =
        ejecutarConPoda(
            politica,
            alfabeto
        );

    mostrarMetricas(
        "Con poda",
        conPoda
    );

    if (opcion == "prueba") {
        std::cout << "\n";

        MetricasBT sinPoda =
            ejecutarSinPoda(
                politica,
                alfabeto
            );

        mostrarMetricas(
            "Sin poda",
            sinPoda
        );

        double reduccion = 0.0;

        if (sinPoda.nodosVisitados > 0) {
            reduccion =
                (
                    static_cast<double>(
                        sinPoda.nodosVisitados -
                        conPoda.nodosVisitados
                    )
                    /
                    static_cast<double>(
                        sinPoda.nodosVisitados
                    )
                ) * 100.0;
        }

        std::cout
            << "\nReduccion de nodos: "
            << reduccion
            << "%\n";

        if (
            conPoda.soluciones ==
            sinPoda.soluciones
        ) {
            std::cout
                << "Verificacion: ambas versiones coinciden.\n";
        } else {
            std::cout
                << "ERROR: las versiones no coinciden.\n";

            return 1;
        }

    } else {
        std::cout
            << "\nLa version sin poda no se ejecuta "
            << "automaticamente para esta instancia.\n";
    }

    return 0;
}