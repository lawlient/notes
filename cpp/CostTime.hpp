#pragma once

#include <time.h>                                                                
// #include <sys/time.h>                                                            
#include <chrono>                                                                

#include <string>
#include <iostream>


class CostTime
{
public:
    CostTime() = delete;
    CostTime(const std::string& tag) : tag_(tag) {
        start_ = std::chrono::system_clock::now();
    }

    ~CostTime() {
        stop_ = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(stop_ - start_);
        std::cout << "[" << tag_ << "] cost " << ms.count() << "ms";
    }

private:
    std::chrono::system_clock::time_point start_;
    std::chrono::system_clock::time_point stop_;

    std::string tag_;
};
