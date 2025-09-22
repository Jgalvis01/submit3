# MPI Programming Exercises - Parallel Computing

Este repositorio contiene tres ejercicios de programación paralela usando MPI (Message Passing Interface) en C++.

## Requisitos

- Compilador compatible con MPI (mpic++)
- Biblioteca MPI instalada
- Sistema operativo compatible (Linux, Windows con MPI, macOS)

### Instalación de MPI en Windows
```bash
# Usando Microsoft MPI
# Descargar e instalar MS-MPI desde Microsoft
# O usar MinGW-w64 con MPICH
```

### Instalación de MPI en Linux
```bash
sudo apt-get install mpich libmpich-dev  # Ubuntu/Debian
# o
sudo yum install mpich mpich-devel       # CentOS/RHEL
```

## Ejercicios

### Ejercicio 1: Comunicación Punto a Punto (`MPI_Ssend.cpp`)

**Descripción:** Dos procesos se intercambian arreglos de floats usando `MPI_Send` y `MPI_Recv`.

**Funcionalidad:**
- Cada proceso llena un arreglo A[] con su rank
- Los procesos se intercambian sus arreglos
- Cada proceso recibe el arreglo del otro en B[]
- Imprime los valores recibidos

**Compilación y ejecución:**
```bash
mpic++ -o MPI_Ssend MPI_Ssend.cpp
mpirun -np 2 ./MPI_Ssend
```

**Salida esperada:**
```
Proceso 0 - Arreglo A: 0 0 0 0 0
Proceso 1 - Arreglo A: 1 1 1 1 1
Proceso 0 - Valor recibido B[0]: 1
Proceso 0 - Arreglo B completo: 1 1 1 1 1
Proceso 1 - Valor recibido B[0]: 0
Proceso 1 - Arreglo B completo: 0 0 0 0 0
```

### Ejercicio 2: Broadcast (`MPI_brodcast.cpp`)

**Descripción:** El proceso 0 inicializa una variable, la modifica y la difunde a todos los procesos usando `MPI_Bcast`.

**Funcionalidad:**
- Proceso 0 inicializa variable = 47
- Proceso 0 calcula el cuadrado (47² = 2209)
- Proceso 0 difunde la variable a todos los procesos
- Todos los procesos imprimen el valor recibido

**Compilación y ejecución:**
```bash
mpic++ -o MPI_brodcast MPI_brodcast.cpp
mpirun -np 4 ./MPI_brodcast
```

**Salida esperada:**
```
Proceso 0 - Variable inicial: 47
Proceso 0 - Variable modificada (cuadrado): 2209
Proceso 0 - Difundiendo variable a todos los procesos...
Proceso 0 - Variable recibida via broadcast: 2209
Proceso 1 - Variable recibida via broadcast: 2209
Proceso 2 - Variable recibida via broadcast: 2209
Proceso 3 - Variable recibida via broadcast: 2209
Broadcast completado exitosamente en 4 procesos.
```

### Ejercicio 3: Anillo de Procesos (`MPI_SsendExt.cpp`)

**Descripción:** Extensión del Ejercicio 1 donde múltiples procesos forman un anillo y cada uno envía su arreglo al siguiente proceso.

**Funcionalidad:**
- Funciona con cualquier número de procesos ≥ 2
- Cada proceso envía su arreglo A[] al proceso siguiente
- El último proceso (rank N-1) envía de vuelta al proceso 0
- Cada proceso recibe en B[] el arreglo del proceso anterior
- Usa comunicación asíncrona para evitar deadlocks

**Compilación y ejecución:**
```bash
mpic++ -o MPI_SsendExt MPI_SsendExt.cpp
mpirun -np 4 ./MPI_SsendExt
```

**Salida esperada:**
```
Proceso 0 - Arreglo A inicial: 0 0 0 0 0
Proceso 1 - Arreglo A inicial: 1 1 1 1 1
Proceso 2 - Arreglo A inicial: 2 2 2 2 2
Proceso 3 - Arreglo A inicial: 3 3 3 3 3

Iniciando comunicación en anillo con 4 procesos...
Cada proceso envía su arreglo al siguiente en el anillo.
Proceso 3 envía de vuelta al proceso 0.

Proceso 0 - Recibió de proceso 3: 3 3 3 3 3
Proceso 1 - Recibió de proceso 0: 0 0 0 0 0
Proceso 2 - Recibió de proceso 1: 1 1 1 1 1
Proceso 3 - Recibió de proceso 2: 2 2 2 2 2

Comunicación en anillo completada exitosamente.
```

## Conceptos de MPI Utilizados

### Funciones MPI Básicas
- `MPI_Init()` / `MPI_Finalize()`: Inicialización y finalización de MPI
- `MPI_Comm_rank()`: Obtener el rank del proceso
- `MPI_Comm_size()`: Obtener el número total de procesos

### Comunicación Punto a Punto
- `MPI_Send()`: Envío síncrono bloqueante
- `MPI_Recv()`: Recepción síncrona bloqueante
- `MPI_Isend()` / `MPI_Irecv()`: Comunicación asíncrona no bloqueante
- `MPI_Wait()`: Esperar a que se complete una operación asíncrona

### Comunicación Colectiva
- `MPI_Bcast()`: Difusión de datos desde un proceso a todos
- `MPI_Barrier()`: Sincronización de todos los procesos

## Estructura de Archivos

```
sumbit3/
├── MPI_Ssend.cpp      # Ejercicio 1: Comunicación punto a punto
├── MPI_brodcast.cpp   # Ejercicio 2: Broadcast
├── MPI_SsendExt.cpp   # Ejercicio 3: Anillo de procesos
└── README.md          # Este archivo
```

## Notas Importantes

1. **Número de procesos**: 
   - Ejercicio 1 requiere exactamente 2 procesos
   - Ejercicios 2 y 3 pueden ejecutarse con cualquier número ≥ 2

2. **Sincronización**: Se usan `MPI_Barrier()` para ordenar la salida y evitar intercalado de mensajes

3. **Prevención de deadlocks**: El Ejercicio 3 usa comunicación asíncrona para evitar bloqueos mutuos

4. **Tipos de datos**: Se utilizan `MPI_FLOAT` para arreglos y `MPI_INT` para variables enteras

## Compilación Alternativa

Si hay problemas con `mpic++`, también se puede compilar con:
```bash
g++ -o programa programa.cpp -lmpi
```

## Troubleshooting

- **Error "mpirun not found"**: Verificar que MPI esté instalado correctamente
- **Deadlock en Ejercicio 3**: Asegurar que se usa comunicación asíncrona
- **Salida desordenada**: Los `MPI_Barrier()` ayudan a sincronizar la salida
