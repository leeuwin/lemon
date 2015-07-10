#include "looper.h"
#include <fcntl.h>
#include <cerrno>
#include <unistd.h>

void setNonBlocking(int fd)
{
	int opts = -1;
	int ret = -1;
	opts = fcntl(fd, F_GETFL);
	if(opts < 0)
	{
		printf("fcntl:%d\n",opts);
	}
	opts = opts | O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, opts);
	if(ret < 0)
	{
		printf("fcntl:%d\n",opts);
	}
}

looper::looper():fd_(0)
{
	tv_.tv_sec = 0;
	tv_.tv_usec = 0;
	FD_ZERO(&read_set_);
	FD_ZERO(&temp_set_);
	setNonBlocking(fd_);
	FD_SET(fd_, &temp_set_);
}

looper::looper( std::string timerfile )
{
	tv_.tv_sec = 0;
	tv_.tv_usec = 0;
	FD_ZERO(&read_set_);
	FD_ZERO(&temp_set_);
	
	if(mkfifo(timerfile.c_str(), 0644) )
	{
		std::cout << "warn:fifo is already exist!\n";
	}
	fd_ = open(timerfile.c_str(),O_RDWR|O_NONBLOCK);
	if(fd_ < 0)
	{
		fd_ = 0;
	}
	FD_SET(fd_, &temp_set_);

}

int looper::wait(unsigned long interval)
{
	
	int ret = 0;
	if (interval != 0)
	{
		tv_.tv_sec = interval;
		tv_.tv_usec = 0;
		read_set_ = temp_set_;
		ret=select(fd_+1, &read_set_, NULL, NULL, &tv_);
		return ret?1:0;
	}
	read_set_ = temp_set_;
	ret=select(fd_+1, &read_set_, NULL, NULL, NULL);
	return 2;
}

int looper::add_timer(timer &t, bool force)
{	
	//maintenance a set accord timer::name_
	//
	std::pair<std::set<std::string>::iterator, bool> ret;
	ret = namelist.insert( t.get_name() );
	if ( ret.second )
	{
		timerlist.insert(t);
		return 0;
	}
	else	//timer already exist
	{
		//if insist, first del from timerlist, then insert
		if ( force )
		{
			del_timer( t.get_name() );
			timerlist.insert(t);
			return 0;
		}
		return 1;
	}
}

int looper::del_timer(std::string t_name)
{
	std::set<std::string>::iterator nl_it = namelist.find(t_name);
	if (nl_it != namelist.end())
	{
		namelist.erase(nl_it);
	}
	std::multiset<timer>::iterator tl_it = timerlist.begin();
	for(;tl_it != timerlist.end(); tl_it++)
	{
		if(tl_it->get_name() == t_name)
		{
			timerlist.erase(tl_it);
			return 0;
		}
	}
	return 1;
}

int looper::clear_fd()
{
	//read fd_ until EAGAIN
	int iret = 0;
	char buffer[10]={0};
	while(true)
	{
		iret = read(fd_, buffer, 10);
		if ( iret < 0 && errno == EAGAIN)
		{
			break;
		}
	}
}

int looper::reload(config &conf)
{
	//read conf & rebuild timerlist
	show_list();
	return 0;
}

int looper::run_expired()
{
	if ( !timerlist.size() )
	{
		wait();
		clear_fd();
		return 1;
	}
	//notice: can't replace by "else"!
	if ( 0 < timerlist.size() )
	{	
		timer head_timer = *timerlist.begin();
		unsigned long remain = head_timer.is_expired();
		if( !remain )
		{
		show_list();
			head_timer.run();
			if( head_timer.get_repeat() )
			{
				timerlist.erase( timerlist.begin() );
				timerlist.insert( head_timer );
			}
			else
			{
				del_timer(head_timer.get_name());
			}
		}
		else
		{
			if (wait( remain ))
			{
				clear_fd();
				return 2;
			}
		}
		return 0;
	}
}

void looper::show_list() const
{
	std::multiset<timer>::iterator it;
	std::cout << "----current time:" << time(0) << "-----------" << std::endl;
	for(it=timerlist.begin(); it!=timerlist.end(); it++)
	{
		std::cout << "timer:"<< it->get_name() << " tstamp:" << it->get_timestamp() << " left:" << it->get_timestamp()-time(0) << " repeat:" << it->get_repeat() << std::endl;
	}	
}

