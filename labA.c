/*
Programa:	labA.c
Sinopsis:	Práctica del tema A. Menú para la gestiónde un fichero binario
		que contiene números enteros.
@author	:	Paco González Moya
@version:	1.0
@date 	:	May 2018
*/

#include<stdio.h>
#include<stdlib.h>

//Constantes utilizadas
#define MAX 50				//Dimensión máxima nombre de fichero
#define MAX_INFO 5			//Dimensión máxima del vector de información sobre el fichero				
#define NUM_VALORES_PRECARGA 10		//Máximo de valores en fichero en la precarga
#define NOMFICH "mis_numeros.dat"	//Nombre del fichero
#define MIN_VALOR 1			//Valor mínimo del rango de valores permitidos
#define MAX_VALOR 100			//Valor máximo del rango de valores permitidos

//Una forma de administrar los mensajes del menú
//Opciones del menú
#define MNU_ITEMS 6
#define MNU_EXIT '0'
#define MNU_OP1 '1'
#define MNU_OP2 '2'
#define MNU_OP3 '3'
#define MNU_OP4 '4'
#define MNU_OP5 '5'

//Mensajes del menú
#define MNU_TX01_ES "1. Muestra todos los datos del archivo\n"
#define MNU_TX02_ES "2. Añade un dato al final del archivo\n" 
#define MNU_TX03_ES "3. Muestra el dato i del archivo\n"
#define MNU_TX04_ES "4. Cambia el último dato mostrado o escrito\n"
#define MNU_TX05_ES "5. Precarga del fichero con set de prueba\n"
#define MNU_TX00_ES "0. Salir\n"

//Mensajes de error tipificados
#define ERR_MSG_01_ES "No se ha podido mostrar el fichero\n" 
#define ERR_MSG_02_ES "No se ha podido mostrar añadir %d al fichero\n" 
#define ERR_MSG_03_ES "No se ha podido mostrar el valor seleccionado\n" 
#define ERR_MSG_04_ES "No se ha podido modificar el valor %d en la posición %d\n" 
#define ERR_MSG_05_ES "No se ha podido precargar el fichero\n" 
#define ERR_MSG_14_ES "No es posible modificar la posición %d\n"
#define ERR_MSG_99_ES "Opción no válida\n"

//Mensajes de información tipificados
#define NFO_MSG_00_ES "Teclee un número entero entre %d y %d\n"
#define NFO_MSG_01_ES "Pulse una tecla para continuar\n"
#define NFO_MSG_11_ES "Valor %3d: %3d\n"
#define NFO_MSG_02_ES "Valor %d añadido al final del fichero\n"
#define NFO_MSG_03_ES "Valor leído: %d en posición %d\n"
#define NFO_MSG_04_ES "Teclee un nuevo valor para la posición %d (actual %d)\n"

/*****************************************
	Funciones de gestión de ficheros
*******************************************/

//Apertura del fichero
int abrirFichero (FILE **fich, char nomarch[MAX], char *modo);

//Escritura en el fichero
int escribeFichero (FILE *fich, int valor, int index);

//Cierre del fichero
int cerrarFichero (FILE *fich);

//Función que ofrece cietas informaciones sobre el fichero
int numValoresFichero(FILE *fich, int tamano_tipo_base, int info[MAX_INFO]);


/*******************************************
	Funciones relacionadas con menú y 
	opciones
*******************************************/

//Menu de opciones
void menu();

//opción 1: Mostrar los datos del fichero
int mostrarFichero (FILE *fich);

//opción 2: añade un datos al final del fichero
int anadirFinalFichero (FILE *fich, int nuevo_valor);

//opción 3: muestra el dato i del archivo
int mostrarPosicionFichero (FILE **fich, int posicion);

//opción 4: cambia el último dato leído o mostrado
int modificarPosicionFichero (FILE **fich, int posicion, int nuevo_valor);

//opción 5: Hace una precarga de datos en el fichero (opcional)
int precargar (FILE *fich);


/*******************************************
	Funciones auxiliares

*******************************************/

//Función que aplica el método de cálculo de números a añadir (precarga)
int generaValor (int semilla);

//Función que pide un número entero por consola entre unos límites
int pedirNumero (int min, int max);


/**
	int main()

Objetivo:	Función main. 

@param		nada	
@return 	0 si ok
*/

int main() {
	//La función main queda reducida a su mínima expresión
	//Llamada a menu
	menu();		
	return 0;
}


