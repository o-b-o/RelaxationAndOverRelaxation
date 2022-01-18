/// intentamo implementar sobrerelajacion

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>

#define nmax 150
#define tsteps 1000

// distancia del cuadrado a la pared (asumiendo que esta centrado)
#define cparam 48
#define alfa 1.9

double drand ( double low, double high )
{
    srand((unsigned int)clock());
     return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}



int main() {


  
  FILE*arch=fopen("rel.txt","w");
  FILE *gnuplot=popen("gnuplot","w");
  fprintf(gnuplot, "set pm3d\n");
  //fprintf(gnuplot, "set view map\n");
  
  fprintf(gnuplot, "set cntrparam levels incr -1,0.1,1\n");
  fprintf(gnuplot, "set contour\n");
  fprintf(gnuplot, "set term gif animate size 800,800 delay 2 \n");
  //  fprintf(gnuplot, "set size ratio -1\n");  
  fprintf(gnuplot, "set size square\n");  
  fprintf(gnuplot, "set output 'hippy2.gif'\n");


  fprintf(gnuplot, "set title 'a'\n");




  // necesitamos una matriz con condiciones iniciales en la frontera
  // podemos incluir condiciones fijas dentro tambien
  double v[nmax][nmax];
  double foo;
  
  for (int i=0; i<nmax; i++)
    {
      for (int j=0; j<nmax;j++)
	{

	  v[i][j]=drand(-1,1);
	}
    }
  
  //for printing result                                            
  
  printf("la matriz inicial es: \n");
  for(int k = 0; k<tsteps;k++)
    {
      printf("[ iteracion %d / %d] \n",k,tsteps);
      fprintf(gnuplot, "splot '-' pt -1 ps 1.25\n");  
      //fprintf(gnuplot, "splot '-' w lines lw 3\n");  
      for(int i=0;i<nmax;i++)
	{

	  for(int j=0;j<nmax;j++)
	    {
	      //printf("%lf\t",v[i][j]);
	      fprintf(arch, "%d %d %lf\n",i,j,v[i][j]);	  
	      fprintf(gnuplot, "%d %d %lf\n",i,j,v[i][j]);
	      // condiciones de fronteras
	      if (i == 0)
		{
		  if (j==0)
		    {
		      // esquina superior izquierda
		      foo =(v[i+1][j]+v[i][j+1])/2;
		      foo -= v[i][j];
		      foo*= alfa;
		      v[i][j]+=foo;
		    }
		  else if (j==nmax-1)
		    {
		      // esquina superior derecha
		      foo=(v[i+1][j]+v[i][j-1])/2;
		      foo -= v[i][j];
		      foo*= alfa;
		      v[i][j]+=foo;
		    }
		  else
		    {
		      // borde superior
		      foo=(v[i][j+1]+v[i][j-1]+v[i+1][j])/3;
		      foo -= v[i][j];
		      foo*= alfa;
		      v[i][j]+=foo;
		    }
		}
	      else if (i == nmax-1 )
		{
		  if (j==0)
		    {
		      // esquina inferior izquierda
		      foo=(v[i-1][j]+v[i][j+1])/2;
		      foo -= v[i][j];
		      foo*= alfa;
		      v[i][j]+=foo;
		    }
		  else if (j==nmax-1)
		    {
		      // esquina inferior derecha
		      foo=(v[i-1][j]+v[i][j-1])/2;
		      foo -= v[i][j];
		      foo*= alfa;
		      v[i][j]+=foo;
		    }
		  else
		    {
		      // borde inferior
		      foo=(v[i][j+1]+v[i][j-1]+v[i-1][j])/3;
		      foo -= v[i][j];
		      foo*= alfa;
		      v[i][j]+=foo;
		    }
		}
	      
	      // condicion de frontera no fija
	      else if (j == 0 )
		{
		  v[i][j]=(v[i+1][j]+v[i-1][j]+v[i][j+1])/3;
		}
	      else if ( j == nmax-1)
		{
		  v[i][j]=(v[i+1][j]+v[i-1][j]+v[i][j-1])/3;
		}
	      else if ( i == cparam  && j > cparam && nmax-1-cparam > j)
		{
		  v[i][j]=1;
		}
	      else if ( i == nmax-1-cparam  && j > cparam && nmax-1-cparam > j)
		{
		  v[i][j]=-1;
		}
	      
	      else
		{
		  foo =(v[i+1][j]+v[i-1][j]+v[i][j+1]+v[i][j-1])/4;
		  foo -= v[i][j];
		  foo *= alfa;
		  v[i][j]+=foo;
		}
	    }
	  fprintf(gnuplot, "\n");	    
	}
      fprintf(gnuplot, "e\n");
    }

  // aplicamos metodo de relajacion Gauss Siedel
  // note que la iteracion esta definida en la matriz sin orillas (i-1  e i+1) no estan definidos
  // en el ultimo y primer elemento.

 
  

  fclose(arch);
  fflush(gnuplot);
  int alv=0;
  printf("da una letra y enter para terminar \n");
  scanf("%c", &alv);
  fclose(gnuplot);
  return 0;

  
} // parentesis del int main
