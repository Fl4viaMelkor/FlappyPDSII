# FlappyPDSII

Projeto acadêmico desenvolvido em C++ utilizando a biblioteca gráfica Allegro 5, como parte da disciplina de Programação e Desenvolvimento de Software II, da Universidade Federal de Minas Gerais(UFMG).

## Introdução

O objetivo do projeto foi implementar uma versão funcional do clássico jogo *Flappy Bird*, explorando conceitos de orientação a objetos, modularização e manipulação de gráficos em tempo real com Allegro. O jogo simula a movimentação de um pássaro que deve evitar colisões com obstáculos para acumular pontos, com foco em estruturação modular, detecção de colisões e animações.

## Visão geral da solução

O projeto está estruturado em diferentes módulos, organizados nas seguintes pastas, como requisitado:

- `src/` e `include/`: contêm a lógica principal do jogo dividida por responsabilidade (controle, física, interface, dados).
- `assets/`: reúne os recursos gráficos e sonoros utilizados.
- `tests/`: abriga os testes unitários desenvolvidos com a biblioteca `doctest`.
- `docs/`: contém documentação gerada automaticamente via Doxygen, além de documentos auxiliares.

A lógica do jogo é dividida em camadas (como interface, controle e física) e permite reutilização e fácil manutenção de código. Foram utilizados padrões como Strategy e State, além de banco de dados com SQLite para registrar pontuações.

## Dificuldades encontradas

As principais dificuldades estiveram relacionadas à divisão de tarefas entre membros do grupo, garantindo integração adequada entre os módulos desenvolvidos separadamente. Além disso, mesmo sendo bastante útil, a biblioteca Allegro 5 exige certo improviso para suprir funcionalidades que seriam mais diretas com o uso de uma engine dedicada para jogos. Também surgiram desafios na correção de bugs lógicos relacionados à movimentação, detecção de colisão e sincronização dos estados do jogo.

## Extras implementados

- (espaço reservado para listar os extras do grupo)

## Compilação

Pré-requisitos:

- CMake
- Allegro 5
- g++

Para compilar:

```bash
mkdir build
cd build
cmake ..
make
./bin/FlappyPDSII
````

## Testes

Os testes usam a biblioteca [doctest](https://github.com/doctest/doctest). Para executar:

```bash
cd tests/build
make
./run_tests
```

## Documentação

A documentação pode ser gerada com [Doxygen](https://www.doxygen.nl/):

```bash
doxygen Doxyfile
```

Abra `docs/html/index.html` no navegador para consultar.

## Contribuidores

* Flávia Silva ([Fl4viaMelkor](https://github.com/Fl4viaMelkor))
* \[Demais nomes do grupo aqui]

## Licença

Projeto acadêmico desenvolvido exclusivamente para fins educacionais.
