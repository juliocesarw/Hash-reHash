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
int retornaIndiceHash(char * nome);
bool insere(int indice, Aluno * ponteiroParaAluno);
int ehPrimo(int x);
int proximoPrimoDobro();
void imprimirHash();
int somaDosCaracteres(char * nome);
bool apenasInsere(int indice, Aluno * ponteiroParaAluno);
bool dinamizacaoVetor(Aluno * ponteiro);
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
        int indice = retornaIndiceHash(c->nome);
        insere(indice, c);
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

int retornaIndiceHash(char * nome){
    int indice = somaDosCaracteres(nome);
    return indice % a.tamanhoAtual;
}    

bool apenasInsere(int indice, Aluno * ponteiroParaAluno){

    if(a.hash[indice] == NULL){
        a.hash[indice] = ponteiroParaAluno;
        if (a.hashOcupada[indice] == false) a.hashOcupada[indice] = true; 
        a.quantidade++;
        return true;
    }    
    return false; 
}

bool dinamizacaoVetor(Aluno * ponteiro){

}

bool insere(int indice, Aluno * ponteiroParaAluno){

    if(apenasInsere(indice, ponteiroParaAluno) == true){ 
        return true;
    }    
    else{
        int somaDoNome = somaDosCaracteres(ponteiroParaAluno->nome);
        int h2 = 1 + (somaDoNome % (a.tamanhoAtual - 1));

        for (int i = 2; i < 20; i++)
        {
            int indiceNovo = (indice + (h2 * i)) % 1021;
            if(apenasInsere(indiceNovo, ponteiroParaAluno) == true){
                    return true;
            }
        }
    }    
    return false; 
}    

int main() {

    inicializa();
    processoLeituraInsercao();
    imprimirHash();
    // char nom'e[70] = "Lisa Alzira Jacobs";
    // int e = 'somaDosCaracteres(nome);
    // int e1 =' retornaIndiceHash(nome);
    // cout << 'e << endl;
    // cout << e1 << endl;
    return 0;
}

