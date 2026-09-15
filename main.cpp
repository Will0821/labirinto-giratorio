/*
  Código criado por Thiago Felski Pereira
  Move um caractere na matriz sem testes
  Útil para implementação de jogos
  com movimentação de personagem.
  (Típicos de algoritmos 1)

  VERSÃO WINDOWS (usa a API do console, sem códigos de escape)

  Compilação (MinGW/g++):  g++ movimenta_matriz_windows.cpp -o jogo.exe
*/
#include <iostream>
#include <conio.h>   // _getch()
#include <windows.h> // funções de cursor e cor do console
using namespace std;

int getch(void) {
  return _getch(); // o conio.h já lê a tecla sem ENTER e sem mostrar na tela
}

// Coloca o cursor de escrita na linha/coluna indicada.
// Obs: a função SetCursorPos do windows.h NÃO serve aqui,
// ela move o mouse. A do console é a SetConsoleCursorPosition.
void PosicionaCursor(int XPos, int YPos) {
  COORD pos;
  pos.X = XPos;
  pos.Y = YPos;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Esconde o traço piscante do cursor, para não atrapalhar o desenho
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

bool portaFechada(int tipoPorta, int orientacao){
    if(tipoPorta == 6){
        return orientacao == 0 || orientacao == 180;
    }
    if(tipoPorta == 7){
        return orientacao == 90 || orientacao == 270;
    }
    return false;
}

bool podemover(int novoX, int novoY, int m[11][11], int bx, int by, int orientacao){

    if(novoX == bx && novoY == by){
        return false;
    }

    if(m[novoX][novoY] == 0 || m[novoX][novoY] == 4 || m[novoX][novoY] == 5){
        return true;
    }
    if(m[novoX][novoY] == 6 || m[novoX][novoY] == 7){
        return !portaFechada(m[novoX][novoY], orientacao);
    }
    return false;
}

bool moveplayer(char tecla, int &px, int &py, int m[11][11], int bx, int by, int orientacao){
    switch(tecla){

    case 'w':
        if(podemover(px - 1, py, m, bx, by, orientacao)){
            px = px - 1;
            return true;
        }
      break;

    case 's':
        if(podemover(px + 1, py, m, bx, by, orientacao)){
            px = px + 1;
            return true;
        }
      break;

    case 'a':
        if(podemover(px, py - 1, m, bx, by, orientacao)){
            py = py - 1;
            return true;
        }
      break;

    case 'd':
        if(podemover(px, py + 1, m, bx, by, orientacao)){
            py = py + 1;
            return true;
        }
      break;
    }
    return false;
}

void mapalabirinto(int m[11][11], int px, int py, int bx, int by, int orientacao){
    for(int i = 0;i < 11;i++){
        for(int j = 0;j < 11;j++){
                if(i == px && j == py){
                    cout<<"@";
                }
                else if(i == bx && j == by){
                    cout<<"O";
                }else{
                    switch(m[i][j]){

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
                        if(portaFechada(6, orientacao)){
                            cout<<"=";
                        }else{
                            cout<<":";
                        }
                        break;

                    case 7:
                        if(portaFechada(7, orientacao)){
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

bool vitoria(int px, int py, int m[11][11]){
    return m[px][py] == 5;
}

bool alavancaPlayerEmcima(int px, int py, int m[11][11]){
    return m[px][py] == 4;
}

void rotacionar(char tecla, int &orientacao, int&rotacoes){
    if(tecla == 'e'){
        orientacao = (orientacao + 90) % 360;
        rotacoes++;
    }
    if(tecla == 'q'){
        orientacao = (orientacao - 90 + 360) %360;
        rotacoes++;
    }
}

void girarDireita(int m[11][11]){
    int aux[11][11];

    for(int i = 0;i < 11;i++){
        for(int j = 0;j < 11;j++){
            aux[j][10 - i] = m[i][j];
        }
    }

    for(int i = 0;i < 11;i++){
        for(int j = 0;j < 11;j++){
            m[i][j] = aux[i][j];
        }
    }
}

void girarPlayerDireita(int &px, int &py){

    int novaX = py;
    int novaY = 10 - px;

    px = novaX;
    py = novaY;
}

void girarBlocoDireita(int &bx, int &by){
    if(bx == -1 && by == -1){
        return;
    }

    int novaX = by;
    int novaY = 10 - bx;

    bx = novaX;
    by = novaY;
}

void girarEsquerda(int m[11][11]) {
    int aux[11][11];

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            aux[10 - j][i] = m[i][j];
        }
    }

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            m[i][j] = aux[i][j];
        }
    }
}

void girarPlayerEsquerda(int &px, int &py){

    int novaX = 10 - py;
    int novaY = px;

    px = novaX;
    py = novaY;
}

void girarBlocoEsquerda(int &bx, int &by){
    if(bx == -1 && by == -1){
        return;
    }

    int novaX = 10 - by;
    int novaY = bx;

    bx = novaX;
    by = novaY;
}

void gravidade(int m[11][11], int &bx, int &by, int px, int py, int orientacao){

    if(bx == -1 && by == -1){
        return;
    }

    while(true){
        int baixo = bx + 1;

        if(baixo == px && by == py){
            break;
        }
        if(baixo >= 10){
            break;
        }
        if(m[baixo][by] == 0){
            bx = baixo;
        }
        else if((m[baixo][by] == 6 || m[baixo][by] == 7) && !portaFechada(m[baixo][by], orientacao)){
            bx = baixo;
        }else{
            break;
        }
    }
}

void esmagarBloco(int m[11][11], int &bx, int &by, int orientacao){

    if(bx == -1 && by == -1){
        return;
    }

    if(m[bx][by] == 6 || m[bx][by] == 7){
        if(portaFechada(m[bx][by], orientacao)){
            bx = -1;
            by = -1;
        }
    }
}

bool esmagarPlayer(int px, int py, int m[11][11], int orientacao){
    if(m[px][py] == 6 || m[px][py] == 7){
        if(portaFechada(m[px][py], orientacao)){
            return true;
        }
    }
    return false;
}

void carregarMapa(int numeroMapa, int m[11][11], int &px, int &py, int &bx, int &by, int &orientacao, int &movimentos, int &rotacoes){

    if(numeroMapa == 1){

        /* 0 = Vazio, 1 = Parede fixa, 4 = Alavanca, 5 = Saida,
        6 = Porta tipo A, 7 = Porta tipo B */

        int mapa[11][11] = {
            {1,1,1,1,1,1,1,1,1,1,1},
            {1,0,4,1,0,0,0,0,0,0,1},
            {1,1,0,1,0,1,1,1,1,0,1},
            {1,7,0,1,0,0,0,0,1,0,1},
            {1,0,1,1,1,1,1,0,1,0,1},
            {1,0,0,1,0,0,0,0,1,0,1},
            {1,1,0,1,0,1,1,1,4,0,1},
            {1,0,0,1,0,1,0,0,0,0,1},
            {1,0,1,1,0,1,0,1,1,1,1},
            {1,0,0,0,0,1,0,6,0,5,1},
            {1,1,1,1,1,1,1,1,1,1,1}
        };

        for(int i = 0;i < 11;i++){
            for(int j = 0;j < 11;j++){
                m[i][j] = mapa[i][j];
            }
        }
        /* px = linha do jogador, py = coluna do jogador
        bx = linha do bloco, by = coluna do bloco */

        px = 1, py = 1;
        bx = 9, by = 8;
        orientacao = 0, movimentos = 0, rotacoes = 0;
    }

    if(numeroMapa == 2){

        int mapa[11][11] = {
            {1,1,1,1,1,1,1,1,1,1,1},
            {1,0,1,0,6,0,0,0,0,4,1},
            {1,4,1,0,1,1,1,1,0,1,1},
            {1,0,0,0,1,5,1,0,0,1,1},
            {1,1,1,1,1,0,1,7,1,1,1},
            {1,0,7,7,7,0,1,0,1,1,1},
            {1,0,1,1,1,1,1,0,1,1,1},
            {1,0,0,0,0,0,1,0,1,1,1},
            {1,0,4,1,1,0,1,7,1,1,1},
            {1,7,0,6,0,0,6,0,4,1,1},
            {1,1,1,1,1,1,1,1,1,1,1}
        };

        for(int i = 0;i < 11;i++){
            for(int j = 0;j < 11;j++){
                m[i][j] = mapa[i][j];
            }
        }
        px = 1; py = 1;
        bx = 3; by = 1;
        orientacao = 0, movimentos = 0, rotacoes = 0;
    }

    if(numeroMapa == 3){

        int mapa[11][11] = {
            {1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,1,0,0,0,5,1},
            {1,1,1,1,0,1,7,1,1,1,1},
            {1,0,0,0,0,1,0,0,0,0,1},
            {1,0,1,1,1,1,1,1,1,7,1},
            {1,0,0,0,0,1,0,0,0,0,1},
            {1,1,1,1,0,1,7,1,1,1,1},
            {1,1,1,0,0,1,0,0,0,0,1},
            {1,1,1,0,1,1,1,1,1,7,1},
            {1,1,4,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1}
        };

        for(int i = 0;i < 11;i++){
            for(int j = 0;j < 11;j++){
                m[i][j] = mapa[i][j];
            }
        }
        px = 1; py = 1;
        bx = 1; by = 8;
        orientacao = 0, movimentos = 0, rotacoes = 0;
    }
}

int escolherMapa(){
    int opc;
    system("cls");

    cout<<"============="<<endl;
    cout<<"  NOVO JOGO  "<<endl;
    cout<<"============="<<endl<<endl;

    cout<<"1 - Mapa 1"<<endl;
    cout<<"2 - Mapa 2"<<endl;
    cout<<"3 - Mapa 3"<<endl;
    cout<<"4 - Mapa aleatorio"<<endl<<endl;

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
    cout<<"3 - Sobre"<<endl;
    cout<<"4 - Fim"<<endl<<endl;

    cout<<"Digite uma opcao: ";
    cin>>opc;

    return opc;
}

bool jogo(int m[11][11], int &px, int &py, int &bx, int &by, int &orientacao, int &movimentos, int &rotacoes, int numeroMapa){
    char x;
    system("cls");

    while (true) {

        cout<<"Mapa: "<<numeroMapa<<"      "<<endl;
        cout<<"Movimentos: "<<movimentos<<"     "<<endl;
        cout<<"Rotacoes: "<<rotacoes<<"     "<<endl;
        cout<<"Orientacao: "<<orientacao<<" graus   "<<endl;

        mapalabirinto(m, px, py, bx, by, orientacao);

        // atualiza jogador
        x = getch(); // pega a tecla

        bool girou = false;

        if(x == 27){
            return false;
        }

        if(x == 'r'){
            carregarMapa(numeroMapa, m, px, py, bx, by, orientacao, movimentos, rotacoes);
        }

        if(x == 'e' && alavancaPlayerEmcima(px, py, m)){
            rotacionar(x, orientacao, rotacoes);
            girarDireita(m);
            girarPlayerDireita(px, py);
            girarBlocoDireita(bx, by);

            girou = true;
        }
        if(x == 'q' && alavancaPlayerEmcima(px, py, m)){
            rotacionar(x, orientacao, rotacoes);
            girarEsquerda(m);
            girarPlayerEsquerda(px, py);
            girarBlocoEsquerda(bx, by);

            girou = true;
        }

        if(moveplayer(x, px, py, m, bx, by, orientacao)){
            movimentos++;
        }

        if(girou){
            esmagarBloco(m, bx, by, orientacao);

            if(esmagarPlayer(px, py, m, orientacao)){
                carregarMapa(numeroMapa, m, px, py, bx, by, orientacao, movimentos, rotacoes);
            }

        gravidade(m, bx, by, px ,py, orientacao);
        }

        if(vitoria(px, py, m)){
            cout<<"Voce venceu!";
            getch();
            return true;
        }
        // coloca o cursor na posição inicial
        PosicionaCursor(0, 0);
        //(void)system("cls");
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

    //Para fazer alteracoes no labirinto mude a funcao carregarMapa.

    int px = 1, py = 1;
    int bx = 1, by = 1;
    int movimentos = 0, rotacoes = 0, orientacao = 0;
    bool partidaEmAndamento = false;
    int numeroMapa;

    int m[11][11];

    while(true){
        int opc = menu();

        if(opc == 1){
            int escolha = escolherMapa();

            if(escolha == 4){
                escolha = 1 + rand() % 3;
            }

            numeroMapa = escolha;

            carregarMapa(escolha, m, px, py, bx, by, orientacao, movimentos, rotacoes);

            partidaEmAndamento = true;

            if(jogo(m, px, py, bx, by, orientacao, movimentos, rotacoes, numeroMapa)){
                partidaEmAndamento = false;
            }
        }
        else if(opc == 2){
            if(partidaEmAndamento){
                if(jogo(m, px, py, bx, by, orientacao, movimentos, rotacoes, numeroMapa)){
                    partidaEmAndamento = false;
                }
            }else{
                cout<<"Nao existe uma partida em andamento."<<endl;
                getch();
            }
        }
        else if(opc == 3){
            sobre();
        }
        else if(opc == 4){
            break;
        }
    }

    return 0;
}
