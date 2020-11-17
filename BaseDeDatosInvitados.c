#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// nombre de la base de datos
#define DBN "DB.txt"

// nivel de informacion a utilizar o pedir
#define JUST_NAME 1
#define FULL_INFO 2

// arreglo de cadenas
typedef char cadenas[][50];

// categorias para amigos
enum ctg{
	compa=1, trabajo, familiar, gym
};

// opciones de operaciones
enum op_oper{
	agregar=1, buscar, borrar, datos, modificar
};

//  invitados
typedef struct{
	char nombre[50], ciudad[50], telf[50];
	int edad;
	enum ctg categoria;
}Invitado;

// muestra un menu con las cadenas como opciones y retorna la opcion
// seleccionada
int menu(cadenas, char *);

// operaciones con invitados
// recopila la info del usuario, completa o solo nombre
void inv_recopilar(Invitado *, const int info_level);
// muestra informacion por ctg, completa o solo nombre
void inv_porctg(Invitado *, enum ctg categoria, const int info_level);
// muestra toda la info del usuario
void inv_show_all(Invitado *);
// muestra solo el nombre del usuario
void inv_show(Invitado *);

// operaciones base de datos
// añade un invitado al archivo
void db_add(Invitado *);
// lee el archivo completo y retorna un arreglo de invitados
Invitado *db_read();
// cuenta cuantos invitados hay en total
int db_contar();
// verifica si el fichero es NULL
void db_verificar(FILE *);
// borra un invitado de la base de datos segun criterio, sobreescribe el archivo
void db_borrar(Invitado *, Invitado *, const int, const enum ctg);
// busca un invitado y lo modifica segun criterio, sobreescribe el archivo
void db_modificar(Invitado *, Invitado *, const int, const enum ctg);
// busca un invitado en el arreglo y retorna la posicion de este en el arreglo
int db_buscar(Invitado *, Invitado *, const int, const enum ctg);

void main(){
	
	// opcion en los menues
	int op=0;
	
	// opciones para cada menu
	cadenas op_categorias = {"5", "Compas", "Trabajo", "Familiares", "Gimnasio", "Salir"};
	cadenas op_operaciones = {"6", "Agregar", "Buscar", "Borrar", "Imprimir datos", "Modificar", "Regresar al menu anterior"};
	cadenas op_busqueda = {"2", "Por nombre", "Informacion completa"};
	
	// para hacer las operaciones de invitados
	Invitado inv;
	Invitado *invs=NULL;
	// bucle del programa
	while(1){	
		op = menu(op_categorias, "Categorias");
		// si se elige salir
		if(op == 5) break;
		// guardar categoria seleccionada
		inv.categoria = op;
		// menu de operaciones
		op = menu(op_operaciones, "Operaciones");
		system("cls");
		// operaciones
		switch(op){
			case agregar:
				
				puts("\tAgregar invitado");
				// recopila toda la info del usuario
				inv_recopilar(&inv, FULL_INFO);
				// lo agrega al archivo
				db_add(&inv);
				break;
				
			case buscar:
				
				printf("\tInvitados en la categoria %s\n", op_categorias[inv.categoria]);
				// librea memoria primero
				if(invs != NULL) free(invs);
				// lee el fichero y lo guarda en una estructura
				invs = db_read();
				// muestra por categoria
				inv_porctg(invs, inv.categoria, JUST_NAME);
				system("pause");
				break;
				
			case borrar:
				// pregunta como se desea buscar
				op = menu(op_busqueda, "Criterio buscar y borrar");
				// librea memoria primero
				if(invs != NULL) free(invs);
				// lee el fichero y lo guarda en una estructura
				invs = db_read();
				puts("Ingrese los datos para buscar");
				// recopila la info del invitado a buscar, completa o solo nombre
				inv_recopilar(&inv, op);
				// busca y borra el usuario, sobreescribe el fichero, lo borra si el usuario
				// que se borro era el unico invitado
				db_borrar(invs, &inv, op, inv.categoria);
				break;
				
			case datos:
				
				printf("\tInvitados en la categoria %s\n", op_categorias[inv.categoria]);
				// librea memoria primero
				if(invs != NULL) free(invs);
				// lee el fichero y lo guarda en un arreglo
				invs = db_read();
				// muestra por categoria pero esta vez con la info
				// completa
				inv_porctg(invs, inv.categoria, FULL_INFO);
				system("pause");
				break;
			
			case modificar:
				op = menu(op_busqueda, "Buscar y modificar");
				// librea memoria primero
				if(invs != NULL) free(invs);
				// lee fichero y lo guarda en el arreglo
				invs = db_read();
				// guarda la info del inv segun el nivel proporcionado
				inv_recopilar(&inv, op);
				// busca y modifica el usuario
				db_modificar(invs, &inv, op, inv.categoria);
				break;
			default:
				puts("Opcion invalida");
		}
	}
}

