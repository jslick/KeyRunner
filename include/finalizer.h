/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#pragma once

#include <functional>
#include <utility>

class Finalizer
{
public:
    Finalizer(std::function<void()>&& callback)
        : callback(std::forward<std::function<void()>>(callback))
    {}

    ~Finalizer()
    {
        try
        {
            this->doNow();
        }
        catch (...)
        {
            // Throwing exceptions from destructor is undefined.
            // Your callback should catch all exceptions.
        }
    }

    void doNow()
    {
        if (this->callback)
        {
            this->callback();
            this->callback = nullptr;
        }
    }

private:
    std::function<void()> callback;
};
