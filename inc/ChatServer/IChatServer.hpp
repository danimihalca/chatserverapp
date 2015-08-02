#ifndef ICHATSERVER_HPP
#define ICHATSERVER_HPP


class IChatServer
{
public:
    virtual void run() = 0;

    virtual ~IChatServer()
    {
    }
};

#endif // ICHATSERVER_HPP

