/*
 * Autor: Ing. Eduardo Murillo
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



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

//Funcion para el hilo 1
void * funcion_hilo(void *arg){

	Intervalos *intervalo = (Intervalos *)arg;			//Argumento lo recibimos como tipo void*, lo convertimos a int.

	int suma = 0;

	for (int i = intervalo->inicio; i <= intervalo->final; i++){
		printf("elemento: %d, suma: %d, i: %d\n", intervalo->arreglo[i], suma, i);
		suma = intervalo->arreglo[i] + suma;
		printf("Del %d al %d: %d\n\n", intervalo->inicio, intervalo->final, suma);
	}
	return (void *)0;		//tenemos que devolver algo
}


void hilo(int longitud, int cont, int num_hilo, int *arreglo){
	int ini = 0, fin = 0;
	int status;
	ini = num_hilo * cont;
	fin = (num_hilo * cont) + (cont - 1);
	if ((longitud - fin) < 0){
		fin = longitud;
	}
	//printf("inicio: %d - fin: %d\n", ini, fin);
	Intervalos *intervalo = malloc(sizeof(Intervalos));
	intervalo->inicio = ini;
	intervalo->final = fin;
	intervalo->arreglo = arreglo;
	status = pthread_create(&intervalo->thread_id, NULL, funcion_hilo, (void *)intervalo);	//al hilo 1 le mandamos el argumento 50;
	sleep(1);
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
	int longitud = 16, cont = 4;
	//Crear arreglo
	int *arreglo = malloc(5 * sizeof *arreglo);

	//Llenar arreglo
	printf("Arreglo:\n");
	for (int i = 0; i < longitud; i++)
	{
		arreglo[i] = aleatorio(1, 9);
		printf("%d: %d\n", i, arreglo[i]);
	}
	sleep(1);
	
	for (int i = 0; i < cont; ++i) //cont es el numero de hilos
	{
		hilo(longitud, cont, i, arreglo); //longitud es el num de elementos
	}
	
	for (int i = 0; i < longitud; i++)
	{
		printf("%d: %d\n", i, arreglo[i]);
	}
	
	sleep(3);
	printf("Hilos terminaron normalmente\n");
	exit(0);
	

}
