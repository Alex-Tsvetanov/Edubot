#pragma once

#include <string>
#include <stdlib.h>

namespace Resources
{
	struct resource_key 
	{
		int type;
		int id;

		resource_key (std::string type = "", std::string id = "")
		{
			this->type = atoi (type.c_str ());
			this->id = atoi (id.c_str ());
		}
	};

	bool operator < (resource_key a, resource_key b)
	{
		if (a.type == b.type)
			return a.id < b.id;
		return a.type < b.type;
	}

	struct resource 
	{
		resource_key key;
		double goody; // good coefficient

		resource (std::string type, std::string id, double goody = 0)
		{
			this->key = resource_key (type, id);
			this->goody = goody;
		}

		resource (resource_key key, double goody = 0)
		{
			this->key = key;
			this->goody = goody;
		}
	};
}
