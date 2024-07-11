#include "MySqlConfig.h"
#include "Util/Path.h"
#include <fstream>
#include <iostream>
#include <boost/mysql.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/mysql/tcp_ssl.hpp>
#include <boost/mysql/handshake_params.hpp>
#include <boost/mysql/error_with_diagnostics.hpp>


namespace ServerCore
{

// The CA file that signed the server's certificate
constexpr const char CA_PEM[] = R"%(-----BEGIN CERTIFICATE-----
MIIDZzCCAk+gAwIBAgIUWznm2UoxXw3j7HCcp9PpiayTvFQwDQYJKoZIhvcNAQEL
BQAwQjELMAkGA1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3RhdGUxDjAMBgNVBAoM
BW15c3FsMQ4wDAYDVQQDDAVteXNxbDAgFw0yMDA0MDQxNDMwMjNaGA8zMDE5MDgw
NjE0MzAyM1owQjELMAkGA1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3RhdGUxDjAM
BgNVBAoMBW15c3FsMQ4wDAYDVQQDDAVteXNxbDCCASIwDQYJKoZIhvcNAQEBBQAD
ggEPADCCAQoCggEBAN0WYdvsDb+a0TxOGPejcwZT0zvTrf921mmDUlrLN1Z0hJ/S
ydgQCSD7Q+6za4lTFZCXcvs52xvvS2gfC0yXyYLCT/jA4RQRxuF+/+w1gDWEbGk0
KzEpsBuKrEIvEaVdoS78SxInnW/aegshdrRRocp4JQ6KHsZgkLTxSwPfYSUmMUo0
cRO0Q/ak3VK8NP13A6ZFvZjrBxjS3cSw9HqilgADcyj1D4EokvfI1C9LrgwgLlZC
XVkjjBqqoMXGGlnXOEK+pm8bU68HM/QvMBkb1Amo8pioNaaYgqJUCP0Ch0iu1nUU
HtsWt6emXv0jANgIW0oga7xcT4MDGN/M+IRWLTECAwEAAaNTMFEwHQYDVR0OBBYE
FNxhaGwf5ePPhzK7yOAKD3VF6wm2MB8GA1UdIwQYMBaAFNxhaGwf5ePPhzK7yOAK
D3VF6wm2MA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAAoeJCAX
IDCFoAaZoQ1niI6Ac/cds8G8It0UCcFGSg+HrZ0YujJxWIruRCUG60Q2OAbEvn0+
uRpTm+4tV1Wt92WFeuRyqkomozx0g4CyfsxGX/x8mLhKPFK/7K9iTXM4/t+xQC4f
J+iRmPVsMKQ8YsHYiWVhlOMH9XJQiqERCB2kOKJCH6xkaF2k0GbM2sGgbS7Z6lrd
fsFTOIVx0VxLVsZnWX3byE9ghnDR5jn18u30Cpb/R/ShxNUGIHqRa4DkM5la6uZX
W1fpSW11JBSUv4WnOO0C2rlIu7UJWOROqZZ0OsybPRGGwagcyff2qVRuI2XFvAMk
OzBrmpfHEhF6NDU=
-----END CERTIFICATE-----
)%";

MySqlConfig::MySqlConfig(const std::string& _filename)
{
	try {
		std::string configFilePath = Path::GetRelativeFilePath(_filename);
		m_ConnectionConfig = ReadConfig(configFilePath);
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}
}

std::vector<std::string> MySqlConfig::ReadConfig(const std::string& filename)
{
	std::ifstream file(filename);
	std::vector<std::string> config;
	std::string line;

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			config.push_back(line);
		}
		file.close();
	}
	else
	{
		throw std::runtime_error("Unable to open file");
	}

	return config;
}

