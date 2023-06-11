
#include "../Mysql/Mysql.h"
#include "../ThreadQueue/ThreadSafeQueue.h"

#define MAX_INITIAL_SIZE       3
#define MAX_POOL_SIZE          6
#define MAX_IDLE_TIME          5     //单位s

class DB_Pool
{
public:    
     DB_Pool(const DB_Pool&)=delete;
     DB_Pool(const DB_Pool&&)=delete;
     DB_Pool operator=(const DB_Pool&)=delete;
    ~DB_Pool();
    static DB_Pool* GetDBPool();   //获取链接池
    shared_ptr<Mysql>GetConnection();  //获取链接对象
    void ProduceNewConnection();        //产生新的链接
    void ScannerConnection();

private:
    DB_Pool();
    
    int _initSize;              // 连接池的最大初始连接量
    int _maxSize;               // 连接池的最大连接量
    int _maxIdleTime;           // 连接池的最大空闲时间
    int _connectionTimeout;     // 连接池获取连接的超时时间
    ThreadSafeQueue<Mysql*>_que;   
    atomic_int _connectionCnt;    // 记录connection连接的总数量
};