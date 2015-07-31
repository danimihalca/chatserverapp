#include "DAO/UserDAO.h"

#include "JsonProtocol/pod.h"
#include <debug_utils/log_debug.h>

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

bool UserDAO::isValidUser(UserPOD& userCredentials)
{
    sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
        "Select * from User where username=? and password=?");
    prep_stmt->setString(1,userCredentials.username);
    prep_stmt->setString(2,userCredentials.password);
    sql::ResultSet* res = prep_stmt->executeQuery();

    if (res->next())
    {
        userCredentials.id = res->getInt("id");
        userCredentials.fullname = res->getString("fullname");
        return true;
    }
    return false;
}
