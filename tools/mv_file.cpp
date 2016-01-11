#include <iostream>
#include <fstream>
#include <sys/types.h>

#include <dirent.h>

using namespace std;

int main(int argc,char *argv[])
{
 string source_path = "/home/ftpsite/";
 string new_path    = "/home/";

 DIR *dp = opendir(source_path.c_str());
         if(dp == NULL) return -1;
         struct dirent *entry;
         while((entry = readdir(dp)))
         {
                 if(entry->d_name[0] == '.')
                         continue;
                 //检测文件后缀为.gz 的文件
                 string s=entry->d_name;
                 if(s.size()>3 && (s.substr(s.size()-3,3) == ".gz")){
                     //m_listLogFile.insert(s);
                     string new_path_file = new_path + s ;
                     string old_path_file = source_path +s ;
                     printf(".gz 老文件位置:%s 新文件位置:%s \n",old_path_file.c_str(),new_path_file.c_str());
                     if(rename(old_path_file.c_str(),new_path_file.c_str()) != 0)
                     {
                        printf("移动文件:%s失败!!!\n",old_path_file.c_str());
                        //remove(strPathName.c_str());
 
                     }

                 }

         }
         closedir(dp);
}