bool MySqlConfig::Connect()
{
	if (m_ConnectionConfig.size() != 5)
		throw std::runtime_error("Invalid configuration");

	std::string host = m_ConnectionConfig[0];
	std::string port = m_ConnectionConfig[1];
	std::string user = m_ConnectionConfig[2];
	std::string password = m_ConnectionConfig[3];
	std::string database = m_ConnectionConfig[4];

	// I/O context
	boost::asio::io_context ctx;

	// Resolver for hostname resolution
	boost::asio::ip::tcp::resolver resolver(ctx.get_executor());

	// Connection params
	boost::mysql::handshake_params params(
		user, // username
		password, // password
		database // database to use(leave empty or omit the parameter for no database)
	);

	boost::mysql::tcp_connection conn(ctx);
	// Hostname resolution
	auto endpoints = resolver.resolve(host, port);
	try
	{
		// Connect to the server. This operation will perform the SSL handshake as part of
		// it, and thus will fail if the certificate is found to be invalid.
		conn.connect(*endpoints.begin(), params);
	}
	catch (const boost::mysql::error_with_diagnostics& err)
	{
		// Some errors include additional diagnostics, like server-provided error messages.
		// If a store procedure fails (e.g. because a SIGNAL statement was executed), an error
		// like this will be raised.
		// Security note: diagnostics::server_message may contain user-supplied values (e.g. the
		// field value that caused the error) and is encoded using to the connection's encoding
		// (UTF-8 by default). Treat is as untrusted input.
		std::cerr << "Error: " << err.what() << ", error code: " << err.code() << '\n'
			<< "Server diagnostics: " << err.get_diagnostics().server_message() << std::endl;
		return 1;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}
	// We can now use the connection as we would normally do.
	const char* sql = "SELECT * from users";
	// const char* sql = "SELECT first_name, last_name, salary FROM row";
	boost::mysql::results result;
	conn.execute(sql, result);

	for (const auto& meta : result.meta())
		std::cout << meta.column_name() << '\t';

	for (auto row : result.rows())
	{
		auto columns = row.size();
		for (int i = 0; i < columns; i++)
			std::cout << row.at(i) << "\t";
		std::cout << '\n';
	}

	// Cleanup
	conn.close();
	return true;
}

bool MySqlConfig::ConnectWithSsl()
{
	if (m_ConnectionConfig.size() != 5)
		throw std::runtime_error("Invalid configuration");

	std::string host = m_ConnectionConfig[0];
	std::string port = m_ConnectionConfig[1];
	std::string user = m_ConnectionConfig[2];
	std::string password = m_ConnectionConfig[3];
	std::string database = m_ConnectionConfig[4];

	// I/O context
	boost::asio::io_context ctx;

	// Resolver for hostname resolution
	boost::asio::ip::tcp::resolver resolver(ctx.get_executor());

	// Connection params
	boost::mysql::handshake_params params(
		user, // username
		password, // password
		database // database to use(leave empty or omit the parameter for no database)
	);

	// This context will be used by the underlying SSL stream object. We can
	// set up here any SSL-related options, like peer verification or CA
	// certificates. We will do these in the next lines.
	boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::tls_client);

	// Check whether the server's certificate is valid and signed by a trusted CA.
    // If it's not, our handshake or connect operation will fail.
	ssl_ctx.set_verify_mode(boost::asio::ssl::verify_peer);

	// Load a trusted CA, which was used to sign the server's certificate.
	// This will allow the signature verification to succeed in our example.
	// You will have to run your MySQL server with the test certificates
	// located under $BOOST_MYSQL_ROOT/tools/ssl/
	ssl_ctx.add_certificate_authority(boost::asio::buffer(CA_PEM));

	// We expect the server certificate's common name to be "mysql".
	// If it's not, the certificate will be rejected and handshake or connect will fail.
	ssl_ctx.set_verify_callback(boost::asio::ssl::host_name_verification("mysql"));

	// Pass in our SSL context to the connection. Note that we
	// can create many connections out of a single context. We need to keep the
	// context alive until we finish using the connection.
	boost::mysql::tcp_ssl_connection conn(ctx, ssl_ctx);

	// Hostname resolution
	auto endpoints = resolver.resolve(host, port);

	try
	{
		// Connect to the server. This operation will perform the SSL handshake as part of
		// it, and thus will fail if the certificate is found to be invalid.
		conn.connect(*endpoints.begin(), params);
	}
	catch (const boost::mysql::error_with_diagnostics& err)
	{
		// Some errors include additional diagnostics, like server-provided error messages.
		// If a store procedure fails (e.g. because a SIGNAL statement was executed), an error
		// like this will be raised.
		// Security note: diagnostics::server_message may contain user-supplied values (e.g. the
		// field value that caused the error) and is encoded using to the connection's encoding
		// (UTF-8 by default). Treat is as untrusted input.
		std::cerr << "Error: " << err.what() << ", error code: " << err.code() << '\n'
			<< "Server diagnostics: " << err.get_diagnostics().server_message() << std::endl;
		return 1;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}

	// We can now use the connection as we would normally do.
	const char* sql = "SELECT * from users";
	// const char* sql = "SELECT first_name, last_name, salary FROM row";
	boost::mysql::results result;
	conn.execute(sql, result);

	for (const auto& meta : result.meta())
		std::cout << meta.column_name() << '\t';

	for (auto row : result.rows())
	{
		auto columns = row.size();
		for (int i = 0; i < columns; i++)
			std::cout << row.at(i) << "\t";
		std::cout << '\n';
	}

	// Cleanup
	conn.close();
	return true;
}
}
