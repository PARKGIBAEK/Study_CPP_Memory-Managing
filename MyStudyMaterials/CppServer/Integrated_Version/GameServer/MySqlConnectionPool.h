#pragma once
#include <string>
#include <vector>
#include <boost/asio/any_completion_executor.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <future>

using namespace boost;

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

	struct shared_state
	{
		mysql::connection_pool pool;
		shared_state(mysql::connection_pool pool)
			: pool(std::move(pool)) {}
	};

public:
	// '_filename' is directory which is based on executable path
	MySqlConnectionPool(const std::string& _filename);
	mysql::pooled_connection GetPooledConnection() const;
	std::future<mysql::pooled_connection> GetPooledConnectionFuture();
private:
	bool Init(int _threadCount);
	static std::vector<std::string> ReadConfig(const std::string& filename);
	bool Connect();
	bool ConnectWithSsl();

private:
	std::vector<std::string> m_connectionConfig;
	mysql::connection_pool* m_pool;
	asio::thread_pool m_threadPool;
};
