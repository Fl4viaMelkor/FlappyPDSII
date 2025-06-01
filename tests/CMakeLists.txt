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

# -------------------------------------------------------------
# 3. Adicionar as fontes da aplicação que os testes dependem
# Os testes precisam das implementações das classes que estão sendo testadas.
# Por exemplo, test_dados_jogador.cpp testa a implementação em dados.cpp.
target_sources(run_tests PRIVATE
        ../src/dados/dados.cpp # Adiciona o arquivo de implementação de Dado_Jogador
        # ../src/common_code.cpp # Se 'common_tests.cpp' ou outros testes dependem de common_code
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
# target_link_libraries(run_tests my_app_lib) # Se precisar linkar com sua lib