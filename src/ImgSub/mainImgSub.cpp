// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>

#include "ImgSub.h"
#include "libUtil/util.h"
#include "libCfg/Cfg.h"


#include <mutex>
#include <shared_mutex>

using namespace std;
using namespace app;

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		std::cout << "pass a config file" << std::endl;
		return 0;
	}

	// read cfg file
	printf("used cfg=<%s>\n", argv[1]);
	CfgPtr cfg(new Cfg());
	cfg->readFromFile(argv[1]);
	cout << cfg->toString() << endl;
	CfgSubImg cfgSub = cfg->getSubImg();


	// create log
	const std::string logFilename = std::string("log-sub-") + cfgSub.topicName + ".txt";
	startLogThread(logFilename, cfg->getLog().ishowLogInConsole );

	//run sub
	bool use_environment_qos = false;
	std::shared_ptr<CfgSubImg> cfgSubImg( new CfgSubImg( cfgSub ) );

	ImgSub mysub(cfgSubImg);
	if (mysub.init(use_environment_qos))
	{
		mysub.run();
	}

	endLogThread();
	return 0;
}
