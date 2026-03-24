# 👑 Problema das N-Rainhas em C++

Este projeto implementa uma solução para o clássico problema das **N-Rainhas**, utilizando a técnica de **backtracking** em C++.

## 📌 Descrição

O objetivo do problema é posicionar **N rainhas** em um tabuleiro de xadrez **N x N**, de forma que **nenhuma rainha ataque outra**.

Uma rainha pode atacar:
- Horizontalmente
- Verticalmente
- Diagonalmente

O programa recebe um valor **N** informado pelo usuário e exibe **todas as soluções possíveis**.

---

## 🧠 Estratégia Utilizada

A solução utiliza o algoritmo de **backtracking**, que funciona da seguinte forma:

1. Coloca uma rainha em uma coluna válida
2. Verifica se a posição é segura:
   - mesma linha ❌
   - diagonais ❌
3. Se for válida, avança para a próxima linha
4. Se não houver solução, **retrocede (backtrack)** e tenta outra posição

---

## ⚙️ Como Executar

### Pré-requisitos
- Compilador C++ (g++, clang, etc.)

### Compilação
```bash
g++ main.cpp -o nqueens
