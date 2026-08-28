# Contribución individual — Samuel Serna

Mi contribución principal al proyecto correspondió al módulo de Backtracking.

Realicé el modelamiento de la política de contraseñas a partir de la semilla del equipo y definí la representación de los estados parciales, incluyendo el prefijo construido y los contadores de caracteres minúsculos, mayúsculos, dígitos y símbolos.

Diseñé e implementé en C++17 el algoritmo de Backtracking con poda. La poda se basa en determinar si las posiciones restantes de una contraseña todavía permiten satisfacer los requisitos mínimos de la política. También se implementó el descarte inmediato de caracteres idénticos consecutivos cuando la política lo requiere.

Implementé una versión de exploración sin poda para comparar experimentalmente ambas estrategias y verificar que la poda no cambie las soluciones encontradas.

Desarrollé pruebas automáticas para validar:

- clasificación de caracteres;
- validación de soluciones;
- estados factibles e infactibles;
- equivalencia entre la versión con poda y sin poda en una instancia pequeña;
- funcionamiento del límite de nodos para experimentos grandes.

Realicé experimentos con:

- política del equipo, n = 6;
- política del equipo, n = 8;
- política del equipo, n = 10;
- política relajada, n = 8;
- configuración sin restricciones, n = 6;
- instancia de referencia.

También implementé un mecanismo de límite máximo de nodos para estudiar instancias exponenciales sin confundir ejecuciones parciales con resultados completos.

Registré las métricas de nodos visitados, nodos podados, soluciones encontradas y tiempos de ejecución.

En la comparación completa de prueba pequeña, ambas versiones encontraron 192 soluciones. La versión con poda visitó 356 nodos, mientras que la versión sin poda visitó 2801 nodos, obteniendo una reducción de 87.290%.

Preparé los archivos de resultados y el script utilizado para generar las gráficas experimentales del módulo de Backtracking.

Finalmente, redacté el aporte correspondiente a Backtracking para el informe, incluyendo modelamiento, diseño algorítmico, pseudocódigo, implementación, análisis de complejidad, metodología experimental, resultados, análisis y conclusiones.
