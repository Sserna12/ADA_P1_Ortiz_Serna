#include "bt_backtracking.hpp"

#include "fb_core.hpp"
#include "fb_dictionary.hpp"
#include "fb_instancias.hpp"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>


// =========================================================
// FUERZA BRUTA
// =========================================================

static void imprimirUsoFB() {
    std::cout
        << "Fuerza Bruta:\n"
        << "  ./ada_p1 brute --hash HASH --alfabeto a1|a2 --min n [--max n]\n"
        << "  ./ada_p1 dict --hash HASH --diccionario ruta\n"
        << "  ./ada_p1 seed --apellidos apellido1,apellido2\n"
        << "  ./ada_p1 experiment --config archivo.csv --out salida.csv "
        << "[--diccionario ruta]\n";
}


static std::string obtenerArg(
    const std::vector<std::string>& args,
    const std::string& clave,
    const std::string& porDefecto = ""
) {
    for (std::size_t i = 0; i + 1 < args.size(); ++i) {
        if (args[i] == clave) {
            return args[i + 1];
        }
    }

    return porDefecto;
}


static std::vector<std::string> separarPorComas(
    const std::string& texto
) {
    std::vector<std::string> partes;

    std::stringstream ss(texto);
    std::string parte;

    while (std::getline(ss, parte, ',')) {
        if (!parte.empty()) {
            partes.push_back(parte);
        }
    }

    return partes;
}


static int cmdBrute(
    const std::vector<std::string>& args
) {
    const std::string hash =
        obtenerArg(args, "--hash");

    const std::string nombreAlfabeto =
        obtenerArg(args, "--alfabeto");

    const std::string minStr =
        obtenerArg(args, "--min");

    const std::string maxStr =
        obtenerArg(args, "--max", minStr);


    if (
        hash.empty() ||
        nombreAlfabeto.empty() ||
        minStr.empty()
    ) {
        std::cerr
            << "Faltan argumentos obligatorios para 'brute'.\n";

        imprimirUsoFB();
        return 1;
    }


    const fb::Alfabeto& alfabeto =
        fb::alfabetoPorNombre(nombreAlfabeto);

    const int minLen =
        std::stoi(minStr);

    const int maxLen =
        std::stoi(maxStr);


    fb::ResultadoBusqueda r =
        fb::buscarPorFuerzaBruta(
            hash,
            alfabeto,
            minLen,
            maxLen
        );


    std::cout
        << "=== Fuerza bruta pura ===\n";

    std::cout
        << "Alfabeto: "
        << alfabeto.nombre
        << " (|Sigma|="
        << alfabeto.simbolos.size()
        << ")\n";

    std::cout
        << "Longitudes exploradas: ["
        << minLen
        << ", "
        << maxLen
        << "]\n";

    std::cout
        << "Candidatos evaluados: "
        << r.candidatosEvaluados
        << "\n";

    std::cout
        << "Tiempo: "
        << r.tiempoMs
        << " ms\n";


    if (r.encontrada) {
        std::cout
            << "Resultado: ENCONTRADA -> \""
            << r.password
            << "\" (longitud "
            << r.longitudEncontrada
            << ")\n";
    } else {
        std::cout
            << "Resultado: no encontrada "
            << "dentro del espacio explorado\n";
    }


    return 0;
}


static int cmdDict(
    const std::vector<std::string>& args
) {
    const std::string hash =
        obtenerArg(args, "--hash");

    const std::string ruta =
        obtenerArg(args, "--diccionario");


    if (hash.empty() || ruta.empty()) {
        std::cerr
            << "Faltan argumentos obligatorios para 'dict'.\n";

        imprimirUsoFB();
        return 1;
    }


    fb::ResultadoDiccionario r =
        fb::buscarPorDiccionario(
            hash,
            ruta
        );


    if (r.errorLectura) {
        std::cerr
            << "No se pudo abrir el diccionario: "
            << ruta
            << "\n";

        return 1;
    }


    std::cout
        << "=== Ataque por diccionario ===\n";

    std::cout
        << "Diccionario: "
        << ruta
        << "\n";

    std::cout
        << "Candidatos evaluados: "
        << r.candidatosEvaluados
        << "\n";

    std::cout
        << "Tiempo: "
        << r.tiempoMs
        << " ms\n";


    if (r.encontrada) {
        std::cout
            << "Resultado: ENCONTRADA -> \""
            << r.password
            << "\"\n";
    } else {
        std::cout
            << "Resultado: no encontrada "
            << "en el diccionario\n";
    }


    return 0;
}


