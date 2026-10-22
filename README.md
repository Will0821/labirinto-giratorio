Labirinto Giratório — M2
Trabalho de Algoritmos e Programação 2

Curso: Ciência da Computação
Instituição: UNIVALI - Itajaí
Integrante: William da Silva Wagner
Versão: M2 - 2026
Professor: Thiago Felski Pereira

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

Movimentação do jogador

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

Gravidade

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

Rotação do cenário

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

Defeitos corrigidos e decisões de implementação

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
