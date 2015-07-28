#ifndef ICHATSERVER_H
#define ICHATSERVER_H


class IChatServer
{
public:
    virtual void run() = 0;
    virtual ~IChatServer()
    {
    }
};

#endif // ICHATSERVER_H