/**
	int menu()

Objetivo:	Mostrar el menú de opciones y ejecutar las diversas opciones
		según la selección del usuario

@param		nada	
@return 	nada
*/
void menu() {
	//Vector de mensajes del menú
	char items[MNU_ITEMS][MAX]={MNU_TX01_ES, MNU_TX02_ES, MNU_TX03_ES, MNU_TX04_ES, MNU_TX05_ES, MNU_TX00_ES};
	
	int i;			//Variable de control de bucle
	int ok;			//Variable de control de valores de scanf
	char opcion=32;		//Selector de la opción tecleada
	char car=32;
	FILE *fich;		//Apuntador al buffer del fichero
	int resp=-1;		//Variable de respuesta co código de éxito o error

	int ultima_posicion=-1;	//Última posición accedida del fichero en 
				//lectura o en escritura

	int valor;		//Valor numérico leído o escrito
	int posicion=-1;	//Posición del fichero que será acccedida

	int info[MAX_INFO];	//Vector con información relevante sobre el 
				//fichero de números.

	do  {
		//Mostramos el menú de opciones. Rótulo inicial.
				
		printf("+--------------------------------------------------+\n");
		printf ("|\t\tMENU DE OPCIONES\t\t   |\n");
		printf("+--------------------------------------------------+\n");

		//Mostramos el menú
		for (i=0; i< MNU_ITEMS;i++) {
			printf("\t%s", items[i]);
		}
		printf("----------------------------------------------------\n");

		//lectura de la opción
		ok+=scanf("%c", &opcion);
	
		switch(opcion) {
			case MNU_EXIT:	//Opción de salida
					break;
			case MNU_OP1://Opción 1: mostrar todos los datos del fichero
				resp=abrirFichero(&fich, NOMFICH, "rb");
				if (resp==0) {
					//muestra el contenido del fichero
					resp=mostrarFichero (fich);
					if (resp==0) {
						//Capturamos la última posición
						resp =numValoresFichero(fich, sizeof(int), info);
						ultima_posicion=info[1];
					}
					//Cerramos el fichero
					cerrarFichero (fich);	
				}
				//Si se ha capturado algún error, mensaje de error
				if (resp!=0)
					printf(ERR_MSG_01_ES);	 
				break;
			case MNU_OP2://Opción 2: añadir un valor al final del fichero

				//Abrimos el fichero en  modo append
				resp=abrirFichero(&fich, NOMFICH, "ab");
				if (resp==0) {
					//Pide el valor en el rango oportuno
					valor=pedirNumero(MIN_VALOR, MAX_VALOR);
					if (valor != -1) {
						//Añadimos al fichero
						resp=anadirFinalFichero(fich, valor);
						if (resp==0) {
							//Correcto
							printf(NFO_MSG_02_ES, valor);
							//Capturamos la última posición
							resp =numValoresFichero(fich, sizeof(int), info);
							ultima_posicion=info[1];	
						} else {
							printf(ERR_MSG_02_ES, valor);

							}
						} else {
							printf(ERR_MSG_02_ES, valor);
						}
							
					}
					//Cerramos el fichero
					cerrarFichero (fich);	

					if (resp>0) 
						printf(ERR_MSG_02_ES, valor);
					break;
	
					
			//OPción 3: mostrar el valor en la poxición i-ésima
			case MNU_OP3:	
					//Abrimos el fichero en lectura
					resp=abrirFichero(&fich, NOMFICH, "rb");
					if (resp==0) {
						//Captura cierta información sobre el fichero
						resp =numValoresFichero(fich, sizeof(int), info);
						if (resp==0) {
							//Pide el valor en el rango oportuno
							posicion=pedirNumero(1, info[1]);
							//Obtiene el valor de la posición dada
							valor=mostrarPosicionFichero(&fich, posicion);
							if (valor>-1) {
								printf(NFO_MSG_03_ES, valor, posicion);
								//Conserva la última posición
								ultima_posicion=posicion;
							}	
	
						}
						//Cerramos el fichero
						resp=cerrarFichero(fich);
					}
					//Notificamos si hay error
					if (resp>0) {
						printf(ERR_MSG_03_ES);
					}
					break;
			
			//Opción 4: cambiar el último valor leído o escrito
			case MNU_OP4:
					resp=abrirFichero(&fich, NOMFICH, "r+b");
					if (resp==0) {
						if (ultima_posicion >0) {
							valor=mostrarPosicionFichero(&fich, ultima_posicion);
							printf(NFO_MSG_04_ES, ultima_posicion, valor);
							//Pide el valor en el rango oportuno
							valor=pedirNumero(MIN_VALOR, MAX_VALOR);
							if (valor>0) {
								//Modificamos en el fichero
								resp=modificarPosicionFichero (&fich, ultima_posicion, valor);
								//resp=pruebas();
							} else 	
								printf(ERR_MSG_04_ES, valor, ultima_posicion);
						} else {
							printf(ERR_MSG_14_ES, ultima_posicion);

						}
						//Cerramos el fichero: importante
						resp=cerrarFichero(fich);
							

					} else {
						printf(ERR_MSG_04_ES, valor, ultima_posicion);
					}
					break;
					
			
			//Opción 5: es adicional, pero util poder precargar el fichero
			case MNU_OP5: 	
					resp=abrirFichero(&fich, NOMFICH, "wb");
					if (resp==0) {
						printf("precargar...\n");
						//Hacemos la precarga
						resp=precargar(fich);
						resp+=cerrarFichero(fich);

						if (resp==0) {
							
							//Abrimos en modo lectura
							resp=abrirFichero(&fich, NOMFICH, "rb");
							if (resp==0) {

								//Capturamos la última posición
								resp =numValoresFichero(fich, sizeof(int), info);
								ultima_posicion=info[1];
								//Cerramos el fichero
								resp=cerrarFichero(fich);

							}
						}
					}
					//Notificamos error
					if (resp<0)
						printf(ERR_MSG_05_ES);	 
					break;
			default:
				//Opción distinta mensaje de error
				printf(ERR_MSG_99_ES);
				break;	//lo ponemos por convención, pero en este lugar ya no es preciso
		};	
		printf(NFO_MSG_01_ES);
		scanf("%c", &car);

	} while (opcion!= MNU_EXIT );
	if (opcion!=MNU_EXIT)
		printf("--------------------------------\n");
	return;

}

