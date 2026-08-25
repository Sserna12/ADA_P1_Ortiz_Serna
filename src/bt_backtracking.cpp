#include "bt_backtracking.hpp"
#include <algorithm>

bool esMinuscula(char c) {
    return c >= 'a' && c <= 'z';
}

bool esMayuscula(char c) {
    return c >= 'A' && c <= 'Z';
}

bool esDigito(char c) {
    return c >= '0' && c <= '9';
}

bool esSimbolo(char c) {
    return c == '!' || c == '@' || c == '#' || c == '$' || c == '%';
}

bool solucionValida(
    const EstadoBT& estado,
    const Politica& politica
) {
    // Una solución completa debe tener exactamente longitud n.
    if (static_cast<int>(estado.prefijo.size()) != politica.n) {
        return false;
    }

    // Verificar los mínimos exigidos por la política.
    if (estado.lower < politica.minLower) {
        return false;
    }

    if (estado.upper < politica.minUpper) {
        return false;
    }

    if (estado.digit < politica.minDigit) {
        return false;
    }

    if (estado.symbol < politica.minSymbol) {
        return false;
    }

    // Verificar que no haya dos caracteres iguales consecutivos.
    if (politica.sinRepetidosConsecutivos) {
        for (std::size_t i = 1; i < estado.prefijo.size(); ++i) {
            if (estado.prefijo[i] == estado.prefijo[i - 1]) {
                return false;
            }
        }
    }

    return true;
}

bool estadoFactible(
    const EstadoBT& estado,
    const Politica& politica
) {
    // Si el prefijo ya supera la longitud permitida,
    // esta rama no puede producir una solución.
    if (static_cast<int>(estado.prefijo.size()) > politica.n) {
        return false;
    }

    // Número de posiciones disponibles para completar la contraseña.
    int restantes =
        politica.n - static_cast<int>(estado.prefijo.size());

    // Cantidad mínima que todavía falta de cada categoría.
    int faltanLower =
        std::max(0, politica.minLower - estado.lower);

    int faltanUpper =
        std::max(0, politica.minUpper - estado.upper);

    int faltanDigit =
        std::max(0, politica.minDigit - estado.digit);

    int faltanSymbol =
        std::max(0, politica.minSymbol - estado.symbol);

    // Número total de posiciones necesarias para cumplir
    // todos los mínimos pendientes.
    int faltantes =
        faltanLower +
        faltanUpper +
        faltanDigit +
        faltanSymbol;

    // Si hacen falta más caracteres de los espacios disponibles,
    // ninguna extensión de este prefijo puede ser válida.
    if (faltantes > restantes) {
        return false;
    }

    return true;
}