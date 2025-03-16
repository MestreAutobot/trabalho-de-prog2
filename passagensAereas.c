#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição das estruturas
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
    int assentosOcupados[100]; // 100 assentos no avião
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

// Protótipos das funções
void cadastrarAeroporto();
void cadastrarRota();
void cadastrarPassageiro();
void alterarDadosPassageiro(int codigo);
void cadastrarFuncionario();
void alterarFuncionario(int matricula);
void menuOpcoes();
void menuConfiguracoes();
void menuVendas();
int gerarNumeroETicket(Cliente *cliente, Rota *rota, int dia, int mes, int ano);
void gerarETicketTXT(Rota *rota, Cliente *cliente, int dia, int mes, int ano, char *portao, float preco);
void precoPassagem(double *preco, double Milhas, double Per, double Duffs, double ret, double proc, double dist);
int ehFeriado(int dia, int mes);
int ehFinalSemana(int dia, int mes, int ano);
int diasParaViagem(int diaViagem, int mesViagem, int anoViagem);
int diasRetorno(int diaIda, int mesIda, int anoIda, int diaVolta, int mesVolta, int anoVolta);
float calcularPreco(Rota *rota, int diasParaViagem, int ehFeriado, int ehFinalSemana, int diasRetorno, int poltronasVagas);
void buscarCliente();
void funcaoDinheiro();
void listarRotas();
Rota selecionarRota();
Rota buscarRotaPorCodigo(char *codigo);
void atualizarRota(Rota *rotaAtualizada);
void inicializarRotasPreCadastradas();
void inicializarAeroportosPreCadastrados();
void selecionarOrigemDestino(char *origem, char *destino);
int listarHorariosDisponiveis(char *origem, char *destino, Rota *conexoes, int numConexoes);
void escolherAssento(Rota *rota);
int verificarConexao(char *origem, char *destino, Rota *conexoes, int maxConexoes);

