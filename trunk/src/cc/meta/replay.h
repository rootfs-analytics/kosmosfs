/*!
 * $Id: //depot/SOURCE/OPENSOURCE/kfs/src/cc/meta/replay.h#3 $
 *
 * \file replay.h
 * \brief log replay definitions
 * \author Blake Lewis (Kosmix Corp.)
 *
 * Copyright 2006 Kosmix Corp.
 *
 * This file is part of Kosmos File System (KFS).
 *
 * Licensed under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#if !defined(KFS_REPLAY_H)
#define KFS_REPLAY_H

#include <string>
#include <fstream>

using std::string;
using std::ifstream;

namespace KFS {

class Replay {
	ifstream file;		//!< the log file being replayed
	string path;		//!< path name for log file
	int number;		//!< sequence number for log file
public:
	Replay(): number(-1) { };
	~Replay() { };
	void openlog(const string &p);	//!< open the log file for replay
	int playlog();			//!< read and apply its contents
	int logno() { return number; }
};

extern Replay replayer;

}
#endif // !defined(KFS_REPLAY_H)
