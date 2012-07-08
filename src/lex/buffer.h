/*
 * This file is a part of pl0-compiler.
 *
 * Copyright (C) 2012 xStone <chen_fu_hua@126.com>
 *
 * pl0-compiler is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include<string>

namespace pl0 {

class Buffer {

public:
    explicit Buffer(const std::string& data);
    ~Buffer();

    void setBufferData(const std::string& data);
    std::string getBufferData()const;

    const char* getBufferBegin()const;
    const char* getBufferEnd()const;

    size_t getBufferSize()const;

private:
    char* buffer_begin_;
    char* buffer_end_;

private:
    Buffer(const Buffer&) = delete;
    void operator=(const Buffer&) = delete;

};

}   //end namespace pl0

#endif // BUFFER_H_
