# Lab 12 - Project Application

## E1 - Filter and ownership
Identify selected and excluded IDs, their reasons, and who owns each object.
* **Registros seleccionados (Órdenes activas `DELIVERY`):**
  * **ID 420:(Cumple con tipo y estado activo)*.
  * **ID 180:(Cumple con tipo y estado activo)*.
  * **ID 310:(Cumple con tipo y estado activo)*.
  * **ID 205:(Cumple con tipo y estado activo)*.
* **Registros excluidos:**
  * **ID 650:(Excluido porque su tipo no es `DELIVERY`)*.
  * **ID 900:(Excluido porque su estado es `COMPLETED`, por lo que ya no esta activa)*.
*El arreglo original `orders` (declarado en el `main`) es el **propietario exclusivo** de los objetos `OrderPreview` en memoria.
*El arreglo `view` actúa como una (borrowed view), almacenando únicamente direcciones de memoria (`OrderPreview*`) de los elementos que pasaron el filtro, sin duplicar los objetos ni alterar su almacenamiento original.

## E2 - Report order and algorithm
List the observed report IDs and explain how each comparison rule affects them.
State the sorting algorithm used and its time and auxiliary-space bounds.
### 1. IDs observados en el reporte y aplicación de reglas de comparación
Aplicando el comparador que siguiere.
1. **ID 205** (`Biblioteca`, Prioridad 3)
2. **ID 310** (`Biblioteca`, Prioridad 3)
3. **ID 420** (`Biblioteca`, Prioridad 1)
4. **ID 180** (`Ingenieria`, Prioridad 3)
**Edificio ascendente:** Las ordenes de `"Biblioteca"` van primero alfabeticamente frente a `"Ingenieria"`.
- Prioridad descendente (dentro de "Biblioteca"): Las ordenes con prioridad 3 (`205` y `310`) se colocan antes que la de prioridad 1 (`420`).
- ID ascendente (en empate de edificio y prioridad): Entre `205` y `310` (ambas en `Biblioteca` con prioridad 3), el ID menor (`205`) aparece primero.

--2. En el caso de Algoritmo de ordenamiento y límites de complejidad--
* **Algoritmo utilizado:** **Quicksort** (adaptado para operar sobre un arreglo de punteros `OrderPreview*[]`).
* **Límite de tiempo (Time Complexity):** 
  * Caso promedio y mejor caso: O (n log n)
  * Peor caso: O(n^2)
* **Límite de espacio auxiliar (Auxiliary Space):** O(log n) debido a los marcos de pila recursivos (`call stack`), ya que solo se intercambian punteros y no las estructuras completas.

## E3 - Lookup choice and tests
Explain why this report cannot support binary search by orderId as currently sorted.
Binary Search no funcionaria porque la vista está ordenada bajo un criterio compuesto: **primero por edificio, luego por prioridad y finalmente por ID**. Por lo tanto, los valores de `orderId` no forman una secuencia (creciente) en todo el arreglo. La búsqueda binaria requiere una ordenación lineal completa, condición que esta vista no cumple. Por ello se emplea una **búsqueda lineal**.

Give one found, one excluded, and one nonexistent test with observed results.
***Caso encontrado:** Se busca el ID `310`.
  * *Resultado:* Devuelve la posición `1` de la vista y muestra correctamente los datos del edificio (`Biblioteca`), estado (`PENDING`) y prioridad (`3`).
***Caso excluido:** Se busca el ID `900`.
  * *Resultado:* Devuelve `-1` (no encontrado en la vista), ya que la orden fue filtrada al estar en estado `COMPLETED`.
***Caso inexistente:** Se busca el ID `999`.
  * *Resultado:* Devuelve `-1` porque el ID no existe en la colección.

  Describe what an additional ID-sorted view would cost and enable.
  **Cost:** Requeriria declarar y poblar un segundo arreglo de punteros, y ordenarlo mediante un comparador basado exclusivamente en `orderId`, con un costo de O(n log n) en tiempo de ordenamiento y almacenamiento adicional de O(n) punteros.
* **Enable:** Habilita el uso de **binary search** con un tiempo de ejecución logarítmico O(log n) para consultas rápidas por ID.

## E4 - Evidence and project connection
Source: delivery_report_practice.cpp
Output: delivery_report_output.txt
Explain how you verified that original records kept their order and field values.
**Evidencia:** Al invocar `displayOrders` antes de generar la vista y después de realizar el reporte y la consulta, se comprueba que ningún campo (ni ID, tipo, estado, edificio o prioridad) ni la posición original en el arreglo `orders` ha cambiado. 
Asimismo, no se ejecuta ninguna operacion `delete` sobre los punteros prestados porque apuntan a memoria estatica en la pila (stack) gestionada por el arreglo original, evitando errores de memoria.

Explain how this separation protects the operational FIFO sequence in the project.
Separar la lógica de presentación (filtrado y ordenamiento en vistas basadas en punteros) protege el arreglo operativo principal. Esto asegura que:
-La secuencia cronologica o de llegada original (**FIFO**) del sistema de órdenes no sufra alteraciones por requerimientos temporales de reportes.
-Se puedan mantener multiples vistas concurrentes (por ejemplo, una vista para la cocina ordenada por prioridad y otra para el despacho ordenada por edificio) sin alterar ni corromper los datos base de la coleccion central.