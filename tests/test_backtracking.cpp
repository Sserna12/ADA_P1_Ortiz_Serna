#include "../src/bt_backtracking.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    Politica politica;
    politica.n = 8;
    politica.minLower = 3;
    politica.minUpper = 2;
    politica.minDigit = 2;
    politica.minSymbol = 1;
    politica.sinRepetidosConsecutivos = true;


    // =====================================================
    // CLASIFICACION
    // =====================================================

    assert(esMinuscula('a'));
    assert(esMinuscula('z'));
    assert(!esMinuscula('A'));

    assert(esMayuscula('A'));
    assert(esMayuscula('Z'));
    assert(!esMayuscula('a'));

    assert(esDigito('0'));
    assert(esDigito('9'));
    assert(!esDigito('a'));

    assert(esSimbolo('!'));
    assert(esSimbolo('@'));
    assert(esSimbolo('#'));
    assert(esSimbolo('$'));
    assert(esSimbolo('%'));
    assert(!esSimbolo('&'));


    // =====================================================
    // SOLUCION VALIDA
    // =====================================================

    EstadoBT valido;
    valido.prefijo = "abcAB12!";
    valido.lower = 3;
    valido.upper = 2;
    valido.digit = 2;
    valido.symbol = 1;

    assert(solucionValida(valido, politica));


    // =====================================================
    // LONGITUD INCORRECTA
    // =====================================================

    EstadoBT longitudIncorrecta;
    longitudIncorrecta.prefijo = "abcAB12";
    longitudIncorrecta.lower = 3;
    longitudIncorrecta.upper = 2;
    longitudIncorrecta.digit = 2;
    longitudIncorrecta.symbol = 0;

    assert(
        !solucionValida(
            longitudIncorrecta,
            politica
        )
    );


    // =====================================================
    // REPETICION CONSECUTIVA
    // =====================================================

    EstadoBT repetido;
    repetido.prefijo = "abbAB12!";
    repetido.lower = 3;
    repetido.upper = 2;
    repetido.digit = 2;
    repetido.symbol = 1;

    assert(!solucionValida(repetido, politica));


    // =====================================================
    // ESTADO FACTIBLE
    // =====================================================

    EstadoBT factible;
    factible.prefijo = "abA1";
    factible.lower = 2;
    factible.upper = 1;
    factible.digit = 1;
    factible.symbol = 0;

    assert(estadoFactible(factible, politica));


    // =====================================================
    // ESTADO NO FACTIBLE
    // =====================================================

    EstadoBT noFactible;
    noFactible.prefijo = "abcA12x";
    noFactible.lower = 4;
    noFactible.upper = 1;
    noFactible.digit = 2;
    noFactible.symbol = 0;

    assert(!estadoFactible(noFactible, politica));


    // =====================================================
    // EQUIVALENCIA CON PODA VS SIN PODA
    // =====================================================

    Politica politicaPequena;
    politicaPequena.n = 3;
    politicaPequena.minLower = 1;
    politicaPequena.minUpper = 1;
    politicaPequena.minDigit = 1;
    politicaPequena.minSymbol = 0;
    politicaPequena.sinRepetidosConsecutivos = true;

    std::string alfabetoPequeno = "aA1";

    MetricasBT conPoda =
        ejecutarConPoda(
            politicaPequena,
            alfabetoPequeno
        );

    MetricasBT sinPoda =
        ejecutarSinPoda(
            politicaPequena,
            alfabetoPequeno
        );

    assert(
        conPoda.soluciones ==
        sinPoda.soluciones
    );

    assert(conPoda.soluciones == 6);

    assert(
        conPoda.nodosVisitados <=
        sinPoda.nodosVisitados
    );


    // =====================================================
    // INSTANCIA IMPOSIBLE
    // =====================================================

    Politica imposible;
    imposible.n = 6;
    imposible.minLower = 3;
    imposible.minUpper = 2;
    imposible.minDigit = 2;
    imposible.minSymbol = 1;
    imposible.sinRepetidosConsecutivos = true;

    MetricasBT resultadoImposible =
        ejecutarConPoda(
            imposible,
            "abAB12!"
        );

    assert(resultadoImposible.soluciones == 0);


    // =====================================================
    // LIMITE DE NODOS
    // =====================================================

    Politica grande;
    grande.n = 8;
    grande.minLower = 0;
    grande.minUpper = 0;
    grande.minDigit = 0;
    grande.minSymbol = 0;
    grande.sinRepetidosConsecutivos = false;

    LimitesBT limite;
    limite.maxNodos = 100;

    MetricasBT limitada =
        ejecutarSinPoda(
            grande,
            "abc",
            limite
        );

    assert(limitada.interrumpido);
    assert(limitada.nodosVisitados == 100);


    std::cout
        << "Todas las pruebas de Backtracking "
        << "pasaron correctamente.\n";

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

    std::cout
        << "Prueba de limite: "
        << limitada.nodosVisitados
        << " nodos, interrumpida correctamente.\n";

    return 0;
}
