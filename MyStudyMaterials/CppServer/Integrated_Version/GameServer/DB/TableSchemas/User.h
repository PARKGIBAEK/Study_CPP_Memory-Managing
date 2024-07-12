#pragma once
#include <string>
#include <boost/describe/class.hpp>
#include "Core/Types.h"

namespace DB
{
struct user
{
    uint32_t user_id;
    std::string name;
    uint32_t humor;
    uint32_t image;
    uint32_t video;
    uint32_t chat;
};

BOOST_DESCRIBE_STRUCT(user, (), (user_id, name,humor,image,video,chat));
}
