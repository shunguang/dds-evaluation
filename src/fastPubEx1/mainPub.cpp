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

#include "HelloWorldPublisher.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include "libUtil/util.h"

using namespace std;
using namespace app;

int main(int argc, char** argv)
{
	const string logFilename("logPub.txt");
	const bool showInConsole = true;
	startLogThread(logFilename, showInConsole);

	uint32_t count = 10;
	uint32_t sleep_ms = 100;
	bool use_environment_qos = false;

	HelloWorldPublisher mypub;
	if (mypub.init(use_environment_qos))
	{
		mypub.run(count, sleep_ms);
	}

	endLogThread();
	return 0;
}
