#include "fb_core.hpp"
#include "fb_dictionary.hpp"
#include "fb_instancias.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void imprimirUso() {
    cout << "Como se usa:\n";
    cout << "ada_p1 brute --hash [HASH] --alfabeto a1 o a2 --min n --max n (max es opcional)\n";
    cout << "ada_p1 dict --hash [HASH] --diccionario ruta_del_archivo\n";
    cout << "ada_p1 seed --apellidos apellido1,apellido2\n";
    cout << "ada_p1 experiment --config archivo.csv --out salida.csv\n";
}

string obtenerArg(const vector<string>& args, const string& clave, const string& porDefecto = "") {
    for (int i = 0; i + 1 < args.size(); i++) {
        if (args[i] == clave) {
            return args[i + 1];
        }
    }
    return porDefecto;
}

vector<string> separarPorComas(const string& texto) {
    vector<string> partes;
    stringstream ss(texto);
    string parte;
    while (getline(ss, parte, ',')) {
        if (!parte.empty()) {
            partes.push_back(parte);
        }
    }
    return partes;
}

int cmdBrute(const vector<string>& args) {
    const string hash = obtenerArg(args, "--hash");
    const string nombreAlfabeto = obtenerArg(args, "--alfabeto");
    const string minStr = obtenerArg(args, "--min");
    const string maxStr = obtenerArg(args, "--max", minStr);

    if (hash.empty() || nombreAlfabeto.empty() || minStr.empty()) {
        cerr << "Faltan argumentos obligatorios para 'brute'.\n";
        imprimirUso();
        return 1;
    }

    const fb::Alfabeto& alfabeto = fb::alfabetoPorNombre(nombreAlfabeto);
    const int minLen = stoi(minStr);
    const int maxLen = stoi(maxStr);

    fb::ResultadoBusqueda r = fb::buscarPorFuerzaBruta(hash, alfabeto, minLen, maxLen);

    cout << "=== Fuerza bruta pura ===\n";
    cout << "Alfabeto: " << alfabeto.nombre << " (|Sigma|=" << alfabeto.simbolos.size() << ")\n";
    cout << "Longitudes exploradas: [" << minLen << ", " << maxLen << "]\n";
    cout << "Candidatos evaluados: " << r.candidatosEvaluados << "\n";
    cout << "Tiempo: " << r.tiempoMs << " ms\n";
    if (r.encontrada) {
        cout << "Resultado: ENCONTRADA -> \"" << r.password << "\" (longitud " << r.longitudEncontrada << ")\n";
    } else {
        cout << "Resultado: no encontrada dentro del espacio explorado\n";
    }
    return 0;
}

int cmdDict(const vector<string>& args) {
    const string hash = obtenerArg(args, "--hash");
    const string ruta = obtenerArg(args, "--diccionario");

    if (hash.empty() || ruta.empty()) {
        cerr << "Faltan argumentos obligatorios para 'dict'.\n";
        imprimirUso();
        return 1;
    }

    fb::ResultadoDiccionario r = fb::buscarPorDiccionario(hash, ruta);

    if (r.errorLectura) {
        cerr << "No se pudo abrir el diccionario: " << ruta << "\n";
        return 1;
    }

    cout << "=== Ataque por diccionario ===\n";
    cout << "Diccionario: " << ruta << "\n";
    cout << "Candidatos evaluados: " << r.candidatosEvaluados << "\n";
    cout << "Tiempo: " << r.tiempoMs << " ms\n";
    if (r.encontrada) {
        cout << "Resultado: ENCONTRADA -> \"" << r.password << "\"\n";
    } else {
        cout << "Resultado: no encontrada en el diccionario\n";
    }
    return 0;
}

int cmdSeed(const vector<string>& args) {
    const string apellidosStr = obtenerArg(args, "--apellidos");
    
    if (apellidosStr.empty()) {
        cerr << "Falta --apellidos (lista separada por comas).\n";
        imprimirUso();
        return 1;
    }

    vector<string> apellidos = separarPorComas(apellidosStr);
    long long semilla = fb::calcularSemilla(apellidos);

    vector<string> apellidosNormalizados;
    for (int i = 0; i < apellidos.size(); i++) {
        apellidosNormalizados.push_back(fb::normalizarApellido(apellidos[i]));
    }
    sort(apellidosNormalizados.begin(), apellidosNormalizados.end());

    cout << "=== Semilla del equipo (Seccion 9.1) ===\n";
    cout << "Apellidos (orden alfabetico, normalizados): ";
    for (int i = 0; i < apellidosNormalizados.size(); i++) {
        cout << apellidosNormalizados[i];
        if (i + 1 < apellidosNormalizados.size()) {
            cout << " + ";
        }
    }
    cout << "\nSemilla = " << semilla << "\n\n";

    vector<fb::InstanciaEquipo> instancias = fb::generarInstanciasEquipo(semilla);
    cout << "id,alfabeto,longitud,password,hash_sha256\n";
    for (int i = 0; i < instancias.size(); i++) {
        fb::InstanciaEquipo inst = instancias[i];
        cout << (i + 1) << "," << inst.alfabetoNombre << "," << inst.longitud << "," << inst.password << "," << inst.hashHex << "\n";
    }
    return 0;
}

