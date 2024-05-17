#include <cstring>
#include <vector>

using std::vector; using std::string;

vector<string> split(string str, string delim, const int limit = 2)
{
    vector<string> tokens;
    size_t start = 0, pos = 0;
    int found = 0;

    while(start < str.length())
    {
        pos = str.find(delim, start);
        if(pos == string::npos || found == limit - 1)
            pos = str.length();

        string token = str.substr(start, pos - start);

        if(!token.empty())
            tokens.push_back(token);
        start = pos + delim.length();
        found++;
    }
    return tokens;
}
