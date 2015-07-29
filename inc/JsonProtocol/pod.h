#ifndef POD_H
#define POD_H

#include <string>

struct UserPOD
{
    int id = -1;
    std::string username = "";
    std::string password = "";
    std::string fullname = "";
};

#endif // POD_H