/**
	int abrirFichero (FILE **fich, char nomarch[MAX], char *modo);

Objetivo:	apertura del fichero apuntado por fich en el modo establecido
		por modo
@param		FILE **fich 	E/S	puntero al buffer del fichero (cambia)
@param		nomarch		E	nombre del fichero
@param		modo		E	modo de apertura	
@return 	0 si ok
		1 si no se puede abrir			
*/
int abrirFichero (FILE **fich, char nomarch[MAX], char *modo) {
	*fich=fopen(nomarch, modo);
	return (*fich==NULL);	//Una forma compacta de devolver 0 si ok o bie 1 si error
}

/**
	int cerrarFichero (FILE *fich);

Objetivo:	cierre del fichero apuntado por fich
@param		FILE *fich 	E/S	puntero al buffer del fichero
@return 	0 si ok
		1 si no se puede cerrar			
*/

int cerrarFichero (FILE *fich) {
	return (fclose(fich));
}

/**
 	int mostrarFichero(FILE *fich) 

Objetivo:	muestra por consola el contenido del fichero apuntado por fich

@param		FILE *fich 	E/S	puntero al buffer del fichero (cambia)
@return 	0 si ok
		1 si no se puede mostrar	
*/

int mostrarFichero(FILE *fich) {
	int ok=0;		//Variable para control de fread
	int contador=1;		//Variable contador de elementos
	int numero;		//Variable donde se lee el valor del fichero

	if (fich!= NULL) {
		if (!feof(fich)) {	//Testamos EOF
			//Lectura adelantada
			ok = fread(&numero, sizeof(numero), 1, fich);
			while (!feof(fich)) {
				if (ok>0) {
					printf(NFO_MSG_11_ES, contador, numero);
					contador++;
				}
				ok = fread(&numero, sizeof(numero), 1, fich);
			}
		 
		}
	}
	return ok;
}


/**
 	int anadirFinalFichero (FILE *fich, int nuevo_valor) 

Objetivo:	Añade un nuevo valor al final del fichero (append)

@param		FILE *fich	E/	puntero al buffer del fichero
@param		int nuevo_valor	E/	valor a añadir

@return		0 si correcto
		-1 si error 	
*/

int anadirFinalFichero (FILE *fich, int nuevo_valor) {
	int ok=-1;	//Variable para control de fwrite

	//Escribimos en el fichero
	if (fich!=NULL) {
		ok=fwrite(&nuevo_valor, 1, sizeof(nuevo_valor), fich);
		if (ok>0)
			ok=0;	//Hacemos esto porque por convención se suele devolver cero si tood ha ido bien

	}
	return ok;
}


/**
 	int  generaValor (int semilla) 

Objetivo:	Genera un nuevo valor para el fichero	
		Se ha seguido un algoritmo sencillo que consiste en multiplicar 
		por tres un valor "semilla" que se porporciona

@param		semilla 	E	valor empleado como semilla
@return 	el valor generado
*/

int generaValor (int semilla) {
	const int multiplicador=3;	//Multiplicador para el cálculo del valor
	//Cálculo sencilo de un valor: el triple de la semilla
	return multiplicador*semilla;
}


/**
 	int  precargar (FILE **fich, char nomarch[MAX]) 
Objetivo:	

@param		FILE **fich 	E/S	puntero al buffer del fichero (cambia)
@param		nomarch		E	nombre del fichero
@return 	0 si ok
		1 si no se puede precargar
*/

