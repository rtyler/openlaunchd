# $FreeBSD$

MAINTAINER=tyler@freebsd.org

CFLAGS+=-g -fPIC -fno-exceptions -fstack-protector -fvisibility=hidden

CC_COMPILER!= ${CC} --version

# Detect what compilers is used.
# Two  issues happens here.
# 1- the use of -Wno-deprecated with gcc, is only for C++ not for C,
# so detect the compiler used.
# 2- the use of pragma and -Wall, with gcc <= 4.7.2, cannot be used.
# in the futur, the version of gcc should be detected,
# (for use of -Wall).
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
