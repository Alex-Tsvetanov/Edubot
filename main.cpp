#include <iostream>
#include <lib/init.hpp>

#include <lib/fcgi/params.hpp>
#include <lib/fcgi/base.hpp>

void Throw (string a)
{
	cout << a << "\n";
}

Process proc;

int main ()
{
	MySQL::client_t Infocourse ("Infocourse");
	
	std::vector < Resources::resource_key > all;

	//std::cout << "News" << std::endl;
	for (auto& x : Infocourse.result ("SELECT `Num` FROM `News`", "", ""))
	{
		all.push_back (Resources::resource_key ("0", x [0]));
	}

	//std::cout << "News" << std::endl;
	for (auto& x : Infocourse.result ("SELECT `UniqueID` FROM `Videos` WHERE `ExamLink`!=\"#\"", "", ""))
	{
		all.push_back (Resources::resource_key ("1", x [0]));
		all.push_back (Resources::resource_key ("2", x [0]));
	}

	//std::cout << "News" << std::endl;
	for (auto& x : Infocourse.result ("SELECT `UniqueID` FROM `Videos` WHERE `ExamLink`=\"#\"", "", ""))
	{
		all.push_back (Resources::resource_key ("1", x [0]));
	}

 	Resources::States.init (all);
	Resources::States.update ();

	int count = 0;
    long pid = getpid();

    streambuf * cin_streambuf  = cin.rdbuf ();
    streambuf * cout_streambuf = cout.rdbuf ();
    streambuf * cerr_streambuf = cerr.rdbuf ();

    FCGX_Request request;

    FCGX_Init ();
    FCGX_InitRequest (&request, 0, 0);

    while (FCGX_Accept_r (&request) == 0)
    {
        fcgi_streambuf cin_fcgi_streambuf (request.in);
        fcgi_streambuf cout_fcgi_streambuf (request.out);
        fcgi_streambuf cerr_fcgi_streambuf (request.err);

#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
        cin  = &cin_fcgi_streambuf;
        cout = &cout_fcgi_streambuf;
        cerr = &cerr_fcgi_streambuf;
#else
        cin.rdbuf (&cin_fcgi_streambuf);
        cout.rdbuf (&cout_fcgi_streambuf);
        cerr.rdbuf (&cerr_fcgi_streambuf);
#endif

        char * content;
        unsigned long clen = gstdin(&request, &content);
		count ++;
		proc.set_args (request.envp);
		cout << "Content-type: text/html\r\n";
		if (proc.params ["QUERY_STRING"] == "")
			Throw (HTML::invalid_page);
		else
		{
			cout << HTML::viewport << "\n";
			if (proc.query_string ["type"] == "update")
			{
				try
				{
					Resources::States.update ();
				}
				catch (exception& e)
				{
					Throw (e.what ());
				}
			}
			else if (proc.query_string ["type"] == "get")
			{
				try
				{
					person test (proc.query_string ["person"]);
					auto x = Resources::States.next (Resources::States.transform (test.get_visitations ()));
					size_t n = std::min (atoi (proc.query_string ["n"].c_str ()), (int)x.size ());

					std::cout << "[";
					for (int i = 0 ; i < n - 1 ; i ++)
						std::cout << "[" << x [i].key.id << "," << x [i].key.type << "," << x [i].goody << "],";
					std::cout << "[" << x [n - 1].key.id << "," << x [n - 1].key.type << "," << x [n - 1].goody << "]]";
				}
				catch (exception& e)
				{
					Throw (e.what ());
				}
			}
		}
		if (content) delete []content;
#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
		cin  = cin_streambuf;
		cout = cout_streambuf;
		cerr = cerr_streambuf;
#else
		cin.rdbuf(cin_streambuf);
		cout.rdbuf(cout_streambuf);
		cerr.rdbuf(cerr_streambuf);
#endif
	}

#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
	cin  = cin_streambuf;
	cout = cout_streambuf;
	cerr = cerr_streambuf;
#else
	cin.rdbuf(cin_streambuf);
	cout.rdbuf(cout_streambuf);
	cerr.rdbuf(cerr_streambuf);
#endif

	//person test ("2");
	//auto x = Resources::States.next (Resources::States.transform (test.get_visitations ()));

	//for (auto y : x)
	//	std::cout << y.key.id << " " << y.key.type << " " << y.goody << std::endl;
}
