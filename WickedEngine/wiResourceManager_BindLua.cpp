#include "wiResourceManager_BindLua.h"
#include "wiSound_BindLua.h"

#include "wiHelper.h"


const char wiResourceManager_BindLua::className[] = "Resource";

Luna<wiResourceManager_BindLua>::FunctionType wiResourceManager_BindLua::methods[] = {
	lunamethod(wiResourceManager_BindLua, Get),
	lunamethod(wiResourceManager_BindLua, Add),
	lunamethod(wiResourceManager_BindLua, Del),
	lunamethod(wiResourceManager_BindLua, List),
	{ NULL, NULL }
};
Luna<wiResourceManager_BindLua>::PropertyType wiResourceManager_BindLua::properties[] = {
	{ NULL, NULL }
};

wiResourceManager_BindLua::wiResourceManager_BindLua(lua_State *L)
{
	wiResourceManager::wiResourceManager();
}
wiResourceManager_BindLua::~wiResourceManager_BindLua()
{
}

int wiResourceManager_BindLua::Get(lua_State *L)
{
	int argc = wiLua::SGetArgCount(L);
	if (argc > 1)
	{
		string name = wiLua::SGetString(L, 2);
		const Resource* data = get(name);
		if (data != nullptr)
		{
			switch (data->type)
			{
			case Data_Type::MUSIC:
			case Data_Type::SOUND:
				Luna<wiSound_BindLua>::push(L, new wiSound_BindLua((wiSound*)data->data));
				return 1;
				break;
			default:
				wiLua::SError(L, "Resource:Get(string name) resource type not supported!");
				break;
			}
		}
		else
		{
			wiLua::SError(L, "Resource:Get(string name) resource not found!");
		}
		return 0;
	}
	else
	{
		wiLua::SError(L, "Resource:Get(string name) not enough arguments!");
	}
	return 0;
}
int wiResourceManager_BindLua::Add(lua_State *L)
{
	int argc = wiLua::SGetArgCount(L);
	if (argc > 1)
	{
		string name = wiLua::SGetString(L, 2);
		Data_Type type = Data_Type::DYNAMIC;
		if (argc > 2) //type info also provided in this case
		{
			string typeStr = wiHelper::toUpper( wiLua::SGetString(L, 3) );
			if (!typeStr.compare("SOUND"))
				type = Data_Type::SOUND;
			else if (!typeStr.compare("MUSIC"))
				type = Data_Type::MUSIC;
		}
		void* data = add(name, type);
		wiLua::SSetString(L, (data != nullptr ? "ok" : "not found"));
		return 1;
	}
	else
	{
		wiLua::SError(L, "Resource:Add(string name, (opt) string type) not enough arguments!");
	}
	return 0;
}
int wiResourceManager_BindLua::Del(lua_State *L)
{
	int argc = wiLua::SGetArgCount(L);
	if (argc > 1)
	{
		string name = wiLua::SGetString(L, 2);
		wiLua::SSetString(L, (del(name) ? "ok" : "not found"));
		return 1;
	}
	else
	{
		wiLua::SError(L, "Resource:Del(string name) not enough arguments!");
	}
	return 0;
}
int wiResourceManager_BindLua::List(lua_State *L)
{
	stringstream ss("");
	ss << "Resources of: " << wiLua::SGetString(L, 1) << endl;
	for (auto& x : resources)
	{
		ss << x.first << endl;
	}
	wiLua::SSetString(L, ss.str());
	return 1;
}

void wiResourceManager_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		Luna<wiResourceManager_BindLua>::Register(wiLua::GetGlobal()->GetLuaState());
		wiLua::GetGlobal()->RegisterObject(className, "globalResources", wiResourceManager::GetGlobal());
		initialized = true;
	}
}

