#ifndef BASE_H
#define BASE_H

class Base {

public:

    virtual ~Base() {};
    virtual const char* className() const = 0;

};

#endif

