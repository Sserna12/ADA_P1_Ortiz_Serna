#include "bt_backtracking.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>


// =========================================================
// MOSTRAR METRICAS
// =========================================================

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


// =========================================================
// TAMANO TEORICO DEL ARBOL EXHAUSTIVO
// sumatoria desde k = 0 hasta n de |Sigma|^k
// =========================================================

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


// =========================================================
// PORCENTAJE DE REDUCCION
// =========================================================

static double calcularReduccion(
    std::uint64_t nodosSinPoda,
    std::uint64_t nodosConPoda
) {
    if (nodosSinPoda == 0) {
        return 0.0;
    }

    return (
        (
            static_cast<double>(nodosSinPoda)
            -
            static_cast<double>(nodosConPoda)
        )
        /
        static_cast<double>(nodosSinPoda)
    ) * 100.0;
}


// =========================================================
// CARGAR CONFIGURACION
// =========================================================

static bool cargarConfiguracion(
    const std::string& nombre,
    Politica& politica,
    std::string& alfabeto
) {
    const std::string alfabetoCompleto =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%";

    const std::string alfabetoPrueba =
        "abAB12!";


    if (nombre == "prueba") {
        politica = {
            4,
            1,
            1,
            1,
            1,
            true
        };

        alfabeto = alfabetoPrueba;
        return true;
    }


    if (nombre == "referencia") {
        politica = {
            6,
            2,
            1,
            1,
            1,
            true
        };

        alfabeto = alfabetoCompleto;
        return true;
    }


    if (nombre == "equipo_n6") {
        politica = {
            6,
            3,
            2,
            2,
            1,
            true
        };

        alfabeto = alfabetoCompleto;
        return true;
    }


    if (nombre == "equipo_n8") {
        politica = {
            8,
            3,
            2,
            2,
            1,
            true
        };

        alfabeto = alfabetoCompleto;
        return true;
    }


    if (nombre == "equipo_n10") {
        politica = {
            10,
            3,
            2,
            2,
            1,
            true
        };

        alfabeto = alfabetoCompleto;
        return true;
    }


    if (nombre == "relajada_n8") {
        politica = {
            8,
            1,
            0,
            0,
            0,
            true
        };

        alfabeto = alfabetoCompleto;
        return true;
    }


    if (nombre == "sin_restricciones_n6") {
        politica = {
            6,
            0,
            0,
            0,
            0,
            false
        };

        alfabeto = alfabetoCompleto;
        return true;
    }


    return false;
}


// =========================================================
// MOSTRAR DATOS DE LA INSTANCIA
// =========================================================

static std::uint64_t mostrarInstancia(
    const std::string& nombre,
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites
) {
    std::uint64_t nodosTeoricos =
        nodosArbolExhaustivo(
            alfabeto.size(),
            politica.n
        );


    std::cout
        << "\n====================================\n"
        << nombre
        << "\n====================================\n";

    std::cout
        << "Longitud n: "
        << politica.n
        << "\n";

    std::cout
        << "Tamano del alfabeto: "
        << alfabeto.size()
        << "\n";

    std::cout
        << "minLower: "
        << politica.minLower
        << "\n";

    std::cout
        << "minUpper: "
        << politica.minUpper
        << "\n";

    std::cout
        << "minDigit: "
        << politica.minDigit
        << "\n";

    std::cout
        << "minSymbol: "
        << politica.minSymbol
        << "\n";

    std::cout
        << "Sin repetidos consecutivos: "
        << (
            politica.sinRepetidosConsecutivos
            ? "si"
            : "no"
        )
        << "\n";


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


    return nodosTeoricos;
}


// =========================================================
// EJECUCION NORMAL: SOLO BACKTRACKING CON PODA
// =========================================================

static int ejecutarInstancia(
    const std::string& nombre,
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites
) {
    std::uint64_t nodosTeoricos =
        mostrarInstancia(
            nombre,
            politica,
            alfabeto,
            limites
        );


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

        if (conPoda.interrumpido) {
            std::cout
                << "Motivo: la ejecucion con poda "
                << "fue interrumpida por el limite.\n";
        }
    }


    return 0;
}


// =========================================================
// COMPARACION CON PODA VS SIN PODA
// =========================================================

