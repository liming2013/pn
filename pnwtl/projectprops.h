/**
 * @file projectprops.cpp
 * @brief Project Properties
 * @author Simon Steele
 * @note Copyright (c) 2004-2005 Simon Steele <s.steele@pnotepad.org>
 *
 * Programmers Notepad 2 : The license file (license.[txt|html]) describes 
 * the conditions under which this source may be modified / distributed.
 */

#ifndef projectprops_h__included
#define projectprops_h__included

namespace Projects
{

/**
 * Types of property
 */
typedef enum {propBool, propInt, propChoice, propString, propLongString, propFile, propFolder} PropType;

/**
 * Represent one property in a template.
 */
class ProjectProp
{
public:
	ProjectProp(LPCTSTR name, LPCTSTR description);
	ProjectProp(LPCTSTR name, LPCTSTR description, PropType type);
	virtual ~ProjectProp(){}

	LPCTSTR GetName() const;
	LPCTSTR GetDescription() const;
	PropType GetType() const;

protected:
	PropType	m_type;
	tstring		m_name;
	tstring		m_description;
};

class Choice
{
public:
	tstring Description;
	tstring Value;
};

typedef std::list<Choice*> ValueList;

class ListProp : public ProjectProp
{
public:
	ListProp(LPCTSTR name, LPCTSTR description);
	virtual ~ListProp();

	void Add(Choice* value);

	const ValueList& GetValues();

protected:
	void clear();

protected:
	ValueList m_values;
};

typedef std::list<ProjectProp*> PropList;

/**
 * Named collection of properties.
 */
class PropCategory
{
public:
	PropCategory(LPCTSTR name, LPCTSTR description);
	~PropCategory();

	void Add(ProjectProp* property);
	void Remove(ProjectProp* property);

	LPCTSTR GetName() const;
	LPCTSTR GetDescription() const;

	PropList& GetProperties();

protected:
	void clear();

protected:
	tstring			m_name;
	tstring			m_description;
	PropList		m_list;
};

typedef std::list<PropCategory*> PropCatList;

class PropGroup;

typedef std::list<PropGroup*> PropGroupList;

/**
 * Class containing a number of categories and possible
 * subsets of categories.
 */
class PropGroup
{
public:
	PropGroup(LPCTSTR name, LPCTSTR description);
	~PropGroup();

	void Add(PropCategory* cat);
	void Remove(PropCategory* cat);

	PropCatList& GetCategories();

	void Add(PropGroup* subGroup);
	void Remove(PropGroup* subGroup);

	PropGroupList& GetSubGroups();

	LPCTSTR GetName() const;
	LPCTSTR GetDescription() const;

protected:
	void clear();

protected:
	PropGroupList	m_subgroups;
	PropCatList		m_cats;
	tstring			m_name;
	tstring			m_description;
};

/**
 * A set of categories, groups and properties - designed to match one criteria.
 */
class PropSet
{
public:
	~PropSet();

	void Add(PropGroup* group);
	void Remove(PropGroup* group);

	PropGroupList& GetGroups();

	void SetCriteria(PROJECT_TYPE pType, LPCTSTR extmask);

	bool Matches(PROJECT_TYPE pType, LPCTSTR extmask) const;

protected:
	void clear();

protected:
	PropGroupList	m_groups;
	PROJECT_TYPE	m_type;
	tstring			m_mask;
};

typedef std::list<PropSet*> PropSetList;

/**
 * This class will define a single project template.
 */
class ProjectTemplate
{
public:
	ProjectTemplate(LPCTSTR id, LPCTSTR name, LPCTSTR nameSpace);
	~ProjectTemplate();

	LPCTSTR GetID() const;
	LPCTSTR GetName() const;
	LPCTSTR GetNamespace() const;

	PropSet* GetProperties(PROJECT_TYPE type) const;

	void AddProperties(PROJECT_TYPE type, LPCTSTR extmask, PropGroupList& orphanProps);

protected:
	PropSetList	m_propsets;
	tstring		m_id;
	tstring		m_name;
	tstring		m_namespace;
};

class TemplateLoader : XMLParseState
{
public:
	TemplateLoader();

	ProjectTemplate* FromFile(LPCTSTR path);

//XMLParseState
public:
	virtual void startElement(LPCTSTR name, XMLAttributes& atts);
	virtual void endElement(LPCTSTR name);
	virtual void characterData(LPCTSTR data, int len);

	int m_parseState;

protected:
	void onProjectConfig(XMLAttributes& atts);
	void onSet(XMLAttributes& atts);
	void onEndSet();
	void onGroup(XMLAttributes& atts);
	void onEndGroup();
	void onCategory(XMLAttributes& atts);
	void onEndCategory();
	void onOption(XMLAttributes& atts);
	void onInt(XMLAttributes& atts);
	void onFilePath(XMLAttributes& atts);
	void onFolderPath(XMLAttributes& atts);
	void onOptionList(XMLAttributes& atts);
	void onOptionListValue(XMLAttributes& atts);
	void onEndOptionList();
	void onText(XMLAttributes& atts);

	void makeProp(XMLAttributes& atts, PropType type);

protected:
	ProjectTemplate*	m_pTemplate;
	PropGroupList		m_PropGroups;
	PropGroup*			m_pParentGroup;
	PropGroup*			m_pCurrentGroup;
	PropCategory*		m_pCurrentCat;
	ListProp*			m_pCurrentListProp;
	PROJECT_TYPE		m_currentSetType;
};

} // namespace Projects

#endif //#ifndef projectprops_h__included