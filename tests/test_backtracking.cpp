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

    // Caso 1: solución válida
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

    // Caso 3: repetición consecutiva
    EstadoBT repetido;
    repetido.prefijo = "abbAB12!";
    repetido.lower = 3;
    repetido.upper = 2;
    repetido.digit = 2;
    repetido.symbol = 1;

    assert(!solucionValida(repetido, politica));

    // Caso 4: estado todavía factible
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

    std::cout << "Todas las pruebas de Backtracking pasaron correctamente.\n";

    return 0;
}