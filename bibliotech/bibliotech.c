#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_USUARIOS 100
#define MAX_LIVROS 100
#define MAX_EMPRESTIMOS 100
#define MAX_EMPRESTIMOS_POR_USUARIO 3
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_LIVROS "livros.txt"
#define ARQUIVO_EMPRESTIMOS "emprestimos.txt"

// Credenciais do administrador
#define ADMIN_USUARIO "admin"
#define ADMIN_SENHA "123123"

// Estrutura para armazenar dados do aluno
typedef struct
{
    char nome[100];
    char ra[100];
    char senha[100];
    char email[100];
} Usuario;

// Estrutura para armazenar dados dos livros
typedef struct
{
    char titulo[100];
    char autor[100];
    char editora[100];
    char isbn[20];
    int quantidade;
    int disponivel;
} Livro;

// Estrutura para armazenar dados dos empréstimos
typedef struct
{
    char ra[100];
    char titulo_livro[100];
    char isbn[20];
    char data_emprestimo[11];
} Emprestimo;

Usuario usuarios[MAX_USUARIOS];
Livro livros[MAX_LIVROS];
Emprestimo emprestimos[MAX_EMPRESTIMOS];

int quantidade_usuarios = 0;
int quantidade_livros = 0;
int quantidade_emprestimos = 0;

// Declarações de funções
void registrar_aluno(Usuario usuarios[], int *quantidade_usuarios);
void redefinir_senha(Usuario usuarios[], int quantidade_usuarios);
void salvar_usuarios(Usuario usuarios[], int quantidade_usuarios);
int carregar_usuarios(Usuario usuarios[]);

// Função para ocultar a senha enquanto é digitada
void ler_senha_oculta(char *senha, int tamanho_maximo)
{
    int i = 0;               // Inicializa o índice do caractere na senha
    char caractere;           // Variável para armazenar o caractere digitado

    printf("Digite sua senha: "); // Solicita ao usuário que digite a senha

    while (1)
    {
        caractere = getch();  // Lê o caractere sem exibi-lo no console

        // Quebra o loop ao pressionar Enter
        if (caractere == '\r') // '\r' representa a tecla Enter
        {
            senha[i] = '\0';   // Adiciona o caractere nulo para terminar a string
            break;             // Encerra o loop
        }
        // Permite deletar um caractere ao pressionar Backspace
        else if (caractere == '\b' && i > 0) // '\b' representa a tecla Backspace
        {
            i--;               // Decrementa o índice para remover o último caractere
            printf("\b \b");   // Apaga o último caractere exibido no console
        }
        // Armazena o caractere e exibe um asterisco para ocultar a senha
        else if (i < tamanho_maximo - 1 && caractere != '\b') // Limita o tamanho da senha
        {
            senha[i++] = caractere; // Armazena o caractere na senha
            printf("*");            // Exibe um asterisco no console
        }
    }
    printf("\n"); // Move para a linha seguinte após a senha ser digitada
}

// Função para salvar os dados dos livros em um arquivo "livros.txt"
void salvar_livros(Livro livros[], int quantidade_livros)
{
    // Abre o arquivo "livros.txt" em modo de escrita, sobrescrevendo o conteúdo anterior
    FILE *arquivo = fopen(ARQUIVO_LIVROS, "w");
    if (arquivo == NULL)// Verifica se o arquivo foi aberto com sucesso
    {
        printf("Erro ao abrir o arquivo para salvar os livros.\n"); // Exibe uma mensagem de erro
        return; // Encerra a função se não foi possível abrir o arquivo
    }
    // Percorre o array de livros e grava cada um no arquivo em formato CSV
    for (int i = 0; i < quantidade_livros; i++)
    {
        // Escreve os dados do livro no arquivo, separados por ponto e vírgula
        fprintf(arquivo, "%s;%s;%s;%s;%d\n", livros[i].titulo, livros[i].autor, livros[i].editora, livros[i].isbn, livros[i].quantidade);
    }
    // Fecha o arquivo após salvar todos os livros
    fclose(arquivo);
}

// Função para carregar os dados dos livros salvos no arquivo "livros.txt"
int carregar_livros(Livro livros[])
{
    // Abre o arquivo "livros.txt" em modo de leitura
    FILE *arquivo = fopen(ARQUIVO_LIVROS, "r");
    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo == NULL)
    {
        printf("Arquivo de livros nao encontrado. Nenhum livro carregado.\n"); // Mensagem de erro caso o arquivo não seja encontrado
        return 0; // Retorna 0 para indicar que nenhum livro foi carregado
    }
    int quantidade = 0; // Variável para armazenar a quantidade de livros carregados
    // Lê cada linha do arquivo e armazena os dados no array de livros
    while (fscanf(arquivo, "%99[^;];%99[^;];%99[^;];%19[^;];%d\n",
                  livros[quantidade].titulo,
                  livros[quantidade].autor,
                  livros[quantidade].editora,
                  livros[quantidade].isbn,
                  &livros[quantidade].quantidade) == 5)
    {
        // Define o status de disponibilidade do livro com base na quantidade em estoque
        livros[quantidade].disponivel = livros[quantidade].quantidade > 0 ? 1 : 0;
        quantidade++; // Incrementa a contagem de livros
        // Verifica se atingiu o limite máximo de livros permitidos
        if (quantidade >= MAX_LIVROS)
        {
            break; // Interrompe a leitura para não exceder o número máximo de livros
        }
    }
    // Fecha o arquivo após o término da leitura
    fclose(arquivo);
    printf("Livros carregados com sucesso! Total: %d\n", quantidade); // Informa o total de livros carregados
    return quantidade; // Retorna a quantidade total de livros carregados
}

