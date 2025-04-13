//SISTEMA PARA BIBLIOTECA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define QTD_MAX_USUARIOS 1000
#define QTD_MAX_LIVROS 2000
#define QTD_MAX_EMPRESTIMOS 5000
#define TAM_STRING 50
#define TAMX2_STRING 100
typedef char string[TAM_STRING];

struct livro
{
    int id;
    string titulo;
    string editora, local_publicacao;
    string palavra_1, palavra_2, palavra_3;
    int ano_publicacao, qtd_paginas, qtd_autores, qtd_palavras, edicao, qtd_exemplares;
    string autor_1, autor_2, autor_3;

};

struct usuario
{
    string nome;
    string endereco;
    string telefone;
    string email;
    int qtd_livros_emprestados;
    int cracha;
    bool usuario_ativo;
    int pendencia;
};

struct emprestimo
{
    string titulo;
    int cracha_usuario;
    int id_livro;
    int pos_usu, pos_liv, pos_emp;
    int qtd_dias;
    bool emp_ativo;
    int multa;
    int dias_atraso;
};

//variaveis globais:
struct livro vetor_livro[QTD_MAX_LIVROS];
struct usuario vetor_usuario[QTD_MAX_USUARIOS];
struct emprestimo vetor_emprestimo[QTD_MAX_EMPRESTIMOS];

int qtd_livros = 0;
int qtd_usuarios = 0;
int qtd_emprestimos = 0;

//cabeçalho das subrotinas:
struct livro cadastro_exemplares();
struct usuario cadastro_usuario();
int menu();
int menu_relatorios();
struct emprestimo novo_emprestimo(int qtd_dias, int pos_livro, int pos_usuario);
int localizar_usuario();
int localizar_livro();
int localizar_emprestimo();
void mostrar_dados_usuario(int pos);
void mostrar_dados_livro(int pos);
void mostrar_dados_emprestimo(int pos);
void mostrar_pendencias();
void retirar_enter_string(string str);
void formatar_maiuscula(string str);
void relatorio_acervo();
void relatorio_usuario();
void relatorio_emprestimos();
void controle_exemplares(int pos);
void relatorio_faixa_ano();
int devolucao(int pos_emprestimo);

int main(){
    int opcao, opc_relatorio, dev;
    int idx_livro, idx_usuario, idx_emprestimo, qtd_dias;
    do
    {
        opcao = menu();        

        switch (opcao)
        {
        case 0:
            printf("Tchau!");
            break;

        case 1: //cadastro de novos exemplares:
            printf("Cadastro de novos exemplares:\n");
            vetor_livro[qtd_livros] = cadastro_exemplares() ;
            qtd_livros ++;
            break;

        case 2: //cadastro de novos usuarios:
            printf("Cadastro de novos usuarios: \n");
            vetor_usuario[qtd_usuarios] = cadastro_usuario();
            qtd_usuarios++;
            break;
            
            
        case 3: //localizar exemplares
        
            idx_livro = localizar_livro();

            mostrar_dados_livro(idx_livro);
            
            break;
        
        case 4: //localizar usuario
            idx_usuario = localizar_usuario();

            mostrar_dados_usuario(idx_usuario);
            break;

        case 5: //cadastrar emprestimo
            idx_livro = localizar_livro();
            idx_usuario = localizar_usuario();

            printf("Digite a quantidade de dias: ");
            scanf("%i", &qtd_dias);
            fflush(stdin);
            vetor_emprestimo[qtd_emprestimos] = novo_emprestimo(qtd_dias, idx_livro, idx_usuario);

            if(vetor_emprestimo[qtd_emprestimos].emp_ativo == true)
            {
                qtd_emprestimos ++;
                vetor_usuario[idx_usuario].qtd_livros_emprestados ++;
            }
            break;
        
        case 6: //Devolucao --> Inativar um emprestimo e aumentar 1 exemplar
            //vetorempr[emprestimo] --> ativo = false qtdexemplar++
            idx_emprestimo = localizar_emprestimo();
            if(idx_emprestimo >= 0)
            {
                dev = devolucao(idx_emprestimo);
                if(dev == 1)
                {
                    printf("Devolucao realizada com sucesso\n");
                }
                    else
                    {
                        printf("Falha na devolucao. Tente novamente!\n");
                    }
            }
            break;

        case 7: //Baixa e acrescimo de exemplares
            idx_livro = localizar_livro();

            controle_exemplares(idx_livro);

            break;
        case 8:
            opc_relatorio = menu_relatorios();

            if(opc_relatorio == 1)//relatorio do acervo
            {
                relatorio_acervo();
            }
                else if(opc_relatorio == 2)//relatorio de usuarios
                {
                    relatorio_usuario();   
                }
                    else if(opc_relatorio == 3)//relatorio de emprestimos
                    {
                        relatorio_emprestimos();
                    }
                        else if(opc_relatorio == 4)//relatorio de pendencias
                        {
                            mostrar_pendencias();
                        }
                            else if(opc_relatorio == 5)//faixa de ano
                            {
                                relatorio_faixa_ano();
                            }
                                else
                                {
                                    printf("Opcao invalida.\n");
                                }
            break;
        default:
            printf("Opcao invalida. Tente novamente!");
            break;
        }
        
    } while (opcao != 0);
    return 0;    
}