static int cmdSeed(
    const std::vector<std::string>& args
) {
    const std::string apellidosStr =
        obtenerArg(args, "--apellidos");


    if (apellidosStr.empty()) {
        std::cerr
            << "Falta --apellidos "
            << "(lista separada por comas).\n";

        imprimirUsoFB();
        return 1;
    }


    std::vector<std::string> apellidos =
        separarPorComas(apellidosStr);

    long long semilla =
        fb::calcularSemilla(apellidos);


    std::vector<std::string> apellidosNormalizados;

    for (const std::string& apellido : apellidos) {
        apellidosNormalizados.push_back(
            fb::normalizarApellido(apellido)
        );
    }

    std::sort(
        apellidosNormalizados.begin(),
        apellidosNormalizados.end()
    );


    std::cout
        << "=== Semilla del equipo ===\n";

    std::cout
        << "Apellidos "
        << "(orden alfabetico, normalizados): ";


    for (
        std::size_t i = 0;
        i < apellidosNormalizados.size();
        ++i
    ) {
        std::cout
            << apellidosNormalizados[i];

        if (
            i + 1 <
            apellidosNormalizados.size()
        ) {
            std::cout << " + ";
        }
    }


    std::cout
        << "\nSemilla = "
        << semilla
        << "\n\n";


    std::vector<fb::InstanciaEquipo> instancias =
        fb::generarInstanciasEquipo(semilla);


    std::cout
        << "id,alfabeto,longitud,password,hash_sha256\n";


    for (
        std::size_t i = 0;
        i < instancias.size();
        ++i
    ) {
        const fb::InstanciaEquipo& inst =
            instancias[i];

        std::cout
            << (i + 1)
            << ","
            << inst.alfabetoNombre
            << ","
            << inst.longitud
            << ","
            << inst.password
            << ","
            << inst.hashHex
            << "\n";
    }


    return 0;
}


struct FilaConfig {
    std::string id;
    std::string tipo;
    std::string alfabetoNombre;
    int longitud = 0;
    std::string hashHex;
};


static std::vector<FilaConfig> leerConfigCsv(
    const std::string& ruta
) {
    std::vector<FilaConfig> filas;

    std::ifstream archivo(ruta);

    if (!archivo) {
        return filas;
    }


    std::string linea;
    bool primera = true;


    while (std::getline(archivo, linea)) {
        if (
            !linea.empty() &&
            linea.back() == '\r'
        ) {
            linea.pop_back();
        }


        if (linea.empty()) {
            continue;
        }


        if (primera) {
            primera = false;
            continue;
        }


        std::stringstream ss(linea);

        std::string campo;

        std::vector<std::string> campos;


        while (std::getline(ss, campo, ',')) {
            campos.push_back(campo);
        }


        if (campos.size() < 5) {
            continue;
        }


        FilaConfig f;

        f.id = campos[0];
        f.tipo = campos[1];
        f.alfabetoNombre = campos[2];
        f.longitud = std::stoi(campos[3]);
        f.hashHex = campos[4];

        filas.push_back(f);
    }


    return filas;
}


