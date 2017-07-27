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



//Funcion para el hilo 1
void * funcion_hilo1(void *arg){

	int argumento = (long)arg;			//Argumento lo recibimos como tipo void*, lo convertimos a int.

	int i = 0;
	for(i = argumento - 1; i >= 0; i--){
		sleep(1);		//duerme 1 segundo a la vez
		printf("Hilo 1: faltan %d segundos para terminar\n", i);	
	}

	return (void *)0;		//tenemos que devolver algo
}

//Funcion para el hilo 2
void * funcion_hilo2(void *arg){
	
	estructura *argumentos = (estructura *)arg;		//convertimos al tipo de dato correcto

	int i = 0;

	for(i = 0; i < argumentos->conteo; i++){
		printf("Hilo 2: %s realizando una plana de %d lineas\n", argumentos->str, argumentos->conteo);
		sleep(1);
	}
	
	return (void *)10;

}

void hilo(int longitud, int cont, int num_hilo){
	int ini = 0, fin = 0;
	ini = num_hilo * cont;
	fin = (num_hilo * cont) + (cont - 1);
	if ((longitud - fin) < 0){
		fin = longitud;
	}
	printf("inicio: %d - fin: %d\n", ini, fin);
	for (int i = ini; i <= fin; ++i)
	{
		printf("Hilo numero %d tiene el elemento: %d\n", num_hilo, i);
	}
}

int aleatorio(int min, int max){
	return (rand() % (max - min + 1)) + min;
}

int main(int argc, char **argv){

	//tmp
	int longitud = 10, cont = 4;
	//
	
	pthread_t id1, id2;		//aqui almacenamos los IDs de los hilos

	int status;

	//Crear arreglo
	int *arreglo = malloc(5 * sizeof *arreglo);

	//Llenar arreglo
	for (int i = 0; i < 16; i++)
	{
		arreglo[i] = aleatorio(1, 10);
		//printf("%d\n", arreglo[i]);
	}

	//Prueba inicio
	//printf("%d - %d\n", argv[0][0], argv[0][0]);

	
	for (int i = 0; i < cont; ++i) //cont es el numero de hilos
	{
		hilo(longitud, cont, i); //longitud es el num de elementos, cont es la division
	}
	
	//Prueba final
	long argumento_entero = 50;
	status = pthread_create(&id1, NULL, funcion_hilo1, (void *)argumento_entero);	//al hilo 1 le mandamos el argumento 50;

	if(status < 0){
		fprintf(stderr, "Error al crear el hilo 1\n");
		exit(-1);	
	}

	//Creamos hilo 2
	estructura *mi_argumento_estructura = malloc(sizeof(estructura));	//Creamos la estrucura que mandaremos como argumento al hilo 2

	mi_argumento_estructura->conteo  = 40;
	mi_argumento_estructura->str = "este es el segundo hilo";

	status = pthread_create(&id2, NULL, funcion_hilo2, (void *)mi_argumento_estructura);	//al hilo 1 le mandamos el argumento 50;

	if(status < 0){
		fprintf(stderr, "Error al crear el hilo 2\n");
		exit(-1);	
	}
	
	//Esperamos a que ambos hilos terminen
	void * valor_retorno = NULL;
	
	printf("Hilo principal esta esperando a que terminen los otros hilos\n");
	/*int status1 = pthread_join(id1, NULL);			//Para el hilo 1, no nos importa el valor de retorno, por eso mandamos NULL
	int status2 = pthread_join(id2, &valor_retorno);	//Al inicio, valor_retorno apunta a NULL; al regresar de esta funcion, apuntara a la estructura/valor que retorne el hilo 2

	if(status1 < 0){
		fprintf(stderr, "Error al esperar por el hilo 1\n");
		exit(-1);
	}

	if(status2 < 0){
		fprintf(stderr, "Error al esperar por el hilo 2\n");
		exit(-1);
	}*/

	printf("El hilo 2 devolvio %lu\n", (long)valor_retorno); 
		

	printf("Hilos terminaron normalmente\n");
	exit(0);
	

}
