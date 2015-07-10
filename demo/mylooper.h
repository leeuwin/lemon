#include "looper.h"
#include <sstream>

namespace configure
{
	std::string bin_path;
}

void callback(std::string name)
{
	std::cout << "timer-name:" << name << std::endl;
}

class mylooper:public looper
{
public:
	mylooper():looper(){}
	mylooper(std::string conf_file):looper(conf_file){}
	int reload(config &conf)
	{
		//read conf & rebuild timerlist
		conf.load_conf();

		configure::bin_path = conf.node_get_text("defaultscriptpath");
		if(configure::bin_path.length() == 0)
		{
			configure::bin_path = "./sbin/";
		}

		//read from conf file
		std::string is_clear = conf.node_get_text("reload");
		if( is_clear.length() == 0 || is_clear == "yes")
		{
			//clear timerlist
			clear_list();
		}
		if (conf.task_reset() )
		{
			return 2;
		}
		std::istringstream is;
		std::string name("name");
		int interval = 0;
		bool repeat = true;
		unsigned long timestamp = 0;
		do
		{
		//1.get timer name 2.get timer interval 3.get repeat 4.get start timestamp
			name = conf.task_get_text("name");
			if(conf.task_get_text("repeat") == "yes")
			{
				repeat = true;
			}
			else
			{
				repeat = false;
			}
			is.clear();
			is.str(conf.task_get_text("interval"));
			is >> interval;

			is.clear();
			is.str(conf.task_get_text("timestamp"));
			is >> timestamp;
			std::cout <<"name:"<< name << " interval:" << interval << " timestamp:" << timestamp << std::endl;

			timer t(name, callback, interval, repeat, timestamp);
			add_timer(t);

		}while(!conf.task_next());

		show_list();

		return 0;
	}

};

