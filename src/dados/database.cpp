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

#include "../../include/dados/sqlite3.h"

static void bind_value(sqlite3_stmt *stmt, int index, const string &value_str, const string &sql_type)
{
    if (sql_type == "INTEGER") {
        try {
            sqlite3_bind_int(stmt, index, stoi(value_str));
        }
        catch (const exception &e) {
            // cerr << "Aviso: Falha ao converter '" << value_str << "' para INTEGER. Bind como NULL. Erro: " <<
            // e.what() << endl;
            sqlite3_bind_null(stmt, index);
        }
    }
    else if (sql_type == "REAL") {
        try {
            sqlite3_bind_double(stmt, index, stod(value_str));
        }
        catch (const exception &e) {
            // cerr << "Aviso: Falha ao converter '" << value_str << "' para REAL. Bind como NULL. Erro: " << e.what()
            // <<         endl;
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

SQLDatabase::SQLDatabase(const string &fileName, const string &tableName, const unordered_map<string, string> &columns,
                         const string &primaryKeyName, bool pkAutoIncrement)
  : fileName_(fileName)
  , table_name_(tableName)
  , primary_key_name_(primaryKeyName)
  , pkAutoIncrement_(pkAutoIncrement)
  , colunas_(columns)
{
    rc_ = sqlite3_open(fileName_.c_str(), &db_);
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR: Nao foi possivel abrir o banco de dados " << fileName_ << ": " <<
        // sqlite3_errmsg(db_) << endl;
        if (db_)
            sqlite3_close(db_);
        db_ = nullptr;
        throw runtime_error("Falha critica ao abrir o banco de dados.");
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

    // cout << "SQLDatabase: Criando tabela '" << table_name_ << "' com o esquema: " << sql_query_str_ << endl;

    rc_ = sqlite3_exec(db_, sql_query_str_.c_str(), 0, 0, &zErrMsg_);
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR: Nao foi possivel criar a tabela " << table_name_ << ": " << zErrMsg_ << endl;
        sqlite3_free(zErrMsg_);
        sqlite3_close(db_);
        db_ = nullptr;
        throw runtime_error("Falha critica ao criar a tabela do banco de dados.");
    }
    // cout << "SQLDatabase: Tabela '" << table_name_ << "' criada (se nao existia)." << endl;
}

// Destructor
SQLDatabase::~SQLDatabase()
{
    if (db_) {
        // cout << "SQLDatabase: Fechando banco de dados '" << fileName_ << "'." << endl;
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
    if (!db_) {
        // cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para adicionar." << endl;
        return false;
    }

    objeto normalized_o;
    for (const auto &pair : o.dados)
        normalized_o.dados[toUpper(pair.first)] = pair.second;
    ostringstream cols_oss, placeholders_oss;

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

    // cout << "SQLDatabase: Adicionando objeto ao banco de dados. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0); // Prepare query
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR (adicionar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ <<
        // endl;
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
    if (!success) {
        // cerr << "SQLDatabase ERROR (adicionar exec): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt); // Finalize statement
    return success;
}

bool SQLDatabase::excluir(const objeto &o)
{
    if (!db_) {
        // cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para excluir." << endl;
        return false;
    }

    objeto normalized_o;
    for (const auto &pair : o.dados)
        normalized_o.dados[toUpper(pair.first)] = pair.second;

    if (!normalized_o.dados.count(primary_key_name_)) {
        // cerr << "SQLDatabase ERROR: Chave primaria '" << primary_key_name_ <<        "' nao encontrada no objeto
        // normalizado para exclusao." << endl;
        return false;
    }

    const vector<objeto> results = buscar(primary_key_name_, normalized_o.dados.at(primary_key_name_));
    if (results.size() != 1)
        return false;

    ostringstream sql_oss;
    sql_oss << "DELETE FROM " << table_name_ << " WHERE " << primary_key_name_ << " = ?;";

    sql_query_str_ = sql_oss.str();
    // cout << "SQLDatabase: Excluindo objeto. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR (excluir prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ << endl;
        return false;
    }

    bind_value(stmt, 1, normalized_o.dados.at(primary_key_name_), get_sql_type_from_schema(primary_key_name_));

    rc_ = sqlite3_step(stmt);
    bool success = (rc_ == SQLITE_DONE);
    if (!success) {
        // cerr << "SQLDatabase ERROR (excluir exec): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return success;
}

vector<objeto> SQLDatabase::buscar(const string &chave, const string &valor)
{
    vector<objeto> results;
    if (!db_) {
        // cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para buscar." << endl;
        return results;
    }

    const string normalized_chave = toUpper(chave);

    if (colunas_.find(normalized_chave) == colunas_.end()) {
        // cerr << "SQLDatabase ERROR: Chave '" << chave << "' (normalizada para '" << normalized_chave <<        "')
        // nao e uma coluna valida para busca." << endl;
        return results;
    }

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << " WHERE " << normalized_chave << " = ?;";

    sql_query_str_ = sql_oss.str();
    // cout << "SQLDatabase: Buscando objeto. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR (buscar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ << endl;
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

    if (rc_ != SQLITE_DONE) {
        // cerr << "SQLDatabase ERROR (buscar step): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return results;
}

vector<objeto> SQLDatabase::listar()
{
    vector<objeto> results;
    if (!db_) {
        // cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para listar." << endl;
        return results;
    }

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << ";";

    sql_query_str_ = sql_oss.str();
    // cout << "SQLDatabase: Listando objetos. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR (listar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ << endl;
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

    if (rc_ != SQLITE_DONE) {
        // cerr << "SQLDatabase ERROR (listar step): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return results;
}

vector<objeto> SQLDatabase::listar_ordenado(string chave, bool crescente)
{
    vector<objeto> results;
    if (!db_) {
        // cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para listar_ordenado." << endl;
        return results;
    }

    string normalized_chave = toUpper(chave);

    if (colunas_.find(normalized_chave) == colunas_.end()) {
        // cerr << "SQLDatabase ERROR: Chave '" << chave << "' (normalizada para '" << normalized_chave << "') nao e uma
        // coluna valida para ordenacao." << endl;
        return results;
    }

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << " ORDER BY " << normalized_chave << (crescente ? " ASC" : " DESC")
            << ";";

    sql_query_str_ = sql_oss.str();
    // cout << "SQLDatabase: Listando objetos ordenados. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR (listar_ordenado prepare): " << sqlite3_errmsg(db_) << " SQL: " <<  sql_query_str_
        // << endl;
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

    if (rc_ != SQLITE_DONE) {
        // cerr << "SQLDatabase ERROR (listar_ordenado step): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return results;
}

bool SQLDatabase::limpar()
{
    if (!db_) {
        // cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para limpar." << endl;
        return false;
    }

    ostringstream sql_oss;
    sql_oss << "DELETE FROM " << table_name_ << ";";

    sql_query_str_ = sql_oss.str();
    // cout << "SQLDatabase: Limpando tabela. Query: '" << sql_query_str_ << "'." << endl;

    rc_ = sqlite3_exec(db_, sql_query_str_.c_str(), 0, 0, &zErrMsg_);
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR (limpar exec): " << zErrMsg_ << endl;
        sqlite3_free(zErrMsg_);
        return false;
    }
    return true;
}

bool SQLDatabase::atualizar(objeto o)
{
    if (!db_) {
        // cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para atualizar." << endl;
        return false;
    }
    vector<objeto> results = buscar(primary_key_name_, o.dados.at(primary_key_name_));
    if (results.size() != 1)
        return false;

    objeto normalized_o;
    for (const auto &pair : o.dados)
        normalized_o.dados[toUpper(pair.first)] = pair.second;

    if (!normalized_o.dados.count(primary_key_name_)) {
        // cerr << "SQLDatabase ERROR: Chave primaria '" << primary_key_name_ << "' nao encontrada no objeto normalizado
        // para atualizacao." << endl;
        return false;
    }

    ostringstream set_clause_oss;
    bool first = true;
    for (const auto &col_pair : colunas_) {
        // Iterate through DB schema to build SET clause
        if (col_pair.first == primary_key_name_)
            continue; // Skip PK in SET clause
        if (normalized_o.dados.count(col_pair.first)) {
            // Only include columns present in normalized object
            if (!first)
                set_clause_oss << ", ";
            set_clause_oss << col_pair.first << " = ?";
            first = false;
        }
    }

    ostringstream sql_oss;
    sql_oss << "UPDATE " << table_name_ << " SET " << set_clause_oss.str() << " WHERE " << primary_key_name_ << " = ?;";

    sql_query_str_ = sql_oss.str();
    // cout << "SQLDatabase: Atualizando objeto. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        // cerr << "SQLDatabase ERROR (atualizar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ <<
        // endl;
        return false;
    }

    int bind_index = 1;
    for (const auto &col_pair : colunas_) {
        // Iterate through DB schema for correct binding order
        if (col_pair.first == primary_key_name_)
            continue;
        if (normalized_o.dados.count(col_pair.first)) {
            bind_value(stmt, bind_index++, normalized_o.dados.at(col_pair.first),
                       get_sql_type_from_schema(col_pair.first));
        }
    }
    bind_value(stmt, bind_index, normalized_o.dados.at(primary_key_name_), get_sql_type_from_schema(primary_key_name_));

    rc_ = sqlite3_step(stmt);
    bool success = (rc_ == SQLITE_DONE);
    if (!success) {
        // cerr << "SQLDatabase ERROR (atualizar exec): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return success;
}

bool SQLDatabase::atualizar_ou_adicionar(const objeto o)
{
    objeto normalized_o;
    for (const auto &pair : o.dados)
        normalized_o.dados[toUpper(pair.first)] = pair.second;

    const vector<objeto> existing_objects = buscar(primary_key_name_, normalized_o.dados.at(primary_key_name_));
    if (!existing_objects.empty())
        return atualizar(normalized_o);
    else
        return adicionar(normalized_o);
}

bool SQLDatabase::hasDb() const { return db_ != nullptr; }

JogadorSQLDatabase::JogadorSQLDatabase(const string &fileName)
  : SQLDatabase(fileName, "JOGADOR", // Table name
                                     // Column schema for the JOGADOR table
                Dado_Jogador::get_sql_columns(), toUpper(Dado_Jogador::get_primary_key()),
                // Primary key name
                false // pkAutoIncrement_ is true for ID
    )
{
    if (db_ == nullptr) {
        // cerr << "JogadorSQLDatabase ERROR: A classe base SQLDatabase falhou ao inicializar o banco de dados." <<
        // endl;
    }
}

// Destructor
JogadorSQLDatabase::~JogadorSQLDatabase() {}
