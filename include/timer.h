#ifndef TIMER_H_H_
#define TIMER_H_H_

#include <iostream>
#include <string>

typedef void (*time_callback)(std::string name);

class timer{
public:
	timer();
	timer(std::string name, time_callback callback, int interval, bool repeat = true, unsigned long timestamp = 0);
	bool get_repeat() const { return repeat_; }
	void set_repeat( bool repeat ) { repeat_ = repeat; }
	struct timeval get_interval() {  return interval_; }
	void set_interval(int seconds) { this->interval_.tv_sec = seconds; this->interval_.tv_usec = 0; }
	unsigned long get_timestamp() const { return timestamp_; }
	unsigned long is_expired(); //0:expired >0:time to expired
	void run()
	{
		callback_(name_);
		update_ = false;
		update_timestamp();
	}
	bool operator<(const timer& t)const
	{
		return timestamp_ < t.timestamp_;
	}
	std::string get_name() const { return name_; }
private:
	time_callback callback_;
	bool repeat_;
	bool update_;			//in order to prevent update too many times
	unsigned long timestamp_;	//exec timestamp in future
	struct timeval interval_;	//the step between to exec timestamp
	std::string name_;		//the timer name, required uniqe
	
	int update_timestamp();
	void set_timestamp(unsigned long timestamp = 0); 
};


#endif//TIMER_H_H_

