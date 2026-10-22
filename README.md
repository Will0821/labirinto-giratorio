Labirinto Giratório — M2
Trabalho de Algoritmos e Programação 2

Curso: Ciência da Computação
Instituição: UNIVALI - Itajaí
Integrante: William da Silva Wagner
Versão: M2 - 2026
Professor: Thiago Felski Pereira
Desenvolvido em Windows

| Sobre o projeto |

O Labirinto Giratório é um jogo desenvolvido em C++ no qual o jogador precisa encontrar a saída de diferentes labirintos. Durante a partida, é necessário movimentar o personagem, utilizar alavancas, evitar obstáculos e rotacionar o cenário para modificar a orientação do labirinto e a ação da gravidade.

Na M2, o projeto foi reorganizado utilizando estruturas (struct), leitura de mapas a partir de arquivos, salvamento de partidas, reprodução de soluções e sistema de recordes.

| Como executar |

Baixe ou clone este repositório.
Abra o arquivo do projeto .cbp no Code::Blocks.
Compile o projeto.
Execute o programa.

Mantenha os arquivos dos mapas, das soluções e os demais arquivos necessários na pasta utilizada pelo programa, respeitando os nomes esperados pelo código.

| Controles |

W - Movimentar para cima
A -Movimentar para a esquerda
S -Movimentar para baixo
D -Movimentar para a direita
Q -Rotacionar o cenário no sentido anti-horário
E -Rotacionar o cenário no sentido horário
R - Recarregar o mapa, conforme implementado no jogo
ESC - Sair da partida atual e salvar o progresso

| Regras do jogo |

O objetivo é conduzir o personagem até a saída do labirinto.
As paredes bloqueiam a movimentação.
O bloco sofre ação da gravidade e pode modificar o caminho disponível.
As alavancas permitem interagir com a rotação do cenário conforme as regras do jogo.
As portas mudam entre os estados aberto e fechado conforme a orientação do cenário.
O jogador deve considerar a posição dos obstáculos, das portas e da saída antes de realizar seus movimentos.
O jogo contabiliza movimentos e rotações durante a partida.

| Arquivos dos mapas |

Os mapas são armazenados em arquivos separados, seguindo o padrão mapaN.csv, em que N representa o número do mapa.
Cada arquivo possui:

Primeira linha: dimensão do mapa.
Segunda linha: título do mapa.
Linhas seguintes: matriz do cenário, com valores separados por vírgulas.

Códigos utilizados na matriz

0 - Espaço vazio
1 - Parede
2 - Posição inicial do jogador
3 - Posição inicial do bloco
4 - Alavanca
5 - Saída
6 - Porta do tipo A
7 - Porta do tipo B

O programa verifica a estrutura e os valores dos mapas antes de utilizá-los, incluindo suas dimensões, a quantidade de jogadores e a presença da saída.

O projeto contém cinco mapas, incluindo mapas maiores que o tamanho utilizado na M1.

| Arquivos de solução |

Cada mapa possui um arquivo de solução com o padrão mapaN.sol.
O arquivo contém uma sequência de teclas que representa uma tentativa de completar o respectivo mapa.

| Salvamento da partida |

O progresso da partida é armazenado no arquivo jogo_salvo.csv.
O salvamento registra informações necessárias para continuar a partida, como:

Número e título do mapa.
Dimensão e orientação do cenário.
Posição do jogador e do bloco.
Quantidade de movimentos e rotações.
Estado atual da matriz.

Ao selecionar a opção de continuar, o programa recupera o estado salvo, desde que o arquivo seja válido. Ao vencer, o salvamento da partida é invalidado.

| Sistema de recordes |

Os recordes são armazenados em recordes.csv, com os campos:
nome,mapa,movimentos,rotacoes

Ao concluir um mapa, o jogador informa seu nome, com limite de 15 caracteres e sem vírgulas.
Os resultados são ordenados por:

Número do mapa.
Menor quantidade de movimentos.
Menor quantidade de rotações em caso de empate nos movimentos.

O ranking mantém os cinco melhores resultados por mapa.

| Comparação entre M1 e M2 |

A M2 reorganiza partes do código da M1 para facilitar a manutenção e permitir novas funcionalidades.

Estado do jogo
M1 - Informações mantidas em variáveis e parâmetros separados
M2 - informações organizadas na estrutura Jogo

Mapas
M1 - Mapas definidos diretamente no código
M2 - Mapas carregados de arquivos CSV

