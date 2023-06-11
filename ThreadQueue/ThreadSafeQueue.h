#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
using namespace std;

template <typename T>
class ThreadSafeQueue
{
private:
    int _a;
    queue<T> _q;
    condition_variable _cv;
    mutex _mut;

public:
    ThreadSafeQueue() {}
    ~ThreadSafeQueue() {}

    void push(T data)
    {
        lock_guard<mutex> lock(_mut);
        _q.push(move(data));
        _cv.notify_all();
    }

    //队列为空才向队列中添加数据
    void wait_and_push(T data){
         unique_lock<mutex> lock(_mut);
         _cv.wait(lock, [this]
                 { return _q.empty(); });
         _q.push(move(data));
         _cv.notify_all();
    }

    //取数据会阻塞
    void wait_and_pop(T& data)
    {
        unique_lock<mutex> lock(_mut);
        _cv.wait(lock, [this]
                 { return !_q.empty(); });       
        data=move(_q.front());       
        _q.pop();         
        _cv.notify_all();
    }

    shared_ptr<T> wait_span_and_pop(){
        unique_lock<mutex> lock(_mut);        
        while(_q.empty()){
            if(_cv.wait_for(lock,chrono::milliseconds(500))==cv_status::timeout){
                if(_q.empty()){                    
                    return nullptr;
                }
            }
        }
       shared_ptr<T>sp(make_shared<T>(move(_q.front())));      
        _q.pop();
        _cv.notify_all();
        return sp;
    }

     void wait_span_pop(T& data){
        unique_lock<mutex> lock(_mut);        
        while(_q.empty()){
            if(_cv.wait_for(lock,chrono::milliseconds(2000))==cv_status::timeout){
                if(_q.empty()){                    
                    return;
                }
            }
        }
        data=move(_q.front());           
        _q.pop();
        _cv.notify_all();        
    }
    
    shared_ptr<T> wait_and_pop(){
         unique_lock<mutex> lock(_mut);
        _cv.wait(lock, [this]
                 { return !_q.empty(); });
        shared_ptr<T>sp(make_shared<T>(
            move(_q.front())
            ));
         _q.pop();
        _cv.notify_all();
        return sp;
    }

    bool try_pop(T& data){
         unique_lock<mutex> lock(_mut);
         if(!_q.empty()){
            data=move(_q.front());
            _q.pop();     
            return true;
         }else{
            return false;
         }
    }

    //取数据不会阻塞，有就有，没有就没有，立即返回
    shared_ptr<T> try_pop()
    {
        unique_lock<mutex> lock(_mut);
        if(!_q.empty()){
            shared_ptr<T>sp(make_shared(
                move(_q.front())
                ));
            _q.pop();           
         }
         else{            
            return nullptr;           
         }        
    }

    void getfont(T& data){

        unique_lock<mutex> lock(_mut);
        _cv.wait(lock, [this]
                 { return !_q.empty(); });       
        data=move(_q.front());        
    }

    bool empty()
    {
        lock_guard<mutex> lock(_mut);
        return _q.empty();
    }
};
