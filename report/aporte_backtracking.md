# Aporte al informe - Módulo Backtracking

## Modelamiento

El módulo de Backtracking modela la construcción de contraseñas como un árbol de búsqueda. Cada estado representa un prefijo parcial de longitud k, mientras que el estado inicial corresponde a la cadena vacía. Los estados terminales son las cadenas que alcanzan la longitud n.

Para cada prefijo se mantienen contadores de caracteres minúsculos, mayúsculos, dígitos y símbolos. Estos contadores permiten determinar si todavía es posible satisfacer la política de contraseñas con las posiciones restantes.

La política obtenida para el equipo a partir de la semilla 1105 fue:

- minLower = 3
- minUpper = 2
- minDigit = 2
- minSymbol = 1
- prohibición de caracteres idénticos consecutivos

El alfabeto utilizado en la implementación fue:

abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%

Este conjunto contiene 67 caracteres. El enunciado indica un tamaño de alfabeto de 69, pero los conjuntos de caracteres enumerados explícitamente en el mismo documento suman 67. Por esta razón se utilizaron los caracteres especificados de forma explícita.

La salida principal de las ejecuciones experimentales corresponde al conteo de soluciones encontradas, junto con las métricas de nodos visitados, nodos podados y tiempo de ejecución.

---

## Diseño algorítmico

La solución utiliza construcción incremental mediante Backtracking.

En cada nivel del árbol se intenta agregar un carácter del alfabeto al prefijo actual. Después de agregarlo, se verifica si el estado todavía puede conducir a una solución válida.

La función de factibilidad calcula cuántos caracteres de cada categoría todavía hacen falta:

faltanLower = max(0, minLower - lower)
faltanUpper = max(0, minUpper - upper)
faltanDigit = max(0, minDigit - digit)
faltanSymbol = max(0, minSymbol - symbol)

Luego se suman estos valores y se comparan con el número de posiciones restantes.

Si:

faltantes > posicionesRestantes

el estado se considera inviable y se poda.

También se realiza una poda inmediata cuando la política prohíbe caracteres idénticos consecutivos y el nuevo carácter sería igual al último carácter del prefijo.

Cuando el prefijo alcanza longitud n, se verifica que cumpla todas las condiciones y, si es válido, se incrementa el número de soluciones.

Para efectos experimentales también se implementó una versión sin poda. Esta genera sistemáticamente todos los prefijos posibles hasta longitud n y solamente verifica las restricciones cuando alcanza un estado terminal.

---

## Pseudocódigo

### Backtracking con poda

BACKTRACKING(estado, politica, alfabeto)

    contar estado como visitado

    SI estado no es factible
        contar estado como podado
        RETORNAR
    FIN SI

    SI longitud(estado.prefijo) = politica.n
        SI estado es solucion valida
            soluciones = soluciones + 1
        FIN SI
        RETORNAR
    FIN SI

    PARA cada caracter c en alfabeto

        SI no se permiten repetidos consecutivos
           Y ultimoCaracter(prefijo) = c
            contar poda
            CONTINUAR
        FIN SI

        agregar c al estado

        SI estado es factible
            BACKTRACKING(estado, politica, alfabeto)
        SINO
            contar poda
        FIN SI

        retirar c del estado

    FIN PARA

FIN BACKTRACKING


### Enumeración sin poda

SIN_PODA(estado, politica, alfabeto)

    contar estado como visitado

    SI longitud(estado.prefijo) = politica.n
        SI estado es solucion valida
            soluciones = soluciones + 1
        FIN SI
        RETORNAR
    FIN SI

    PARA cada caracter c en alfabeto
        agregar c al estado
        SIN_PODA(estado, politica, alfabeto)
        retirar c del estado
    FIN PARA

FIN SIN_PODA

---

## Implementación

La implementación fue realizada en C++17.

Se definieron estructuras separadas para representar la política, el estado parcial, las métricas de ejecución y los límites experimentales.

La estructura de estado almacena:

- prefijo actual
- cantidad de minúsculas
- cantidad de mayúsculas
- cantidad de dígitos
- cantidad de símbolos

Mantener estos contadores evita recorrer nuevamente el prefijo completo para determinar su composición en cada llamada recursiva.

Las métricas almacenan:

- nodos visitados
- nodos podados
- soluciones encontradas
- tiempo de ejecución
- indicador de interrupción