int menu()
{
    int opc;
    printf("\n**MENU**\n");
    printf("0. Sair do programa\n");
    printf("1. Cadastro de novos exemplares\n");
    printf("2. Cadastro de novos usuarios\n");
    printf("3 .Localizar exemplares\n");
    printf("4. Localizar usuario\n");
    printf("5. Novo emprestimo\n");
    printf("6. Devolucao e atrasos\n");
    printf("7. Baixa e acrescimo de exemplares\n");
    printf("8. Relatorios\n");
    printf(":: ");
    scanf("%i", &opc);
    fflush(stdin);

    return opc;
}

int menu_relatorios()
{
    int opc;
    printf("\n0 - Voltar ao menu\n");
    printf("1 - Relatorio de acervo\n");
    printf("2 - Relatorio de usuarios\n");
    printf("3 - Relatorio de emprestimos\n");
    printf("4 - Relatorio de pendencias\n");
    printf("5 - Relatorio de livros editados em uma determinada faixa de anos\n");
    printf(":: ");
    scanf("%i", &opc);
    getchar();

    return opc;
}
 
struct livro cadastro_exemplares()
{
    struct livro novo;
    int opc;
    novo.qtd_autores = 0;
    novo.qtd_palavras = 0;

    fflush(stdin);

    printf("Digite o titulo do livro...: ");
    fgets(novo.titulo, TAM_STRING, stdin);
    retirar_enter_string(novo.titulo);
    formatar_maiuscula(novo.titulo);

    printf("Digite o nome do primeiro autor(a): ");
    fgets(novo.autor_1, TAM_STRING, stdin);
    retirar_enter_string(novo.autor_1);
    formatar_maiuscula(novo.autor_1);
    novo.qtd_autores ++;

    printf("Possui mais um autor?\n");
    printf("1 - Sim\n");
    printf("2 - Nao\n");
    printf(":: ");
    scanf("%i", &opc);
    fflush(stdin);
    if(opc == 1)
    {
        printf("Digite o nome do segundo autor(a): ");
        fgets(novo.autor_2, TAM_STRING, stdin);
        retirar_enter_string(novo.autor_2);
        formatar_maiuscula(novo.autor_2);
        novo.qtd_autores ++;

        printf("Possui mais um autor?\n");
        printf("1 - Sim\n");
        printf("2 - Nao\n");
        printf(":: ");
        scanf("%i", &opc);
        fflush(stdin);

        if(opc == 1)
        {
            printf("Digite o nome do terceiro autor(a): ");
            fgets(novo.autor_3, TAM_STRING, stdin);
            retirar_enter_string(novo.autor_3);
            formatar_maiuscula(novo.autor_3);
            novo.qtd_autores ++;
            fflush(stdin);
        }
    }

    //cadastrar palavras chaves
    printf("Digite uma palavra chave: ");
    fgets(novo.palavra_1, TAM_STRING, stdin);
    retirar_enter_string(novo.palavra_1);
    formatar_maiuscula(novo.palavra_1);
    novo.qtd_palavras ++;

