//
// Created by leand on 01/06/2025.
//
#include "../../include/dados/database.hpp"

#include "../../include/dados/dados.hpp" // Para a struct 'objeto' (se não estiver em um cabeçalho separado)

#include "../../include/util.hpp" // Para a struct 'objeto' (se n├úo estiver em um cabe├ºalho separado)"
#include <algorithm>              // Para transform
#include <iomanip>                // Para quoted (C++14+) ou formata├º├úo manual
#include <iostream>               // Para // cerr, endl
#include <map>                    // Para map (usado para ordenar colunas em listar_dados)
#include <sstream>                // Para ostringstream
#include <stdexcept>              // Para runtime_error, invalid_argument
#include <utility>

#include "../../include/dados/sqlite3.h"

static void bind_value(sqlite3_stmt *stmt, int index, const string &value_str, const string &sql_type)
{
    if (sql_type == "INTEGER") {
        try {
            sqlite3_bind_int(stmt, index, stoi(value_str));
        }
        catch (const exception &e) {
            cerr << "Aviso: Falha ao converter '" << value_str << "' para INTEGER. Bind como NULL. Erro: " << e.what()
                 << endl;
            sqlite3_bind_null(stmt, index);
        }
    }
    else if (sql_type == "REAL") {
        try {
            sqlite3_bind_double(stmt, index, stod(value_str));
        }
        catch (const exception &e) {
            cerr << "Aviso: Falha ao converter '" << value_str << "' para REAL. Bind como NULL. Erro: " << e.what()
                 << endl;
            sqlite3_bind_null(stmt, index);
        }
    }
    else {
        sqlite3_bind_text(stmt, index, value_str.c_str(), -1, SQLITE_TRANSIENT);
    }
}

static string column_to_string(sqlite3_stmt *stmt, int col_index, int col_type)
{
    if (col_type == SQLITE_INTEGER) {
        return to_string(sqlite3_column_int(stmt, col_index));
    }
    else if (col_type == SQLITE_FLOAT) {
        ostringstream oss;
        oss << fixed << setprecision(6) << sqlite3_column_double(stmt, col_index);
        return oss.str();
    }
    else if (col_type == SQLITE_TEXT) {
        const unsigned char *text_val = sqlite3_column_text(stmt, col_index);
        return text_val ? reinterpret_cast<const char *>(text_val) : "";
    }
    else {
        return "";
    }
}

string SQLDatabase::get_sql_type_from_schema(const string &col_name) const
{
    auto it = colunas_.find(toUpper(col_name));
    if (it != colunas_.end()) {
        string type_def = it->second;
        transform(type_def.begin(), type_def.end(), type_def.begin(), ::toupper);
        if (type_def.find("INTEGER") != string::npos)
            return "INTEGER";
        if (type_def.find("REAL") != string::npos)
            return "REAL";
        if (type_def.find("TEXT") != string::npos)
            return "TEXT";
        return "";
    }
    return "";
}

SQLDatabase::SQLDatabase(string fileName, string tableName, const unordered_map<string, string> &columns,
                         string primaryKeyName, bool pkAutoIncrement)
  : fileName_(move(fileName))
  , table_name_(move(tableName))
  , primary_key_name_(move(primaryKeyName))
  , pkAutoIncrement_(pkAutoIncrement)
  , colunas_(columns)
{
    rc_ = sqlite3_open(fileName_.c_str(), &db_);
    if (rc_ != SQLITE_OK) {
        if (db_)
            sqlite3_close(db_);
        db_ = nullptr;
        throw DataIOErrorException("Falha critica ao abrir o banco de dados.");
    }
    // cout << "SQLDatabase: Banco de dados '" << fileName_ << "' aberto com sucesso." << endl;

    ostringstream create_table_sql_oss;
    create_table_sql_oss << "CREATE TABLE IF NOT EXISTS " << table_name_ << " (";
    bool first_col = true;
    for (const auto &col_pair : colunas_) {
        if (!first_col)
            create_table_sql_oss << ", ";
        create_table_sql_oss << col_pair.first << " " << col_pair.second;
        first_col = false;
    }
    create_table_sql_oss << ");";

    sql_query_str_ = create_table_sql_oss.str();

    rc_ = sqlite3_exec(db_, sql_query_str_.c_str(), 0, 0, &zErrMsg_);
    if (rc_ != SQLITE_OK) {
        sqlite3_free(zErrMsg_);
        sqlite3_close(db_);
        db_ = nullptr;
        throw DataIOErrorException("Falha critica ao criar a tabela do banco de dados.");
    }
}

