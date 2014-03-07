# $FreeBSD$

MAINTAINER=tyler@freebsd.org

CFLAGS+=-g -fPIC -fno-exceptions -fstack-protector -fvisibility=hidden

CC_COMPILER!= ${CC} --version

.if !empty(CC_COMPILER:M*clang*)
CFLAGS+=-Wall -Wno-deprecated
.endif

# WARNINGS
CFLAGS+=-Wextra -Wno-unused-parameter \
		-Wno-unused-label -Wformat -Wreturn-type \
		-Wsign-compare -Wmultichar -Winit-self \
		-Wuninitialized -Wformat-security \
		-Werror

# INCLUDES
CFLAGS+=-I${PWD}/../launchd \
		-I${PWD}/../liblaunch \
		-I/usr/local/include

# DEFINES
CFLAGS+=-DDEBUG

.include <bsd.prog.mk>
