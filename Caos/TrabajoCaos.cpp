#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define NormRANu (2.3283063671E-10F)




const int N=200; //Número de osciladores
const int pasos=2000; //Número de iteraciones de la simulación
const double deltat=0.05; //Intervalo temporal de la simulación
const double pi=3.14159265358979323846; //número pi
const double K=5.0; //parámetro de orden
const int red=2; //1: ER ; 2: BA ; 3:Todo 1

///CAbeceras de funciones que vamos a utilizar

        ///Números aleatorios, los pongo antes del main porque dependen de variables globales
        //Esta funciones las guardo de cuando fisica computacional.
            unsigned int irr[256];
            unsigned int ir1;
            unsigned char ind_ran, ig1, ig2, ig3;

            void ini_ran(int SEMILLA){
                int INI, FACTOR, SUM, i;
                srand(SEMILLA);
                INI = SEMILLA;
                FACTOR = 67397;
                SUM = 7364893;
                for (i = 0; i < 256; i++)
                {
                    INI = (INI * FACTOR + SUM);
                    irr[i] = INI;
                }
                ind_ran = ig1 = ig2 = ig3 = 0;
            }

            double Random(void){
                double r;
                ig1 = ind_ran - 24;
                ig2 = ind_ran - 55;
                ig3 = ind_ran - 61;
                irr[ind_ran] = irr[ig1] + irr[ig2];
                ir1 = (irr[ind_ran] ^ irr[ig3]);
                ind_ran++;
                r = ir1 * NormRANu;
                //printf("r=%f\n",r);
                return r;
            }
        ///Fin números aleatorios (eso si, acordarse de inicializar la semilla)

//

void RK(double *fases, double *frecuencias, int A[N][N]);
double frand(double a, double b); // a: inicio, b: fin
double calcular_r(double *fases); //calcular parámetro de orden.

///Programa principal
int main(){

double frecuencias[N]; //las w_i
double fases[N]; //las \theta_i
int A[N][N]; //Matriz de adyacencia

//Inicializamos a 0 para quitarnos problemas
for(int i=0;i<N;i++){
    frecuencias[i]=0;
    fases[i]=0;
    for(int j=0; j<N ;j++){
        A[i][j]=0;
    }
}
//Iniciar la semilla para generar los números aleatorios
ini_ran(time(NULL));

///Generamos la distribución uniforme de frecuencias entre -0.5 y 0.5 y de fases entre 0 y 2pi

//Escribimos la distribución de frecuancias y la de fases
for(int i=0;i<N;i++){
    frecuencias[i]=frand(-0.5,0.5);
    fases[i]=frand(0,2*pi);
}

///Elegimos la matriz de adyacencia según la red
//ER
if(red==1){
    FILE *f1=fopen("matriz_BA.txt","r");
    if(f1==NULL){
        printf("Error al abrir el archivo.\n");
        return 1;
    }
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            fscanf(f1,"%d",&A[i][j]);
        }
    }
    fclose(f1);
}
//BA
if(red==2){
    FILE *f2=fopen("matriz_ER.txt","r");
    if(f2==NULL){
        printf("Error al abrir el archivo.\n");
        return 1;
    }
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            fscanf(f2,"%d",&A[i][j]);
        }
    }
    fclose(f2);
}
//Todo 1
if(red==3){
for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
        A[i][j]=1;
    }
}
}

///Realizamos la simulación RK y vamos guardando los datos en un fichero para poder representarlos

// Abriendo el archivo para guardar los resultados
FILE *fp=fopen("EvolucionFases.txt","w");
if(fp==NULL){
    printf("Error al abrir el archivo.\n");
    return 1;
}

//Termalizamos

for(int i=0;i<1000;i++){
    RK(fases,frecuencias,A,K);
}

for(int l=0;l<pasos;l++){
    fprintf(fp,"%.6f\t",l*deltat);
    RK(fases,frecuencias,A);
    for(int j=0;j<N;j++){
        fprintf(fp,"%.6f\t",fases[j]);
    }
    fprintf(fp, "\n");
}

fclose(fp);

printf("Parametro de orden: %f . \n", calcular_r(fases));


return (0);
}

//Runge_Kutta orden 4 Kuramoto
void RK(double *fases, double *frecuencias, int A[N][N]){
    double K1[N],K2[N],K3[N],K4[N];
    double fasesTemp[N]; //Fases itermedias para hacer los updates en paralelo

    //Calculamos K1
    for(int i=0;i<N;i++){
        double suma=0;
        for(int j=0;j<N;j++){
            suma+=A[i][j]*sin(fases[j]-fases[i]);
        }
        K1[i]=deltat*(frecuencias[i]+(K/N)*suma);
    }

    //Calculamos K2
    for(int i=0;i<N;i++){
        fasesTemp[i]=fases[i]+0.5*K1[i];
    }
    for(int i=0;i<N;i++){
        double suma=0;
        for(int j=0;j<N;j++){
            suma+=A[i][j]*sin(fasesTemp[j]-fasesTemp[i]);
        }
        K2[i]=deltat*(frecuencias[i]+(K/N)*suma);
    }

    //Calculamos K3
    for(int i=0;i<N;i++){
        fasesTemp[i]=fases[i]+0.5*K2[i];
    }
    for(int i=0;i<N;i++){
        double suma=0;
        for(int j=0;j<N;j++){
            suma+=A[i][j]*sin(fasesTemp[j]-fasesTemp[i]);
        }
        K3[i]=deltat*(frecuencias[i]+(K/N)*suma);
    }

    //Calculamos K4
    for(int i=0;i<N;i++){
        fasesTemp[i]=fases[i]+K3[i];
    }
    for(int i=0;i<N;i++){
        double suma=0;
        for(int j=0;j<N;j++){
            suma+=A[i][j]*sin(fasesTemp[j]-fasesTemp[i]);
        }
        K4[i]=deltat*(frecuencias[i]+(K/N)*suma);
    }

    // Actualizar las fases finales
    for(int i=0;i<N;i++){
        fases[i]=fases[i]+(K1[i]+2*K2[i]+2*K3[i]+K4[i])/6.0;
        // Normalizar al intervalo a [0, 2pi)
        fases[i] = fmod(fases[i], 2*pi);
        if (fases[i] < 0) fases[i] += 2*pi;
    }
}

//Generar números aleatorios
double frand(double a, double b){ // a: inicio, b: fin
    double r = a + (b-a) * Random();
    return r;
}

double calcular_r(double *fases) {
    double sum_cos = 0.0, sum_sin = 0.0;
    for(int i=0; i<N; i++) {
        sum_cos += cos(fases[i]);
        sum_sin += sin(fases[i]);
    }
    double r = sqrt((sum_cos*sum_cos + sum_sin*sum_sin)) / N;
    return r;
}
