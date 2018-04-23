#include<stdio.h>
#define FILENAME "datos.dat"

int main() {

	FILE *dir_buff;
	double dato, tabla[10];
	int usados=0, ok=0;
	int i=0;
	
	ok++;	//evita warning
	usados++;

	dir_buff=fopen(FILENAME, "rb");
	if (dir_buff!=NULL) {

		///...
		 // 2 formas de solicitar leer del archivo 8 bytes y almacenarlos
		 // en la variable local o en el parámetro por valor dato
		ok = fread(&dato, sizeof(dato), 1, dir_buff);
		//mostramos por pantalla
		printf("ok: %d -> %lf\n", ok, dato);
		ok = fread(&dato, sizeof(double), 1, dir_buff);
		//mostramos por pantalla
		printf("ok: %d -> %lf\n", ok, dato);

		 // ok valdrá 1, si no ha habido errores,

		 // solicita leer del archivo 80 bytes y almacenarlos en la variable local tabla,
		 //  no se puede usar si tabla es un parámetro,
		ok = fread(&(tabla[0]), sizeof(tabla), 1, dir_buff);
		ok = fread(tabla, sizeof(tabla), 1, dir_buff);
		// ok valdrá 1, si no ha habido errores
		//mostramos
		for (i=0;i<10;i++) {
			printf("\ttabla[%d]=%lf\n", i, tabla[i]);
		}

	
	 	// solicita leer del archivo 10 datos de 8 bytes y almacenarlos
	 	// en la variable local o en el parámetro por referencia tabla
		usados = fread(&(tabla[0]), sizeof(double), 10, dir_buff);
		usados = fread(tabla, sizeof(double), 10, dir_buff);
		// usados valdrá cuantos datos ha leído sin errores (<=10)

		// solicita leer en el archivo los 32 bytes (4 bloques de 8 bytes)
		 // de los datos de la variable local o del parámetro por referencia tabla 
		
		usados = fread(&(tabla[0]), sizeof(double), 4, dir_buff);
		usados = fread(tabla, sizeof(double), 4, dir_buff);

		//mostramos
		printf("\nUsados: %d", usados);
		printf("\nValores usados:\n");
		for (i=0;i<usados;i++) {
			printf("\ttabla[%d]=%lf\n", i, tabla[i]);
		}


		fclose(dir_buff);
	}
	return 0;
}
