#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aeroporto {
    char codigo[10];
    char nome[50];
    char cidade[50];
    char estado[3];
} Aeroporto;

typedef struct rota {
    char codigo[10];
    char nome[50]; 
    char origem[10];
    char destino[10];
    int horarioHoras;
    int horarioMinutos;
    int poltronasDisponiveis;
    float distanciaMilhas;
} Rota;

typedef struct cliente {
    int codigo;
    char nome[100];
    char RG[15];
    char CPF[15];
    char dataNascimento[11];
    char telefone[20];
    char email[100];
    char contatoEmergencia[100]; 
    char bagagemExtra;
} Cliente;

typedef struct funcionario {
    int matricula;
    char nome[100];
    char cargo[50];
} Funcionario;


void cadastrarAeroporto();
void cadastrarRota();
void cadastrarPassageiro();
void alterarDadosPassageiro();
void cadastrarFuncionario();
void alterarFuncionario();
void menuOpcoes();
void menuConfiguracoes();
void menuVendas();

void menuOpcoes(){ //mudei o nome de "menuVendas" para "menuOpcoes", assim fica "menuOpcoes > menuConfiguracoes e menuVendas", espero que esteja tudo bem
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

void menuConfiguracoes() {
    int opcao;
    do {
        printf("\nConfigurações:\n");
        printf("[1] Cadastrar Aeroportos\n");
        printf("[2] Cadastrar Rotas\n");
        printf("[3] Cadastrar passageiros fidelizados\n");
        printf("[4] Alterar dados de passageiros\n");
        printf("[5] Cadastrar Funcionários\n");
        printf("[6] Pesquisar/Alterar Funcionários\n");
        printf("[7] Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarAeroporto(); break;
            case 2: cadastrarRota(); break;
            case 3: cadastrarPassageiro(); break;
            case 4: alterarDadosPassageiro(); break;
            case 5: cadastrarFuncionario(); break;
            case 6: alterarFuncionario(); break;
            case 7: printf("Voltando...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 7);
}

void cadastrarAeroporto() {
    FILE *file = fopen("aeroportos.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de aeroportos.\n");
        return;
    }

    Aeroporto aeroporto;
    printf("Código do aeroporto: ");
    scanf("%s", aeroporto.codigo);
    printf("Nome do aeroporto: ");
    scanf(" %[^\n]", aeroporto.nome);
    printf("Cidade: ");
    scanf(" %[^\n]", aeroporto.cidade);
    printf("Estado (UF): ");
    scanf("%s", aeroporto.estado);

    fwrite(&aeroporto, sizeof(Aeroporto), 1, file);
    fclose(file);

    printf("Aeroporto cadastrado com sucesso!\n");
}

void cadastrarRota() {
    FILE *file = fopen("voos.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Rota rota;
    printf("Código da rota: ");
    scanf("%s", rota.codigo);
    printf("Nome da rota (ex: São Paulo X Rio de Janeiro): ");
    scanf(" %[^\n]", rota.nome);
    printf("Código do aeroporto de origem: ");
    scanf("%s", rota.origem);
    printf("Código do aeroporto de destino: ");
    scanf("%s", rota.destino);
    printf("Horário (HH MM): ");
    scanf("%d %d", &rota.horarioHoras, &rota.horarioMinutos);
    printf("Número de poltronas disponíveis: ");
    scanf("%d", &rota.poltronasDisponiveis);
    printf("Distância em milhas: ");
    scanf("%f", &rota.distanciaMilhas);

    fwrite(&rota, sizeof(Rota), 1, file);
    fclose(file);

    printf("Rota cadastrada com sucesso!\n");
}

void cadastrarPassageiro() {
    FILE *file = fopen("clientes.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    Cliente cliente;
    printf("Código do passageiro: ");
    scanf("%d", &cliente.codigo);
    printf("Nome: ");
    scanf(" %[^\n]", cliente.nome);
    printf("RG: ");
    scanf("%s", cliente.RG);
    printf("CPF: ");
    scanf("%s", cliente.CPF);
    printf("Data de nascimento (DD/MM/AAAA): ");
    scanf("%s", cliente.dataNascimento);
    printf("Telefone: ");
    scanf("%s", cliente.telefone);
    printf("E-mail: ");
    scanf("%s", cliente.email);
    printf("Contato de emergência (opcional, pressione Enter para ignorar): ");
    getchar();
    fgets(cliente.contatoEmergencia, sizeof(cliente.contatoEmergencia), stdin);
    printf("Bagagem extra acima de 10Kg? (S/N): ");
    scanf(" %c", &cliente.bagagemExtra);

    fwrite(&cliente, sizeof(Cliente), 1, file);
    fclose(file);

    printf("Passageiro cadastrado com sucesso!\n");
}

void cadastrarFuncionario() {
    FILE *file = fopen("funcionarios.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }

    Funcionario funcionario;
    printf("Matrícula: ");
    scanf("%d", &funcionario.matricula);
    printf("Nome: ");
    scanf(" %[^\n]", funcionario.nome);
    printf("Cargo: ");
    scanf(" %[^\n]", funcionario.cargo);

    fwrite(&funcionario, sizeof(Funcionario), 1, file);
    fclose(file);

    printf("Funcionário cadastrado com sucesso!\n");
}

void alterarDadosPassageiro() {
    FILE *file = fopen("clientes.dat", "rb+"), *temp;
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }
    
    int codigo;
    printf("Digite o código do passageiro que deseja alterar: ");
    scanf("%d", &codigo);
    
    Cliente cliente;
    int encontrado = 0;
    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        if (cliente.codigo == codigo) {
            encontrado = 1;
            printf("Novo nome: ");
            scanf(" %[^\n]", cliente.nome);
            printf("Novo RG: ");
            scanf("%s", cliente.RG);
            printf("Novo CPF: ");
            scanf("%s", cliente.CPF);
            printf("Nova data de nascimento (DD/MM/AAAA): ");
            scanf("%s", cliente.dataNascimento);
            printf("Novo telefone: ");
            scanf("%s", cliente.telefone);
            printf("Novo e-mail: ");
            scanf("%s", cliente.email);
            printf("Novo contato de emergência: ");
            getchar();
            fgets(cliente.contatoEmergencia, sizeof(cliente.contatoEmergencia), stdin);
            printf("Bagagem extra acima de 10Kg? (S/N): ");
            scanf(" %c", &cliente.bagagemExtra);
            
            fseek(file, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, file);
            break;
        }
    }
    fclose(file);
    
    if (encontrado)
        printf("Dados do passageiro alterados com sucesso!\n");
    else
        printf("Passageiro não encontrado!\n");
}

void alterarFuncionario() {
    FILE *file = fopen("funcionarios.dat", "rb+"), *temp;
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }
    
    int matricula;
    printf("Digite a matrícula do funcionário que deseja alterar: ");
    scanf("%d", &matricula);
    
    Funcionario funcionario;
    int encontrado = 0;
    while (fread(&funcionario, sizeof(Funcionario), 1, file)) {
        if (funcionario.matricula == matricula) {
            encontrado = 1;
            printf("Novo nome: ");
            scanf(" %[^\n]", funcionario.nome);
            printf("Novo cargo: ");
            scanf(" %[^\n]", funcionario.cargo);
            
            fseek(file, -sizeof(Funcionario), SEEK_CUR);
            fwrite(&funcionario, sizeof(Funcionario), 1, file);
            break;
        }
    }
    fclose(file);
    
    if (encontrado)
        printf("Dados do funcionário alterados com sucesso!\n");
    else
        printf("Funcionário não encontrado!\n");
}

void buscarCliente(){
    FILE *file = fopen("clientes.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }
    char resposta = 'n';
    char cpf[15];
    int encontrado = 0;
    Cliente cliente;
    
    printf("Cadastro:\n");
    printf("Possui status de Cliente Fiel? (s/n)\n");
    scanf("%s", resposta);
    
    switch(resposta){
        case 's':
            printf("Por favor, insira seu CPF:\n");
            scanf("%s", cpf);
        
            while (fread(&cliente, sizeof(Cliente), 1, file){
                if(strcmp(cpf, cliente.cpf)==0){
                    printf("Cliente já cadastrado. Cliente: %s\n", cliente.nome");
                    encontrado = 1;
                    break;
                }       
            }
            if(encontrado==0):
                printf("Cliente não cadastrado. Por favor, realize o cadastro:\n");
                cadastrarPassageiro();
                break;
        case 'n':
            printf("Por favor, realize o cadastro:\n");
            cadastrarPassageiro();
            break;
        
        default:
            printf("Resposta Inválida\n");
            break;
    }
    fclose(file);
}

void menuVendas(){
    int opcao;
    
    do{
        printf("\nForma de pagamento\n");
        printf("[1] Cartão de Crédito\n");
        printf("[2] Cartão de Débito\n");
        printf("[3] Dinheiro\n");
        printf("[4] Voltar\n");
        printf("Escolha uma opção: \n");
        scanf("%d", &opcao);
    
        switch(opcao){
            case 1:
                printf("Você escolheu pagar com Cartão de Crédito.\n");
                buscarCliente();
                printf(calcularPreco());
                break;
            case 2:
                printf("Você escolheu pagar com Cartão de Débito.\n");
                buscarCliente();
                break;
            case 3:
                printf("Você escolheu pagar em Dinheiro.\n");
                buscarCliente();
                break;
            case 4:
                printf("Voltando.\n");
                break;
            default: printf("Opção inválida!\n");
        }
    }while(opcao!=4);
}

void gerarETicketTXT(Rota *rota, Cliente *cliente, int dia, int mes, int ano, char *portao) {
    FILE *file = fopen("etickets.txt", "a");
    if (file == NULL) {
        printf("Erro ao gerar o e-ticket.\n");
        return;
    }

    int numeroETicket = rand() % 100000 + 10000; // Número aleatório de 5 dígitos

    fprintf(file, "----------------------------------------\n");
    fprintf(file, "E-TICKET: %d\n", numeroETicket);
    fprintf(file, "Passageiro: %s\n", cliente->nome);
    fprintf(file, "Data do voo: %02d/%02d/%04d\n", dia, mes, ano);
    fprintf(file, "Origem: %s  |  Destino: %s\n", rota->origem, rota->destino);
    fprintf(file, "Horário: %02d:%02d\n", rota->horarioHoras, rota->horarioMinutos);
    fprintf(file, "Código do voo: %s\n", rota->codigo);
    fprintf(file, "Portão de embarque: %s\n", portao);
    fprintf(file, "----------------------------------------\n\n");

    fclose(file);
    printf("\nE-ticket gerado com sucesso! (TXT)\n");
}


    


int main() {
    int opcao;
    do {
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: menuConfiguracoes(); break;
            case 2: menuVendas(); break;
            case 3: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 3);
    printf("Processo finalizado. Obrigado por comprar conosco!\n");
    return 0;
}
