#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "morion.h"
#include "parser.h"

int main(){
    char str[200];    
    char ** items;
    int i, num, background;
    int TAM = 100;
    char expresion[TAM];
    do{
        ImprimirCarpeta(); 
        printf("> ");
        scanf("%s", str);      
        
        fgets (expresion, TAM, stdin);      
        num = separaItems (expresion, &items, &background);      
        printf ("Numero de parametros: %d\n", num);      
        for (i=0; i<num; i++){
            printf("%s\n",items[i]);
            /*if(strcmp("mypwd",items[i])==0){
                printf("\n");
                ImprimirCarpeta(); 
                printf("\n");                    
            }*/
        }
        liberaItems (items);
    }

    while(strcmp(str,"myexit")!= 0);

    return 0;
}