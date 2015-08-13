#ifndef USERDAO_HPP
#define USERDAO_HPP

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "IUserDAO.hpp"

class UserDAO : public IUserDAO
{
public:
    UserDAO();
    ~UserDAO();

    // Implements IUserDAO interface
public:
    bool isValidUser(const UserCredentials& userCredentials);
    UserDetails getUserDetails(const std::string& userName);
	std::vector<Contact> getContacts(int userId);
	BaseUser getBaseUser(int userId);
	BaseUser getBaseUser(const std::string& userName);
	void addContactRelation(int user1Id, int user2Id);
	void removeContactRelation(int user1Id, int user2Id);
	bool isContactRelation(int user1Id, int user2Id);

	void registerUser(const User& user);
	void updateUser(const User& user);

private:
    sql::Connection* p_con;
};

#endif // USERDAO_HPP