static int cmdExperiment(
    const std::vector<std::string>& args
) {
    const std::string rutaConfig =
        obtenerArg(args, "--config");

    const std::string rutaSalida =
        obtenerArg(args, "--out");

    const std::string rutaDiccionario =
        obtenerArg(
            args,
            "--diccionario",
            "resources/diccionario.txt"
        );


    if (
        rutaConfig.empty() ||
        rutaSalida.empty()
    ) {
        std::cerr
            << "Faltan argumentos obligatorios "
            << "para 'experiment'.\n";

        imprimirUsoFB();
        return 1;
    }


    std::vector<FilaConfig> filas =
        leerConfigCsv(rutaConfig);


    if (filas.empty()) {
        std::cerr
            << "No se pudieron leer instancias desde: "
            << rutaConfig
            << "\n";

        return 1;
    }


    std::ofstream salida(rutaSalida);


    if (!salida) {
        std::cerr
            << "No se pudo crear el archivo de salida: "
            << rutaSalida
            << "\n";

        return 1;
    }


    salida
        << "id,tipo,alfabeto,longitud,"
        << "espacio_teorico,candidatos_fb,"
        << "tiempo_fb_ms,encontrada_fb,"
        << "candidatos_dict,tiempo_dict_ms,"
        << "encontrada_dict\n";


    for (const FilaConfig& f : filas) {
        const fb::Alfabeto& alfabeto =
            fb::alfabetoPorNombre(
                f.alfabetoNombre
            );


        unsigned long long espacioTeorico = 1;


        for (int i = 0; i < f.longitud; ++i) {
            espacioTeorico *=
                alfabeto.simbolos.size();
        }


        fb::ResultadoBusqueda rFb =
            fb::buscarPorFuerzaBrutaLongitudFija(
                f.hashHex,
                alfabeto,
                f.longitud
            );


        fb::ResultadoDiccionario rDict =
            fb::buscarPorDiccionario(
                f.hashHex,
                rutaDiccionario
            );


        std::cout
            << "["
            << f.id
            << "] fb: "
            << rFb.tiempoMs
            << " ms ("
            << rFb.candidatosEvaluados
            << " candidatos) | dict: "
            << rDict.tiempoMs
            << " ms\n";


        const int fbOk =
            rFb.encontrada ? 1 : 0;

        const int dictOk =
            rDict.encontrada ? 1 : 0;


        salida
            << f.id
            << ","
            << f.tipo
            << ","
            << alfabeto.nombre
            << ","
            << f.longitud
            << ","
            << espacioTeorico
            << ","
            << rFb.candidatosEvaluados
            << ","
            << rFb.tiempoMs
            << ","
            << fbOk
            << ","
            << rDict.candidatosEvaluados
            << ","
            << rDict.tiempoMs
            << ","
            << dictOk
            << "\n";
    }


    std::cout
        << "\nResultados escritos en "
        << rutaSalida
        << "\n";


    return 0;
}


// =========================================================
// BACKTRACKING
// =========================================================

static void mostrarMetricasBT(
    const std::string& titulo,
    const MetricasBT& metricas
) {
    std::cout
        << "\n"
        << titulo
        << "\n";

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
            std::numeric_limits<
                std::uint64_t
            >::max() / tamAlfabeto
        ) {
            return 0;
        }


        potencia *= tamAlfabeto;


        if (
            total >
            std::numeric_limits<
                std::uint64_t
            >::max() - potencia
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


    return (
        (
            static_cast<double>(
                nodosSinPoda
            )
            -
            static_cast<double>(
                nodosConPoda
            )
        )
        /
        static_cast<double>(
            nodosSinPoda
        )
    ) * 100.0;
}


