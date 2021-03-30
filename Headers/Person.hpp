#pragma once
#include "iostream"

class Person{
public:
    int id;
    std::string name;
    int age;
    float weight;
    Person(){};
    Person(std::string vName, int vAge, float vWeight): name(vName), age(vAge), weight(vWeight) {};


};