#include "../src/bt_backtracking.hpp"

#include <cassert>
#include <iostream>

int main() {
    Politica politica;
    politica.n = 8;
    politica.minLower = 3;
    politica.minUpper = 2;
    politica.minDigit = 2;
    politica.minSymbol = 1;
    politica.sinRepetidosConsecutivos = true;

    // Caso 1: solucion valida
    EstadoBT valido;
    valido.prefijo = "abcAB12!";
    valido.lower = 3;
    valido.upper = 2;
    valido.digit = 2;
    valido.symbol = 1;

    assert(solucionValida(valido, politica));

    // Caso 2: longitud incorrecta
    EstadoBT longitudIncorrecta;
    longitudIncorrecta.prefijo = "abcAB12";
    longitudIncorrecta.lower = 3;
    longitudIncorrecta.upper = 2;
    longitudIncorrecta.digit = 2;
    longitudIncorrecta.symbol = 0;

    assert(!solucionValida(longitudIncorrecta, politica));

    // Caso 3: repeticion consecutiva
    EstadoBT repetido;
    repetido.prefijo = "abbAB12!";
    repetido.lower = 3;
    repetido.upper = 2;
    repetido.digit = 2;
    repetido.symbol = 1;

    assert(!solucionValida(repetido, politica));

    // Caso 4: estado todavia factible
    EstadoBT factible;
    factible.prefijo = "abA1";
    factible.lower = 2;
    factible.upper = 1;
    factible.digit = 1;
    factible.symbol = 0;

    assert(estadoFactible(factible, politica));

    // Caso 5: estado que debe podarse
    EstadoBT noFactible;
    noFactible.prefijo = "abcA12x";
    noFactible.lower = 4;
    noFactible.upper = 1;
    noFactible.digit = 2;
    noFactible.symbol = 0;

    assert(!estadoFactible(noFactible, politica));

    // Caso 6: comparar con poda vs sin poda en una instancia pequena
    Politica politicaPequena;
    politicaPequena.n = 3;
    politicaPequena.minLower = 1;
    politicaPequena.minUpper = 1;
    politicaPequena.minDigit = 1;
    politicaPequena.minSymbol = 0;
    politicaPequena.sinRepetidosConsecutivos = true;

    std::string alfabetoPequeno = "aA1";

    MetricasBT conPoda =
        ejecutarConPoda(politicaPequena, alfabetoPequeno);

    MetricasBT sinPoda =
        ejecutarSinPoda(politicaPequena, alfabetoPequeno);

    // Ambas estrategias deben encontrar exactamente las mismas soluciones.
    assert(conPoda.soluciones == sinPoda.soluciones);

    // Con tres caracteres distintos y tres posiciones,
    // las 3! = 6 permutaciones son validas.
    assert(conPoda.soluciones == 6);

    // La version con poda no deberia visitar mas nodos que la exhaustiva.
    assert(conPoda.nodosVisitados <= sinPoda.nodosVisitados);

    std::cout
        << "Todas las pruebas de Backtracking pasaron correctamente.\n";

    std::cout
        << "Soluciones prueba pequena: "
        << conPoda.soluciones
        << "\n";

    std::cout
        << "Nodos con poda: "
        << conPoda.nodosVisitados
        << "\n";

    std::cout
        << "Nodos sin poda: "
        << sinPoda.nodosVisitados
        << "\n";

    std::cout
        << "Nodos podados: "
        << conPoda.nodosPodados
        << "\n";

    return 0;
}