#ifndef CONFIG_H_H_
#define CONFIG_H_H_

#include <string>
#include <mxml.h>

class config
{
public:
	config(std::string conf_file);
	~config() { unload_conf(); }
	int load_conf();

	mxml_node_t* node_get(std::string name, mxml_node_t *parent = NULL);
	std::string node_get_text(std::string name, mxml_node_t *parent = NULL);
	int task_reset();
	int task_next();
	std::string task_get_text(std::string name) { return node_get_text(name, current_task_); }
private:
	std::string conf_file_;	
	mxml_node_t *tree_;
	mxml_node_t *current_task_;
	
	void unload_conf() { if (NULL != tree_) mxmlDelete(tree_); }
};

#endif//CONFIG_H_H_