static int ejecutarComparacion(
    const std::string& nombre,
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites
) {
    mostrarInstancia(
        "COMPARACION: " + nombre,
        politica,
        alfabeto,
        limites
    );


    std::cout
        << "\nEjecutando version CON poda...\n";


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


    std::cout
        << "\nEjecutando version SIN poda...\n";


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


    // -----------------------------------------------------
    // COMPARACION COMPLETA
    // -----------------------------------------------------

    if (
        !conPoda.interrumpido &&
        !sinPoda.interrumpido
    ) {
        std::cout
            << "\n=== RESULTADO DE LA COMPARACION ===\n";

        if (
            conPoda.soluciones ==
            sinPoda.soluciones
        ) {
            std::cout
                << "Soluciones: ambas versiones coinciden.\n";
        } else {
            std::cout
                << "ERROR: las versiones encontraron "
                << "diferente numero de soluciones.\n";

            return 1;
        }


        double reduccion =
            calcularReduccion(
                sinPoda.nodosVisitados,
                conPoda.nodosVisitados
            );


        std::cout
            << "Nodos sin poda: "
            << sinPoda.nodosVisitados
            << "\n";

        std::cout
            << "Nodos con poda: "
            << conPoda.nodosVisitados
            << "\n";

        std::cout
            << "Reduccion medida: "
            << reduccion
            << "%\n";


        if (conPoda.tiempoMs > 0.0) {
            double relacionTiempo =
                sinPoda.tiempoMs
                /
                conPoda.tiempoMs;

            std::cout
                << "Relacion de tiempo sin/con poda: "
                << relacionTiempo
                << "x\n";
        }


        std::cout
            << "Comparacion: COMPLETA\n";

        return 0;
    }


    // -----------------------------------------------------
    // COMPARACION PARCIAL
    // -----------------------------------------------------

    std::cout
        << "\n=== RESULTADO DE LA COMPARACION ===\n";

    std::cout
        << "Comparacion: PARCIAL\n";

    std::cout
        << "Al menos una version alcanzo "
        << "el limite de nodos.\n";

    std::cout
        << "Las soluciones encontradas NO pueden "
        << "compararse como totales.\n";

    std::cout
        << "Esta ejecucion sirve como calibracion "
        << "del costo computacional.\n";


    return 0;
}


// =========================================================
// AYUDA
// =========================================================

static void mostrarAyuda() {
    std::cout
        << "Uso:\n\n"

        << "Ejecucion con poda:\n"
        << "  ./ada_p1 prueba\n"
        << "  ./ada_p1 referencia [max_nodos]\n"
        << "  ./ada_p1 equipo_n6 [max_nodos]\n"
        << "  ./ada_p1 equipo_n8 [max_nodos]\n"
        << "  ./ada_p1 equipo_n10 [max_nodos]\n"
        << "  ./ada_p1 relajada_n8 [max_nodos]\n"
        << "  ./ada_p1 sin_restricciones_n6 [max_nodos]\n\n"

        << "Comparacion con poda vs sin poda:\n"
        << "  ./ada_p1 comparar prueba\n"
        << "  ./ada_p1 comparar referencia max_nodos\n"
        << "  ./ada_p1 comparar equipo_n6 max_nodos\n"
        << "  ./ada_p1 comparar equipo_n8 max_nodos\n"
        << "  ./ada_p1 comparar equipo_n10 max_nodos\n"
        << "  ./ada_p1 comparar relajada_n8 max_nodos\n"
        << "  ./ada_p1 comparar sin_restricciones_n6 max_nodos\n";
}


// =========================================================
// MAIN
// =========================================================

int main(int argc, char* argv[]) {
    std::cout
        << std::fixed
        << std::setprecision(3);


    if (argc < 2) {
        mostrarAyuda();
        return 0;
    }


    // =====================================================
    // MODO COMPARACION
    // =====================================================

    if (std::string(argv[1]) == "comparar") {
        if (argc < 3 || argc > 4) {
            mostrarAyuda();
            return 1;
        }


        std::string nombre = argv[2];

        Politica politica;
        std::string alfabeto;


        if (
            !cargarConfiguracion(
                nombre,
                politica,
                alfabeto
            )
        ) {
            std::cerr
                << "Configuracion no valida.\n";

            return 1;
        }


        LimitesBT limites;


        if (argc == 4) {
            try {
                limites.maxNodos =
                    std::stoull(argv[3]);
            } catch (...) {
                std::cerr
                    << "El limite de nodos no es valido.\n";

                return 1;
            }
        }


        // La prueba pequena puede ejecutarse completa.
        if (nombre == "prueba") {
            limites.maxNodos = 0;
        } else {
            // Para cualquier instancia real exigimos limite
            // al ejecutar la version sin poda.
            if (limites.maxNodos == 0) {
                std::cerr
                    << "SEGURIDAD: para comparar una instancia "
                    << "real debes indicar max_nodos.\n";

                std::cerr
                    << "Ejemplo:\n"
                    << "./ada_p1 comparar "
                    << nombre
                    << " 1000000\n";

                return 1;
            }
        }


        return ejecutarComparacion(
            nombre,
            politica,
            alfabeto,
            limites
        );
    }


    // =====================================================
    // MODO NORMAL
    // =====================================================

    if (argc > 3) {
        mostrarAyuda();
        return 1;
    }


    std::string nombre = argv[1];

    Politica politica;
    std::string alfabeto;


    if (
        !cargarConfiguracion(
            nombre,
            politica,
            alfabeto
        )
    ) {
        std::cerr
            << "Configuracion no valida.\n";

        return 1;
    }


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


    return ejecutarInstancia(
        nombre,
        politica,
        alfabeto,
        limites
    );
}
