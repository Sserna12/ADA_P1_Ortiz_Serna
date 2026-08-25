#include "bt_backtracking.hpp"

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
