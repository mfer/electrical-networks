void lubksb(float **a, int n, int *indx, float b[])
/*Solves the set of n linear equations A·X = B. Here a[1..n][1..n] is input, not as the matrix
A but rather as its LU decomposition, determined by the routine ludcmp. indx[1..n] is input
as the permutation vector returned by ludcmp. b[1..n] is input as the right-hand side vector
B, and returns with the solution vector X. a, n, and indx are not modiﬁed by this routine
and can be left in place for successive calls with diﬀerent right-hand sides b. This routine takes
into account the possibility that b will begin with many zero elements, so it is eﬃcient for use
in matrix inversion.*/
{
	int i,ii=0,ip,j;
	float sum;
	for (i=1;i<=n;i++) { /*When ii is set to a positive value, it will become the
		index of the ﬁrst nonvanishing element of b. We now
		do the forward substitution, equation (2.3.6). The
		only new wrinkle is to unscramble the permutation
		as we go.*/
		ip=indx[i];
		sum=b[ip];
		b[ip]=b[i];
		if (ii)
		  for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
		else if (sum) ii=i; //A nonzero element was encountered, so from now on we
		b[i]=sum; //will have to do the sums in the loop above.
	}
	for (i=n;i>=1;i--) { //Now we do the backsubstitution, equation (2.3.7).
		sum=b[i];
		for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
		b[i]=sum/a[i][i]; //Store a component of the solution vector X.
	} //All done!
}
