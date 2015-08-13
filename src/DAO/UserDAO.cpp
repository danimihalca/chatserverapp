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
	LOG_DEBUG("VALID:%d\n", res->rowsCount());
	bool valid = res->rowsCount() > 0;
	prep_stmt->close();
    return valid;
}

UserDetails UserDAO::getUserDetails(const std::string& userName)
{
    sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
        "Select id, firstname, lastname from User where username=?");
    prep_stmt->setString(1,userName);
    sql::ResultSet* res = prep_stmt->executeQuery();
    if (res->next())
    {
        int id = res->getInt("id");
        std::string firstName = res->getString("firstname");
		std::string lastName = res->getString("lastname");
		prep_stmt->close();
		return UserDetails(id, firstName, lastName);
    }
	prep_stmt->close();
	return UserDetails();
}

std::vector<Contact> UserDAO::getContacts(int userId)
{
    LOG_DEBUG("Getting contacts for %d\n",userId);
    sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
        "Select User.id, User.username, User.firstname, User.lastname from User left join"
        " ContactRelation on User.id=ContactRelation.user2Id where ContactRelation.user1Id=?");

    prep_stmt->setInt(1,userId);
    sql::ResultSet* res = prep_stmt->executeQuery();

	std::vector<Contact> contacts(res->rowsCount());
    int count = 0;
    while (res->next())
    {
        int id = res->getInt("id");
        std::string username = res->getString("username");
        std::string firstname = res->getString("firstname");
		std::string lastname = res->getString("lastname");
		contacts[count++] = Contact(id, username, firstname,lastname);
    }
	prep_stmt->close();
	LOG_DEBUG("No. contacts: %d\n", count);
    return contacts;

}
BaseUser UserDAO::getBaseUser(int userId)
{
	LOG_DEBUG("Geting user with  with id %d\n", userId);
	BaseUser user;
	sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
		"Select id, username, firstname, lastname from User where id=?");

	prep_stmt->setInt(1, userId);
	sql::ResultSet* res = prep_stmt->executeQuery();

	std::vector<Contact> contacts(res->rowsCount());
	int count = 0;
	if (res->next())
	{
		auto idd = res->getInt("id");
		user.setId(idd);
		user.setUserName(res->getString("username"));
		user.setFirstName(res->getString("firstname"));
		user.setLastName(res->getString("lastname"));
	}
	prep_stmt->close();
	return user;
}

BaseUser UserDAO::getBaseUser(const std::string& userName)
{
	LOG_DEBUG("Geting user with  with username %d\n", userName.c_str());
	BaseUser user;
	sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
		"Select id, username, firstname, lastname from User where username=?");

	prep_stmt->setString(1, userName);
	sql::ResultSet* res = prep_stmt->executeQuery();

	std::vector<Contact> contacts(res->rowsCount());
	int count = 0;
	if (res->next())
	{
		auto idd = res->getInt("id");
		user.setId(idd);
		user.setUserName(res->getString("username"));
		user.setFirstName(res->getString("firstname"));
		user.setLastName(res->getString("lastname"));
	}
	else
	{
		user.setId(-1);
	}
	prep_stmt->close();
	return user;
}

void UserDAO::addContactRelation(int user1Id, int user2Id)
{
	sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
		"insert into contactrelation (user1Id, user2id) values (?, ?)");
	prep_stmt->setInt(1, user1Id);
	prep_stmt->setInt(2, user2Id);
	prep_stmt->executeUpdate();
	prep_stmt->close();
}

void UserDAO::removeContactRelation(int user1Id, int user2Id)
{
	sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
		"delete from ContactRelation where user1Id=? and user2id=?");
	prep_stmt->setInt(1, user1Id);
	prep_stmt->setInt(2, user2Id);
	prep_stmt->executeUpdate();
	prep_stmt->close();
}
bool  UserDAO::isContactRelation(int user1Id, int user2Id)
{
	sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
		"select id from ContactRelation where user1Id=? and user2id=?");
	prep_stmt->setInt(1, user1Id);
	prep_stmt->setInt(2, user2Id);
	prep_stmt->executeQuery();
		bool contactrelation = prep_stmt->executeQuery()->rowsCount() > 0;
		prep_stmt->close();
	return (contactrelation);
}


void UserDAO::registerUser(const User& user)
{
	sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
		"insert into user (username, password, firstname, lastname) values (?, ?, ?, ?)");
	prep_stmt->setString(1, user.getUserName());
	prep_stmt->setString(2, user.getPassword());
	prep_stmt->setString(3, user.getFirstName());
	prep_stmt->setString(4, user.getLastName());
	prep_stmt->executeUpdate();
	prep_stmt->close();
}

void UserDAO::updateUser(const User& user)
{
	sql::PreparedStatement* prep_stmt = p_con->prepareStatement(
		"update user set username=?, password=?, firstname=?, lastname=? where id=?");
	prep_stmt->setString(1, user.getUserName());
	prep_stmt->setString(2, user.getPassword());
	prep_stmt->setString(3, user.getFirstName());
	prep_stmt->setString(4, user.getLastName());
	prep_stmt->setInt(4, user.getId());
	prep_stmt->executeUpdate();
	prep_stmt->close();
}