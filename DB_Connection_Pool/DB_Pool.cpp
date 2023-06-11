#include "DB_Pool.h"
DB_Pool::~DB_Pool()
{
}

DB_Pool *DB_Pool::GetDBPool()
{    
    static DB_Pool* pool= new DB_Pool();    
    return pool;
}

shared_ptr<Mysql> DB_Pool::GetConnection()
{
    Mysql* mysql;
    _que.wait_span_pop(mysql);
    shared_ptr<Mysql>sp(mysql,[&](Mysql*p){
        p->RefreshFreeTime();
        _que.push(p);
        cout<<"归还数据库连接"<<endl;
    });
    return sp;
}
void DB_Pool::ProduceNewConnection()
{
    while(true)
    {
        if (_connectionCnt < _maxSize)
        {
            Mysql* connect=new Mysql();
            connect->Connect();
             _que.wait_and_push(connect);     
            _connectionCnt++;
            cout<<"默认连接不够，添加连接："<<_connectionCnt<<endl;
        }       
    }
}
void DB_Pool::ScannerConnection()
{
    _maxIdleTime=MAX_IDLE_TIME;
   while(true)
    {       
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));  //30s 
        while (_connectionCnt > _initSize)
        {
            Mysql* connect;
             _que.getfont(connect);
            if (connect->GetFreeTime() >= (_maxIdleTime * 1000))
            {
                Mysql* connect_front;
                _que.try_pop(connect_front);
                if(connect_front!=nullptr){
                    delete connect_front;
                }
                _connectionCnt--;
                 cout<<"销毁链接后的个数："<<_connectionCnt<<endl;
            }
            else
            {               
                break;
            }
        }
    }
}

DB_Pool::DB_Pool()
{
    _initSize = MAX_INITIAL_SIZE;
    _maxSize=MAX_POOL_SIZE;
    _connectionCnt=0;
    for (int i = 0; i < _initSize; ++i)
    {
        Mysql* connect=new Mysql();
        connect->Connect();
        connect->RefreshFreeTime(); // 记录连接的起始空闲时刻
        _que.push(connect);
        _connectionCnt++;
        cout<<"初始化链接个数："<<_connectionCnt<<endl;
    }
    thread produce(std::bind(&DB_Pool::ProduceNewConnection, this));
    produce.detach();
    // 启动一个新的定时线程，扫描超过maxIdleTime时间的空闲连接，并对其进行回收
    thread scanner(std::bind(&DB_Pool::ScannerConnection, this));
    scanner.detach();
}
