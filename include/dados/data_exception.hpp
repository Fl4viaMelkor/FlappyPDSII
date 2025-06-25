//
// Created by leand on 25/06/2025.
//

#ifndef DATA_EXCEPTION_HPP
#define DATA_EXCEPTION_HPP

#include <stdexcept>
#include <string>
using namespace std;
/**
 * @brief Classe base para todas as exceções relacionadas a erros nos dados.
 * Herda de std::runtime_error para se integrar ao sistema de exceções padrão do C++.
 */
class DataException : public runtime_error {
  public:
    // Construtor que aceita uma mensagem de erro.
    // Explicit para evitar conversões implícitas.
    explicit DataException(const std::string &message)
      : runtime_error(message)
    {
    }

    // O construtor de std::runtime_error já fornece a implementação de what().
    // Não precisamos sobrescrevê-lo novamente, a menos que queiramos um comportamento diferente.
};

/**
 * @brief Exceção específica para dados ausentes ou não encontrados.
 * Ex: Uma chave esperada em um mapa de dados não foi encontrada.
 */
class DataNotFoundException : public DataException {
  public:
    explicit DataNotFoundException(const std::string &message = "Dados esperados nao foram encontrados.")
      : DataException(message)
    {
    }
};

/**
 * @brief Exceção específica para dados em formato inválido.
 * Ex: Tentativa de converter uma string nao numerica em um inteiro.
 */
class InvalidDataFormatException : public DataException {
  public:
    explicit InvalidDataFormatException(const std::string &message = "Formato de dados invalido.")
      : DataException(message)
    {
    }
};

/**
 * @brief Exceção específica para falha na operação de E/S de dados (carregar/salvar).
 * Ex: Erro ao ler/escrever de/para um arquivo, ou erro de acesso ao banco de dados.
 */
class DataIOErrorException : public DataException {
  public:
    explicit DataIOErrorException(const std::string &message = "Erro de entrada/saida de dados.")
      : DataException(message)
    {
    }
};

// Você pode adicionar mais classes de exceção específicas conforme necessário, por exemplo:
/*
class DuplicateDataException : public DataException {
public:
    explicit DuplicateDataException(const std::string& message = "Dados duplicados detectados.")
        : DataException(message) {}
};
*/

#endif // DATA_EXCEPTION_HPP