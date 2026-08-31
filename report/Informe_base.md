# 1. Portada

**Práctica 1 — Fuerza Bruta y Backtracking**

Curso: Análisis y Diseño de Algoritmos

Integrantes:

- Samuel Serna
- Nicolas Ortiz

Fecha: 30 de agosto de 2026

---

# 2. Introducción

Esta práctica estudia experimentalmente dos paradigmas algorítmicos: Fuerza Bruta y Backtracking. Ambos módulos trabajan sobre espacios de búsqueda asociados a contraseñas sintéticas, pero abordan problemas diferentes.

En el módulo de Fuerza Bruta se estudia el costo de encontrar una contraseña mediante enumeración sistemática y verificación por hash SHA-256. También se compara esta estrategia con una búsqueda basada en diccionario.

En el módulo de Backtracking se estudia la construcción incremental de contraseñas que satisfacen una política determinada, utilizando poda para evitar explorar estados que no pueden conducir a una solución válida.

El objetivo del trabajo es relacionar el análisis teórico de complejidad con mediciones experimentales de tiempo, candidatos o estados explorados y reducción del espacio de búsqueda.

---

# 3. Contexto del problema

## Fuerza Bruta

El problema consiste en recuperar una contraseña sintética a partir de su hash SHA-256. Como el hash no permite recuperar directamente la contraseña original, se generan candidatos, se calcula el hash de cada uno y se compara con el hash objetivo.

Se estudiaron dos formas de búsqueda.

La primera es Fuerza Bruta pura, que enumera sistemáticamente las cadenas pertenecientes al alfabeto y longitud definidos. Esta estrategia puede encontrar una contraseña si se encuentra dentro del espacio explorado, pero su costo crece exponencialmente.

La segunda estrategia utiliza un diccionario de 500 candidatos. Esta búsqueda tiene un espacio mucho menor y por ello requiere menos tiempo, pero solamente tiene éxito cuando la contraseña objetivo se encuentra dentro del diccionario.

## Backtracking

El módulo de Backtracking considera el problema de generar contraseñas que satisfacen una política de composición. Una contraseña válida debe cumplir determinados mínimos de minúsculas, mayúsculas, dígitos y símbolos, además de restricciones adicionales como la prohibición de caracteres idénticos consecutivos.

En lugar de generar primero todas las cadenas posibles y comprobarlas al final, Backtracking construye cada contraseña de forma incremental y descarta anticipadamente los prefijos que no pueden convertirse en una solución válida.

---

# 4. Fundamentación teórica

## Fuerza Bruta

Fuerza Bruta realiza una exploración exhaustiva del espacio de candidatos.

Si el alfabeto tiene tamaño b y todas las contraseñas tienen longitud n, existen:

b^n

candidatos posibles.

Al aumentar n, el espacio crece exponencialmente. Esto significa que pequeños incrementos en la longitud producen incrementos muy grandes en la cantidad de candidatos y en el tiempo requerido.

Para cada candidato se calcula su SHA-256 y se compara con el hash objetivo.

La búsqueda mediante diccionario sigue una estrategia diferente. En lugar de enumerar todas las cadenas posibles, prueba únicamente las palabras almacenadas en un archivo. Su costo depende del número de elementos del diccionario, pero no garantiza encontrar la contraseña.

## Backtracking

Backtracking recorre un árbol de decisiones mediante búsqueda en profundidad. Cada nodo representa una solución parcial, cada arista corresponde a la elección de un nuevo carácter y las hojas representan cadenas completas o ramas descartadas.

La principal diferencia frente a la enumeración exhaustiva consiste en la poda. Si un estado parcial no puede satisfacer las restricciones con las posiciones que todavía quedan disponibles, esa rama se descarta sin continuar explorándola.

En el peor caso, si ninguna restricción permite podar anticipadamente, Backtracking puede recorrer prácticamente el mismo espacio que la enumeración exhaustiva.

---

# 5. Modelamiento

## Fuerza Bruta

Una instancia de Fuerza Bruta está definida principalmente por:

- hash SHA-256 objetivo;
- alfabeto utilizado;
- longitud mínima;
- longitud máxima.

Se utilizaron dos alfabetos:

- A1: 26 letras minúsculas.
- A2: 26 letras minúsculas y 10 dígitos, para un total de 36 caracteres.

