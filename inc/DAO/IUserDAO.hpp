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
	virtual BaseUser getBaseUser(int userId) = 0;
	virtual BaseUser getBaseUser(const std::string& userName) = 0;
	virtual void addContactRelation(int user1Id, int user2Id) = 0;
	virtual void removeContactRelation(int user1Id, int user2Id) = 0;
	virtual bool isContactRelation(int user1Id, int user2Id) = 0;

	virtual void registerUser(const User& user) = 0;
	virtual void updateUser(const User& user) = 0;

    virtual ~IUserDAO()
    {
    }
};

#endif // IUSERDAO_HPP

