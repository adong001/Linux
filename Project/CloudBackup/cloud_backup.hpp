#include<cstdio>
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<zlib.h>
#include"httplib.h"
#include<pthread.h>
#include<boost/filesystem.hpp>

namespace Cloud_Sys
{
    class FileTool//文件具类
    {
        private:
        public:
            static bool Read(const std::string& filename,std::string& body)//从文件读取所有内容
            {
                std::fstream ifs(filename,std::ios::binary);
                if(ifs.is_open() == false)
                {
                    std::cout << "open file " << filename << " failed\n";
                    return false;
                }
                //boost::filesysem::file_size(); 获取文件大小
                int64_t fsize = boost::filesystem::file_size(filename);
                body.resize(fsize);
                ifs.read(&body[0],fsize);
                if(ifs.good() == false)
                {
                    std::cout << "file " << filename << " read data failed\n";
                    return false;
                }
                ifs.close();
                return true;
            }
            static bool Write(const std::string& filename,const std::string& body)//向文件写入内容
            {
                std::ofstream ofs(filename,std::ios::binary);
                //ofstream打开文件默认会清空原有内容
                //当前策略是覆盖写入,无需更改
                if(ofs.is_open() == false)
                {
                    std:: cout << "open file " << filename << " failed\n";
                    return false;
                }

                ofs.write(&body[0],body.size());
                if(ofs.good() == false)
                {
                    std::cout << "write file" << filename << " failed\n";
                    return false;
                }

                ofs.close();
                return true;
            }
    };


    class CompressTool//压缩工具类
    {
        private:
        public:
            static bool Compress(const std::string& file_src,const std::string& file_det)//文件压缩
            {
                std::string body;
                FileTool::Read(file_src,body);

                gzFile gf = gzopen(file_det.c_str(),"wb");
                if(gf == NULL)
                {
                    std::cout << "open file " << file_det << " failed\n";
                    return false;
                }

                int len = 0;
                while(len < body.size())//若一次不能将所有数据压缩完，则下次在未压缩的位置继续压缩
                {
                    int ret = gzwrite(gf,&body[len],body.size()-len);
                    if(ret == 0)
                    {
                        std::cout << "file " << file_det <<" write compress data failed\n";
                        return false;
                    }
                    len += ret;
                }
                gzclose(gf);
                return true;
            }

           static bool DeCompress(const std::string& file_src,const std::string& file_det)//文件解压缩>
           {
                std::ofstream ofs(file_det,std::ios::binary);
                if(ofs.is_open() == false)
                {
                    std::cout << "open file " << file_det << " failed\n";
                    return false;
                }

                gzFile gf = gzopen(file_src.c_str(),"rb");
                if(gf == NULL)
                {
                    std::cout << "open file " << file_src << " failed\n";
                    ofs.close();
                    return false;
                }

                char buff[4096] = {0};
                int ret ;
                while((ret = gzread(gf,buff,4096)) > 0)
                {
                    ofs.write(buff,ret);
                }
                ofs.close();
                gzclose(gf);
           }
    };



    class NonHotPotCompress//非热点压缩类
    {
        private:
            std::string m_gz_dir;//压缩后的文件路径
            bool IsHotPotFile(const std::string& filename);//判断文件是否为热点文件

        public:
            bool Start();//总体向外提供的功能接口,开始压缩模块
    };


    class Server//服务器类
    {
        private:
            std::string m_file_dir;//上传文件备份路径
            httplib::Server m_server;//实例化httplib库提供的Server对象,便于实现http协议
            static void FileUpLoad(const httplib::Request& req,httplib::Response& rsp);//文件上传处理回调函数
            static void List(const httplib::Request& req,httplib::Response& rsp);//文件列表处理回调函数
            static void FielDownLoad(const httplib::Request& req,httplib::Response& rsp);//文件下载处理回调函数
    };

    class DataManage//数据管理类
    {
        private:
            std::string m_back_file;//持久化存储文件名称
            std::unordered_map<std::string,std::string> m_file_list;//数据管理容器
            pthread_rwlock_t m_rwlock;//读写锁
       
        public:

            DataManage()
            {}
    
            bool Exit(const std::string& filename);//判断文件是否存在
            bool IsCompress(const std::string& filename);//判断文件是否已经压缩
            bool Noncompress(std::vector<std::string>* list);//获取未压缩文件列表
            bool Insert(const std::string& file_src,const std::string* file_det);//插入或更新数据
            bool GetAllFileName(std::vector<std::string>* list);//获取所有文件名称
            bool Stroage();//数据改变后持久化存储
            bool InitLoad();//启动时初始化加载原有数据 
    };


};