// Função para salvar os dados dos empréstimos de livros no arquivo "emprestimos.txt"
void salvar_emprestimos_em_arquivo()
{
    // Abre o arquivo "emprestimos.txt" em modo de escrita
    FILE *arquivo = fopen("emprestimos.txt", "w");
    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar os empréstimos.\n"); // Exibe mensagem de erro caso o arquivo não possa ser aberto
        return; // Sai da função caso haja erro ao abrir o arquivo
    }
    // Itera sobre todos os empréstimos registrados para salvá-los no arquivo
    for (int i = 0; i < quantidade_emprestimos; i++)
    {
        // Grava as informações do empréstimo atual no arquivo
        fprintf(arquivo, "RA: %s\n", emprestimos[i].ra); // RA do aluno que fez o empréstimo
        fprintf(arquivo, "Titulo: %s\n", emprestimos[i].titulo_livro); // Título do livro emprestado
        fprintf(arquivo, "ISBN: %s\n", emprestimos[i].isbn); // ISBN do livro emprestado
        fprintf(arquivo, "Data de Emprestimo: %s\n", emprestimos[i].data_emprestimo); // Data em que o empréstimo foi realizado
        fprintf(arquivo, "------------------------------\n"); // Linha divisória para separar cada empréstimo
    }
    // Fecha o arquivo após a gravação
    fclose(arquivo);
}

// Função para carregar os dados dos empréstimos armazenados no arquivo "emprestimos.txt"
void carregar_emprestimos_do_arquivo()
{
    // Abre o arquivo "emprestimos.txt" em modo de leitura
    FILE *arquivo = fopen("emprestimos.txt", "r");
    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo == NULL)
    {
        printf("Nenhum arquivo de empréstimos encontrado para carregar.\n"); // Exibe uma mensagem caso o arquivo não exista
        return; // Sai da função caso não haja arquivo para carregar
    }
    // Lê cada empréstimo do arquivo até o final ou até atingir o máximo permitido
    while (!feof(arquivo) && quantidade_emprestimos < MAX_EMPRESTIMOS)
    {
        // Lê e armazena o RA do aluno
        fscanf(arquivo, "RA: %[^\n]\n", emprestimos[quantidade_emprestimos].ra);        
        // Lê e armazena o título do livro emprestado
        fscanf(arquivo, "Titulo: %[^\n]\n", emprestimos[quantidade_emprestimos].titulo_livro);        
        // Lê e armazena o ISBN do livro emprestado
        fscanf(arquivo, "ISBN: %[^\n]\n", emprestimos[quantidade_emprestimos].isbn);
        // Lê e armazena a data do empréstimo
        fscanf(arquivo, "Data de Emprestimo: %[^\n]\n", emprestimos[quantidade_emprestimos].data_emprestimo);
        // Lê a linha divisória entre empréstimos, mas não armazena essa informação
        fscanf(arquivo, "------------------------------\n");
        // Incrementa a contagem de empréstimos após carregar um conjunto de dados
        quantidade_emprestimos++;
    }
    // Fecha o arquivo após o carregamento
    fclose(arquivo);
    // Exibe uma mensagem indicando que o carregamento foi bem-sucedido
    printf("Empréstimos carregados com sucesso de 'emprestimos.txt'.\n");
}

// Função para salvar os dados dos usuários cadastrados no arquivo "usuarios.txt"
void salvar_usuarios(Usuario usuarios[], int quantidade_usuarios)
{
    // Abre o arquivo "usuarios.txt" em modo de escrita (cria ou substitui o arquivo existente)
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "w");
    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar os usuarios.\n"); // Exibe mensagem de erro caso não consiga abrir o arquivo
        return; // Sai da função se ocorrer um erro ao abrir o arquivo
    }
    // Itera sobre todos os usuários cadastrados
    for (int i = 0; i < quantidade_usuarios; i++)
    {
        // Escreve os dados de cada usuário no arquivo, separando os campos por ponto e vírgula
        // Os dados incluem o RA, nome, e-mail e senha do usuário
        fprintf(arquivo, "%s;%s;%s;%s\n", usuarios[i].ra, usuarios[i].nome, usuarios[i].email, usuarios[i].senha);
    }
    // Fecha o arquivo após a escrita dos dados
    fclose(arquivo);
}

