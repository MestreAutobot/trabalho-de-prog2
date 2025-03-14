#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    int assentosOcupados[100]; // ADIÇÃO
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

void menuOpcoes(){
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

int feriados[][2] = {
    {1, 1},   // Ano Novo (01/01)
    {21, 4},  // Tiradentes (21/04)
    {1, 5},   // Dia do Trabalho (01/05)
    {7, 9},   // Independência do Brasil (07/09)
    {12, 10}, // Nossa Senhora Aparecida (12/10)
    {2, 11},  // Finados (02/11)
    {15, 11}, // Proclamação da República (15/11)
    {25, 12}  // Natal (25/12)
};

int ehFeriado(int dia, int mes) {
    //int totalFeriados = sizeof(feriados) / sizeof(feriados[0]);
    for (int i = 0; i < (sizeof(feriados) / sizeof(feriados[0])); i++) {//
        if (feriados[i][0] == dia && feriados[i][1] == mes) {
            return 1;
        }
    }
    return 0;
}

int ehFinalSemana(int dia, int mes, int ano) {
    struct tm tm = {0};
    tm.tm_mday = dia; 
    tm.tm_mon = mes - 1;
    tm.tm_year = ano - 1900; 

    mktime(&tm);
    return (tm.tm_wday == 0 || tm.tm_wday == 6); // ADIÇÃO

    //if (tm.tm_wday == 0 || tm.tm_wday == 6) {
        //return 1;
    //}
    //return 0;
}

int diasParaViagem(int diaViagem, int mesViagem, int anoViagem) {
    time_t agora;
    time(&agora);
        
    struct tm tmViagem = {0};
    tmViagem.tm_mday = diaViagem;
    tmViagem.tm_mon = mesViagem - 1;
    tmViagem.tm_year = anoViagem - 1900;

    time_t tempoViagem = mktime(&tmViagem);
    double diferenca = difftime(tempoViagem, agora);
    return (int)(diferenca / (60 * 60 * 24));//

    //return dias > 0 ? dias : 0;
}

int diasRetorno(int diaIda, int mesIda, int anoIda, int diaVolta, int mesVolta, int anoVolta) {
    struct tm tmIda = {0};
    tmIda.tm_mday = diaIda;
    tmIda.tm_mon = mesIda - 1;
    tmIda.tm_year = anoIda - 1900;

    struct tm tmVolta = {0};
    tmVolta.tm_mday = diaVolta;
    tmVolta.tm_mon = mesVolta - 1;
    tmVolta.tm_year = anoVolta - 1900;

    time_t tempoIda = mktime(&tmIda);
    time_t tempoVolta = mktime(&tmVolta);

    double diferenca = difftime(tempoVolta, tempoIda);
    return (int)(diferenca / (60 * 60 * 24));

    //return dias > 0 ? dias : 0;
}

float calcularPreco(Rota *rota, int diasParaViagem, int ehFeriado, int ehFinalSemana, int diasRetorno, int poltronasVagas) {
    float precoMilha;
    
    if (rota->distanciaMilhas <= 500) precoMilha = 0.36;
    else if (rota->distanciaMilhas <= 800) precoMilha = 0.29;
    else precoMilha = 0.21;

    float fatorPeriodo;
    if (diasParaViagem <= 3) fatorPeriodo = 4.52;
    else if (diasParaViagem <= 6) fatorPeriodo = 3.21;
    else if (diasParaViagem <= 10) fatorPeriodo = 2.25;
    else if (diasParaViagem <= 15) fatorPeriodo = 1.98;
    else if (diasParaViagem <= 20) fatorPeriodo = 1.78;
    else if (diasParaViagem <= 30) fatorPeriodo = 1.65;
    else fatorPeriodo = 1.45;

    float fatorDuffs = ehFeriado ? 3.56 : (ehFinalSemana ? 1.21 : 1.00);
    
    float fatorRetorno;
    if (diasRetorno <= 2) fatorRetorno = 1.09;
    else if (diasRetorno <= 5) fatorRetorno = 1.05;
    else if (diasRetorno <= 8) fatorRetorno = 1.02;
    else fatorRetorno = 1.00;

    float fatorProcura;
    float ocupacao = 100 - ((float)poltronasVagas / rota->poltronasDisponiveis) * 100;
    if (ocupacao > 90) fatorProcura = 0.75;
    else if (ocupacao > 70) fatorProcura = 0.85;
    else if (ocupacao > 60) fatorProcura = 0.95;
    else if (ocupacao > 40) fatorProcura = 1.00;
    else if (ocupacao > 20) fatorProcura = 1.15;
    else if (ocupacao > 10) fatorProcura = 1.20;
    else fatorProcura = 1.35;

    return rota->distanciaMilhas * precoMilha * fatorPeriodo * fatorDuffs * fatorRetorno * fatorProcura;
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
    scanf(" %c", &resposta);
    
    switch(resposta){
        case 's':
            printf("Por favor, insira seu CPF:\n");
            scanf("%s", cpf);
        
            while (fread(&cliente, sizeof(Cliente), 1, file){
                if(strcmp(cpf, cliente.cpf)==0){
                    printf("Cliente já cadastrado. Cliente: %s\n", cliente.nome);
                    encontrado = 1;
                    break;
                }       
            }
            if(encontrado==0){
                printf("Cliente não cadastrado. Por favor, realize o cadastro:\n");
                cadastrarPassageiro();
                break;
            }
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

void funcaoDinheiro(){
    FILE *file = fopen("funcionarios.dat", "rb+");
    if (file == NULL){
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }
    int matricula;
    int encontrado = 0;
    char confirmacao;

    do{
        printf("Informe o número de matrícula do funcionário: \n");
        scanf("%d", &matricula);
        
        Funcionario funcionario;
        rewind(file);
        
        while(fread(&funcionario, sizeof(Funcionario), 1, file)==1){
            if(funcionario.matricula==matricula){
                encontrado = 1;
                printf("Confirmar pagamento? (s/n)");
                scanf(" %c", &confirmacao);
                
                if (confirmacao == 's' || confirmacao == 'S'){
                    printf("Pagamento confirmado!\n");
                }
                else{
                    printf("Pagamento não confirmado.\n");
                }
                break;
            }
        }
        if(encontrado==0){
            printf("Funcionário não cadastrado. Por favor, realize o cadastro:\n");
            cadastrarPassageiro();
            break;
        }
    }while(encontrado==0);
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
                printf("Preço: %.2f\n", calcularPreco(Rota *rota, int diasParaViagem, int ehFeriado, int ehFinalSemana, int diasRetorno, int poltronasVagas));
                break;
            case 2:
                printf("Você escolheu pagar com Cartão de Débito.\n");
                buscarCliente();
                break;
            case 3:
                printf("Você escolheu pagar em Dinheiro.\n");
                buscarCliente();
                funcaoDinheiro();
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

    int numeroETicket = rand() % 100000 + 10000;

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
        menuOpcoes();
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
