#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <locale.h>
#include <time.h>
#include <cstring>
#include <string> 


using namespace std;


FILE *ArqProduto; // arquivo


typedef struct data {
    int dia, mes, ano;
    int hora, min, seg;
} DATA;


typedef struct Produto {
    long int Codigo;
    char DescricaoProd[50];
    int Qtd;
    DATA DataCadastro;        // data e hora do cadastro
    float PrecoCusto, PrecoVenda;
    int ID_Fornecedor; 
} RegProduto;
// Variavel Registro
RegProduto Prod;


FILE *ArqFornecedores;
typedef struct Fornecedores {
    long int id;
    char nome_empresa[50];
    char cnpj[20];          
    char telefone[20];      
    char email[30];
    char nome_contato[50];
    char cidade[40];
    char estado[4];              
} RegFornecedor;


RegFornecedor Forne;


FILE *ArqItens;
typedef struct Item {
    long int NumeroNota;
    long int CodigoItem; // CodigoItem tem relacao com Codigo (Produto)
    char DescricaoItem[50]; // DescricaoProd tem relacao com DescricaoItem
    int QtdVendida;
    float PrecoUnitario; // PrecoUnitario tem relacao com PrecoVenda
} RegItem;




FILE *ArqVendas; 
typedef struct Venda {
    long int NumeroNota;
    char clienteNome[50];
    float totalNota;
    DATA DataVenda; 
    char formaPagamento[50];
    int totalItens;
} RegVenda;


// Protótipos Produtos
void MenuPrincipal();
void MenuProdutosBar();
void pausa();
void CadastrarProd();
DATA gerarData();
void imprimirData(DATA D);
long int ultimoCod();
void RelatorioProd();
void BuscaProduto();
long int ConsultaProduto(int cod);
void MenuEdicaoProd();
void EditarProduto(int pos);
void MenuExclusaoProd();
long int ExcluirProduto(int cod);
void ArquivoProd();


// Protótipos Vendas / Itens
void MenuVendas();
void AdicionarVenda();
long int ultimaNota(); 
void BaixaEstoque(long int pos, int qtd);
void RelatorioVenda();
void PesquisaVenda();
long int ConsultaVenda(int cod);
long int ConsultaItem(int cod);
void MenuEdicaoVenda();
void EditarVenda(int pos);
void EditarItem(long int numNota, long int posVenda);
void MenuExclusaoVenda();
long int ExcluirVenda(int numNota);
long int ExcluirItem(int numNota, int codItem);


// Protótipos Fornecedores
void MenuFornecedores();
void AdicionarForne();
long int ultimoForne();
void RelatorioForne();
void BuscaForne();
long int ConsultaForne(int id);
void MenuEdicaoForne();
void EditarForne(int pos);
void MenuExclusaoForne();
long int ExcluirForne(int id);


int main(int argc, char** argv) {
    setlocale(LC_ALL, "Portuguese");
    MenuPrincipal();
    return 0;
}


void MenuPrincipal() {
    int opcao;
    do {
        system("cls");
        cout << "\n ================    SISTEMA DE CONTROLE DE BAR    ===============";
        cout << "\n =================================================================";
        cout << "\n 1 - Menu de Produtos";
        cout << "\n 2 - Menu de Vendas";
        cout << "\n 3 - Menu de Fornecedores";
        cout << "\n 0 - Encerrar Programa";
        cout << "\n\n - Escolha a opção: ";
        cin >> opcao;
        switch(opcao) {
            case 1: MenuProdutosBar();
                    break;
            case 2: MenuVendas();
                    break;
            case 3: MenuFornecedores();
                    break;
            default: cout << "\n Tecle <Enter> para sair do programa "; system("pause>>null");
                    break;
        }
    } while(opcao != 0);
}


void MenuProdutosBar() {
    int opcao;
    do {
        system("cls");
        cout << "\n ================ MENU DE PRODUTOS ================";
        cout << "\n\n ";
        cout << "\n 1 - Cadastrar Produto";
        cout << "\n 2 - Relatório dos Produtos";
        cout << "\n 3 - Pesquisar Produto";
        cout << "\n 4 - Editar Produto";
        cout << "\n 5 - Excluir Produto";
        cout << "\n 0 - Retornar ao Menu Principal";
        cout << "\n\n - Escolha a opção: ";
        cin >> opcao;
        switch(opcao) {
            case 1: CadastrarProd();
                    break;
            case 2: RelatorioProd();
                    pausa();
                    break;
            case 3: system("cls");
                    BuscaProduto();
                    pausa();
                    break;  
            case 4: MenuEdicaoProd();
                    break;
            case 5: MenuExclusaoProd();
                    break;
            default: cout << "\n Tecle <Enter> para retornar ao menu principal "; system("pause>>null");
                    break;
        }
    } while(opcao != 0);
}


void pausa() {
    cout << "\n Tecle <Enter> para voltar ao Menu de Opções";
    system("pause>>null");
}


long int ultimoCod() {
    long int cod;
    RegProduto aux;                             // local: não mexe na global Prod
    FILE *f = fopen("Produtos.dat", "rb");      // local: não mexe na global ArqProduto


    if(f == NULL) return 1;


    if(fread(&aux, sizeof(aux), 1, f) == 0) {   // arquivo vazio: 1º código
        cod = 1;
    } else {
        fseek(f, -(long)sizeof(aux), SEEK_END); // vai ao início do último registro
        fread(&aux, sizeof(aux), 1, f);         // lê o último registro
        cod = aux.Codigo + 1;
    }
    
    fclose(f);                                  // fecha o arquivo
    return cod;
}


