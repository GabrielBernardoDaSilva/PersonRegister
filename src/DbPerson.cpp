#include "../Headers/Database.hpp"
#include "../Headers/Person.hpp"
#include "memory"
#include "algorithm"

static int Callback(void *data, int argc, char **argv, char **azColName) {

    int id = atoi(argv[0]);
    std::string name = argv[1];
    int age= atoi(argv[2]);
    float weight = atof(argv[3]);
    std::shared_ptr<Person> person = std::make_shared<Person>();
    person->id = id;
    person->name = name;
    person->age = age;
    person->weight = weight;
    std::vector<Person> *people = (std::vector<Person>*) data;
    people->push_back(*person);


    return 0;
}

int DbConnPerson::insertPerson(const Person &person) {
    std::string sqlInsert = "INSERT INTO PERSON(NAME, AGE, WEIGHT)"\
          "VALUES('" + person.name + "', " + std::to_string(person.age) + ", " + std::to_string(person.weight) + ")";
    connStatus = sqlite3_exec(db, sqlInsert.c_str(), NULL, 0, &errorMsg);
    if (connStatus == SQLITE_OK) {
        std::cout << "Recorded!" << std::endl;
        return 1;
    } else {
        std::cout << "Error: " << errorMsg << std::endl;
        return 0;
    }
}

std::vector<Person> DbConnPerson::select() {
    std::string sqlSelect = "SELECT * FROM PERSON";
    std::vector<Person> people;
    connStatus = sqlite3_exec(db, sqlSelect.c_str(), Callback, &people, &errorMsg);
    return people;
}
int DbConnPerson::deletePerson(int id) {
    std::string sqlDelete = "DELETE FROM PERSON WHERE ID="+ std::to_string(id);
    connStatus = sqlite3_exec(db, sqlDelete.c_str(), NULL, 0, &errorMsg);
    if (connStatus == SQLITE_OK)
    {
        std::cout << "Deleted!" << std::endl;
        return 1;
    }else{
        std::cout << "Error: " << errorMsg << std::endl;
        return 0;
    }
}

int DbConnPerson::updatePerson(const Person &person, int id) {
    std::string sqlUpdate = "UPDATE PERSON SET NAME='"+person.name+"'"\
                            ", AGE="+std::to_string(person.age)+""\
                            ",WEIGHT="+std::to_string(person.weight)+" "\
                            "WHERE ID="+std::to_string(person.id);
    connStatus = sqlite3_exec(db, sqlUpdate.c_str(), NULL, 0, &errorMsg);
    if (connStatus == SQLITE_OK)
    {
        std::cout << "Updated!" << std::endl;
        return 1;
    }else{
        std::cout << "Error: " << errorMsg << std::endl;
        return 0;
    }
}

Person DbConnPerson::selectById(int id) {
    std::vector<Person> people = this->select();
    auto it = std::find_if(people.begin(), people.end(), [&id](Person p){
        return p.id == id;
    });
    if (it == people.end())
        return Person();
    Person p = *it;
    return p;
}