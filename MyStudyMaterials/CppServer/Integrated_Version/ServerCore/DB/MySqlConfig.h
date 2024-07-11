#pragma once
#include <string>
#include <vector>


namespace ServerCore
{
class MySqlConfig
{
public:
        // directory of _filename must be based on executable path
        MySqlConfig(const std::string& _filename);
	MySqlConfig(const MySqlConfig&) = delete;
	MySqlConfig& operator=(MySqlConfig&) = delete;
        static std::vector<std::string> ReadConfig(const std::string& filename);
        bool Connect();
        bool ConnectWithSsl();
private:
        std::vector<std::string> m_ConnectionConfig;
};

}
