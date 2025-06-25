//
// Created by leand on 25/05/2025.
//

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "dados.hpp"
#include "sqlite3.h"

using namespace std;

/**
 * @brief Interface genérica para bancos de dados de objetos.
 *
 * Define métodos abstratos para inserção, exclusão, busca e atualização.
 */
class Database {
  public:
    virtual ~Database() = default;

    /**
     * @brief Adiciona um novo objeto ao banco de dados.
     * @param o Objeto a ser adicionado.
     * @return true se a operação for bem-sucedida.
     */
    virtual bool adicionar(objeto o) = 0;

    /**
     * @brief Remove um objeto do banco.
     * @param o Objeto a ser removido.
     * @return true se a operação for bem-sucedida.
     */
    virtual bool excluir(const objeto &o) = 0;

    /**
     * @brief Busca objetos que correspondem a um valor em uma chave específica.
     * @param chave Nome da chave a buscar.
     * @param valor Valor a ser procurado.
     * @return Vetor com objetos encontrados.
     */
    virtual vector<objeto> buscar(const string &chave, const string &valor) = 0;

    /**
     * @brief Retorna todos os objetos cadastrados.
     * @return Vetor com todos os objetos.
     */
    virtual vector<objeto> listar() = 0;

    /**
     * @brief Lista os objetos ordenados por uma determinada chave.
     * @param chave A chave pela qual ordenar.
     * @param crescente Se true, ordena de forma crescente; senão, decrescente.
     * @return Vetor de objetos ordenados.
     */
    virtual vector<objeto> listar_ordenado(string chave, bool crescente) = 0;

    /**
     * @brief Remove todos os objetos do banco de dados.
     * @return true se a operação for bem-sucedida.
     */
    virtual bool limpar() = 0;

    /**
     * @brief Atualiza os dados de um objeto existente.
     * @param o Objeto com dados atualizados.
     * @return true se a atualização for bem-sucedida.
     */
    virtual bool atualizar(objeto o) = 0;

    /**
     * @brief Atualiza o objeto, ou o adiciona caso não exista.
     * @param o Objeto a ser atualizado ou adicionado.
     * @return true se a operação for bem-sucedida.
     */
    virtual bool atualizar_ou_adicionar(objeto o) = 0;
};

/**
 * @brief Implementação de banco de dados usando SQLite.
 *
 * Armazena e manipula objetos em um arquivo `.db` via SQL.
 */
class SQLDatabase : public Database {
  protected:
    sqlite3 *db_ = nullptr;                 ///< Ponteiro para o banco de dados SQLite.
    string fileName_;                       ///< Caminho do arquivo de banco.
    string table_name_;                     ///< Nome da tabela.
    string primary_key_name_;               ///< Nome da chave primária.
    string sql_query_str_;                  ///< Consulta SQL atual (pode ser reutilizada).
    bool pkAutoIncrement_ = false;          ///< Indica se a chave primária é autoincrementada.
    unordered_map<string, string> colunas_; ///< Mapeamento de colunas SQL.
    char *zErrMsg_ = nullptr;               ///< Armazena mensagens de erro da execução.
    int rc_ = 0;                            ///< Código de retorno das operações SQLite.

    /**
     * @brief Retorna o tipo SQL da coluna conforme o esquema fornecido.
     * @param col_name Nome da coluna.
     * @return Tipo SQL correspondente.
     */
    string get_sql_type_from_schema(const string &col_name) const;

    /**
     * @brief Construtor protegido, utilizado por classes filhas.
     * @param fileName Nome do arquivo do banco.
     * @param tableName Nome da tabela.
     * @param columns Mapeamento das colunas.
     * @param primaryKeyName Nome da chave primária.
     * @param pkAutoIncrement Se a chave primária é autoincrementada.
     */
    SQLDatabase(string fileName, string tableName, const unordered_map<string, string> &columns, string primaryKeyName,
                bool pkAutoIncrement);

  public:
    ~SQLDatabase() override;

    bool adicionar(objeto o) final;
    bool excluir(const objeto &o) final;
    vector<objeto> buscar(const string &chave, const string &valor) final;
    vector<objeto> listar() final;
    vector<objeto> listar_ordenado(string chave, bool crescente) final;
    bool limpar() final;
    bool atualizar(objeto o) final;
    bool atualizar_ou_adicionar(objeto o) final;

    /**
     * @brief Verifica se o banco foi inicializado corretamente.
     * @return true se o ponteiro do banco não for nulo.
     */
    bool hasDb() const;
};

/**
 * @brief Implementação concreta da base de dados específica para jogadores.
 */
class JogadorSQLDatabase : public SQLDatabase {
  public:
    /**
     * @brief Construtor da base de dados de jogadores.
     * @param fileName Nome do arquivo SQLite. Por padrão, "jogadores.db".
     */
    explicit JogadorSQLDatabase(const string &fileName = "jogadores.db");

    ~JogadorSQLDatabase() override;
};

#endif // DATABASE_HPP
