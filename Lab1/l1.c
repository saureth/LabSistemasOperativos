#include <stdio.h>

int main()
{	
	float cm, pul;
	printf("Ingrese longitud en centímetros: ");
	scanf("%f",&cm);
	pul = cm/2.54;
	printf("Resultado: %5.2f cm = %5.2f pulg\n ", cm, pul);
	return 0;
}