La búsqueda genera candidatos en el espacio definido y termina cuando encuentra una cadena cuyo SHA-256 coincide con el hash objetivo o cuando se agota el espacio.

Para la búsqueda por diccionario, el estado está determinado por el hash objetivo y el archivo con los 500 candidatos.

Las instancias privadas del equipo se generan a partir de los apellidos normalizados y ordenados:

ortiz + serna

La suma de sus códigos ASCII produce la semilla:

1105

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

La búsqueda exhaustiva genera sistemáticamente cada candidato posible para el alfabeto y longitud seleccionados.

Para cada candidato:

1. Se calcula el SHA-256.
2. Se compara contra el hash objetivo.
3. Si coincide, se detiene la búsqueda y se reporta la contraseña.
4. Si no coincide, se continúa con el siguiente candidato.

Para la búsqueda por diccionario se recorren secuencialmente las 500 palabras almacenadas en el archivo. Para cada palabra también se calcula el SHA-256 y se compara con el objetivo.

Adicionalmente, se implementó la generación reproducible de instancias privadas a partir de la semilla del equipo y un modo de experimentación por lotes utilizando un archivo CSV.

## Backtracking

El algoritmo construye la contraseña un carácter a la vez.

Antes de expandir un estado se verifica su factibilidad. Se calcula cuántos caracteres de cada categoría todavía hacen falta y se compara esa cantidad con las posiciones restantes.

Si los caracteres faltantes superan las posiciones disponibles, el estado es inviable y se poda.

También se evita generar un hijo cuando el carácter propuesto es igual al último carácter del prefijo y la política prohíbe repeticiones consecutivas.

Se implementó adicionalmente una versión sin poda para realizar la comparación experimental.

---

# 7. Pseudocódigo

## Fuerza Bruta

FUERZA_BRUTA(hashObjetivo, alfabeto, n)

    PARA cada candidato de longitud n
        hashCandidato = SHA256(candidato)

        SI hashCandidato = hashObjetivo
            RETORNAR candidato
        FIN SI
    FIN PARA

    RETORNAR no encontrado

FIN FUERZA_BRUTA


DICCIONARIO(hashObjetivo, archivo)

    PARA cada palabra del archivo
        hashPalabra = SHA256(palabra)

        SI hashPalabra = hashObjetivo
            RETORNAR palabra
        FIN SI
    FIN PARA

    RETORNAR no encontrado

FIN DICCIONARIO

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

El módulo fue desarrollado en C++17.

Sus archivos principales son:

- src/fb_core.cpp
- src/fb_core.hpp
- src/fb_dictionary.cpp
- src/fb_dictionary.hpp
- src/fb_instancias.cpp
- src/fb_instancias.hpp
- src/third_party/picosha2.h
- tests/test_fb.cpp

`fb_core` contiene la generación exhaustiva y el cálculo/verificación de hashes.

`fb_dictionary` implementa la búsqueda sobre el archivo de candidatos.

`fb_instancias` implementa la normalización de apellidos, cálculo de semilla y generación de instancias privadas.

El programa principal permite ejecutar los subcomandos `brute`, `dict`, `seed` y `experiment`.

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

Para un alfabeto de tamaño b y longitud n existen:

b^n

candidatos.

Por tanto, la complejidad temporal de la exploración exhaustiva es:

O(b^n)

considerando como constante el tamaño del hash calculado para cada candidato.

Si se explora un rango de longitudes desde min hasta max, el número de candidatos es:

b^min + b^(min+1) + ... + b^max

El ataque por diccionario con d palabras tiene complejidad:

O(d)

respecto al número de candidatos del diccionario.

En esta práctica d = 500.

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

Se utilizaron 13 configuraciones experimentales.

Para A1 se evaluaron:

- n = 3
- n = 4
- n = 5
- n = 6

Para A2 se evaluaron:

- n = 3
- n = 4
- n = 5

También se utilizó la instancia de referencia asociada a la contraseña:

abc12

con A2 y n = 5.

Finalmente se probaron cinco instancias privadas generadas con la semilla 1105.

Las pruebas automáticas verifican:

- cálculo SHA-256;
- búsqueda por Fuerza Bruta;
- instancia de referencia;
- búsqueda por diccionario;
- cálculo de semilla;
- generación de las cinco instancias privadas.

Todas las pruebas automáticas del módulo pasaron correctamente.

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

