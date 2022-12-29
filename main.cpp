#include <iostream>
#include <cassert>
#include "details/LinuxThreadDetails.h"
#include "LinuxThread.h"

using namespace rau;

void LinuxThreadConstructorsTest()
{
    // using ThreadType = std::thread; // Works perfect.
    using ThreadType = LinuxThread; // Should work perfect.

    {
        ThreadType t1;
        assert(!t1.joinable() && "t1: should not be joinable.");
        assert((t1.get_id() == ThreadType::id{}) && "t1: invalid LinuxThread::id value.");
    }

    {
        double b = 0.;
        ThreadType t2([](int a, double& b){ b += 1.; return a + b; }, 1, std::ref(b));
        assert(t2.joinable() && "t2: Should be joinable.");
        ThreadType t3 = std::move(t2);
        assert(!t2.joinable() && "t2: must not be joinable after passing ownership to t3");
        assert(t3.joinable() && "t3: must be joinable after accepting the ownership");
        t3.join();
        assert(std::abs(b - 1.) < 0.0001 && "t2: passing with reference didn't work.");
    }

    {
        double b = 0.;
        ThreadType t4([](int a, double& b){ b += 1.; return a + b; }, 1, std::ref(b));
        t4.detach();
        assert(!t4.joinable() && "t4: t4 mustn't be joinable.");
    }
}

int main()
{
    LinuxThreadConstructorsTest();
    return 0;
}
