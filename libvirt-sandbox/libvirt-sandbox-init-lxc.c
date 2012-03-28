/*
 * libvirt-sandbox-init-lxc.c: lxc guest startup
 *
 * Copyright (C) 2009-2011 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

/*
 * This is a crazy small init process that runs as the init
 * of the container. Its job is to decode the command line
 * and run the user specified real include.
 */

#include <config.h>

#include <glib.h>

#include <stdio.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void set_debug(void);

static int debug = 0;

int
main(int argc G_GNUC_UNUSED, char **argv G_GNUC_UNUSED)
{
    const char *args[50];
    int narg = 0;

    if (getenv("LIBVIRT_LXC_UUID") == NULL) {
        fprintf(stderr, "libvirt-sandbox-init-lxc: must be run as the 'init' program of an LXC guest\n");
        exit(EXIT_FAILURE);
    }

    set_debug();

    if (debug)
        fprintf(stderr, "libvirt-sandbox-init-lxc: starting up\n");

#if 0
#define STRACE "/usr/bin/strace"
#define STRACE_FILTER "trace=read,write,poll,close"
#endif

    memset(&args, 0, sizeof(args));
#ifdef STRACE
    args[narg++] = STRACE;
    args[narg++] = "-q";
    // args[narg++] = "-f";
    args[narg++] = "-e";
    args[narg++] = STRACE_FILTER;
//    args[narg++] = "-s";
//    args[narg++] = "2000";
#endif
    args[narg++] = LIBEXECDIR "/libvirt-sandbox-init-common";
    if (debug)
        args[narg++] = "-d";

    if (debug)
        fprintf(stderr, "Running interactive\n");
    execv(args[0], (char**)args);
    fprintf(stderr, "libvirt-sandbox-init-lxc: %s: cannot execute %s: %s\n",
            __func__, args[0], strerror(errno));
    exit(EXIT_FAILURE);
}


static void set_debug(void)
{
    const char *env = getenv("LIBVIRT_LXC_CMDLINE");

    if (env &&
        strstr(env, "debug"))
        debug=1;
}
