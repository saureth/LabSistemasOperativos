#include <stdio.h>

//Declaraciones
float cm2Pulg(float cm);

int main()
{	
	float cm, pul;
	printf("Ingrese longitud en centímetros: ");
	scanf("%f",&cm);
	pul = cm2Pulg(cm);
	printf("Resultado: %5.2f cm = %5.2f pulg\n ", cm, pul);
	return 0;
}

//Definiciones
float cm2Pulg(float cm){
	return cm/2.54;
}