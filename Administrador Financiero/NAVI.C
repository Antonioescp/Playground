#include "NAVI.H"

// muestra un menu de opciones (requiere el numero de opciones en la primera pos)
int navi_menu(cadenas opciones, char *title){
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