#include <iostream>  
#include <bits/stdc++.h>  
#include <mysql.h>
using namespace std;  

//Define the database write function
int databasefunction(char *sql){
	MYSQL conn;		    
	mysql_init(&conn);
	mysql_real_connect(&conn, "127.0.0.1", "root", "212999", "test", 0, NULL, 0);
	mysql_query(&conn, "SET NAMES GBK"); 
    int res = mysql_query(&conn, sql);
    if (res!=0){
    	printf("Database error: %s \n",sql);
	}
	my_ulonglong affected_row = mysql_affected_rows(&conn);
	printf("Mysql Attention: %d rows affected.\n", (int)affected_row);
    mysql_close(&conn);
}
 
 
//Define the database read function
float databaseread(char *sql){
	MYSQL conn;		    
	mysql_init(&conn);
	mysql_real_connect(&conn, "127.0.0.1", "root", "212999", "test", 0, NULL, 0);
	mysql_query(&conn, "SET NAMES GBK"); 
	float set=0.0;		
	MYSQL_RES *result;
    int res = mysql_query(&conn, sql);
    if (res!=0){
    	printf("Database error: %s \n",sql);
	}
	result = mysql_store_result(&conn);
    if (result){
        MYSQL_ROW sql_row;
        sql_row = mysql_fetch_row(result);
	 	float result = atof(sql_row[0]);
	 	mysql_close(&conn);	
	 	set =  result;
	}
	return set;
}
 
