#include "Mysql.h"
Mysql::Mysql()
{
    _flag = false;
}
Mysql::~Mysql()
{
}
bool Mysql::Connect()
{
    _mysql = mysql_init(nullptr);
    if (_mysql != nullptr)
    {
        mysql_options(_mysql, MYSQL_SET_CHARSET_NAME, "gbk"); // MYSQL_OPT_RECONNECT
        char reconnect = 1;
        mysql_options(_mysql, MYSQL_OPT_RECONNECT, &reconnect);
        if (!mysql_real_connect(_mysql, "localhost", "root", "12345678", "", 3306, NULL, 0))
        {
            return false;
        }
        else
        {
            _flag = true;
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Mysql::Swtich()
{
    _sql = "use family";
    if (!mysql_query(_mysql, _sql))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Mysql::QueryStudent(vector<vector<string>>& infor)
{
    if (_flag)
    {
        _sql = "select * from student";
        if (!mysql_query(_mysql, _sql))
        {
            MYSQL_RES *result = mysql_store_result(_mysql);
            unsigned int rows = (unsigned int)mysql_num_rows(result);
            unsigned int columns = mysql_num_fields(result);
            if (rows == 0)
            {
                mysql_free_result(result);
                return 0;
            }
            else
            {
            }
            MYSQL_ROW row;
            unsigned int index = 0;
            MYSQL_ROW roww;
            while ((roww = mysql_fetch_row(result)))
            {
                vector<string> temp;
                for (unsigned int i = 0; i < columns; i++)
                {
                    temp.push_back(roww[i]);
                }
                infor.push_back(temp);
            }
            mysql_free_result(result);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
void Mysql::Close()
{
    if (_mysql != nullptr)
    {
        mysql_close(_mysql);
    }
}

void Mysql::RefreshFreeTime()
{
    _freeTime = clock();
}

clock_t Mysql::GetFreeTime()
{
    return clock() - _freeTime;
}
