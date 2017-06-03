#pragma once

#include <mysql/mysql.h>
#include <vector>
#include <string>

namespace MySQL
{
	struct client_t
	{
	private:
		MYSQL* con;
	public:
		client_t (std::string db = "AIT")
		{
			this->con = mysql_init (NULL);

			if (this->con == NULL) 
			{
				fprintf (stderr, "%s\n", mysql_error (this->con));
				throw "Failed";
			}
			if (mysql_real_connect (  this->con, "localhost", "root", "tts2002", 
									db.c_str (),           0,   NULL,         0) == NULL) 
			{
				fprintf (stderr, "%s\n", mysql_error (this->con));
				mysql_close (this->con);
				throw "Failed";
			} 
		}

		std::vector <MYSQL_ROW> result (std::string sql, std::string add, std::string sql2)
		{
			char* to = new char [add.size ()];
			mysql_real_escape_string (this->con, to, add.c_str (), add.size ()); 
			add = std::string (to);
			sql = sql + add + sql2;
			//std::cout << sql << std::endl;
			delete[] to;
			if (mysql_query (this->con, sql.c_str ())) 
			{
				fprintf (stderr, "%s\n", mysql_error (this->con));
				mysql_close (this->con);
				throw "Failed";
			}
			MYSQL_RES* res = mysql_store_result (this->con);
			MYSQL_ROW row;
			std::vector <MYSQL_ROW> ans;
			while (row = mysql_fetch_row (res))
				ans.push_back (row);
			return std::move (ans);
		}
	} client;
}
