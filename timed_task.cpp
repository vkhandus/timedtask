#include "timed_task.h"

// STL headers
#include <condition_variable>
#include <thread>

namespace utils
{
    class TimedTask::TimedTaskImpl
    {
    public:
        TimedTaskImpl( const TimedTask::TimeoutCallable& timeoutFn, std::chrono::milliseconds timeout )
            : m_Thread{},
              m_Mutex{},
              m_WaitCondition{},
              m_TimeoutAction{ timeoutFn },
              m_Timeout{ timeout },
              m_bStopRequested{ true }
        {
        }

        ~TimedTaskImpl()
        {
            stop();
        }

        void start()
        {
            if ( !m_Thread.joinable() && m_TimeoutAction )
            {
                // mutex is not need. Thread is not yet started or already finished
                m_bStopRequested = false;
                m_Thread = std::thread{ &TimedTaskImpl::run, this };
            }
        }

        void stop()
        {
            if ( m_Thread.joinable() )
            {
                {
                    std::lock_guard<std::mutex> lock{ m_Mutex };
                    m_bStopRequested = true;
                }
                m_WaitCondition.notify_one();
                m_Thread.join();
            }
        }

        bool isRunning() const
        {
            return m_Thread.joinable();
        }

        void setTimeout( std::chrono::milliseconds timeout )
        {
            std::lock_guard<std::mutex> lock{ m_Mutex };
            m_Timeout = timeout;
        }

    private:
        void run()
        {
            std::unique_lock<std::mutex> lock{ m_Mutex };
            while ( !m_bStopRequested )
            {
                lock.unlock();
                m_TimeoutAction();
                lock.lock();
                m_WaitCondition.wait_for( lock, std::chrono::milliseconds{ m_Timeout.count() }, [ this ] { return m_bStopRequested; } );
            }
        }

        std::thread m_Thread;
        mutable std::mutex m_Mutex;
        std::condition_variable m_WaitCondition;

        TimedTask::TimeoutCallable m_TimeoutAction;
        std::chrono::milliseconds m_Timeout;
        bool m_bStopRequested;
    };

    TimedTask::TimedTask( const TimeoutCallable& timeoutFn, std::chrono::milliseconds timeout )
        : m_pImpl{ std::make_unique<TimedTaskImpl>( timeoutFn, timeout ) }
    {
    }

    TimedTask::~TimedTask() = default;

    TimedTask::TimedTask( TimedTask&& ) = default;
    TimedTask& TimedTask::operator=( TimedTask&& ) = default;

    void TimedTask::start()
    {
        m_pImpl->start();
    }

    void TimedTask::stop()
    {
        m_pImpl->stop();
    }

    bool TimedTask::isRunning() const
    {
        return m_pImpl->isRunning();
    }

    void TimedTask::setTimeout( std::chrono::milliseconds timeout )
    {
        m_pImpl->setTimeout( timeout );
    }
} // namespace utils
