/* resolver o sistema de equações lineares
 * para obter a corrente de equilíbrio
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ludcmp.h"
#include "lubksb.h"
#define TINY 1.0e-20 //A small number.

int main(int argc, char **argv) {	
	FILE *arq;
  if (argc < 3)
  {
    puts("Numero insuficiente de argumentos. Saindo...");
    printf("Uso: %s [input] [output]\n",argv[0]);
    exit(1);
  }
  arq = fopen(argv[1], "rb");
  if (arq == NULL)
  {
    puts("Erro ao tentar abrir arquivo de entrada. Saindo...");
    exit(2);
  }

  FILE *out;
  out = fopen(argv[2], "wb");
  if (out == NULL)
  {
    puts("Erro ao tentar abrir arquivo de saída. Entrando...");
    exit(3);
  }

  int cont, vertices, elos, noi, noj;
  fscanf(arq,"%d %d",&vertices,&elos);
  //printf("%d %d\n",vertices,elos);
 
  double **a,**adj,*b,d;
	int i,j,*indx;
	int n = vertices+1;	
	//printf("%d\n",n);

		
	a = malloc (n*sizeof(double*));
	adj = malloc (n*sizeof(double*));

  if(a == NULL || adj == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	for(i = 0; i < n; i++)
	{
		a[i] = malloc (n*sizeof(double));
		adj[i] = malloc (n*sizeof(double));
		if(a[i] == NULL || adj[i] == NULL)
		{
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
	}
	indx = (int *) malloc (n*sizeof(int));
	b = (double *) malloc (n*sizeof(double));



  for(i=0;i<n;++i)
  {
		b[i]=0.0;
		indx[i]=0;
		for(j=0;j<n;++j)
		{
		  a[i][j]=0.0;
		  adj[i][j]=0.0;
		}
	}    


  double resistencia=0.0; //apenas inicializando
  int int_resistencia=0;
  for(cont=0;cont<elos;++cont)
  {
		fscanf(arq,"%d %d %d",&noi, &noj, &int_resistencia);
		//printf("%d %d %d\n",noi,noj,int_resistencia);
		if (int_resistencia == 0) resistencia = TINY;
		else resistencia = (double) int_resistencia;
		//printf("%f\n",resistencia);
    a[noi][noi] = a[noi][noi] - 1.0 / resistencia;
	  a[noj][noj] = a[noj][noj] - 1.0 / resistencia;
		a[noi][noj] = a[noi][noj] + 1.0 / resistencia;
		adj[noi][noj]=resistencia;
		//printf("%f\n",a[noi][noj]);		
	}


  for(i=1;i<n;++i)
  	for(j=i+1;j<n-1;++j)
		  a[j][i]=a[i][j];


	int fontsorv;
	fscanf(arq,"%d",&fontsorv);
	//printf("%d\n",fontsorv);
	double corrente=0.0;
	int int_corrente=0;
	for (cont=0;cont<fontsorv;++cont)
	{
		fscanf(arq,"%d %d",&noi,&int_corrente);
		//printf("%d %d\n",noi,int_corrente);
		corrente = (double) int_corrente;
		//printf("%d %f\n",noi,corrente);
		b[noi]=corrente;
	}


/*
  for(i=1;i<n;++i)
  {
  	for(j=1;j<n;++j)
  	{
		  printf("%f ",a[i][j]);
		}
		printf(" _ %f\n",b[i]);
	} 
*/


	ludcmp(a,vertices,indx,&d);
	lubksb(a,vertices,indx,b);
	

	for(i=1;i<n;++i) fprintf(out,"%d %f\n",i,b[i]);	
	for(i=1;i<n;++i)
  {
  	for(j=1;j<n;++j)
  	{
		  if(adj[i][j] != 0)
		    fprintf(out,"%d %d %f\n",i,j,(b[j]-b[i])/adj[i][j]);
		}
	} 



	free(indx);
	free(b);
	for(i = 0; i < n; i++)
	{
		free(a[i]);
		free(adj[i]);
	}
	free(a);
	free(adj);

	
	fclose(out);
	fclose(arq);

	return 0;
}
