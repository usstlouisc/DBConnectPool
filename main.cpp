
#include "DB_Connection_Pool/DB_Pool.h"
#include <iostream>
using namespace std;

void DO()
{
    vector<vector<string>> infor;
    DB_Pool *pool = DB_Pool::GetDBPool();
    shared_ptr<Mysql> sp = pool->GetConnection();
    Mysql *p = sp.get();    
    p->Swtich();
    p->QueryStudent(infor);
    for(int i=0;i<infor.size();i++){
        for(int j=0;j<infor[i].size();j++){
            cout<<infor[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"sp析构"<<endl;
   
}
int main()
{    
    thread t1(DO);
    thread t2(DO);
    thread t3(DO);
    thread t4(DO);
    this_thread::sleep_for(chrono::seconds(20));
    cout<<"............"<<endl;
    thread t5(DO);
    thread t6(DO);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
   
}