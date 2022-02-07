/**
 * @file    Assertion.cpp
 * @author  Y.Nakaue
 */

#include "Assertion.hpp"

#include <iostream>

void SDFW_Assertion::OutputLog(std::string message)
{
    std::cout << "SDFW_Log: " << message << std::endl;
}

void SDFW_Assertion::Abort(std::string message)
{
    std::cout << "SDFW_Log (Abort): " << message << std::endl;
}
