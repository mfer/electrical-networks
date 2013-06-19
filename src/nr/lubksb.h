/*Solves the set of n linear equations A·X = B. Here a[1..n][1..n] is input, not as the matrix
A but rather as its LU decomposition, determined by the routine ludcmp. indx[1..n] is input
as the permutation vector returned by ludcmp. b[1..n] is input as the right-hand side vector
B, and returns with the solution vector X. a, n, and indx are not modiﬁed by this routine
and can be left in place for successive calls with diﬀerent right-hand sides b. This routine takes
into account the possibility that b will begin with many zero elements, so it is eﬃcient for use
in matrix inversion.*/
void lubksb(double **a, int n, int *indx, double b[]);
