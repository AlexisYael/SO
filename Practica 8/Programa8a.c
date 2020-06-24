#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	int bitP;//Bit presente ausente
	int num;//Correspondencia en la memoria fisica
}Pagina;
typedef struct
{
	int pagina;//La pagina que ocupa el marco de pagina
}Marco;

int mov_reg(int dir);
int fallo_de_pagina();
void inicializarTabla();
void inicializarRAM();
void setPagina(int num,int bp,int pag);
void setMarco(int num,int pag);
int elevar(int a,int b);
void imprimirRAM();
void imprimirTabla();

Marco RAM[8];
Pagina Tabla[16];
int turno;

int main(){
	int op,val;
	inicializarTabla();
	inicializarRAM();
	turno=0;//turno que se escogera en caso de remplazo por fallo de pagina
	printf("Bienvenido a su memoria, ¿Qué desea hacer?\n\n");
	printf("1.- Mostrar la RAM. \n");
	printf("2.- Mostrar la Tabla de paginas.\n");
	printf("3.- Convertir a memoria física.\n");
	printf("4.- Salir.\n");
	while(1){
		printf("\nIngresa el número de la operación arealizar:");
		scanf("%d",&op);
		switch(op){
			case 1:
				imprimirRAM();
			break;
			case 2:
				imprimirTabla();
			break;
			case 3:
				printf("Ingresa la dirección de memoria virtual: ");
				scanf("%d",&val);
				printf("̣̣̣̣Su dirección en memoria fisica es: %d\n",mov_reg(val));
			break;
			case 4:
				return 0;
			break;
			default:
				printf("Ingrese una opción del menú.\n");
			break;

		}
	}

}
void imprimirRAM(){
	for (int i = 0; i <= 7; ++i)
	{
		if (RAM[i].pagina!=-1)
		{
			printf("Marco de pagina %d - Tiene la pagina: %d\n",i,RAM[i].pagina);
		}else{
			printf("Marco de pagina %d se encuentra vacio.\n",i);
		}
	}
}
void imprimirTabla(){
	for (int i = 0; i <= 15; ++i)
	{
		if (Tabla[i].bitP!=0)
		{
			printf("Pagina %d tiene el Marco: %d\n",i,Tabla[i].num);
		}else{
			printf("Pagina %d se encuentra vacia.\n",i);
		}
	}
}
int mov_reg(int dir){//Recibe la dirección de memoria virtual y la mueve a la memoria fisica, regresa la nueva dirección generada
	int cont =0;
	int np=0,d=0;//Numero de pagina y de desplazamiento en decimal
	for (int i = 11; i >= 0; --i)//Obtenemos el numero de pagina en binario
	{
		if((dir%2)==1){
			d=d+elevar(2,cont);
		}
		dir=dir/2;
		cont++;
		//printf("%d %d\n",np,dir%2 );

	}
	cont =0;
	for (int i = 3; i >= 0; --i)//Obtenemos el valor del desplazamiento
	{
		if(dir%2==1){
			np=np+elevar(2,cont);
		}
		dir=dir/2;
		cont++;
	}
	if (Tabla[np].bitP==1)
	{
		RAM[Tabla[np].num].pagina=np;
		return d+4096*Tabla[np].num;
	}else{//Existe fallo de pagina
		cont=fallo_de_pagina();
		setMarco(cont,np);
		Tabla[np].num= cont;setPagina(np,1,cont);
		return d+4096*cont;
	}
	
}
int fallo_de_pagina(){//Regresara numero del marco de pagina que desalojo
	int c=0;
	setMarco(turno,-1);//Desalojamos el marco de pagina
	for (int i = 0; i < 16; ++i)
	{
		if (Tabla[i].num==turno && Tabla[i].bitP==1)//Quitamos la referencia de la memoria fisica en la Tabla 
		{
			printf("Se regreso el contenido de la pagina %d a la memoria virtual \n",i);
			setPagina(i,0,0);
		}
	}
	if(turno==7){//Marco que se haya alojado 
		turno=0;
		return 7;
	}else{
		turno++;
		return turno-1;
	}

}
void inicializarTabla(){//Inicializamos la Tabla de paginas, ninguna tiene asignada un dirección de memoria fisica
	setPagina(0,0,0);
	setPagina(1,0,0);
	setPagina(2,0,0);
	setPagina(3,0,0);
	setPagina(4,0,0);
	setPagina(5,0,0);
	setPagina(6,0,0);
	setPagina(7,0,0);
	setPagina(8,0,0);
	setPagina(9,0,0);
	setPagina(10,0,0);
	setPagina(11,0,0);
	setPagina(12,0,0);
	setPagina(13,0,0);
	setPagina(14,0,0);
	setPagina(15,0,0);
}
void inicializarRAM(){//Inicializamos la RAM, ningun espacio tiene una direccion virtual
	setMarco(0,-1);
	setMarco(1,-1);
	setMarco(2,-1);
	setMarco(3,-1);
	setMarco(4,-1);
	setMarco(5,-1);
	setMarco(6,-1);
	setMarco(7,-1);
}

void setPagina(int num,int bp,int pag){
	Tabla[num].bitP=bp;
	Tabla[num].num=pag;
}
void setMarco(int num,int pag){
	RAM[num].pagina=pag;
}
int elevar(int a,int b){
	if(b==0){
		return 1;
	}else
	return a*elevar(a,b-1);
}
