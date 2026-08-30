# ADA Práctica 1 - Módulo FB (Fuerza Bruta)

Integrantes: Nicolas Ortiz, Samuel Serna.

## Estructura

```
src/            fb_core.*, fb_dictionary.*, fb_instancias.*, main.cpp, third_party/picosha2.h
tests/          test_fb.cpp
resources/      diccionario.txt, fb_instancias.csv (13 instancias: referencia + barrido + equipo)
results/        fb_tiempos.csv, fb_tiempo_vs_n.png, fb_tiempo_vs_candidatos.png
```

## Compilación

```
g++ -std=c++17 -O2 -o ada_p1 src/main.cpp src/fb_core.cpp src/fb_dictionary.cpp src/fb_instancias.cpp
```

## Ejecución

Fuerza bruta contra un hash, sobre un alfabeto y un rango de longitudes:

```
./ada_p1 brute --hash <hex_sha256> --alfabeto a1|a2 --min <n> [--max <n>]
```

Ataque por diccionario:

```
./ada_p1 dict --hash <hex_sha256> --diccionario resources/diccionario.txt
```

Cálculo de la seed y de las 5 instancias propias:

```
./ada_p1 seed --apellidos Ortiz,Serna
```

Corrida por lotes (fuerza bruta + diccionario) a partir de un CSV de instancias,
generando la tabla de tiempos en `results/`:

```
./ada_p1 experiment --config resources/fb_instancias.csv --out results/fb_tiempos.csv
```

Nota: el experimento completo (13 instancias) tarda cerca de 9 minutos, dominado
por las instancias A1/A2 con n=5 y n=6.

## Pruebas

```
g++ -std=c++17 -O2 -I src -o tests/test_fb tests/test_fb.cpp src/fb_core.cpp src/fb_dictionary.cpp src/fb_instancias.cpp
./tests/test_fb
```

Valida `sha256Hex` y la búsqueda por fuerza bruta contra la instancia ("abc12", alfabeto A2, n=5), 
el comportamiento del ataque por diccionario,
el cálculo de la seed del equipo y la generación de las 5 instancias propias.

## Seed e instancias

Apellidos ordenados alfabéticamente: `ortiz`, `serna` → concatenado `ortizserna`.
Seed = (suma de códigos ASCII) mod 100000 = **1105**.

Aquí se implementó una secuencia **continua**

## Resultados y experimentación

`resources/fb_instancias.csv` tiene 13 instancias: La de referencia, un
barrido de longitudes por alfabeto y las 5 instancias
propias. `results/fb_tiempos.csv` ya contiene los tiempos y candidatos
evaluados de una ejecucion real de las 13.

