#include <boost/process.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return -1;
    }

    std::string cmd = argv[1];
    for (size_t i = 2; i < argc; ++i)
    {
        cmd += " ";
        cmd += argv[i];
    }

    boost::process::ipstream pipe_stream;
    boost::process::child c(cmd, boost::process::std_out > pipe_stream);
    
    std::string line;
    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
    {
        std::cout << line << std::endl;
    }

    c.wait();
    return 0;
}
