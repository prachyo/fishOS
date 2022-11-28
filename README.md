toy-os
===

This is a rudimentary operating system built from scratch for didactic purposes. The current features include:

* A bootloader
* Tiny libc implementation
* Serial output
* Basic memory management
* Paging and virtual memory 
* User mode
* Small set of system calls

Dependencies
-------
```
sudo apt-get install build-essential nasm genisoimage bochs bochs-x
```

Running
-------

```
cd src
make
make run
```
