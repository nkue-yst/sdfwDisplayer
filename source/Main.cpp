/**
 * @file    Assertion.cpp
 * @author  Y.Nakaue
 */

#include "sdfwDisplayer.hpp"

#define PORT_NUM 62491

int main(int argc, char** argv)
{
    sdfwDisplayer app;
    app.init(PORT_NUM);

    app.run();

    return 0;
}
