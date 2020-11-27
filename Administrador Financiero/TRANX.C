#include "TRANX.H"

// recopila la informacion de la transaccion
void tranx_recopilar(Transaccion *tranx){
	printf("Nombre: ");
	fgets( (*tranx).nombre, 50, stdin );

	printf("Monto: ");
	scanf("%f", &(*tranx).monto);
	fflush(stdin);

	printf("Tipo(1.Gasto 2.Ingreso): ");
	scanf("%d", &(*tranx).tipo);
	fflush(stdin);
}

// muestra la transaccion
void tranx_mostrar(Transaccion *tranx){
	printf("Nombre: %s", (*tranx).nombre);
	if( (*tranx).tipo == gasto){
		printf("Monto: -C$%.2f\n", (*tranx).monto);
	}else{
		printf("Monto: +C$%.2f\n", (*tranx).monto);
	}
}

// muestra la transaccion toda la info
void tranx_mostrar_todo(Transaccion *tranx){
	printf("Nombre: %s", (*tranx).nombre);
	if( (*tranx).tipo == gasto){
		printf("Monto: -C$%.2f\n", (*tranx).monto);
	}else{
		printf("Monto: +C$%.2f\n", (*tranx).monto);
	}
	printf("Cartera: %s", (*tranx).cartera.propietario);
	printf("Saldo: %.2f; Ingresos: %.2f; Gastos: %.2f\n", (*tranx).cartera.saldo, 
														(*tranx).cartera.ingreso_total, 
														(*tranx).cartera.gasto_total);
}

// abre la base de datos
FILE *tranx_db_abrir(char *modo){
	FILE *f = fopen(Tranx_DBN, modo);
	if(f == NULL){
		puts("No se pudo abrir el archivo, creando...");
		f = fopen(Tranx_DBN, "w");
		if(f == NULL){
			puts("No se ha podido crear el archivo");
			exit(-1);
		}
	}
	return f;
}

// agrega una transaccion a la base de datos
void tranx_db_agregar(Transaccion *tranx, Cartera *cartera){
	FILE *f = tranx_db_abrir("a");

	fprintf(f, "%s", (*tranx).nombre);
	fprintf(f, "%f %d\n", (*tranx).monto, (*tranx).tipo);
	fprintf(f, "%s", (*tranx).cartera.propietario);

	if( (*tranx).tipo == gasto ){
		(*cartera).saldo -= (*tranx).monto;
		(*cartera).gasto_total += (*tranx).monto;
	}else{
		(*cartera).saldo += (*tranx).monto;
		(*cartera).ingreso_total += (*tranx).monto;
	}

	(*tranx).cartera = *cartera;

	fprintf(f, "%f %f %f\n", (*tranx).cartera.saldo, (*tranx).cartera.ingreso_total, (*tranx).cartera.gasto_total);

	fclose(f);
	cartera_db_actualizar_tranx(cartera);
}

// cuenta el numero de transacciones
int tranx_db_contar(){
	FILE *f = tranx_db_abrir("r");

	int cont=0;	char c;

	while( !feof(f) ){
		c = fgetc(f);
		if(c == '\n') cont++;
	}

	cont /= 4;
	return cont;
}

// lee las transacciones y las guarda
Transaccion *tranx_db_leer(){
	FILE *f = tranx_db_abrir("r");
	int cont = tranx_db_contar();
	Transaccion *tranx = (Transaccion *)calloc(cont, sizeof(Transaccion));

	int i;
	for(i = 0; i < cont; i++){
		fgets( (*(tranx + i)).nombre, 50, f );
		fscanf(f, "%f %d\n", &(*(tranx + i)).monto, &(*(tranx + i)).tipo );

		fgets( (*(tranx + i)).cartera.propietario, 100, f );
		fscanf(f, "%f %f %f\n", &(*(tranx + i)).cartera.saldo, &(*(tranx + i)).cartera.ingreso_total, &(*(tranx + i)).cartera.gasto_total);
	}

	fclose(f);
	return tranx;
}

void tranx_db_pipe(Cartera *cartera){
	Transaccion *tranx = tranx_db_leer();

	int cant = tranx_db_contar(), i;
	for(i = 0; i < cant; i++){
		if( strcmp( (*(tranx + i)).cartera.propietario, (*cartera).propietario ) == 0 ){
			tranx_mostrar_todo( (tranx + i) );
			printf("\n");
		}
	}
}