#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void menuVendas(){
    printf("[O]---------------------------------[O]\n");
    printf(" |  Bem vindo ao site linhas aereas  |\n");
    printf("[O]---------------------------------[O]\n");
    printf(" |                                   |\n");
    printf(" | [1] Configuracoes                 |\n");
    printf(" | [2] Vendas                        |\n");
    printf(" | [3] Sair                          |\n");
    printf(" |                                   |\n");
    printf("[O]---------------------------------[O]\n");
    printf(" | Digite uma opcao acima: ");
}

void precoPassagem(double*preco,double Milhas, double Per, double Duffs, double ret, double proc, double dist){
    double aux;
    aux = Milhas*Per*Duffs*ret*proc*dist;
    *preco= aux; 

}

int main (){
    return 0;
}