void CadastrarProd() {
    char opcao, opcaoC;
    int consultaID;
    system("cls");
    // Cria um arquivo para .dat para armazenar os produtos
    ArqProduto = fopen("Produtos.dat", "ab");
    if(ArqProduto == NULL) {
        cout << "Erro na abertura do arquivo \n";
        system("pause");
        exit(1);
    }
   
    do {
        Prod.Codigo = ultimoCod();
        cout << "\nCódigo:  " << Prod.Codigo;
        cin.ignore(80, '\n');
        cout << "\nDescrição:  ";
        cin.getline(Prod.DescricaoProd, sizeof(Prod.DescricaoProd));
        cout << "\nQuantidade Estoque (por unidade): ";
        cin >> Prod.Qtd;
        while(Prod.Qtd < 0) {
            cout << "\nQuantidade Inválida! Digite novamente: ";
            cin >> Prod.Qtd;
        }
       
        cout << "\nPreço de Custo (R$ 0.00): ";
        cin >> Prod.PrecoCusto;
        while(Prod.PrecoCusto < 0) {
            cout << "\nPreço de Custo Inválida! Digite novamente: ";
            cin >> Prod.PrecoCusto;
        }
        cout << "\nPreço de Venda (R$ 0.00): ";
        cin >> Prod.PrecoVenda;
        while(Prod.PrecoVenda < 0) {
            cout << "\nPreço de Venda Inválida! Digite novamente: ";
            cin >> Prod.PrecoVenda;
        }
        
        cout << "\nID do Fornecedor: ";
        cin >> Prod.ID_Fornecedor;
        consultaID = ConsultaForne(Prod.ID_Fornecedor);
        if(consultaID == -1) {
            cout << "\n Fornecedor não cadastrado";
            cout << "\n Deseja Cadastrar um Fornecedor (S/N): ";
            cin >> opcaoC;
			if(toupper(opcaoC) == 'S') {
				AdicionarForne();
				Prod.ID_Fornecedor = Forne.id;
			} else {
	            Prod.ID_Fornecedor = 0;
			}
        } else {
            ArqFornecedores = fopen("Fornecedores.dat", "rb");
            fseek(ArqFornecedores, consultaID, SEEK_SET);
            fread(&Forne, sizeof(Forne), 1, ArqFornecedores);
            fclose(ArqFornecedores);
            
            cout << "\n ============ FORNECEDOR ============";
            cout << "\n ID       : " << Forne.id;
            cout << "\n Empresa  : " << Forne.nome_empresa;
            cout << "\n CNPJ     : " << Forne.cnpj;
            cout << "\n Vendedor : " << Forne.nome_contato;
            cout << "\n Telefone : " << Forne.telefone;
            cout << "\n ====================================\n";
            
            Prod.ID_Fornecedor = Forne.id;
        }
        cout << "\n ID Fornecedor: " << Prod.ID_Fornecedor;
       
        Prod.DataCadastro = gerarData();
        cout << "\n Data e Hora do cadastro: ";
        imprimirData(Prod.DataCadastro);
        cout << endl;
       
        fwrite(&Prod, sizeof(Prod), 1, ArqProduto); // Gravando no arquivo
        fflush(ArqProduto);   // grava o buffer no disco agora, para o ultimoCod() ver esse registro
        cout << "\n Adicionar outro produto (S/N)? ";
        cin >> opcao;
    } while(toupper(opcao) == 'S');
    fclose(ArqProduto); // Fechando Arquivo
}


DATA gerarData() {
    DATA D;
    time_t mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    D.dia  = tm.tm_mday;
    D.mes  = tm.tm_mon + 1;
    D.ano  = tm.tm_year + 1900;
    D.hora = tm.tm_hour;
    D.min  = tm.tm_min;
    D.seg  = tm.tm_sec;
    return D;
}


// Imprime no formato dd/mm/aaaa hh:mm:ss (19 caracteres)
void imprimirData(DATA D) {
    if(D.dia < 10)
        cout << "0" << D.dia << "/";
    else
        cout << D.dia << "/";
    if(D.mes < 10)
        cout << "0" << D.mes << "/";
    else
        cout << D.mes << "/";
    cout << D.ano << " ";
    if(D.hora < 10)
        cout << "0" << D.hora << ":";
    else
        cout << D.hora << ":";
    if(D.min < 10)
        cout << "0" << D.min << ":";
    else
        cout << D.min << ":";
    if(D.seg < 10)
        cout << "0" << D.seg;
    else
        cout << D.seg;
}  


void RelatorioProd() {
    system("cls");
    ArqProduto = fopen("Produtos.dat", "rb"); // open read binary
    if(ArqProduto == NULL) {
        cout << "Nenhum produto cadastrado ainda \n";
        pausa();
        return;
    }
    cout << "\n                                *** PRODUTOS CADASTRADOS ***";
    cout << "\n -----------------------------------------------------------------------------------------------------------------------\n";
    cout << left  << setw(10) << "Código"
         << left  << setw(30) << "Descrição"
         << right << setw(20) << "Quantidade Estoque"
         << right << setw(16) << "Preço Custo"
         << right << setw(16) << "Preço Venda"
         << right << setw(10) << "Fornecedor"
         << right << setw(24) << "Data e Hora";
    cout << "\n -----------------------------------------------------------------------------------------------------------------------";
   
    while(fread(&Prod, sizeof(Prod), 1, ArqProduto) == 1) {
        cout << "\n";
        cout << left  << setw(10) << Prod.Codigo;
        cout << left  << setw(30) << Prod.DescricaoProd;
        cout << right << setw(20) << Prod.Qtd;
        cout << right << setw(16) << fixed << setprecision(2) << Prod.PrecoCusto;
        cout << right << setw(16) << fixed << setprecision(2) << Prod.PrecoVenda;
        cout << right << setw(10) << Prod.ID_Fornecedor;
        
        // Espaçamento do campo da data
        cout << setw(5) << " ";                    // 2 espaços + 19 da data = 21 (alinha com o cabeçalho)
        imprimirData(Prod.DataCadastro);
    }
    cout << endl;
       
    fclose(ArqProduto);
}


void BuscaProduto() {
    int buscaCod, result;
    
    cout << "\n *** CONSULTAR PRODUTO ***";
    cout << "\n ---------------------------------------\n";
    cout << "\n Digite o código do produto: ";
    cin >> buscaCod;
    result = ConsultaProduto(buscaCod);
    if(result == -1) {
        cout << "\n Produto não cadastrado";
    } else {
        ArqProduto = fopen("Produtos.dat", "rb");
        fseek(ArqProduto, result, SEEK_SET);
        fread(&Prod, sizeof(Prod), 1, ArqProduto);
        fclose(ArqProduto);
        cout << "\n --------------------------------------";
        cout << "\n  *** INFORMAÇÕES DO PRODUTO ***";
        cout << "\n --------------------------------------";
        cout << "\n  Código          : " << Prod.Codigo;
        cout << "\n  Descrição       : " << Prod.DescricaoProd;
        cout << "\n  Qtd. em Estoque : " << Prod.Qtd;
        cout << "\n  Preço de Custo  : " << Prod.PrecoCusto;
        cout << "\n  Preço de Venda  : " << Prod.PrecoVenda;
        cout << "\n  ID Fornecedor   : " << Prod.ID_Fornecedor;
        cout << "\n  Data  e  Horas  : "; imprimirData(Prod.DataCadastro);
        cout << "\n --------------------------------------";
    }
}


long int ConsultaProduto(int cod) {
    system("cls");
    ArqProduto = fopen("Produtos.dat", "rb");
    if(ArqProduto == NULL) {
        cout << "Erro na abertura do arquivo \n";
        pausa();
        exit(1);
    }      
    while(fread(&Prod, sizeof(Prod), 1, ArqProduto) == 1) {
        if(Prod.Codigo == cod) {
            long int posicao = ftell(ArqProduto) - (long)sizeof(Prod); // posição do registro
            fclose(ArqProduto);
            return posicao;
        }
    }
    fclose(ArqProduto);
    return -1;
}


