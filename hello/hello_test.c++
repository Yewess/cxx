
#include <pstream.h>
#include <iostream>
#include <string>
#include <locale>
#include "hello.h"

std::string trim(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return std::string(it, rit.base());
}

int main(int argc, char* argv[]) {
    using namespace std;
    const string command("./hello");
    redi::pstream pstream(command);
    vector<std::string> stdoutlines, stderrlines;
    while (pstream.out()) {
        string line;
        getline(pstream.out(), line);
        stdoutlines.push_back(line + string("\n"));
    }
    while (pstream.err()) {
        string line;
        getline(pstream.out(), line);
        stderrlines.push_back(line + string("\n"));
    }
    if (stderrlines.size() == 0) {
        if (stdoutlines.size() >= 1) {
            string line(trim(stdoutlines.front()));
            if (line.compare(Hello::get_message()) == 0)
                exit(EXIT_SUCCESS);
            else
                cerr << "\"" << line << "\" != \"" << Hello::get_message()
                     << "\"" << endl;
        } else
            cerr << "zero-length stdout output" << endl;
    } else
        cerr << "non-zero length stderr output" << endl;
    exit(EXIT_FAILURE);
}
