#pragma once

#include <map>
#include <lib/resources/resource.hpp>
#include <vector>
#include <algorithm>
#include <lib/database.hpp>

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
						ln [x][y] = 1;
					else
						ln [x][y] = 0;
		}
		/*static */void update ()
		{
			//std::cout << "UPDATE()" << std::endl;
			auto res = MySQL::client.result ("SELECT `IdUser`, `Type`, `IdResource` FROM `Visits` ORDER BY `ID` DESC LIMIT 10000", "", "");
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
						ln [x.first][y.first] *= success [x.first][y.first] / all [x.first];
				else
					for (auto& y : ln)
						ln [x.first][y.first] = 0.1;
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
			double total = 0.0;
			std::map < Resources::resource_key, double > pre_result;

			for (auto& x : ln)
			{
				if (current [x.first] == 1.0)
				{
					pre_result [x.first] = 0.5;
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
				if (total > 1e-7)
					ans.push_back (Resources::resource (x.first, x.second / total));
				else
					ans.push_back (Resources::resource (x.first, x.second));


			std::sort (ans.begin (), ans.end (), [](Resources::resource a, Resources::resource b) 
					{
						if (a.goody == b.goody)
							return a.key.id < b.key.id;
						return a.goody > b.goody;
					}
				);

			return std::move (ans);
		}
	} States; // similar to namespace with hidden members
}
