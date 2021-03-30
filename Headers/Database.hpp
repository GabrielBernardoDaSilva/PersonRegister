#pragma once
#include "iostream"
#include "sqlite3.h"
#include "Person.hpp"
#include "vector"
#include "string"

class Db{
    public:
    sqlite3 *db;
    int connStatus;
    Db(const char* connectionString){
        connStatus = sqlite3_open(connectionString, &db);
        if (connStatus)
        {
            std::cout <<"Can't open database!" <<  std::endl;
            return;
        }
    }
    ~Db(){
        sqlite3_close(db);
    }
};


class DbConnPerson: public Db{
public:
    char* sql;
    char *errorMsg = 0;
    DbConnPerson(const char* connectionString): Db(connectionString){
        sql= "CREATE TABLE PERSON("\
                               "ID INTEGER PRIMARY KEY AUTOINCREMENT,"\
                               "NAME CHAR(50),"
                               "AGE INTEGER,"
                               "WEIGHT REAL)";

        connStatus = sqlite3_exec(db, sql, NULL, 0, &errorMsg);
        if (connStatus == SQLITE_OK)
            std::cout << "Created with success!" << std::endl;
        else
            std::cout << errorMsg << std::endl;
    }

    int insertPerson(const Person& person);
    int deletePerson(int id);
    int updatePerson(const Person& person, int id);
    std::vector<Person> select();
    Person selectById(int id);

};