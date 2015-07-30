#ifndef USERDAO_H
#define USERDAO_H

// #include <mysql/mysql.h>

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "IUserDAO.h"

class UserDAO: public IUserDAO
{
public:
    UserDAO();
    ~UserDAO();

    // Implements IUserDAO interface
public:
    bool isValidUser(UserPOD& userCredentials);

private:
    sql::Connection* p_con;
};

#endif // USERDAO_H

