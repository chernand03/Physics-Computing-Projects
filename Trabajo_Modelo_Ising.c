#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include<time.h>

#define RANDOM (rand()/((double)RAND_MAX+1)) //Para generar los números aleatorios entre [0,1)
#define L 24 //variable global así no la tengo que definir todo el rato o meterla como cabecera en las funciones. La tengo que cambiar para cada caso

///CABECERAS DE FUNCIONES

//cabecera de la función para crear xp,xm, yp, ym:
void funciones_xy_adelante_atras(int *xp,int *xm,int *yp,int *ym);

//cabecera para la configuración de todo unos
void configuracion_uno(char *spines);

//cabecera de la configuración todos menos uno
void configuracion_menos_uno(char *spines);

//cabecera configuracion aleatoria
void configuracion_aleatoria(char *spines);

//cabecera de la función para elegir la configuración
void elegir_configuracion_inicial(int x,char *spines);

//cabecera función valor absoluto de un número
double valor_absoluto(double numero);

//cabecera función para m y e (intensivas) a una configuración dada
void e_m(char *spines,double *e,double *m,int *xp,int *yp);

//cabecera función para calcular la media de un vector dado de X componentes
void media(double *vector,double *media,double X);

//cabecera función para calcular la media de un vector de componentes al cuadrado de X componentes
void media_cuadrado(double *vector,double *media, double X);

//función para el cálculo de errores por análisis por bloques
void analisisBloques(double *datos, int X, int factor, double media, double Deriv, double*smed,double*sDeriv, int *term);

//cabecera función que realiza todo el proceso
void Ising(int Nter, int Nmed, int Nbmed,char *spines,double *prob, int *xp,int *yp,int *xm,int *ym,double *result);

//cabecera de la función histograma
void GenerarHistograma(double *NumerosAleatorios,double *histograma, char *nombre_archivo,double Nmed);


///PROGRAMA PRINCIPAL

int main(){
srand(time(NULL)); //para los numeros aleatorios
//definimos variables
double beta_inicial,beta_final,diferencial_beta,beta,prob[5],resultados[8];
int numero_pasos,configuracion_inicial,Nter,Nmed,Nbmed,xp[L],xm[L],yp[L],ym[L],i,j;
char spines[L*L];
FILE *f;

beta_inicial=0.01;
beta_final=1.5;
diferencial_beta=0.05;
numero_pasos=(beta_final-beta_inicial)/diferencial_beta;
configuracion_inicial=3;

f=fopen("Resultados.txt","w");

//creamos xp,xm,yp,ym para movernos
funciones_xy_adelante_atras(xp,xm,yp,ym);


//generamos la configuración inicial
elegir_configuracion_inicial(configuracion_inicial,spines);

//elegimos los pasoa a realizar
Nter=300; //300 pasos de termalización
Nmed=1000; //1000 medidas
Nbmed=1; //numero de pasos de metropolis entre medidas

beta=beta_inicial;  //iniciamos con beta inicial

///Histeresis
for(i=0;i<numero_pasos;i++){
    //generamos el vector prob
    for(j=0;j<5;j++){
        prob[j]=exp(-beta*((j-2)*4));
    }
    //realizamos el proceso(un ciclo)
    Ising(Nter,Nmed,Nbmed,spines,prob,xp,yp,xm,ym,resultados);
    //escribimos los resultados después de cada ciclo
    fprintf(f,"%f %f %f %f %f %f\n",beta,resultados[1],resultados[3],2.0*L*L*resultados[5]-resultados[1]*resultados[1],L*L*(resultados[6]-resultados[7]*resultados[7]),resultados[7]);
    beta=beta+diferencial_beta;
}
fclose(f);
}


///FUNCIONES

void funciones_xy_adelante_atras(int *xp,int *xm,int *yp,int *ym){
    int i;

    //generamos xp: (paso hacia delante en x)

    for(i=0;i<(L-1);i++){
        xp[i]=1;
    }
    xp[L-1]=-(L-1);

    //generamos yp: (paso hacia arriba en y)

    for(i=0;i<(L-1);i++){
        yp[i]=L;
    }
    yp[L-1]=-L*(L-1);

    //generamos xm: (paso hacia atrás en x)

    for(i=1;i<L;i++){
        xm[i]=-1;
    }
    xm[0]=(L-1);

    //generamos ym: (paso hacia abajo en y)

    for(i=1;i<L;i++){
        ym[i]=-L;
    }
    ym[0]=L*(L-1);
}

