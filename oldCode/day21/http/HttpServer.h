

/*
对http server的处理
*/

#pragma once
#include <functional>
#include <memory>
#include <stdio.h>
#include "common.h"
#include "TimeStamp.h"

// 自动关闭的时间，以秒为单位
#define AUTOCLOSETIMEOUT 5

class TimeStamp;
class TcpServer;
class TcpConnection;
class HttpRequest;
class HttpResponse;
class EventLoop;
class HttpServer
{
public:
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

    typedef std::function<void(const HttpRequest &,
                               HttpResponse *)>
        HttpResponseCallback;
    DISALLOW_COPY_AND_MOVE(HttpServer);
    HttpServer(EventLoop *loop, const char *ip, const int port, bool auto_close_conn);
    ~HttpServer();

    void HttpDefaultCallBack(const HttpRequest &request, HttpResponse *resp);

    void SetHttpCallback(const HttpResponseCallback &cb);

    void start();

    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn);
    void onRequest(const TcpConnectionPtr &conn, const HttpRequest &request);

    void SetThreadNums(int thread_nums);

    void ActiveCloseConn(std::weak_ptr<TcpConnection> &conn);
    // void TestTimer_IntervalEvery3Seconds() const {
    //     printf("%s TestTimer_IntervalEvery3Seconds\n", TimeStamp::Now().ToFormattedString().data());
    // }

private:
    EventLoop *loop_;
    std::unique_ptr<TcpServer> server_;

    // 是否自动关闭连接。
    bool auto_close_conn_;
    HttpResponseCallback response_callback_;
};
