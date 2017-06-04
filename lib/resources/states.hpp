#pragma once

#include <map>
#include <lib/resources/resource.hpp>
#include <vector>
#include <algorithm>
#include <lib/database.hpp>
#include <stdlib.h>

namespace Resources
{
	struct states_t
	{
	private:
		/*static */std::map < Resources::resource_key, std::map < Resources::resource_key, double > > ln;
	public:
		void init (std::vector < Resources::resource_key > resources)
		{
			for (auto& x : resources)
				for (auto& y : resources)
					if (x.id != y.id or x.type != y.type)
						ln [x][y] = 0;
					else
						ln [x][y] = 0;
		}
		/*static */void update ()
		{
			//std::cout << "UPDATE()" << std::endl;
			//auto res = MySQL::client.result ("SELECT `IdUser`, `Type`, `IdResource` FROM `Visits` ORDER BY `ID` DESC LIMIT 10000", "", "");
			auto res = MySQL::client.result ("SELECT `IdUser`, `Type`, `IdResource` FROM `Visits`", "", "");
			std::map < Resources::resource_key, std::map < Resources::resource_key, int > > success;
			std::map < Resources::resource_key, int > all;
			std::map < std::string, Resources::resource_key > users;

			for (auto& x : res)
			{
				if (users.find (x [0]) == users.end ())
				{
					users [x [0]] = Resources::resource_key (x [1], x [2]);
				}
				else
				{
					success [users [x [0]]][Resources::resource_key (x [1], x [2])] ++;
					all [users [x [0]]] ++;
					users [x [0]] = Resources::resource_key (x [1], x [2]);
				}
			}

			for (auto& x : ln)
				if (all [x.first] != 0)
					for (auto& y : ln)
						ln [x.first][y.first] = (double)success [x.first][y.first] / (double)all [x.first];
				else
					for (auto& y : ln)
						ln [x.first][y.first] = 0;
		}

		/*static */std::map < Resources::resource_key, double > transform (std::vector < Resources::resource_key > visited)
		{
			//std::cout << "TRANSFORM ()" << std::endl;
			std::map < Resources::resource_key, double > ans;

			for (auto& x : ln)
				ans [x.first] = 0;
			
			for (auto& x : visited)
				ans [x] = 1;

			return std::move (ans);
		}

		/*static */std::vector < Resources::resource > next (std::map < Resources::resource_key, double > current)
		{
			update ();
			double total = 0.0;
			std::map < Resources::resource_key, double > pre_result;

			for (auto& x : ln)
			{
				if (1.0 - current [x.first] < 1e-4)
				{
					pre_result [x.first] = 0.123456;
				}
				else
				{
					pre_result [x.first] = 0.0;
					for (auto& neighbor : x.second)
						pre_result [x.first] += current [neighbor.first] * neighbor.second;
					total += pre_result [x.first];
				}
			}

			std::vector < Resources::resource > ans;
			for (auto& x : pre_result)
				if (total != 0)
					ans.push_back (Resources::resource (x.first, x.second / total));
				else
					ans.push_back (Resources::resource (x.first, rand () / rand ()));


			std::sort (ans.begin (), ans.end (), [](Resources::resource a, Resources::resource b) 
					{
						if (a.goody == b.goody)
						{
							if (a.key.type == b.key.type)
								return a.key.id < b.key.id;
							return a.key.type < b.key.type;
						}
						return a.goody > b.goody;
					}
				);

			return std::move (ans);
		}
	} States; // similar to namespace with hidden members
}