    printf("Quer cadastrar mais uma palavra? \n");
    printf("1 - Sim\n");
    printf("2 - Nao\n");
    printf(":: ");
    scanf("%i", &opc);
    fflush(stdin);
    
    if(opc == 1)
    {
        printf("Digite mais uma palavra: ");
        fgets(novo.palavra_2, TAM_STRING, stdin);
        retirar_enter_string(novo.palavra_2);
        formatar_maiuscula(novo.palavra_2);
        novo.qtd_palavras ++;

        printf("Quer cadastrar mais uma palavra? \n");
        printf("1 - Sim\n");
        printf("2 - Nao\n");
        printf(":: ");
        scanf("%i", &opc);
        fflush(stdin);
        if(opc == 1)
        {
            printf("Digite mais uma palavra: ");
            fgets(novo.palavra_3, TAM_STRING, stdin);
            retirar_enter_string(novo.palavra_3);
            formatar_maiuscula(novo.palavra_3);
            novo.qtd_palavras ++;
            fflush(stdin);
        }
    }

    printf("\nDigite o nome da editora...: ");
    fgets(novo.editora, TAM_STRING, stdin);
    retirar_enter_string(novo.editora);
    formatar_maiuscula(novo.editora);

    printf("Digite o local da publicacao...: ");
    fgets(novo.local_publicacao, TAM_STRING, stdin);
    retirar_enter_string(novo.local_publicacao);
    formatar_maiuscula(novo.local_publicacao);

    printf("Digite o ano da publicacao...: ");
    scanf("%i", &novo.ano_publicacao);

    printf("Digite a quantidade de paginas...: ");
    scanf("%i", &novo.qtd_paginas); 

    printf("Digite a edicao...: ");
    scanf("%i", &novo.edicao);
    
    printf("Digite a quantidade de exemplares...: ");
    scanf("%i", &novo.qtd_exemplares);
    fflush(stdin);

    novo.id = qtd_livros + 1;

    return novo;

}

struct usuario cadastro_usuario()
{
    fflush(stdin);

    struct usuario novo;
    printf("digite o nome...: ");
    fgets(novo.nome, TAM_STRING, stdin);
    retirar_enter_string(novo.nome);
    formatar_maiuscula(novo.nome);

    printf("Digite o email......: ");
    fgets(novo.email, TAM_STRING, stdin);
    retirar_enter_string(novo.email);
    formatar_maiuscula(novo.email);

    printf("Digite o endereco...: ");
    fgets(novo.endereco, TAM_STRING, stdin);
    retirar_enter_string(novo.endereco);
    formatar_maiuscula(novo.endereco);

    printf("Digite o telefone...: ");
    fgets(novo.telefone, TAM_STRING, stdin);
    retirar_enter_string(novo.telefone);

    novo.cracha = qtd_usuarios + 1;
    printf("\nNumero do cracha: %i", novo.cracha);

    novo.usuario_ativo = true;
    printf("\nUsuario ativo");

    novo.qtd_livros_emprestados = 0;
    novo.pendencia = 0;
    return novo;

}

struct emprestimo novo_emprestimo(int qtd_dias, int pos_livro, int pos_usuario)
{
    struct emprestimo novo;
    struct emprestimo negado;
    int opc;

    negado.qtd_dias = 0;
    negado.id_livro = -1;
    negado.cracha_usuario = -1;
    negado.emp_ativo = false;

    do{
        mostrar_dados_usuario(pos_usuario);
        printf("\nConfirmar usuario: \n");
        printf("1- Sim\n");
        printf("2 - Nao\n");
        printf(":: ");
        scanf("%i", &opc);
        fflush(stdin);
        if(opc == 2)
        {
            printf("Usuario nao confirmado\n");
            return negado;
        }
        if(vetor_usuario[pos_usuario].usuario_ativo == false)
        {
            printf("Usuario INATIVO");
            return negado;
        }
    } while(opc != 1 && opc != 2);

