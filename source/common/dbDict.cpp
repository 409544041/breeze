﻿

/*
* breeze License
* Copyright (C) 2014-2016 YaweiZhang <yawei.zhang@foxmail.com>.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "dbDict.h"
#include "config.h"
#include "utls.h"



DBHelperPtr buildHelper(const std::string & db)
{
    auto dbconfigs = ServerConfig::getRef().getDBConfig();
    auto founder = std::find_if(dbconfigs.begin(), dbconfigs.end(), [db](const DBConfig & config) {return config._name == db; });
    if (founder == dbconfigs.end())
    {
        LOGE(db << " not found");
        return nullptr;
    }
    auto helper = std::make_shared<DBHelper>();
    helper->init(founder->_ip, founder->_port, founder->_db, founder->_user, founder->_pwd, true);
    if (!helper->connect())
    {
        LOGE("can't connect mysql " << db << ",  config=" << *founder);
        return nullptr;
    }
    return helper;
}




