Todas las pruebas automáticas de Backtracking pasaron correctamente.

---

# 11. Experimentación

## Fuerza Bruta

Las mediciones fueron almacenadas en:

results/fb_tiempos.csv

Los principales resultados del barrido fueron:

| Configuración | Espacio teórico | Candidatos FB | Tiempo FB |
|---|---:|---:|---:|
| A1, n=3 | 17,576 | 17,576 | 16.357 ms |
| A1, n=4 | 456,976 | 456,976 | 483.164 ms |
| A1, n=5 | 11,881,376 | 11,881,376 | 11,296.6 ms |
| A1, n=6 | 308,915,776 | 308,915,776 | 323,710 ms |
| A2, n=3 | 46,656 | 46,656 | 43.040 ms |
| A2, n=4 | 1,679,616 | 1,679,616 | 1,577.59 ms |
| A2, n=5 | 60,466,176 | 60,466,176 | 57,886.8 ms |

Las gráficas obtenidas fueron:

- results/fb_tiempo_vs_n.png
- results/fb_tiempo_vs_candidatos.png

La instancia A1 con n = 6 tardó aproximadamente 323.7 segundos, equivalentes a 5.4 minutos. Este resultado muestra el punto en que el costo comienza a ser poco manejable para la experimentación local.

En A2 el crecimiento es todavía mayor porque el alfabeto contiene 36 caracteres.

## Backtracking

Las pruebas fueron compiladas con:

g++ -std=c++17 -O2

El tiempo fue medido mediante `std::chrono`.

En la comparación completa pequeña se obtuvo:

| Métrica | Con poda | Sin poda |
|---|---:|---:|
| Nodos visitados | 356 | 2801 |
| Soluciones | 192 | 192 |
| Tiempo última ejecución | 0.032 ms | 0.085 ms |
| Reducción de nodos | 87.290% | - |

Para las instancias grandes se realizaron ejecuciones limitadas, manteniendo explícitamente su estado como `INTERRUMPIDO POR LIMITE`.

Las gráficas generadas son:

- results/bt_tiempo_vs_n.png
- results/bt_tiempo_vs_nodos.png

---

# 12. Resultados

## Fuerza Bruta

La Fuerza Bruta encontró correctamente las 13 contraseñas objetivo utilizadas en la experimentación.

En la referencia `abc12` con A2 y n = 5 se evaluaron 50,249 candidatos y se obtuvo un tiempo de 49.1765 ms.

Las cinco instancias privadas también fueron encontradas:

| Instancia | Alfabeto | n | Candidatos FB | Tiempo |
|---|---|---:|---:|---:|
| equipo_1 | A1 | 4 | 244,917 | 226.558 ms |
| equipo_2 | A2 | 4 | 833,017 | 791.850 ms |
| equipo_3 | A1 | 5 | 11,682,276 | 11,049.9 ms |
| equipo_4 | A2 | 5 | 4,262,007 | 4,012.32 ms |
| equipo_5 | A1 | 6 | 130,784,415 | 125,835 ms |

La Fuerza Bruta obtuvo:

13 de 13 objetivos encontrados.

Tasa de éxito:

100%.

El diccionario evaluó 500 candidatos para cada instancia y no encontró ninguna de las 13 contraseñas.

Tasa de éxito del diccionario:

0%.

Los tiempos del diccionario permanecieron aproximadamente entre 0.46 y 0.68 ms.

## Backtracking

### Equipo n = 6

Nodos teóricos sin poda: 91,828,963,717.

Con poda:

- nodos visitados: 1;
- nodos podados: 1;
- soluciones: 0;
- estado: COMPLETADO.

La política exige ocho caracteres mínimos, pero n = 6, por lo cual la instancia es imposible.

### Equipo n = 8

Límite: 10,000,000 nodos.

- nodos visitados: 10,000,000;
- nodos podados: 52,628,655;
- soluciones parciales: 9,065,240;
- tiempo observado: 423.289 ms;
- estado: INTERRUMPIDO POR LIMITE.

### Equipo n = 10

Límite: 10,000,000 nodos.

- nodos visitados: 10,000,000;
- nodos podados: 52,628,656;
- soluciones parciales: 9,065,238;
- tiempo observado: 431.392 ms;
- estado: INTERRUMPIDO POR LIMITE.

### Política relajada n = 8

