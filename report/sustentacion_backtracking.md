# Preparación para sustentación — Backtracking

## 1. ¿Qué hace tu módulo?

Mi módulo genera contraseñas que cumplen una política utilizando Backtracking.

La contraseña se construye carácter por carácter. Antes de continuar explorando una rama se verifica si todavía es posible cumplir los requisitos mínimos con las posiciones restantes.

Si ya no es posible, esa rama se poda.

---

## 2. ¿Cuál fue la política de su equipo?

La semilla del equipo fue 1105.

La política principal fue:

n = 8
minLower = 3
minUpper = 2
minDigit = 2
minSymbol = 1

Además, no se permiten caracteres idénticos consecutivos.

---

## 3. ¿Qué representa un estado?

Un estado representa una contraseña parcial.

Contiene:

- el prefijo actual;
- número de minúsculas;
- número de mayúsculas;
- número de dígitos;
- número de símbolos.

Los contadores permiten comprobar la factibilidad sin tener que recorrer nuevamente toda la cadena.

---

## 4. ¿Cómo funciona la poda?

Primero calculo cuántos caracteres todavía hacen falta de cada categoría.

Por ejemplo:

faltanLower = max(0, minLower - lower)

Se hace lo mismo para mayúsculas, dígitos y símbolos.

Después sumo todos los caracteres que faltan.

Si hacen falta más caracteres que las posiciones disponibles:

faltantes > posicionesRestantes

la rama no puede producir una solución y se poda.

También se poda inmediatamente una opción cuando produce dos caracteres idénticos consecutivos y la política lo prohíbe.

---

## 5. ¿Por qué Backtracking es diferente de Fuerza Bruta?

Fuerza Bruta genera sistemáticamente todas las posibilidades.

Backtracking también puede tener un espacio exponencial, pero utiliza las restricciones del problema para evitar explorar ramas que ya se sabe que no pueden producir una solución.

---

## 6. ¿Cuál es la complejidad?

Si b es el tamaño del alfabeto y n la longitud:

el número de hojas posibles es:

b^n

Por lo tanto, el peor caso temporal es:

O(b^n)

Backtracking no cambia necesariamente la complejidad asintótica del peor caso.

Su ventaja está en que puede reducir mucho la cantidad real de estados explorados mediante poda.

El espacio auxiliar es:

O(n)

por la profundidad de la recursión y el prefijo actual.

---

## 7. ¿Cuál es el mejor ejemplo de poda de sus experimentos?

La configuración equipo_n6.

La política exige:

3 minúsculas
2 mayúsculas
2 dígitos
1 símbolo

En total se necesitan mínimo 8 caracteres.

Pero la contraseña tiene longitud n = 6.

Por lo tanto, es imposible satisfacer la política.

El algoritmo con poda lo detectó desde la raíz y visitó solamente 1 nodo.

---

## 8. ¿Qué pasó sin restricciones?

En la configuración sin_restricciones_n6 se obtuvieron:

0 nodos podados.

Eso ocurre porque no existe ninguna condición que permita descartar un prefijo.

En ese caso Backtracking se aproxima a una enumeración exhaustiva.

---

## 9. ¿Cómo comprobaron que la poda no cambia las soluciones?

Se utilizó una instancia pequeña que podía recorrerse completamente con ambas versiones.

Resultados:

Con poda:
356 nodos visitados.

Sin poda:
2801 nodos visitados.

Ambas encontraron:
192 soluciones.

Por lo tanto, la poda redujo el espacio explorado sin cambiar las soluciones.

---

## 10. ¿Cuál fue el porcentaje de reducción?

En la comparación pequeña:

Nodos sin poda = 2801
Nodos con poda = 356

Reducción:

((2801 - 356) / 2801) * 100

= 87.290%

---

## 11. ¿Por qué algunas pruebas aparecen como INTERRUMPIDO POR LIMITE?

Las instancias grandes tienen un espacio de búsqueda exponencial.

Se implementó un límite máximo de nodos para evitar ejecutar durante un tiempo no razonable.

Cuando se alcanza el límite, la ejecución se marca como:

INTERRUMPIDO POR LIMITE

Los resultados de soluciones de esas ejecuciones son parciales y nunca se presentan como el número total de soluciones.

---

## 12. ¿Qué métricas midieron?

Se registraron:

- nodos visitados;
- nodos podados;
- soluciones encontradas;
- tiempo de ejecución;
- si la ejecución fue completada o interrumpida.

---

## 13. ¿Cómo midieron el tiempo?

Se utilizó:

std::chrono

en C++.

El tiempo se registra en milisegundos.

---

## 14. ¿Qué significa nodos visitados?

Son los estados a los que realmente entró el algoritmo durante la exploración.

---

## 15. ¿Qué significa nodos podados?

Son estados u opciones que se descartan porque las restricciones permiten determinar que no vale la pena continuar explorándolos.

Podar una rama puede evitar visitar muchos descendientes.

Por eso nodos podados no necesariamente es igual a:

nodos sin poda - nodos con poda.

---

## 16. ¿Por qué usan un alfabeto de 67 si el documento dice 69?

El documento menciona 69 símbolos, pero enumera explícitamente:

26 minúsculas
26 mayúsculas
10 dígitos
5 símbolos

Eso suma 67.

La implementación utilizó exactamente los caracteres enumerados en el documento y la discrepancia fue documentada.

---

## 17. ¿Qué archivos implementaste?

Principalmente:

src/bt_backtracking.hpp
src/bt_backtracking.cpp
src/main.cpp
tests/test_backtracking.cpp

También preparé resultados, gráficas, documentación y el aporte de Backtracking para el informe.

---

## 18. ¿Qué hace estadoFactible?

Determina si un prefijo parcial todavía puede convertirse en una contraseña válida.

Compara los requisitos que faltan con las posiciones restantes.

Si no alcanzan las posiciones, retorna false y la rama se poda.

---

## 19. ¿Qué hace solucionValida?

Se utiliza cuando la contraseña alcanza longitud n.

Comprueba que:

- tenga la longitud correcta;
- cumpla mínimos de minúsculas;
- cumpla mínimos de mayúsculas;
- cumpla mínimos de dígitos;
- cumpla mínimos de símbolos;
- cumpla la restricción de repetición consecutiva.

---

## 20. ¿Qué aprendiste con los experimentos?

Que Backtracking no elimina el crecimiento exponencial en el peor caso.

Sin embargo, cuando las restricciones son fuertes, puede disminuir enormemente el espacio realmente explorado.

Cuando las restricciones son débiles o inexistentes, la poda pierde efectividad.

---

# Respuesta corta para explicar todo el módulo

Mi parte fue Backtracking para generación de contraseñas con restricciones.

El algoritmo construye la contraseña carácter por carácter y mantiene contadores de minúsculas, mayúsculas, dígitos y símbolos.

Antes de seguir una rama calculo si las posiciones restantes alcanzan para cumplir lo que todavía falta de la política. Si no alcanzan, podo la rama.

También implementé una versión sin poda para comparar.

En una prueba completa ambas versiones encontraron 192 soluciones, pero con poda se visitaron 356 nodos frente a 2801 sin poda, una reducción de 87.290%.

El peor caso sigue siendo O(b^n), pero experimentalmente se observa que las restricciones pueden reducir mucho el espacio recorrido.
