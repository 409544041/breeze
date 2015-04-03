﻿#include "netManager.h"
#include "dbManager.h"
using namespace zsummer::mysql;

DBManager::DBManager()
{
	_authDB = std::make_shared<DBHelper>();
	_infoDB = std::make_shared<DBHelper>();
	_logDB = std::make_shared<DBHelper>();
	_dbAsync = std::make_shared<DBAsync>();
}
DBManager::~DBManager()
{
	
}

bool DBManager::stop()
{
	while (_dbAsync->getFinalCount() != _dbAsync->getPostCount())
	{
		LOGA("Waiting the db data to store. waiting count=" << _dbAsync->getPostCount() - _dbAsync->getFinalCount());
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	LOGA("_dbAsync total FinalCount  :" << _dbAsync->getFinalCount());
	_authDB->stop();
	_infoDB->stop();
	_logDB->stop();
	_dbAsync->stop();
	return true;
}
bool DBManager::start()
{
	//启动db异步操作线程
	if (!_dbAsync->start())
	{
		return false;
	}
	LOGD("DBAsync start success.  begin connect to db ...");
	
	//创建DBHelper
	//////////////////////////////////////////////////////////////////////////
	{
		const auto authConfig = ServerConfig::getRef().getDBConfig(AuthDB);
		const auto infoConfig = ServerConfig::getRef().getDBConfig(InfoDB);
		const auto logConfig = ServerConfig::getRef().getDBConfig(LogDB);
		_authDB->init(authConfig._ip, authConfig._port, authConfig._db, authConfig._user, authConfig._pwd);
		_infoDB->init(infoConfig._ip, infoConfig._port, infoConfig._db, infoConfig._user, infoConfig._pwd);
		_logDB->init(logConfig._ip, logConfig._port, logConfig._db, logConfig._user, logConfig._pwd);

		if (!_authDB->connect())
		{
			LOGE("connect Auth DB false. db config=" << authConfig);
			return false;
		}
		LOGD("connect Auth DB success. db config=" << authConfig);

		if (!_infoDB->connect())
		{
			LOGE("connect Info DB false. db config=" << infoConfig);
			return false;
		}
		LOGD("connect Info DB success. db config=" << infoConfig);

		if (!_logDB->connect())
		{
			LOGE("connect Log DB false. db config=" << logConfig);
			return false;
		}
		LOGD("connect Log DB success. db config=" << logConfig);
	}

	return true;
}
