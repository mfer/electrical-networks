/* resolver o sistema de equações lineares
 * para obter a corrente de equilíbrio
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ludcmp.h"
#include "lubksb.h"

int main(int argc, char **argv) {	
	FILE *arq;
  if (argc < 3)
  {
    puts("Numero insuficiente de argumentos. Saindo...");
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
  float res;
  fscanf(arq,"%d %d",&vertices,&elos);
  //printf("%d %d\n",vertices,elos);
  
  
  float **a,*b,d;
	int i,j,**adj,*indx, n= vertices+1;	
	a = malloc (n*sizeof(float*));
	adj = malloc (n*sizeof(int*));
  if(a == NULL || adj == NULL)
	{
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	for(i = 0; i < n; i++)
	{
		a[i] = malloc (n*sizeof(float));
		adj[i] = malloc (n*sizeof(int));
		if(a[i] == NULL || adj[i] == NULL)
		{
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
	}  
	indx = (int *) malloc (n*sizeof(int));
	b = (float *) malloc (n*sizeof(float));
	
  for(i=0;i<n;++i)
  {
		b[i]=0.0;
		for(j=0;j<n;++j)
		{
		  a[i][j]=0.0;
		  adj[i][j]=0;
		}
	}    
	
  for(cont=0;cont<elos;++cont)
  {
		fscanf(arq,"%d %d %f",&noi, &noj, &res);
		//printf("%d %d %d\n",noi,noj,res);
		a[noi][noi] = a[noi][noi] - 1.0 / res;
		a[noj][noj] = a[noj][noj] - 1.0 / res;
		a[noi][noj] = a[noi][noj] + 1.0 / res;
		adj[noi][noj]=res;
		//printf("%f\n",a[noi][noj]);
	}
  for(i=1;i<n;++i)
  	for(j=i+1;j<n;++j)
		  a[j][i]=a[i][j];

	int fontsorv;
	fscanf(arq,"%d",&fontsorv);
	//printf("%d\n",fontsorv);
	float cor;
	for (cont=0;cont<fontsorv;++cont)
	{
		fscanf(arq,"%d %f",&noi,&cor);
		//printf("%d %f\n",noi,cor);
		b[noi]=cor;
	}
  
  for(i=1;i<n;++i)
  {
  	for(j=1;j<n;++j)
  	{
		  //printf("%f ",a[i][j]);
		}
		//printf(" - %f\n",b[i]);
	} 
	
	ludcmp(a,vertices,indx,&d);

	lubksb(a,vertices,indx,b);
	
	for(i=1;i<n;++i) printf("b[%d] = %f\n",i,b[i]);	
	
	for(i=1;i<n;++i)
  {
  	for(j=1;j<n;++j)
  	{
		  if(adj[i][j]!=0)
		    printf("%d-%d : %f\n",j,i,(b[j]-b[i])/adj[i][j]);
		}
	} 

	free(indx);
	free(b);
	for(i = 0; i < n; i++)
		free(a[i]);
	free(a);

	return 0;
}
