#ifndef INCLUDED__BIOAPPLICATION__MYEXEPTIONS__HPP
#define INCLUDED__BIOAPPLICATION__MYEXEPTIONS__HPP
#include <stdexcept>
#include <sstream>

/**
 *****************************************************************************
 *
 *  This header defines execptions for the Application
 *
 *****************************************************************************/

class CreatureLoadException : public std::exception
{
public:
    CreatureLoadException(){}
    virtual ~CreatureLoadException() noexcept{}
    virtual const char* what() const noexcept
        { return "CreatureLoadException"; }
};


class TGALoadException : public std::exception
{
public:
    TGALoadException(){}
    virtual ~TGALoadException() noexcept {}
    virtual const char* what() const noexcept
        { return "TGALoadException"; }
};

class AStarException : public std::exception
{
public:
    AStarException(){}
    virtual ~AStarException() noexcept{}
    virtual const char* what() const noexcept
        { return "AStarException"; }
};

#endif // INCLUDED__BIOAPPLICATION__MYEXEPTIONS__HPP