// Função para carregar os dados dos usuários salvos no arquivo "usuarios.txt"
int carregar_usuarios(Usuario usuarios[])
{
    // Abre o arquivo "usuarios.txt" em modo de leitura
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "r");
    // Verifica se o arquivo foi aberto com sucesso
    if (arquivo == NULL)
    {
        printf("Arquivo de usuarios nao encontrado. Nenhum usuario carregado.\n"); // Exibe mensagem de erro caso não consiga abrir o arquivo
        return 0; // Retorna 0 caso o arquivo não exista ou não possa ser aberto
    }
    // Variável para contar a quantidade de usuários carregados
    int quantidade = 0;
    // Lê os dados dos usuários no arquivo, separando os campos por ponto e vírgula
    // O formato esperado para cada linha é: RA;nome;email;senha
    // O loop continua enquanto a leitura dos 4 campos for bem-sucedida
    while (fscanf(arquivo, "%99[^;];%99[^;];%99[^;];%99[^\n]\n",
                  usuarios[quantidade].ra,
                  usuarios[quantidade].nome,
                  usuarios[quantidade].email,
                  usuarios[quantidade].senha) == 4)
    {
        quantidade++; // Incrementa a quantidade de usuários carregados
        // Verifica se o número de usuários excedeu o limite máximo (MAX_USUARIOS)
        if (quantidade >= MAX_USUARIOS)
        {
            break; // Interrompe o loop para evitar ultrapassar o número máximo de usuários
        }
    }
    // Fecha o arquivo após a leitura dos dados
    fclose(arquivo);
    // Exibe a quantidade de usuários carregados com sucesso
    printf("Usuarios carregados com sucesso! Total: %d\n", quantidade);
    // Retorna a quantidade de usuários carregados
    return quantidade;
}

// Função para registrar um novo aluno no sistema
void registrar_aluno(Usuario usuarios[], int *quantidade_usuarios)
{
    // Verifica se o número máximo de usuários foi atingido
    if (*quantidade_usuarios >= MAX_USUARIOS)
    {
        printf("Limite de usuarios atingido.\n"); // Exibe mensagem de erro se o limite for atingido
        return; // Encerra a função se o número de usuários estiver no limite
    }
    char ra[100], email[100];
    // Solicita o RA do aluno e remove o caractere de nova linha no final da string
    printf("RA do aluno: ");
    getchar(); // Limpa o buffer de entrada
    fgets(ra, sizeof(ra), stdin); // Lê o RA do aluno
    ra[strcspn(ra, "\n")] = 0; // Remove o caractere de nova linha da string
    // Verifica se o RA já está registrado no sistema
    for (int i = 0; i < *quantidade_usuarios; i++)
    {
        if (strcmp(usuarios[i].ra, ra) == 0) // Compara o RA com os já registrados
        {
            printf("Erro: O RA fornecido ja esta registrado.\n"); // Exibe erro caso o RA já esteja registrado
            return; // Encerra a função se o RA já existir
        }
    }
    // Solicita o e-mail do aluno e remove o caractere de nova linha
    printf("Email do aluno: ");
    fgets(email, sizeof(email), stdin); // Lê o e-mail do aluno
    email[strcspn(email, "\n")] = 0; // Remove o caractere de nova linha
    // Verifica se o e-mail já está registrado no sistema
    for (int i = 0; i < *quantidade_usuarios; i++)
    {
        if (strcmp(usuarios[i].email, email) == 0) // Compara o e-mail com os já registrados
        {
            printf("Erro: O email fornecido ja esta registrado.\n"); // Exibe erro caso o e-mail já esteja registrado
            return; // Encerra a função se o e-mail já existir
        }
    }
    // Caso o RA e o e-mail sejam únicos, prossegue com o registro do aluno
    strcpy(usuarios[*quantidade_usuarios].ra, ra); // Armazena o RA do aluno
    strcpy(usuarios[*quantidade_usuarios].email, email); // Armazena o e-mail do aluno
    // Solicita o nome do aluno e armazena na estrutura
    printf("Nome do aluno: ");
    fgets(usuarios[*quantidade_usuarios].nome, sizeof(usuarios[*quantidade_usuarios].nome), stdin);
    usuarios[*quantidade_usuarios].nome[strcspn(usuarios[*quantidade_usuarios].nome, "\n")] = 0; // Remove o caractere de nova linha
    // Chama a função para ler a senha do aluno de forma oculta
    ler_senha_oculta(usuarios[*quantidade_usuarios].senha, sizeof(usuarios[*quantidade_usuarios].senha));
    // Incrementa o contador de usuários
    (*quantidade_usuarios)++;
    // Exibe uma mensagem de sucesso no registro do aluno
    printf("Aluno registrado com sucesso!\n");
    // Salva os dados atualizados de usuários no arquivo
    salvar_usuarios(usuarios, *quantidade_usuarios);
}

