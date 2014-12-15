#include "helper.h"

void normalize_text(String& txt, bool should_cap) {
    bool read_first_char = true;
    for(unsigned int i = 0; i < txt.size(); i++) {
        if (read_first_char && std::isalpha(txt[i]) && should_cap) {
            txt[i] = static_cast<char>(toupper(txt[i]));
            read_first_char = false;
        }
        else {
            txt[i] = static_cast<char>(tolower(txt[i]));
        }
    }
}