    do{
        mostrar_dados_livro(pos_livro);
        printf("\nConfirmar livro: \n");
        printf("1- Sim\n");
        printf("2 - Nao\n");
        scanf("%i", &opc);
        fflush(stdin);
        if(opc == 2)
        {
            printf("Livro nao confirmado\n");
            return negado;
        }
        if(vetor_livro[pos_livro].qtd_exemplares == 0)
        {
            printf("Livro sem exemplares disponivel.\n");
            return negado;
        }
    } while(opc != 1 && opc != 2);

    novo.id_livro = vetor_livro[pos_livro].id;
    novo.cracha_usuario = vetor_usuario[pos_usuario].cracha;
    novo.qtd_dias = qtd_dias;

    if(vetor_livro[pos_livro].qtd_exemplares == 0)
    {
        printf("Livro sem exemplares disponivel.\n");
        return negado;
    }
    vetor_livro[pos_livro].qtd_exemplares--;
    vetor_usuario[pos_usuario].qtd_livros_emprestados++;
    novo.pos_liv = pos_livro;
    novo.pos_usu = pos_usuario;
    strcpy(novo.titulo, vetor_livro[pos_livro].titulo);
    novo.multa = 0;
    novo.dias_atraso = 0;
    printf("Emprestimo realizado.\n");
    return novo;
}

int localizar_emprestimo()
{
    struct emprestimo localizar;
    int pos_usuario, pos_livro, i;
    pos_usuario = localizar_usuario();
    pos_livro = localizar_livro();
    localizar.cracha_usuario = vetor_usuario[pos_usuario].cracha;
    localizar.id_livro = vetor_livro[pos_livro].id;

    for(i = 0; i < qtd_emprestimos; i++)
    {
        if(vetor_emprestimo[i].cracha_usuario == localizar.cracha_usuario)
        {
            if(vetor_emprestimo[i].id_livro == localizar.id_livro)
            {
                if(vetor_emprestimo[i].emp_ativo == true)
                {
                    printf("Emprestimo localizado.\n");
                    return i;
                }   else
                    {
                        printf("Emprestimo INATIVO.\n");
                        return -1;
                    }
            }
        }
    }
    printf("Emprestimo nao localizado.\n");
    return -1;
}

int localizar_usuario()
{
    int opc, i;
    string nome, email;
    int cracha;
    do{
    //Selecionar usuario
    printf("Localizar usuario por:\n");
    printf("1 - Nome\n");
    printf("2 - email\n");
    printf("3 - Cracha\n");
    scanf("%i", &opc);
    fflush(stdin);

        switch(opc)
        {
            case 1: //nome
                printf("Digite o nome:\n");
                fgets(nome, TAM_STRING, stdin);
                retirar_enter_string(nome);
                formatar_maiuscula(nome);
                fflush(stdin);
                //comparacao
                for(i = 0; i < qtd_usuarios; i++)
                {
                    if(strcmp(nome, vetor_usuario[i].nome) == 0)
                    {
                        printf("Usuario localizado \n");
                        return i;
                        break;
                    }
                }
                printf("Usuario nao localizado.\n");
            case 2: //email
                printf("Digite o email: ");
                fgets(email, TAM_STRING, stdin);
                retirar_enter_string(email);
                formatar_maiuscula(email);
                fflush(stdin);
                //comparacao
                for(i = 0; i < qtd_usuarios; i++)
                {
                    if(strcmp(email, vetor_usuario[i].email) == 0)
                    {
                        printf("Usuario localizado\n");
                        return i;
                        break;
                    }
                }
                printf("Usuario nao localizado.\n");
            case 3: //id
                printf("Digite o cracha: ");
                scanf("%i", &cracha);
                for(i = 0; i < cracha; i++)
                {
                    if(cracha == vetor_usuario[i].cracha)
                    {
                        printf("Usuario localizado: \n");
                        return i;
                        break;
                    }
                }
                printf("Usuario nao localizado.\n");
                
            default: //opcao invalida
                printf("Opcao inválida. Tente novamente!\n");
                break;
        }                
    } while(opc < 0 || opc > 3);
    fflush(stdin);
    return -1;
}

