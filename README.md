# Y86VM

y86vm is a simple Y86 virtual machine/simulator designed to run programs written in the Y86 ISA.

## Building

Simply `cd y86vm` and `make` to build for Linux. For OSX, an XCode Project is provided.

## Running

Full instructions can be accessed with the `-h/--help` flag from the command line. As of the latest revision, they are such:

    y86vm v0.1
    A bare-bones Y86 virtual machine/emulator.
    Usage: ./y86vm [-h|--help] [-V|--version] [-v|--verbose] [-l|--limit [n]] inputfile
      -h/--help
        Show this screen and exit.
      -V/--version
        Print version information and exit.
      -v/--verbose
        Print every step.

      -f/--format (a|b)
        Set input format. (a)scii, or raw (b)ytes. Default ASCII.

      -l/--limit n
        Maximum numner of steps (debugging tool to pause execution at
        n steps). Unlimited when argument is unspecified, 1024 when argument is bare.


Sample program (`sample47`), in source (`.ys`) and assembled (bytes `.ybo`, ASCII encoded bytes `.yao`) is provided.

An example invocation:

    ./y86vm -f ascii -v --limit 1024 sample47.yao

Or for bytes:

    ./y86vm -f bytes --limit sample47.ybo

Email dakota(a)hackthetruth.org or create a new Github Issue with any questions.
