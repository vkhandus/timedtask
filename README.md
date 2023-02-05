# Timedtask
Periodically executes user code in a background thread.

# Usage
```cpp
#include <iostream>

// STL headers
#include <chrono>
#include <thread>

#include "timed_task.h"

void fun( int param1, long param2 )
{
    std::cout << "Fun called with params: " << param1 << ", " << param2 << std::endl;
}

class C
{
public:
    void mem( int param1, long param2 )
    {
        std::cout << "mem called with params: " << param1 << ", " << param2 << std::endl;
    }
};

int main()
{
    {
        // Example 0: executing lambda once in 200ms. Ended by timedTask goes out of scope
        utils::TimedTask timedTask( [] { std::cout << "Hi, there" << std::endl; }, std::chrono::milliseconds( 200 ) );
        timedTask.start();

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }


    {
        // Example 1: executing free function once in 100 ms, ended after timedTask goes out of scope
        utils::TimedTask timedTask( std::bind( fun, 3, 4 ), std::chrono::milliseconds( 100 ) );
        timedTask.start();

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    {
        // Example 2: executing member function once in 200 ms, ended after 1 second by explicit stop()
        C c;
        utils::TimedTask timedTask( std::bind( &C::mem, &c, 1, 2 ), std::chrono::milliseconds( 200 ) );
        timedTask.start();

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        timedTask.stop();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    {
        // Example 3: calling closure once in 500 ms, ended after timedTask goes out of scope
        auto test = []( int t ) {
            std::cout << "closure called with params " << t << std::endl;
        };

        utils::TimedTask timedTask( std::bind( test, 23 ), std::chrono::milliseconds( 500 ) );
        timedTask.start();

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    return 0;
}
```
