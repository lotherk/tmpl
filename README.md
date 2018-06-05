# tmpl

**tmpl** - Generates [mkstemp(3)](https://man.openbsd.org/mkstemp) file and writes output of `PROGRAM` to it

## Help
```
tmpl 0.1 Copyright (C) 2018 Konrad Lother <k@hiddenbox.org>

This software is supplied WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. This is free
software, and you are welcome to redistribute it under certain conditions; see
the template COPYING for details.

Generates temp file via mkstemp(3) and writes output of PROGRAM to it.
Optionally prints output to STDOUT instead of creating temp file.

Usage: tmpl [-hV] [-f] [-d SECONDS] [-e KEY=VALUE] [-p PROGRAM] [ -c | [-B] -r COMMAND
[--stdout=FILE] [--stderr=FILE]] [--] ARGS

  -h, --help             Print help and exit
  -V, --version          Print version and exit


  -f, --force            Do not abort output generation if PROGRAM fails with
                           ARGS  (default=off)
  -e, --env=KEY=VALUE    Set environment variable KEY to VALUE prior to running
                           PROGRAM or COMMAND
  -d, --delete=SECONDS   Spawns new process which deletes temp file after
                           SECONDS seconds
  -p, --program=PROGRAM  Run PROGRAM for each given ARGS  (default=`/bin/sh')
  -c, --cat              Print output to STDOUT and exit (does not write temp
                           file)  (default=off)
  -r, --run=COMMAND      Run COMMAND and delete temp file on exit.
                           Example: tmpl -r "neomutt -F %f" ...
                           %f - path to temp file
  -B, --background       Fork to background when used with -r  (default=off)
      --stdout=FILE      Redirect STDOUT from COMMAND to FILE
      --stderr=FILE      Redirect STDERR from COMMAND to FILE

See tmpl(1) for more informations and examples.
Send bug reports to bugs+tmpl@hiddenbox.org
Visit https://github.com/lotherk/tmpl

```

## Manpage
```
tmpl(1)			    General Commands Manual		       tmpl(1)

NAME
     tmpl  Generates mkstemp(3) file and writes output of PROGRAM to it

SYNOPSIS
     tmpl [-hV] [-f] [-T FORMAT] [-e KEY=VALUE] [-d SECONDS] [-p PROGRAM]
	  [[-c] | [-r COMMAND] [[-B] [--stdout FILE] [--stderr FILE]]] [--]
	  ARGS

DESCRIPTION
     tmpl generates mkstemp(3) file and writes output of PROGRAM to it.
     Alternativley, when used with -c, prints generated output to STDOUT and
     does not create and return a mkstemp(3) file.  mkstemp(3) files are
     created with mode 0600 and set to 0400 via fchmod(2) after writing
     STDOUT.

     This utility is especially useful if you need to dynamically create
     configuration files for programs.

     tmpl does not have any templating language itself, instead it uses STDOUT
     from whatever you pass to -p PROGRAM.

     To use, for example, erb you'd do something like this:

	   tmpl -p '/usr/local/bin/erb -T-' /path/to/your.erb

OPTIONS
     -h, --help
	     Print help and exit

     -f, --force
	     Do not abort output generation if PROGRAM fails with one or more
	     ARGS.  Use with caution!

     -e, --env KEY=VALUE
	     Set environment variable KEY to VALUE prior to running PROGRAM or
	     COMMAND

     -d, --delete SECONDS
	     Spawns new process which deletes mkstemp(3) file after SECONDS
	     seconds. Value is passed to sleep(3).

     -p, --programm PROGRAM
	     Run PROGRAM for each given argument in ARGS.  Default is /bin/sh.

     -c, --cat
	     Print output to STDOUT and do not create and return mkstemp(3)
	     file. This can not be used with -r.

     -r, --run COMMAND
	     Run COMMAND and delete mkstemp(3) file on exit. Can not be used
	     with -r.  You can use the following variables in your COMMAND
	     string to be replaced by tmpl:

	     %f -   Path to mkstemp(3) generated file

	     Example:
		   tmpl -r 'neomutt -F %f' ~/.mutt/genconf.sh

     -B, --background
	     Fork to background. Can only be used with -r.

     --stdout FILE
	     Pipe STDOUT from COMMAND to FILE.	Can only be used with -r

     --stderr FILE
	     Pipe STDERR from COMMAND to FILE.	Defaults to the value of
	     --stdout if not set. Can only be used with -r.

FUNCTION
     tmpl calls PROGRAM for each given argument in ARGS.  The default for
     PROGRAM is /bin/sh.

     Example:

	   tmpl -p /bin/sh /path/to/script1.sh /path/to/script2.sh

     passes script1.sh and script2.sh subsequently to PROGRAM and will result
     in two system calls:

	   /bin/sh /path/to/script1.sh
	   /bin/sh /path/to/script2.sh

     If either script fails to execute no output or mkstemp(3) file will be
     generated unless -f is used.

     Example:

	   tmpl -f -r 'httpd -f %f' server.sh vhosts.sh

     will run httpd with the generated mkstemp(3) file as its configuration
     file even though one of the scripts might have failed. This could lead to
     an unsafe httpd config or otherwise unwanted behaviour. The use of -f is
     not encouraged unless you are really sure!	 STDERR of PROGRAM is never
     written to temp file or output.


EXAMPLES
     A typical use case would be running (neo)mutt with multiple accounts
     using a single configuration file.

	   tmpl -r 'neomutt -F %f' -e ACCOUNT=k@hiddenbox.org
	   ~/.mutt/mutt-gen-config.sh

     This will pass ~/.mutt/mutt-gen-config.sh, which is a script that
     generates a mutt configuration file, to /bin/sh. All output mutt-gen-
     config.sh generates will be written to a mkstemp(3) file which is then
     read by neomutt. Since neomutt is called by tmpl via -r, tmpl
     automatically deletes the generated mkstemp(3) file as soon as neomutt
     exits.

     The same example could also be written as:

	   neomutt -F $(tmpl -e ACCOUNT=k@hiddenbox.org
	   ~/.mutt/mutt-gen-config.sh)

     but in this case tmpl can not remove the mkstemp(3) file since it will
     never know when neomutt exits. A better approach would be something like

	   CFG=$(tmpl -e ACCOUNT=k@hiddenbox.org ~/.mutt/mutt-gen-config.sh)
	   neomutt -F $CFG; rm -f $CFG

     or

	   neomutt -F $(tmpl -d 5 -e ACCOUNT=k@hiddenbox.org
	   ~/.mutt/mutt-gen-config.sh)

     will give neomutt 5 seconds (via -d) to read the config from mkstemp(3)
     file, which then gets deleted.

     Try the following example in your terminal:

	   cat $(tmpl -p /bin/echo 'this is a test')

     or
	   tmpl -r 'cat %f' -p /bin/echo 'this is still a test'

     This should give you a basic overview of what tmpl can do for you.

FEATURES
     tmpl uses pledge(2) if available.

BUGS
     The parser for -r is pretty bad so don't try to use any fancy shell
     syntax with quotes or other special chars. It might lead to unexpected
     behaviour. You should use a wrapper script instead.

     Send bug reports to bugs+tmpl@hiddenbox.org.

SEE ALSO
     mkstemp(3)

AUTHORS
     tmpl is developed by Konrad Lother <k@hiddenbox.org>.

OpenBSD 6.3			 May 28, 2018			   OpenBSD 6.3

```
