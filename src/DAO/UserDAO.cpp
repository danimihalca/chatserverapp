#include "DAO/UserDAO.hpp"

#include <debug_utils/log_debug.hpp>

UserDAO::UserDAO() :
    p_con(nullptr)
{
    sql::Driver* driver = get_driver_instance();
    p_con = driver->connect("localhost:3306", "root", "root");
    log_debug("DB CONNECTION: %s\n", p_con->isValid() ? "valid" : "invalid");
    p_con->setSchema("ChatDatabase");
}

UserDAO::~UserDAO()
{
    if (p_con->isValid() && !p_con->isClosed())
    {
        p_con->close();
    }
    delete p_con;
}

bool UserDAO::isValidUser(const UserCredentials& userCredentials)
{
    sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
        "Select * from User where username=? and password=?");
    prep_stmt->setString(1,userCredentials.getUserName());
    prep_stmt->setString(2,userCredentials.getPassword());
    sql::ResultSet* res = prep_stmt->executeQuery();

    return res->rowsCount() > 0;
}

UserDetails UserDAO::getUserDetails(const std::string& userName)
{
    sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
        "Select id, fullname from User where username=?");
    prep_stmt->setString(1,userName);
    sql::ResultSet* res = prep_stmt->executeQuery();
    if (res->next())
    {
        int id = res->getInt("id");
        std::string fullName = res->getString("fullname");
        return UserDetails(id,fullName);
    }
    return UserDetails();
}
