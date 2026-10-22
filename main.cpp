/*
  C�digo criado por Thiago Felski Pereira
  Move um caractere na matriz sem testes
  �til para implementa��o de jogos
  com movimenta��o de personagem.
  (T�picos de algoritmos 1)

  VERS�O WINDOWS (usa a API do console, sem c�digos de escape)

  Compila��o (MinGW/g++):  g++ movimenta_matriz_windows.cpp -o jogo.exe
*/
#include <iostream>
#include <conio.h>   // _getch()
#include <windows.h> // fun��es de cursor e cor do console
#include <string>
#include <fstream>
#include <sstream> // stringstream
#include <cctype> // converte teclas maiusculas para minusculas
#include <cstdio> // remove()
using namespace std;

const int TAM = 21;
const int MAX_RECORDE = 200;

struct Posicao {
    int linha, coluna;
};

struct Jogo {
    int cenario[TAM][TAM];
    int n;
    int orientacao;
    Posicao jogador;
    Posicao bloco;
    int movimentos, rotacoes;
    int numeroMapa;
    string titulo;
};

struct Recorde {
    string nome;
    int numeroMapa, movimentos, rotacoes;
};

int getch(void) {
  return _getch(); // o conio.h j� l� a tecla sem ENTER e sem mostrar na tela
}

