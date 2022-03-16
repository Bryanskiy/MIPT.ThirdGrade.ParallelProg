#include <iostream>
#include <gmpxx.h>
#include <mpi.h>
#include <string>

using bfloat =  mpf_class;
using bint   =  mpz_class;

int main(int argc, char** argv) {
   if(argc != 2) {
       printf("ERROR: expected 1 argument\n");
       exit(0);
   }

   int rank, commsize;   
   int n = atoi(argv[1]);
   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &commsize);
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

   bfloat partialSum{0};
   for(int i = rank; i < n; i += commsize) {
       partialSum += bfloat{1} / mpz_class::factorial(i);
   }

   // std::cout << "rank: " << rank << " partial sum: "  << partialSum << std::endl;

   if(rank) {
       mp_exp_t exp;
       std::string data = partialSum.get_str(exp, 10, 5);
       std::cout << data << std::endl;
       unsigned stringSize = data.size() + 1;

       int res = MPI_Send(&stringSize, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
       MPI_Send(data.c_str(), stringSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
   }
   else {
       bfloat ans = partialSum;
       int i = 1;
       for(; i < commsize; ++i) {
           unsigned stringSize;
           MPI_Recv(&stringSize, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);     

           char* str = new char[stringSize];
           MPI_Recv(str, stringSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

           partialSum.set_str(str, 10);
           ans += partialSum;
       }

       std::cout << "ANS: " << ans << std::endl;
    }
   MPI_Finalize();
   return 0;
}