void MenuEdicaoProd() {
    int cod, pos;
    int opcaoM;
    system("cls");
    do {
        cout << "\n -----------------------------------------\n";
        cout << "           *** MENU DE MODIFICAÇÃO ***";
        cout << "\n -----------------------------------------\n";
        cout << "\n1) Ver Relatório de Produtos Cadastrados; ";
        cout << "\n0) Para retornar ao Menu de Produtos;       ";
        cout << "\n- Digite sua escolha: ";
        cin >> opcaoM;
        while((opcaoM < 0) || (opcaoM > 1)) {
            cout << "\n Opção Inválida! Digite novamente: ";
            cin >> opcaoM;
        }
        if(opcaoM == 1) {
            RelatorioProd();
            cout << "\n Se quiser retorna a página anterior (Digite = 0) ";
            cout << "\n- Código do Produto que será modificado: ";
            cin >> cod;
            if(cod == 0) {
                pausa();
                system("cls");
            } else {
                pos = ConsultaProduto(cod);
                if(pos == -1) {
                    cout << "\n Produto não existe";
                    pausa();
                } else {
                    system("cls");
                    ArqProduto = fopen("Produtos.dat", "rb");
                    fseek(ArqProduto, pos, SEEK_SET);
                    fread(&Prod, sizeof(Prod), 1, ArqProduto);
                    fclose(ArqProduto);
                    cout << "\n --------------------------------------";
                    cout << "\n  *** INFORMAÇÕES DO PRODUTO ***";
                    cout << "\n --------------------------------------";
                    cout << "\n  Código          : " << Prod.Codigo;
                    cout << "\n  Descrição       : " << Prod.DescricaoProd;
                    cout << "\n  Data  e  Horas  : "; imprimirData(Prod.DataCadastro);
                    cout << "\n  Qtd. em Estoque : " << Prod.Qtd;
                    cout << "\n  Preço de Custo  : " << Prod.PrecoCusto;
                    cout << "\n  Preço de Venda  : " << Prod.PrecoVenda;
                    cout << "\n  ID Fornecedor   : " << Prod.ID_Fornecedor;
                    cout << "\n --------------------------------------";
                    EditarProduto(pos);
                }  
            }
        }
    } while(opcaoM != 0);
    pausa();
}


void EditarProduto(int pos) {
    char escolha;
    int consultaID, opcaoC;
    ArqProduto = fopen("Produtos.dat", "r+b");
    fseek(ArqProduto, pos, SEEK_SET);
    fread(&Prod, sizeof(Prod), 1, ArqProduto);
   
    cout << "\n *** MODIFICAR PRODUTO *** ";
    cout << "\n- Modificar Descrição (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n- Nova Descrição: ";
        cin.ignore(80, '\n');
        cin.getline(Prod.DescricaoProd, sizeof(Prod.DescricaoProd));
    }
    cout << "\n- Modificar a Quantidade em Estoque (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n- Nova Quantidade em Estoque (por unidade): ";
        cin >> Prod.Qtd;
        while(Prod.Qtd < 0) {
            cout << "\nQuantidade Inválida! Digite novamente: ";
            cin >> Prod.Qtd;
        }
    }
   
    cout << "\n- Modificar Preço de Custo (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n- Novo Preço de Custo (R$ 0.00): ";
        cin >> Prod.PrecoCusto;
        while(Prod.PrecoCusto < 0) {
            cout << "\n Preço de Custo Inválido! Digite novamente: ";
            cin >> Prod.PrecoCusto;
        }
    }
   
    cout << "\n- Modificar Preço de Venda (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n- Novo Preço de Venda (R$ 0.00): ";
        cin >> Prod.PrecoVenda;
        while(Prod.PrecoVenda < 0) {
            cout << "\n Preço de Venda Inválido! Digite novamente: ";
            cin >> Prod.PrecoVenda;
        }
    }
    
    cout << "\n- Modificar ID do Fornecedor (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n Novo ID do Fornecedor: ";
        cin >> Prod.ID_Fornecedor;
        consultaID = ConsultaForne(Prod.ID_Fornecedor);
        if(consultaID == -1) {
            cout << "\n Fornecedor não cadastrado";
            cout << "\n Deseja Cadastrar um Fornecedor (S/N): ";
            cin >> opcaoC;
			if(toupper(opcaoC) == 'S') {
				AdicionarForne();
				Prod.ID_Fornecedor = Forne.id;
			} else {
	            Prod.ID_Fornecedor = 0;
			}
        } else {
            ArqFornecedores = fopen("Fornecedores.dat", "rb");
            fseek(ArqFornecedores, consultaID, SEEK_SET);
            fread(&Forne, sizeof(Forne), 1, ArqFornecedores);
            fclose(ArqFornecedores);
            
            cout << "\n ============ FORNECEDOR ============";
            cout << "\n ID       : " << Forne.id;
            cout << "\n Empresa  : " << Forne.nome_empresa;
            cout << "\n CNPJ     : " << Forne.cnpj;
            cout << "\n Vendedor : " << Forne.nome_contato;
            cout << "\n Telefone : " << Forne.telefone;
            cout << "\n ====================================\n";
            
            Prod.ID_Fornecedor = Forne.id;
        }
        cout << "\n ID Fornecedor: " << Prod.ID_Fornecedor;
    }
    
    fseek(ArqProduto, pos, SEEK_SET); // volta ao início do registro
    fwrite(&Prod, sizeof(Prod), 1, ArqProduto);
    fclose(ArqProduto);
    cout << "\n Produto Alterado com sucesso";
    pausa();
    system("cls");
}


void MenuExclusaoProd() {
    system("cls");
    int opcaoE, posicao, cod;
    char excluir;
    do {
        cout << "\n -----------------------------------------\n";
        cout << "           *** MENU DE EXCLUSÃO ***";
        cout << "\n -----------------------------------------\n";
        cout << "\n1) Ver Relatório de Produtos Cadastrados; ";
        cout << "\n0) Para retornar ao Menu Principal;       ";
        cout << "\n- Digite sua escolha: ";
        cin >> opcaoE;
        while((opcaoE < 0) || (opcaoE > 1)) {
            cout << "\nEscolha inválida! Digite Novamente";
            cin >> opcaoE;
        }
        if(opcaoE == 1) {
            RelatorioProd();
            cout << "\n- Pesquise qual Item quer excluir: ";
            cin >> cod;
            posicao = ConsultaProduto(cod);
            if (posicao == -1) {
                cout << "\n Item não cadastrado";
                pausa();
            } else {
                system("cls");
                ArqProduto = fopen("Produtos.dat", "rb");
                fseek(ArqProduto, posicao, SEEK_SET);
                fread(&Prod, sizeof(Prod), 1, ArqProduto);
                fclose(ArqProduto);
                cout << "\n --------------------------------------";
                cout << "\n  *** INFORMAÇÕES DO PRODUTO ***";
                cout << "\n --------------------------------------";
                cout << "\n  Código          : " << Prod.Codigo;
                cout << "\n  Descrição       : " << Prod.DescricaoProd;
                cout << "\n  Qtd. em Estoque : " << Prod.Qtd;
                cout << "\n  Preço de Custo  : " << Prod.PrecoCusto;
                cout << "\n  Preço de Venda  : " << Prod.PrecoVenda;
                cout << "\n  ID Fornecedor   : " << Prod.ID_Fornecedor;
                cout << "\n  Data  e  Horas  : "; imprimirData(Prod.DataCadastro);
                cout << "\n --------------------------------------";
                cout << "\n Excluir (S/N)?";
                cin >> excluir;
                if(toupper(excluir) == 'S') {
                    ExcluirProduto(cod);
                    cout << "Exclusão feita com sucesso";
                } else {
                    cout << "Exclusão cancelada";
                }
                pausa();
                system("cls");
            }
        }
    } while(opcaoE != 0);
    pausa();
}


