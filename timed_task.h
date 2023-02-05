#ifndef TIMED_TASK_H
#define TIMED_TASK_H

// STL headers
#include <chrono>
#include <functional>
#include <memory>

namespace utils
{
    /*!
     * \brief Executes periodic call of callable object in thread
     */
    class TimedTask
    {
    public:
        using TimeoutCallable = std::function<void()>;

        TimedTask( const TimeoutCallable& timeoutFn, std::chrono::milliseconds timeout );
        ~TimedTask();

        // supports moving, but not copying
        TimedTask( const TimedTask& ) = delete;
        TimedTask& operator=( const TimedTask& ) = delete;

        TimedTask( TimedTask&& );
        TimedTask& operator=( TimedTask&& );

        void start();
        void stop();
        bool isRunning() const;

        void setTimeout( std::chrono::milliseconds timeout );

    private:
        class TimedTaskImpl;
        std::unique_ptr<TimedTaskImpl> m_pImpl;
    };
} // namespace utils

#endif // TIMED_TASK_H