// Destructor
SQLDatabase::~SQLDatabase()
{
    if (db_) {
        cout << "SQLDatabase: Fechando banco de dados '" << fileName_ << "'." << endl;
        sqlite3_close(db_);
        db_ = nullptr;
    }
    if (zErrMsg_) {
        sqlite3_free(zErrMsg_);
        zErrMsg_ = nullptr;
    }
}

bool SQLDatabase::adicionar(objeto o)
{
    if (db_ == nullptr)
        throw DataIOErrorException("Erro ao carregar a DB");

    objeto normalized_o;
    for (const auto &pair : o.dados)
        normalized_o.dados[toUpper(pair.first)] = pair.second;
    ostringstream cols_oss, placeholders_oss;

    if (!normalized_o.dados.count(primary_key_name_))
        throw InvalidDataFormatException("Chave primaria '" + primary_key_name_ +
                                         "' ausente no objeto para operacao de upsert.");

    bool first = true;
    for (const auto &col_pair : colunas_) {
        if (pkAutoIncrement_ && col_pair.first == primary_key_name_)
            continue; // Skip PK for AUTOINCREMENT columns
        if (normalized_o.dados.count(col_pair.first)) {
            if (!first) {
                cols_oss << ", ";
                placeholders_oss << ", ";
            }
            cols_oss << col_pair.first;
            placeholders_oss << "?";
            first = false;
        }
    }

    ostringstream sql_oss;
    sql_oss << "INSERT INTO " << table_name_ << " (" << cols_oss.str() << ") VALUES (" << placeholders_oss.str()
            << ");";

    sql_query_str_ = sql_oss.str(); // Store the query string

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0); // Prepare query
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (adicionar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ << endl;
        return false;
    }

    int bind_index = 1;
    for (const auto &col_pair : colunas_) {
        // Iterate through DB schema for correct binding order
        if (pkAutoIncrement_ && col_pair.first == primary_key_name_)
            continue;
        if (normalized_o.dados.count(col_pair.first)) {
            // Bind only if column is present in normalized object
            bind_value(stmt, bind_index++, normalized_o.dados.at(col_pair.first),
                       get_sql_type_from_schema(col_pair.first));
        }
    }
    rc_ = sqlite3_step(stmt); // Execute query
    bool success = (rc_ == SQLITE_DONE);
    if (!success)
        cerr << "SQLDatabase ERROR (adicionar exec): " << sqlite3_errmsg(db_) << endl;
    sqlite3_finalize(stmt); // Finalize statement
    return success;
}

bool SQLDatabase::excluir(const objeto &o)
{
    if (db_ == nullptr)
        throw DataIOErrorException("Erro ao carregar a DB");

    objeto normalized_o;
    for (const auto &pair : o.dados)
        normalized_o.dados[toUpper(pair.first)] = pair.second;

    if (!normalized_o.dados.count(primary_key_name_))
        throw InvalidDataFormatException("Chave primaria '" + primary_key_name_ +
                                         "' ausente no objeto para operacao de upsert.");

    const vector<objeto> results = buscar(primary_key_name_, normalized_o.dados.at(primary_key_name_));
    if (results.size() != 1)
        return false;

    ostringstream sql_oss;
    sql_oss << "DELETE FROM " << table_name_ << " WHERE " << primary_key_name_ << " = ?;";

    sql_query_str_ = sql_oss.str();

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (excluir prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ << endl;
        return false;
    }

    bind_value(stmt, 1, normalized_o.dados.at(primary_key_name_), get_sql_type_from_schema(primary_key_name_));

    rc_ = sqlite3_step(stmt);
    bool success = (rc_ == SQLITE_DONE);
    if (!success)
        cerr << "SQLDatabase ERROR (excluir exec): " << sqlite3_errmsg(db_) << endl;
    sqlite3_finalize(stmt);
    return success;
}