// Função principal
int main() {
    inicializarAeroportosPreCadastrados();
    inicializarRotasPreCadastradas();

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

// Implementação das funções

void menuOpcoes() {
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

void menuConfiguracoes() {
    int opcao;

    do {
        printf("\nConfigurações:\n");
        printf("[1] Cadastrar Aeroportos\n");
        printf("[2] Cadastrar Rotas\n");
        printf("[3] Cadastrar Passageiros Fidelizados\n");
        printf("[4] Alterar Dados de Passageiros\n");
        printf("[5] Cadastrar Funcionários\n");
        printf("[6] Pesquisar/Alterar Funcionários\n");
        printf("[7] Voltar\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarAeroporto(); break;
            case 2: cadastrarRota(); break;
            case 3: cadastrarPassageiro(); break;
            case 4: {
                int codigo;
                printf("Digite o código do passageiro que deseja alterar: ");
                scanf("%d", &codigo);
                alterarDadosPassageiro(codigo);
                break;
            }
            case 5: cadastrarFuncionario(); break;
            case 6: {
                int matricula;
                printf("Digite a matrícula do funcionário que deseja alterar: ");
                scanf("%d", &matricula);
                alterarFuncionario(matricula);
                break;
            }
            case 7: printf("Voltando...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 7);
}

void menuVendas() {
    char origem[10], destino[10];
    Rota rotaEscolhida;
    Rota conexoes[10]; // Armazena as conexões encontradas
    int numConexoes;
    Cliente cliente;
    int dia, mes, ano;
    char portao[10];
    int opcaoPagamento;

    // Etapa 1: Selecionar origem e destino
    selecionarOrigemDestino(origem, destino);

    // Verifica se há voos diretos ou conexões
    numConexoes = verificarConexao(origem, destino, conexoes, 10);

    // Etapa 2: Selecionar horário
    int numRotasDisponiveis = listarHorariosDisponiveis(origem, destino, conexoes, numConexoes);

    if (numRotasDisponiveis == 0) {
        printf("Não há voos disponíveis para a rota selecionada.\n");
        return; // Retorna ao menu principal
    }

    rotaEscolhida = selecionarRota(conexoes, numConexoes);

    // Etapa 3: Escolher assento
    escolherAssento(&rotaEscolhida);

    // Etapa 4: Calcular preço da passagem
    printf("\nInforme a data do voo (DD MM AAAA): ");
    scanf("%d %d %d", &dia, &mes, &ano);
    while (getchar() != '\n'); // Limpa o buffer de entrada após o scanf

    int diasParaViagemValor = diasParaViagem(dia, mes, ano);
    int ehFeriadoValor = ehFeriado(dia, mes);
    int ehFinalSemanaValor = ehFinalSemana(dia, mes, ano);
    int diasRetornoValor = 0; // Supondo que não há retorno para este exemplo

    float preco = calcularPreco(&rotaEscolhida, diasParaViagemValor, ehFeriadoValor, ehFinalSemanaValor, diasRetornoValor, rotaEscolhida.poltronasDisponiveis);
    printf("\nPreço da passagem: R$ %.2f\n", preco);

    // Etapa 5: Buscar ou cadastrar o cliente
    buscarCliente(); // Busca ou cadastra o cliente

    FILE *file = fopen("clientes.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }
    fread(&cliente, sizeof(Cliente), 1, file); // Lê o cliente cadastrado
    fclose(file);

    // Etapa 6: Realizar pagamento
    printf("\nForma de pagamento:\n");
    printf("[1] Cartão de Crédito\n");
    printf("[2] Cartão de Débito\n");
    printf("[3] Dinheiro\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcaoPagamento);
    while (getchar() != '\n'); // Limpa o buffer de entrada após o scanf

    if (opcaoPagamento == 3) {
        funcaoDinheiro(); // Processa pagamento em dinheiro
    } else {
        printf("Pagamento com cartão processado com sucesso!\n");
    }

    // Etapa 7: Gerar e-ticket
    printf("Informe o portão de embarque: ");
    fgets(portao, sizeof(portao), stdin);
    portao[strcspn(portao, "\n")] = 0; // Remove o '\n' do final

    gerarETicketTXT(&rotaEscolhida, &cliente, dia, mes, ano, portao, preco); // Passa o preço e o cliente

    // Atualiza a rota no arquivo após a venda
    atualizarRota(&rotaEscolhida);

    printf("Venda concluída com sucesso! E-ticket gerado.\n");
}


void inicializarRotasPreCadastradas() {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Arquivo voos.dat não encontrado. Criando...\n");
        file = fopen("voos.dat", "wb");
        if (file == NULL) {
            printf("Erro ao criar o arquivo de voos.\n");
            return;
        }
        fclose(file);
        file = fopen("voos.dat", "rb");
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fclose(file);
        file = fopen("voos.dat", "ab");

        Rota rotas[] = {
            {"R001", "São Paulo X Rio de Janeiro", "CGH", "SDU", 8, 0, 100, 222.0, {0}},
            {"R002", "São Paulo X Belo Horizonte", "CGH", "CNF", 9, 30, 100, 305.0, {0}},
            {"R003", "São Paulo X Vitória", "CGH", "VIX", 10, 0, 100, 462.0, {0}},
            {"R004", "Rio de Janeiro X Vitória", "SDU", "VIX", 11, 0, 100, 257.0, {0}},
            {"R005", "São Paulo X Fortaleza", "CGH", "FOR", 12, 0, 100, 1474.0, {0}},
            {"R006", "São Paulo X Brasília", "CGH", "BSB", 13, 0, 100, 541.0, {0}},
            {"R007", "Porto Alegre X São Paulo", "POA", "CGH", 14, 0, 100, 531.0, {0}},
            {"R008", "Vitória X Recife", "VIX", "REC", 15, 0, 100, 924.0, {0}}
        };

        for (int i = 0; i < sizeof(rotas) / sizeof(rotas[0]); i++) {
            fwrite(&rotas[i], sizeof(Rota), 1, file);
        }

        printf("Rotas pré-cadastradas adicionadas com sucesso!\n");
    } else {
        printf("O arquivo voos.dat já contém dados. Nenhuma rota pré-cadastrada foi adicionada.\n");
    }

    fclose(file);
}

void inicializarAeroportosPreCadastrados() {
    FILE *file = fopen("aeroportos.dat", "rb");
    if (file == NULL) {
        printf("Arquivo aeroportos.dat não encontrado. Criando...\n");
        file = fopen("aeroportos.dat", "wb");
        if (file == NULL) {
            printf("Erro ao criar o arquivo de aeroportos.\n");
            return;
        }
        fclose(file);
        file = fopen("aeroportos.dat", "rb");
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fclose(file);
        file = fopen("aeroportos.dat", "ab");

        Aeroporto aeroportos[] = {
            {"CGH", "Aeroporto de Congonhas", "São Paulo", "SP"},
            {"SDU", "Aeroporto Santos Dumont", "Rio de Janeiro", "RJ"},
            {"CNF", "Aeroporto Internacional de Belo Horizonte", "Belo Horizonte", "MG"},
            {"VIX", "Aeroporto de Vitória", "Vitória", "ES"},
            {"FOR", "Aeroporto Internacional Pinto Martins", "Fortaleza", "CE"},
            {"BSB", "Aeroporto Internacional de Brasília", "Brasília", "DF"},
            {"POA", "Aeroporto Internacional Salgado Filho", "Porto Alegre", "RS"},
            {"REC", "Aeroporto Internacional do Recife", "Recife", "PE"}
        };

        for (int i = 0; i < sizeof(aeroportos) / sizeof(aeroportos[0]); i++) {
            fwrite(&aeroportos[i], sizeof(Aeroporto), 1, file);
        }

        printf("Aeroportos pré-cadastrados adicionados com sucesso!\n");
    } else {
        printf("O arquivo aeroportos.dat já contém dados. Nenhum aeroporto pré-cadastrado foi adicionado.\n");
    }

    fclose(file);
}

// Função para selecionar origem e destino
void selecionarOrigemDestino(char *origem, char *destino) {
    FILE *file = fopen("aeroportos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de aeroportos.\n");
        return;
    }

    Aeroporto aeroporto;
    int contador = 1;
    int escolhaOrigem, escolhaDestino;

    printf("\nAeroportos disponíveis:\n");
    while (fread(&aeroporto, sizeof(Aeroporto), 1, file)) {
        printf("[%d] %s (%s) - %s/%s\n", contador, aeroporto.nome, aeroporto.codigo, aeroporto.cidade, aeroporto.estado);
        contador++;
    }
    fclose(file);

    printf("\nEscolha o aeroporto de origem (1 a %d): ", contador - 1);
    scanf("%d", &escolhaOrigem);
    printf("Escolha o aeroporto de destino (1 a %d): ", contador - 1);
    scanf("%d", &escolhaDestino);

    if (escolhaOrigem < 1 || escolhaOrigem >= contador || escolhaDestino < 1 || escolhaDestino >= contador) {
        printf("Escolha inválida. Tente novamente.\n");
        return;
    }

    file = fopen("aeroportos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de aeroportos.\n");
        return;
    }

    fseek(file, (escolhaOrigem - 1) * sizeof(Aeroporto), SEEK_SET);
    fread(&aeroporto, sizeof(Aeroporto), 1, file);
    strcpy(origem, aeroporto.codigo);

    fseek(file, (escolhaDestino - 1) * sizeof(Aeroporto), SEEK_SET);
    fread(&aeroporto, sizeof(Aeroporto), 1, file);
    strcpy(destino, aeroporto.codigo);

    fclose(file);
}

int listarHorariosDisponiveis(char *origem, char *destino, Rota *conexoes, int numConexoes) {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return 0;
    }

    Rota rota;
    int contador = 0;

    printf("\nHorários disponíveis para %s -> %s:\n", origem, destino);

    // Exibe as conexões encontradas
    for (int i = 0; i < numConexoes; i += 2) {
        printf("[%d] Conexão: %s -> %s -> %s | Horário: %02d:%02d e %02d:%02d | Poltronas disponíveis: %d\n",
               contador + 1, conexoes[i].origem, conexoes[i].destino, conexoes[i + 1].destino,
               conexoes[i].horarioHoras, conexoes[i].horarioMinutos,
               conexoes[i + 1].horarioHoras, conexoes[i + 1].horarioMinutos,
               conexoes[i].poltronasDisponiveis);
        contador++;
    }

    // Exibe as rotas diretas
    while (fread(&rota, sizeof(Rota), 1, file)) {
        if (strcmp(rota.origem, origem) == 0 && strcmp(rota.destino, destino) == 0 && rota.poltronasDisponiveis > 0) {
            printf("[%d] Código: %s | Nome: %s | Horário: %02d:%02d | Poltronas disponíveis: %d\n",
                   contador + 1, rota.codigo, rota.nome, rota.horarioHoras, rota.horarioMinutos, rota.poltronasDisponiveis);
            contador++;
        }
    }

    fclose(file);

    return contador; // Retorna o número de rotas disponíveis
}

// Função para escolher assento
void escolherAssento(Rota *rota) {
    int assento;
    while (1) {
        printf("\nAssentos disponíveis:\n");
        for (int i = 0; i < 100; i++) {
            if (rota->assentosOcupados[i] == 0) {
                printf("\033[34m[%02d] Livre\t\033[0m", i + 1); // Azul para assentos livres
            } else {
                printf("\033[31m[%02d] Ocupado\t\033[0m", i + 1); // Vermelho para assentos ocupados
            }
            if ((i + 1) % 5 == 0) printf("\n"); // Organiza os assentos em linhas de 5
        }

        printf("\nEscolha um assento (1 a 100): ");
        scanf("%d", &assento);

        if (assento < 1 || assento > 100) {
            printf("Assento inválido. Tente novamente.\n");
        } else if (rota->assentosOcupados[assento - 1] == 1) {
            printf("Assento já ocupado. Tente novamente.\n");
        } else {
            rota->assentosOcupados[assento - 1] = 1; // Marca o assento como ocupado
            rota->poltronasDisponiveis--; // Reduz o número de poltronas disponíveis
            printf("Assento %d escolhido com sucesso!\n", assento);
            break;
        }
    }
}

// Função para atualizar uma rota no arquivo
void atualizarRota(Rota *rotaAtualizada) {
    FILE *file = fopen("voos.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Rota rota;
    while (fread(&rota, sizeof(Rota), 1, file)) {
        if (strcmp(rota.codigo, rotaAtualizada->codigo) == 0) {
            fseek(file, -sizeof(Rota), SEEK_CUR); // Volta para o início da rota
            fwrite(rotaAtualizada, sizeof(Rota), 1, file); // Atualiza a rota
            fclose(file);
            printf("Rota atualizada com sucesso!\n");
            return;
        }
    }

    fclose(file);
    printf("Rota com código %s não encontrada.\n", rotaAtualizada->codigo);
}

// Função para buscar uma rota por código
Rota buscarRotaPorCodigo(char *codigo) {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        Rota rotaVazia = {0};
        return rotaVazia;
    }

    Rota rota;
    while (fread(&rota, sizeof(Rota), 1, file)) {
        if (strcmp(rota.codigo, codigo) == 0) {
            fclose(file);
            return rota;
        }
    }

    fclose(file);
    printf("Rota com código %s não encontrada.\n", codigo);
    Rota rotaVazia = {0};
    return rotaVazia;
}

// Função para selecionar uma rota
Rota selecionarRota(Rota *conexoes, int numConexoes) {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        Rota rotaVazia = {0};
        return rotaVazia;
    }

    Rota rota;
    int contador = 1;
    int escolha;

    printf("\nRotas disponíveis:\n");

    // Exibe as conexões encontradas
    for (int i = 0; i < numConexoes; i += 2) {
        printf("[%d] Conexão: %s -> %s -> %s | Horário: %02d:%02d e %02d:%02d | Poltronas disponíveis: %d\n",
               contador, conexoes[i].origem, conexoes[i].destino, conexoes[i + 1].destino,
               conexoes[i].horarioHoras, conexoes[i].horarioMinutos,
               conexoes[i + 1].horarioHoras, conexoes[i + 1].horarioMinutos,
               conexoes[i].poltronasDisponiveis);
        contador++;
    }

    // Exibe as rotas diretas
    while (fread(&rota, sizeof(Rota), 1, file)) {
        printf("[%d] Código: %s | Nome: %s | Origem: %s | Destino: %s | Horário: %02d:%02d | Poltronas disponíveis: %d\n",
               contador, rota.codigo, rota.nome, rota.origem, rota.destino, rota.horarioHoras, rota.horarioMinutos, rota.poltronasDisponiveis);
        contador++;
    }
    fclose(file);

    printf("Escolha uma rota (1 a %d): ", contador - 1);
    scanf("%d", &escolha);

    // Verifica se a escolha é uma conexão
    if (escolha <= numConexoes / 2) {
        // Retorna a primeira parte da conexão
        return conexoes[(escolha - 1) * 2];
    }

    // Caso contrário, retorna uma rota direta
    file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        Rota rotaVazia = {0};
        return rotaVazia;
    }

    Rota rotaEscolhida;
    fseek(file, (escolha - 1 - numConexoes / 2) * sizeof(Rota), SEEK_SET);
    fread(&rotaEscolhida, sizeof(Rota), 1, file);
    fclose(file);

    return rotaEscolhida;
}

// Função para listar todas as rotas
void listarRotas() {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Rota rota;
    printf("\nRotas disponíveis:\n");
    while (fread(&rota, sizeof(Rota), 1, file)) {
        printf("Código: %s | Nome: %s | Origem: %s | Destino: %s | Horário: %02d:%02d | Poltronas disponíveis: %d\n", 
               rota.codigo, rota.nome, rota.origem, rota.destino, rota.horarioHoras, rota.horarioMinutos, rota.poltronasDisponiveis);
    }
    fclose(file);
}

// Função para gerar um número de e-ticket
int gerarNumeroETicket(Cliente *cliente, Rota *rota, int dia, int mes, int ano) {
    srand(cliente->codigo + rota->codigo[0] * dia * mes * ano);
    return (rand() % 900000) + 100000; // Número entre 100000 e 999999
}

// Função para gerar um e-ticket em formato TXT
void gerarETicketTXT(Rota *rota, Cliente *cliente, int dia, int mes, int ano, char *portao, float preco) {
    // Gera o número do e-ticket
    int numeroETicket = gerarNumeroETicket(cliente, rota, dia, mes, ano);

    // Cria o nome do arquivo com o nome do passageiro e o número do e-ticket
    char nomeArquivo[150];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "eticket_%s_%d.txt", cliente->nome, numeroETicket);

    // Abre o arquivo para escrita
    FILE *file = fopen(nomeArquivo, "w");
    if (file == NULL) {
        printf("Erro ao gerar o e-ticket.\n");
        return;
    }

    // Escreve as informações do e-ticket no arquivo
    fprintf(file, "----------------------------------------\n");
    fprintf(file, "E-TICKET: %d\n", numeroETicket);
    fprintf(file, "Passageiro: %s\n", cliente->nome);
    fprintf(file, "Data do voo: %02d/%02d/%04d\n", dia, mes, ano);
    fprintf(file, "Origem: %s  |  Destino: %s\n", rota->origem, rota->destino);
    fprintf(file, "Horário: %02d:%02d\n", rota->horarioHoras, rota->horarioMinutos);
    fprintf(file, "Código do voo: %s\n", rota->codigo);
    fprintf(file, "Portão de embarque: %s\n", portao);
    fprintf(file, "Preço da passagem: R$ %.2f\n", preco);
    fprintf(file, "----------------------------------------\n\n");

    // Fecha o arquivo
    fclose(file);

    printf("\nE-ticket gerado com sucesso! Arquivo: %s\n", nomeArquivo);
}

