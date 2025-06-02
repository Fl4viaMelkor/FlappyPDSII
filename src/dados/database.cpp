//
// Created by leand on 01/06/2025.
//
#include "../../include/dados/database.hpp"


#include "../../include/dados/dados.hpp"        // Para a struct 'objeto' (se não estiver em um cabeçalho separado)

#include <iostream>        // Para cerr, endl
#include <sstream>         // Para ostringstream
#include <stdexcept>       // Para runtime_error, invalid_argument
#include <algorithm>       // Para transform
#include <iomanip>         // Para quoted (C++14+) ou formatação manual
#include <map>             // Para map (usado para ordenar colunas em listar_dados)
#include "../../include/util.hpp"        // Para a struct 'objeto' (se não estiver em um cabeçalho separado)"


// Inclui a API C do SQLite. Certifique-se de que este cabeçalho esteja no seu caminho de inclusão.
#include "../../include/dados/sqlite3.h"
// =============================================================================
// Funções Auxiliares (static ou namespace anônimo para escopo local)
// =============================================================================

const unordered_map<string, string> SQLDatabase::c_to_sql_type = {
    {"INT", "INTEGER"},
    {"STRING", "TEXT"},
    {"BOOL", "INTEGER"},
    {"FLOAT", "REAL"},
    {"VECTOR", "STRING_VECTOR"}
};
//
// Created by leand on 01/06/2025.
//

// =============================================================================
// Helper Functions (static to this compilation unit)
// =============================================================================

// Converts a C++ string to the appropriate SQL type and binds it to the statement.
// Assumes basic type mapping (INTEGER, REAL, TEXT).
static void bind_value(sqlite3_stmt *stmt, int index, const string &value_str, const string &sql_type) {
    if (sql_type == "INTEGER") {
        try {
            sqlite3_bind_int(stmt, index, stoi(value_str));
        }
        catch (const exception &e) {
            cerr << "Aviso: Falha ao converter '" << value_str << "' para INTEGER. Bind como NULL. Erro: " << e.
                    what() << endl;
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
        // Default to TEXT (includes other non-numeric types or NULL)
        sqlite3_bind_text(stmt, index, value_str.c_str(), -1, SQLITE_TRANSIENT);
    }
}

// Converts a column value read from SQLite to a C++ string.
static string column_to_string(sqlite3_stmt *stmt, int col_index, int col_type) {
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
        // SQLITE_NULL or SQLITE_BLOB (if not explicitly handled)
        return ""; // Represents NULL or unhandled BLOB data as an empty string
    }
}

// =============================================================================
// SQLDatabase Implementation
// =============================================================================

// Helper method to get the SQL type definition from a column name based on 'colunas_'.
// This is a member function as it accesses 'colunas_'.
string SQLDatabase::get_sql_type_from_schema(const string &col_name) const {
    auto it = colunas_.find(toUpper(col_name)); // Normalize search key to uppercase
    if (it != colunas_.end()) {
        string type_def = it->second; // e.g., "INTEGER PRIMARY KEY AUTOINCREMENT"
        // Extract basic type (INTEGER, REAL, TEXT, BLOB) from the definition
        transform(type_def.begin(), type_def.end(), type_def.begin(), ::toupper);
        if (type_def.find("INTEGER") != string::npos)
            return "INTEGER";
        if (type_def.find("REAL") != string::npos)
            return "REAL";
        if (type_def.find("TEXT") != string::npos)
            return "TEXT";
        if (type_def.find("BLOB") != string::npos)
            return "BLOB";
        // Default to TEXT if a complex type definition is provided without a clear basic type match
        return "TEXT";
    }
    return "TEXT"; // Default if column not found in schema (error condition)
}

// Static method to convert a simplified C++ type string to a basic SQL type string.
string SQLDatabase::get_sql_type_mapping(const string &c_type) {
    const auto it = c_to_sql_type.find(toUpper(c_type)); // toUpper is crucial for the lookup

    if (it != c_to_sql_type.end()) {
        return it->second;
    }
    else {
        cerr << "Aviso: Tipo C++ '" << c_type << "' nao mapeado para tipo SQL conhecido." << endl;
        return ""; // Or "TEXT" as a safe default
    }
}

