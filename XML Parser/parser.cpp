#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace xml;

bool Parser::load_file(const string &fileName)
{
  ifstream fin(fileName);
  if (fin.fail())
    return false;

  stringstream ss;
  ss << fin.rdbuf();
  m_str = ss.str();
  m_pos = 0;
  fin.close();
  return true;
}

bool Parser::skip_declaration()
{
  m_pos += 5; // skip "<?xml"
  size_t pos = m_str.find("?>", m_pos);
  if (pos == string::npos)
    return false;

  m_pos = pos + 2; // skip "?>"
  return true;
}

bool Parser::skip_comment()
{
  m_pos += 4; // skip "<!--"
  size_t pos = m_str.find("-->", m_pos);
  if (pos == string::npos)
    return false;

  m_pos = pos + 3; // skip "-->"
  return true;
}

void Parser::skip_white_space()
{
  while (m_str[m_pos] == ' ' || m_str[m_pos] == '\n' ||
         m_str[m_pos] == '\r' || m_str[m_pos] == '\t')
  {
    ++m_pos;
  }
}

Xml Parser::parser()
{
  skip_white_space();
  /*
  // declaration
  if (m_str.compare(m_pos, 5, "<?xml") == 0)
  {
    if (!skip_declaration())
      throw logic_error("Parser declaration error!!!");
  }
  skip_white_space();
  */

  /*
  // comment
  while (m_str.compare(m_pos, 4, "<!--") == 0)
  {
    if (!skip_comment())
      throw logic_error("Parser comment error!!!");
    skip_white_space();
  }
  */
  if (m_str[m_pos] == '<' &&
      (isalpha(m_str[m_pos + 1]) || m_str[m_pos + 1] == '_')) // check legal node name
  {
    return parser_node();
  }

  throw logic_error("Parser node error!!!");
}

Xml Parser::parser_node()
{
  Xml node("init");

  ++m_pos; // skip '<'
  string name = parser_node_name();
  // cout << "Parser Debug: " << name << endl;
  node.setName(name); // set new name
  while (m_str[m_pos] != '\0')
  {
    skip_white_space();
    if (m_str[m_pos] == '>')
    {
      ++m_pos; // skip '>'
      string text = parser_node_text();
      if (text != "")
        node.setText(text);
    }
    else if (m_str[m_pos] == '<')
    {
      if (m_str[m_pos + 1] == '/')
      {
        // label
        string end_tag = "</" + name + ">";
        size_t s_pos = m_str.find(end_tag, m_pos);
        m_pos = s_pos + end_tag.size();
        break;
      }
      /*
      else if (m_str.compare(m_pos, 4, "<!--") == 0)
      {
        // comment
        if (!skip_comment())
          throw logic_error("Parser comment error!!!");
      }
      */
      else
      {
        // sub node
        node.addChild(parser_node());
      }
    }
  }

  return node;
}

string Parser::parser_node_name()
{
  int s_pos = m_pos; // start
  ++m_pos;           // skip the first alpha or '_'
  while (isalnum(m_str[m_pos]) || m_str[m_pos] == '_' || m_str[m_pos] == '-' ||
         m_str[m_pos] == ':' || m_str[m_pos] == '.')
  { // does not stop until get '>'
    ++m_pos;
  }

  return m_str.substr(s_pos, m_pos - s_pos);
}

string Parser::parser_node_text()
{
  int s_pos = m_pos;
  while (m_str[m_pos] != '<')
    ++m_pos;

  return m_str.substr(s_pos, m_pos - s_pos);
}