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

/**
 * @file HelloWorld_main.cpp
 *
 */

#include <limits>
#include <sstream>

#include "HelloWorldSubscriber.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastrtps/log/Log.h>
#include "libUtil/util.h"

using namespace std;
using namespace app;
int main(int argc, char** argv)
{
	bool use_environment_qos = false;
	const string logFilename("./logSub.txt");
	const bool showInConsole = true;
	startLogThread( logFilename, showInConsole);

	HelloWorldSubscriber mysub;
	if (mysub.init(use_environment_qos))
	{
		mysub.run();
	}

	endLogThread();
	return 0;
}
