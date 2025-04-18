#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class SqlSelectQueryBuilder {
  private:
    std::vector<std::string> columns;
    std::string table;
    std::map<std::string, std::string> whereConditions;

  public:
    // Добавление колонок
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns.push_back(column);
        return *this;
    }

    // Установка таблицы
    SqlSelectQueryBuilder& AddFrom(const std::string& tableName) {
        table = tableName;
        return *this;
    }

    // Добавление условий WHERE
    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) {
        whereConditions[key] = value;
        return *this;
    }

    // Построение итогового запроса
    std::string BuildQuery() const {
        std::string query = "SELECT ";

        // Если колонки не добавлены, используем "*"
        if (columns.empty()) {
            query += "* ";
        } else {
            // Объединяем колонки через запятую
            for (size_t i = 0; i < columns.size(); ++i) {
                query += columns[i];
                if (i != columns.size() - 1) {
                    query += ", ";
                }
            }
            query += " ";
        }

        // Добавляем таблицу
        if (!table.empty()) {
            query += "FROM " + table + " ";
        } else {
            throw std::runtime_error("Table name is not specified");
        }

        // Добавляем условия WHERE
        if (!whereConditions.empty()) {
            query += "WHERE ";
            size_t conditionCount = 0;
            for (const auto& [key, value] : whereConditions) {
                query += key + "=" + value;
                if (conditionCount != whereConditions.size() - 1) {
                    query += " AND ";
                }
                ++conditionCount;
            }
            query += " ";
        }

        // Добавляем точку с запятой в конце
        query += ";";

        return query;
    }
};

int main() {
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    std::cout << query_builder.BuildQuery() << std::endl;

    assert(query_builder.BuildQuery() == "SELECT name, phone FROM students WHERE id=42 AND name=John;");

    return 0;
}
