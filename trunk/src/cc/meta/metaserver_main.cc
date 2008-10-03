//---------------------------------------------------------- -*- Mode: C++ -*-
// $Id$ 
//
// Created 2006/03/22
// Author: Blake Lewis
//
// Copyright 2008 Quantcast Corp.
// Copyright 2006-2008 Kosmix Corp.
//
// This file is part of Kosmos File System (KFS).
//
// Licensed under the Apache License, Version 2.0
// (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.
//
// \file Main.cc
// \brief Driver code that starts up the meta server
//
//----------------------------------------------------------------------------

#include <signal.h>

#include "common/properties.h"
#include "libkfsIO/NetManager.h"
#include "libkfsIO/Globals.h"

#include "NetDispatch.h"
#include "startup.h"

using namespace KFS;

// NetDispatch KFS::gNetDispatch;

// Port at which KFS clients connect and send RPCs
int gClientPort;

// Port at which Chunk servers connect
int gChunkServerPort;

// paths for logs and checkpoints
string gLogDir, gCPDir;

// min # of chunk servers to exit recovery mode
uint32_t gMinChunkservers;

int16_t gMinReplicasPerFile;

Properties gProp;

int ReadMetaServerProperties(char *fileName);

using std::cout;
using std::endl;

int
main(int argc, char **argv)
{
        fd_set rfds;

        if (argc < 2) {
                cout << "Usage: " << argv[0] << " <properties file> {<msg log file>}" << endl;
                exit(0);
        }

	if (argc > 2) {
		KFS::MsgLogger::Init(argv[2]);
	} else {
		KFS::MsgLogger::Init(NULL);
	}

        if (ReadMetaServerProperties(argv[1]) != 0) {
                cout << "Bad properties file: " << argv[1] << " aborting..." << endl;
                exit(-1);
        }

	libkfsio::InitGlobals();

        kfs_startup(gLogDir, gCPDir, gMinChunkservers, gMinReplicasPerFile);

        // Ignore SIGPIPE's that generated when clients break TCP
        // connection.
        //
        signal(SIGPIPE, SIG_IGN);

        gNetDispatch.Start(gClientPort, gChunkServerPort);

        while (1) {
		struct timeval timeout;

		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

                FD_ZERO(&rfds);
                FD_SET(0, &rfds);
                select(1, &rfds, NULL, NULL, &timeout);
        	// block the main thread without consuming too much CPU
		// if the net dispatch thread has gotten going, this method
		// never returns
		gNetDispatch.WaitToFinish();
        }


        return 0;
}

///
/// Read and validate the configuration settings for the meta
/// server. The configuration file is assumed to contain lines of the
/// form: xxx.yyy.zzz = <value>
/// @result 0 on success; -1 on failure
/// @param[in] fileName File that contains configuration information
/// for the chunk server.
///

int
ReadMetaServerProperties(char *fileName)
{
	string logLevel;
#ifdef NDEBUG
	const char *defLogLevel = "INFO";
#else
	const char *defLogLevel = "DEBUG";
#endif

        if (gProp.loadProperties(fileName, '=', true) != 0)
                return -1;

        gClientPort = gProp.getValue("metaServer.clientPort", -1);
        if (gClientPort < 0) {
                cout << "Aborting...bad client port: " << gClientPort << endl;
                return -1;
        }
        cout << "Using meta server client port: " << gClientPort << endl;

        gChunkServerPort = gProp.getValue("metaServer.chunkServerPort", -1);
        if (gChunkServerPort < 0) {
                cout << "Aborting...bad chunk server port: " << gChunkServerPort << endl;
                return -1;
        }
        cout << "Using meta server chunk server port: " << gChunkServerPort << endl;
	gLogDir = gProp.getValue("metaServer.logDir","");
	gCPDir = gProp.getValue("metaServer.cpDir","");
	bool wormMode = gProp.getValue("metaServer.wormMode", false);

	const char *clusterKey = gProp.getValue("metaServer.clusterKey", "");
	cout << "Setting cluster key to: " << clusterKey << endl;
	setClusterKey(clusterKey);

	// min # of chunkservers that should connect to exit recovery mode
	gMinChunkservers = gProp.getValue("metaServer.minChunkservers", 1);
	cout << "min. # of chunkserver that should connect: " << gMinChunkservers << endl;

	// desired min. # of replicas per file
	gMinReplicasPerFile = gProp.getValue("metaServer.minReplicasPerFile", 1);
	cout << "min. # of replicas per file: " << gMinReplicasPerFile << endl;

	if (wormMode) {
		cout << "Enabling WORM mode" << endl;
		setWORMMode(wormMode);
	}

	logLevel = gProp.getValue("metaServer.loglevel", defLogLevel);
        if (logLevel == "INFO")
	        KFS::MsgLogger::SetLevel(log4cpp::Priority::INFO);
	else
		KFS::MsgLogger::SetLevel(log4cpp::Priority::DEBUG);

        return 0;
}
