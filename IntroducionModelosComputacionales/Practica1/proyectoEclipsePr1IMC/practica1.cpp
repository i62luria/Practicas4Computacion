//============================================================================
// Introducción a los Modelos Computacionales
// Name        : practica1.cpp
// Author      : Pedro A. Gutiérrez
// Version     :
// Copyright   : Universidad de Córdoba
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <ctime>    // Para coger la hora time()
#include <cstdlib>  // Para establecer la semilla srand() y generar números aleatorios rand()
#include <string.h>
#include <math.h>
#include "imc/PerceptronMulticapa.h"


using namespace imc;
using namespace std;

int main(int argc, char **argv) {
    // Procesar los argumentos de la línea de comandos
    bool Tflag = 0,tflag = 0, wflag = 0, pflag = 0;
    char *Tvalue = NULL, *wvalue = NULL,*tvalue = NULL;
    int c;
    //Valores Iniciales
        float dEta = 0.10;
        float dMu = 0.90;
        float dValidacion = 0.0;
        float dDecremento = 1;
        int numIteraciones=1000;
        int numcapasOcultas=1;
        int numneucaOcultas=5;
    //
    opterr = 0;

    // a: opción que requiere un argumento
    // a:: el argumento requerido es opcional
    while ((c = getopt(argc, argv, "t:i::l::h::e::m::v::d::")) != -1)
    {
        // Se han añadido los parámetros necesarios para usar el modo opcional de predicción (kaggle).
        // Añadir el resto de parámetros que sean necesarios para la parte básica de las prácticas.
        switch(c){
            case 't':
            //fichero con datos de entrenamiento
                tflag = true;
                tvalue = optarg;
                break;
            case 'i':
            //indica el  numero de interaciones
                numIteraciones = atoi(optarg);
                break;
            case 'l':
            //indica el  numero de capas ocultas
                numcapasOcultas = atoi(optarg);
                if(numcapasOcultas < 1)
                {
                    cout<<" El numero de capas ocultas no puede ser inferior a 1.\n";
                    exit(-1);
                }
                break;
            case 'h':
            //indica el  numero de neuronas en cada capa oculta
                numneucaOcultas = atoi(optarg);
                if(numneucaOcultas < 1)
                {
                    cout<<" El numero de nodos en capa oculta no puede ser inferior a 1.\n";
                    exit(-1);
                }
                break;
            case 'e':
            //indica el  valor del parametro eta(n), por defecto n=0.1
                dEta = atof(optarg);
                break;
            case 'm':
            //indica el  valor del parametro mu(n), por defecto n=0.9
                dMu = atof(optarg);;
                break;
            case 'v':
            //indica el  ratio de patrones de entramiento a usar como valores de validacion, por defecto n=0.0
                dValidacion = atof(optarg);;
                break;
                
            case 'd':
            //indica el valor del factor de decremento F, por devecto n=1
                dDecremento = atof(optarg);;
                break; 
            //Paramametros opcionales del algoritmo   
            case 'T':
                Tflag = true;
                Tvalue = optarg;
                break;
            case 'w':
                wflag = true;
                wvalue = optarg;
                break;
            case 'p':
                pflag = true;
                break;
            case '?':
                if (optopt == 'T' || optopt == 'w' || optopt == 'p' || optopt == 't' || optopt == 'i')
                    fprintf (stderr, "La opción -%c requiere un argumento.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Opción desconocida `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Caracter de opción desconocido `\\x%x'.\n",
                             optopt);
                return EXIT_FAILURE;
            default:
                return EXIT_FAILURE;
        }
    }
    if (!pflag) {
        ////////////////////////////////////////
        // MODO DE ENTRENAMIENTO Y EVALUACIÓN //
        ///////////////////////////////////////

        if(tflag == false)
        {
            exit(-1);
        } 
    
        if(Tflag==false){
            Tvalue=tvalue;
        }
        // Objeto perceptrón multicapa
    	PerceptronMulticapa mlp;
        // Parámetros del mlp. Por ejemplo, mlp.dEta = valorQueSea;
        mlp.dEta = dEta;
        mlp.dMu = dMu;
        mlp.dValidacion = dValidacion;
        mlp.dDecremento = dDecremento;
        // Lectura de datos de entrenamiento y test: llamar a mlp.leerDatos(...)
        Datos *pDatosTrain;
        Datos *pDatosTest;
    
        pDatosTrain = mlp.leerDatos(tvalue);
    
        pDatosTest = mlp.leerDatos(tvalue);
   
        if(pDatosTrain == NULL || pDatosTest == NULL)
        {
            cerr << "El conjunto de datos de test no es válido. No se puede continuar." << endl;
            exit(-1);
        }
        // Inicializar vector topología
        //La topologia es el modelo de capas que la red tendra,
        //por ejemplo si tiene una sola capa oculta 
        //por defecto de 5 neuronas, tendriamos:
        //          'Entrada' - topologia[0] = 'Tantas entradas como existan'
        //          'Ocultas' - topologia[1] = '5'
        //          'Salida'  - topologia[2] = 'Tantas salidas como existan'
     
        int *topologia = new int[numcapasOcultas+2];//2 porque las capas de entrada+salida=2
        topologia[0] = pDatosTrain->nNumEntradas;
        for(int i=1; i<(numcapasOcultas+1); i++)
        	topologia[i] = numneucaOcultas;
        topologia[numneucaOcultas+1] = pDatosTrain->nNumSalidas;
      
        // Inicializar red con vector de topología
        mlp.inicializar(numcapasOcultas+2,topologia);        //------>

       

        // Semilla de los números aleatorios
        int semillas[] = {100,200,300,400,500};
        double *erroresTest = new double[5];
        double *erroresTrain = new double[5];
        double mejorErrorTest = 1.0;
        for(int i=0; i<5; i++){
        	cout << "**********" << endl;
        	cout << "SEMILLA " << semillas[i] << endl;
        	cout << "**********" << endl;
    		srand(semillas[i]);
       
    		mlp.ejecutarAlgoritmoOnline(pDatosTrain,pDatosTest,numIteraciones,&(erroresTrain[i]),&(erroresTest[i]));
    		cout << "Finalizamos => Error de test final: " << erroresTest[i] << endl;

            // (Opcional - Kaggle) Guardamos los pesos cada vez que encontremos un modelo mejor.
            if(wflag && erroresTest[i] <= mejorErrorTest)
            {
                mlp.guardarPesos(wvalue);
                mejorErrorTest = erroresTest[i];
            }
    	}

        cout << "HEMOS TERMINADO TODAS LAS SEMILLAS" << endl;

        double mediaErrorTest = 0, desviacionTipicaErrorTest = 0;
        double mediaErrorTrain = 0, desviacionTipicaErrorTrain = 0;
        
        // Calcular medias y desviaciones típicas de entrenamiento y test

        cout << "INFORME FINAL" << endl;
        cout << "*************" << endl;
        cout << "Error de entrenamiento (Media +- DT): " << mediaErrorTrain << " +- " << desviacionTipicaErrorTrain << endl;
        cout << "Error de test (Media +- DT):          " << mediaErrorTest << " +- " << desviacionTipicaErrorTest << endl;
        return EXIT_SUCCESS;
    }
    }
