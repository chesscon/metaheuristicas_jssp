
# Vecindades 

## Operaciones críticas
De acuerdo a la representación de solucionadas basadas en las gráficas disyuntivas, podemos considerar vecindades en las cuáles se garantice que los `vecinos` son soluciones factibles. Generalmente estas vecindades están basadas en la noción de operación crítica. 

Consideramos que una operación es crítica si pertenece a alguna ruta crítica, que se define como la ruta más larga desde la operación fitcia 0 (operación que marca el comienzo de todas las operaciones) hasta la operación fictia $N+1$ (operación que marca el fin de todas las operaciones); $N$ es el número total de operaciones. 

Se sabe que una operación es crítica si y sólo sí se cumple que:
 $$
 r_i + q_i = C_{max}
 $$

- $r_i$ es la ruta más larga desde la operación 0 hasta la operación $i$ (sin contarla); representa el tiempo más temprano en el que puede iniciar la operación $i$ (considerando que para que una operación pueda iniciar, sus predecesores en el job, y en la máquina correspondiente, ya deben haber finalizado).

- $q_i$ es la ruta más largada desde la operación $i$ (incluyendola) hasta la operación $N+1$; representa el tiempo del trabajo que está pendiente por realizarse (a partir de la operación $i$).

## Evaluación de vecindades
En general, podemos considerar dos tipos de evaluación:

- **Evaluación Exacta**. 
En este tipo de evaluación se recalcula el makespan para cada vecino; considerando que calcular el makespan tiene una complejidad de $O(N)$, y que en general las vecindades suelen tener en promedio $O(N)$ vecinos, evaluar una vecindad con esta estrategia suele tener una complejidad de $O(N^2)$.

- **Evaluación Estimada**. 
Dependiendo del tipo de vecindad que se tenga, se pueden considerar diferentes criterios para poder realizar una estimación del makespan de los vecinos. Calcular esta estimación suele tener una complejidad $O(1)$, por lo que evaluar una vecindad, con esta estrategia, se suele hacer en $O(N)$


## Vecindad N1
En el artículo de Taillard [2], se considera la vecindad N1, la cual consiste en intercambiar dos operaciones críticas consecutivas (en la misma máquina) .

```
CALCULAR VECINOS:
    Para cada maquina i:
    - Considerar los pares de operaciones que surgen en las rutas criticas:
    Para cada operacion j en la maquina i:
        - SI
            j es una operacion critica( es decir, si j.r + j.q == makespan)
            y ademas:
            j+1 es una operación crirtica
        ENTONCES:
                Agregar el par (j, j+1) a la lista de vecinos
```

Para evaluar a los vecinos, se puede considerar la siguiente estimación:

Si intercambiamos las operaciones $a, b \  (b = SM_{a} )$, la estimación es la siguiente:

$$
    C'_{max} = \max( r'_b + q'_b, r'a + q'_a )
$$
donde:
- $d_i$ es la duración de la $i-ésima$  operación

- $r'_b = \max( r_{PM_a} + d_{PM_a}, r_{PJ_b} + d_{PJ_b} )$

- $r'_a = \max( r'_b + d_{b}, r_{PJ_a} + d_{PJ_a} )$

- $q'_a = \max( q_{SM_b}, q_{SJ_a} ) + d_a$

- $q'_b = \max( q'_a, q_{SJ_b} ) + d_b$

