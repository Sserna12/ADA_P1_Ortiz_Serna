#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace fb {

struct InstanciaEquipo {
    std::string password;
    std::string alfabetoNombre;
    int longitud = 0;
    std::string hashHex;
};

std::string normalizarApellido(const std::string& apellido);

long long calcularSemilla(std::vector<std::string> apellidos);

std::vector<InstanciaEquipo> generarInstanciasEquipo(long long semilla);

}
