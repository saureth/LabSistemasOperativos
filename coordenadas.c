#include <stdio.h>
#include <math.h>

//para compilar: gcc -Wall coordenadas.c -lm -o coord

//Declaraciones
double calculateDist(double x1, double y1, double x2, double y2);
void printPoint(double x, double y);

int main()
{
	double x1, x2, y1, y2;

	printf("Ingrese primer punto: ");
	scanf("(%lf,%lf)",&x1,&y1);
	printf("Ingrese segundo punto: ");
	scanf("(%lf,%lf)",&x2,&y2);

	/*
	printf("Ingrese primer punto: ");
	scanf("%lf%lf",&x1,&y1);
	printf("Ingrese segundo punto: ");
	scanf("%lf%lf",&x2,&y2);*/
	printf("La distancia entre ");
	printPoint(x1,y1);
	printf(" y ");
	printPoint(x2,y2);
	double c = calculateDist(x1,y1,x2,y2);
	printf(" es: %lf \n", c);
	return 0;
}

double calculateDist(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;
	double d = sqrt(pow(dx,2) + pow (dy,2));
	return d;
}

void printPoint(double x, double y)
{
	printf(" (%.2lf , %.2lf)", x, y);
}