# 1. Portada

**Práctica 1 — Fuerza Bruta y Backtracking**

Curso: Análisis y Diseño de Algoritmos

Integrantes:
- Samuel Serna
- Nicolas Ortiz

Fecha: [FECHA DE ENTREGA]

---

# 2. Introducción

Esta práctica estudia experimentalmente dos paradigmas algorítmicos: Fuerza Bruta y Backtracking. Ambos módulos trabajan sobre espacios de búsqueda asociados a contraseñas sintéticas, pero abordan problemas diferentes.

En el módulo de Fuerza Bruta se estudia el costo de encontrar una contraseña mediante enumeración sistemática y verificación por hash. En el módulo de Backtracking se estudia la construcción incremental de contraseñas que satisfacen una política determinada, utilizando poda para evitar explorar estados que no pueden conducir a una solución válida.

El objetivo del trabajo es relacionar el análisis teórico de complejidad con mediciones experimentales de tiempo, estados explorados y reducción del espacio de búsqueda.

---

# 3. Contexto del problema

## Fuerza Bruta

[PENDIENTE NICOLAS: síntesis del problema de búsqueda mediante hash y comparación fuerza bruta vs. diccionario.]

## Backtracking

El módulo de Backtracking considera el problema de generar contraseñas que satisfacen una política de composición. Una contraseña válida debe cumplir determinados mínimos de minúsculas, mayúsculas, dígitos y símbolos, además de restricciones adicionales como la prohibición de caracteres idénticos consecutivos.

En lugar de generar primero todas las cadenas posibles y comprobarlas al final, Backtracking construye cada contraseña de forma incremental y descarta anticipadamente los prefijos que no pueden convertirse en una solución válida.

---

# 4. Fundamentación teórica

## Fuerza Bruta

[PENDIENTE NICOLAS]

## Backtracking

Backtracking recorre un árbol de decisiones mediante búsqueda en profundidad. Cada nodo representa una solución parcial, cada arista corresponde a la elección de un nuevo carácter y las hojas representan cadenas completas o ramas descartadas.

La principal diferencia frente a la enumeración exhaustiva consiste en la poda. Si un estado parcial no puede satisfacer las restricciones con las posiciones que todavía quedan disponibles, esa rama se descarta sin continuar explorándola.

En el peor caso, si ninguna restricción permite podar anticipadamente, Backtracking puede recorrer prácticamente el mismo espacio que la enumeración exhaustiva.

---

# 5. Modelamiento

## Fuerza Bruta

[PENDIENTE NICOLAS]

## Backtracking

El estado parcial utilizado contiene:

- prefijo construido;
- número de minúsculas;
- número de mayúsculas;
- número de dígitos;
- número de símbolos.

La política principal del equipo fue derivada de la semilla 1105:

- n = 8
- minLower = 3
- minUpper = 2
- minDigit = 2
- minSymbol = 1
- sin caracteres idénticos consecutivos

El alfabeto utilizado fue:

abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%

Este conjunto tiene 67 caracteres. El enunciado menciona 69 símbolos, pero los conjuntos enumerados explícitamente suman 67, por lo cual se utilizaron los caracteres especificados.

---

# 6. Diseño algorítmico

## Fuerza Bruta

[PENDIENTE NICOLAS]

## Backtracking

El algoritmo construye la contraseña un carácter a la vez.

Antes de expandir un estado se verifica su factibilidad. Se calcula cuántos caracteres de cada categoría todavía hacen falta y se compara esa cantidad con las posiciones restantes.

Si los caracteres faltantes superan las posiciones disponibles, el estado es inviable y se poda.

También se evita generar un hijo cuando el carácter propuesto es igual al último carácter del prefijo y la política prohíbe repeticiones consecutivas.

Se implementó adicionalmente una versión sin poda para realizar la comparación experimental.

---

# 7. Pseudocódigo

## Fuerza Bruta

[PENDIENTE NICOLAS]

## Backtracking con poda

BACKTRACKING(estado)

    contar estado como visitado

    SI estado no es factible
        contar poda
        RETORNAR
    FIN SI

    SI longitud(prefijo) = n
        SI cumple politica
            soluciones = soluciones + 1
        FIN SI
        RETORNAR
    FIN SI

    PARA cada caracter c del alfabeto

        SI c repite consecutivamente un caracter
            contar poda
            CONTINUAR
        FIN SI

        agregar c

        SI el nuevo estado es factible
            BACKTRACKING(estado)
        SINO
            contar poda
        FIN SI

        retirar c

    FIN PARA

