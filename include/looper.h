#ifndef LOOPER_H_H_
#define LOOPER_H_H_

#include <sys/select.h>
#include <set>
#include "timer.h"
#include "config.h"

class looper{
public:
	looper();
	looper( std::string timerfile );
	virtual int reload(config &conf);	//load configure from class config
	int run_expired();			//exec timer callback which is expired, reinsert into list or drop it which is judged by timer::repeat.
	
	int get_fd() const { return fd_; }
protected:
	int add_timer(timer &t, bool force=false);
	int del_timer(std::string t_name);
	void show_list() const;
	void clear_list(){ timerlist.clear(); namelist.clear();}
private:
	std::multiset<timer> timerlist;		//timer list
	std::set<std::string> namelist;		//timer name list, in order to be unique
	int fd_;				//timer fd
	fd_set read_set_;
	fd_set temp_set_;
	struct timeval tv_;
	
	int wait(unsigned long interval = 0);	//interval:0  wait until fd_ is readble; interval:not-0 wait interval seconds or break by fd_'s readble
	int clear_fd();				//clear fd_'s readble
};



#endif//LOOPER_H_H_