// operaciones de base de datos
// busca un invitado y lo modifica
void db_modificar(Invitado *invs, Invitado *inv, const int info_level, const enum ctg categoria){
	int pos, i, inv_cant = db_contar();
	pos = db_buscar(invs, &(*inv), info_level, categoria);
	
	if( pos >= 0){
		printf("El invitado fue encontrado, sus datos son:\n");
		inv_show_all( (invs+pos) );
		puts("Ingresa sus nuevos datos");
		inv_recopilar(&(*inv), FULL_INFO);
		puts("Guardando...");
		*(invs + pos) = *inv;
		// sobreescribimos el archivo anterior
		remove(DBN);
		for(i=0;i<inv_cant;i++){
			db_add( (invs+i) );
		}
		
		puts("El invitado se actualizo con exito!");
	}else{
		printf("El invitado no se encuentra en esta categoria\n");
		system("pause");
	}
}

// busca un invitado en el arreglo y retorna la posicion de este en el arreglo
int db_buscar(Invitado *invs, Invitado *inv, const int info_level, const enum ctg categoria){
	// iterador, cantidad de invitados, posicion del invitado, bool encontrado
	int i, cant_inv = db_contar(), pos=0, encontrado=0;
	
	if(info_level == JUST_NAME){
		// solo por el nombre
		for(i=0;i<cant_inv;i++){
			if( strcmp( (*(invs + i)).nombre, (*inv).nombre ) == 0 && (*(invs + i)).categoria == (*inv).categoria ){
				// se encontro
				encontrado = 1;
				// se guarda la posicion
				pos = i;
			}
		}
	}else if(info_level == FULL_INFO){
		// usando toda la info
		for(i=0;i<cant_inv;i++){
			
			if( strcmp( (*(invs + i)).nombre, (*inv).nombre ) == 0 && 
				(*(invs + i)).edad == (*inv).edad &&
				strcmp( (*(invs + i)).telf, (*inv).telf ) == 0 &&
				strcmp( (*(invs + i)).ciudad, (*inv).ciudad ) == 0 &&
				(*(invs + i)).categoria == (*inv).categoria ){
				// se encontro
				encontrado = 1;
				// se guarda la posicion
				pos = i;
			}
			
		}
	}
	
	if(encontrado){
		return pos;
	}else{
		return -1;
	}
}

// borra un invitado de la base de datos segun criterio
void db_borrar(Invitado *invs, Invitado *inv, const int info_level, const enum ctg categoria){
	
	int pos = db_buscar(invs, &(*inv), info_level, categoria), i, 
		cant_inv = db_contar();
		
	// se se encuentra
	if( pos >= 0 ){
		if(cant_inv <= 1){
			remove(DBN);
		}else{
			// se remplaza el invitado en esa posicion por el siguiente 
			// y el siguiente por el sieguiente...
			for(i=pos;i<cant_inv;i++){
				*(invs + i) = *(invs + i + 1); 
			}
			// se reduce la cant_inv
			cant_inv--;
			// reasigna memoria para la nueva cantidad de invitados
			realloc(invs, sizeof(Invitado)*cant_inv);
			// reescribe el archivo
			remove(DBN);
			for(i=0;i<cant_inv;i++){
				db_add( &(*(invs+i)) );
			}
		}
		puts("El invitado se borro con exito");
		system("pause");
	}else{
		puts("El invitado no se encuentra en esta categoria");
		system("pause");
	}
}

