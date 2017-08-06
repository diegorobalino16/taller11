/*
 * Autor: Ing. Eduardo Murillo
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>



//Declaramos una estructura
typedef struct mi_estructuraTDA{
	int conteo;
	char *str;
} estructura;

typedef struct IntervalosTDA {
	pthread_t thread_id;
   	int inicio;
   	int final;
   	int *arreglo;
} Intervalos;

double obtenerTiempoActual(){
	struct timespec tsp;

	clock_gettime( CLOCK_REALTIME, &tsp);

	double secs = (double)tsp.tv_sec;
	double nano = (double)tsp.tv_nsec / 1000000000.0;

	return secs + nano;
}

//Funcion para el hilo 1
void * funcion_hilo(void *arg){

	Intervalos *intervalo = (Intervalos *)arg;			//Argumento lo recibimos como tipo void*, lo convertimos a int.

	int suma = 0;
	double time_ini, time_fin, time_total = 0;
	time_ini = obtenerTiempoActual();
	for (int i = intervalo->inicio; i <= intervalo->final; i++){
		//printf("elemento: %d, suma: %d, i: %d\n", intervalo->arreglo[i], suma, i);
		suma = intervalo->arreglo[i] + suma;
		printf("Del %d al %d: %d\n", intervalo->inicio, intervalo->final, suma);
	}
	time_fin = obtenerTiempoActual();
	time_total = time_fin - time_ini;
	printf("Tiempo: %lf - %lf = %lf\n", time_fin, time_ini, time_total);
	return (void *)0;		//tenemos que devolver algo
}


void hilo(int longitud, int cont, int num_hilo, int *arreglo){
	int ini = 0, fin = 0;
	int status;
	ini = num_hilo * cont;
	fin = (num_hilo * cont) + (cont - 1);
	if ((longitud - fin) < 0){
		fin = longitud - 1;
	}
	//printf("inicio: %d - fin: %d\n", ini, fin);
	Intervalos *intervalo = malloc(sizeof(Intervalos));
	intervalo->inicio = ini;
	intervalo->final = fin;
	intervalo->arreglo = arreglo;
	status = pthread_create(&intervalo->thread_id, NULL, funcion_hilo, (void *)intervalo);	//al hilo 1 le mandamos el argumento 50;
	//sleep(1);
	if(status < 0){
		fprintf(stderr, "Error al crear el hilo 1\n");
		exit(-1);	
	}
}

int aleatorio(int min, int max){
	return (rand() % (max - min + 1)) + min;
}

int main(int argc, char **argv){

	//tmp
	int cont;
	//Crear arreglo
	int longitud = atoi(argv[1]);
	int num_hilos = atoi(argv[2]);
	//printf("%d, %d\n", longitud2, cont2);
	int *arreglo = (int *) malloc(longitud * sizeof(*arreglo));

	//Calcular el contador
	float div = 0;
	int div_entero;
	div = (float)longitud / num_hilos;
	div_entero = longitud / num_hilos;
	printf("%d / %d = %f\n", longitud, num_hilos, div);
	printf("%d / %d = %d\n", longitud, num_hilos, div_entero);

	if (div > div_entero) cont = div_entero + 1;
	else cont = div_entero;
	printf("Entonces cada hilo tendrá: %d elementos.\n", cont);
	//Fin de calcular cont

	//Llenar arreglo
	printf("Arreglo:\n");
	for (int i = 0; i < longitud; i++)
	{
		arreglo[i] = aleatorio(1, 9);
		printf("%d: %d\n", i, arreglo[i]);
	}
	//Fin de llenar el arreglo
	sleep(1);
	
	for (int i = 0; i < num_hilos; ++i) //cont es el numero de elementos por hilo
	{
		hilo(longitud, cont, i, arreglo); //longitud es el num de elementos
	}
	
	sleep(1);
	printf("Hilos terminaron normalmente\n");
	exit(0);
	

}