long int ExcluirProduto(int cod) {
    system("cls");
    long int pos = -1; // Posicao do registro excluído
    FILE *ArqTemp;
   
    ArqProduto = fopen("Produtos.dat", "rb");
    if(ArqProduto == NULL) {
        cout << "Erro na abertura do arquivo \n";
        pausa();
        exit(1);
    }
           
    ArqTemp = fopen("Temp.dat", "wb");
    if(ArqTemp == NULL) {
        cout << "Erro ao criar arquivo temporario \n";
        fclose(ArqProduto);
        pausa();
        exit(1);
    }
   
    while(fread(&Prod, sizeof(Prod), 1, ArqProduto) == 1) {
        if(Prod.Codigo == cod) {
            pos = ftell(ArqProduto) - (long)sizeof(Prod); // guarda posicao, nao copia o registro
        } else {
            fwrite(&Prod, sizeof(Prod), 1, ArqTemp); // copia os demais
        }
    }
    fclose(ArqProduto);
    fclose(ArqTemp);
   
    if(pos != -1) {                         // ArqTemp se torna o ArqProduto
        remove("Produtos.dat");            
        rename("Temp.dat", "Produtos.dat");
    } else {
        remove("Temp.dat");
    }
    return pos;
}


void MenuVendas() {
    int opcao;
    do {
        system("cls");
        cout << "\n ================ MENU DE VENDAS ================";
        cout << "\n\n ";
        cout << "\n 1 - Adicionar Venda";
        cout << "\n 2 - Relatório das Vendas";
        cout << "\n 3 - Pesquisar Venda Feita";
        cout << "\n 4 - Editar Venda";
        cout << "\n 5 - Excluir Venda";
        cout << "\n 0 - Retornar ao Menu Principal";
        cout << "\n\n - Escolha a opção: ";
        cin >> opcao;
        switch(opcao) {
            case 1: system("cls");
                    AdicionarVenda();
                    break;
            case 2: RelatorioVenda();
                    pausa();
                    break;
            case 3: system("cls");
                    PesquisaVenda();
                    pausa();
                    break;  
            case 4: MenuEdicaoVenda();
                    break;
            case 5: MenuExclusaoVenda();
                    break;
            default: cout << "\n Tecle <Enter> para retornar ao menu principal "; system("pause>>null");
                    break;
        }
    } while(opcao != 0);
}


void AdicionarVenda() {
    char opcao;
    RegVenda V;
    RegItem Item;
    long int pos;         
    int qtd;
    
    V.totalItens = 0;
    V.totalNota = 0;
    
    cout << "\n================== ADICIONAR VENDA ==================\n";
        
    V.NumeroNota = ultimaNota();
    cout << "\n Nº da Nota: " << V.NumeroNota;
    
    cin.ignore(80, '\n');
    cout << "\n Nome do Cliente (ou Mesa): ";
    cin.getline(V.clienteNome, sizeof(V.clienteNome));
    cout << "\n Forma de Pagamento: ";
    cin.getline(V.formaPagamento, sizeof(V.formaPagamento));
    V.DataVenda = gerarData();
    
    do {
        ArqItens = fopen("Itens.dat", "ab");
        if(ArqItens == NULL) {
            cout << "\nErro na Abertura do Arquivo - Itens.dat";
            pausa();
            exit(1);
        }
        
        cout << "\n --- Adicionar Item: " << V.totalItens + 1 << "º \n";
        cout << "\n Código do Produto: ";
        cin >> Item.CodigoItem;        
        pos = ConsultaProduto(Item.CodigoItem);
        
        if(pos == -1) {
            cout << "\n Produto não encontrado! Cadastre-o antes de adicionar";
            pausa();
        } else {
            ArqProduto = fopen("Produtos.dat", "rb");
            fseek(ArqProduto, pos, SEEK_SET);
            fread(&Prod, sizeof(Prod), 1, ArqProduto);
            fclose(ArqProduto);
            
            cout << "\n ============ ITEM ============";
            cout << "\n Descrição do Item: " << Prod.DescricaoProd;
            cout << "\n Preço Unitário: " << Prod.PrecoVenda;
            cout << "\n Quantidade em Estoque: " << Prod.Qtd;
            cout << "\n ==============================";
            
            if(Prod.Qtd == 0) {
                cout << "\n Produto sem estoque";
                pausa();
            } else {
                cout << "\n Quantidade Vendida (por unidade): ";
                cin >> qtd;
                while(qtd <= 0 || qtd > Prod.Qtd) {
                    cout << "\n Inválido! Digite novamente: ";
                    cin >> qtd;
                }
                
                Item.NumeroNota = V.NumeroNota;
                Item.CodigoItem = Prod.Codigo;
                strcpy(Item.DescricaoItem, Prod.DescricaoProd);
                Item.PrecoUnitario = Prod.PrecoVenda;
                Item.QtdVendida = qtd;
                
                ArqItens = fopen("Itens.dat", "ab");
                if(ArqItens == NULL) {
                    cout << "\n Erro na Abertura do Arquivo - Itens.dat";
                    pausa();
                    exit(1);
                }
                fwrite(&Item, sizeof(Item), 1, ArqItens);
                fflush(ArqItens);
                
                BaixaEstoque(pos, qtd);
                
                V.totalNota += qtd * Prod.PrecoVenda;
                V.totalItens++;
            }    
        }
            
        cout << "\n Deseja adicionar mais um item a venda (S/N): ";
        cin >> opcao;
    } while(toupper(opcao) == 'S');
    
    fclose(ArqItens);    
    
    ArqVendas = fopen("Vendas.dat", "ab");
    if(ArqVendas == NULL) {
        cout << "\nErro na Abertura do Arquivo";
        pausa();
        exit(1);
    }
    fwrite(&V, sizeof(V), 1, ArqVendas);
    fclose(ArqVendas);
    
    cout << "\nTotal da nota: R$ " << V.totalNota;
    cout << "\nVenda registrada em ";
    imprimirData(V.DataVenda);
}


long int ultimaNota() {
    long int cod;
    RegVenda aux;
    
    FILE *f = fopen("Vendas.dat", "rb");
    
    if(f == NULL) return 1;
    if(fread(&aux, sizeof(aux), 1, f) == 0) {
        cod = 1;
    } else {
        fseek(f, -(long)sizeof(aux), SEEK_END);
        fread(&aux, sizeof(aux), 1, f);
        cod = aux.NumeroNota + 1;
    }
    fclose(f);
    return cod;
}


void BaixaEstoque(long int pos, int qtd) {
    RegProduto aux;
    FILE *f = fopen("Produtos.dat", "r+b");
    fseek(f, pos, SEEK_SET);
    fread(&aux, sizeof(aux), 1, f);
    
    aux.Qtd -= qtd;
    
    fseek(f, pos, SEEK_SET);
    fwrite(&aux, sizeof(aux), 1, f);
    fclose(f);
}


