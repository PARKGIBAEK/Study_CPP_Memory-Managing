#pragma once
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/any_completion_executor.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <future>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/mysql/tcp.hpp>

#include "Core/Types.h"

using namespace boost;
using namespace ServerCore;

class MySqlConnectionPool : public asio::noncopyable
{
    enum Config
    {
        host,
        port,
        user,
        password,
        database,
    };

public:
    MySqlConnectionPool(const std::string& _filename, asio::io_context& _ioContext, int32 _poolSize);
    ~MySqlConnectionPool();
    bool CreateConnectionPool(int32 _poolCount);
    mysql::tcp_connection* CreateConnection();
    mysql::tcp_connection* GetPooledConnection();
    void ReturnConnection(mysql::tcp_connection* conn);
    bool TestConnect(mysql::tcp_connection* conn);

private:
    static std::vector<std::string> ReadConfig(const std::string& filename);
    void CleanupConnectionPool();

private:
    std::vector<std::string> m_connectionConfig;
    lockfree::queue<mysql::tcp_connection*> m_connectionPool;
    std::atomic<int32> m_poolAvailable;
    std::atomic<int32> m_poolBorrowed;
    asio::io_context& m_ioContext;
    asio::ip::basic_resolver_results<asio::ip::tcp>* m_endpoints;
    mysql::handshake_params* m_params;
};