struct FilaConfig {
    string id;
    string tipo;
    string alfabetoNombre;
    int longitud = 0;
    string hashHex;
};

vector<FilaConfig> leerConfigCsv(const string& ruta) {
    vector<FilaConfig> filas;
    ifstream archivo(ruta);
    if (!archivo) {
        return filas;
    }

    string linea;
    bool primera = true;
    while (getline(archivo, linea)) {
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }
        if (linea.empty()) {
            continue;
        }
        if (primera) {
            primera = false;
            continue;
        }

        stringstream ss(linea);
        string campo;
        vector<string> campos;
        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }
        if (campos.size() < 5) {
            continue;
        }

        FilaConfig f;
        f.id = campos[0];
        f.tipo = campos[1];
        f.alfabetoNombre = campos[2];
        f.longitud = stoi(campos[3]);
        f.hashHex = campos[4];
        filas.push_back(f);
    }
    return filas;
}

int cmdExperiment(const vector<string>& args) {
    const string rutaConfig = obtenerArg(args, "--config");
    const string rutaSalida = obtenerArg(args, "--out");
    const string rutaDiccionario = obtenerArg(args, "--diccionario", "resources/diccionario.txt");

    if (rutaConfig.empty() || rutaSalida.empty()) {
        cerr << "Faltan argumentos obligatorios para 'experiment'.\n";
        imprimirUso();
        return 1;
    }

    vector<FilaConfig> filas = leerConfigCsv(rutaConfig);
    if (filas.empty()) {
        cerr << "No se pudieron leer instancias desde: " << rutaConfig << "\n";
        return 1;
    }

    ofstream salida(rutaSalida);
    if (!salida) {
        cerr << "No se pudo crear el archivo de salida: " << rutaSalida << "\n";
        return 1;
    }

    salida << "id,tipo,alfabeto,longitud,espacio_teorico,candidatos_fb,tiempo_fb_ms,encontrada_fb,candidatos_dict,tiempo_dict_ms,encontrada_dict\n";

    for (int j = 0; j < filas.size(); j++) {
        FilaConfig f = filas[j];
        const fb::Alfabeto& alfabeto = fb::alfabetoPorNombre(f.alfabetoNombre);

        unsigned long long espacioTeorico = 1;
        for (int i = 0; i < f.longitud; i++) {
            espacioTeorico = espacioTeorico * alfabeto.simbolos.size();
        }

        fb::ResultadoBusqueda rFb = fb::buscarPorFuerzaBrutaLongitudFija(f.hashHex, alfabeto, f.longitud);
        fb::ResultadoDiccionario rDict = fb::buscarPorDiccionario(f.hashHex, rutaDiccionario);

        cout << "[" << f.id << "] fb: " << rFb.tiempoMs << " ms (" << rFb.candidatosEvaluados << " candidatos) | dict: " << rDict.tiempoMs << " ms\n";

        int fbOk = 0;
        if (rFb.encontrada) {
            fbOk = 1;
        }
        int dictOk = 0;
        if (rDict.encontrada) {
            dictOk = 1;
        }

        salida << f.id << "," << f.tipo << "," << alfabeto.nombre << "," << f.longitud << "," << espacioTeorico << "," << rFb.candidatosEvaluados << "," << rFb.tiempoMs << "," << fbOk << "," << rDict.candidatosEvaluados << "," << rDict.tiempoMs << "," << dictOk << "\n";
    }

    cout << "\nResultados escritos en " << rutaSalida << "\n";
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        imprimirUso();
        return 1;
    }

    const string subcomando = argv[1];
    vector<string> args(argv + 2, argv + argc);

    try {
        if (subcomando == "brute") return cmdBrute(args);
        if (subcomando == "dict") return cmdDict(args);
        if (subcomando == "seed") return cmdSeed(args);
        if (subcomando == "experiment") return cmdExperiment(args);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    cerr << "Subcomando desconocido: " << subcomando << "\n";
    imprimirUso();
    return 1;
}
