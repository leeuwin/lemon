#include "config.h"

config::config(std::string conf_file):conf_file_(conf_file)
{
	tree_ = NULL;
	current_task_ = NULL;
}

int config::load_conf()
{
	FILE *fp = fopen(conf_file_.c_str(), "r");
	if (!fp)
	{
		return 1;
	}
	tree_ = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	if (!tree_)
	{
		return 2;
	}
	fclose(fp);
	return 0;
}

mxml_node_t* config::node_get(std::string name, mxml_node_t *parent)
{
	if(!tree_)
	{	return NULL; }
	if (!parent)
	{	parent = tree_; }
	return mxmlFindElement(parent, tree_, name.c_str(), NULL, NULL, MXML_DESCEND);
}

std::string config::node_get_text(std::string name, mxml_node_t *parent)
{
	if(!tree_)
	{	return NULL; }
	if (!parent)
	{	parent = tree_; }
	mxml_node_t *target = mxmlFindElement(parent, tree_, name.c_str(), NULL, NULL, MXML_DESCEND);
	if(NULL != target)
	{
		return mxmlGetText(target, NULL);
	}
	return "";
}

int config::task_reset()
{
	if(!tree_)
	{	return false; }
	current_task_ = mxmlFindElement(tree_, tree_, "task", NULL, NULL, MXML_DESCEND);
	if (NULL != current_task_)
	{
		return 0;
	}
	return 1;
}

int config::task_next()
{
	if (NULL == current_task_ || !tree_)
	{
		return 2;
	}
	current_task_ = mxmlFindElement(current_task_, tree_, "task", NULL, NULL, MXML_DESCEND);
	if (NULL != current_task_)
	{
		return 0;
	}
	return 1;
}


