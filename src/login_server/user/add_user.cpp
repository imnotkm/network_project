#include "../login_server.h"

void add_user(int sock, std::string id, const std::string &file_path)
{
    User &user = users[sock];
    user.id = id;
    user.is_available = 1;

    // Read the existing JSON file
    std::ifstream in_file(file_path);
    rapidjson::Document doc;
    
    if (in_file.is_open())
    {
        std::string content((std::istreambuf_iterator<char>(in_file)), (std::istreambuf_iterator<char>()));
        in_file.close();
        if (doc.Parse(content.c_str()).HasParseError())
        {
            std::cerr << "[add_user] error: Failed to parse JSON file: " << file_path << std::endl;
            return;
        }
    }
    else
    {
        doc.SetObject();
    }

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value userObj(rapidjson::kObjectType);
    userObj.AddMember("id", rapidjson::Value(user.id.c_str(), allocator), allocator);
    userObj.AddMember("socket #", user.sock, allocator);

    rapidjson::Value addr(rapidjson::kObjectType);
    addr.AddMember("sin_family", user.addr.sin_family, allocator);
    addr.AddMember("sin_port", ntohs(user.addr.sin_port), allocator);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(user.addr.sin_addr), ip, INET_ADDRSTRLEN);
    addr.AddMember("sin_addr", rapidjson::Value(ip, allocator), allocator);

    userObj.AddMember("addr", addr, allocator);

    // Add the new user object to the JSON object with id as the key
    doc.AddMember(rapidjson::Value(user.id.c_str(), allocator), userObj, allocator);

    // Write the updated JSON back to the file
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer.SetIndent(' ', 4);
    doc.Accept(writer);

    std::ofstream out_file(file_path);
    if (!out_file.is_open())
    {
        std::cerr << "[add_user] error: Failed to open file for writing: " << file_path << std::endl;
        return;
    }

    out_file << buffer.GetString();
    out_file.close();
}