// Função para redefinir a senha do aluno
void redefinir_senha(Usuario usuarios[], int quantidade_usuarios)
{
    char ra[100]; // Variável para armazenar o RA do aluno
    printf("Digite o RA do aluno: "); // Solicita o RA do aluno
    getchar(); // Limpa o buffer de entrada
    fgets(ra, sizeof(ra), stdin); // Lê o RA fornecido pelo usuário
    ra[strcspn(ra, "\n")] = 0; // Remove o caractere de nova linha da string
    // Procura o aluno pelo RA fornecido
    for (int i = 0; i < quantidade_usuarios; i++)
    {
        if (strcmp(usuarios[i].ra, ra) == 0) // Compara o RA fornecido com os registrados
        {
            // Se o RA for encontrado, solicita uma nova senha
            printf("Digite a nova senha \n");
            ler_senha_oculta(usuarios[i].senha, sizeof(usuarios[i].senha)); // Lê a nova senha de forma oculta
            printf("Senha redefinida com sucesso!\n"); // Informa ao usuário que a senha foi redefinida
            // Salva os dados atualizados dos usuários no arquivo
            salvar_usuarios(usuarios, quantidade_usuarios);
            return; // Encerra a função após redefinir a senha
        }
    }
    // Se o RA não for encontrado, exibe uma mensagem de erro
    printf("RA nao encontrado.\n");
}

// Função para adicionar livros à biblioteca
void adicionar_livro()
{
    char titulo[100], autor[100], editora[100], isbn[20]; // Variáveis para armazenar as informações do livro
    int quantidade; // Variável para armazenar a quantidade de livros a ser adicionada

    // Verifica se o limite máximo de livros foi atingido
    if (quantidade_livros >= MAX_LIVROS)
    {
        printf("Limite de livros atingido.\n"); // Informa que não é possível adicionar mais livros
        return; // Encerra a função
    }
    // Solicita e lê os dados do livro
    printf("Titulo do livro: ");
    getchar(); // Limpa o buffer de entrada
    fgets(titulo, sizeof(titulo), stdin); // Lê o título do livro
    titulo[strcspn(titulo, "\n")] = 0; // Remove o caractere de nova linha da string

    printf("Autor do livro: ");
    fgets(autor, sizeof(autor), stdin); // Lê o autor do livro
    autor[strcspn(autor, "\n")] = 0; // Remove o caractere de nova linha

    printf("Editora do livro: ");
    fgets(editora, sizeof(editora), stdin); // Lê a editora do livro
    editora[strcspn(editora, "\n")] = 0; // Remove o caractere de nova linha

    printf("ISBN do livro: ");
    fgets(isbn, sizeof(isbn), stdin); // Lê o ISBN do livro
    isbn[strcspn(isbn, "\n")] = 0; // Remove o caractere de nova linha

    printf("Quantidade de livros a serem adicionados: ");
    scanf("%d", &quantidade); // Lê a quantidade de livros a ser adicionada
    // Verifica se o livro já existe na biblioteca
    for (int i = 0; i < quantidade_livros; i++)
    {
        // Se o título e ISBN coincidirem, o livro já existe
        if (strcmp(livros[i].titulo, titulo) == 0 && strcmp(livros[i].isbn, isbn) == 0)
        {
            // Aumenta a quantidade de exemplares do livro
            livros[i].quantidade += quantidade;
            livros[i].disponivel = livros[i].quantidade > 0 ? 1 : 0; // Atualiza a disponibilidade do livro
            printf("Quantidade do livro '%s' atualizada para %d.\n", titulo, livros[i].quantidade);
            return; // Encerra a função, pois o livro foi atualizado
        }
    }
    // Se o livro não existe, cria um novo registro para ele
    strcpy(livros[quantidade_livros].titulo, titulo); // Copia o título
    strcpy(livros[quantidade_livros].autor, autor); // Copia o autor
    strcpy(livros[quantidade_livros].editora, editora); // Copia a editora
    strcpy(livros[quantidade_livros].isbn, isbn); // Copia o ISBN
    livros[quantidade_livros].quantidade = quantidade; // Define a quantidade de livros
    livros[quantidade_livros].disponivel = quantidade > 0 ? 1 : 0; // Define se o livro está disponível
    quantidade_livros++; // Aumenta a quantidade total de livros
    // Informa que o livro foi adicionado com sucesso
    printf("Livro adicionado com sucesso!\n");
    // Salva a lista atualizada de livros no arquivo
    salvar_livros(livros, quantidade_livros);
}

