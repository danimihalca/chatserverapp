#ifndef IUSERDAO_H
#define IUSERDAO_H

#include <Model/User.h>

class IUserDAO
{
public:
    virtual bool isValidUser(const UserCredentials& userCredentials) = 0;
    virtual UserDetails getUserDetails(const std::string& userName) = 0;
    virtual ~IUserDAO()
    {
    }
};

#endif // IUSERDAO_H