Soluções
M1 - Sequências de teclas documentadas
M2 - Soluções armazenadas em arquivos .sol e reproduzidas pelo programa

Salvamento
M1 - Continuação disponível apenas enquanto o programa permanece aberto
M2 - Estado da partida salvo em arquivo

Recordes
M1 - Não existia
M2 - Resultados registrados e ordenados por mapa

Validação
M1 - Estrutura original da M1
M2 - Verificação da estrutura e dos valores dos arquivos de mapa

| Alterações nas funções |

Função Movimentação do jogador

M1 - bool moveplayer(char tecla, int &px, int &py, int m[11][11], int bx, int by, int orientacao){
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

M2 - bool moveplayer(char tecla, Jogo &jogo){
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

Função Gravidade

M1 - void gravidade(int m[11][11], int &bx, int &by, int px, int py, int orientacao){

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

M2 - void gravidade(Jogo &jogo){

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

Função Rotação do cenário

M1 - void rotacionar(char tecla, int &orientacao, int&rotacoes){
    if(tecla == 'e'){
        orientacao = (orientacao + 90) % 360;
        rotacoes++;
    }
    if(tecla == 'q'){
        orientacao = (orientacao - 90 + 360) %360;
        rotacoes++;
    }
}

M2 - void rotacionar(char tecla, Jogo &jogo){
    if(tecla == 'e'){
        jogo.orientacao = (jogo.orientacao + 1) % 4;
        jogo.rotacoes++;
    }
    if(tecla == 'q'){
        jogo.orientacao = (jogo.orientacao - 1 + 4) % 4;
        jogo.rotacoes++;
    }
}

Função Carregar mapa

M1 - void carregarMapa(int numeroMapa, int m[11][11], int &px, int &py, int &bx, int &by, int &orientacao, int &movimentos, int &rotacoes){

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

M2 - bool carregarMapa(Jogo &jogo, int numeroMapa, string &erro){
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

Função Jogo

M1 - bool jogo(int m[11][11], int &px, int &py, int &bx, int &by, int &orientacao, int &movimentos, int &rotacoes, int numeroMapa){
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

M2 - bool jogo(Jogo &jogo){
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

| Defeitos corrigidos e decisões de implementação |

Durante o desenvolvimento da M2, o código foi adaptado para utilizar a estrutura Jogo, carregar mapas externos e persistir o progresso em arquivos. Também foram tratados problemas relacionados à leitura dos mapas, ao carregamento do estado salvo e à atualização da tela durante a partida.

Defeitos corrigidos:

Jogador chegando na saída sem tocar nela
sequência de teclas no README incorretas

| Histórico de versões |

M1: versão inicial do Labirinto Giratório.
M2: reorganização do código, carregamento externo dos mapas, validação de arquivos, reprodução de soluções, salvamento de partidas e sistema de recordes.

| Guia das fases |

Mapa 1
Sequência de teclas:

D, S, S, A, S, S, D, S, S, A, S, S, D, D, D, W, W, W, W, D, D, D, W, W, A, A, A, W, W, D, D, D, D, D, S, S, S, S, S, A, Q, Q, Q, A, W, W, A, A, S, S, S.

Mapa 2
Sequência de teclas:

S, E, E, Q, Q, Q, E, Q, D, W, W, A, A, W, W, W, W, W, W, E, A, S, S, A, S, S, S, S, S, S, D, Q, S, S, S, A, A, S, S, S, D, E, A, W, W, W, D, D, D, D, W, W

Mapa 3 
Sequência de teclas:

D, D, D, S, S, A, A, A, S, S, D, D, D, S, S, A, S, S, A, Q, E, E, Q, Q, E, E, Q, Q, Q, A, A, A, A, A, A, A, S, S, D, D, D, S, S, A, A, A, S, S, D, D, D, S, S, A, A, A.

Mapa 4
Sequência de teclas:

S, E, A, A, W, W, D, D, D, W, W, W, W, A, A, W, W, W, W, W, W, A, A, A, S, A, A, W, A, A, A, S, S, S, S, S, S, S, S, S, S, A, A, W,
W, W, W, W, W, W, W, W, W.

Mapa 5
Sequencia de teclas:

S, S, S, S, D, D, D, D, D, D, D, W, W, D, D, W, W, A, A, A, W, W, A, W, W, W, A, W, W, D, W, D, D, S, S, D, D, E, A, A, S, Q, S, S.
