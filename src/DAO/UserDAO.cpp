#include "DAO/UserDAO.hpp"

#include <debug_utils/log_debug.hpp>

UserDAO::UserDAO() :
    p_con(nullptr)
{
    sql::Driver* driver = get_driver_instance();
    p_con = driver->connect("localhost:3306", "root", "root");
    LOG_DEBUG("DB CONNECTION: %s\n", p_con->isValid() ? "valid" : "invalid");
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

Contacts UserDAO::getContacts(int userId)
{
    LOG_DEBUG("Getting contacts for %d\n",userId);
    sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
        "Select User.id, User.username, User.fullname from User left join"
        " Contact on User.id=Contact.user2Id where Contact.user1Id=?");

    prep_stmt->setInt(1,userId);
    sql::ResultSet* res = prep_stmt->executeQuery();

    Contacts contacts(res->rowsCount());
    int count = 0;
    while (res->next())
    {
        int id = res->getInt("id");
        std::string username = res->getString("username");
        std::string fullname = res->getString("fullname");
        contacts[count++] = Contact(id, username, fullname);
    }
    LOG_DEBUG("No. contacts: %d\n",count);
    return contacts;

}
