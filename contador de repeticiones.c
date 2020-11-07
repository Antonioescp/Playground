#include <stdio.h>

#define LENGTH 10

void ordenar(int *); /* ordena el arreglo */

void main(){ 
	
	srand(time(NULL)); /* semilla en base al tiempo actual de tu pc */
	
	int numeros[LENGTH], contador[LENGTH], i, num, total;
	
	for(i = 0; i < LENGTH; i++){ /* iniciamos el contador en 0 */
		contador[i] = 0;
	}
	
	for(i=0;i<LENGTH;i++){ /* numeros se repiten en pares */
		if(i % 2 == 0){
			num = rand() % 300 + 1;
		}
		numeros[i] = num;
	}
	
	ordenar(numeros); /* ordena el arreglo */
	
	num = 0; /* indice de contador */
	
	for(i = 0; i < LENGTH; i++){
		if(i == 0){ /* cuenta el primer numero */
			contador[num]++; /* aqui pasamos num en lugar de i porque el 
								contar depende de qué numero esté en el arreglo
								y no de i */
		}else{
			if(numeros[i] != numeros[i-1]){ /* si el arreglo contiene un numero diferente
											   al anterior entonces cambiamos de posicion
											   en el contador, por eso es importante ordenar */
				num++;
			}
			contador[num]++; /* no nos olvidemos de contar */
		}
	}
	total = num + 1; /* guardamos cuantos numeros hay en total */
	num = 0; /* de nuevo, iterador del contador */
	
	printf("\tNumeros en arreglo\n");
	
	for(i = 0; i < LENGTH; i++){
		printf("pos[%d]: %d\n", i, numeros[i]);
	}
	
	printf("\tContador correspondiente\n");
	for(i = 0; i < total; i++){ /* imprimimos el arreglo */
		printf("%d\n", contador[i]);
	}
	
	num = 0;
	
	printf("\tMostrar info\n");
	
	for(i = 0; i < total; i++){ /* aqui imprimimos el total de numeros unicos */
		/* la manera en la que se itera numeros para que esto funciones es sumando
		la cantidad de veces que se conto, de esta manera podemos imprimir el siguiente
		numero que contamos */
		printf("%d se encuentra %d veces\n", numeros[num], contador[i]); 
		num += contador[i];
	}
}

void ordenar(int *arr){ /* ordenamiento burbuja */
	int i, j, temp;
	
	for(i = 0; i < LENGTH-1; i++){
		for(j = 0; j < LENGTH-1; j++){
			if( *(arr + j) > *(arr + j + 1) ){
				temp = *(arr + j);
				*(arr + j) = *(arr + j + 1);
				*(arr + j + 1) = temp;
			}
		}
	}
}
