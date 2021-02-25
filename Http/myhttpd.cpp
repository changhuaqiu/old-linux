#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Timestamp.h>
#include "Util.h"
#include <sys/types.h>
#include <sys/wait.h>


#include <iostream>
#include <map>
#include <string>
using namespace std;


void onRequest(const HttpRequest& req, HttpResponse* resp)
{

    std::cout << "Headers "<<req.methodString() << " "<<req.path()<<std::endl;

    if(req.method()!=HttpRequest::GET&&req.method()!=HttpRequest::POST)
    {

		resp->setStatusCode(HttpResponse::CODE_501);

		resp->setStatusMessage("Method Not Implemented");

		resp->setContentType("text/html");

		resp->setBody("<HTML><HEAD><TITLE>Method Not Implemented</TITLE></HEAD><BODY><P>HTTP request method not supported.</BODY></HTML>");

		return ;
    }
    
    else
    {

		string path=Util::ConstructPath(req.path());

		string extent=Util::GetExtent(path);

		string contentType="";

		Util::GetContentType(extent,contentType);

		string content=Util::GetContent(path);

	if(content.empty())
	{
	    resp->setStatusCode(HttpResponse::CODE_404);

	    resp->setStatusMessage("Not Found");
	}
	else
	{
	    resp->setBody(content);

	    resp->setStatusCode(HttpResponse::CODE_200);

	    resp->setStatusMessage("OK");

	    resp->setContentType(contentType);
	} 
		
    }
    //CGI SCRIPT

    if(req.method()==HttpRequest::POST)
    {
//		cout<<req.getBody()<<endl;
		cout<<"xxxx"<<req.getHeader("Content-Length")<<endl;
	
    }//POST END
    else if(req.method()==HttpRequest::GET)//GET
    {

		resp->setStatusCode(HttpResponse::CODE_200);

		resp->setStatusMessage("OK");

		if(req.path() == "/")
		{
			resp->setContentType("text/html");
    	resp->addHeader("Server", "Muduo");
    	string now = muduo::Timestamp::now().toFormattedString();
    	resp->setBody("<html><head><title>This is title</title></head>"
        "<body><h1>Hello</h1>Now is " + now +
        "</body></html>");
		}
	

    }//GET END
    else
    {
	// Bad Request!
	//FIXME:
	return ;
    }

}

int main(int argc, char* argv[])
{
    int numThreads = 0;
    if (argc > 1)
    {
	//benchmark = true;
	muduo::Logger::setLogLevel(muduo::Logger::WARN);
	numThreads = atoi(argv[1]);
    }
    muduo::net::EventLoop loop;
    HttpServer server(&loop, muduo::net::InetAddress(8000) );
    server.setHttpCallback(onRequest);
    server.setThreadNum(numThreads);
    server.start();
    loop.loop();
    return 0;
}