// Função para processar pagamento em dinheiro
void funcaoDinheiro() {
    FILE *file = fopen("funcionarios.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }

    int matricula;
    int encontrado = 0;
    char confirmacao;

    printf("Informe o número de matrícula do funcionário: ");
    scanf("%d", &matricula);

    Funcionario funcionario;
    while (fread(&funcionario, sizeof(Funcionario), 1, file)) {
        if (funcionario.matricula == matricula) {
            encontrado = 1;
            printf("Confirmar pagamento? (s/n): ");
            scanf(" %c", &confirmacao);

            if (confirmacao == 's' || confirmacao == 'S') {
                printf("Pagamento confirmado!\n");
            } else {
                printf("Pagamento não confirmado.\n");
            }
            break;
        }
    }

    if (!encontrado) {
        printf("Funcionário não encontrado. Cadastro necessário.\n");
    }

    fclose(file);
}

// Função para buscar ou cadastrar um cliente
void buscarCliente() {
    FILE *file = fopen("clientes.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes. Criando novo arquivo...\n");
        file = fopen("clientes.dat", "wb");
        fclose(file);
        return;
    }

    char resposta;
    char cpfDigitado[15];
    int encontrado = 0;
    Cliente cliente;

    printf("Possui status de Cliente Fiel? (s/n): ");
    scanf(" %c", &resposta); // Espaço antes do %c para ignorar espaços em branco
    while (getchar() != '\n'); // Limpa o buffer de entrada após o scanf

    if (resposta == 's' || resposta == 'S') {
        printf("Por favor, insira seu CPF: ");
        fgets(cpfDigitado, sizeof(cpfDigitado), stdin);
        cpfDigitado[strcspn(cpfDigitado, "\n")] = 0; // Remove o '\n' do final

        while (fread(&cliente, sizeof(Cliente), 1, file)) {
            if (strcmp(cpfDigitado, cliente.CPF) == 0) {
                printf("Cliente já cadastrado. Nome: %s\n", cliente.nome);
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            printf("Cliente não cadastrado. Por favor, realize o cadastro:\n");
            cadastrarPassageiro(); // Chama a função de cadastro de passageiro
        }
    } else {
        printf("Por favor, realize o cadastro:\n");
        cadastrarPassageiro(); // Chama a função de cadastro de passageiro
    }

    fclose(file);
}

// Função para alterar dados de um funcionário
void alterarFuncionario(int matricula) {
    FILE *file = fopen("funcionarios.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }

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

// Função para alterar dados de um passageiro
void alterarDadosPassageiro(int codigo) {
    FILE *file = fopen("clientes.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

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

// Função para cadastrar um funcionário
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

// Função para cadastrar um passageiro
void cadastrarPassageiro() {
    FILE *file = fopen("clientes.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    Cliente cliente;

    printf("Código do passageiro: ");
    scanf("%d", &cliente.codigo);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    printf("Nome: ");
    fgets(cliente.nome, sizeof(cliente.nome), stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = 0; // Remove o '\n' do final

    printf("RG: ");
    fgets(cliente.RG, sizeof(cliente.RG), stdin);
    cliente.RG[strcspn(cliente.RG, "\n")] = 0; // Remove o '\n' do final

    printf("CPF: ");
    fgets(cliente.CPF, sizeof(cliente.CPF), stdin);
    cliente.CPF[strcspn(cliente.CPF, "\n")] = 0; // Remove o '\n' do final

    printf("Data de nascimento (DD/MM/AAAA): ");
    fgets(cliente.dataNascimento, sizeof(cliente.dataNascimento), stdin);
    cliente.dataNascimento[strcspn(cliente.dataNascimento, "\n")] = 0; // Remove o '\n' do final

    printf("Telefone: ");
    fgets(cliente.telefone, sizeof(cliente.telefone), stdin);
    cliente.telefone[strcspn(cliente.telefone, "\n")] = 0; // Remove o '\n' do final

    printf("E-mail: ");
    fgets(cliente.email, sizeof(cliente.email), stdin);
    cliente.email[strcspn(cliente.email, "\n")] = 0; // Remove o '\n' do final

    printf("Contato de emergência (opcional, pressione Enter para ignorar): ");
    fgets(cliente.contatoEmergencia, sizeof(cliente.contatoEmergencia), stdin);
    cliente.contatoEmergencia[strcspn(cliente.contatoEmergencia, "\n")] = 0; // Remove o '\n' do final

    printf("Bagagem extra acima de 10Kg? (S/N): ");
    scanf(" %c", &cliente.bagagemExtra); // Espaço antes do %c para ignorar espaços em branco

    fwrite(&cliente, sizeof(Cliente), 1, file);
    fclose(file);

    printf("Passageiro cadastrado com sucesso!\n");
}

// Função para cadastrar uma rota
void cadastrarRota() {
    FILE *file = fopen("voos.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Rota rota;

    printf("Código da rota: ");
    scanf("%s", rota.codigo);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    printf("Nome da rota (ex: São Paulo X Rio de Janeiro): ");
    fgets(rota.nome, sizeof(rota.nome), stdin);
    rota.nome[strcspn(rota.nome, "\n")] = 0; // Remove o '\n' do final

    printf("Código do aeroporto de origem: ");
    scanf("%s", rota.origem);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    printf("Código do aeroporto de destino: ");
    scanf("%s", rota.destino);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    printf("Horário (HH MM): ");
    scanf("%d %d", &rota.horarioHoras, &rota.horarioMinutos);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    printf("Número de poltronas disponíveis: ");
    scanf("%d", &rota.poltronasDisponiveis);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    printf("Distância em milhas: ");
    scanf("%f", &rota.distanciaMilhas);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    // Inicializa todos os assentos como livres (0)
    for (int i = 0; i < 100; i++) {
        rota.assentosOcupados[i] = 0;
    }

    fwrite(&rota, sizeof(Rota), 1, file);
    fclose(file);

    printf("Rota cadastrada com sucesso!\n");
}

int verificarConexao(char *origem, char *destino, Rota *conexoes, int maxConexoes) {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return 0;
    }

    Rota rotas[100]; // Armazena todas as rotas do arquivo
    int numRotas = 0;

    // Lê todas as rotas do arquivo
    while (fread(&rotas[numRotas], sizeof(Rota), 1, file)) {
        numRotas++;
        if (numRotas >= 100) { // Limite máximo de rotas
            break;
        }
    }
    fclose(file);

    int numConexoes = 0;

    // Verifica se há um voo direto
    for (int i = 0; i < numRotas; i++) {
        if (strcmp(rotas[i].origem, origem) == 0 && strcmp(rotas[i].destino, destino) == 0) {
            printf("Voo direto encontrado de %s para %s!\n", origem, destino);
            return 0; // Retorna 0 para indicar que há um voo direto
        }
    }

    // Verifica se há uma conexão com um voo intermediário
    for (int i = 0; i < numRotas; i++) {
        if (strcmp(rotas[i].origem, origem) == 0) {
            for (int j = 0; j < numRotas; j++) {
                if (strcmp(rotas[i].destino, rotas[j].origem) == 0 && strcmp(rotas[j].destino, destino) == 0) {
                    if (numConexoes < maxConexoes) {
                        // Adiciona a conexão à lista de conexões
                        conexoes[numConexoes] = rotas[i]; // Primeiro trecho
                        numConexoes++;
                        conexoes[numConexoes] = rotas[j]; // Segundo trecho
                        numConexoes++;
                    }
                    printf("Conexão encontrada: %s -> %s -> %s!\n", origem, rotas[i].destino, destino);
                }
            }
        }
    }

    return numConexoes; // Retorna o número de conexões encontradas
}

// Função para cadastrar um aeroporto
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

// Função para calcular o preço da passagem
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

// Função para calcular dias para a viagem
int diasParaViagem(int diaViagem, int mesViagem, int anoViagem) {
    time_t agora;
    time(&agora);

    struct tm tmViagem = {0};
    tmViagem.tm_mday = diaViagem;
    tmViagem.tm_mon = mesViagem - 1;
    tmViagem.tm_year = anoViagem - 2025;

    time_t tempoViagem = mktime(&tmViagem);
    double diferenca = difftime(tempoViagem, agora);
    return (int)(diferenca / (60 * 60 * 24));
}

// Função para calcular dias de retorno
int diasRetorno(int diaIda, int mesIda, int anoIda, int diaVolta, int mesVolta, int anoVolta) {
    struct tm tmIda = {0};
    tmIda.tm_mday = diaIda;
    tmIda.tm_mon = mesIda - 1;
    tmIda.tm_year = anoIda - 2025;

    struct tm tmVolta = {0};
    tmVolta.tm_mday = diaVolta;
    tmVolta.tm_mon = mesVolta - 1;
    tmVolta.tm_year = anoVolta - 2025;

    time_t tempoIda = mktime(&tmIda);
    time_t tempoVolta = mktime(&tmVolta);

    double diferenca = difftime(tempoVolta, tempoIda);
    int dias = (int)(diferenca / (60 * 60 * 24));

    return dias > 0 ? dias : 0;
}

// Função para verificar se é feriado
int ehFeriado(int dia, int mes) {
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

    for (int i = 0; i < sizeof(feriados) / sizeof(feriados[0]); i++) {
        if (feriados[i][0] == dia && feriados[i][1] == mes) {
            return 1;
        }
    }
    return 0;
}

// Função para verificar se é final de semana
int ehFinalSemana(int dia, int mes, int ano) {
    struct tm tm = {0};
    tm.tm_mday = dia;
    tm.tm_mon = mes - 1;
    tm.tm_year = ano - 2025;

    mktime(&tm);
    return (tm.tm_wday == 0 || tm.tm_wday == 6);
}

// Função para calcular o preço da passagem
void precoPassagem(double *preco, double Milhas, double Per, double Duffs, double ret, double proc, double dist) {
    double aux;
    aux = Milhas * Per * Duffs * ret * proc * dist;
    *preco = aux;
}
