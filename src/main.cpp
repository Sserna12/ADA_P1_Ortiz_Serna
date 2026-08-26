#include "bt_backtracking.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>


static void mostrarMetricas(
    const std::string& titulo,
    const MetricasBT& metricas
) {
    std::cout << "\n" << titulo << "\n";

    std::cout
        << "Nodos visitados: "
        << metricas.nodosVisitados
        << "\n";

    std::cout
        << "Nodos podados: "
        << metricas.nodosPodados
        << "\n";

    std::cout
        << "Soluciones encontradas: "
        << metricas.soluciones
        << "\n";

    std::cout
        << "Tiempo: "
        << metricas.tiempoMs
        << " ms\n";

    std::cout
        << "Estado: "
        << (
            metricas.interrumpido
            ? "INTERRUMPIDO POR LIMITE"
            : "COMPLETADO"
        )
        << "\n";
}


static std::uint64_t nodosArbolExhaustivo(
    std::uint64_t tamAlfabeto,
    int n
) {
    std::uint64_t total = 1;
    std::uint64_t potencia = 1;

    for (int k = 1; k <= n; ++k) {
        if (
            potencia >
            std::numeric_limits<std::uint64_t>::max()
            / tamAlfabeto
        ) {
            return 0;
        }

        potencia *= tamAlfabeto;

        if (
            total >
            std::numeric_limits<std::uint64_t>::max()
            - potencia
        ) {
            return 0;
        }

        total += potencia;
    }

    return total;
}


static double calcularReduccion(
    std::uint64_t nodosSinPoda,
    std::uint64_t nodosConPoda
) {
    if (nodosSinPoda == 0) {
        return 0.0;
    }

    return
        (
            static_cast<double>(
                nodosSinPoda - nodosConPoda
            )
            /
            static_cast<double>(
                nodosSinPoda
            )
        ) * 100.0;
}


int main(int argc, char* argv[]) {
    const std::string alfabetoCompleto =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%";

    const std::string alfabetoPrueba =
        "abAB12!";


    if (argc < 2 || argc > 3) {
        std::cout
            << "Uso:\n\n"
            << "./ada_p1 prueba\n"
            << "./ada_p1 referencia [max_nodos]\n"
            << "./ada_p1 equipo_n6 [max_nodos]\n"
            << "./ada_p1 equipo_n8 [max_nodos]\n"
            << "./ada_p1 equipo_n10 [max_nodos]\n"
            << "./ada_p1 relajada_n8 [max_nodos]\n"
            << "./ada_p1 sin_restricciones_n6 [max_nodos]\n";

        return 0;
    }


    std::string opcion = argv[1];

    LimitesBT limites;

    if (argc == 3) {
        try {
            limites.maxNodos =
                std::stoull(argv[2]);
        } catch (...) {
            std::cerr
                << "El limite de nodos no es valido.\n";
            return 1;
        }
    }


    Politica politica;
    std::string alfabeto;


    if (opcion == "prueba") {
        politica = {
            4,
            1,
            1,
            1,
            1,
            true
        };

        alfabeto = alfabetoPrueba;

    } else if (opcion == "referencia") {
        politica = {
            6,
            2,
            1,
            1,
            1,
            true
        };

        alfabeto = alfabetoCompleto;

    } else if (opcion == "equipo_n6") {
        politica = {
            6,
            3,
            2,
            2,
            1,
            true
        };

        alfabeto = alfabetoCompleto;

    } else if (opcion == "equipo_n8") {
        politica = {
            8,
            3,
            2,
            2,
            1,
            true
        };

        alfabeto = alfabetoCompleto;

    } else if (opcion == "equipo_n10") {
        politica = {
            10,
            3,
            2,
            2,
            1,
            true
        };

        alfabeto = alfabetoCompleto;

    } else if (opcion == "relajada_n8") {
        politica = {
            8,
            1,
            0,
            0,
            0,
            true
        };

        alfabeto = alfabetoCompleto;

    } else if (
        opcion == "sin_restricciones_n6"
    ) {
        politica = {
            6,
            0,
            0,
            0,
            0,
            false
        };

        alfabeto = alfabetoCompleto;

    } else {
        std::cerr
            << "Opcion no valida.\n";

        return 1;
    }


    std::cout
        << std::fixed
        << std::setprecision(3);


    std::cout
        << "\n====================================\n"
        << opcion
        << "\n====================================\n";

    std::cout
        << "Longitud n: "
        << politica.n
        << "\n";

    std::cout
        << "Tamano del alfabeto: "
        << alfabeto.size()
        << "\n";


    std::uint64_t nodosTeoricos =
        nodosArbolExhaustivo(
            alfabeto.size(),
            politica.n
        );

    if (nodosTeoricos > 0) {
        std::cout
            << "Nodos teoricos sin poda: "
            << nodosTeoricos
            << "\n";
    } else {
        std::cout
            << "Nodos teoricos sin poda: "
            << "fuera de rango\n";
    }


    if (limites.maxNodos == 0) {
        std::cout
            << "Limite de nodos: sin limite\n";
    } else {
        std::cout
            << "Limite de nodos: "
            << limites.maxNodos
            << "\n";
    }


    MetricasBT conPoda =
        ejecutarConPoda(
            politica,
            alfabeto,
            limites
        );

    mostrarMetricas(
        "=== CON PODA ===",
        conPoda
    );


    if (
        !conPoda.interrumpido &&
        nodosTeoricos > 0
    ) {
        double reduccion =
            calcularReduccion(
                nodosTeoricos,
                conPoda.nodosVisitados
            );

        std::cout
            << "\nReduccion teorica del espacio: "
            << reduccion
            << "%\n";
    } else {
        std::cout
            << "\nReduccion final del espacio: "
            << "NO CALCULADA\n";

        std::cout
            << "Motivo: la ejecucion con poda "
            << "no termino completamente.\n";
    }


    if (opcion == "prueba") {
        MetricasBT sinPoda =
            ejecutarSinPoda(
                politica,
                alfabeto,
                limites
            );

        mostrarMetricas(
            "=== SIN PODA ===",
            sinPoda
        );

        if (
            !conPoda.interrumpido &&
            !sinPoda.interrumpido
        ) {
            double reduccionReal =
                calcularReduccion(
                    sinPoda.nodosVisitados,
                    conPoda.nodosVisitados
                );

            std::cout
                << "\nReduccion medida: "
                << reduccionReal
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
        }

        return 0;
    }


    std::cout
        << "\nLa version sin poda no se ejecuta "
        << "automaticamente en instancias reales.\n";

    std::cout
        << "El numero teorico de nodos sin poda "
        << "se muestra arriba para la comparacion.\n";


    return 0;
}
