/*!
 * $Id$ 
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
 *
 *
 * \file thread.h
 * \brief thread control for KFS metadata server
 * \author Blake Lewis (Kosmix Corp.)
 */
#if !defined(KFS_THREAD_H)
#define KFS_THREAD_H

#include <cassert>
#include "common/config.h"

extern "C" {
#include <pthread.h>
}

namespace KFS {

class MetaThread {
	pthread_mutex_t mutex;
	pthread_cond_t cv;
	pthread_t thread;
	static const pthread_t NO_THREAD = -1u;
public:
	typedef void *(*thread_start_t)(void *);
	MetaThread(): thread(NO_THREAD)
	{
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&cv, NULL);
	}
	~MetaThread()
	{
		pthread_mutex_destroy(&mutex);
                if (thread != NO_THREAD) {
                    int UNUSED_ATTR status = pthread_cancel(thread);
                    assert(status == 0);
                }
		pthread_cond_destroy(&cv);
	}
	void lock()
	{
		int UNUSED_ATTR status = pthread_mutex_lock(&mutex);
		assert(status == 0);
	}
	void unlock()
	{
		int UNUSED_ATTR status = pthread_mutex_unlock(&mutex);
		assert(status == 0);
       	}
	void wakeup()
	{
		int UNUSED_ATTR status = pthread_cond_broadcast(&cv);
		assert(status == 0);
	}
	void sleep()
	{
		int UNUSED_ATTR status = pthread_cond_wait(&cv, &mutex);
		assert(status == 0);
	}
	void start(thread_start_t func, void *arg)
	{
		int UNUSED_ATTR status;
		status = pthread_create(&thread, NULL, func, arg);
		assert(status == 0);
	}
	void stop()
	{
		int UNUSED_ATTR status = pthread_cancel(thread);
		assert(status == 0);
	}
};

}

#endif // !defined(KFS_THREAD_H)