// Coloca o cursor de escrita na linha/coluna indicada.
// Obs: a fun��o SetCursorPos do windows.h N�O serve aqui,
// ela move o mouse. A do console � a SetConsoleCursorPosition.
void PosicionaCursor(int XPos, int YPos) {
  COORD pos;
  pos.X = XPos;
  pos.Y = YPos;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Esconde o tra�o piscante do cursor, para n�o atrapalhar o desenho
void EscondeCursor() {
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 1;
  info.bVisible = FALSE;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

// Liga o fundo azul para o que for escrito a seguir
void FundoAzul() {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                          BACKGROUND_BLUE | BACKGROUND_INTENSITY);
}

// Volta para o texto normal (cinza claro sobre fundo preto)
void CorNormal() {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                          FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

template <typename T>
void trocar(T &a, T &b){
    T aux = a;
    a = b;
    b = aux;
}

int carregarRecordes(Recorde recordes[], int capacidade){
    ifstream arquivo("recordes.csv");

    if(!arquivo.is_open()){
        return 0;
    }

    int quantidade = 0;
    string linha;

    while(quantidade < capacidade && getline(arquivo, linha)){
        stringstream conversao(linha);
        Recorde r;
        string campo;

        // Le o nome
        if(!getline(conversao, r.nome, ',')){
            continue;
        }

        // Le o numero do mapa
        if(!getline(conversao, campo, ',')){
            continue;
        }
        stringstream(campo)>>r.numeroMapa;

        // Le os movimentos
        if(!getline(conversao, campo, ',')){
            continue;
        }
        stringstream(campo)>>r.movimentos;

        // Le as rotacoes
        if(!getline(conversao, campo)){
            continue;
        }
        stringstream(campo)>>r.rotacoes;

        if(r.nome.empty() || r.numeroMapa < 1 || r.movimentos < 0 || r.rotacoes < 0){
            continue;
        }

        recordes[quantidade] = r;
        quantidade++;
    }

    arquivo.close();
    return quantidade;
}

void salvarRecordes(const Recorde recordes[], int quantidade){
    ofstream arquivo("recordes.csv");

    if(!arquivo.is_open()){
        cout<<"Erro ao salvar os recordes!"<<endl;
        return;
    }

    for(int i = 0; i < quantidade; i++){
        arquivo<<recordes[i].nome<<","<<recordes[i].numeroMapa<<","<<recordes[i].movimentos<<","<<recordes[i].rotacoes<<endl;
    }

    arquivo.close();
}

void registrarRecorde(string nome, int numeroMapa, int movimentos, int rotacoes){
    Recorde recordes[MAX_RECORDE];
    int quantidade = carregarRecordes(recordes, MAX_RECORDE);

    // Adiciona o novo resultado
    if(quantidade < MAX_RECORDE){
        recordes[quantidade].nome = nome;
        recordes[quantidade].numeroMapa = numeroMapa;
        recordes[quantidade].movimentos = movimentos;
        recordes[quantidade].rotacoes = rotacoes;
        quantidade++;
    }

    // Organiza os resultados
    for(int i = 0; i < quantidade - 1;i++){
        for(int j = i + 1; j < quantidade;j++){

            bool precisaTrocar = false;

            if(recordes[j].numeroMapa < recordes[i].numeroMapa){
                precisaTrocar = true;
            }
            else if(recordes[j].numeroMapa == recordes[i].numeroMapa){
                if(recordes[j].movimentos < recordes[i].movimentos){
                    precisaTrocar = true;
                }

                else if(recordes[j].movimentos == recordes[i].movimentos && recordes[j].rotacoes < recordes[i].rotacoes){
                precisaTrocar = true;
                }
            }

            if(precisaTrocar){
                trocar(recordes[i], recordes[j]);
            }
        }
    }

    // Mantem no maximo 5 recordes por mapa
    int mantidos = 0;
    int mapaAtual = -1;
    int noMapa = 0;

    for(int i = 0; i < quantidade;i++){
        if(recordes[i].numeroMapa != mapaAtual){
            mapaAtual = recordes[i].numeroMapa;
            noMapa = 0;
        }

        if(noMapa < 5){
            recordes[mantidos] = recordes[i];
            mantidos++;
            noMapa++;
        }
    }

    salvarRecordes(recordes, mantidos);
}

void verRecordes(){
    system("cls");

    Recorde recordes[MAX_RECORDE];
    int quantidade = carregarRecordes(recordes, MAX_RECORDE);

    cout<<"p - q - p - q -- p - q - p - q"<<endl;
    cout<<"           RECORDES           "<<endl;
    cout<<"p - q - p - q -- p - q - p - q"<<endl<<endl;

    if(quantidade == 0){
        cout<<"Ainda nao existem recordes registrados."<<endl;
    }else{
        int mapaAtual = -1;
        int posicao = 0;

        for(int i = 0; i < quantidade;i++){
            if(recordes[i].numeroMapa != mapaAtual){
                mapaAtual = recordes[i].numeroMapa;
                posicao = 1;

                cout<<endl;
                cout<<"------ MAPA "<<mapaAtual<<" ------"<<endl;
            }else{
                posicao++;
            }

            cout<<posicao<<"o - "<<recordes[i].nome<<endl;
            cout<<" | Movimentos: "<<recordes[i].movimentos<<endl;
            cout<<" | Rotacoes: "<<recordes[i].rotacoes<<endl<<endl;
        }
    }

    cout<<endl<<"Pressione uma tecla para voltar...";
    getch();
}

bool portaFechada(int tipoPorta, int orientacao){
    if(tipoPorta == 6){
        return orientacao == 0 || orientacao == 2;
    }
    if(tipoPorta == 7){
        return orientacao == 1 || orientacao == 3;
    }
    return false;
}

bool podemover(int novaLinha, int novaColuna, const Jogo &jogo){

    if(novaLinha < 0 || novaLinha >= jogo.n || novaColuna < 0 || novaColuna >= jogo.n){
        return false;
    }

    if(novaLinha == jogo.bloco.linha && novaColuna == jogo.bloco.coluna){
        return false;
    }

    int tipo = jogo.cenario[novaLinha][novaColuna];

    if(tipo == 0 || tipo == 4 || tipo == 5){
        return true;
    }

    if(tipo == 6 || tipo == 7){
        return !portaFechada(tipo, jogo.orientacao);
    }

    return false;
}

bool moveplayer(char tecla, Jogo &jogo){
    switch(tecla){

    case 'w':
        if(podemover(jogo.jogador.linha - 1, jogo.jogador.coluna, jogo)){
            jogo.jogador.linha--;
            return true;
        }
      break;

    case 's':
        if(podemover(jogo.jogador.linha + 1, jogo.jogador.coluna, jogo)){
            jogo.jogador.linha++;
            return true;
        }
      break;

    case 'a':
        if(podemover(jogo.jogador.linha, jogo.jogador.coluna - 1, jogo)){
            jogo.jogador.coluna--;
            return true;
        }
      break;

    case 'd':
        if(podemover(jogo.jogador.linha, jogo.jogador.coluna + 1, jogo)){
            jogo.jogador.coluna++;
            return true;
        }
      break;
    }
    return false;
}

void mapalabirinto(const Jogo &jogo){
    for(int i = 0;i < jogo.n;i++){
        for(int j = 0;j < jogo.n;j++){
                if(i == jogo.jogador.linha && j == jogo.jogador.coluna){
                    cout<<"@";
                }
                else if(i == jogo.bloco.linha && j == jogo.bloco.coluna){
                    cout<<"O";
                }else{
                    switch(jogo.cenario[i][j]){

                    case 0:
                        cout<<" ";
                        break;

                    case 1:
                        FundoAzul();
                        cout<<" ";
                        CorNormal();
                        break;

                    case 2:
                        cout<<"@";
                        break;

                    case 3:
                        cout<<"O";
                        break;

                    case 4:
                        cout<<"A";
                        break;

                    case 5:
                        cout<<"S";
                        break;

                    case 6:
                        if(portaFechada(6, jogo.orientacao)){
                            cout<<"=";
                        }else{
                            cout<<":";
                        }
                        break;

                    case 7:
                        if(portaFechada(7, jogo.orientacao)){
                            cout<<"|";
                        }else{
                            cout<<";";
                        }
                        break;

                    default:
                        cout<<" ";
                    }
                }
        }
        cout<<endl;
    }
}

bool vitoria(const Jogo &jogo){
    return jogo.cenario[jogo.jogador.linha][jogo.jogador.coluna] == 5;
}

bool alavancaPlayerEmcima(const Jogo &jogo){
    return jogo.cenario[jogo.jogador.linha][jogo.jogador.coluna] == 4;
}

void rotacionar(char tecla, Jogo &jogo){
    if(tecla == 'e'){
        jogo.orientacao = (jogo.orientacao + 1) % 4;
        jogo.rotacoes++;
    }
    if(tecla == 'q'){
        jogo.orientacao = (jogo.orientacao - 1 + 4) % 4;
        jogo.rotacoes++;
    }
}

void girarDireita(Jogo &jogo){
    int aux[TAM][TAM];

    for(int i = 0;i < jogo.n;i++){
        for(int j = 0;j < jogo.n;j++){
            aux[j][jogo.n - 1 - i] = jogo.cenario[i][j];
        }
    }

    for(int i = 0;i < jogo.n;i++){
        for(int j = 0;j < jogo.n;j++){
            jogo.cenario[i][j] = aux[i][j];
        }
    }
}

void girarPlayerDireita(Jogo &jogo){

    int novaLinha = jogo.jogador.coluna;
    int novaColuna = jogo.n - 1 - jogo.jogador.linha;

    jogo.jogador.linha = novaLinha;
    jogo.jogador.coluna = novaColuna;
}

void girarBlocoDireita(Jogo &jogo){
    if(jogo.bloco.linha == -1 && jogo.bloco.coluna == -1){
        return;
    }

    int novaLinha = jogo.bloco.coluna;
    int novaColuna = jogo.n - 1 - jogo.bloco.linha;

    jogo.bloco.linha = novaLinha;
    jogo.bloco.coluna = novaColuna;
}

void girarEsquerda(Jogo &jogo) {
    int aux[TAM][TAM];

    for (int i = 0; i < jogo.n; i++) {
        for (int j = 0; j < jogo.n; j++) {
            aux[jogo.n - 1 - j][i] = jogo.cenario[i][j];
        }
    }

    for (int i = 0; i < jogo.n; i++) {
        for (int j = 0; j < jogo.n; j++) {
            jogo.cenario[i][j] = aux[i][j];
        }
    }
}

void girarPlayerEsquerda(Jogo &jogo){

    int novaLinha = jogo.n - 1 - jogo.jogador.coluna;
    int novaColuna = jogo.jogador.linha;

    jogo.jogador.linha = novaLinha;
    jogo.jogador.coluna = novaColuna;
}

void girarBlocoEsquerda(Jogo &jogo){
    if(jogo.bloco.linha == -1 && jogo.bloco.coluna == -1){
        return;
    }

    int novaLinha = jogo.n - 1 - jogo.bloco.coluna;
    int novaColuna = jogo.bloco.linha;

    jogo.bloco.linha = novaLinha;
    jogo.bloco.coluna = novaColuna;
}

void gravidade(Jogo &jogo){

    if(jogo.bloco.linha == -1 && jogo.bloco.coluna == -1){
        return;
    }

    while(true){
        int baixo = jogo.bloco.linha + 1;

        if(baixo >= jogo.n){
            break;
        }

        int tipo = jogo.cenario[baixo][jogo.bloco.coluna];

        if(tipo == 0){
            jogo.bloco.linha     = baixo;
        }
        else if((tipo == 6 || tipo == 7) && !portaFechada(tipo, jogo.orientacao)){
            jogo.bloco.linha = baixo;
        }else{
            break;
        }
    }
}

void esmagarBloco(Jogo &jogo){

    if(jogo.bloco.linha == -1 && jogo.bloco.coluna == -1){
        return;
    }

    int linha = jogo.bloco.linha;
    int coluna = jogo.bloco.coluna;
    int tipo = jogo.cenario[linha][coluna];

    if(tipo == 6 || tipo == 7){
        if(portaFechada(tipo, jogo.orientacao)){
            jogo.bloco.linha = -1;
            jogo.bloco.coluna = -1;
        }
    }
}

bool esmagarPlayer(const Jogo &jogo){

    int linha = jogo.jogador.linha;
    int coluna = jogo.jogador.coluna;
    int tipo = jogo.cenario[linha][coluna];

    if(tipo == 6 || tipo     == 7){
        if(portaFechada(tipo, jogo.orientacao)){
            return true;
        }
    }
    return false;
}

bool carregarMapa(Jogo &jogo, int numeroMapa, string &erro){
    ifstream arquivo;

    // Monta o nome do arquivo conforme o numero do mapa e abre o arquivo para leitura
    string nomeArquivo = "mapa" + to_string(numeroMapa) + ".csv";
    arquivo.open(nomeArquivo);

    if(!arquivo.is_open()){
        erro = "Arquivo CSV nao encontrado.";
        return false;
    }

    string linha;
    int numeroLinha = 1; // primeira linha do csv contem a dimensao do mapa

    if(!getline(arquivo, linha)){
        erro = "Dimensao do mapa ausente.";
        arquivo.close();
        return false;
    }

    stringstream converteN(linha);
    if(!(converteN >> jogo.n)){
        erro = "Linha 1: dimensao invalida. Informe um numero inteiro.";
        arquivo.close();
        return false;
    }
    string extra;
    if(converteN >> extra){
        erro = "Linha 1: dimensao invalida. Existem caracteres extras.";
        arquivo.close();
        return false;
    }
    if(jogo.n < 11 || jogo.n > TAM){
        erro = "Linha 1: dimensao invalida. Deve estar entre 11 e " + to_string(TAM) + ".";
        arquivo.close();
        return false;
    }

    numeroLinha++;
    // jogo.titulo.empty verifica se a string esta vazia
    if(!getline(arquivo, jogo.titulo) || jogo.titulo.empty()){
        erro = "Linha " + to_string(numeroLinha) + ": titulo ausente ou vazio.";
        arquivo.close();
        return false;
    }

    jogo.numeroMapa = numeroMapa;
    jogo.orientacao = 0;
    jogo.movimentos = 0;
    jogo.rotacoes = 0;

    jogo.jogador.linha = -1;
    jogo.jogador.coluna = -1;
    jogo.bloco.linha = -1;
    jogo.bloco.coluna = -1;

    int saidas = 0;

    //Le matriz
    for(int i = 0;i < jogo.n;i++){
        numeroLinha++;

        if(!getline(arquivo, linha)){
            erro = "Linha " + to_string(numeroLinha) + ": linha da matriz ausente.";
            arquivo.close();
            return false;
        }

        stringstream linhaCSV(linha);
        string valor;

        for(int j = 0;j < jogo.n;j++){
            if(!getline(linhaCSV, valor, ',')){
                erro = "Linha " + to_string(numeroLinha) + ": quantidade de colunas insuficiente.";
                arquivo.close();
                return false;
            }

            //length vai rejeitar valores com + ou - de 1 caractere
            if(valor.length() != 1 || valor[0] < '0' || valor[0] > '7'){
                erro = "Linha " + to_string(numeroLinha) + ", coluna " + to_string(j + 1) + ": valor invalido. Use numeros de 0 a 7.";
                arquivo.close();
                return false;
            }
            int tipo = valor[0] - '0'; //converte para inteiro

            jogo.cenario[i][j] = tipo;

            //guarda posicao inicial do jogador
            if(tipo == 2){
                if(jogo.jogador.linha != -1){
                    erro = "Linha " + to_string(numeroLinha) + ": jogador duplicado. O mapa deve ter apenas um jogador.";
                    arquivo.close();
                    return false;
                }

                jogo.jogador.linha = i;
                jogo.jogador.coluna = j;
                jogo.cenario[i][j] = 0;
            }

            //guarda posicao inicial do bloco
            if(tipo == 3){
                if(jogo.bloco.linha != -1){
                    erro = "Linha " + to_string(numeroLinha) + ": bloco duplicado. O mapa deve ter apenas um bloco.";
                    arquivo.close();
                    return false;
                }

                jogo.bloco.linha = i;
                jogo.bloco.coluna = j;
                jogo.cenario[i][j] = 0;
            }

            if(tipo == 5){
                saidas++;
            }
        }

        if(getline(linhaCSV, valor, ',')){
            erro = "Linha " + to_string(numeroLinha) + ": quantidade de colunas excedida.";
            arquivo.close();
            return false;
        }
    }

    if(getline(arquivo, linha)){
        erro = "Linha " + to_string(numeroLinha + 1) + ": existe uma linha extra apos o final da matriz.";
        arquivo.close();
        return false;
    }

    arquivo.close();

    if(jogo.jogador.linha == -1){
        erro = "Jogador ausente. O mapa deve ter exatamente um jogador (2).";
        return false;
    }

    if(jogo.bloco.linha == -1){
        erro = "Bloco ausente. O mapa deve ter exatamente um bloco (3).";
        return false;
    }

    if(saidas == 0){
        erro = "Saida ausente. O mapa deve ter pelo menos uma saida (5).";
        return false;
    }

    // verifica se o bloco comeca sem flutuar
    Jogo copia = jogo;
    gravidade(copia);

    if(copia.bloco.linha != jogo.bloco.linha){
        erro = "Mapa instavel: o bloco comeca no ar e cairia pela gravidade.";
        return false;
    }

    return true;
}

bool arquivoExiste(const string &nomeArquivo){
    ifstream arquivo(nomeArquivo);

    return arquivo.is_open();
}

int contarMapas(){
    int numeroMapa = 1;

    while(true){
        string nomeArquivo = "mapa" + to_string(numeroMapa) + ".csv";

        if(!arquivoExiste(nomeArquivo)){
            break;
        }

        numeroMapa++;
    }

    return numeroMapa - 1;
}

int contarMapasValidos(){
    int totalMapas = contarMapas();
    int quantidadeValidos = 0;

    for(int i = 0;i <= totalMapas;i++){
        Jogo jogo;
        string erro;

        if(carregarMapa(jogo, i, erro)){
            quantidadeValidos++;
        }
    }

    return quantidadeValidos;
}

int sortearMapaValido(){
    int totalMapas = contarMapas();
    int mapasValidos[TAM], quantidadeValidos = 0;

    for(int i = 1; i <= totalMapas; i++){
        Jogo jogo;
        string erro;

        if(carregarMapa(jogo, i, erro)){
            mapasValidos[quantidadeValidos] = i;
            quantidadeValidos++;
        }
    }

    if(quantidadeValidos == 0){
        return 0;
    }

    int sorteio = rand() % quantidadeValidos;

    return mapasValidos[sorteio];
}

int escolherMapa(){
    int opc;
    int totalMapas = contarMapas();
    system("cls");

    cout<<"============="<<endl;
    cout<<"  NOVO JOGO  "<<endl;
    cout<<"============="<<endl<<endl;

    if(totalMapas == 0){
        cout<<"Nenhum mapa encontrado!"<<endl;
        getch();
        return 0;
    }

    for(int i = 1;i <= totalMapas;i++){
        Jogo jogo;
        string erro;

        if(carregarMapa(jogo, i, erro)){
            cout<<i<<" - "<<jogo.titulo<<endl;
        }else{
            cout<<i<<" - Mapa invalido: "<<erro<<endl;
        }
    }

    cout<<totalMapas + 1<<" - Mapa aleatorio"<<endl<<endl;

    cout<<"Digite uma opcao: ";
    cin>>opc;

    return opc;
}

int menu(){
    int opc;
    system("cls");

    cout<<"WWWWWWWWWWW"<<endl;
    cout<<" LABIRINTO "<<endl;
    cout<<"MMMMMMMMMMM"<<endl<<endl;

    cout<<"1 - Novo jogo"<<endl;
    cout<<"2 - Continuar"<<endl;
    cout<<"3 - Ver solucao"<<endl;
    cout<<"4 - Recordes"<<endl;
    cout<<"5 - Sobre"<<endl;
    cout<<"6 - Fim"<<endl<<endl;

    cout<<"Digite uma opcao: ";
    cin>>opc;

    return opc;
}

bool carregarSolucao(string &solucao, int numeroMapa){
    ifstream arquivo;

    string nomeArquivo = "mapa" + to_string(numeroMapa) + ".sol";
    arquivo.open(nomeArquivo);

    if(!arquivo.is_open()){
        return false;
    }

    getline(arquivo, solucao);
    arquivo.close();

    if(solucao.empty()){
        return false;
    }

    return true;
}

bool processarTecla(char x, Jogo &jogo){
    bool girou = false;

    if(x == 'e' && alavancaPlayerEmcima(jogo)){
        rotacionar(x, jogo);
        girarDireita(jogo);
        girarPlayerDireita(jogo);
        girarBlocoDireita(jogo);

        girou = true;
    }

    if(x == 'q' && alavancaPlayerEmcima(jogo)){
        rotacionar(x, jogo);
        girarEsquerda(jogo);
        girarPlayerEsquerda(jogo);
        girarBlocoEsquerda(jogo);

        girou = true;
    }

    if(moveplayer(x, jogo)){
        jogo.movimentos++;
    }

    if(girou){
        esmagarBloco(jogo);

        if(esmagarPlayer(jogo)){
            return false;
        }

        gravidade(jogo);
    }

    return true;
}

const int solucaoValida = 1; // jogador chegou no final
const int solucaoIncompleta = 0; // as teclas acabaram sem chegar ao final
const int solucaoDerrota = -1; // o jogador foi esmagado durante a solucao
const int solucaoTeclaInvalida = -2; // existe uma tecla nao permitida no arquivo

int executarSolucao(Jogo &jogo, const string &solucao, int &posicaoErro){
    posicaoErro = 0; // informa a tecla que deu erro

    for(int i = 0;i < solucao.length();i++){
        char tecla = tolower(solucao[i]);

        if(tecla != 'w' && tecla != 'a' && tecla != 's' && tecla != 'd' && tecla != 'q' && tecla != 'e'){
            posicaoErro = i + 1;
            return solucaoTeclaInvalida;
        }

        if(!processarTecla(tecla, jogo)){
            posicaoErro = i + 1;
            return solucaoDerrota;
        }

        if(vitoria(jogo)){
            return solucaoValida;
        }
    }

    return solucaoIncompleta;
}

bool testarSolucao(Jogo &jogo){
    string solucao;
    int posicaoErro;

    if(!carregarSolucao(solucao, jogo.numeroMapa)){
        cout<<"Erro ao carregar a solucao!"<<endl;
        return false;
    }

    int resultado = executarSolucao(jogo, solucao, posicaoErro);

    if(resultado == solucaoValida){
        cout<<"Solucao concluida!"<<endl;
        cout<<"Movimentos: "<<jogo.movimentos<<endl;
        cout<<"Rotacoes: "<<jogo.rotacoes<<endl;
        return true;
    }
    else if(resultado == solucaoIncompleta){
        cout<<"A sequencia terminou sem concluir o mapa."<<endl;
    }
    else if(resultado == solucaoDerrota){
        cout<<"O jogador foi esmagado durante a solucao!"<<endl;
        cout<<"Posicao da tecla: "<<posicaoErro<<endl;
    }
    else if(resultado == solucaoTeclaInvalida){
        cout<<"Solucao possui uma tecla invalida!"<<endl;
        cout<<"Posicao da tecla: "<<posicaoErro<<endl;
    }

    return false;
}

void testarTodosMapas(){
    int numeroMapa = 1, totalMapas = 0;
    Jogo jogoTeste;

    while(true){
        string nomeArquivo = "mapa" + to_string(numeroMapa) + ".csv";

        if(!arquivoExiste(nomeArquivo)){
            break;
        }

        totalMapas++;
        cout<<"\nMapa"<<numeroMapa<<": ";

        string erro;

        if(!carregarMapa(jogoTeste, numeroMapa, erro)){
            cout<<"Mapa invalido: "<<erro<<endl;
            numeroMapa++;
            continue;
        }

        cout<<jogoTeste.titulo<<" - ";

        string solucao;

        if(!carregarSolucao(solucao, numeroMapa)){
            cout<<"Arquivo de solucao indisponivel ou invalido!"<<endl;
            numeroMapa++;
            continue;
        }

        int posicaoErro;
        int resultado = executarSolucao(jogoTeste, solucao, posicaoErro);

        if(resultado == solucaoValida){
            cout<<"Solucao valida! ";
            cout<<"Movimentos: "<<jogoTeste.movimentos;
            cout<<", Rotacoes: "<<jogoTeste.rotacoes<<endl;
        }
        else if(resultado == solucaoIncompleta){
            cout<<"Solucao terminou sem vencer."<<endl;
        }
        else if(resultado == solucaoDerrota){
            cout<<"Derrota na tecla "<<posicaoErro<<"."<<endl;
        }
        else if(resultado == solucaoTeclaInvalida){
            cout<<"Tecla invalida na posicao "<<posicaoErro<<"."<<endl;
        }

        numeroMapa++;
    }

    if(totalMapas == 0){
        cout<<"Nenhum arquivo de mapa foi encontrado."<<endl;
    }

    cout<<"\nTeste de todos os mapas concluido."<<endl;
}

void verSolucao(int numeroMapa){
    Jogo jogo;
    string solucao, erro;

    if(!carregarMapa(jogo, numeroMapa, erro)){
        cout<<"Erro ao carregar o mapa: "<<erro<<endl;
        getch();
        return;
    }

    if(!carregarSolucao(solucao, numeroMapa)){
        string nomeArquivo = "mapa" + to_string(numeroMapa) + ".sol";

        if(!arquivoExiste(nomeArquivo)){
            cout<<"Arquivo de solucao nao encontrado: "<<nomeArquivo<<endl;
        }else{
            cout<<"Arquivo de solucao invalido ou vazio: "<<nomeArquivo<<endl;
        }

        getch();
        return;
    }

    system("cls");

    cout<<"Mapa: "<<jogo.numeroMapa<<endl;
    cout<<"Solucao: "<<jogo.titulo<<endl;
    cout<<"Pressione uma tecla para iniciar..."<<endl;
    getch();

    for(int i = 0;i < solucao.length();i++){
        char tecla = tolower(solucao[i]);

        if(tecla != 'w' && tecla != 'a' && tecla != 's' && tecla != 'd' && tecla != 'q' && tecla != 'e'){
            cout<<"tecla invalida na posicao "<<i+1<<"."<<endl;
            getch();
            return;
        }

        if(!processarTecla(tecla, jogo)){
            cout<<"\nDerrota na tecla "<<i+1<<"."<<endl;
            getch();
            return;
        }

        system("cls");

        cout<<"Mapa: "<<jogo.numeroMapa<<endl;
        cout<<"Movimentos: "<<jogo.movimentos<<endl;
        cout<<"Rotacoes: "<<jogo.rotacoes<<endl;
        cout<<"Orientacao: "<<jogo.orientacao * 90<<" graus"<<endl;

        mapalabirinto(jogo);

        cout<<"\nTecla executada: "<<tecla<<endl;

        if(vitoria(jogo)){
            cout<<"\nSolucao concluida!"<<endl;
            cout<<"Total de teclas: "<<i+1<<endl;
            cout<<"Movimentos: "<<jogo.movimentos<<endl;
            cout<<"Rotacoes: "<<jogo.rotacoes<<endl;
            getch();
            return;
        }

        cout<<"Pressione uma tecla para continuar..."<<endl;
        getch();
    }

    cout<<"\nA sequencia terminou sem concluir o mapa."<<endl;
    getch();
}

bool salvarJogo(const Jogo &jogo){
    ofstream arquivo("jogo_salvo.csv");

    if(!arquivo.is_open()){
        return false;
    }

    arquivo<<jogo.numeroMapa<<endl;
    arquivo<<jogo.titulo<<endl;
    arquivo<<jogo.n<<endl;
    arquivo<<jogo.orientacao<<endl;
    arquivo<<jogo.movimentos<<endl;
    arquivo<<jogo.rotacoes<<endl;

    arquivo<<jogo.jogador.linha<<","<<jogo.jogador.coluna<<endl;
    arquivo<<jogo.bloco.linha<<","<<jogo.bloco.coluna<<endl;

    for(int i = 0; i < jogo.n; i++){
        for(int j = 0; j < jogo.n; j++){
            arquivo<<jogo.cenario[i][j];

            if(j < jogo.n - 1){
                arquivo<<",";
            }
        }

        arquivo<<endl;
    }
    arquivo.close();

    return true;
}

void apagarJogoSalvo(){
    remove("jogo_salvo.csv");
}

bool carregarJogo(Jogo &jogo){
    ifstream arquivo("jogo_salvo.csv");

    if(!arquivo.is_open()){
        return false;
    }

    string linha;

    // Le o numero do mapa
    if(!getline(arquivo, linha)){
        arquivo.close();
        return false;
    }
    stringstream conversao(linha);

    if(!(conversao>>jogo.numeroMapa)){
        arquivo.close();
        return false;
    }

    string extra;
    if(conversao>>extra){
        arquivo.close();
        return false;
    }

    if(jogo.numeroMapa < 1){
        arquivo.close();
        return false;
    }

    string nomeMapa = "mapa" + to_string(jogo.numeroMapa) + ".csv";

    if(!arquivoExiste(nomeMapa)){
        arquivo.close();
        return false;
    }

    // Le o titulo
    if(!getline(arquivo, jogo.titulo) || jogo.titulo.empty()){
        arquivo.close();
        return false;
    }

    Jogo mapaOriginal;
    string erro;

    if(!carregarMapa(mapaOriginal, jogo.numeroMapa, erro)){
        arquivo.close();
        return false;
    }

    if(jogo.titulo != mapaOriginal.titulo){
        arquivo.close();
        return false;
    }

    // Le a dimensao do mapa
    if(!getline(arquivo, linha)){
        arquivo.close();
        return false;
    }
    stringstream conversaoN(linha);

    if(!(conversaoN>>jogo.n)){
        arquivo.close();
        return false;
    }

    if(conversaoN>>extra){
        arquivo.close();
        return false;
    }

    if(jogo.n < 11 || jogo.n > TAM){
        arquivo.close();
        return false;
    }

    if(jogo.n != mapaOriginal.n){
        arquivo.close();
        return false;
    }

    // Le a orientacao
    if(!getline(arquivo, linha)){
        arquivo.close();
        return false;
    }
    stringstream conversaoOrientacao(linha);

    if(!(conversaoOrientacao>>jogo.orientacao)){
        arquivo.close();
        return false;
    }

    if(conversaoOrientacao>>linha){
        arquivo.close();
        return false;
    }

    if(jogo.orientacao < 0 || jogo.orientacao > 3){
        arquivo.close();
        return false;
    }

    // Le os movimentos
    if(!getline(arquivo, linha)){
        arquivo.close();
        return false;
    }
    stringstream conversaoMovimentos(linha);

    if(!(conversaoMovimentos>>jogo.movimentos)){
        arquivo.close();
        return false;
    }

    if(conversaoMovimentos>>linha){
        arquivo.close();
        return false;
    }

    if(jogo.movimentos < 0){
        arquivo.close();
        return false;
    }

    // Le as rotacoes
    if(!getline(arquivo, linha)){
        arquivo.close();
        return false;
    }
    stringstream conversaoRotacoes(linha);

    if(!(conversaoRotacoes>>jogo.rotacoes)){
        arquivo.close();
        return false;
    }

    if(conversaoRotacoes>>linha){
        arquivo.close();
        return false;
    }

    if(jogo.rotacoes < 0){
        arquivo.close();
        return false;
    }

    // Le a posicao do jogador
    if(!getline(arquivo, linha)){
        arquivo.close();
        return false;
    }

    char virgula;

    stringstream jogadorLinha(linha);
    if(!(jogadorLinha>>jogo.jogador.linha>>virgula>>jogo.jogador.coluna) || virgula != ','){
        arquivo.close();
        return false;
    }

    if(jogadorLinha>>linha){
        arquivo.close();
        return false;
    }

    // Le a posicao do bloco
    if(!getline(arquivo, linha)){
        arquivo.close();
        return false;
    }

    stringstream blocoLinha(linha);
    if(!(blocoLinha>>jogo.bloco.linha>>virgula>>jogo.bloco.coluna) || virgula != ','){
        arquivo.close();
        return false;
    }

    if(blocoLinha >> linha){
        arquivo.close();
        return false;
    }

    // Le a matriz do cenario
    for(int i = 0; i < jogo.n; i++){
        if(!getline(arquivo, linha)){
            arquivo.close();
            return false;
        }

        stringstream linhaCSV(linha);

        for(int j = 0; j < jogo.n; j++){
            if(!(linhaCSV >> jogo.cenario[i][j])){
                arquivo.close();
                return false;
            }

            if(j < jogo.n - 1){
                if(!(linhaCSV>>virgula) || virgula != ','){
                    arquivo.close();
                    return false;
                }
            }

            if(jogo.cenario[i][j] < 0 || jogo.cenario[i][j] > 7){
                arquivo.close();
                return false;
            }
        }

        // Impede colunas extras na linha
        string extra;
        if(linhaCSV>>extra){
            arquivo.close();
            return false;
        }
    }

    // Verifica se existem linhas extras no arquivo
    if(getline(arquivo, linha)){
        arquivo.close();
        return false;
    }

    arquivo.close();

    // Confere se as posicoes estao dentro do mapa
    if(jogo.jogador.linha < 0 || jogo.jogador.linha >= jogo.n || jogo.jogador.coluna < 0 || jogo.jogador.coluna >= jogo.n){
        return false;
    }

    if(jogo.cenario[jogo.jogador.linha][jogo.jogador.coluna] == 1){
        return false;
    }

    if(jogo.bloco.linha < -1 || jogo.bloco.linha >= jogo.n || jogo.bloco.coluna < -1 || jogo.bloco.coluna >= jogo.n){
        return false;
    }

    // O bloco pode nao existir na partida
    if((jogo.bloco.linha == -1 && jogo.bloco.coluna != -1) || (jogo.bloco.linha != -1 && jogo.bloco.coluna == -1)){
        return false;
    }

    // Verifica se jogador e bloco ocupam a mesma posicao
    if(jogo.bloco.linha != -1 && jogo.jogador.linha == jogo.bloco.linha && jogo.jogador.coluna == jogo.bloco.coluna){
        return false;
    }

    // Verifica se a posicao do jogador e permitida
    int tipoJogador = jogo.cenario[jogo.jogador.linha][jogo.jogador.coluna];

    if(tipoJogador == 6 || tipoJogador == 7){
        if(portaFechada(tipoJogador, jogo.orientacao)){
            return false;
        }
    }
    else if(tipoJogador != 0 && tipoJogador != 4 && tipoJogador != 5){
        return false;
    }

    bool temSaida = false;

    for(int i = 0; i < jogo.n;i++){
        for(int j = 0; j < jogo.n;j++){
            if(jogo.cenario[i][j] == 5){
                temSaida = true;
            }
        }
    }
    if(!temSaida){
        return false;
    }

    return true;
}

bool jogo(Jogo &jogo){
    char x;
    string erro;
    system("cls");

    while (true) {
        // coloca o cursor na posi��o inicial
        PosicionaCursor(0, 0);
        //(void)system("cls");

        cout<<"Mapa: "<<jogo.numeroMapa<<"      "<<endl;
        cout<<"Movimentos: "<<jogo.movimentos<<"      "<<endl;
        cout<<"Rotacoes: "<<jogo.rotacoes<<"      "<<endl;
        cout<<"Orientacao: "<<jogo.orientacao * 90<<" graus      "<<endl;

        mapalabirinto(jogo);

        // atualiza jogador
        x = getch(); // pega a tecla

        if(x == 27){
            if(salvarJogo(jogo)){
                cout<<"\nPartida salva com sucesso!"<<endl;
            }else{
                cout<<"\nErro ao salvar a partida!"<<endl;
            }

            getch();
            return false;
        }

        if(x == 'r'){
            if(!carregarMapa(jogo, jogo.numeroMapa, erro)){
                cout<<"Erro ao carregar o mapa: "<<erro<<endl;
                getch();
                return false;
            }

            continue;
        }

        bool continuar = processarTecla(x, jogo);

        if(!continuar){
            if(!carregarMapa(jogo, jogo.numeroMapa, erro)){
                cout<<"Erro ao carregar o mapa: "<<erro<<endl;
                getch();
                return false;
            }

            continue;
        }

        if(vitoria(jogo)){
            system("cls");

            cout<<"Mapa: "<<jogo.numeroMapa<<endl;
            cout<<"Movimentos: "<<jogo.movimentos<<endl;
            cout<<"Rotacoes: "<<jogo.rotacoes<<endl;
            cout<<"Orientacao: "<<jogo.orientacao * 90<<" graus"<<endl;

            mapalabirinto(jogo);

            cout<<endl<<endl;
            cout<<"Voce venceu!"<<endl;

            string nome;
            bool nomeInvalido;
            do{
                nomeInvalido = false;

                cout<<"Digite seu nome para o ranking (maximo 15 caracteres, sem virgulas): ";
                cin>>nome;

                if(nome.length() > 15){
                    nomeInvalido = true;
                }

                for(int i = 0;i < nome.length();i++){
                    if(nome[i] == ','){
                        nomeInvalido = true;
                    }
                }

                if(nomeInvalido){
                    cout<<"Nome invalido! Tente novamente."<<endl;
                }
            }while(nomeInvalido);

            registrarRecorde(nome, jogo.numeroMapa, jogo.movimentos, jogo.rotacoes);

            cout<<endl;
            cout<<"Recorde registrado!"<<endl;
            cout<<"Pressione uma tecla para continuar...";
            getch();

            apagarJogoSalvo();
            system("cls");

            return true;
        }
    }

    return false;
}

void sobre(){
    system("cls");

    cout<<"<><><><><><><><><><>"<<endl;
    cout<<"<>  SOBRE O JOGO  <>"<<endl;
    cout<<"<><><><><><><><><><>"<<endl<<endl;

    cout<<"Integrante: William da Silva Wagner"<<endl;
    cout<<"Disciplina: Algoritmos e Programacao 2"<<endl;
    cout<<"Professor: Thiago Felski Pereira"<<endl;
    cout<<"Setembro de 2026"<<endl<<endl;

    cout<<"| REGRAS |"<<endl;
    cout<<"W/A/S/D - Movimento"<<endl;
    cout<<"Q - Gira para a esquerda"<<endl;
    cout<<"E - Gira para a direita"<<endl;
    cout<<"R - Reinicia a fase"<<endl;
    cout<<"ESC - Volta ao menu"<<endl<<endl;

    cout<<"OBJETIVO: "<<endl;
    cout<<"Chegue ate a saida do labirinto."<<endl<<endl;

    cout<<"Pressione qualquer tecla para voltar..."<<endl;
    getch();
}

int main() {
    EscondeCursor();

    Jogo jogoAtual;

    bool partidaEmAndamento = false;

    // Verifica se existe uma partida salva
    if(carregarJogo(jogoAtual)){
        partidaEmAndamento = true;
    }

    while(true){
        int opc = menu();

        if(opc == 1){

            //Verifica se existe uma partida salva
            if(arquivoExiste("jogo_salvo.csv")){
                char confirmar;

                cout<<"Ja existe uma partida salva."<<endl;
                cout<<"Deseja iniciar um novo jogo e substitui-la? (s/n): ";
                cin>>confirmar;

                if(confirmar != 's' && confirmar != 'S'){
                    continue;
                }
            }

            int escolha = escolherMapa();
            int totalMapas = contarMapas();

            if(escolha == totalMapas + 1){
                escolha = sortearMapaValido();

                if(escolha == 0){
                    cout<<"Nenhum mapa valido disponivel"<<endl;
                    getch();
                    continue;
                }
            }

            else if(escolha < 1 || escolha > totalMapas){
                cout<<"Opcao invalida!"<<endl;
                getch();
                continue;
            }

            jogoAtual.numeroMapa = escolha;

            string erro;

            if(!carregarMapa(jogoAtual, jogoAtual.numeroMapa, erro)){
                cout<<" Erro ao carregar o mapa: "<<erro<<endl;
                getch();
                continue;
            }

            apagarJogoSalvo();

            partidaEmAndamento = true;

            if(jogo(jogoAtual)){
                partidaEmAndamento = false;
            }
        }
        else if(opc == 2){
            if(!partidaEmAndamento){
                if(carregarJogo(jogoAtual)){
                    partidaEmAndamento = true;
                }
            }

            if(partidaEmAndamento){
                if(jogo(jogoAtual)){
                    partidaEmAndamento = false;
                }
            }else{
                cout<<"Nao existe uma partida em andamento."<<endl;
                getch();
            }
        }
        else if(opc == 3){
            int escolha = escolherMapa();
            int totalMapas = contarMapas();

            if(escolha >= 1 && escolha <= totalMapas){
                verSolucao(escolha);
            }
            else if(escolha == totalMapas + 1){
                int mapaAleatorio = sortearMapaValido();

                if(mapaAleatorio == 0){
                    cout<<"Nenhum mapa valido disponivel!"<<endl;
                    getch();
                }else{
                verSolucao(mapaAleatorio);
                }
            }

        }
        else if(opc == 4){
            verRecordes();
        }
        else if(opc == 5){
            sobre();
        }
        else if(opc == 6){
            break;
        }
        else{
            cout<<"Opcao invalida"<<endl;
            getch();
        }
    }

    return 0;
}
