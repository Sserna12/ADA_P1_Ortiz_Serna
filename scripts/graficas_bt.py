import csv
import matplotlib.pyplot as plt


archivo = "results/bt_resultados_grafica.csv"

datos = []

with open(archivo, newline="", encoding="utf-8") as f:
    lector = csv.DictReader(f)

    for fila in lector:
        datos.append(
            {
                "configuracion": fila["configuracion"],
                "n": int(fila["n"]),
                "limite_nodos": int(fila["limite_nodos"]),
                "nodos_visitados": int(fila["nodos_visitados"]),
                "nodos_podados": int(fila["nodos_podados"]),
                "soluciones": int(fila["soluciones"]),
                "tiempo_ms": float(fila["tiempo_ms"]),
                "estado": fila["estado"],
            }
        )


# ==========================================================
# GRAFICA 1: TIEMPO VS TAMANO DE ENTRADA
# ==========================================================

equipo = [
    d
    for d in datos
    if d["configuracion"]
    in {"equipo_n6", "equipo_n8", "equipo_n10"}
]

equipo.sort(key=lambda x: x["n"])

x_n = [d["n"] for d in equipo]
y_tiempo = [d["tiempo_ms"] for d in equipo]

plt.figure(figsize=(8, 5))

plt.plot(
    x_n,
    y_tiempo,
    marker="o",
)

plt.xlabel("Longitud de la contraseña (n)")
plt.ylabel("Tiempo observado (ms)")
plt.title("Backtracking - Tiempo observado vs tamaño de entrada")

plt.xticks(x_n)
plt.grid(True, alpha=0.3)

for d in equipo:
    texto = (
        "completo"
        if d["estado"] == "COMPLETADO"
        else "límite 10M nodos"
    )

    plt.annotate(
        texto,
        (d["n"], d["tiempo_ms"]),
        textcoords="offset points",
        xytext=(5, 7),
    )

plt.tight_layout()

plt.savefig(
    "results/bt_tiempo_vs_n.png",
    dpi=200,
)

plt.close()


# ==========================================================
# GRAFICA 2: TIEMPO VS NODOS PROCESADOS
# ==========================================================

nodos_referencia = [
    100_000,
    1_000_000,
    10_000_000,
    100_000_000,
]

tiempos_referencia = [
    12.447,
    68.244,
    364.151,
    3168.311,
]

plt.figure(figsize=(8, 5))

plt.plot(
    nodos_referencia,
    tiempos_referencia,
    marker="o",
)

plt.xscale("log")

plt.xlabel("Nodos visitados (escala logarítmica)")
plt.ylabel("Tiempo (ms)")
plt.title("Backtracking - Tiempo vs nodos procesados")

plt.grid(True, alpha=0.3)

for x, y in zip(
    nodos_referencia,
    tiempos_referencia,
):
    plt.annotate(
        f"{y:.3f} ms",
        (x, y),
        textcoords="offset points",
        xytext=(5, 7),
    )

plt.tight_layout()

plt.savefig(
    "results/bt_tiempo_vs_nodos.png",
    dpi=200,
)

plt.close()


print("Graficas generadas correctamente:")
print("results/bt_tiempo_vs_n.png")
print("results/bt_tiempo_vs_nodos.png")
