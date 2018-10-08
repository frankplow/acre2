#pragma once

#include "_CONSTANTS.h"
#include "Macros.h"
#include "compat.h"
#include "Types.h"

//typedef ACRE_RESULT (*IServerCallback)(IServer *, IMessage *msg, void *data);

class IServer
{
public:
    virtual ~IServer(){}

    virtual ACRE_RESULT initialize(void) = 0;
    virtual ACRE_RESULT shutdown(void) = 0;

    virtual ACRE_RESULT sendMessage(IMessage *const msg) = 0;
    virtual ACRE_RESULT handleMessage(unsigned char *const data) = 0;
    virtual ACRE_RESULT release(void) = 0;

    virtual void setConnected(const bool value) = 0;
    virtual bool getConnected() const = 0;

    virtual void setId(const ACRE_ID value) = 0;
    virtual ACRE_ID getId() const = 0;
};
