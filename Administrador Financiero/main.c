#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "TRANX.H"
#include "NAVI.H"

enum op_cart{
	nueva=1, seleccionar, ver_carteras, salir
};

enum op_info{
	nombre=1, inf_full
};

enum op_tranx{
	nueva_tranx=1, ver_tranx, menu_anterior
};

// programa principal
void main() {
	// opciones
	cadenas op_carteras = {"4", "Nueva", "Seleccionar", "Ver Carteras", "Salir"};
	cadenas op_info = {"2", "Nombre", "Informacion completa"};
	cadenas op_transacciones = {"3", "Nueva", "Ver Todas", "Menu Anterior"};
	
	// seleccion
	bool seleccionada = false;
	
	// operaciones con cartera
	Cartera cartera;
	Cartera *carteras=NULL;
	
	// operaciones con transacciones
	Transaccion transaccion;
	Transaccion *transacciones=NULL;
	
	while(1){
		// carteras
		int op = navi_menu(op_carteras, "Carteras");
		if(op == salir) break;
		system("cls");
	
		switch(op){
			case nueva:
				puts("*********Cartera Nueva*********");
				cartera_recopilar(&cartera, FULL);
				cartera_db_agregar(&cartera);
				
				seleccionada = true;
				
				system("pause");
				break;
			case seleccionar:
				puts("*********Buscar y Seleccionar*********");
				op = navi_menu(op_info, "Buscar por");
				
				cartera_db_seleccionar(&cartera, op);
				
				seleccionada = true;
				
				system("pause");
				break;
			case ver_carteras:
				puts("*********Ver Carteras*********");
				op = navi_menu(op_info, "Mostrar");
				
				free(carteras);
				carteras = cartera_db_leer();
				
				cartera_mostrar_todas(carteras, op);
				system("pause");
				break;
			default:
				puts("Opcion invalida");
		}
		
		if(seleccionada){
			transaccion.cartera = cartera;
			while(1){
				
				op = navi_menu(op_transacciones, "Transacciones");
				if(op == menu_anterior){
					seleccionada = false;
					break;
				}
				
				system("cls");
					
				switch(op){
					case nueva_tranx:
						puts("*********Nueva transaccion*********");
						tranx_recopilar(&transaccion);
						tranx_db_agregar(&transaccion, &cartera);
						break;
					case ver_tranx:
						puts("*********Todas las transacciones*********");
						tranx_db_pipe(&cartera);
						
						
						printf("Cartera Actualmente:\n");
						cartera_mostrar(&cartera, FULL);
						puts("*************************");
						system("pause");
						break;
					default:
						puts("opcion invalida!");
				}
			}
		}
		
	}
}
