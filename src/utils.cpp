#include "utils.h"
#include <fstream>
#include <sstream>

int load_text (const char *fp, std::string &out) {
    std::ifstream text(fp);
    if(!text.is_open()) { return -1; }
    std::stringstream text_stream;
    text_stream << text.rdbuf();
    text.close();
    out = text_stream.str();
    return 1;
}