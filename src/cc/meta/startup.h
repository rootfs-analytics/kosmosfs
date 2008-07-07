/*!
 * $Id$ 
 *
 * Copyright 2008 Quantcast Corp.
 * Copyright 2006-2008 Kosmix Corp.
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
 *
 *
 * \file startup.h
 * \brief code for starting up the metadata server
 * \author Blake Lewis (Kosmix Corp.)
 */
#if !defined(KFS_STARTUP_H)
#define KFS_STARTUP_H

#include <string>

namespace KFS {

extern void kfs_startup(const std::string &logdir, const std::string &cpdir, uint32_t minChunkservers);

}
#endif // !defined(KFS_STARTUP_H)
