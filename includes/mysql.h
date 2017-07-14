#ifndef MYSQL_H
#define MYSQL_H
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#define TCP 1
#define UDP 2
template<const char P> class MySQL;

template<>
class MySQL<TCP> {
  private:
    int port = 3306;
    sql::Driver *driver = NULL;
    sql::Connection *con = NULL;
  public:
  	template <class H, class U, class P>
	MySQL(H host, U user, P passwd, int port){
		std::string s = "tcp://" ;
		s += host;
		s += ":";
		s += std::to_string(port);
		std::cout << s;
		driver = get_driver_instance();
		con = driver->connect(host, user, passwd);
	}
	template<class D>
	switchDatabase(D d){con->setSchema(d);}
	
	template<class S> S execute(S statement);
	template<>
	std::string execute(std::string statement){
		sql::Statement *stmt;
		stmt = con->createStatement();
		print stmt->execute(statement.c_str());
	}
	
	
};
#endif
