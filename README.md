# isolator - PHP extension for onetime minimum container

## Introduction

This extension adds minimum linux container functionality to PHP.

## Requirements

This extension currently works with PHP 5.6.+ on Linux.

## Installation

### Via source

To install via source, clone this repo and then run the following:

```bash
$ cd /path/to/source
$ phpize
$ ./configure
$ make install
```

Then, move the built module to your extensions directory.

### Enable extension

You will want to enable the extension in php.ini by adding:

```text
extension="isolator.so"
```

## Usage


```php
<?php
$HOME= "/home/guest";
$uid = posix_getuid();

Isolator::unshare(CLONE_NEWUSER|CLONE_NEWNET|CLONE_NEWNS|CLONE_NEWUTS);

file_put_contents('/proc/self/uid_map',"0 $uid 1");
file_put_contents('/proc/self/setgroups','deny');
file_put_contents('/proc/self/gid_map','0 1000 1');

system('/sbin/ip link add br0 type bridge');

Isolator::mount('/tmp',                       "$HOME/rootfs/tmp",'bind',MS_MGC_VAL|MS_BIND);
Isolator::mount('/usr/local/ssh-shared/lib'  ,"$HOME/rootfs/lib",'bind',MS_MGC_VAL|MS_BIND);
Isolator::mount('/usr/local/ssh-shared/lib64',"$HOME/rootfs/lib64",'bind',MS_MGC_VAL|MS_BIND);
Isolator::mount('/usr/local/ssh-shared/bin'  ,"$HOME/rootfs/bin",'bind',MS_MGC_VAL|MS_BIND);
Isolator::mount('/usr/local/ssh-shared/usr'  ,"$HOME/rootfs/usr",'bind',MS_MGC_VAL|MS_BIND);
Isolator::mount('/usr/local/lang'            ,"$HOME/rootfs/usr/local/lang",'bind',MS_MGC_VAL|MS_BIND);

Isolator::chroot("$HOME/rootfs");

/*
 *
 * SOME Dangerous Codes :)
 *
 */

system("/bin/rm -rf /home /etc");

```