static bool cargarConfiguracionBT(
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


static std::uint64_t mostrarInstanciaBT(
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


static int ejecutarInstanciaBT(
    const std::string& nombre,
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites
) {
    std::uint64_t nodosTeoricos =
        mostrarInstanciaBT(
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


    mostrarMetricasBT(
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


static int ejecutarComparacionBT(
    const std::string& nombre,
    const Politica& politica,
    const std::string& alfabeto,
    const LimitesBT& limites
) {
    mostrarInstanciaBT(
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


    mostrarMetricasBT(
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


    mostrarMetricasBT(
        "=== SIN PODA ===",
        sinPoda
    );


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


static void mostrarUsoBT() {
    std::cout
        << "Backtracking:\n"
        << "  ./ada_p1 prueba\n"
        << "  ./ada_p1 referencia [max_nodos]\n"
        << "  ./ada_p1 equipo_n6 [max_nodos]\n"
        << "  ./ada_p1 equipo_n8 [max_nodos]\n"
        << "  ./ada_p1 equipo_n10 [max_nodos]\n"
        << "  ./ada_p1 relajada_n8 [max_nodos]\n"
        << "  ./ada_p1 sin_restricciones_n6 [max_nodos]\n\n"

        << "Comparacion Backtracking:\n"
        << "  ./ada_p1 comparar prueba\n"
        << "  ./ada_p1 comparar referencia max_nodos\n"
        << "  ./ada_p1 comparar equipo_n6 max_nodos\n"
        << "  ./ada_p1 comparar equipo_n8 max_nodos\n"
        << "  ./ada_p1 comparar equipo_n10 max_nodos\n"
        << "  ./ada_p1 comparar relajada_n8 max_nodos\n"
        << "  ./ada_p1 comparar sin_restricciones_n6 max_nodos\n";
}


static void mostrarUsoGeneral() {
    std::cout
        << "====================================\n"
        << "Practica 1 - Fuerza Bruta y Backtracking\n"
        << "====================================\n\n";

    imprimirUsoFB();

    std::cout << "\n";

    mostrarUsoBT();
}


// =========================================================
// MAIN INTEGRADO
// =========================================================

int main(int argc, char** argv) {
    std::cout
        << std::fixed
        << std::setprecision(3);


    if (argc < 2) {
        mostrarUsoGeneral();
        return 0;
    }


    const std::string comando = argv[1];


    // =====================================================
    // FUERZA BRUTA
    // =====================================================

    if (
        comando == "brute" ||
        comando == "dict" ||
        comando == "seed" ||
        comando == "experiment"
    ) {
        std::vector<std::string> args(
            argv + 2,
            argv + argc
        );


        try {
            if (comando == "brute") {
                return cmdBrute(args);
            }

            if (comando == "dict") {
                return cmdDict(args);
            }

            if (comando == "seed") {
                return cmdSeed(args);
            }

            if (comando == "experiment") {
                return cmdExperiment(args);
            }
        } catch (const std::exception& e) {
            std::cerr
                << "Error: "
                << e.what()
                << "\n";

            return 1;
        }
    }


    // =====================================================
    // BACKTRACKING - COMPARACION
    // =====================================================

    if (comando == "comparar") {
        if (argc < 3 || argc > 4) {
            mostrarUsoBT();
            return 1;
        }


        const std::string nombre =
            argv[2];

        Politica politica;
        std::string alfabeto;


        if (
            !cargarConfiguracionBT(
                nombre,
                politica,
                alfabeto
            )
        ) {
            std::cerr
                << "Configuracion de Backtracking "
                << "no valida.\n";

            return 1;
        }


        LimitesBT limites;


        if (argc == 4) {
            try {
                limites.maxNodos =
                    std::stoull(argv[3]);
            } catch (...) {
                std::cerr
                    << "El limite de nodos "
                    << "no es valido.\n";

                return 1;
            }
        }


        if (nombre == "prueba") {
            limites.maxNodos = 0;
        } else if (limites.maxNodos == 0) {
            std::cerr
                << "SEGURIDAD: para comparar una "
                << "instancia real debes indicar "
                << "max_nodos.\n";

            std::cerr
                << "Ejemplo:\n"
                << "./ada_p1 comparar "
                << nombre
                << " 1000000\n";

            return 1;
        }


        return ejecutarComparacionBT(
            nombre,
            politica,
            alfabeto,
            limites
        );
    }


    // =====================================================
    // BACKTRACKING - EJECUCION NORMAL
    // =====================================================

    Politica politica;
    std::string alfabeto;


    if (
        cargarConfiguracionBT(
            comando,
            politica,
            alfabeto
        )
    ) {
        if (argc > 3) {
            mostrarUsoBT();
            return 1;
        }


        LimitesBT limites;


        if (argc == 3) {
            try {
                limites.maxNodos =
                    std::stoull(argv[2]);
            } catch (...) {
                std::cerr
                    << "El limite de nodos "
                    << "no es valido.\n";

                return 1;
            }
        }


        return ejecutarInstanciaBT(
            comando,
            politica,
            alfabeto,
            limites
        );
    }


    // =====================================================
    // COMANDO DESCONOCIDO
    // =====================================================

    std::cerr
        << "Comando desconocido: "
        << comando
        << "\n\n";

    mostrarUsoGeneral();

    return 1;
}
