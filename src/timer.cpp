#include "timer.h"
#include <ctime>
#include <iostream>

timer::timer():name_("timer"),callback_(0),repeat_(true), update_(false)
{
	set_interval(0);
	set_timestamp(0);
}

timer::timer(std::string name, time_callback callback, int interval, bool repeat, unsigned long timestamp):name_(name), callback_(callback), repeat_(repeat), update_(false)
{
	set_interval(interval);
	set_timestamp(timestamp);
	update_timestamp();
}

unsigned long timer::is_expired()
{
	long remain = timestamp_ - time(0);
	if( 0 > remain )
		return 0;
	return remain;
}

int timer::update_timestamp() 
{
	if(!update_)
	{
		unsigned long now = time(0);
		if( timestamp_ < now )
		{
			//in order to repaire deviation
			timestamp_ = now + interval_.tv_sec;
		}
		else
		{
			timestamp_ += interval_.tv_sec;
		}
		update_ = true;
		return 0;
	}
	else
	{
//		std::cout << "warm:update too frequence!\n";
		return 1;
	}
}

void timer::set_timestamp(unsigned long timestamp)
{
	if(0 == timestamp)
	{
		time_t now=time(0);
		this->timestamp_ = now;
	}
	else
	{
		this->timestamp_ = timestamp;
	}
}



