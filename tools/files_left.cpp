#include <iostream>
#include <fstream>
#include <sys/types.h>

#include <dirent.h>

using namespace std;

int main(int argc,char *argv[])
{
 string source_path = "/cache/logfiledir/";

 DIR *dp = opendir(source_path.c_str());
         if(dp == NULL) return -1;
         struct dirent *entry;
         int i=0 ;
         while((entry = readdir(dp)))
         {
                 if(entry->d_name[0] == '.')
                         continue;
                 //检测文件后缀为.gz 的文件
                 string s=entry->d_name;
                 if(s.size()>3 && (s.substr(s.size()-3,3) == ".gz")){
                     //m_listLogFile.insert(s);
                       i++;
                 }

         }
         closedir(dp);
printf("TOTAL FILES :%d \n",i);
}
