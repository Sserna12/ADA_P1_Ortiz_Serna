#include "fb_instancias.hpp"
#include "fb_core.hpp"

#include <algorithm>
#include <cctype>

using namespace std;

namespace fb {

    string normalizarApellido(const string& apellido) {
        string resultado = "";

        for (int i = 0; i < apellido.size(); i++) {
            char c = apellido[i];
            if (isspace(c)) {
                continue;
            }
            resultado = resultado + (char)tolower(c);
        }
        return resultado;
    }

    long long calcularSemilla(vector<string> apellidos) {
        
        for (int i = 0; i < apellidos.size(); i++) {
            apellidos[i] = normalizarApellido(apellidos[i]);
        }
        sort(apellidos.begin(), apellidos.end());

        string concatenado = "";
        for (int i = 0; i < apellidos.size(); i++) {
            concatenado = concatenado + apellidos[i];
        }

        long long suma = 0;
        for (int i = 0; i < concatenado.size(); i++) {
            suma = suma + (unsigned char)concatenado[i];
        }
        return suma % 100000;
    }

    vector<InstanciaEquipo> generarInstanciasEquipo(long long semilla) {
        int longitudes[5] = {4, 4, 5, 5, 6};
        const Alfabeto* alfabetos[5] = {&ALFABETO_A1, &ALFABETO_A2, &ALFABETO_A1, &ALFABETO_A2, &ALFABETO_A1};

        vector<InstanciaEquipo> instancias;

        long long x = semilla;
        for (int k = 0; k < 5; k++) {
            const Alfabeto& alf = *alfabetos[k];
            string password = "";

            for (int i = 0; i < longitudes[k]; i++) {
                long long idx = x % (long long)alf.simbolos.size();
                password = password + alf.simbolos[idx];
                x = (1103515245LL * x + 12345LL) % 2147483648LL;
            }

            InstanciaEquipo inst;
            inst.password = password;
            inst.alfabetoNombre = alf.nombre;
            inst.longitud = longitudes[k];
            inst.hashHex = sha256Hex(password);
            instancias.push_back(inst);
        }
        return instancias;
    }

}
