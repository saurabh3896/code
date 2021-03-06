#ifndef CLASSES__H__
#define CLASSES__H__

#include <string>

class Person
{
public:
  Person(std::string const& name): m_name(name) {}
  std::string name();

private:
  std::string m_name;
};


class Animal
{
public:
  Animal(std::string const& name): m_name(name) {}
  std::string name();

private:
  std::string m_name;

};

#endif
