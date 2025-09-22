#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    // Inicializar MPI
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Verificar que tengamos al menos 2 procesos
    if (size < 2) {
        if (rank == 0) {
            std::cout << "Este programa requiere al menos 2 procesos" << std::endl;
        }
        MPI_Finalize();
        return -1;
    }
    
    const int ARRAY_SIZE = 5;
    std::vector<float> A(ARRAY_SIZE);
    std::vector<float> B(ARRAY_SIZE);
    
    // Llenar el arreglo A con el valor del rank
    for (int i = 0; i < ARRAY_SIZE; i++) {
        A[i] = static_cast<float>(rank);
    }
    
    std::cout << "Proceso " << rank << " - Arreglo A inicial: ";
    for (int i = 0; i < ARRAY_SIZE; i++) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
    
    // Sincronización para ordenar la salida
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        std::cout << "\nIniciando comunicación en anillo con " << size << " procesos..." << std::endl;
        std::cout << "Cada proceso envía su arreglo al siguiente en el anillo." << std::endl;
        std::cout << "Proceso " << (size-1) << " envía de vuelta al proceso 0.\n" << std::endl;
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Determinar el destino y origen para la comunicación en anillo
    int dest = (rank + 1) % size;  // Proceso siguiente en el anillo
    int source = (rank - 1 + size) % size;  // Proceso anterior en el anillo
    
    // Comunicación en anillo: cada proceso envía a la derecha y recibe de la izquierda
    MPI_Request send_request, recv_request;
    
    // Envío asíncrono al proceso siguiente
    MPI_Isend(A.data(), ARRAY_SIZE, MPI_FLOAT, dest, 0, MPI_COMM_WORLD, &send_request);
    
    // Recepción asíncrona del proceso anterior
    MPI_Irecv(B.data(), ARRAY_SIZE, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &recv_request);
    
    // Esperar a que se completen ambas operaciones
    MPI_Wait(&send_request, MPI_STATUS_IGNORE);
    MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
    
    // Sincronización antes de imprimir resultados
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Cada proceso imprime lo que recibió en B[]
    std::cout << "Proceso " << rank << " - Recibió de proceso " << source << ": ";
    for (int i = 0; i < ARRAY_SIZE; i++) {
        std::cout << B[i] << " ";
    }
    std::cout << std::endl;
    
    // Sincronización final
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        std::cout << "\nComunicación en anillo completada exitosamente." << std::endl;
        std::cout << "Cada proceso recibió el arreglo del proceso anterior en el anillo." << std::endl;
    }
    
    // Finalizar MPI
    MPI_Finalize();
    return 0;
}
