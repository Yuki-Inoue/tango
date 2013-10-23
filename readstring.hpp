#ifndef YUKI_INOUE_READ_STRING_HPP
#define YUKI_INOUE_READ_STRING_HPP

#include <readline/readline.h>
#include <readline/history.h>
#include <ios>

class readstring_error : public std::exception {
public:
    const char *what() const noexcept {
        return "readstring_error";
    }
};

inline std::string readstring(const std::string &prompt) {
    char *read = readline(prompt.c_str());
    if(read == nullptr)
        throw (readstring_error());
    std::string ret(read);
    free(read);
    return ret;
}

#endif
