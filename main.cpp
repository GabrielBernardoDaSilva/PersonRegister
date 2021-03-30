#include <iostream>
#include "Headers/Person.hpp"
#include "Headers/Database.hpp"
#include <cpprest/http_listener.h>
#include "nlohmann/json.hpp"

#define TRACE(msg)            std::wcout << msg

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

DbConnPerson dataBase("people.db");

static void Print(const std::vector<Person> &people) {
    for (auto person: people) {
        std::cout << "Id: " << person.id << ",Name: " << person.name << ", Age: " << person.age << ", Weight: "
                  << person.weight << std::endl;
    }
}

void handle_get_person(const http_request &request) {
    TRACE(L"\nhandle GET\n");
    std::string query = request.relative_uri().query();
    json::value answer;
    int i = 0;
    auto people = dataBase.select();

    if (query.empty()) {
        for (const auto &person : people) {
            json::value obj;
            obj["id"] = person.id;
            obj["name"] = json::value::string(person.name);
            obj["age"] = person.age;
            obj["weight"] = person.weight;
            answer[i] = obj;
            i++;
        }

    } else {
        std::string key = query.replace(query.begin(), query.begin() + 3, "");
        int id = atoi(key.c_str());
        Person person = dataBase.selectById(id);
        if (person.name == "") {
            request.reply(status_codes::NotFound);
            return;
        }
        answer["id"] = person.id;
        answer["name"] = json::value::string(person.name);
        answer["age"] = person.age;
        answer["weight"] = person.weight;

    }
    request.reply(status_codes::OK, answer);
}

void handle_request(http_request const &request,
                    std::function<void(json::value const &, json::value &)> action) {
    auto answer = json::value::object();
    request
            .extract_json()
            .then([&answer, &action](pplx::task<json::value> const &task) {
                try {
                    auto const &jvalue = task.get();
                    if (!jvalue.is_null())
                        action(jvalue, answer);

                } catch (std::exception const &e) {
                    std::cout << e.what() << std::endl;
                }
            }).wait();


}

void handle_post_person(const http_request &request) {
    handle_request(request, [&request](const json::value &jvalue, json::value &answer) {
        std::shared_ptr<Person> p = std::make_shared<Person>();
        auto req = jvalue.as_object();
        p->name = req["name"].as_string();
        p->age = req["age"].as_integer();
        p->weight = req["weight"].as_double();

        dataBase.insertPerson(*p);

        request.reply(status_codes::OK);
    });

}

void handle_delete_person(const http_request &request) {
    auto query = request.absolute_uri().query();
    if (!query.empty()) {
        auto id = query.replace(query.begin(), query.begin() + 3, "");
        int status = dataBase.deletePerson(atoi(id.c_str()));
        if (status == 1)
            request.reply(status_codes::OK);
        else
            request.reply(status_codes::NotFound);
    }
}


void handle_update_person(const http_request &request) {
    handle_request(request, [&request](const json::value &jvalue, json::value &answer) {
        auto req = jvalue.as_object();
        std::shared_ptr<Person> p = std::make_shared<Person>();

        p->id = req["id"].as_integer();
        p->name = req["name"].as_string();
        p->age = req["age"].as_integer();
        p->weight = req["weight"].as_double();

        int status = dataBase.updatePerson(*p, p->id);
        if (status == 1)
            request.reply(status_codes::OK);
        else
            request.reply(status_codes::NotFound);

    });
}

int main() {
    http_listener listener("http://localhost:9080");

    listener.support(methods::GET, handle_get_person);
    listener.support(methods::POST, handle_post_person);
    listener.support(methods::DEL, handle_delete_person);
    listener.support(methods::PUT, handle_update_person);

    try {
        int num{1};
        listener
                .open()
                .then([]() { TRACE("SERVER STARTED AND LISTEN!"); })
                .wait();
        while (num == 1)
            std::cin >> num;
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