// Função para remover livro da biblioteca
void remover_livro()
{
    char titulo[100], isbn[20]; // Variáveis para armazenar o título e ISBN do livro
    int quantidade_a_remover; // Variável para armazenar a quantidade de cópias a ser removida

    // Solicita o título do livro a ser removido
    printf("\nDigite o titulo do livro que deseja remover: ");
    getchar(); // Limpa o buffer de entrada
    fgets(titulo, sizeof(titulo), stdin); // Lê o título do livro
    titulo[strcspn(titulo, "\n")] = 0; // Remove o caractere de nova linha da string

    // Solicita o ISBN do livro a ser removido
    printf("Digite o ISBN do livro: ");
    fgets(isbn, sizeof(isbn), stdin); // Lê o ISBN do livro
    isbn[strcspn(isbn, "\n")] = 0; // Remove o caractere de nova linha

    // Solicita a quantidade de cópias a serem removidas
    printf("Quantas copias deseja remover: ");
    scanf("%d", &quantidade_a_remover); // Lê a quantidade de cópias

    // Percorre a lista de livros para encontrar o livro com o título e ISBN fornecidos
    for (int i = 0; i < quantidade_livros; i++)
    {
        // Verifica se o título e o ISBN do livro correspondem
        if (strcmp(livros[i].titulo, titulo) == 0 && strcmp(livros[i].isbn, isbn) == 0)
        {
            // Verifica se a quantidade de livros no estoque é suficiente para a remoção
            if (livros[i].quantidade >= quantidade_a_remover)
            {
                // Reduz a quantidade de cópias do livro
                livros[i].quantidade -= quantidade_a_remover;
                printf("Removidas %d copias do livro '%s'.\n", quantidade_a_remover, titulo);

                // Se a quantidade do livro chega a zero, define que o livro não está mais disponível
                if (livros[i].quantidade == 0)
                {
                    livros[i].disponivel = 0; // O livro não está mais disponível para empréstimo
                    salvar_livros(livros, quantidade_livros); // Salva as alterações no arquivo
                }
                return; // Encerra a função
            }
            else
            {
                // Caso a quantidade solicitada para remoção seja maior que a disponível
                printf("Quantidade solicitada para remoção maior do que a disponível. Remover apenas %d copias.\n", livros[i].quantidade);
                livros[i].quantidade = 0; // Define que todas as cópias do livro foram removidas
                livros[i].disponivel = 0; // O livro não está mais disponível para empréstimo
                return; // Encerra a função
            }
        }
    }
    printf("Livro nao encontrado.\n"); // Informa que o livro não foi encontrado
}

// Função de ordenação alfabética dos livros
void ordenar_livros_alfabeticamente(Livro livros[], int quantidade_livros) {
    for (int i = 0; i < quantidade_livros - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < quantidade_livros; j++) {
            if (strcmp(livros[j].titulo, livros[min_idx].titulo) < 0) {
                min_idx = j;
            }
        }
        // Troca de posição
        Livro temp = livros[i];
        livros[i] = livros[min_idx];
        livros[min_idx] = temp;
    }
}

// Função para visualizar livros
void visualizar_livros(Livro livros[], int quantidade_livros) {
    // Verifica se não há livros cadastrados no sistema
    if (quantidade_livros == 0) {
        printf("Nenhum livro cadastrado no sistema.\n");
        return; // Se não houver livros, sai da função
    }
    // Ordena os livros em ordem alfabética pelo título usando a função ordenar_livros_alfabeticamente
    ordenar_livros_alfabeticamente(livros, quantidade_livros);
    // Exibe a lista de livros cadastrados
    printf("--- Lista de Livros ---\n");
    // Loop para exibir os detalhes de cada livro
    for (int i = 0; i < quantidade_livros; i++) {
        printf("Titulo: %s\n", livros[i].titulo);       // Exibe o título do livro
        printf("Autor: %s\n", livros[i].autor);         // Exibe o autor do livro
        printf("Quantidade: %d\n", livros[i].quantidade); // Exibe a quantidade de cópias do livro
        printf("ISBN: %s\n", livros[i].isbn);           // Exibe o ISBN do livro
        printf("----------------------\n");              // Linha separadora entre livros
    }
}