int precargar (FILE *fich) {
	int ok=1;		//Variable para el control de escritura
	int numero;		//Valor que se desea escribir
	int i;			//Variable de control de bucle

	if (fich != NULL) {
		//Hacemos el bucle para cargar valores
		for (i=0;i<NUM_VALORES_PRECARGA && ok>0;i++) {
			numero=generaValor(i);	//forma de calcular el número a cargar
			ok=fwrite(&numero, sizeof(numero),1, fich);
		}
		if (ok>0)
			ok=0;	//Hacemos esto porque por convención se suele devolver cero si tood ha ido bien
		
	}
	return ok;

}

/**
 	int mostrarPosicionFichero (FILE **fich, char nomarch[MAX], int posicion) 

Objetivo:	Retorna el valor que ocupa la posición i-ésima en el fichero
		posición apuntada por "posicion"	

@param		FILE **fich 	E/	puntero al buffer del fichero
@param		int posicion	E/	posición que se desea leer
@return 	valor de la posición si ok
		-1 si no se encuentra
*/

int mostrarPosicionFichero (FILE **fich, int posicion) {
	
	int numero=-1;		//Valor leído
	int ok=-1;		//Variable para control de lectura por fread
	int encontrado=0;	//Variable bandera 

	if (*fich!=NULL) {
		//Hacemos un acceso directo a la posición: de antemano validada
		ok=fseek(*fich, sizeof(numero)*(posicion-1), SEEK_SET);
		if (ok==0) {
			//Leemos el valor
			ok = fread(&numero, sizeof(numero), 1, *fich);
			if (ok>0) {
				//Variable bandera poara indicar que se ha encontrado
				encontrado=-1;
			}
		}
	}
	if (encontrado==-1)
		return numero;
	else
		return -1;


}

/**
 		int  pedirNumero () 

Objetivo:	Pide por consola un número entrero en un rango prefijado

@param		min	E/ 	valor mìnimo admisible del rango 
		max	E/	valor máximo admisible del rango
@return 	numero si está en el rango [min..max]
		-1 en caso de error
*/

int pedirNumero(int min, int max) {
	int numero=-1;		//varible del número a leer
	int ok=-1;		//control para scanf	
	int intentos=0;		//número máximo de intentos

	const int max_intentos=5;//número máximo de lecturas a practicar

	//Pedimos un valor entre min y max mientras y persistimos
	//mientras el dato no esté en rango
	//o bien no se teclea nada valido y no superamos el número max_intentos
	do {
		printf(NFO_MSG_00_ES, min, max);
		ok=scanf("%d", &numero);
		intentos++;
	} while (numero<min || numero > max || (ok<1 && intentos < max_intentos));
	
	//Retornamos el número o valor de error 
	if (numero>=min && numero <= max)
		return numero;
	else
		return -1; 

}


/**
 	
		int numValoresFichero(FILE *fich, int info[MAX_INFO])

Objetivo:	Ofrece cierta información elevante sobre el fichero apuntado
		por fich

@param		FILE *fich		E/ 	puntero al buffer del fichero
@param 		int tamano_tipo_base	E/	tamaño en bytes del tipo base
@param 		int info[MAX_INFO]	E/S	array de valores relevantes
						[0] -> tamaño en bytes 
						[1] -> número de valores
						[2] -> indicador de discrepancia
						
@return		0 en caso de éxito	
		-1 en caso de error
*/

int numValoresFichero(FILE *fich, int tamano_tipo_base,  int info[MAX_INFO]) {
	int ok=-1;	//variable de control error

	if (fich!=NULL) {
		fseek(fich, -tamano_tipo_base, SEEK_END);
		info[0] = ftell(fich)+tamano_tipo_base;
		info[1] = info[0] / tamano_tipo_base ;
		info[2] = info[0] > info[1]*tamano_tipo_base;
		ok=0;
	}
	return ok;
}


/**
 	
		int modificarPosicionFichero (FILE *fich, int posicion, int nuevo_valor) 

Objetivo:	Modifica el valor que opcula la posicón dada en el fichero

@param		FILE *fich		E/ 	puntero al buffer del fichero
@param 		int poisicion		E/	posición que ocupa el valor a cambiar
@param 		int nuevo_valor		E/	valor que reemplaza al existente
						
@return		0 en caso de éxito	
		-1 en caso de error
*/

int modificarPosicionFichero (FILE **fich, int posicion, int nuevo_valor) {
	int ok=-1;	//VAriable de control de fseek o fwrite
	
	if (*fich != NULL) {
		ok=fseek(*fich, sizeof(nuevo_valor)*(posicion-1), SEEK_SET);
		if (ok==0) {
			ok=fwrite(&nuevo_valor, sizeof(nuevo_valor), 1, *fich);
			//printf("ok->%d %d\n", ok, nuevo_valor);
			if (ok>0)
				ok=0;//Hacemos esto porque por convención se suele devolver cero si tood ha ido bien

		}
	}
	return ok;

}


