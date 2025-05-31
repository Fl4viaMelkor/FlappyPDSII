# test/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)  # Require C++11

project(FlappyPDS CXX)

# Adicione suas fontes de código (se necessário para os testes)
# Por exemplo, se os testes precisam linkar com a biblioteca principal
# add_library(my_app_lib STATIC ../src/common_code.cpp ...)
# target_link_libraries(my_app_lib ...)

# Crie um executável de teste
add_executable(run_tests
        dados/test_database.cpp)

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