FIN BACKTRACKING

## Enumeración sin poda

SIN_PODA(estado)

    contar estado como visitado

    SI longitud(prefijo) = n
        SI cumple politica
            soluciones = soluciones + 1
        FIN SI
        RETORNAR
    FIN SI

    PARA cada caracter c del alfabeto
        agregar c
        SIN_PODA(estado)
        retirar c
    FIN PARA

FIN SIN_PODA

---

# 8. Implementación

## Fuerza Bruta

[PENDIENTE NICOLAS]

## Backtracking

La implementación fue desarrollada en C++17.

Los archivos principales son:

- src/bt_backtracking.hpp
- src/bt_backtracking.cpp
- src/main.cpp
- tests/test_backtracking.cpp

Se definieron estructuras independientes para la política, el estado parcial, las métricas y los límites de ejecución.

Las métricas registran:

- nodos visitados;
- nodos podados;
- soluciones encontradas;
- tiempo;
- estado de interrupción.

Para los experimentos grandes se implementó un máximo configurable de nodos. Las ejecuciones detenidas mediante este mecanismo se identifican explícitamente como parciales.

---

# 9. Análisis de complejidad

## Fuerza Bruta

[PENDIENTE NICOLAS]

## Backtracking

Sea b el tamaño del alfabeto y n la longitud de la contraseña.

El árbol exhaustivo contiene:

1 + b + b^2 + ... + b^n

y tiene b^n hojas.

Por lo tanto, la complejidad temporal en el peor caso es:

O(b^n)

Backtracking conserva esta cota en el peor caso porque puede existir una política que no permita podar estados de manera anticipada.

En situaciones favorables, las restricciones permiten reducir drásticamente la cantidad de estados realmente explorados.

La búsqueda utiliza profundidad máxima n, por lo que el espacio auxiliar es:

O(n)

sin considerar almacenamiento de soluciones, ya que los experimentos contabilizan las soluciones en lugar de almacenarlas todas.

---

# 10. Casos de prueba

## Fuerza Bruta

[PENDIENTE NICOLAS: A1, A2, referencia y semilla.]

## Backtracking

Semilla del equipo: 1105.

Las configuraciones estudiadas fueron:

- política del equipo, n = 6;
- política del equipo, n = 8;
- política del equipo, n = 10;
- política relajada, n = 8;
- sin restricciones, n = 6;
- instancia de referencia;
- instancia pequeña para validación completa.

La instancia pequeña utilizó el alfabeto:

abAB12!

con n = 4 y mínimo un carácter de cada categoría.

Ambas versiones encontraron 192 soluciones.

---

# 11. Experimentación

## Fuerza Bruta

[PENDIENTE NICOLAS: tabla y gráfica.]

## Backtracking

Las pruebas fueron compiladas con:

g++ -std=c++17 -O2

El tiempo fue medido mediante std::chrono.

En la comparación completa pequeña se obtuvo:

| Métrica | Con poda | Sin poda |
|---|---:|---:|
| Nodos visitados | 356 | 2801 |
| Soluciones | 192 | 192 |
| Tiempo | 0.032 ms aprox. | 0.064 ms aprox. |
| Reducción de nodos | 87.290% | - |

Para las instancias grandes se realizaron ejecuciones limitadas, manteniendo explícitamente su estado como INTERRUMPIDO POR LIMITE.

Las gráficas generadas son:

- results/bt_tiempo_vs_n.png
- results/bt_tiempo_vs_nodos.png

---

# 12. Resultados

## Fuerza Bruta

[PENDIENTE NICOLAS]

## Backtracking

### Equipo n = 6

Nodos teóricos sin poda: 91828963717.

Con poda:

- nodos visitados: 1;
- nodos podados: 1;
- soluciones: 0;
- estado: COMPLETADO.

La política exige ocho caracteres mínimos, pero n = 6, por lo cual la instancia es imposible.

### Equipo n = 8

Límite: 10000000 nodos.

- nodos visitados: 10000000;
- nodos podados: 52628655;
- soluciones parciales: 9065240;
- tiempo observado: 423.289 ms;
- estado: INTERRUMPIDO POR LIMITE.

### Equipo n = 10

Límite: 10000000 nodos.

- nodos visitados: 10000000;
- nodos podados: 52628656;
- soluciones parciales: 9065238;
- tiempo observado: 431.392 ms;
- estado: INTERRUMPIDO POR LIMITE.

### Política relajada n = 8