int localizar_livro()
{
    string titulo, palavra_chave, autor;
    int id, opc, i;

    printf("Localizar livro por: \n");
    printf("1 - Titulo\n");
    printf("2 - Id\n");
    printf("3 - Palavra chave\n");
    printf("4 - Autor\n");// fazer
    scanf("%i", &opc);
    fflush(stdin);
    
    if(opc == 1)
    {       
        printf("Digite o titulo: ");
        fgets(titulo, TAM_STRING, stdin);
        retirar_enter_string(titulo);
        formatar_maiuscula(titulo);
        fflush(stdin);

        //comparacao
        for(i = 0; i < qtd_livros; i ++)
        {
            if(strcmp(titulo, vetor_livro[i].titulo) == 0)
            {
            printf("Livro localizado\n");
            return i;
            break;
            }
        }
        printf("Livro nao localizado.\n");
        return -1;
    }
       else if(opc == 2)
        {
            printf("Digite o ID: ");
            scanf("%i", &id);
            //comparacao
            for(i = 0; i < qtd_livros; i++)
            {
                if(id == vetor_livro[i].id)
                {
                    printf("Livro localizado\n");
                    return i;
                }
            }
            printf("Livro nao localizado.\n");
            return -1;
        }
            else if (opc == 3)
            {
                printf("Digite uma palavra chave: ");
                fgets(palavra_chave, TAM_STRING, stdin);
                retirar_enter_string(palavra_chave);
                formatar_maiuscula(palavra_chave);
                //comparacao
                for(i = 0; i < qtd_livros; i++)
                {
                    if(strcmp(palavra_chave, vetor_livro[i].palavra_1) == 0)
                    {
                        printf("Livro localizado\n");
                        return i;
                        if(vetor_livro[i].qtd_palavras > 1)
                        {
                            if(strcmp(palavra_chave, vetor_livro[i].palavra_2) == 0)
                            {
                                printf("Livro localizado.\n");
                                return i;
                            }
                            if(vetor_livro[i].qtd_palavras  == 3)
                            {
                                if(strcmp(palavra_chave, vetor_livro[i].palavra_3) == 0)
                                {
                                    printf("Livro localizado\n");
                                    return i;
                                }
                            }
                        }
                    }
                }
    
            }
                else if(opc == 4)
                {
                printf("Digite o nome: ");
                fgets(autor, TAM_STRING, stdin);
                retirar_enter_string(autor);
                formatar_maiuscula(autor);
                //comparacao
                for(i = 0; i < qtd_livros; i++)
                {
                    if(strcmp(autor, vetor_livro[i].autor_1) == 0)
                    {
                        printf("Livro localizado\n");
                        return i;
                        if(vetor_livro[i].qtd_autores > 1)
                        {
                            if(strcmp(autor, vetor_livro[i].autor_2) == 0)
                            {
                                printf("Livro localizado.\n");
                                return i;
                            }
                            if(vetor_livro[i].qtd_autores  == 3)
                            {
                                if(strcmp(autor, vetor_livro[i].autor_3) == 0)
                                {
                                    printf("Livro localizado\n");
                                    return i;
                                }
                            }
                        }
                    }
                }
                }
    fflush(stdin);
}

void mostrar_dados_usuario(int pos)
{
    printf("\n");
    printf("Nome.................: %s\n", vetor_usuario[pos].nome);
    printf("email................: %s\n", vetor_usuario[pos].email);
    printf("Endereco.............: %s\n", vetor_usuario[pos].endereco);
    printf("Numero de telefone...: %s\n", vetor_usuario[pos].telefone);
    printf("Numero do cracha.....: %i\n", vetor_usuario[pos].cracha);
    if(vetor_usuario[pos].usuario_ativo == true)
    {
        printf("USUARIO ATIVO\n\n\n");
    }
    else{
        printf("USUARIO INATIVO\n\n\n");
    }
}

