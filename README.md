# FlappyPDSII



- Flávia
    ## Classe `Player`

    Classe que representa o jogador do jogo.

    ### Construtor

    ```cpp
    Player(const std::string &filename, float x, float y, float speed, float width, float height);
    ```

    - `filename`: caminho da imagem do sprite sheet do jogador.
    - `x`, `y`: posição inicial do jogador.
    - `speed`: velocidade horizontal/vertical.
    - `width`, `height`: dimensões da hitbox do jogador.

    ### Atributos principais

    - `SpriteAnimado sprite`: classe reponsável pela animação do jogador.
    - `velY`: velocidade vertical (afetada pela gravidade).
    - `gravidade`: força gravitacional que acelera o jogador para baixo.
    - `IsAlive`: booleano que determina se o jogador está vivo.
    - `lastJumpTime`: último momento em que o jogador pulou.
    - `jumpCooldown`: tempo de espera entre pulos.

    ### Métodos

    - `void update(const ALLEGRO_KEYBOARD_STATE &key_state)`  
    Atualiza posição e física do jogador com base na gravidade e input do teclado.

    - `void draw()`  
    Desenha o frame atual da animação do jogador na tela.(Guarda a posição)

    - `void Kill()`  
    Seta o estado do jogador para morto, congelando ele.

    - `void onCollision()`  
    Mata o jogador e lança a exceção `PlayerException`.

    - `bool colisao(coordenadas p)`  
    Verifica se um ponto colide com a hitbox do jogador.

    - `bool getIsAlive() const`  
    Retorna se o jogador está vivo.

    ---

    ## Classe `SpriteAnimado`

    Classe responsável por controlar e desenhar uma animação 2D a partir de um spritesheet.

    ### Construtor

    ```cpp
    SpriteAnimado(const std::string& filename, int numFrames, int frameLargura, int frameAltura, float fps);
    ```

    - `filename`: caminho para o sprite sheet.
    - `numFrames`: número total de quadros no sprite sheet.
    - `frameLargura`, `frameAltura`: dimensões de cada quadro da animação.
    - `fps`: quantos quadros por segundo a animação deve exibir.

    ### Atributos principais

    - `bitmap`: imagem carregada do sprite sheet.
    - `frameAtual`: índice do frame atual da animação.
    - `tempoAcumulado`: tempo acumulado para trocar de frame.
    - `tempoPorFrame`: tempo que cada frame deve durar.

    ### Métodos

    - `void update(float deltaTime)`  
    Atualiza o frame atual com base no tempo passado(Contador de FPS).

    - `void draw(const coordenadas& pos) const`  
    Desenha o quadro atual na posição `pos`.

    - `~SpriteAnimado()`  
    Libera os recursos alocados pela imagem.



    ## Classe: `Sprite`

    Representa um sprite 2D com várias imagens (quadros), útil para animações.

    #### Métodos Públicos

    - `Sprite(std::string caminho, int largura, int altura, int espacamento = 0)`
    - **Construtor**: Carrega uma imagem com múltiplos quadros.
    - `caminho`: Caminho para o arquivo da imagem.
    - `largura`: Largura de cada quadro.
    - `altura`: Altura de cada quadro.
    - `espacamento`: Espaçamento entre os quadros (opcional).

    - `~Sprite()`
    - **Destrutor**: Libera os recursos de imagem.

    - `void desenhar(float x, float y, int indice = 0, bool flip = false) const`
    - Desenha o quadro indicado no ponto (x, y).
    - `indice`: Índice do quadro.
    - `flip`: Se verdadeiro, espelha horizontalmente.

    - `int get_num_quadros() const`
    - Retorna o número total de quadros.

    - `int get_largura() const`
    - Retorna a largura de um quadro.

    - `int get_altura() const`
    - Retorna a altura de um quadro.

    ---

    ## `player_exception.hpp`

    ### Classe: `PlayerException : public std::runtime_error`

    Classe de exceção personalizada para erros relacionados ao `Player`.

    #### Métodos Públicos

    - `PlayerException(const std::string& mensagem)`
    - Construtor que recebe uma mensagem de erro.

    ---