También se agregó un límite máximo de nodos para las pruebas experimentales grandes. Este mecanismo evita que una instancia exponencial permanezca ejecutándose durante un tiempo no razonable y permite identificar experimentalmente el punto en que el problema deja de ser manejable.

Una ejecución interrumpida se marca explícitamente como parcial y sus soluciones no se reportan como el número total de soluciones de la instancia.

---

## Análisis de complejidad

Sea b = |Sigma| el tamaño del alfabeto y n la longitud de la contraseña.

En ausencia de poda, el árbol completo contiene:

1 + b + b^2 + ... + b^n

nodos.

La cantidad de hojas es:

b^n

Por tanto, la complejidad temporal de la enumeración exhaustiva es:

O(b^n)

En el peor caso, Backtracking tampoco puede garantizar una cota asintótica mejor. Si las restricciones no permiten descartar estados antes de llegar a las hojas, el algoritmo puede terminar recorriendo prácticamente el mismo espacio de búsqueda:

O(b^n)

La ventaja de Backtracking aparece cuando las restricciones permiten detectar prefijos inviables antes de alcanzar profundidad n.

Un ejemplo extremo ocurrió en la configuración equipo_n6. La política exige:

3 minúsculas + 2 mayúsculas + 2 dígitos + 1 símbolo = 8 caracteres mínimos.

Como n = 6, la política es imposible. La versión con poda detectó la inviabilidad desde la raíz y visitó solamente un nodo.

Por tanto, para esta instancia particular el comportamiento observado corresponde al mejor caso de poda temprana.

Respecto al espacio auxiliar, la implementación utiliza búsqueda en profundidad. El prefijo y la pila de recursión tienen como máximo profundidad n, por lo que el espacio auxiliar es:

O(n)

sin contar el almacenamiento de resultados, ya que experimentalmente se contabilizan las soluciones en lugar de almacenarlas todas.

---

## Casos de prueba

Se utilizaron pruebas unitarias y configuraciones experimentales.

La prueba pequeña empleó:

n = 4

alfabeto:

abAB12!

restricciones:

- mínimo 1 minúscula
- mínimo 1 mayúscula
- mínimo 1 dígito
- mínimo 1 símbolo
- sin caracteres idénticos consecutivos

Esta instancia pudo ejecutarse completamente tanto con poda como sin poda.

Ambas versiones encontraron exactamente:

192 soluciones

La versión con poda visitó:

356 nodos

La versión sin poda visitó:

2801 nodos

Esto permitió verificar que la poda no modifica el conjunto de soluciones.

También se probaron las configuraciones solicitadas para la política del equipo:

- política del equipo, n = 6
- política del equipo, n = 8
- política del equipo, n = 10
- política relajada, n = 8
- política sin restricciones de composición, n = 6

Adicionalmente se realizaron ejecuciones de calibración sobre la instancia de referencia.

---

## Metodología experimental

Las pruebas fueron compiladas con C++17 y optimización:

g++ -std=c++17 -O2

Las métricas de tiempo se obtuvieron utilizando std::chrono.

Para las instancias que no podían completarse razonablemente se estableció un máximo de nodos visitados. Cuando se alcanza este máximo la ejecución se marca como INTERRUMPIDO POR LIMITE.

Los resultados de estas ejecuciones se utilizan únicamente para estudiar el costo computacional. Los conteos de soluciones obtenidos en ejecuciones interrumpidas se consideran parciales.

Para garantizar reproducibilidad, las configuraciones, resultados y scripts utilizados para generar las gráficas se almacenan en las carpetas results/ y scripts/ del repositorio.

---

## Resultados

### Comparación completa de prueba pequeña

| Métrica | Con poda | Sin poda |
|---|---:|---:|
| Nodos visitados | 356 | 2801 |
| Soluciones | 192 | 192 |
| Tiempo | 0.033 ms | 0.063 ms |
| Reducción de nodos | 87.290% | - |

Las dos estrategias encontraron el mismo número de soluciones.

### Política del equipo con n = 6

Nodos teóricos del árbol exhaustivo:

91828963717

Con poda:

- nodos visitados: 1
- nodos podados: 1
- soluciones: 0
- estado: COMPLETADO

La versión sin poda fue ejecutada con límite de 1000000 de nodos y alcanzó dicho límite sin completar el árbol.

### Política del equipo con n = 8

Con límite de 10000000 de nodos:

- nodos visitados: 10000000
- nodos podados: 52628655
- soluciones parciales: 9065240
- tiempo: 423.289 ms
- estado: INTERRUMPIDO POR LIMITE