- nodos visitados: 10,000,000;
- nodos podados: 151,517;
- soluciones parciales: 9,848,479;
- tiempo observado: 130.724 ms;
- estado: INTERRUMPIDO POR LIMITE.

### Sin restricciones n = 6

- nodos visitados: 10,000,000;
- nodos podados: 0;
- soluciones parciales: 9,850,742;
- tiempo observado: 102.420 ms;
- estado: INTERRUMPIDO POR LIMITE.

---

# 13. Análisis de resultados

## Fuerza Bruta

Los resultados experimentales muestran claramente el crecimiento exponencial de Fuerza Bruta.

En A1, pasar de n = 3 a n = 6 incrementa el espacio desde 17,576 hasta 308,915,776 candidatos.

El tiempo medido aumentó de aproximadamente 16.4 ms a 323.7 segundos.

A2 presenta un crecimiento todavía mayor. Con n = 5 ya existen 60,466,176 combinaciones posibles y la ejecución completa tomó aproximadamente 57.9 segundos.

El caso A2 con n = 6 tendría:

36^6 = 2,176,782,336

candidatos posibles, por lo que el costo esperado sería considerablemente superior y ya resulta poco conveniente para una ejecución exhaustiva local.

Los resultados empíricos son coherentes con la complejidad O(b^n): al crecer la longitud o el tamaño del alfabeto, aumenta rápidamente tanto el número de candidatos como el tiempo requerido.

El diccionario mostró el comportamiento contrario. Las búsquedas terminaron en menos de un milisegundo aproximadamente porque solo se evaluaron 500 candidatos. Sin embargo, ninguna contraseña objetivo se encontraba en el archivo, por lo que su tasa de éxito fue 0%.

Esto muestra el intercambio entre cobertura y costo: Fuerza Bruta tiene un espacio mucho mayor pero puede encontrar cualquier contraseña contenida en ese espacio; el diccionario es mucho más rápido pero depende completamente de que el objetivo pertenezca a su conjunto de candidatos.

## Backtracking

Los resultados confirman que la efectividad de la poda depende de la capacidad de las restricciones para detectar estados inviables.

El caso `equipo_n6` representa un escenario extremo. La suma de requisitos mínimos es ocho y solamente existen seis posiciones. La versión con poda detectó esta condición desde la raíz y visitó un único nodo.

En el extremo contrario, la configuración sin restricciones produjo cero nodos podados. En ese escenario, Backtracking no dispone de información que permita descartar ramas y su comportamiento se aproxima al de una enumeración exhaustiva.

La política relajada produjo significativamente menos poda que la política principal del equipo.

La comparación completa pequeña mostró que ambas versiones encuentran exactamente las mismas 192 soluciones, mientras que la poda reduce los nodos visitados de 2801 a 356, equivalente a 87.290%.

Las ejecuciones de n = 8 y n = 10 alcanzaron el límite establecido de 10 millones de nodos. Por esta razón sus cantidades de soluciones son parciales y no se presentan como totales.

---

# 14. Comparación algorítmica

## Fuerza Bruta pura vs. diccionario

La Fuerza Bruta encontró las 13 contraseñas utilizadas en los experimentos, por lo que obtuvo una tasa de éxito del 100%.

El diccionario encontró 0 de 13 objetivos, para una tasa de éxito del 0%.

Sin embargo, la búsqueda por diccionario evaluó solamente 500 candidatos en cada ejecución y tardó aproximadamente entre 0.46 y 0.68 ms.

Fuerza Bruta necesitó desde miles hasta cientos de millones de candidatos dependiendo de la configuración.

Por tanto, el diccionario puede ser mucho más rápido cuando contiene la contraseña objetivo, pero no ofrece cobertura completa del espacio.

Fuerza Bruta es exhaustiva dentro del alfabeto y longitud definidos, aunque su costo crece exponencialmente.

## Backtracking con poda vs. sin poda

En la prueba completa pequeña:

- con poda: 356 nodos;
- sin poda: 2801 nodos;
- soluciones en ambas versiones: 192;
- reducción: 87.290%.

Esto muestra que la poda conserva las soluciones y reduce el espacio efectivamente recorrido.

En `equipo_n6`, la versión con poda terminó después de visitar la raíz. La versión sin poda alcanzó un límite experimental de 1,000,000 de nodos sin completar el árbol.

