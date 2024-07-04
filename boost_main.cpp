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

    boost::process::ipstream pipe_out_stream;
    boost::process::ipstream pipe_err_stream;
    boost::process::child c(cmd
        , boost::process::std_out > pipe_out_stream
        // Uncommenting this line fixed the bug
        //, boost::process::std_err > pipe_err_stream
    );
    
    std::string line;
    while (pipe_out_stream && std::getline(pipe_out_stream, line) && !line.empty())
    {
        std::cout << line << std::endl;
    }

    c.wait();
    return 0;
}