// Função para emprestar livros
void emprestar_livro(char ra[])
{
    char titulo[100], isbn[20], data_emprestimo[11];

    // Contar quantos livros o usuário já emprestou
    int livros_emprestados = 0;
    for (int i = 0; i < quantidade_emprestimos; i++)
    {
        if (strcmp(emprestimos[i].ra, ra) == 0) // Verifica se o RA do empréstimo é igual ao informado
        {
            livros_emprestados++; // Incrementa o contador de livros emprestados
        }
    }

    // Verifica se o usuário já atingiu o limite de empréstimos
    if (livros_emprestados >= MAX_EMPRESTIMOS_POR_USUARIO)
    {
        printf("Limite de %d livros emprestados atingido. Devolva um livro para emprestar outro.\n", MAX_EMPRESTIMOS_POR_USUARIO);
        return; // Se o limite for atingido, não permite o empréstimo e encerra a função
    }

    // Solicita o título do livro
    printf("\nDigite o titulo do livro que deseja emprestar: ");
    getchar(); // Limpa o buffer de entrada
    fgets(titulo, sizeof(titulo), stdin); // Lê o título do livro
    titulo[strcspn(titulo, "\n")] = 0; // Remove o caractere de nova linha no final do título

    // Solicita o ISBN do livro
    printf("Digite o ISBN do livro: ");
    fgets(isbn, sizeof(isbn), stdin); // Lê o ISBN do livro
    isbn[strcspn(isbn, "\n")] = 0; // Remove o caractere de nova linha no final do ISBN

    // Solicita a data de empréstimo
    printf("Digite a data de emprestimo (dd/mm/aaaa): ");
    fgets(data_emprestimo, sizeof(data_emprestimo), stdin); // Lê a data de empréstimo
    data_emprestimo[strcspn(data_emprestimo, "\n")] = 0; // Remove o caractere de nova linha no final da data

    // Verifica se o livro existe, está disponível e tem cópias disponíveis
    for (int i = 0; i < quantidade_livros; i++)
    {
        if (strcmp(livros[i].titulo, titulo) == 0 && strcmp(livros[i].isbn, isbn) == 0 && livros[i].disponivel && livros[i].quantidade > 0)
        {
            // Atualiza a quantidade do livro, decrementando uma cópia
            livros[i].quantidade--;
            if (livros[i].quantidade == 0)
            {
                livros[i].disponivel = 0; // O livro não está mais disponível para empréstimo
            }
            // Registra o empréstimo
            strcpy(emprestimos[quantidade_emprestimos].ra, ra); // Armazena o RA do aluno
            strcpy(emprestimos[quantidade_emprestimos].titulo_livro, titulo); // Armazena o título do livro emprestado
            strcpy(emprestimos[quantidade_emprestimos].isbn, isbn); // Armazena o ISBN do livro emprestado
            strcpy(emprestimos[quantidade_emprestimos].data_emprestimo, data_emprestimo); // Armazena a data de empréstimo
            quantidade_emprestimos++; // Incrementa o contador de empréstimos
            printf("Livro emprestado com sucesso!\n");
            // Salva os empréstimos atualizados no arquivo
            salvar_emprestimos_em_arquivo();
            return;
        }
    }
    // Se o livro não foi encontrado ou não está disponível, exibe uma mensagem de erro
    printf("Livro indisponivel ou nao encontrado.\n");
}

//funçao para converter data para inteiro
int converter_data_para_inteiro(const char *data)
{
    int dia, mes, ano;
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    return ano * 10000 + mes * 100 + dia; // Retorna no formato aaaammdd
}

// Função para devolver livro
void devolver_livro(char ra[])
{
    char titulo[100], isbn[20], data_devolucao[11];

    // Solicita o título do livro
    printf("\nDigite o titulo do livro que deseja devolver: ");
    getchar(); // Limpa o buffer de entrada
    fgets(titulo, sizeof(titulo), stdin); // Lê o título do livro
    titulo[strcspn(titulo, "\n")] = 0; // Remove o caractere de nova linha no final do título

    // Solicita o ISBN do livro
    printf("Digite o ISBN do livro: ");
    fgets(isbn, sizeof(isbn), stdin); // Lê o ISBN do livro
    isbn[strcspn(isbn, "\n")] = 0; // Remove o caractere de nova linha no final do ISBN

    // Solicita a data de devolução
    printf("Digite a data de devolucao (dd/mm/aaaa): ");
    fgets(data_devolucao, sizeof(data_devolucao), stdin); // Lê a data de devolução
    data_devolucao[strcspn(data_devolucao, "\n")] = 0; // Remove o caractere de nova linha no final da data

    // Converte a data de devolução para um valor inteiro para comparação
    int data_devolucao_int = converter_data_para_inteiro(data_devolucao);

    // Verifica se o aluno fez o empréstimo do livro
    for (int i = 0; i < quantidade_emprestimos; i++)
    {
        if (strcmp(emprestimos[i].ra, ra) == 0 && strcmp(emprestimos[i].titulo_livro, titulo) == 0 && strcmp(emprestimos[i].isbn, isbn) == 0)
        {
            // Converte a data de empréstimo para um valor inteiro para comparação
            int data_emprestimo_int = converter_data_para_inteiro(emprestimos[i].data_emprestimo);

            // Verifica se a data de devolução é válida (não pode ser anterior à data do empréstimo)
            if (data_devolucao_int < data_emprestimo_int)
            {
                printf("Erro: A data de devolucao nao pode ser anterior a data de emprestimo.\n");
                return; // Se a data for inválida, encerra a função
            }
            // O aluno fez o empréstimo do livro, agora vamos devolvê-lo
            for (int j = 0; j < quantidade_livros; j++)
            {
                if (strcmp(livros[j].isbn, isbn) == 0)
                {
                    livros[j].quantidade++;   // Aumenta a quantidade do livro disponível
                    livros[j].disponivel = 1; // Marca o livro como disponível
                    printf("Livro devolvido com sucesso!\n");

                    // Remove o registro do empréstimo da lista
                    for (int k = i; k < quantidade_emprestimos - 1; k++)
                    {
                        emprestimos[k] = emprestimos[k + 1]; // Desloca os registros para preencher a posição do empréstimo removido
                    }
                    quantidade_emprestimos--; // Atualiza a quantidade de empréstimos
                    salvar_emprestimos_em_arquivo(); // Salva a lista de empréstimos atualizada
                    return;
                }
            }
        }
    }
    // Se o empréstimo não foi encontrado ou não pertence ao aluno, exibe uma mensagem de erro
    printf("Emprestimo nao encontrado ou nao pertence a este aluno.\n");
}

