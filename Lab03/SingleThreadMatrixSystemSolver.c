#include <stddef.h>
#include <stdint.h>

// Ax = b
// Decompose A such that...
// A = P'LU
//      where L is the lower triangle
//      and U is the upper triangle
//      Solve, such that,
//          PA = LU
// LUx = Pb
// Solve Ly = Pb for y
//      y = L'Pb
// Solve Ux = y for x
//      x = U'L'Pb

double * matrixSystemSolver(double ** A, double * b, uint32_t length) {

}