void RelatorioVenda() {
    system("cls");
    RegVenda V;
    RegItem itens;
    ArqVendas = fopen("Vendas.dat", "rb");
    if(ArqVendas == NULL) {
        cout << "\nNenhuma venda foi cadastrada!";
        pausa();
        return;
    }
    
    ArqItens = fopen("Itens.dat", "rb");
    if(ArqItens == NULL) {
        cout << "\nNenhum item foi cadastrada!";
        pausa();
        return;
    }
    
    cout << fixed << setprecision(2);
    string linha(89, '-');


    cout << "\n                                  *** VENDAS FEITAS ***\n";
    cout << " " << linha << "\n ";
    cout << left  << setw(8)  << "Nº Nota"
         << left  << setw(22) << "Cliente (ou Mesa)"
         << right << setw(12) << "Valor Total" << "  "
         << left  << setw(18) << "Pagamento"
         << right << setw(6)  << "Itens" << "  "
         << left  << "Data e Hora";
    cout << "\n " << linha;
    
    while(fread(&V, sizeof(V), 1, ArqVendas) == 1) {
        // linha da venda
        cout << "\n ";
        cout << left  << setw(8)  << V.NumeroNota
             << left  << setw(22) << string(V.clienteNome).substr(0, 21)
             << right << setw(12) << V.totalNota << "  "
             << left  << setw(18) << string(V.formaPagamento).substr(0, 17)
             << right << setw(6)  << V.totalItens << "  ";
        imprimirData(V.DataVenda);


        // cabeçalho dos itens (uma vez por venda)
        cout << "\n ";
        cout << "\n     " << left << setw(8) << "Código"
             << left  << setw(30) << "Descrição"
             << right << setw(6)  << "Qtd"
             << right << setw(14) << "Preço Unit.";


        // itens somente desta nota
        rewind(ArqItens);
        while(fread(&itens, sizeof(itens), 1, ArqItens) == 1) {
            if(itens.NumeroNota == V.NumeroNota) {
                cout << "\n     ";
                cout << left  << setw(8)  << itens.CodigoItem
                     << left  << setw(30) << string(itens.DescricaoItem).substr(0, 29)
                     << right << setw(6)  << itens.QtdVendida
                     << right << setw(14) << itens.PrecoUnitario;
            }
        }
        cout << "\n " << linha;
    }
    cout << endl;


    fclose(ArqVendas);
    fclose(ArqItens);
    
}




long int ConsultaVenda(int cod) {
    system("cls");
    ArqVendas = fopen("Vendas.dat", "rb");
    RegVenda aux;
    if(ArqVendas == NULL) {
        cout << "Erro na abertura do arquivo \n";
        pausa();
        exit(1);
    } 
    while(fread(&aux, sizeof(aux), 1, ArqVendas) == 1) {
        if(aux.NumeroNota == cod) {
            long int pos = ftell(ArqVendas) - (long)sizeof(aux);
            fclose(ArqVendas);
            return pos;
        }
    } 
    fclose(ArqVendas);
    return -1;
}




long int ConsultaItem(int cod) {
    system("cls");
    ArqItens = fopen("Itens.dat", "rb");
    RegItem aux;
    if(ArqItens == NULL) {
        cout << "Erro na abertura do arquivo \n";
        pausa();
        exit(1);
    } 
    while(fread(&aux, sizeof(aux), 1, ArqItens) == 1) {
        if(aux.CodigoItem == cod) {
            long int pos = ftell(ArqItens) - (long)sizeof(aux);
            fclose(ArqItens);
            return pos;
        }
    } 
    fclose(ArqItens);
    return -1;
}




void PesquisaVenda() {
    int buscaCod, result;
    RegVenda V;
    RegItem itens;
    
    cout << "\n*** PESQUISAR VENDA ***";
    cout << "\n-----------------------\n";
    cout << "\n Digite o Nº Nota: ";
    cin >> buscaCod;
    result = ConsultaVenda(buscaCod);
    if(result == -1) {
        cout << "\n Venda não cadastrada";
    } else {
        ArqVendas = fopen("Vendas.dat", "rb");
        fseek(ArqVendas, result, SEEK_SET);
        fread(&V, sizeof(V), 1, ArqVendas);
        fclose(ArqVendas);
        
        cout << "\n --------------------------------------";
        cout << "\n  *** INFORMAÇÕES DA VENDA ***";
        cout << "\n --------------------------------------";
        cout << "\n  Nº Nota : " << V.NumeroNota;
        cout << "\n  Nome do Cliente (ou Mesa) : " << V.clienteNome;
        cout << "\n  Qtd. Itens Vendidos : " << V.totalItens;
        cout << "\n  Preço Total da Nota : " << fixed << setprecision(2) << V.totalNota;
        cout << "\n  Forma de Pagamento : " << V.formaPagamento;
        cout << "\n  Data  e  Horas : "; imprimirData(V.DataVenda);
        cout << "\n --------------------------------------\n";
        
        cout << "\n --------------------------------------";
        cout << "\n  *** INFORMAÇÕES DOS ITENS ***";
        cout << "\n --------------------------------------";
        
        ArqItens = fopen("Itens.dat", "rb");
        fseek(ArqItens, result, SEEK_SET);
        fread(&itens, sizeof(itens), 1, ArqItens);


        rewind(ArqItens);
        while(fread(&itens, sizeof(itens), 1, ArqItens) == 1) {
            if(itens.NumeroNota == V.NumeroNota) {
                cout << "\n  Código do Produto : " << itens.CodigoItem;
                cout << "\n  Descrição : " << itens.DescricaoItem;
                cout << "\n  Preço Unitário : " << fixed << setprecision(2) << itens.PrecoUnitario;
                cout << "\n  Qtd. Vendida : " << itens.QtdVendida;
                cout << "\n --------------------------------------";
            }
        }
        fclose(ArqItens);
    }
}




