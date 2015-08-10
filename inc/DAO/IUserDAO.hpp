#ifndef IUSERDAO_HPP
#define IUSERDAO_HPP

#include <vector>

#include <Model/User.hpp>

class IUserDAO
{
public:
    virtual bool isValidUser(const UserCredentials& userCredentials) = 0;
    virtual UserDetails getUserDetails(const std::string& userName) = 0;
    virtual std::vector<Contact> getContacts(int userId) = 0;


    virtual ~IUserDAO()
    {
    }
};

#endif // IUSERDAO_HPP