// Protected constructor (called by derived classes)
// Opens the database connection and creates the table with the specified schema.
SQLDatabase::SQLDatabase(const string &fileName, const string &tableName,
                         const unordered_map<string, string> &columns,
                         const string &primaryKeyName, bool pkAutoIncrement) : fileName_(fileName),
                                                                               table_name_(tableName),
                                                                               primary_key_name_(primaryKeyName),
                                                                               pkAutoIncrement_(pkAutoIncrement),
                                                                               colunas_(columns) {
    // Open the database connection
    rc_ = sqlite3_open(fileName_.c_str(), &db_);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR: Nao foi possivel abrir o banco de dados " << fileName_ << ": " <<
                sqlite3_errmsg(db_) << endl;
        if (db_)
            sqlite3_close(db_);
        db_ = nullptr;
        throw runtime_error("Falha critica ao abrir o banco de dados.");
    }
    cout << "SQLDatabase: Banco de dados '" << fileName_ << "' aberto com sucesso." << endl;

    // Build the CREATE TABLE statement
    ostringstream create_table_sql_oss;
    create_table_sql_oss << "CREATE TABLE IF NOT EXISTS " << table_name_ << " (";
    bool first_col = true;
    for (const auto &col_pair: colunas_) {
        if (!first_col) {
            create_table_sql_oss << ", ";
        }
        create_table_sql_oss << col_pair.first << " " << col_pair.second;
        first_col = false;
    }
    create_table_sql_oss << ");";

    sql_query_str_ = create_table_sql_oss.str(); // Store the query string in the member variable

    cout << "SQLDatabase: Criando tabela '" << table_name_ << "' com o esquema: " << sql_query_str_ << endl;

    // Execute table creation
    rc_ = sqlite3_exec(db_, sql_query_str_.c_str(), 0, 0, &zErrMsg_);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR: Nao foi possivel criar a tabela " << table_name_ << ": " << zErrMsg_ <<
                endl;
        sqlite3_free(zErrMsg_);
        sqlite3_close(db_);
        db_ = nullptr;
        throw runtime_error("Falha critica ao criar a tabela do banco de dados.");
    }
    cout << "SQLDatabase: Tabela '" << table_name_ << "' criada (se nao existia)." << endl;
}

