# Práctica 1 — Fuerza Bruta y Backtracking

Curso: Análisis y Diseño de Algoritmos

## Integrantes

- Samuel Serna
- Nicolas Ortiz

## Descripción

Este proyecto implementa y analiza dos estrategias algorítmicas:

- Fuerza Bruta
- Backtracking con poda

El objetivo es comparar experimentalmente el crecimiento del espacio de búsqueda, medir tiempos de ejecución y analizar el efecto de las podas sobre la cantidad de estados explorados.

El proyecto utiliza C++17.

---

## Estructura del repositorio

```text
ADA_P1_Ortiz_Serna/
│
├── src/
│   ├── main.cpp
│   ├── bt_backtracking.cpp
│   ├── bt_backtracking.hpp
│   └── archivos del módulo de Fuerza Bruta
│
├── tests/
│   └── test_backtracking.cpp
│
├── resources/
│   └── instancia_equipo.txt
│
├── results/
│   ├── bt_equipo_n6.txt
│   ├── bt_equipo_n8_calibracion.txt
│   ├── bt_equipo_n10_calibracion.txt
│   ├── bt_relajada_n8_calibracion.txt
│   ├── bt_sin_restricciones_n6_calibracion.txt
│   ├── bt_referencia_calibracion.txt
│   ├── bt_comparacion_equipo_n6.txt
│   ├── bt_resumen.txt
│   ├── bt_resultados_grafica.csv
│   ├── bt_tiempo_vs_n.png
│   └── bt_tiempo_vs_nodos.png
│
├── scripts/
│   └── graficas_bt.py
│
├── report/
│   └── aporte_backtracking.md
│
└── README.md