Esta segunda comparación se considera parcial y se utiliza solamente como evidencia del costo computacional.

Los resultados permiten concluir que la principal ventaja de Backtracking frente a una enumeración exhaustiva aparece cuando existen restricciones capaces de detectar anticipadamente ramas que no pueden producir soluciones válidas.

---

# 15. Conclusiones

Los experimentos realizados permitieron comprobar en la práctica el crecimiento exponencial de los espacios de búsqueda.

En Fuerza Bruta, el aumento de la longitud y del tamaño del alfabeto incrementó de manera considerable tanto el número de candidatos como el tiempo de ejecución. A1 con n = 6 requirió aproximadamente 5.4 minutos para recorrer 308,915,776 candidatos, mostrando un punto donde el costo ya comienza a ser poco manejable.

La búsqueda por diccionario fue considerablemente más rápida, pero ninguna de las contraseñas objetivo pertenecía al conjunto de 500 candidatos. Esto demuestra que su eficiencia temporal se obtiene a cambio de una menor cobertura.

Backtracking permitió reducir considerablemente el espacio explorado cuando las restricciones hicieron posible detectar prefijos inviables.

La implementación también mostró que la técnica no mejora automáticamente todos los casos: en ausencia de restricciones, el número de estados podados fue cero.

La comparación completa de Backtracking confirmó que la poda mantiene la corrección del resultado, ya que ambas versiones encontraron 192 soluciones, mientras que la versión con poda redujo en 87.290% los nodos visitados.

En conjunto, la práctica permitió observar que el diseño de una estrategia de búsqueda no depende únicamente de su complejidad teórica. Las restricciones disponibles, la estructura de los datos y el tamaño real del espacio influyen directamente en su comportamiento experimental.

---

# 16. Referencias

- Material del curso de Análisis y Diseño de Algoritmos.
- Enunciado de la Práctica 1 — Fuerza Bruta y Backtracking.
- PicoSHA2, biblioteca SHA-256 incluida en `src/third_party/picosha2.h`.
- Archivo de licencia de PicoSHA2 incluido en `src/third_party/LICENSE_picosha2.txt`.
- Documentación estándar de C++17 utilizada para estructuras, manejo de archivos y medición de tiempos con `std::chrono`.

---

# 17. Uso de herramientas de IA

Herramienta: OpenAI ChatGPT.

Fecha de uso: agosto de 2026.

Se utilizó como herramienta de apoyo durante el desarrollo para:

- aclarar conceptos de Backtracking y poda;
- apoyar la revisión de errores de compilación y ejecución;
- recibir retroalimentación sobre organización y documentación;
- apoyar la preparación de procedimientos reproducibles;
- revisar la interpretación de resultados experimentales;
- apoyar la integración final de los módulos de Fuerza Bruta y Backtracking;
- apoyar la organización y redacción del informe.

Todo el código integrado fue posteriormente verificado mediante compilación, pruebas automáticas y ejecuciones experimentales registradas en el repositorio.

No se atribuyen a Nicolás Ortiz herramientas de IA adicionales que no hayan sido declaradas por él.

---

# 18. Contribución individual de los integrantes

## Samuel Serna

La contribución principal de Samuel correspondió al módulo de Backtracking.

Realizó el modelamiento de la política y los estados parciales, la implementación del Backtracking con poda y de la versión sin poda, las pruebas automáticas, el mecanismo de límite de nodos, las configuraciones experimentales, la recolección de métricas, las comparaciones con y sin poda, la generación de gráficas, la documentación reproducible y la redacción del aporte de Backtracking para el informe.

También participó en la integración final de los módulos y en la verificación conjunta del proyecto.

La contribución detallada se encuentra en:

report/contribucion_samuel.md

## Nicolas Ortiz

La contribución principal de Nicolás correspondió al módulo de Fuerza Bruta.

Realizó la implementación de la búsqueda exhaustiva, la búsqueda por diccionario, la integración del cálculo SHA-256 mediante PicoSHA2, el cálculo de la semilla del equipo, la generación de las instancias privadas, las pruebas automáticas del módulo, la ejecución de las configuraciones experimentales, el registro de candidatos y tiempos, la generación de las gráficas de Fuerza Bruta y la documentación asociada al módulo.

También aportó los archivos de recursos y resultados utilizados durante la integración final del proyecto.