void MenuEdicaoVenda() {
    int Nnota, pos;
    int opcaoM;
    RegVenda V;
    RegItem itens;
    int escolha;
    
    system("cls");
    do {
        cout << "\n ------------------------------------------------\n";
        cout << "           *** MENU DE EDIÇÃO (VENDA) ***";
        cout << "\n ------------------------------------------------\n";
        cout << "\n1) Ver Relatório de Vendas Cadastrados; ";
        cout << "\n0) Para retornar ao Menu de Vendas;     ";
        cout << "\n- Digite sua escolha: ";
        cin >> opcaoM;
        while((opcaoM < 0) || (opcaoM > 1)) {
            cout << "\n Opção Inválida! Digite novamente: ";
            cin >> opcaoM;
        }
        if(opcaoM == 1) {
            RelatorioVenda();
            cout << "\n *-------------------------------* \n";
            cout << "\n Se quiser retornar, digite = 0 ";
            cout << "\n- Venda que será modificada (Nº Nota): ";
            cin >> Nnota;
            if(Nnota == 0) {
                pausa();
                system("cls");
            } else {
                pos = ConsultaVenda(Nnota);
                if(pos == -1) {
                    cout << "\n Venda não existe";
                    pausa();
                } else {
                    system("cls");
                    ArqVendas = fopen("Vendas.dat", "rb");
                    fseek(ArqVendas, pos, SEEK_SET);
                    fread(&V, sizeof(V), 1, ArqVendas);
                    fclose(ArqVendas);
                    cout << "\n --------------------------------------";
                    cout << "\n    *** INFORMAÇÕES DA VENDA ***";
                    cout << "\n --------------------------------------";
                    cout << "\n Nº da Nota: " << V.NumeroNota;
                    cout << "\n Cliente (ou Mesa): " << V.clienteNome;
                    cout << "\n Forma de Pagamento: " << V.formaPagamento;
                    cout << "\n Qtd. Vendida: " << fixed << setprecision(2) << V.totalItens;
                    cout << "\n Preço Vendido: " << fixed << setprecision(2) << V.totalNota;
                    cout << "\n Data e Horas: "; imprimirData(V.DataVenda);
                    cout << "\n --------------------------------------\n";
                    
                    cout << "\n --------------------------------------";
                    cout << "\n  *** INFORMAÇÕES DOS ITENS ***";
                    cout << "\n --------------------------------------";
                    
                    ArqItens = fopen("Itens.dat", "rb");
                    fseek(ArqItens, pos, SEEK_SET);
                    fread(&itens, sizeof(itens), 1, ArqItens);
            
                    rewind(ArqItens);
                    while(fread(&itens, sizeof(itens), 1, ArqItens) == 1) {
                        if(itens.NumeroNota == V.NumeroNota) {
                            cout << "\n  Código do Produto : " << itens.CodigoItem;
                            cout << "\n  Descrição : " << itens.DescricaoItem;
                            cout << "\n  Preço Unitário : " << fixed << setprecision(2) << itens.PrecoUnitario;
                            cout << "\n  Qtd. Vendida : " << itens.QtdVendida;
                            cout << "\n --------------------------------------";
                        }
                    }
                    fclose(ArqItens);
                    cout << "\n1) Para editar Venda";
                    cout << "\n0) Para sair...";
                    cout << "\n- Informe sua escolha: ";
                    cin >> escolha;
                    while((escolha < 0) || (escolha > 2)) {
                        cout << "\nInválido! Digite um valor [1, 2 ou 3]: ";
                        cin >> escolha;
                    }
                    if(escolha == 1) {
                        EditarVenda(pos);
                    } else {
                        cout << "\n Saindo...";
                    }
                }
            }
        }
    } while(opcaoM != 0);
    pausa();
}




void EditarVenda(int pos) {
    char escolha;
    RegVenda V;
    
    ArqVendas = fopen("Vendas.dat", "r+b");
    fseek(ArqVendas, pos, SEEK_SET);
    fread(&V, sizeof(V), 1, ArqVendas);
    
    cout << "\n *** MODIFICAR VENDA ***";
    cout << "\n Número da Nota: " << V.NumeroNota;
    cout << "\n- Modificar Descrição de Venda (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n- Novo Descrição de Venda: ";
        cin.ignore(80, '\n');
        cin.getline(V.clienteNome, sizeof(V.clienteNome));
    }
    
    cout << "\n Modificar Forma de Pagamento (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n- Novo Descrição de Forma de Pagamento: ";
        cin.ignore(80, '\n');
        cin.getline(V.formaPagamento, sizeof(V.formaPagamento));
    }
    
    fseek(ArqVendas, pos, SEEK_SET);
    fwrite(&V, sizeof(V), 1, ArqVendas);
    fclose(ArqVendas);
    cout << "\n Venda Alterada com sucesso";
    pausa();
    system("cls");
}




void MenuExclusaoVenda() {
    system("cls");
    int opcaoE, posicao, posicaoS, opcaoExcluir, escolha, escolhaS;
    char excluir;
    
    do {
        cout << "\n -----------------------------------------\n";
        cout << "           *** MENU DE EXCLUSÃO ***";
        cout << "\n -----------------------------------------\n";
        cout << "\n1) Deseja Excluir a Venda Inteira;      ";
        cout << "\n0) Para retornar ao Menu Principal...   ";
        cout << "\n- Digite sua escolha: ";
        cin >> opcaoE;
        while((opcaoE < 0) || (opcaoE > 1)) {
            cout << "\nEscolha inválida! Digite Novamente";
            cin >> opcaoE;
        }
        if(opcaoE == 1) {
            RelatorioVenda();
            
            cout << "\n";
            cout << "\n -----------------------------------------\n";
            cout << "           *** EXCLUSÃO DE VENDA ***";
            cout << "\n -----------------------------------------\n";
            cout << "\n- Para sair: Digite = 0 ";
            cout << "\n- Informe o Nº da Nota: ";
            cin >> escolha;
            while(escolha < 0) {
                cout << "\nEscolha inválida! Digite novamente: ";
                cin >> escolha;
            }
            posicao = ConsultaVenda(escolha);
            
            if(posicao == -1) {
                cout << "\n Venda não cadastrado ou Nº Nota não existe";
                pausa();
            } else {
                system("cls");
                RegVenda V;
                
                ArqVendas = fopen("Vendas.dat", "rb");
                fseek(ArqVendas, posicao, SEEK_SET);
                fread(&V, sizeof(V), 1, ArqVendas);
                fclose(ArqVendas);
                cout << "\n --------------------------------------";
                cout << "\n  *** INFORMAÇÕES DA VENDA ***";
                cout << "\n --------------------------------------";
                cout << "\n  Nº Nota            : " << V.NumeroNota;
                cout << "\n  (Nome ou Mesa)     : " << V.clienteNome;
                cout << "\n  Forma de Pagamento : " << V.formaPagamento;
                cout << "\n  Qtd. Vendida       : " << V.totalItens;
                cout << "\n  Preço da Venda     : " << V.totalNota;
                cout << "\n  Data  e  Horas     : "; imprimirData(V.DataVenda);
                cout << "\n --------------------------------------";
                cout << "\n Excluir (S/N)?";
                cin >> excluir;
                if(toupper(excluir) == 'S') {
                    ExcluirVenda(escolha);
                    cout << "Exclusão feita com sucesso";
                } else {
                    cout << "Exclusão cancelada";
                }
                pausa();
                system("cls");
            }
        } 
    } while(opcaoE != 0);
    pausa();
}




long int ExcluirVenda(int numNota) {
    system("cls");
    long int pos = -1;
    RegVenda v;
    
    FILE *ArqTemp;
    
    ArqVendas = fopen("Vendas.dat", "rb");
    if(ArqVendas == NULL) {
        cout << "Erro na abertura do arquivo \n";
        pausa();
        exit(1);
    } 
    
    ArqTemp = fopen("Temp.dat", "wb");
    if(ArqTemp == NULL) {
        cout << "Erro ao criar arquivo temporario \n";
        fclose(ArqVendas);
        pausa();
        exit(1);
    }
    
    while(fread(&v, sizeof(v), 1, ArqVendas) == 1) {
        if(v.NumeroNota == numNota) {
            pos = ftell(ArqItens) - (long)sizeof(v);
        } else {
            fwrite(&v, sizeof(v), 1, ArqTemp);
        }
    }
    fclose(ArqVendas);
    fclose(ArqTemp);
    
    if(pos != -1) {
        remove("Vendas.dat");
        rename("Temp.dat", "Vendas.dat");
    } else {
        remove("Temp.dat");
    }
    return pos;
}


