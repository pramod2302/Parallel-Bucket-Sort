#include <stdio.h>
#include <string.h>  
#include <mpi.h> 
#include<stdlib.h>
#include<time.h>
const int MAX_STRING = 100;
int n=200;
int ar1[1000];
int c2=0;
void bucksort(int *array, int s)
{
	int i,j;
	int *count;
	count=malloc(s*sizeof(int));

	for(i=0; i < s; i++)
	{
		count[i] = 0;
	}

	for(i=0; i < s; i++)
	{
		count[array[i]]++;
		//printf("item  %d (%d):%d elements\n", i,array[i],count[array[i]]);
	}
	for(i=0,j=0; i < s; i++)
	{
		//printf("bucket[%d] = %d\n", i,count[i]);
		for (; count[i]>0; count[i]--)
		{
			array[j++] = i;
		}
	}
}


void output(int *array)
{
	int i;
	for (i = 0;i < n;i++)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
}

void mpir(int arr[])
{

	
	int      greeting=100;
	int ar[100];
	int        my_rank, p, q,x=1,i;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	double t1 = MPI_Wtime();
	int m=n/(p-1);
	if (n%(p-1)!=0)m=m+1;
	if (my_rank == 0) 
	{
		
		for (q = 0; q < n; q++) 
		{
			int x=(arr[q]/m)+1;
			int rem=arr[q]%m;
			MPI_Send(&rem, 1, MPI_INT, x, 0,MPI_COMM_WORLD);
       		
      	} 
      	x=-999;
      	for (q = 1; q < p; q++) 
		{
			MPI_Send(&x, 1, MPI_INT, q, 0,MPI_COMM_WORLD);
		}
		printf("sent values\n");
		int c=0;

		greeting=0;
		for (q=1;q<p;q++)
		{
			while(1)
			{
				MPI_Recv(&greeting, 1, MPI_INT, q, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				if (greeting>=0)
					ar1[c++]=greeting;
				else break;
			}

		}
		printf("sorted : \n");
		for (q=0;q<c;q++)
		{	
			printf("%d, ",ar1[q] );
		}
		printf("done\n");
		double t2 = MPI_Wtime();
		printf("time taken : %lf\n\n\n\n", (t2-t1));
	}
	else 
	{
		int c=0;
		while(greeting!=-999)
		{
			MPI_Recv(&greeting, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			ar[c++]=greeting;
		}
		bucksort(ar,c-1);
		for (i=0;i<c;i++)
		{
			int x=m*(my_rank-1)+ar[i];
			MPI_Send(&x, 1, MPI_INT, 0, 0,MPI_COMM_WORLD);
		}
   	}

	MPI_Finalize();
}

int main(void) {
	

	
     
    int i;
     for (i=0; i<n; i++)
	{
		ar1[i]=0;
	}
	int *array;
	array=malloc(n*sizeof(int));
	srand(time(NULL));
	for (i=0; i<n; i++)
	{
		array[i]=random()%n;
		//("%d, ",array[i] );
	}
	printf("\n");
	mpir(array);

	
     return 0;
}
