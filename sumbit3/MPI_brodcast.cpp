#include <mpi.h>
#include <iostream>
#include <cmath>

int main(int argc, char* argv[]) {
    // Inicializar MPI
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int variable;
    
    if (rank == 0) {
        // El proceso 0 inicializa la variable
        variable = 47;
        std::cout << "Proceso " << rank << " - Variable inicial: " << variable << std::endl;
        
        // El proceso 0 modifica la variable (calcula el cuadrado)
        variable = variable * variable;
        std::cout << "Proceso " << rank << " - Variable modificada (cuadrado): " << variable << std::endl;
        
        std::cout << "Proceso " << rank << " - Difundiendo variable a todos los procesos..." << std::endl;
    }
    
    // Sincronización para ordenar la salida
    MPI_Barrier(MPI_COMM_WORLD);
    
    // El proceso 0 difunde la variable a todos los procesos
    MPI_Bcast(&variable, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Cada proceso imprime la variable recibida
    std::cout << "Proceso " << rank << " - Variable recibida via broadcast: " << variable << std::endl;
    
    // Sincronización final para ordenar la salida
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        std::cout << "Broadcast completado exitosamente en " << size << " procesos." << std::endl;
    }
    
    // Finalizar MPI
    MPI_Finalize();
    return 0;
}
