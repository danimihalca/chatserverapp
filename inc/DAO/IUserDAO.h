#ifndef IUSERDAO_H
#define IUSERDAO_H

struct UserPOD;

class IUserDAO
{

    virtual bool isValidUser(UserPOD& userCredentials) = 0;

};

#endif // IUSERDAO_H

