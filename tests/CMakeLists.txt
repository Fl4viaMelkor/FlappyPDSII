# test/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)  # Require C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(FlappyPDS CXX)

# É comum ter as fontes da aplicação aqui para que os testes possam linkar com elas
# Exemplo: Se 'common_code.cpp' for uma fonte da sua aplicação
# add_library(my_app_common STATIC ../src/common_code.cpp)
# target_include_directories(my_app_common PUBLIC ../src) # Para incluir cabeçalhos

# -------------------------------------------------------------
# 1. Definição do Executável de Teste
# Começamos com o 'main_test.cpp' que contém o DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
add_executable(run_tests dados/test_database.cpp)

# -------------------------------------------------------------
# 2. Adicionar as fontes de teste específicas
# Cada arquivo .cpp de teste é adicionado separadamente
target_sources(run_tests PRIVATE
        dados/test_dados_jogador.cpp # Inclui o arquivo de teste para Dado_Jogador
)

target_sources(run_tests PRIVATE
        dados/test_database_sql.cpp # Inclui o arquivo de teste para Database
)

target_sources(run_tests PRIVATE
        interface/test_tela_inicial.cpp # Inclui o arquivo de teste para TelaInicial
)

target_sources(run_tests PRIVATE
        interface/test_tela_highscore.cpp # Inclui o arquivo de teste para TelaHighScore
)

target_sources(run_tests PRIVATE
        interface/test_tela_fimdejogo.cpp # Inclui o arquivo de teste para TelaFimDeJogo
)

target_sources(run_tests PRIVATE
        interface/test_tela_cadastro.cpp # Inclui o arquivo de teste para TelaCadastro
)

# -------------------------------------------------------------
# 3. Adicionar as fontes da aplicação que os testes dependem
# Os testes precisam das implementações das classes que estão sendo testadas.
# Por exemplo, test_dados_jogador.cpp testa a implementação em dados.cpp.
target_sources(run_tests PRIVATE
        ../src/dados/dados.cpp # Adiciona o arquivo de implementação de Dado_Jogador
)

target_sources(run_tests PRIVATE
        ../src/dados/sqlite3.c # Adiciona a biblioteca SQLite embutida
        ../src/dados/database.cpp # Adiciona a implementação da classe Database
        ../src/dados/logger.cpp # Adiciona a implementação do logger
        ../src/util.cpp # Utilitários diversos usados nos testes
)

target_sources(run_tests PRIVATE
        ../src/interface/TelaInicial.cpp # Implementação da tela inicial
        ../src/interface/TelaHighScore.cpp # Implementação da tela de high score
        ../src/interface/tela_fimdejogo.cpp # Implementação da tela de fim de jogo
           ../src/interface/ParallaxBackground.cpp
    ../src/interface/TelaHighScores.cpp
    ../src/interface/TelaCadastro.cpp
)

# Adicione condicionalmente os arquivos de teste específicos do SO
#if(WIN32)
#    target_sources(run_tests PRIVATE windows_tests.cpp)
# Se windows_specific.cpp for necessário para windows_tests.cpp
##   target_sources(run_tests PRIVATE ../src/windows_specific.cpp)
#elseif(UNIX) # __linux__ e __APPLE__ geralmente são englobados por UNIX em CMake
#    target_sources(run_tests PRIVATE linux_tests.cpp)
# Se linux_specific.cpp for necessário para linux_tests.cpp
#    target_sources(run_tests PRIVATE ../src/linux_specific.cpp)
#endif()

# Adicione o diretório onde o doctest está (se não estiver instalado globalmente)
# find_package(doctest CONFIG REQUIRED) # Se doctest estiver instalado via find_package
# ou
# include_directories(path/to/doctest/include) # Se você tiver os cabeçalhos diretamente

#target_link_libraries(run_tests doctest_with_main) # ou `doctest::doctest` se usar find_package
target_link_libraries(run_tests sqlite3) # Se precisar linkar com sua lib
