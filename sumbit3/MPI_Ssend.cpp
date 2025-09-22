#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    // Inicializar MPI
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Verificar que tengamos exactamente 2 procesos
    if (size != 2) {
        if (rank == 0) {
            std::cout << "Este programa requiere exactamente 2 procesos" << std::endl;
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
    
    std::cout << "Proceso " << rank << " - Arreglo A: ";
    for (int i = 0; i < ARRAY_SIZE; i++) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
    
    // Comunicación punto a punto entre los dos procesos
    if (rank == 0) {
        // Proceso 0: envía A al proceso 1 y recibe B del proceso 1
        MPI_Send(A.data(), ARRAY_SIZE, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(B.data(), ARRAY_SIZE, MPI_FLOAT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank == 1) {
        // Proceso 1: recibe A del proceso 0 y envía A al proceso 0
        MPI_Recv(B.data(), ARRAY_SIZE, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(A.data(), ARRAY_SIZE, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
    }
    
    // Imprimir un valor del arreglo B recibido
    std::cout << "Proceso " << rank << " - Valor recibido B[0]: " << B[0] << std::endl;
    std::cout << "Proceso " << rank << " - Arreglo B completo: ";
    for (int i = 0; i < ARRAY_SIZE; i++) {
        std::cout << B[i] << " ";
    }
    std::cout << std::endl;
    
    // Finalizar MPI
    MPI_Finalize();
    return 0;
}
