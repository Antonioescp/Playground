#include "CARTERA.H"
#include <string.h>
#include <conio.h>

// recopila los datos de la cartera
void cartera_recopilar(Cartera *cartera, const enum cartera_info_level nivel){

	if(nivel == FULL){
		printf("Propietario: ");
		fgets( (*cartera).propietario, 100, stdin );

		printf("Saldo: ");
		scanf("%f", &(*cartera).saldo);
		fflush(stdin);

		(*cartera).ingreso_total = (*cartera).saldo;
		(*cartera).gasto_total = 0;
	}else{
		printf("Propietario: ");
		fgets( (*cartera).propietario, 100, stdin );
	}

}

// muestra la info de la cartera
void cartera_mostrar(Cartera *cartera, const enum cartera_info_level nivel){

	if( nivel == FULL ){
		printf("Propietario: %s", (*cartera).propietario);
		printf("Saldo: %.2f\n", (*cartera).saldo);
		printf("Ingreso total: %.2f\n", (*cartera).ingreso_total);
		printf("Gasto total: %.2f\n", (*cartera).gasto_total);
	}else{
		printf("Propietario: %s", (*cartera).propietario);
	}

}

// abre o crea una base de datos
FILE *cartera_db_abrir(char *modo){
	FILE *f = fopen(Cartera_DBN, modo);
	if(f == NULL){
		puts("No se pudo leer el archivo, creando...");
		f = fopen(Cartera_DBN, "w");
		if(f == NULL){
			puts("No se pudo crear el archivo");
			exit(-1);
		}
	}
	return f;
}

// agrega una cartera a la base de datos
void cartera_db_agregar(Cartera *cartera){
	FILE *f = cartera_db_abrir("a");

	fprintf(f, "%s", (*cartera).propietario);
	fprintf(f, "%f %f %f\n", (*cartera).saldo, (*cartera).ingreso_total, (*cartera).gasto_total);

	fclose(f);
}

// cuenta el numero de carteras
int cartera_db_contar(){
	FILE *f = cartera_db_abrir("r");

	int cont=0; char c;

	while( !feof(f) ){
		c = fgetc(f);
		if(c == '\n') cont++;
	}
	cont /= 2;

	fclose(f);
	return cont;
}

// Muestra la info de todas las carteras
void cartera_mostrar_todas(Cartera *cartera, const enum cartera_info_level nivel){
	int i, cant = cartera_db_contar();
	for(i = 0; i < cant; i++){
		cartera_mostrar( (cartera + i), nivel );
		if(nivel == FULL) printf("\n");
	}
}

// lee el documento de carteras entero
Cartera *cartera_db_leer(){
	FILE *f = cartera_db_abrir("r");
	int i, cant = cartera_db_contar();
	Cartera *carteras = (Cartera *)calloc(cant, sizeof(Cartera));

	for(i = 0; i < cant; i++){
		fgets( (*(carteras + i)).propietario, 100, f );
		fscanf(f, "%f %f %f\n", &(*(carteras + i)).saldo, &(*(carteras + i)).ingreso_total, &(*(carteras + i)).gasto_total );
	}

	fclose(f);
	return carteras;
}

// busca una cartera
int cartera_db_buscar(Cartera *cartera, const enum cartera_info_level nivel){
	Cartera *carteras = cartera_db_leer();
	int cant = cartera_db_contar(), i, pos=-1;

	if(nivel == NAME){
		for(i = 0; i < cant; i++){
			if( strcmp( (*cartera).propietario, (*(carteras + i)).propietario ) == 0 ){
				pos = i;
			}
		}
	}else{
		for(i = 0; i < cant; i++){
			if( strcmp( (*cartera).propietario, (*(carteras + i)).propietario ) == 0 && (*cartera).saldo == (*(carteras + i)).saldo && (*cartera).gasto_total == (*(carteras + i)).gasto_total && (*cartera).ingreso_total == (*(carteras + i)).ingreso_total){
				pos = i;
			}
		}
	}
	
	if(pos == -1)
		puts("Cartera no encontrada");
	else
		puts("Cartera encontrada!");

	free(carteras);
	return pos;
}

// selecciona la cartera y la retorna
void cartera_db_seleccionar(Cartera *cartera, const enum cartera_info_level nivel){
	Cartera *carteras = cartera_db_leer();
	cartera_recopilar(cartera, nivel);
	int pos = cartera_db_buscar(cartera, nivel);
	
	if(pos == -1){
		puts("Cartera no encontrada!");
	}else{
		puts("Cartera seleccionada con exito!");
		*cartera = *(carteras + pos);
		cartera_mostrar(cartera, FULL);
	}

	free(carteras);
}

// actualiza la informacion de una cartera
void cartera_db_actualizar(Cartera *cartera){
	Cartera *carteras = cartera_db_leer();
	int pos = cartera_db_buscar(cartera, FULL);

	if(pos == -1){
		puts("No se puede actualizar, cartera no existe");
	}else{
		puts("Ingrese los nuevos datos");
		cartera_recopilar(cartera, FULL);

		int cant = cartera_db_contar(), i;
		*(carteras + pos) = *cartera;

		FILE *f = cartera_db_abrir("w");
		for(i = 0; i < cant; i++){
			cartera_db_agregar( &(*(carteras + i)) );
		}
		fclose(f);
	}
	free(carteras);
}

void cartera_db_actualizar_tranx(Cartera *cartera){
	Cartera *carteras = cartera_db_leer();
	int pos = cartera_db_buscar(cartera, NAME);

	if(pos == -1){
		puts("No se puede actualizar, cartera no existe");
	}else{
		int cant = cartera_db_contar(), i;
		*(carteras + pos) = *cartera;

		FILE *f = cartera_db_abrir("w");
		for(i = 0; i < cant; i++){
			cartera_db_agregar( &(*(carteras + i)) );
		}
		fclose(f);
	}
	free(carteras);
}