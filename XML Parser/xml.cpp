#include "xml.h"
#include "parser.h"
#include <sstream>

using namespace std;
using namespace xml;

Xml::Xml() : n_name(nullptr), n_text(nullptr), n_child(nullptr) {}

Xml::Xml(const string &name) : n_text(nullptr), n_child(nullptr)
{
  n_name = new string(name);
}

Xml::Xml(const Xml &copy)
{
  this->n_name = copy.n_name;
  this->n_text = copy.n_text;
  this->n_child = copy.n_child;
}

void Xml::setName(const string &name)
{
  if (n_name != nullptr)
  {
    delete n_name;
    n_name = nullptr;
  }
  n_name = new string(name);
}

string Xml::getName() const
{
  if (n_name == nullptr)
    return "";
  return *n_name;
}

void Xml::setText(const string &text)
{
  if (n_text != nullptr)
  {
    delete n_text;
    n_text = nullptr;
  }
  n_text = new string(text);
}

string Xml::getText() const
{
  if (n_text == nullptr)
    return "";
  return *n_text;
}

void Xml::addChild(const Xml &child)
{
  if (n_child == nullptr)
  {
    n_child = new list<Xml>();
  }
  n_child->push_back(child);
}

string Xml::outputStr() const
{
  if (n_name == nullptr)
    throw logic_error("name is empty.");

  stringstream ss;
  bool show = false;

  // name
  if (*n_name == "SN" || *n_name == "PN" || *n_name == "IO" ||
      *n_name == "CTL" || *n_name == "FW")
  {
    show = true;
    if (*n_name == "SN")
      ss << "[product]\n";

    ss << *n_name << ": ";
  }

  if (n_child != nullptr)
  {
    for (auto it = n_child->begin(); it != n_child->end(); ++it)
      ss << it->outputStr();
  }

  if (show && n_text != nullptr)
    ss << *n_text;

  if (show)
    ss << endl;

  return ss.str();
}

Xml &Xml::operator[](int index)
{
  if (index < 0)
    throw logic_error("Index is less than 0.");

  if (n_child == nullptr)
    n_child = new list<Xml>();

  int size = n_child->size();
  if (index >= 0 && index < size)
  {
    auto it = n_child->begin();
    for (int i = 0; i <= index; ++i)
      ++it;
    return *it;
  }

  if (index == size)
    n_child->push_back(Xml());
  else
    throw logic_error("Index is illegal.");

  return n_child->back();
}

Xml &Xml::operator=(const Xml &copy)
{
  clear();
  this->n_name = copy.n_name;
  this->n_text = copy.n_text;
  this->n_child = copy.n_child;
  return *this;
}

void Xml::clear()
{
  if (n_name != nullptr)
  {
    delete n_name;
    n_name = nullptr;
  }
  if (n_text != nullptr)
  {
    delete n_text;
    n_text = nullptr;
  }
  if (n_child != nullptr)
  {
    for (auto it = n_child->begin(); it != n_child->end(); ++it)
      it->clear();

    delete n_child;
    n_child = nullptr;
  }
}

bool Xml::load(const string &fileName)
{
  Parser p;
  if (!p.load_file(fileName))
    return false;

  *this = p.parser();
  return true;
}