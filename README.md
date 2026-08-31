# Práctica 1 — Fuerza Bruta y Backtracking

Curso: Análisis y Diseño de Algoritmos

## Integrantes

- Samuel Serna
- Nicolas Ortiz

## Descripción

Este proyecto implementa y analiza dos estrategias algorítmicas:

- Fuerza Bruta
- Backtracking con poda

El objetivo es estudiar experimentalmente el crecimiento del espacio de búsqueda, medir tiempos de ejecución, comparar resultados empíricos con el comportamiento teórico y analizar el efecto de las podas sobre la cantidad de estados explorados.

El proyecto está desarrollado en C++17.

---

## Estructura del repositorio

```text
ADA_P1_Ortiz_Serna/
│
├── src/
│   ├── main.cpp
│   ├── bt_backtracking.cpp
│   ├── bt_backtracking.hpp
│   ├── fb_core.cpp
│   ├── fb_core.hpp
│   ├── fb_dictionary.cpp
│   ├── fb_dictionary.hpp
│   ├── fb_instancias.cpp
│   ├── fb_instancias.hpp
│   └── third_party/
│       ├── picosha2.h
│       └── LICENSE_picosha2.txt
│
├── tests/
│   ├── test_backtracking.cpp
│   └── test_fb.cpp
│
├── resources/
│   ├── instancia_equipo.txt
│   ├── diccionario.txt
│   └── fb_instancias.csv
│
├── results/
│   ├── resultados de Backtracking
│   ├── fb_tiempos.csv
│   ├── fb_tiempo_vs_n.png
│   └── fb_tiempo_vs_candidatos.png
│
├── scripts/
│   └── graficas_bt.py
│
├── report/
│   ├── aporte_backtracking.md
│   ├── contribucion_samuel.md
│   ├── sustentacion_backtracking.md
│   └── Informe_base.md
│
└── README.md