void MenuFornecedores() {
    int opcao;
    do {
        system("cls");
        cout << "\n ================ MENU DE FORNECEDORES ================";
        cout << "\n\n ";
        cout << "\n 1 - Adicionar Fornecedor";
        cout << "\n 2 - Relatório de Fornecedores";
        cout << "\n 3 - Pesquisar de Fornecedor";
        cout << "\n 4 - Editar Fornecedor";
        cout << "\n 5 - Excluir Fornecedor";
        cout << "\n 0 - Retornar ao Menu Principal";
        cout << "\n\n - Escolha a opção: ";
        cin >> opcao;
        switch(opcao) {
            case 1: system("cls");
                    AdicionarForne();
                    break;
            case 2: RelatorioForne();
                    pausa();
                    break;
            case 3: system("cls");
                    BuscaForne();
                    pausa();
                    break;  
            case 4: MenuEdicaoForne();
                    break;
            case 5: MenuExclusaoForne();
                    break;
            default: cout << "\n Tecle <Enter> para retornar ao menu principal "; system("pause>>null");
                    break;
        }
    } while(opcao != 0);
}


void AdicionarForne() {
    char opcao;
    system("cls");
    ArqFornecedores = fopen("Fornecedores.dat", "ab");
    if(ArqFornecedores == NULL) {
        cout << "Erro na abertura do arquivo \n";
        system("pause");
        exit(1);
    }
    
    do {
        Forne.id = ultimoForne();
        cout << "\n ID do Fornecedor: " << Forne.id;
        
        cin.ignore(80, '\n');
        
        cout << "\n Nome da Empresa: ";
        cin.getline(Forne.nome_empresa, sizeof(Forne.nome_empresa));


        cout << "\n CNPJ [ Ex.: 00.000.000/0001-00 ]: ";
        cin.getline(Forne.cnpj, sizeof(Forne.cnpj));


        cout << "\n Telefone [ Ex.: (11) 99999-9999 ]: ";
        cin.getline(Forne.telefone, sizeof(Forne.telefone));


        cout << "\n E-mail: ";
        cin.getline(Forne.email, sizeof(Forne.email));


        cout << "\n Nome de Contato ( Representante da Empresa ): ";
        cin.getline(Forne.nome_contato, sizeof(Forne.nome_contato));


        cout << "\n Estado (dois dígitos, ex.: SP): ";
        cin.getline(Forne.estado, sizeof(Forne.estado));
        strupr(Forne.estado);


        cout << "\n Cidade: ";
        cin.getline(Forne.cidade, sizeof(Forne.cidade));
        
        fwrite(&Forne, sizeof(Forne), 1, ArqFornecedores);
        fflush(ArqFornecedores); // buffer para ultimoFornecedor ver o registro e por nº    
        cout << "\n Adicionar outro fornecedor (S/N)? ";
        cin >> opcao;
        
    } while(toupper(opcao) == 'S');
    fclose(ArqFornecedores);
    
}


long int ultimoForne() {
    long int cod;
    RegFornecedor aux;
    
    FILE *f = fopen("Fornecedores.dat", "rb");
    
    if(f == NULL) return 1;
    if(fread(&aux, sizeof(aux), 1, f) == 0) {
        cod = 1;
    } else {
        fseek(f, -(long)sizeof(aux), SEEK_END);
        fread(&aux, sizeof(aux), 1, f);
        cod = aux.id + 1;
    }
    fclose(f);
    return cod;
}


void RelatorioForne() {
    system("cls");
    ArqFornecedores = fopen("Fornecedores.dat", "rb"); // open read binary
    if(ArqFornecedores == NULL) {
        cout << "Nenhum produto cadastrado ainda \n";
        pausa();
        return;
    }
    cout << "\n                                *** FORNECEDORES ***";
    cout << "\n -------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    cout << left  << setw(6) << "ID"
         << left  << setw(22) << "Nome Empresa"
         << right << setw(20) << "CNPJ"
         << right << setw(17) << "Telefone"
         << right << setw(28) << "E-mail"
         << right << setw(22) << "Nome Vendedor"
         << right << setw(8) << "Estado"
         << right << setw(20) << "Cidade";
    cout << "\n -------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
   
    while(fread(&Forne, sizeof(Forne), 1, ArqFornecedores) == 1) {
        cout << "\n";
        cout << left  << setw(6) << Forne.id;
        cout << left  << setw(22) << Forne.nome_empresa;
        cout << right << setw(20) << Forne.cnpj;
        cout << right << setw(17) << Forne.telefone;
        cout << right << setw(28) << Forne.email;
        cout << right << setw(22) << Forne.nome_contato;
        cout << right << setw(8) << Forne.estado;
        cout << right << setw(20) << Forne.cidade;                   
    }
    cout << endl;
       
    fclose(ArqFornecedores);
}


void BuscaForne() {
    int buscaID, result;
    
    cout << "\n *** CONSULTAR FORNECEDOR ***";
    cout << "\n ----------------------------\n";
    cout << "\n Digite o ID do forncedor: ";
    cin >> buscaID;
    result = ConsultaForne(buscaID);    
    if(result == -1) {
        cout << "\n Fornecedor não cadastrado";
    } else {
        ArqFornecedores = fopen("Fornecedores.dat", "rb");
        fseek(ArqFornecedores, result, SEEK_SET);
        fread(&Forne, sizeof(Forne), 1, ArqFornecedores);
        fclose(ArqFornecedores);
        
        cout << "\n ---------------------------------";
        cout << "\n *** INFORMAÇÕES DO FORNECEDOR ***";
        cout << "\n ---------------------------------";
        cout << "\n ID       : " << Forne.id;
        cout << "\n Empresa  : " << Forne.nome_empresa;
        cout << "\n CNPJ     : " << Forne.cnpj;
        cout << "\n Vendedor : " << Forne.nome_contato;
        cout << "\n Telefone : " << Forne.telefone;
        cout << "\n E-mail   : " << Forne.email;
        cout << "\n Estado   : " << Forne.estado;
        cout << "\n Cidade   : " << Forne.cidade;
        cout << "\n ---------------------------------";
    }
}


long int ConsultaForne(int id) {
    ArqFornecedores = fopen("Fornecedores.dat", "rb");
    if(ArqFornecedores == NULL) {
        return -1;    
    }
    while(fread(&Forne, sizeof(Forne), 1, ArqFornecedores) == 1) {
        if(Forne.id == id) {
            long int posicao = ftell(ArqFornecedores) - (long)sizeof(Forne);
            fclose(ArqFornecedores);
            return posicao;
        }
    }
    fclose(ArqFornecedores);
    return -1;
}