vector<objeto> SQLDatabase::buscar(const string &chave, const string &valor)
{
    if (db_ == nullptr)
        throw DataIOErrorException("Erro ao carregar a DB");

    vector<objeto> results;
    const string normalized_chave = toUpper(chave);

    if (colunas_.find(normalized_chave) == colunas_.end())
        throw DataNotFoundException("SQLDatabase ERROR: Chave '" + chave + "' (normalizada para '" + normalized_chave +
                                    "') nao e uma coluna valida para busca.\n");

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << " WHERE " << normalized_chave << " = ?;";

    sql_query_str_ = sql_oss.str();

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (buscar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ << endl;
        return results;
    }

    bind_value(stmt, 1, valor, get_sql_type_from_schema(normalized_chave));

    while ((rc_ = sqlite3_step(stmt)) == SQLITE_ROW) {
        objeto found_obj;
        int num_cols = sqlite3_column_count(stmt);
        for (int i = 0; i < num_cols; ++i) {
            string col_name = sqlite3_column_name(stmt, i);
            int col_type = sqlite3_column_type(stmt, i);
            found_obj.dados[col_name] = column_to_string(stmt, i, col_type);
        }
        results.push_back(found_obj);
    }

    if (rc_ != SQLITE_DONE)
        cerr << "SQLDatabase ERROR (buscar step): " << sqlite3_errmsg(db_) << endl;
    sqlite3_finalize(stmt);
    return results;
}

vector<objeto> SQLDatabase::listar()
{
    if (db_ == nullptr)
        throw DataIOErrorException("Erro ao carregar a DB");

    vector<objeto> results;

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << ";";

    sql_query_str_ = sql_oss.str();

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (listar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ << endl;
        return results;
    }

    while ((rc_ = sqlite3_step(stmt)) == SQLITE_ROW) {
        objeto found_obj;
        int num_cols = sqlite3_column_count(stmt);
        for (int i = 0; i < num_cols; ++i) {
            string col_name = sqlite3_column_name(stmt, i);
            int col_type = sqlite3_column_type(stmt, i);
            found_obj.dados[col_name] = column_to_string(stmt, i, col_type);
        }
        results.push_back(found_obj);
    }

    if (rc_ != SQLITE_DONE)
        cerr << "SQLDatabase ERROR (listar step): " << sqlite3_errmsg(db_) << endl;
    sqlite3_finalize(stmt);
    return results;
}

vector<objeto> SQLDatabase::listar_ordenado(string chave, bool crescente)
{
    if (db_ == nullptr)
        throw DataIOErrorException("Erro ao carregar a DB");
    vector<objeto> results;

    string normalized_chave = toUpper(chave);

    if (colunas_.find(normalized_chave) == colunas_.end())
        throw InvalidDataFormatException("SQLDatabase ERROR: Chave '" + chave + "' (normalizada para '" +
                                         normalized_chave + "') nao e uma coluna valida para busca.\n");

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << " ORDER BY " << normalized_chave << (crescente ? " ASC" : " DESC")
            << ";";

    sql_query_str_ = sql_oss.str();

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (listar_ordenado prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_
             << endl;
        return results;
    }

    while ((rc_ = sqlite3_step(stmt)) == SQLITE_ROW) {
        objeto found_obj;
        int num_cols = sqlite3_column_count(stmt);
        for (int i = 0; i < num_cols; ++i) {
            string col_name = sqlite3_column_name(stmt, i);
            int col_type = sqlite3_column_type(stmt, i);
            found_obj.dados[col_name] = column_to_string(stmt, i, col_type);
        }
        results.push_back(found_obj);
    }

    if (rc_ != SQLITE_DONE)
        cerr << "SQLDatabase ERROR (listar_ordenado step): " << sqlite3_errmsg(db_) << endl;
    sqlite3_finalize(stmt);
    return results;
}

bool SQLDatabase::limpar()
{
    if (db_ == nullptr)
        throw DataIOErrorException("Erro ao carregar a DB");

    ostringstream sql_oss;
    sql_oss << "DELETE FROM " << table_name_ << ";";

    sql_query_str_ = sql_oss.str();

    rc_ = sqlite3_exec(db_, sql_query_str_.c_str(), 0, 0, &zErrMsg_);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (limpar exec): " << zErrMsg_ << endl;
        sqlite3_free(zErrMsg_);
        return false;
    }
    return true;
}

