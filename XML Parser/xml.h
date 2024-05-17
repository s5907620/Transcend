#pragma once

#include <string>
#include <list>
#include <stdexcept>

using namespace std;

namespace xml
{
  class Xml
  {
  private:
    Xml();
    string *n_name; // node name
    string *n_text; // node text
    list<Xml> *n_child;

  public:
    Xml(const string &name);
    Xml(const Xml &copy);

    void setName(const string &name);
    void setText(const string &text);
    void addChild(const Xml &child);
    void clear();
    string getName() const;
    string getText() const;
    string outputStr() const;
    int size() const { return n_child->size(); };

    Xml &operator[](int index);
    Xml &operator=(const Xml &copy);

    // file parser
    bool load(const string &fileName);
  };
} // namespace xml