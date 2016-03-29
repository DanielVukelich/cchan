# C-CHAN
The fastest meme sharing service ever.

## HOW TO
### COMPILE
1. Make sure you are running an FSF-approved, POSIX-compliant GNU/Linux distribution, or at the very least a libre \*BSD (still must be POSIX-compliant). If you still run botnet in $CURRENT_YEAR, just go to <https://gnu.org/distros/free-distros.html> and pick your freedom.
2. C-chan currently only requires an C89 compatible standard library (any version of glibc will do), a proper C89 compatible compiler (I suggest the GNU C Compiler, also known as the GNU Compiler Collection), the package make, and OpenSSL-dev (or your distro's equivalent package).
3. Run "make"

### INSTALL
No installation atm, just play with the files the way it works tbqh lad

### RUN
Once compiled, run "./cchan [port number]". Default port is 80, so you might need root access. 
If you don't want to or can't run cchan as root, provide a port number you're allowed to use.
C-chan currently does:
* Static file serving. Files must be in the static/ directory, and are accessed with the following URI: "/static/path_to_your_file"
* favicon.ico. File "favicon.ico" is served from the static/directory
* index. At URI "/", it statically serves static/index.html

COPYRIGHT (C) 2016 the C-chan contributors.
