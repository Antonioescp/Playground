#ifndef _TRANX
#define _TRANX

#include "CARTERA.H"

#define Tranx_DBN "tranx.fdb"

enum tranx_tipo{
	gasto=1, ingreso
};

typedef struct{
	char nombre[50], fecha[100];
	float monto;
	Cartera cartera;
	enum tranx_tipo tipo;
} Transaccion;

//metodos
void tranx_recopilar(Transaccion *);
void tranx_mostrar(Transaccion *);
void tranx_mostrar_todo(Transaccion *);

// base de datos
FILE *tranx_db_abrir(char *);
void tranx_db_agregar(Transaccion *, Cartera *);
int tranx_db_contar();
Transaccion *tranx_db_leer();
void tranx_db_pipe();

#include "TRANX.C"

#endif