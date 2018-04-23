#include<stdio.h>
#define FILENAME "datos.dat"

int main() {
	FILE *dir_buff;
	double dato, tabla[10] = {1.0, 2.0, 3.0, 4.0};
	int usados = 4, ok=0;
	
	ok++;	//evita warning
	dir_buff=fopen(FILENAME, "wb");
	if (dir_buff!=NULL) {

	dato = 12.3456;	//damos un valor de pruebas

	///...
	  // dos formas de solicitar escribir en el archivo los 8 bytes
	  // de la variable local o del parámetro por valor dato,
		ok = fwrite(&dato, sizeof(dato), 1, dir_buff);
		printf("ok: %d -> escrito: %lf\n", ok, dato);
		ok = fwrite(&dato, sizeof(double), 1, dir_buff);
		printf("ok: %d -> escrito: %lf\n", ok, dato);

	  // ok valdrá 1, si no ha habido errores,

	 // solicita escribir en el archivo los 80 bytes
	 // de la variable local tabla, no se puede usar si tabla es un parámetro,
		ok = fwrite(&(tabla[0]), sizeof(tabla), 1, dir_buff);
		ok = fwrite(tabla, sizeof(tabla), 1, dir_buff);
	 // ok valdrá 1, si no ha habido errores

	 // solicita escribir en el archivo los 80 bytes (10 bloques de 8 bytes)
	 // de la variable local o del parámetro por referencia tabla
		ok = fwrite(&(tabla[0]), sizeof(double), 10, dir_buff);
		ok = fwrite(tabla, sizeof(double), 10, dir_buff);
	 // ok valdrá 10, si no ha habido errores

	 // solicita escribir en el archivo los 32 bytes (4 bloques de 8 bytes)
	 // de los datos de la variable local o del parámetro por referencia tabla 
		ok = fwrite(&(tabla[0]), sizeof(double), usados, dir_buff);
		ok = fwrite(tabla, sizeof(double), usados, dir_buff);
	 // ok valdrá 4, si no ha habido errores

		fclose(dir_buff);
	}

	return 0;
}
