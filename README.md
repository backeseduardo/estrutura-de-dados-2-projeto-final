# Analisador de Árvore de Diretórios em C++

## Descrição
Este projeto foi desenvolvido para a disciplina de **Estrutura de Dados 2**  do curso de Ciencias da Computação da Universidade de Passo Fundo UPF e consiste em um analisador de estrutura de diretórios que permite **visualizar e pesquisar informações sobre arquivos e pastas de forma hierárquica**, tanto no terminal quanto exportando para HTML.

---

- **Alunos:**  
  - Eduardo Backes
  - João Vitor Althaus Godoi    
  - Rafael de Almeida Maurina

---

## Funcionalidades Principais

### 1 Visualização de Árvore de Diretórios
- Exibe a estrutura completa de diretórios com:
  -  Indentação hierárquica
  -  Informações dos arquivos (nome e tamanho em bytes)
  -  Informações das pastas (nome, quantidade de itens diretos e tamanho total acumulado, considerando subpastas)

### 2 Exportação para HTML
- Gera um arquivo `.html` contendo:
  -  Representação visual da árvore de diretórios
  -  As mesmas informações exibidas no terminal

### 3 Ferramentas de Pesquisa
-  **Localizar o maior arquivo** (mostrando caminho completo e tamanho)  
-  **Listar arquivos maiores que N bytes** (solicita valor N ao usuário)  
-  **Identificar a pasta com mais arquivos diretos**  
-  **Filtrar arquivos por extensão**  
-  **Listar pastas vazias** (sem arquivos nem subpastas)  

---

## Requisitos do Sistema
- Compilador compatível com **C++17** (ex.: `g++`)
- Sistema operacional **Linux** ou **Windows com WSL**
- Bibliotecas padrão C++ utilizadas:
  - `<filesystem>`
  - `<iostream>`
  - `<fstream>`

---

### Executar o projeto

Versão mínima g++ 11.4.0

Abra o terminal na pasta raiz do projeto e execute:

```sh
g++ src/main.cpp
```

O executável `a.out` será criado na raiz do projeto.

Para executar o programa:

```sh
./a.out
```

ou passando o caminho de algum diretório como argumento:

```sh
./a.out ../
```
