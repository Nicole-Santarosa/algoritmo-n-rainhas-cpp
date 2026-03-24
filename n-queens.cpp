// =============================================================================
// Problema das N-Rainhas
//
// DESCRIÇÃO DO PROBLEMA:
//   Posicionar N rainhas em um tabuleiro N×N de forma que nenhuma rainha ataque
//   outra. Rainhas se atacam se estiverem na mesma linha, coluna ou diagonal.
//
// ESTRATÉGIA RECURSIVA (Backtracking):
//   A recursão percorre o tabuleiro linha por linha. Em cada linha, tenta
//   posicionar uma rainha em cada coluna. Se a posição for segura, posiciona
//   e chama recursivamente para a próxima linha.
//   Caso base: todas as N linhas foram preenchidas → solução encontrada. Se
//   nenhuma coluna da linha atual for segura, retorna (backtrack).
//
// COMPLEXIDADE (Notação O):
//   - Pior caso: O(N!) → em cada linha temos até N escolhas de coluna, mas
//     as restrições vão reduzindo as opções a cada nível.
//     Linha 0: N opções
//     Linha 1: N-1 opções (no máximo)
//     Linha 2: N-2 opções (no máximo)
//     ...
//     Total: N * (N-1) * (N-2) * ... * 1 = N!
//   - Verificação de conflito: O(1) por uso de arrays booleanos (sem esse
//     truque seria O(N) por verificação, piorando o desempenho).
//   - Impressão de cada solução: O(N²)
//
// OTIMIZAÇÃO IMPLEMENTADA:
//   Três arrays booleanos rastreiam em tempo O(1) se uma coluna ou diagonal
//   já está ocupada, evitando percorrer o tabuleiro inteiro a cada verificação.
// =============================================================================

#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>
#define NOMINMAX
#include <Windows.h>

using namespace std;
// =============================================================================
// VARIÁVEIS GLOBAIS DE CONTROLE
// =============================================================================

// colunaOcupada[c] = true indica que a coluna c já tem uma rainha
vector<bool> colunaOcupada;

// diagPrincipal[linha - coluna + N - 1]: diagonais no sentido (\)
// O deslocamento (+ N - 1) garante que o índice nunca seja negativo.
vector<bool> diagPrincipal;

// diagSecundaria[linha + coluna]: diagonais no sentido (/)
vector<bool> diagSecundaria;

// tabuleiro[linha] = coluna onde a rainha foi posicionada naquela linha
vector<int> tabuleiro;

// Contador de soluções encontradas
int totalSolucoes;

// =============================================================================
// FUNÇÃO: imprimirTabuleiro
// Imprime o estado atual do tabuleiro, com 'R' nas rainhas e '-' nas vazias.
// Complexidade: O(N²) → percorre todas as N linhas e N colunas.
// =============================================================================

void imprimirTabuleiro(int n) {
    cout << "\n--- Solução " << totalSolucoes << " ---\n";
    for (int linha = 0; linha < n; linha++) {
        for (int coluna = 0; coluna < n; coluna++) {
            // Se a rainha dessa linha está nessa coluna, imprime 'R', caso contrário '-'
            if (tabuleiro[linha] == coluna) {
                cout << "R ";
            }
            else {
                cout << "- ";
            }

            // Separa as colunas com espaços
            if (coluna < n - 1) {
                cout << " ";
            }
        }
        cout << "\n";
    }
}

// =============================================================================
// FUNÇÃO RECURSIVA: resolverRainhas
//
// PARÂMETROS:
//   linha → linha atual sendo processada (0 a N-1)
//   n     → tamanho do tabuleiro
//
// CASO BASE:
//   linha == n → todas as rainhas foram posicionadas com sucesso.
//   Incrementa o contador e imprime o tabuleiro.
//
// PASSO RECURSIVO:
//   Para cada coluna da linha atual:
//     1. Verifica em O(1) se a posição (linha, coluna) é segura.
//     2. Se segura: marca os arrays, salva a posição, chama para linha + 1.
//     3. Após retornar (backtrack): desmarca os arrays (desfaz a escolha).
// =============================================================================