void MenuEdicaoForne() {
    int id, pos;
    int opcaoM;
    system("cls");
    
    do {
        cout << "\n --------------------------------------------\n";
        cout << "    *** MENU DE MODIFICAÇÃO DE FORNECEDOR ***    ";
        cout << "\n --------------------------------------------\n";
        cout << "\n1) Ver Relatório de Forncedores Cadastrados; ";
        cout << "\n0) Para retornar ao Menu de Forncedores;     ";
        cout << "\n- Digite sua escolha: ";
        cin >> opcaoM;
        while((opcaoM < 0)||(opcaoM > 1)) {
            cout << "\n Opção inválida! Digite Novamente: ";
            cin >> opcaoM;    
        }
        if(opcaoM == 1) {
            RelatorioForne();
            cout << "\n Se quiser retorna a página anterior (Digite = 0) ";
            cout << "\n- ID do Fornecedor que será modificado: ";
            cin >> id;
            if(id == 0) {
                pausa();
                system("cls");
            } else {
                pos = ConsultaForne(id);
                if(pos == -1) {
                    cout << "\n Fornecedor não existe";
                    pausa();
                } else {
                    system("cls");
                    ArqFornecedores = fopen("Fornecedores.dat", "rb");
                    fseek(ArqFornecedores, pos, SEEK_SET);
                    fread(&Forne, sizeof(Forne), 1, ArqFornecedores);
                    fclose(ArqFornecedores);
                    cout << "\n ---------------------------------";
                    cout << "\n *** INFORMAÇÕES DO FORNECEDOR ***";
                    cout << "\n ---------------------------------";
                    cout << "\n ID       : " << Forne.id;
                    cout << "\n Empresa  : " << Forne.nome_empresa;
                    cout << "\n CNPJ     : " << Forne.cnpj;
                    cout << "\n Vendedor : " << Forne.nome_contato;
                    cout << "\n Telefone : " << Forne.telefone;
                    cout << "\n E-mail   : " << Forne.email;
                    cout << "\n Estado   : " << Forne.estado;
                    cout << "\n Cidade   : " << Forne.cidade;
                    cout << "\n ---------------------------------";
                    EditarForne(pos);
                }
            }
        }
    } while(opcaoM != 0);
    pausa();
}


void EditarForne(int pos) {
    char escolha;
    ArqFornecedores = fopen("Fornecedores.dat", "r+b");
    fseek(ArqFornecedores, pos, SEEK_SET);
    fread(&Forne, sizeof(Forne), 1, ArqFornecedores);
    
    cout << "\n *** MODIFICAR FORNECEDOR ***";
        
    cout << "\n Modificar Nome da Empresa (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n Novo Nome da Empresa: ";
        cin.ignore(80, '\n');
        cin.getline(Forne.nome_empresa, sizeof(Forne.nome_empresa));
    }


    cout << "\n Modificar CNPJ (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {    
        cout << "\n Novo CNPJ [ Ex.: 00.000.000/0001-00 ]:";
        cin.ignore(80, '\n');
        cin.getline(Forne.cnpj, sizeof(Forne.cnpj));
    }
    
    cout << "\n Modificar Telefone (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n Novo Telefone [ Ex.: (11) 99999-9999 ]: ";
        cin.ignore(80, '\n');
        cin.getline(Forne.telefone, sizeof(Forne.telefone));
    }
    
    cout << "\n Modificar E-mail (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n Novo E-mail: ";
        cin.ignore(80, '\n');
        cin.getline(Forne.email, sizeof(Forne.email));
    }
    
    cout << "\n Modificar Nome de Contato [ Representante da Empresa ] (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n Novo Nome de Contado (S/N): ";
        cin.ignore(80, '\n');
        cin.getline(Forne.nome_contato, sizeof(Forne.nome_contato));
    }
    
    cout << "\n Estado (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n Novo Estado (dois dígitos, ex.: SP): ";
        cin.ignore(80, '\n');
        cin.getline(Forne.estado, sizeof(Forne.estado));
        strupr(Forne.estado);
    }
    
    cout << "\n Modificar Cidade (S/N): ";
    cin >> escolha;
    if(toupper(escolha) == 'S') {
        cout << "\n Nova Cidade: ";
        cin.ignore(80, '\n');
        cin.getline(Forne.cidade, sizeof(Forne.cidade));
    }
    
    fseek(ArqFornecedores, pos, SEEK_SET);                 // Volta ao inicio do registro
    fwrite(&Forne, sizeof(Forne), 1, ArqFornecedores);     // Sobreescreve as modificacoes
    fflush(ArqFornecedores);                             // buffer para ultimoFornecedor ver o registro e por nº    
    cout << "\n Dados do Fornecedor Alterados com Sucesso";
    pausa();
    system("cls");
}

void MenuExclusaoForne() {
    system("cls");
    int opcaoE, posicao, id;
    char excluir;
    
    do {
        cout << "\n ---------------------------------------------------\n";
        cout << "           *** MENU DE EXCLUSÃO FORNECEDOR ***";
        cout << "\n ---------------------------------------------------\n";
        cout << "\n1) Ver Relatório de Fornecedores Cadastrados; ";
        cout << "\n0) Para retornar ao Menu Principal;       ";
        cout << "\n- Digite sua escolha: ";
        cin >> opcaoE;
        while((opcaoE < 0) || (opcaoE > 1)) {
            cout << "\nEscolha inválida! Digite Novamente";
            cin >> opcaoE;
        }
        if(opcaoE == 1) {
            RelatorioForne();
            cout << "\n- Excluir Fornecedor (pesquise por ID): ";
            cin >> id;
            posicao = ConsultaForne(id);
            if(posicao == -1) {
                cout << "\n Fornecedor não Cadastrado";
                pausa();
            } else {
                system("cls");
                ArqFornecedores = fopen("Fornecedores.dat", "rb");
                fseek(ArqFornecedores, posicao, SEEK_SET);
                fread(&Forne, sizeof(Forne), 1, ArqFornecedores);
                fclose(ArqFornecedores);
                cout << "\n ---------------------------------";
                cout << "\n *** INFORMAÇÕES DO FORNECEDOR ***";
                cout << "\n ---------------------------------";
                cout << "\n ID       : " << Forne.id;
                cout << "\n Empresa  : " << Forne.nome_empresa;
                cout << "\n CNPJ     : " << Forne.cnpj;
                cout << "\n Vendedor : " << Forne.nome_contato;
                cout << "\n Telefone : " << Forne.telefone;
                cout << "\n E-mail   : " << Forne.email;
                cout << "\n Estado   : " << Forne.estado;
                cout << "\n Cidade   : " << Forne.cidade;
                cout << "\n ---------------------------------";
                cout << "\n Excluir (S/N)?";
                cin >> excluir;
                if(toupper(excluir) == 'S') {
                    ExcluirForne(id);
                    cout << "Exclusão feita com sucesso";
                } else {
                    cout << "Exclusão cancelada";
                }
                pausa();
                system("cls");
            }
        } 
    } while(opcaoE != 0);
    pausa();
}

long int ExcluirForne(int id) {
    system("cls");
    long int pos = -1;
    FILE *ArqTemp;
    
    ArqFornecedores = fopen("Fornecedores.dat", "rb");
    if(ArqFornecedores == NULL) {
        cout << "Erro na abertura do arquivo \n";
        pausa();
        exit(1);
    }
    
    ArqTemp = fopen("Temp.dat", "wb");
    if(ArqTemp == NULL) {
        cout << "Erro ao criar arquivo temporario \n";
        fclose(ArqFornecedores);
        pausa();
        exit(1);
    }


    while(fread(&Forne, sizeof(Forne), 1, ArqFornecedores) == 1) {
        if(Forne.id == id) {
            pos = ftell(ArqFornecedores) - (long)sizeof(Forne);
        } else {
            fwrite(&Forne, sizeof(Forne), 1, ArqTemp);
        }
    }
    fclose(ArqFornecedores);
    fclose(ArqTemp);
    
    if(pos != -1) {
        remove("Fornecedores.dat");
        rename("Temp.dat", "Fornecedores.dat");
    } else {
        remove("Temp.dat");    
    }
    return pos;
}
