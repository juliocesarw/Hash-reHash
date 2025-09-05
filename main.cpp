#include <iostream>
#include <cstdio> 
using namespace std;
#include <cstdlib>
#include <ctype.h>
#include <cstring>

struct Aluno{
    char matricula[9];
    char cpf[15];
    char nome[70];
    double nota;
    int idade;
    char curso[40];
    char cidade[40];
    Aluno *prox;
    Aluno *ante;
};

#define CARGA_MAXIMA 0.6
#define TAMANHO_HASH_INICIAL 1021

struct Alunos{
    Aluno **hash;
    bool *hashOcupada;
    int tamanhoAtual;
    int quantidade;
};

Alunos a;

void inicializa(){
    a.tamanhoAtual = TAMANHO_HASH_INICIAL;
    a.quantidade = 0;
    a.hash = new Aluno*[TAMANHO_HASH_INICIAL]();
    a.hashOcupada = new bool[TAMANHO_HASH_INICIAL]();
}

//declaracao de funcoes
Aluno *lerAluno();
FILE * abrir_arquivo( const char * arquivo, const char * modo);
void processoLeituraInsercao();
int proximoPrimoDobro();
int ehPrimo(int x);
int proximoPrimoDobro();
void imprimirHash();
int somaDosCaracteres(char * nome);
int h(int k, int tamanho);
int h2(int k, int tamanho);
bool inserir(Aluno ** var, int indice, Aluno * ponteiro);
bool processoInsercao(Aluno * ponteiro, int indice);
bool reHash();
bool processoDeReHash(Aluno * ponteiro, Aluno ** hash, int tamanho);


//============================================================================================

// funcoes

Aluno *lerAluno(){
    
    Aluno *b = new Aluno;
    b->ante = NULL;
    b->prox = NULL;
    return b;
}

void imprimirHash(){
    for (int i = 0; i < a.tamanhoAtual; i++)
    {
        if(a.hash[i] != NULL){
            cout << i << " - " << a.hash[i]->nome << endl;
        }
    }
}

FILE * abrir_arquivo(const char * arquivo, const char * modo){

	FILE * arq;
	arq = fopen(arquivo, modo);
    if (arq == NULL){
        printf("Erro ao abrir o arquivo\n");
        //system("pause");
    }
	return arq;
}

void processoLeituraInsercao(){
    FILE * arq;
    arq = abrir_arquivo("../textos/teste.csv", "r");

    while(!feof(arq)){
        Aluno *c = lerAluno();
        fscanf(arq, "%[^,], %[^,], %[^,] , %lf, %d, %[^,], %[^,\n] ", c->matricula, c->cpf, c->nome, &c->nota, &c->idade, c->curso, c->cidade);
        int indice = somaDosCaracteres(c->nome);
        processoInsercao(c, indice);
    }
    fclose(arq);
}

int ehPrimo(int x) {
    if (x < 2) return 0;
    if (x == 2) return 1;
    if (x % 2 == 0) return 0;
    for (int i = 3; i*i <= x; i += 2) {
        if (x % i == 0) return 0;
    }
    return 1;
}

int proximoPrimoDobro() {
    int valor = 2 * a.tamanhoAtual;
    while (true) {
        valor++;
        if (ehPrimo(valor)) {
            return valor;
        }
    }
}

int somaDosCaracteres(char * nome){
    
    int indice = 0;
    
    for (int i = 0; nome[i] != '\0'; i++) {
        indice += (int)nome[i];  
    }  
    return indice * indice * indice;
}   

int h(int k, int tamanho){
    return (k * k * k) % tamanho;
}

int h2(int k, int tamanho){
    return 1 + ((k * k * k) % (tamanho - 1));
}

bool inserir(Aluno ** var, int indice, Aluno * ponteiro){
    if(var[indice] != NULL){
        var[indice] = ponteiro;
        a.hashOcupada[indice] = true;
        return true;
    }
    else{
        return false;
    }
}

bool processoInsercao(Aluno * ponteiro, int indice){
    int i = h(indice, a.tamanhoAtual);
    if(inserir(a.hash, i, ponteiro) == true){
        a.quantidade++;
        return true;
    }
    else{
        i = h2(indice, a.tamanhoAtual);
        for ( int j = 0; j < 20; j++)
        {
            int novoIndice = (h(indice, a.tamanhoAtual) + j * i) % a.tamanhoAtual;
            if(inserir(a.hash, novoIndice, ponteiro) == true){
                a.quantidade++;
                return true;
            }
            if(i == 20){
                reHash();
                processoDeReHash(ponteiro, a.hash, a.tamanhoAtual);
                a.quantidade++;
            }
        }
        
    }
    return true;
}

bool reHash(){
    int novoTamanho = proximoPrimoDobro();
    Aluno **var = new Aluno*[novoTamanho]();
    a.hashOcupada = new bool[novoTamanho]();

    for ( int i = 0; i < a.tamanhoAtual; i++)
    {
        if(a.hash[i] != NULL){
            processoDeReHash(a.hash[i], var, novoTamanho);
        }
    }

    a.hash = var;
    a.quantidade = novoTamanho;
    return true;
}

bool processoDeReHash(Aluno * ponteiro, Aluno ** hash, int tamanho){
    int indice = somaDosCaracteres(ponteiro->nome);
    int i = h(indice, tamanho);
    if(inserir(hash, i, ponteiro) == true){
        return true;
    }
    else{
        i = h2(indice, tamanho);
        for ( int j = 0; j < 20; j++)
        {
            int novoIndice = (h(indice, a.tamanhoAtual) + j * i) % tamanho;
            if(inserir(hash, novoIndice, ponteiro) == true){
                a.quantidade++;
                return true;
            }  
        }
        
    }
    return true;
}

int main() {

    inicializa();
    processoLeituraInsercao();
    imprimirHash();
    return 0;
}

