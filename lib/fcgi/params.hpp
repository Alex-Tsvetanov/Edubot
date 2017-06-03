#include <map>
#include <string>

using namespace std;

namespace HTML
{
	const string viewport = "<meta name=viewport content=\"width=device-width, initial-scale=0.5\">\n";
	const string invalid_page = "<h1><span style=\"color: red;\">TYPE</span> of <span style=\"color: green;\">QUERY</span> is <span style=\"color: red;\">EMPTY</span></h1>";
	const string invalid_type = "<h1><span style=\"color: red;\">Invalid</span> <span style=\"color: red;\">TYPE</span> of <span style=\"color: green;\">QUERY</span></h1>";

std::map <string, char> html_url_encoding = 
{
	{"%20",	' '},
	{"%21",	'!'},
	{"%22",	'\"'},
	{"%23",	'#'},
	{"%24",	'$'},
	{"%25",	'%'},
	{"%26",	'&'},
	{"%27",	'\''},
	{"%28",	'('},
	{"%29",	')'},
	{"%2A",	'*'},
	{"%2B",	'+'},
	{"%2C",	','},
	{"%2D",	'-'},
	{"%2E",	'.'},
	{"%2F",	'/'},
	{"%30",	'0'},
	{"%31",	'1'},
	{"%32",	'2'},
	{"%33",	'3'},
	{"%34",	'4'},
	{"%35",	'5'},
	{"%36",	'6'},
	{"%37",	'7'},
	{"%38",	'8'},
	{"%39",	'9'},
	{"%3A",	':'},
	{"%3B",	';'},
	{"%3C",	'<'},
	{"%3D",	'='},
	{"%3E",	'>'},
	{"%3F",	'?'},
	{"%40",	'@'},
	{"%41",	'A'},
	{"%42",	'B'},
	{"%43",	'C'},
	{"%44",	'D'},
	{"%45",	'E'},
	{"%46",	'F'},
	{"%47",	'G'},
	{"%48",	'H'},
	{"%49",	'I'},
	{"%4A",	'J'},
	{"%4B",	'K'},
	{"%4C",	'L'},
	{"%4D",	'M'},
	{"%4E",	'N'},
	{"%4F",	'O'},
	{"%50",	'P'},
	{"%51",	'Q'},
	{"%52",	'R'},
	{"%53",	'S'},
	{"%54",	'T'},
	{"%55",	'U'},
	{"%56",	'V'},
	{"%57",	'W'},
	{"%58",	'X'},
	{"%59",	'Y'},
	{"%5A",	'Z'},
	{"%5B",	'['},
	{"%5C",	'\\'},
	{"%5D",	']'},
	{"%5E",	'^'},
	{"%5F",	'_'},
	{"%60",	'`'},
	{"%61",	'a'},
	{"%62",	'b'},
	{"%63",	'c'},
	{"%64",	'd'},
	{"%65",	'e'},
	{"%66",	'f'},
	{"%67",	'g'},
	{"%68",	'h'},
	{"%69",	'i'},
	{"%6A",	'j'},
	{"%6B",	'k'},
	{"%6C",	'l'},
	{"%6D",	'm'},
	{"%6E",	'n'},
	{"%6F",	'o'},
	{"%70",	'p'},
	{"%71",	'q'},
	{"%72",	'r'},
	{"%73",	's'},
	{"%74",	't'},
	{"%75",	'u'},
	{"%76",	'v'},
	{"%77",	'w'},
	{"%78",	'x'},
	{"%79",	'y'},
	{"%7A",	'z'}
};
}

struct Process
{
public:
	map <string, string> params;
	map <string, string> query_string;

	static map <string, string> splitQueryString (string a)
	{
		map <string, string> ans;
		string param [2];
		int ind = 0;
		for (int i = 0 ; i < a.size () ; i ++)
		{
			if (a [i] == '&')
			{
				ans [param [0]] = param [1];
				param [0] = param [1] = "";
				ind = 0;
			}
			else if (a [i] == '=')
			{
				ind ++;
			}
			else if (a [i] == '+')
				param [ind] += ' ';
			else if (a [i] == '%')
			{
				if (HTML::html_url_encoding.find (a.substr (i, 3)) != HTML::html_url_encoding.end ())
				{
					param [ind] += HTML::html_url_encoding [a.substr (i, 3)];;
					i += 2;
				}
				else
					param [ind] += '%';
			}
			else
				param [ind] += a [i];
		}
		ans [param [0]] = param [1];
		return move (ans);
	}

	void make_params (const char * const * envp)
	{
		for ( ; *envp; ++envp)
		{
			int ind = 0;
			string index = "", value = "";
			for ( ; (*envp)[ind] != '='; ind ++)
				index += (*envp)[ind];

			for (ind ++ ; (*envp)[ind] != '\0'; ind ++)
				value += (*envp)[ind];

			params [index] = value;
		}
	}

	void set_args (const char * const * envp = nullptr)
	{
		make_params (envp);
		query_string = splitQueryString (params ["QUERY_STRING"]);
	}
};
