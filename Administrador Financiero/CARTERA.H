#ifndef _CARTERA
#define _CARTERA

#define Cartera_DBN "cartera.fdb" 

// nivel de informacion a utilizar
enum cartera_info_level{
	NAME=1, FULL
};

// definicion del tipo cartera para mas facil manejo
typedef struct{
	char propietario[100];
	float saldo, ingreso_total, gasto_total;
} Cartera;

// operaciones de cartera
void cartera_recopilar(Cartera *, const enum cartera_info_level);
void cartera_mostrar(Cartera *, const enum cartera_info_level);
void cartera_mostrar_todas(Cartera *, const enum cartera_info_level);

// operaciones de base de datos de cartera
FILE *cartera_db_abrir(char *);
void cartera_db_agregar(Cartera *);
int cartera_db_contar();
Cartera *cartera_db_leer();
int cartera_db_buscar(Cartera *, const enum cartera_info_level);
void cartera_db_seleccionar(Cartera *, const enum cartera_info_level);
void cartera_db_actualizar(Cartera *);
void cartera_db_actualizar_tranx(Cartera *);

#include "CARTERA.C"

#endif
