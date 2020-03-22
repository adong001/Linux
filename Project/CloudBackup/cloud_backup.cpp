#include"cloud_backup.hpp"

int main(int argc, char* argv[])
{
    //argv[1]---源文件名称
    //argv[2]---压缩包名称
    
    Cloud_Sys::CompressTool::Compress(argv[1],argv[2]);
    std::string file = argv[2];
    file += ".txt";
    Cloud_Sys::CompressTool::DeCompress(argv[2],file.c_str());
    
    return 0;
}