// Função para visualizar empréstimos feitos
void visualizar_emprestimos()
{
    // Exibe o título da seção
    printf("\n--- Livros Emprestados ---\n");
    // Loop para percorrer todos os empréstimos registrados
    for (int i = 0; i < quantidade_emprestimos; i++)
    {
        i+1;
        // Exibe as informações do empréstimo
        printf("Titulo: %s\n", emprestimos[i].titulo_livro); // Título do livro emprestado
        printf("RA aluno: %s\n", emprestimos[i].ra); // RA do aluno que fez o empréstimo
        printf("ISBN: %s\n", emprestimos[i].isbn); // ISBN do livro emprestado
        printf("Data emprestimo: %s\n", emprestimos[i].data_emprestimo); // Data de empréstimo
        // Separador entre os empréstimos
        printf("---------------------\n");
    }
    // Nova linha ao final para formatação
    printf("\n");
}

// Função para o menu do administrador
void menuAdmin()
{
    int opcao;  // Variável para armazenar a escolha do administrador
    do
    {
        system("cls");  // Limpa a tela 
        // Exibe o título do menu
        printf("--- Menu Administrador ---\n");
        // Exibe as opções disponíveis para o administrador
        printf("1. Adicionar Livro\n");
        printf("2. Remover Livro\n");
        printf("3. Visualizar Livros\n");
        printf("4. Visualizar Livros Emprestados\n");
        printf("0. Sair\n");
        // Solicita ao administrador que escolha uma opção
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Processa a escolha do administrador
        switch (opcao)
        {
        case 1:
            // Caso 1: Chama a função para adicionar um livro
            adicionar_livro();
            break;
        case 2:
            // Caso 2: Chama a função para remover um livro
            remover_livro();
            break;
        case 3:
            // Caso 3: Chama a função para visualizar a lista de livros
            visualizar_livros(livros, quantidade_livros);
            break;
        case 4:
            // Caso 4: Chama a função para visualizar os empréstimos realizados
            visualizar_emprestimos();
            break;
        case 0:
            // Caso 0: Encerra o menu do administrador
            printf("Saindo do menu do administrador...\n");
            break;
        default:
            // Caso inválido: Informa que a opção fornecida não é válida
            printf("Opcao invalida! Tente novamente.\n");
            break;
        }
        // Pausa a execução do programa até o usuário pressionar uma tecla (para sistemas Windows)
        system("pause");
    } while (opcao != 0);  // O loop continua até que a opção 0 seja escolhida para sair
}

// Função para login do administrador
void login_administrador()
{
    char usuario[100], senha[100];  // Variáveis para armazenar o nome de usuário e a senha do administrador
    // Solicita ao administrador que digite o nome de usuário
    printf("Digite o usuario do administrador: ");
    getchar();  // Limpa o buffer de entrada
    fgets(usuario, sizeof(usuario), stdin);  // Lê o nome de usuário, incluindo espaços
    usuario[strcspn(usuario, "\n")] = 0;  // Remove o caractere de nova linha no final da string
    // Solicita ao administrador que digite a senha (utilizando uma função para ocultar a entrada)
    ler_senha_oculta(senha, sizeof(senha));
    // Compara o nome de usuário e a senha digitados com as credenciais pré-definidas (ADMIN_USUARIO e ADMIN_SENHA)
    if (strcmp(usuario, ADMIN_USUARIO) == 0 && strcmp(senha, ADMIN_SENHA) == 0)
    {
        // Se o nome de usuário e a senha estiverem corretos, exibe uma mensagem de sucesso e entra no menu do administrador
        printf("Login bem-sucedido!\n");
        system("pause");  // Pausa a execução até o usuário pressionar uma tecla (para sistemas Windows)
        menuAdmin();  // Chama a função do menu do administrador para continuar o gerenciamento
    }
    else
    {
        // Caso o nome de usuário ou a senha estejam incorretos, exibe uma mensagem de erro
        printf("Usuario ou senha incorretos!\n");
        system("pause");  // Pausa a execução para que o administrador possa ver a mensagem de erro
    }
}