void configuracion_uno(char *spines){
    int i;
    for(i=0;i<(L*L);i++){
            spines[i]=1;
        }
}

void configuracion_menos_uno(char *spines){
    int i;
    for(i=0;i<(L*L);i++){
        spines[i]=-1;
    }
}

void configuracion_aleatoria(char *spines){
    int i;
    for(i=0;i<(L*L);i++){
        if(RANDOM<0.5){
            spines[i]=-1;
        }
        else{
            spines[i]=1;
        }
    }
}

void elegir_configuracion_inicial(int x,char *spines){

//tomaremos 1:congelada +1
//          2:congelada -1
//          3:random

switch(x){

    case (1):
        configuracion_uno(spines);
    break;

    case(2):
        configuracion_menos_uno(spines);
    break;

    case(3):
        configuracion_aleatoria(spines);
    break;

    default:
        printf("Error al crear la confiuración inicial");
}
}

double valor_absoluto(double numero){
    if(numero<0){
        return -numero;
    }
    else{
        return numero;
    }
}


void Metropolis (char *spines,double *prob,int *xp,int *yp,int *xm,int *ym){
int n,Indice,y,x;
n=0;

for(y=0;y<L;y++){
        for(x=0;x<L;x++){
            Indice=spines[n]*(spines[n+xp[x]]+spines[n+yp[y]]+spines[n+xm[x]]+spines[n+ym[y]])/2+2;
            if(RANDOM<prob[Indice]){
                spines[n]=-spines[n];
            }
            n++;
        }
    }
}

void e_m(char *spines,double *e,double *m,int *xp,int *yp){
    int n,i,j;
    double e_suma,m_suma;
    n=0;

    //calculamos e
    e_suma=0;
    for(i=0;i<L;i++){
        for(j=0;j<L;j++){
            e_suma+=(-spines[n])*(spines[n+xp[j]]+spines[n+yp[i]]);
            n++;
        }
    }
    *e=((double)e_suma/(2*L*L));

    //calculamos m
    m_suma=0;
    for(i=0;i<(L*L);i++){
        m_suma+=spines[i];
    }
    *m=((double)m_suma/(L*L));
}

void media(double *vector,double *media,double X){
    int i;
    double media_suma;
    media_suma=0;
    for(i=0;i<X;i++){
        media_suma+=vector[i];
    }
    *media=media_suma/((double)X);
}

void media_cuadrado(double *vector,double *media, double X){
    int i;
    double media_suma;
    media_suma=0;
    for(i=0;i<X;i++){
        media_suma+=vector[i]*vector[i];
    }
    *media=media_suma/((double)(X));
}

void analisisBloques(double *datos, int X, int factor, double media, double Deriv, double*smed,double*sDeriv, int *term){ //deriv es el calor especifico o la susceptibilidad
    int i, j,termbool=1;
    int P=10, K; //P datos en bloques de X medidas
    double auxiliar,auxiliar_cuadrado;
    double varianza =0, varianza2=0,Sm [P],Sm_cuadrado[P],medsin;

    K=X/P; //k datos con los que calculando su dispersió podemos estimar el error en la cantidad dada

    for(j=0;j<P;j++){ //recorremos los bloques
        auxiliar=auxiliar_cuadrado=0;
        for(i=0;i<K;i++){
            auxiliar+=*datos;
            auxiliar_cuadrado+=(*datos)*(*datos);
            datos++;
        }
        auxiliar/=(double)(K);
        auxiliar_cuadrado/=(double)(K);
        Sm[j]=auxiliar;
        Sm_cuadrado[j]=factor*(auxiliar_cuadrado-auxiliar*auxiliar);
    }

    for(i=0;i<P;i++){
        varianza+=(Sm[i]-media)*(Sm[i]-media);
        varianza2+=(Sm[i]-Deriv)*(Sm[i]-Deriv);
    }
    varianza/=(P-1);
    varianza2/=(P-1);
    *smed=sqrt(varianza);
    *sDeriv=sqrt(varianza2);
}