### Política del equipo con n = 10

Con límite de 10000000 de nodos:

- nodos visitados: 10000000
- nodos podados: 52628656
- soluciones parciales: 9065238
- tiempo: 431.392 ms
- estado: INTERRUMPIDO POR LIMITE

### Política relajada con n = 8

Con límite de 10000000 de nodos:

- nodos visitados: 10000000
- nodos podados: 151517
- soluciones parciales: 9848479
- tiempo: 130.724 ms
- estado: INTERRUMPIDO POR LIMITE

### Sin restricciones con n = 6

Con límite de 10000000 de nodos:

- nodos visitados: 10000000
- nodos podados: 0
- soluciones parciales: 9850742
- tiempo: 102.420 ms
- estado: INTERRUMPIDO POR LIMITE

---

## Calibración de la instancia de referencia

Los resultados obtenidos fueron:

| Nodos visitados | Tiempo |
|---:|---:|
| 100000 | 12.447 ms |
| 1000000 | 68.244 ms |
| 10000000 | 364.151 ms |
| 100000000 | 3168.311 ms |

Estas ejecuciones fueron limitadas y, por tanto, no representan el tiempo necesario para completar la instancia.

La gráfica bt_tiempo_vs_nodos.png representa estos datos utilizando escala logarítmica para el número de nodos.

---

## Análisis de resultados

Los experimentos muestran que la efectividad del Backtracking depende directamente de la capacidad de las restricciones para detectar estados inviables de forma temprana.

El resultado más claro aparece en equipo_n6. El árbol exhaustivo teórico contiene 91828963717 nodos, pero la versión con poda visita solamente la raíz porque la política requiere ocho caracteres mínimos y solamente existen seis posiciones.

En contraste, la configuración sin restricciones produjo cero nodos podados. En este caso el Backtracking pierde su ventaja sobre la enumeración exhaustiva porque ningún prefijo puede descartarse antes de alcanzar una hoja.

La política relajada también muestra este efecto. Para el mismo límite de 10000000 de nodos únicamente se registraron 151517 podas, mientras que equipo_n8 produjo 52628655 podas. Esto evidencia que una política más restrictiva ofrece mayores oportunidades para descartar ramas.

La prueba pequeña permitió comprobar experimentalmente que la poda mantiene la corrección del algoritmo. Ambas versiones encontraron 192 soluciones, pero la versión con poda visitó 356 nodos frente a 2801 de la enumeración sin poda, equivalente a una reducción de 87.290%.

Los resultados también concuerdan con el análisis teórico. El peor caso continúa siendo exponencial, pero la poda puede reducir considerablemente la parte del árbol que realmente se explora.

---

## Comparación con poda vs. sin poda

La comparación completa sobre la instancia pequeña mostró:

Con poda:
356 nodos

Sin poda:
2801 nodos

Reducción:
87.290%

Soluciones en ambas:
192

Por tanto, para esta instancia la poda redujo considerablemente el espacio recorrido sin cambiar el resultado.

En equipo_n6 el contraste fue todavía más fuerte. La versión con poda terminó después de visitar la raíz, mientras que la versión sin poda alcanzó un límite experimental de 1000000 de nodos.

Esta segunda comparación es parcial porque la versión exhaustiva no completó el árbol. Por esta razón se utiliza como evidencia del costo computacional y no como demostración de equivalencia de soluciones.

---

## Conclusiones del módulo Backtracking

El Backtracking permitió evitar la exploración completa del espacio cuando las restricciones proporcionaron información suficiente para detectar prefijos inviables.

La función de factibilidad basada en los requisitos faltantes y las posiciones restantes permitió realizar podas antes de alcanzar estados terminales.

La configuración equipo_n6 mostró el mejor caso observado, ya que una instancia imposible pudo descartarse desde el estado inicial.

Por otra parte, la configuración sin restricciones confirmó el peor escenario para la técnica: al no existir condiciones de factibilidad que descarten prefijos, el número de estados podados fue cero y el comportamiento se aproxima a la enumeración exhaustiva.

La comparación completa realizada sobre una instancia pequeña confirmó que la poda no altera las soluciones y redujo en 87.290% los nodos visitados.

Finalmente, las ejecuciones limitadas mostraron que el tamaño del espacio de búsqueda crece rápidamente. Por ello, para las instancias grandes fue necesario diferenciar explícitamente entre resultados completos y mediciones parciales de calibración.
