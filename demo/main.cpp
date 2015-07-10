#include "mylooper.h"
#include <unistd.h>
#include <signal.h>

int g_fd = 0;

void sighup(int sig)
{
	write(g_fd, "0", 1);
}

int main(int argc, char *argv[])
{
	if (argc != 2 )
	{
		std::cout << "Usage:" << argv[0] << " <conf_path>" << std::endl;
		return 1;
	}
	
	config conf(argv[1]);

	if(conf.load_conf())
	{
		std::cout << "load config failed!" << std::endl;
		return 2;
	}
	mylooper lp;
	g_fd = lp.get_fd();
	signal(SIGHUP, sighup);
	
	lp.reload(conf);	

	while( true )
	{
		if( lp.run_expired() )
		{
			lp.reload(conf);	
		}
	}
	return 0;
}


