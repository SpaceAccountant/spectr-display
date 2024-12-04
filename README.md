Overview
====
The SPECTR Display project is a sub-project of SPECTR. It provide an interface for the SSD1351 displays of the system.

Requirements
====
The current version of SPECTR and its sub-projects run on a Broadcom BCM2836 based system running Linux, specifically a Raspberry Pi 2B+. This project additionally requires the `spectr_io` module from the SECTR IO project loaded to work.

Building
====
To build the project you can use the provided Makefile so long as you are building on the target system and have the kernel source tree under `/lib/modules/$(uname -r)/build`. If you have the source tree elsewhere just modify the KERNELDIR variable in the Makefile to reflect this. If you're cross compiling then I'm not explaining how to set it up.

You must provide an additional variable at the command line, `SPECTR_IO`, which points to the directory (without trailing slash) that the SPECTR IO project root is located.

Running
====
You must first load the `spectr_io` module provided by the SPECTR IO project. To run the project after loading `spectr_io` and after building you can simply run `insmod spectr_display.ko` as root.

Installing
====
You must first install the `spectr_io` module provided by the SPECTR IO project to install this module. Then put the module somewhere under `/lib/modules/$(uname -r)/kernel/drivers`, I recommend under `/lib/modules/$(uname -r)/kernel/drivers/spectr/display`. Then you can do modify the necessary configuration files for your distro to load it at boot, remembering to load the `spectr_io` module first.