void Ising(int Nter, int Nmed, int Nbmed,char *spines,double *prob,int *xp,int *yp,int *xm,int *ym,double *result){
FILE*g;
g=fopen("Termalizacion.txt","w");

int i,j, terme, termm,term;
double e,m,emed,emed2,mmed,mmed2,semed,sCv,smmed,sSus,absmmed;
double energias[Nmed], magnetiz[Nmed],magnetiz2[Nmed],histograma[40];

//termalización
for(i=0;i<Nter;i++){
    Metropolis(spines,prob,xp,yp,xm,ym);
}

//para cada valor de Beta mido Nmed veces
for(i=0;i<Nmed;i++){
    e_m(spines,&e,&m,xp,yp);
    fprintf(g,"%d %f %f\n",i,e,m);
    energias[i]=e;
    magnetiz[i]=m;
    magnetiz2[i]=valor_absoluto(m);
    for(j=0;j<Nbmed;j++){
        Metropolis(spines,prob,xp,yp,xm,ym); //para cada medida aplico antes Nbmed veces metrópolis par minimizar correlaciones
    }
}

fclose(g);

media(energias,&emed,Nmed); //calculo la media de las energias intensivas
media(magnetiz,&mmed,Nmed);
media(magnetiz2,&absmmed,Nmed);

media_cuadrado(energias,&emed2,Nmed);
media_cuadrado(magnetiz,&mmed2,Nmed);

analisisBloques(magnetiz2,Nmed,L*L,absmmed,L*L*(mmed2-mmed*mmed),&smmed,&sSus,&terme);
analisisBloques(energias,Nmed,2.0*L*L,emed,2.0*L*L*(emed2-emed*emed),&semed,&sCv,&termm);

if(terme!=0||termm!=0){ ///TERMALIZACION

if (terme>termm) term=terme;
else term=termm;

for(i=0;i<Nmed/10*term;i++){ //Divido entre 10 porque es el numero de bloques que tomo en funcion analisis por bloques
    e_m(spines,&e,&m,xp,yp);

    fprintf(g,"%d %f %f\n",i,e,m); //COMPROBACION GRAFICA TERMALIZACION  (Nter=0,Nbmed=1)

    energias[i]=e;
    magnetiz[i]=m;
    magnetiz2[i]=valor_absoluto(m);
    for(j=0;j<Nbmed;j++){
        Metropolis(spines,prob,xp,yp,xm,ym); //para cada medida aplico antes (o despues; da igual) Nbmed veces metrópolis par minimizar correlaciones
    }
}
}

GenerarHistograma(magnetiz,histograma,"histograma_magnetizacion.txt",Nmed);

media(energias,&emed,Nmed); //calculo la media de las energias intensivas
media(magnetiz,&mmed,Nmed);
media(magnetiz2,&absmmed,Nmed);

media_cuadrado(energias,&emed2,Nmed);
media_cuadrado(magnetiz,&mmed2,Nmed);

analisisBloques(magnetiz2,Nmed,L*L,absmmed,L*L*(mmed2-mmed*mmed),&smmed,&sSus,&terme);
analisisBloques(energias,Nmed,2.0*L*L,emed,2.0*L*L*(emed2-emed*emed),&semed,&sCv,&termm);


//result[0] lo defino en main. EN LOS APUNTES LO HACE DISTINTO

result[1]=emed;
result[2]=semed;
result[3]=mmed;
result[4]=smmed;
result[5]=emed2;
result[6]=mmed2;
result[7]=absmmed;

// result[5]=evar+emed; NO MUY CORRECTO
//result[6]=mvar+mmed;

}

void GenerarHistograma(double *NumerosAleatorios,double *histograma, char *nombre_archivo,double Nmed){
    int i,j,indice;
    double delta,maximo,minimo;
    FILE *f;

    f=fopen(nombre_archivo,"wt");

    //bucle para calcular el valor máximo y mínimo de los números aleatorios que he generado

    maximo=NumerosAleatorios[0];
    for(i=0;i<Nmed;i++){
        if((NumerosAleatorios[i]>maximo)){
            maximo=NumerosAleatorios[i];
        }
    }
    minimo=NumerosAleatorios[0];
    for(i=0;i<Nmed;i++){
        if((NumerosAleatorios[i]<minimo)){
            minimo=NumerosAleatorios[i];
        }
    }

    //calculo delta

    delta=(maximo-minimo)/40;     //def delta (folio aparte)

    //inicializo el array histograma a 0

    for(i=0;i<40;i++){
        histograma[i]=0;
    }

    //genero el histograma

    for(i=0;i<Nmed;i++){            //recorro el array de numeros aleatorios
       indice=(int)(NumerosAleatorios[i]-minimo)/delta;       //así se en que caja estoy
       histograma[indice]++;
    }

    //normalizamos el histograma

    for(i=0;i<40;i++){
        histograma[i]=histograma[i]/(delta*Nmed);           //formula normalizar(folio aparte)
    }

    //guardamos el histograma normalizado en un fichero

    for(i=0;i<40;i++){
        fprintf(f,"%d %lf\n",i,histograma[i]);
    }
    fclose(f);
}