// Destructor
SQLDatabase::~SQLDatabase() {
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

// Add an object to the database.
bool SQLDatabase::adicionar(objeto o) {
    if (!db_) {
        cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para adicionar." << endl;
        return false;
    }

    // Normalize keys in the input object to uppercase to match schema (if DB is case-insensitive)
    objeto normalized_o = o;
    for (auto it = normalized_o.dados.begin(); it != normalized_o.dados.end();) {
        string upper_key = toUpper(it->first);
        if (upper_key != it->first) {
            // If key capitalization changed
            normalized_o.dados[upper_key] = it->second;
            it = normalized_o.dados.erase(it); // Remove original key
        }
        else {
            ++it;
        }
    }

    ostringstream cols_oss, placeholders_oss;

    bool first = true;
    for (const auto &col_pair: colunas_) {
        // Iterate through DB schema columns
        if (pkAutoIncrement_ && col_pair.first == primary_key_name_) {
            continue; // Skip PK for AUTOINCREMENT columns
        }
        if (normalized_o.dados.count(col_pair.first)) {
            // Only include columns present in the object
            if (!first) {
                cols_oss << ", ";
                placeholders_oss << ", ";
            }
            cols_oss << col_pair.first;
            placeholders_oss << "?"; // Use placeholders for prepared statements
            first = false;
        }
    }

    ostringstream sql_oss;
    sql_oss << "INSERT INTO " << table_name_ << " (" << cols_oss.str() << ") VALUES (" << placeholders_oss.str() <<
            ");";

    sql_query_str_ = sql_oss.str(); // Store the query string

    cout << "SQLDatabase: Adicionando objeto ao banco de dados. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0); // Prepare query
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (adicionar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ <<
                endl;
        return false;
    }

    int bind_index = 1;
    for (const auto &col_pair: colunas_) {
        // Iterate through DB schema for correct binding order
        if (pkAutoIncrement_ && col_pair.first == primary_key_name_) {
            continue;
        }
        if (normalized_o.dados.count(col_pair.first)) {
            // Bind only if column is present in normalized object
            bind_value(stmt, bind_index++, normalized_o.dados.at(col_pair.first),
                       get_sql_type_from_schema(col_pair.first));
        }
    }
    rc_ = sqlite3_step(stmt); // Execute query
    bool success = (rc_ == SQLITE_DONE);
    if (!success) {
        cerr << "SQLDatabase ERROR (adicionar exec): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt); // Finalize statement
    return success;
}

// Delete an object from the database based on its primary key.
bool SQLDatabase::excluir(objeto o) {
    if (!db_) {
        cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para excluir." << endl;
        return false;
    }

    objeto normalized_o = o;
    // Normalize primary key name in the object's data
    if (normalized_o.dados.count(toUpper(primary_key_name_))) {
        normalized_o.dados[primary_key_name_] = normalized_o.dados.at(toUpper(primary_key_name_));
        if (toUpper(primary_key_name_) != primary_key_name_) {
            normalized_o.dados.erase(toUpper(primary_key_name_));
        }
    }

    if (!normalized_o.dados.count(primary_key_name_)) {
        cerr << "SQLDatabase ERROR: Chave primaria '" << primary_key_name_ <<
                "' nao encontrada no objeto normalizado para exclusao." << endl;
        return false;
    }

    ostringstream sql_oss;
    sql_oss << "DELETE FROM " << table_name_ << " WHERE " << primary_key_name_ << " = ?;";

    sql_query_str_ = sql_oss.str();
    cout << "SQLDatabase: Excluindo objeto. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (excluir prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ <<
                endl;
        return false;
    }

    bind_value(stmt, 1, normalized_o.dados.at(primary_key_name_), get_sql_type_from_schema(primary_key_name_));

    rc_ = sqlite3_step(stmt);
    bool success = (rc_ == SQLITE_DONE);
    if (!success) {
        cerr << "SQLDatabase ERROR (excluir exec): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return success;
}

// Search for objects in the database by a specific key and value.
vector<objeto> SQLDatabase::buscar(string chave, string valor) {
    vector<objeto> results;
    if (!db_) {
        cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para buscar." << endl;
        return results;
    }

    string normalized_chave = toUpper(chave);

    if (colunas_.find(normalized_chave) == colunas_.end()) {
        cerr << "SQLDatabase ERROR: Chave '" << chave << "' (normalizada para '" << normalized_chave <<
                "') nao e uma coluna valida para busca." << endl;
        return results;
    }

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << " WHERE " << normalized_chave << " = ?;";

    sql_query_str_ = sql_oss.str();
    cout << "SQLDatabase: Buscando objeto. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (buscar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ <<
                endl;
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
        cerr << "SQLDatabase ERROR (buscar step): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return results;
}

// List all objects from a table.
vector<objeto> SQLDatabase::listar() {
    vector<objeto> results;
    if (!db_) {
        cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para listar." << endl;
        return results;
    }

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << ";";

    sql_query_str_ = sql_oss.str();
    cout << "SQLDatabase: Listando objetos. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (listar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ <<
                endl;
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
        cerr << "SQLDatabase ERROR (listar step): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return results;
}

// List objects from a table ordered by a specific key.
vector<objeto> SQLDatabase::listar_ordenado(string chave, bool crescente) {
    vector<objeto> results;
    if (!db_) {
        cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para listar_ordenado." <<
                endl;
        return results;
    }

    string normalized_chave = toUpper(chave);

    if (colunas_.find(normalized_chave) == colunas_.end()) {
        cerr << "SQLDatabase ERROR: Chave '" << chave << "' (normalizada para '" << normalized_chave <<
                "') nao e uma coluna valida para ordenacao." << endl;
        return results;
    }

    ostringstream sql_oss;
    sql_oss << "SELECT * FROM " << table_name_ << " ORDER BY " << normalized_chave << (crescente ? " ASC" : " DESC") <<
            ";";

    sql_query_str_ = sql_oss.str();
    cout << "SQLDatabase: Listando objetos ordenados. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (listar_ordenado prepare): " << sqlite3_errmsg(db_) << " SQL: " <<
                sql_query_str_ << endl;
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
        cerr << "SQLDatabase ERROR (listar_ordenado step): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return results;
}

// Clear (delete all records) from a table.
bool SQLDatabase::limpar() {
    if (!db_) {
        cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para limpar." << endl;
        return false;
    }

    ostringstream sql_oss;
    sql_oss << "DELETE FROM " << table_name_ << ";";

    sql_query_str_ = sql_oss.str();
    cout << "SQLDatabase: Limpando tabela. Query: '" << sql_query_str_ << "'." << endl;

    rc_ = sqlite3_exec(db_, sql_query_str_.c_str(), 0, 0, &zErrMsg_);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (limpar exec): " << zErrMsg_ << endl;
        sqlite3_free(zErrMsg_);
        return false;
    }
    return true;
}

// Update an existing object in the database.
bool SQLDatabase::atualizar(objeto o) {
    if (!db_) {
        cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para atualizar." << endl;
        return false;
    }

    objeto normalized_o = o;
    // Normalize primary key name in the object's data
    if (normalized_o.dados.count(toUpper(primary_key_name_))) {
        normalized_o.dados[primary_key_name_] = normalized_o.dados.at(toUpper(primary_key_name_));
        if (toUpper(primary_key_name_) != primary_key_name_) {
            // If key capitalization changed
            normalized_o.dados.erase(toUpper(primary_key_name_));
        }
    }

    if (!normalized_o.dados.count(primary_key_name_)) {
        cerr << "SQLDatabase ERROR: Chave primaria '" << primary_key_name_ <<
                "' nao encontrada no objeto normalizado para atualizacao." << endl;
        return false;
    }

    ostringstream set_clause_oss;
    bool first = true;
    for (const auto &col_pair: colunas_) {
        // Iterate through DB schema to build SET clause
        if (col_pair.first == primary_key_name_)
            continue; // Skip PK in SET clause
        if (normalized_o.dados.count(col_pair.first)) {
            // Only include columns present in normalized object
            if (!first) {
                set_clause_oss << ", ";
            }
            set_clause_oss << col_pair.first << " = ?";
            first = false;
        }
    }

    ostringstream sql_oss;
    sql_oss << "UPDATE " << table_name_ << " SET " << set_clause_oss.str() << " WHERE " << primary_key_name_ << " = ?;";

    sql_query_str_ = sql_oss.str();
    cout << "SQLDatabase: Atualizando objeto. Query: '" << sql_query_str_ << "'." << endl;

    sqlite3_stmt *stmt;
    rc_ = sqlite3_prepare_v2(db_, sql_query_str_.c_str(), -1, &stmt, 0);
    if (rc_ != SQLITE_OK) {
        cerr << "SQLDatabase ERROR (atualizar prepare): " << sqlite3_errmsg(db_) << " SQL: " << sql_query_str_ <<
                endl;
        return false;
    }

    int bind_index = 1;
    for (const auto &col_pair: colunas_) {
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
        cerr << "SQLDatabase ERROR (atualizar exec): " << sqlite3_errmsg(db_) << endl;
    }
    sqlite3_finalize(stmt);
    return success;
}

// Update or add an object.
bool SQLDatabase::atualizar_ou_adicionar(const objeto o) {
    if (!db_) {
        cerr << "SQLDatabase ERROR: Conexao com o banco de dados nao estabelecida para atualizar_ou_adicionar." <<
                endl;
        return false;
    }

    objeto normalized_o = o;
    // Normalize primary key name in the object's data
    if (normalized_o.dados.count(toUpper(primary_key_name_))) {
        normalized_o.dados[primary_key_name_] = normalized_o.dados.at(toUpper(primary_key_name_));
        if (toUpper(primary_key_name_) != primary_key_name_) {
            normalized_o.dados.erase(toUpper(primary_key_name_));
        }
    }

    if (!normalized_o.dados.count(primary_key_name_)) {
        if (colunas_.count(primary_key_name_) && colunas_.at(primary_key_name_).find("AUTOINCREMENT") !=
            string::npos) {
            return adicionar(normalized_o);
        }
        cerr << "SQLDatabase ERROR: Chave primaria '" << primary_key_name_ <<
                "' nao encontrada no objeto normalizado para decidir entre atualizar/adicionar." << endl;
        return false;
    }

    vector<objeto> existing_objects = buscar(primary_key_name_, normalized_o.dados.at(primary_key_name_));
    if (!existing_objects.empty()) {
        return atualizar(normalized_o);
    }
    else {
        return adicionar(normalized_o);
    }
}

bool SQLDatabase::hasDb() const { return db_ != nullptr; }


// =============================================================================
// JogadorSQLDatabase Implementation
// =============================================================================

// Constructor
// Initializes JogadorSQLDatabase by calling the protected SQLDatabase constructor
// with the database file name and the player table schema.
JogadorSQLDatabase::JogadorSQLDatabase(const string &fileName) : SQLDatabase(fileName, "JOGADOR", // Table name
                                                                             // Column schema for the JOGADOR table
                                                                             {
                                                                                 {"NOME", "TEXT NOT NULL"},
                                                                                 {"APELIDO", "TEXT PRIMARY KEY"},
                                                                                 {"VITORIAS", "INTEGER"},
                                                                                 {"DERROTAS", "INTEGER"},
                                                                                 {"PONTUACOES", "TEXT"}
                                                                                 // Store vector<int> as string (e.g., "10,20,30")
                                                                             },
                                                                             "APELIDO", // Primary key name
                                                                             false // pkAutoIncrement_ is true for ID
) {
    // Check if the base SQLDatabase failed to initialize the database
    if (db_ == nullptr) {
        cerr << "JogadorSQLDatabase ERROR: A classe base SQLDatabase falhou ao inicializar o banco de dados." <<
                endl;
        // Depending on requirements, you might throw an exception here.
    }
}

// Destructor
// No specific action needed here, as the base SQLDatabase destructor
// already handles closing the database connection and releasing resources.
JogadorSQLDatabase::~JogadorSQLDatabase() {
    cout << "JogadorSQLDatabase: Destrutor chamado." << endl;
}
