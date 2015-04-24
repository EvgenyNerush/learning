// regex.cpp
// reads variables from conf file that contains variable names, values,
// and comments, for example:
// xi     5
// theta0   10 # comment
// theta1   20
// c        4.5 # would not be read, only int values are allowed

#include <fstream> // for file reading
#include <iostream> // for cout
#include <regex> // for file parsing
#include <sstream> // for reading regex output
#include <map> // for storage of variables
using namespace std;

void parse(map<string, int>&, string);

int main() {
  int theta0, theta1;
  map<string, int> m; // variable name, variable value
  parse(m, "conf"); /* now map contains keys (variable names)
   and values (variable values) from the file "conf" */
  map<string, int>::iterator it;
  it = m.find("theta0");
  if (it != m.end()) // theta0 is found
    cout << "theta0 = " << (theta0 = it->second) << '\n'; // m["theta0"]
  else
    cout << "theta0 not found\n";
  //
  it = m.find("theta1");
  if (it != m.end())
    cout << "theta1 = " << (theta1 = it->second) << '\n';
  else
    cout << "theta1 not found\n";
  return 0;
}

// reads file "filename" and writes variable name and value
// to the map "namevalue"
void parse(map<string, int>& namevalue, string filename) {
  fstream fs;
  fs.open(filename, ios_base::in);
  if (!fs) {
    cout << "parse: file not found!\n";
  } else {
    cout << "parse: file is open!\n";
    string line;
    regex pattern {
        "^([[:alnum:]]+)[[:space:]]+([[:digit:]]+)[[:space:]]*(#.*)?$" };
    /* ^ - start of line, alnum - alphabetic or digit, + - one or more,
     * space - space, tab etc., * - zero or more, . - any single character,
     * $ - end of line */
    // parentheses define submathces
    string format { "$1\n$2\n" }; // $1 - first submatch, $2 -  second submatch
    while (getline(fs, line)) {
      cout << "parse: file line: " << line << '\n';
      stringstream s;
      s
          << regex_replace(line, pattern, format,
              regex_constants::format_no_copy); // only parsing results
      if (s.str() == "") {
        cout << "parse: no match in the string!\n";
      } else {
        string name;
        int value;
        s >> name;
        s >> value;
        cout << "parse: name = " << name << ", value = " << value << '\n';
        namevalue[name] = value;
      }
    }
  }
}
