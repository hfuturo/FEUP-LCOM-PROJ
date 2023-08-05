# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/state_machine
.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/utf-8
.PATH: ${.CURDIR}/ui
.PATH: ${.CURDIR}/file


# source code files to be compiled
SRCS = main.c state_machine.c utils.c timer.c kbc.c mouse.c graphics.c image.c utf8_decode.c scancodes.c text_box.c copy_window.c file_list.c file.c directory.c rtc.c menu.c folder_icon.c background_info.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -finput-charset=UTF-8 -Wno-macro-redefined -fPIC -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