// verifica si el fichero se pudo abrir
void db_verificar(FILE *f){
	if(f==NULL){
		puts("El archivo no se pudo abrir/crear");
		exit(-1);
	}
}

// lee el archivo completo
Invitado *db_read(){
	FILE *f=NULL;
	// cantidad de invitados
	int inv_cant = db_contar(), i;
	// asigna memoria para los nuevos invitados
	Invitado *invs = (Invitado *)calloc(inv_cant, sizeof(Invitado));
	// verificamos
	if(invs == NULL){
		puts("No se pudo asignar la memoria");
	}
	
	// abrimos el archivo
	f = fopen(DBN, "r");
	// verificamos
	db_verificar(f);
	// guardamos los invitados en el arreglo
	for(i=0;i<inv_cant;i++){
		fgets( (*(invs + i)).nombre, 50, f );
		fscanf(f, "%d %s %s %d\n", &(*(invs + i)).edad, (*(invs + i)).telf, 
									(*(invs + i)).ciudad, &(*(invs + i)).categoria);
	}
	
	// cierra el archivo
	fclose(f);
	
	return invs;
}

// agrega un invitado al archivo
void db_add(Invitado *inv){
	FILE *f;
	// abrimos
	f = fopen(DBN, "a");
	// verificamos
	db_verificar(f);
	// guardamos
	fputs((*inv).nombre, f);
	fprintf(f, "%d %s %s %d\n", (*inv).edad, (*inv).telf, (*inv).ciudad, (*inv).categoria);
	// cerramos
	fclose(f);
}

// cuenta el numero de invitados en el archivo
int db_contar(){
	FILE *f=NULL;
	// para comparar caracteres
	char c;
	// para contar los empleados
	int cont=0;
	// abrimos el archivo
	f = fopen(DBN, "r");
	// verficamos
	db_verificar(f);
	// contamos el numero de invitados
	while( !feof(f) ){
		c = fgetc(f);
		if(c == '\n') cont++;
	}
	// cerramos
	fclose(f);
	// retornamos el numero de empleados
	return cont/2;
}

// operaciones con invitados
// recopila la info de los invitados
void inv_recopilar(Invitado *inv, const int info_level){
	if(info_level == JUST_NAME){
		printf("Ingresa el nombre: ");
		fgets((*inv).nombre, 50, stdin);
		fflush(stdin);
		
	}else if(info_level == FULL_INFO){
		printf("Ingresa el nombre: ");
		fgets((*inv).nombre, 50, stdin);
		fflush(stdin);
		
		printf("Ingresa la edad: ");
		scanf("%d", &(*inv).edad);
		fflush(stdin);
		
		printf("Ingresa la ciudad: ");
		scanf("%s", &(*inv).ciudad);
		fflush(stdin);
		
		printf("Ingresa el telefono: ");
		gets((*inv).telf);
	}
}

// muestra por categoria
void inv_porctg(Invitado *invs, enum ctg categoria, const int info_level){
	int i, inv_cant = db_contar();
	for(i=0;i<inv_cant;i++){
		if( (*(invs+i)).categoria == categoria ){
			switch(info_level){
				case FULL_INFO:
					inv_show_all( (invs+i) );
					break;
				case JUST_NAME:
					inv_show( (invs+i) );
					break;
				default:
					puts("opcion invalida");
			}
		}
	}
}

// muestra el invitado
void inv_show_all(Invitado *inv){
	printf("Nombre: %sEdad: %d / Telf: %s / Ciudad: %s\n\n", (*inv).nombre, (*inv).edad, (*inv).telf, (*inv).ciudad);
}

// muestra solo el nombre del usuario
void inv_show(Invitado *inv){
	printf("Nombre: %s", (*inv).nombre);
}

// muestra un menu de opciones (requiere el numero de opciones en la primera pos)
int menu(cadenas opciones, char *title){
	int op, i, size = (int)opciones[0][0]-48;
	// validacion
	do{
		system("cls");
		printf("*********%s*********\n", title);
		// muestra las opciones
		for(i = 1; i <= size; i++){
			printf("%d. %s\n", i, opciones[i]);
		}
		// pide la opcion
		printf("> "); scanf("%d", &op);
		fflush(stdin);
	}while(op<1 || op>size);
	// retorna la seleccion
	return op;
}