bool SQLDatabase::atualizar(objeto o)
{
    if (db_ == nullptr)
        throw DataIOErrorException("Erro ao carregar a DB");

    // 1. Normalizar o objeto de entrada para que todas as chaves sejam maiúsculas
    objeto normalized_o;
    for (const auto &pair : o.dados)
        normalized_o.dados[toUpper(pair.first)] = pair.second;

    // 2. Verificar se a chave primária está presente no objeto de entrada
    if (!normalized_o.dados.count(primary_key_name_))
        throw InvalidDataFormatException("Chave primaria '" + primary_key_name_ +
                                         "' ausente no objeto para operacao de atualizaçao.");

    // 3. Verificar se o registro existe antes de tentar atualizar

    std::vector<objeto> results = buscar(primary_key_name_, normalized_o.dados.at(primary_key_name_));
    if (results.size() != 1) {
        // Se não encontrou ou encontrou múltiplos, significa que não há um registro único para atualizar.
        cerr << "SQLDatabase ERROR: Registro com PK '" << normalized_o.dados.at(primary_key_name_)
             << "' nao encontrado ou nao e unico para atualizacao." << endl;
        return false;
    }

    // 4. Construir a cláusula SET e a lista de valores a serem vinculados dinamicamente
    std::ostringstream set_clause_oss;
    std::vector<std::pair<std::string, std::string>> columns_to_bind; // Armazena (col_name, value_str) em ordem

    bool first_set_col = true;
    for (const auto &col_pair_schema : colunas_) {                 // Itera sobre o SCHEMA do DB (ordem esperada)
        const std::string &col_name_upper = col_pair_schema.first; // Nome da coluna no schema (MAIÚSCULA)

        // Pula a chave primária na cláusula SET
        if (col_name_upper == primary_key_name_)
            continue;

        // Se a coluna estiver presente no objeto de entrada para atualização
        if (normalized_o.dados.count(col_name_upper)) {
            if (!first_set_col)
                set_clause_oss << ", ";
            set_clause_oss << col_name_upper << " = ?";
            first_set_col = false;

            // Adiciona o nome da coluna e seu valor para vincular na ordem correta
            columns_to_bind.emplace_back(col_name_upper, normalized_o.dados.at(col_name_upper));
        }
    }

    // Se nenhuma coluna foi fornecida para atualização (além da PK), não há o que fazer
    if (columns_to_bind.empty()) {
        cerr << "SQLDatabase ERROR: Nenhuma coluna fornecida para atualizacao, alem da chave primaria." << endl;
        return false;
    }

    // 5. Construir a query SQL final
    std::ostringstream sql_oss;
    sql_oss << "UPDATE " << table_name_ << " SET " << set_clause_oss.str() << " WHERE " << primary_key_name_ << " = ?;";

    sql_query_str_ = sql_oss.str(); // Armazena a query para depuração, se necessário

    // 6. Preparar a declaração SQL
    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (atualizar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ << endl;
        return false;
    }

    // 7. Vincular os valores à declaração preparada (Primeiro os SET values, depois o WHERE value)
    int bind_index = 1;
    for (const auto &col_data : columns_to_bind) {
        // col_data.first é o nome da coluna, col_data.second é o valor
        bind_value(stmt, bind_index++, col_data.second, get_sql_type_from_schema(col_data.first));
    }

    // Vincula o valor da chave primária (o último '?' na cláusula WHERE)
    bind_value(stmt, bind_index, normalized_o.dados.at(primary_key_name_), get_sql_type_from_schema(primary_key_name_));

    // 8. Executar a declaração
    rc_ = sqlite3_step(stmt);
    bool success = (rc_ == SQLITE_DONE); // SQLITE_DONE indica que a operação foi concluída sem mais linhas
    if (!success)
        cerr << "SQLDatabase ERROR (atualizar exec): " << sqlite3_errmsg(db_) << endl;

    sqlite3_finalize(stmt);
    return success;
}

bool SQLDatabase::atualizar_ou_adicionar(const objeto o)
{
    objeto normalized_o;
    for (const auto &pair : o.dados)
        normalized_o.dados[toUpper(pair.first)] = pair.second;
    if (!normalized_o.dados.count(primary_key_name_))
        throw InvalidDataFormatException("Chave primaria '" + primary_key_name_ +
                                         "' ausente no objeto para operacao de upsert.");

    const vector<objeto> existing_objects = buscar(primary_key_name_, normalized_o.dados.at(primary_key_name_));
    cout << "Verificação realizada" << endl;
    if (!existing_objects.empty())
        return atualizar(normalized_o);
    else
        return adicionar(normalized_o);
}

bool SQLDatabase::hasDb() const { return db_ != nullptr; }

JogadorSQLDatabase::JogadorSQLDatabase(const string &fileName)
  : SQLDatabase(fileName, "JOGADOR", // Table name
                Dado_Jogador::get_sql_columns(), toUpper(Dado_Jogador::get_primary_key()),
                false // pkAutoIncrement_ is true for ID
    )
{
    if (db_ == nullptr)
        throw DataIOErrorException("Erro ao inicializar a DB");
}

// Destructor
JogadorSQLDatabase::~JogadorSQLDatabase() = default;
