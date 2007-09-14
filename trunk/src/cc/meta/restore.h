/*!
 * $Id: //depot/SOURCE/OPENSOURCE/kfs/src/cc/meta/restore.h#3 $
 *
 * \file restore.h
 * \brief rebuild metatree from saved checkpoint
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
#if !defined(KFS_RESTORE_H)
#define KFS_RESTORE_H

#include <fstream>
#include <string>
#include <deque>
#include <map>
#include "util.h"

using std::ifstream;
using std::string;
using std::deque;
using std::map;

namespace KFS {

/*!
 * \brief state for restoring from a checkpoint file
 */
class Restorer {
	ifstream file;			//!< the CP file
public:
	bool rebuild(string cpname);	//!< process the CP file
};

extern bool restore_chunkVersionInc(deque <string> &c);

}
#endif // !defined(KFS_RESTORE_H)