- nodos visitados: 10000000;
- nodos podados: 151517;
- soluciones parciales: 9848479;
- tiempo observado: 130.724 ms;
- estado: INTERRUMPIDO POR LIMITE.

### Sin restricciones n = 6

- nodos visitados: 10000000;
- nodos podados: 0;
- soluciones parciales: 9850742;
- tiempo observado: 102.420 ms;
- estado: INTERRUMPIDO POR LIMITE.

---

# 13. Análisis de resultados

## Fuerza Bruta

[PENDIENTE NICOLAS]

## Backtracking

Los resultados confirman que la efectividad de la poda depende de la capacidad de las restricciones para detectar estados inviables.

El caso equipo_n6 representa un escenario extremo. La suma de requisitos mínimos es ocho y solamente existen seis posiciones. La versión con poda detectó esta condición desde la raíz y visitó un único nodo.

En el extremo contrario, la configuración sin restricciones produjo cero nodos podados. En ese escenario, Backtracking no dispone de información que permita descartar ramas y su comportamiento se aproxima al de una enumeración exhaustiva.

La política relajada produjo significativamente menos poda que la política principal del equipo.

La comparación completa pequeña mostró que ambas versiones encuentran exactamente las mismas 192 soluciones, mientras que la poda reduce los nodos visitados de 2801 a 356, equivalente a 87.290%.

---

# 14. Comparación algorítmica

## Fuerza Bruta pura vs. diccionario

[PENDIENTE NICOLAS]

## Backtracking con poda vs. sin poda

En la prueba completa pequeña:

- con poda: 356 nodos;
- sin poda: 2801 nodos;
- soluciones en ambas versiones: 192;
- reducción: 87.290%.

Esto muestra que la poda conserva las soluciones y reduce el espacio efectivamente recorrido.

En equipo_n6, la versión con poda terminó después de visitar la raíz. La versión sin poda alcanzó un límite experimental de 1000000 de nodos sin completar el árbol.

Esta segunda comparación se considera parcial y se utiliza solamente como evidencia del costo computacional.

---

# 15. Conclusiones

[PENDIENTE INTEGRACIÓN FINAL DEL EQUIPO]

Conclusión parcial de Backtracking:

Backtracking permitió reducir considerablemente el espacio explorado cuando las restricciones hicieron posible detectar prefijos inviables.

La implementación también mostró que la técnica no mejora automáticamente todos los casos: en ausencia de restricciones, el número de estados podados fue cero.

La comparación completa confirmó que la poda mantiene la corrección del resultado mientras disminuye el número de estados explorados.

Los experimentos evidenciaron además el crecimiento exponencial del problema y la necesidad de diferenciar entre ejecuciones completas y calibraciones parciales.

---

# 16. Referencias

[PENDIENTE COMPLETAR ENTRE AMBOS]

- Material del curso de Análisis y Diseño de Algoritmos.
- Enunciado de la Práctica 1 — Fuerza Bruta y Backtracking.
- [PENDIENTE: biblioteca SHA-256 utilizada por Fuerza Bruta.]
- [PENDIENTE: demás fuentes utilizadas.]

---

# 17. Uso de herramientas de IA

Herramienta: OpenAI ChatGPT.

Fecha de uso: agosto de 2026.

Se utilizó como apoyo durante el desarrollo para:

- aclarar conceptos de Backtracking y poda;
- revisar errores de compilación y ejecución;
- recibir retroalimentación sobre organización y redacción;
- apoyar la preparación de documentación y procedimientos reproducibles;
- revisar la interpretación de resultados experimentales.

La implementación fue verificada mediante compilación, pruebas automáticas y ejecuciones experimentales registradas en el repositorio.

[PENDIENTE: Nicolás debe agregar aquí cualquier herramienta de IA que haya utilizado y para qué.]

---

# 18. Contribución individual de los integrantes

## Samuel Serna

La contribución principal de Samuel correspondió al módulo de Backtracking.

Realizó el modelamiento de la política y los estados parciales, la implementación del Backtracking con poda y de la versión sin poda, las pruebas automáticas, el mecanismo de límite de nodos, las configuraciones experimentales, la recolección de métricas, las comparaciones con y sin poda, la generación de gráficas, la documentación reproducible y la redacción del aporte de Backtracking para el informe.

La contribución detallada se encuentra en:

report/contribucion_samuel.md

## Nicolas Ortiz

[PENDIENTE NICOLAS: describir de forma específica su trabajo en Fuerza Bruta, experimentación, pruebas, documentación y demás contribuciones.]