void mostrar_dados_livro(int pos)
{
    printf("\n");
    printf("Titulo....................: %s\n", vetor_livro[pos].titulo);
    printf("Autor principal...........: %s\n", vetor_livro[pos].autor_1);
    if(vetor_livro[pos].qtd_autores > 1)
    {
        printf("Segundo autor.................: %s\n", vetor_livro[pos].autor_2);
        if(vetor_livro[pos].qtd_autores == 3)
        {
            printf("Terceiro autor................: %s \n", vetor_livro[pos].autor_3);
        }
    }  

    printf("Palavra chave.............: %s\n", vetor_livro[pos].palavra_1); 
    if(vetor_livro[pos].qtd_palavras > 1)
    {
        printf("Segunda palavra...........: %s\n", vetor_livro[pos].palavra_2);
        if(vetor_livro[pos].qtd_palavras == 3)
        {
            printf("Terceiro palavra..........: %s \n", vetor_livro[pos].palavra_3);
        }
    } 
    printf("Edicao....................: %i\n", vetor_livro[pos].edicao);
    printf("Editora...................: %s\n", vetor_livro[pos].editora);
    printf("Local da publicacao.......: %s\n", vetor_livro[pos].local_publicacao);
    printf("Ano de publicacao.........: %i\n", vetor_livro[pos].ano_publicacao);
    printf("Quantidade de paginas.....: %i\n", vetor_livro[pos].qtd_paginas);
    printf("Quantidade de exemplares..: %i\n", vetor_livro[pos].qtd_exemplares);
    printf("Id........................: %i\n\n\n", vetor_livro[pos].id);
}

void mostrar_dados_emprestimo(int pos)
{  
    printf("\n");
    printf("Titulo do livro................: %s\n", vetor_emprestimo[pos].titulo);
    printf("Id do livro....................: %i\n", vetor_emprestimo[pos].id_livro);
    printf("Cracha do usuario..............: %i\n", vetor_emprestimo[pos].cracha_usuario);
    printf("Nome do usuario................: %s\n", vetor_usuario[vetor_emprestimo[pos].pos_usu].nome);
    printf("Quantidade de dias.............: %i\n", vetor_emprestimo[pos].qtd_dias);
    if(vetor_emprestimo[pos].dias_atraso > 0)
    {
        printf("Dias de atraso.................: %i\n", vetor_emprestimo[pos].dias_atraso);
        printf("Valor da multa.................: %i reais\n", vetor_emprestimo[pos].multa);
    }
    if(vetor_emprestimo[pos].emp_ativo == true)
    {
        printf("Emprestimo ativo\n");
    }   else
        {
            printf("Emprestimo nao ativo\n");
            printf("Multa....: %i\n\n", vetor_emprestimo[pos].multa);
        }
}

void mostrar_pendencias()
{
    int i;
    for(i = 0; i < qtd_emprestimos; i++)
    {
        if(vetor_emprestimo[i].multa != 0)
        {
            printf("Usuario.......: %s\n", vetor_usuario[vetor_emprestimo[i].pos_usu].nome);
            printf("Livro.........: %s\n", vetor_livro[vetor_emprestimo[i].pos_liv].titulo);
            printf("Valor multa...: %i\n\n reais", vetor_emprestimo[i].multa);

        }
    }
}

void retirar_enter_string(string str)
{
    int tam;

    tam = strlen(str);
    str[tam - 1] = '\0';
}

void formatar_maiuscula(string str)
{
    int up, tam;

    tam = strlen(str);
    for(up = 0; up < tam; up++)
    {
        str[up] = toupper(str[up]);
    }
}

