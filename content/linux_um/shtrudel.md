# Shtrudel

Help! My keyboard just broke, and the `|` key doesn't work anymore. Let's use `@` instead.

## Level 1

Write a usermode C program that gets a single shell-like command line and runs it.

The following should work:
```
$ cd /my/shtrudel/dir
$ make
$ ./shtrudel /bin/pwd
/my/shtrudel/dir
$ ./shtrudel /bin/echo foo
foo
```

!!! tip
    See `fork(2)`, and `execve(2)` or `exec(3)`

## Level 2

Support optionally getting two commands, concatenated by an `@` token.
Your program should pipe the first command's STDOUT to the second one's STDIN, just like how bash handles `|` tokens.

The following should work:
```
$ cd /my/shtrudel/dir
$ make
$ ./shtrudel /bin/echo foo
foo
$ ./shtrudel /bin/echo foo @ /bin/cat
foo
$ ./shtrudel /bin/echo -e '1 foo\n2 bar\n3 foobar' @ /bin/grep foo
1 foo
3 foobar
```

!!!tip
    See `pipe(2)`, `dup2(2)`. It might be helpful to `strace` a real shell.

## Level 3 (Bonus!)
TODO setup CI, add tests

## Level 4
Finally, we want to support an arbitrary number of commands, just like your shell does.

The following should work:
```
$ cd /my/shtrudel/dir
$ make
$ ./shtrudel /bin/echo foo
foo
$ ./shtrudel /bin/echo foo @ /bin/cat @ /bin/cat
foo
$ ./shtrudel /bin/echo -e '1 foo\n2 bar\n3 foobar' @ /bin/grep foo
1 foo
3 foobar
$ ./shtrudel /bin/echo -e '1 foo\n2 bar\n3 foobar' @ /bin/grep foo @ /bin/grep bar
3 foobar
$ ./shtrudel /bin/echo -e '1 foo\n2 bar\n3 foobar' @ /bin/grep foo @ /bin/grep bar @ /bin/grep nonexist  # make sure the exit code also matches shell behavior
```
