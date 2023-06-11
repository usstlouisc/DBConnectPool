#include "/usr/local/mysql/include/mysql.h"
#include <string>
#include <sstream>
#include <vector>
using namespace std;
using namespace std;
class Mysql
{
public:
    Mysql();
    //Mysql(const Mysql&)=delete;
    //Mysql(const Mysql&&)=delete;
    //Mysql operator=(const Mysql&)=delete;
    ~Mysql();   
    bool Connect();
    bool Swtich();
    bool QueryStudent(vector<vector<string>>& infor);
    void Close();
    void RefreshFreeTime();
    clock_t GetFreeTime();
private:
    MYSQL* _mysql;          //和服务器连接的句柄
    const char* _sql;
    clock_t _freeTime;
    bool _flag;
};