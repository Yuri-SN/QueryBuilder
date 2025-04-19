#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class SqlSelectQueryBuilder {
  public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        m_columns.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& tableName) {
        m_table = tableName;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) {
        m_whereConditions[key] = value;
        return *this;
    }

    std::string BuildQuery() const {
        if (m_table.empty()) {
            throw std::runtime_error("Table name is not specified");
        }

        std::string query = "SELECT ";

        if (m_columns.empty()) {
            query += "* ";
        } else {
            for (size_t i{0}; i < m_columns.size(); ++i) {
                query += m_columns[i];
                if (i != m_columns.size() - 1) {
                    query += ", ";
                }
            }
            query += " ";
        }

        query += "FROM " + m_table;

        if (!m_whereConditions.empty()) {
            query += " WHERE ";
            size_t conditionCount = 0;
            for (const auto& [key, value] : m_whereConditions) {
                query += key + "=" + value;
                if (conditionCount != m_whereConditions.size() - 1) {
                    query += " AND ";
                }
                ++conditionCount;
            }
        }

        query += ";";

        return query;
    }

  private:
    std::vector<std::string> m_columns;
    std::string m_table;
    std::map<std::string, std::string> m_whereConditions;
};

int main() {
    std::string queryResult;

    // Пример из задания
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    queryResult = query_builder.BuildQuery();
    std::cout << queryResult << std::endl;
    assert(queryResult == "SELECT name, phone FROM students WHERE id=42 AND name=John;");

    // Без колонок
    SqlSelectQueryBuilder queryBuilder1;
    queryBuilder1.AddFrom("students");

    queryResult = queryBuilder1.BuildQuery();
    std::cout << queryResult << std::endl;
    assert(queryResult == "SELECT * FROM students;");

    // Без условий WHERE
    SqlSelectQueryBuilder queryBuilder2;
    queryBuilder2.AddColumn("name").AddColumn("phone");
    queryBuilder2.AddFrom("students");

    queryResult = queryBuilder2.BuildQuery();
    std::cout << queryResult << std::endl;
    assert(queryResult == "SELECT name, phone FROM students;");

    return 0;
}