void resolverRainhas(int linha, int n) {

    // Caso base: todas as rainhas foram posicionadas
    if (linha == n) {
        totalSolucoes++;
        imprimirTabuleiro(n);
        return; // Encerra esta chamada e retorna ao nivel anterior
    }

    // Passo recursivo: tenta colocar uma rainha em cada coluna da linha atual
    for (int coluna = 0; coluna < n; coluna++) {

        // Verifica conflito em 0(1), checa se a coluna está ocupada
        bool conflitoColuna = colunaOcupada[coluna];

        // Verifica conflito na diagonal principal (\), rainhas na mesma diagonal têm linha - coluna constante
        bool conflitoDiagPrincipal = diagPrincipal[linha - coluna + n - 1];

        // Verifica conflito na diagonal secundária (/), rainhas na mesma diagonal têm linha + coluna constante
        bool conflitoDiagSecundaria = diagSecundaria[linha + coluna];

        // Se qualquer conflito for verdadeiro, esta posição não é segura
        if (conflitoColuna || conflitoDiagPrincipal || conflitoDiagSecundaria)
            continue; // Tenta a próxima coluna

        // POSICIONA A RAINHA: marca os arrays e salva a posição
        tabuleiro[linha] = coluna; // Registra a escolha
        colunaOcupada[coluna] = true; // Marca a coluna como ocupada
        diagPrincipal[linha - coluna + n - 1] = true; // Marca a diagonal principal como ocupada
        diagSecundaria[linha + coluna] = true; // Marca a diagonal secundária como ocupada

        // Chamada recursiva para a próxima linha
        resolverRainhas(linha + 1, n);

        // BACKTRACK: desmarca os arrays para tentar a próxima coluna. Ao retornar da chamada recursiva, desfazemos a escolha para explorar outras possibilidades.
        colunaOcupada[coluna] = false; // Desmarca a coluna
        diagPrincipal[linha - coluna + n - 1] = false; // Desmarca a diagonal principal
        diagSecundaria[linha + coluna] = false; // Desmarca a diagonal secundária
        // Não é necessário limpar tabuleiro[linha] porque ele será sobrescrito na próxima iteração ou não será lido novamente se voltarmos para níveis anteriores.
    }
    // Se nenhuma coluna funcionou: retorna sem fazer nada.
}

// =============================================================================
// FUNÇÃO: validarEntrada
// Lê e valida o valor de N informado pelo usuário.
// Trata: entrada não-inteira, N <= 0, e N excessivamente grande (> 15),
// pois N = 15 já pode gerar milhões de soluções e demorar bastante.
// Retorna o N validado.
// =============================================================================

int validarEntrada() {
    int n;

    while (true) {
        cout << "\nDigite o valor de N (tamanho do tabuleiro): ";

        // Tenta ler um inteiro
        if (!(cin >> n)) {
            // Entrada inválida (ex: letras)
            cin.clear(); // Limpa o estado de erro do cin
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta a linha de entrada

            cerr << "Erro: entrada inválida. Por favor, digite um número inteiro positivo.\n" << endl;
            continue; // Tenta novamente
        }

        // Verifica os limites
        if (n <= 0) {
            cerr << "Erro: N deve ser um número maior que 0.\n" << endl;
            continue; // Tenta novamente
        }

		// Alerta para N > 15, pois o número de soluções cresce exponencialmente e pode demorar para processar
        if (n > 15) {
            char resposta;

            cout << "Aviso: N > 15 pode gerar muitas soluções e pode demorar para processar.\n";
            cout << "Deseja continuar? (s/n): ";

            cin >> resposta;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (resposta == 's' || resposta == 'S') {
                return n; // Aceita o valor
            }
            else {
                continue; // Volta e pede outro N
            }
        }

        // Entrada válida
        return n;
    }
}

// =============================================================================
// FUNÇÃO PRINCIPAL
// =============================================================================

int main() {
    SetConsoleOutputCP(CP_UTF8);

    cout << "========= Problema das N-Rainhas =========\n" << endl;

    // Lê e valida o valor de N
    int n = validarEntrada();

    // Inicializa as estruturas de controle
    colunaOcupada.assign(n, false);
    diagPrincipal.assign(2 * n - 1, false);
    diagSecundaria.assign(2 * n - 1, false);
    tabuleiro.assign(n, -1); // -1 indica que nenhuma rainha foi posicionada na linha
    totalSolucoes = 0;

    cout << "\nBuscando soluções para " << n << " rainhas..." << endl;

    // Inicializa a recursão a partir da primeira linha
    resolverRainhas(0, n);

    // Resultado final
    if (totalSolucoes == 0) {
        cout << "\nNenhuma solução encontrada para " << n << " rainhas." << endl;
    }
    else {
        cout << "\nTotal de soluções encontradas: " << totalSolucoes << endl;
    }

    return 0;
}

// =============================================================================
// RESUMO DA ANÁLISE DE COMPLEXIDADE
//
// Tempo → Pior caso: O(N!)
//   Cada nível da recursão representa uma linha do tabuleiro. No pior caso,
//   exploramos todas as permutações de colunas:
//   N * (N-1) * (N-2) * ... * 1 = N! chamadas recursivas.
//   As verificações de conflito são O(1) graças aos arrays booleanos. Sem essa
//   otimização, cada verificação seria O(N), elevando o custo para O(N * N!) no
//   pior caso.
//
// Espaço → O(N):
//   O tabuleiro usa O(N) (uma posição por linha).
//   Os três arrays booleanos usam O(N) cada.
//   A pilha de recursão tem profundidade máxima N → O(N).
//   Total: O(N).
//
// Nota: O(N!) é o limite teórico inferior para este problema, pois qualquer
// algoritmo que enumere TODAS as soluções deve visitar pelo menos uma vez cada
// solução válida, e o número de soluções cresce aproximadamente como O(N!).
// =============================================================================