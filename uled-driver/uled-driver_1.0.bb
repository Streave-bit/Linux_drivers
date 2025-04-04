SUMMARY = "Driver de LED utilisateur pour RISC-V 64-bit"
DESCRIPTION = "Un simple driver kernel pour piloter une LED utilisateur"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://${WORKDIR}/uled_driver.c;md5=bc9db906205b6779e040772c5a786635"

SRC_URI = "file://uled_driver.c \
           file://Makefile"

S = "${WORKDIR}"

inherit module

COMPATIBLE_MACHINE = "qemuriscv64"

KERNEL_MODULE_AUTOLOAD = "uled_driver"

FILES_${PN} += "/lib/modules/${KERNEL_VERSION}/extra/uled_driver.ko"

do_compile() {
    oe_runmake -C ${STAGING_KERNEL_DIR} M=${S} modules
}

do_install() {
    install -d ${D}/lib/modules/${KERNEL_VERSION}/extra
    install -m 0644 uled_driver.ko ${D}/lib/modules/${KERNEL_VERSION}/extra/
}
