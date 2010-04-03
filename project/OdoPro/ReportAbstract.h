#pragma once

class ReportAbstract
{
public:
	ReportAbstract(const char* external_name):next(NULL), name(external_name)
	{
		next = root();
		root(this);
	}
	virtual ~ReportAbstract(void){};

	ReportAbstract*     next;
	const char*			name;   // name must be a pointer to a static string

	virtual void ViewReport(long grp_id, long stud_id) = 0;

	// implementation of static list of behaviors  
	static ReportAbstract* root(ReportAbstract* to_set = 0)
	{
		static ReportAbstract* _root = 0;
		if(to_set) _root = to_set;
		return _root;
	}
	// returns behavior implementation by name.
	static ReportAbstract* find(const char* name)
	{
		for(ReportAbstract* t = root(); t; t = t->next)
			if(strcmp(t->name,name)==0)
				return t;
		return 0; // not found
	}
};
