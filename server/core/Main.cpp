/*
 * Copyright (C) 2015 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <StdioLog.h>
#include <Runtime.h>
#include <fcntl.h>
#include <unistd.h>
#include "CoreServer.h"

#ifdef INTEL
#include "IntelCoreServer.h"
#endif

int main(int argc, char **argv)
{
    StdioLog log;
    SystemInformation info;

#ifdef INTEL
    IntelCoreServer server;
#endif

    const char *consolePath = "/dev/serial/serial0/io";
    const char *consoleMount = "/dev/serial";

    log.setMinimumLogLevel(Log::Notice);

    // Is this the master core?
    if (info.coreId == 0)
    {
        close(0);
        close(1);
        close(2);

        waitMount(consoleMount);

        while (open(consolePath, O_RDWR) == -1);
        open(consolePath, O_RDWR);
        open(consolePath, O_RDWR);

        NOTICE("initializing on core0");
    }

#ifdef INTEL
    server.initialize();
    server.test();
    return server.runCore();
#else
    return 0;
#endif
}
