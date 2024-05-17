#pragma once
#include <string>
#include "xml.h"

using namespace std;

namespace xml
{
  class Parser
  {
  private:
    string m_str;
    int m_pos;

    Xml parser_node();
    void skip_white_space();
    bool skip_declaration();
    bool skip_comment();
    string parser_node_name();
    string parser_node_text();

  public:
    Parser() : m_str(""), m_pos(0){};
    Xml parser();

    bool load_file(const string &fileName);
  };
}