int devolucao(int pos_emprestimo)
{   
    int retornou, opc, multa_paga = -1;
    pos_emprestimo = localizar_emprestimo();


    fflush(stdin);
    printf("O usuario esta retornando o livro?\n");
    printf("1 - Sim\n");
    printf("2 - Nao\n");
    printf(":: ");
    scanf("%i", &retornou);
    
    
    printf("O usuario entregou dentro do prazo?\n");
    printf("1 - Sim\n");
    printf("2 - Nao\n");
    printf(":: ");
    scanf("%i", &opc);
    

    if(opc == 1)
    {
        if(retornou == 1)
        {
            vetor_emprestimo[pos_emprestimo].emp_ativo = false;
            vetor_usuario[vetor_emprestimo[pos_emprestimo].pos_usu].qtd_livros_emprestados --;
            vetor_livro[vetor_emprestimo[pos_emprestimo].pos_liv].qtd_exemplares++;
            return 1;
        }          
    }
    if(opc == 2)
    {
        printf("Quantos dias de atraso? ");
        scanf("%i", &vetor_emprestimo[pos_emprestimo].dias_atraso);
        getchar();

        vetor_emprestimo[pos_emprestimo].multa = vetor_emprestimo[pos_emprestimo].dias_atraso * 2;

        printf("O valor da multa e: %i reais\n", vetor_emprestimo[pos_emprestimo].multa);

        if(retornou == 1)
        {
                printf("O usuario pagou a multa?\n");
                printf("1 - Sim\n");
                printf("2 - Nao\n");
                printf(":: ");
                scanf("%i", &multa_paga);
// limpar a tela system("cls")
            if(multa_paga == 1)
            {
                if(retornou == 1)
                {
                    vetor_emprestimo[pos_emprestimo].multa = 0;
                    vetor_emprestimo[pos_emprestimo].emp_ativo = false;
                    vetor_usuario[vetor_emprestimo[pos_emprestimo].pos_usu].qtd_livros_emprestados --;
                    vetor_livro[vetor_emprestimo[pos_emprestimo].pos_liv].qtd_exemplares++;
                    return 1;
                }
            }
                else if(multa_paga == 2)
                {
                    vetor_livro[vetor_emprestimo[pos_emprestimo].pos_liv].qtd_exemplares--;
                    printf("Usuario retornou o livro mas nao pagou a multa. Usuario proibido de fazer novos emprestimos ate acerto.\n");
                    vetor_usuario[vetor_emprestimo[pos_emprestimo].pos_usu].usuario_ativo = false;
                    vetor_usuario[vetor_emprestimo[pos_emprestimo].pos_usu].pendencia = vetor_emprestimo[pos_emprestimo].multa;
                    return 1;
                }    
         }
        
    }               
    //}while(opc != 1 || opc != 2);
    return -1;
}

void controle_exemplares(int pos)
{
    int opc, qtd;

    printf("0 - Voltar ao menu\n");
    printf("1 - Aumentar exemplares\n");
    printf("2 - Diminuir exemplares\n");
    printf("3 - Retirar todos os exemplares\n");
    scanf("%i", &opc);
    fflush(stdin);

    switch (opc)
    {
        case 0: //voltar ao menu
            printf("Voltando ao menu...\n\n");
            break;

        case 1: // aumentar exemplares
            printf("Digite a quantidade de exemplares a serem adicionados: ");
            scanf("%i", &qtd);

            vetor_livro[pos].qtd_exemplares += qtd;

            break;

        case 2: // diminuir exemplares
            printf("Digite a quantidade de exemplares a serem retirados: ");
            scanf("%i", &qtd);

            vetor_livro[pos].qtd_exemplares -= qtd;

            break;

        case 3: //zerar exemplares
            printf("Zerando a quantidade de exemplares...\n");

            vetor_livro[pos].qtd_exemplares = 0;

            break;
        
        default: //voltar ao menu
            printf("Opcao invalida. Voltando ao menu...\n\n");
            break;
    }
}
//Relatorios
void relatorio_acervo()
{
    int i;

    for(i = 0; i < qtd_livros; i++)
    {
        mostrar_dados_livro(i);
    }
}

void relatorio_usuario()
{
    int i;
    
    for(i = 0; i < qtd_usuarios; i++)
    {
        mostrar_dados_usuario(i);
    }
}

void relatorio_emprestimos()
{
    int i;
    
    for(i = 0; i < qtd_emprestimos; i++)
    {
        if(vetor_emprestimo[i].id_livro == -1)
        {
            break;
        }
        mostrar_dados_emprestimo(i);
    }
}

void relatorio_faixa_ano()
{
    int ano_inicial, ano_final, i;

    fflush(stdin);
    printf("Digite o ano inicial: ");
    scanf("%i", &ano_inicial);

    printf("Digite o ano_final: ");
    scanf("%i", &ano_final);

    for(i = 0; i < qtd_livros; i++)
    {
        if(vetor_livro[i].ano_publicacao >= ano_inicial && vetor_livro[i].ano_publicacao <= ano_final)
        {
            mostrar_dados_livro(i);
        }
    }
    fflush(stdin);
}