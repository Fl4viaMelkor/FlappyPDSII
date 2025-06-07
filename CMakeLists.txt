cmake_minimum_required(VERSION 3.10) # Versão mínima do CMake. 3.31 é muito recente e pode não ser comum.
# Mantenho 3.10 para compatibilidade, mas você pode usar 3.31 se necessário.

project(FlappyPDSII CXX) # Define o nome do projeto e a linguagem C++

# Define a versão do padrão C++ a ser usado (C++17 é bom para <filesystem> e outras features)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Exige que o compilador suporte C++17
set(CMAKE_CXX_EXTENSIONS OFF)      # Desativa extensões GNU para um C++ mais padrão


# --- Início da Localização Manual do Allegro 5 ---
# Estas variáveis serão preenchidas pelos comandos find_path e find_library
set(ALLEGRO_FOUND FALSE) # Inicializa como falso, será true se tudo for encontrado

# 1. Localizar o diretório de inclusão principal do Allegro
find_path(ALLEGRO_INCLUDE_DIR
        NAMES allegro5/allegro.h # Nome de um cabeçalho conhecido
        HINTS ENV ALLEGRO_DIR # Sugere usar a variável de ambiente ALLEGRO_DIR
        PATHS ${CMAKE_PREFIX_PATH}/include # Procura no prefixo do MSYS2/MinGW
        DOC "Path to Allegro 5 include directory"
)

# 2. Localizar as bibliotecas Allegro individualmente
# Nomes das bibliotecas Allegro no sistema de arquivos (sem 'lib' e '.a/.dll')
# Para MinGW, os nomes são geralmente 'allegro-5.2', 'allegro_primitives-5.2', etc.
# Ou apenas 'allegro', 'allegro_primitives' dependendo da instalação.
# Vamos tentar os nomes curtos e também os longos com a versão se os curtos falharem.
# CMake geralmente adiciona o prefixo 'lib' e a extensão '.a' ou '.dll' automaticamente.

# Vamos usar os nomes curtos que geralmente funcionam com target_link_libraries
# se a pasta 'lib' estiver no PATH ou CMAKE_PREFIX_PATH.

# Bibliotecas principais que estamos usando
set(ALLEGRO_COMPONENTS_TO_FIND
        allegro
        allegro_primitives
        allegro_font
        allegro_ttf
        allegro_image
        allegro_audio
        allegro_acodec
        allegro_physfs
        allegro_dialog
        allegro_memfile
        # allegro_main # Opcional, se precisar do WinMain
)

set(ALLEGRO_LIBRARIES "") # Lista vazia para acumular as bibliotecas encontradas

foreach (COMPONENT ${ALLEGRO_COMPONENTS_TO_FIND})
    find_library(ALLEGRO_LIB_${COMPONENT}
            NAMES ${COMPONENT} # Tenta o nome curto (ex: 'allegro_primitives')
            HINTS ENV ALLEGRO_DIR
            PATHS ${CMAKE_PREFIX_PATH}/lib # Procura no prefixo do MSYS2/MinGW
            DOC "Path to Allegro 5 ${COMPONENT} library"
    )
    if (ALLEGRO_LIB_${COMPONENT})
        list(APPEND ALLEGRO_LIBRARIES ${ALLEGRO_LIB_${COMPONENT}})
        message(STATUS "Encontrado Allegro ${COMPONENT}: ${ALLEGRO_LIB_${COMPONENT}}")
    else ()
        message(FATAL_ERROR "Allegro ${COMPONENT} não encontrado. Verifique sua instalação.")
    endif ()
endforeach ()

# Verificar se todos os componentes essenciais foram encontrados
if (ALLEGRO_INCLUDE_DIR AND ALLEGRO_LIBRARIES) # AND todas as variáveis ALLEGRO_LIB_...
    set(ALLEGRO_FOUND TRUE)
endif ()

# Verifica se o Allegro foi encontrado
if (ALLEGRO_FOUND)
    message(STATUS "Allegro 5 encontrado e configurado manualmente!")
    message(STATUS "Diretório de inclusão principal do Allegro: ${ALLEGRO_INCLUDE_DIR}")
    message(STATUS "Bibliotecas do Allegro (encontradas): ${ALLEGRO_LIBRARIES}")
else ()
    message(FATAL_ERROR "Allegro 5 não foi totalmente encontrado após a configuração manual. Verifique os logs do CMake.")
endif ()

# --- Fim da Localização Manual do Allegro 5 ---


# Adiciona diretórios para cabeçalhos personalizados do seu projeto.
include_directories(include)
include_directories(include/controladores)
include_directories(include/dados)
include_directories(include/elementos)
include_directories(include/fisica)
include_directories(include/interface)

# Adiciona o diretório de inclusão principal do Allegro encontrado manualmente.

# Define o nome do seu executável principal e LISTA APENAS OS ARQUIVOS FONTE (.cpp e .c).
add_executable(FlappyPDSII
        src/controladores/controlador.cpp
        src/controladores/keybind.cpp
        src/dados/dados.cpp
        src/dados/database.cpp
        src/dados/logger.cpp
        src/dados/sqlite3.c
        src/fisica/hitbox.cpp
        src/fisica/info_colisao.cpp
        src/interface/figuras.cpp
        src/interface/tela_fimdejogo.cpp
        src/interface/tela_jogo.cpp
        src/ator.cpp
        src/main.cpp
        src/objeto_cano.cpp
        src/player.cpp
        src/util.cpp
)
target_include_directories(FlappyPDSII PRIVATE ${ALLEGRO_INCLUDE_DIR})

# Vincula as bibliotecas do Allegro ao seu executável principal.
# Agora, ALLEGRO_LIBRARIES contém os caminhos completos para as bibliotecas encontradas.
target_link_libraries(FlappyPDSII PRIVATE ${ALLEGRO_LIBRARIES} sqlite3)


# --- Configuração para Testes ---
add_executable(FlappyPDSII_Tests
        tests/dados/test_dados_jogador.cpp
        tests/dados/test_database.cpp
        tests/dados/test_database_sql.cpp
)
target_link_libraries(FlappyPDSII_Tests PRIVATE ${ALLEGRO_LIBRARIES} sqlite3 doctest) # Assumindo doctest é uma lib
target_include_directories(FlappyPDSII_Tests PRIVATE tests) # Para encontrar doctest.h

# Opcional: Para ocultar a janela do console no Windows, se 'main' não for WinMain
# set_target_properties(FlappyPDSII PROPERTIES WIN32_EXECUTABLE TRUE)