// Função para login do aluno
void login_aluno(Usuario usuarios[], int quantidade_usuarios)
{
    char ra[100], senha[100];  // Variáveis para armazenar o RA e a senha do aluno
    // Solicita que o aluno digite seu RA
    printf("Digite seu RA: ");
    getchar();  // Limpa o buffer de entrada
    fgets(ra, sizeof(ra), stdin);  // Lê o RA digitado, permitindo espaços
    ra[strcspn(ra, "\n")] = 0;  // Remove o caractere de nova linha no final do RA

    // Solicita que o aluno digite sua senha (utilizando uma função para ocultar a entrada)
    ler_senha_oculta(senha, sizeof(senha));

    // Percorre a lista de usuários para verificar se o RA e a senha correspondem a um usuário válido
    for (int i = 0; i < quantidade_usuarios; i++)
    {
        // Se o RA e a senha do aluno corresponderem aos dados armazenados, o login é bem-sucedido
        if (strcmp(usuarios[i].ra, ra) == 0 && strcmp(usuarios[i].senha, senha) == 0)
        {
            printf("Login bem-sucedido!\n");
            system("pause");  // Pausa a execução para que o aluno veja a mensagem de sucesso

            int opcao;
            do
            {
                // Exibe o menu de opções do aluno
                system("cls");
                printf("--- Menu Aluno ---\n");
                printf("1. Emprestar Livro\n");
                printf("2. Devolver Livro\n");
                printf("3. Visualizar Livros\n");
                printf("0. Sair\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &opcao);
                // Processa a opção escolhida pelo aluno
                switch (opcao)
                {
                case 1:
                    emprestar_livro(ra);  // Chama a função para emprestar um livro
                    break;
                case 2:
                    devolver_livro(ra);  // Chama a função para devolver um livro
                    break;
                case 3:
                    visualizar_livros(livros, quantidade_livros);  // Chama a função para visualizar os livros disponíveis
                    break;
                case 0:
                    // Caso a opção seja 0, o aluno sai do menu e os dados são salvos
                    printf("Saindo do menu do aluno...\n");
                    salvar_emprestimos_em_arquivo();  // Salva os registros de empréstimos
                    salvar_livros(livros, quantidade_livros);  // Salva a lista de livros
                    salvar_usuarios(usuarios, quantidade_usuarios);  // Salva a lista de usuários
                    break;
                default:
                    // Se a opção for inválida, exibe uma mensagem de erro
                    printf("Opcao invalida! Tente novamente.\n");
                    break;
                }
                system("pause");  // Pausa a execução após a escolha de cada opção
            } while (opcao != 0);  // Continua o loop até o aluno escolher sair
            return;  // Encerra a função após o aluno ter realizado suas ações no menu
        }
    }

    // Caso o RA ou a senha não sejam encontrados, exibe uma mensagem de erro
    printf("RA ou senha incorretos!\n");
    system("pause");  // Pausa a execução para que o aluno veja a mensagem de erro
}

// Função principal
int main()
{
    int opcao;  // Variável para armazenar a escolha do usuário no menu principal
    // Carrega os dados iniciais (usuários, livros e empréstimos) do arquivo
    quantidade_usuarios = carregar_usuarios(usuarios);
    quantidade_livros = carregar_livros(livros);
    carregar_emprestimos_do_arquivo();
    do
    {
        // Exibe o menu principal
        system("cls");  // Limpa a tela
        printf("--- Sistema de Biblioteca ---\n");
        printf("1. Registrar Aluno\n");
        printf("2. Login Aluno\n");
        printf("3. Login Administrador\n");
        printf("4. Redefinir Senha\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);  // Lê a opção escolhida pelo usuário
        // Processa a opção escolhida
        switch (opcao)
        {
        case 1:
            system("cls");  // Limpa a tela
            registrar_aluno(usuarios, &quantidade_usuarios);  // Chama a função para registrar um aluno
            system("pause");  // Pausa para o usuário ver a mensagem
            break;
        case 2:
            system("cls");  // Limpa a tela
            login_aluno(usuarios, quantidade_usuarios);  // Chama a função para fazer o login do aluno
            break;
        case 3:
            system("cls");  // Limpa a tela
            login_administrador();  // Chama a função para fazer o login do administrador
            break;
        case 4:
            system("cls");  // Limpa a tela
            redefinir_senha(usuarios, quantidade_usuarios);  // Chama a função para redefinir a senha
            system("pause");  // Pausa para o usuário ver a mensagem
            break;
        case 0:
            // Caso a opção seja 0, o sistema vai sair, e os dados serão salvos
            printf("Saindo do sistema...\n");
            salvar_emprestimos_em_arquivo();  // Salva os empréstimos no arquivo
            salvar_livros(livros, quantidade_livros);  // Salva os livros no arquivo
            salvar_usuarios(usuarios, quantidade_usuarios);  // Salva os usuários no arquivo
            break;
        default:
            // Caso a opção não seja válida, exibe uma mensagem de erro
            printf("Opcao invalida! Tente novamente.\n");
            break;
        }
    } while (opcao != 0);  // Continua o loop até o usuário escolher sair (opção 0)
    return 0;  // Retorna 0, sinalizando que o programa terminou com sucesso
}