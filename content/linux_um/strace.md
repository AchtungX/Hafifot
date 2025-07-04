# Strace

Strace is a powerful tool for tracing system calls and signals made by processes.
This exercise will help you understand how to use `strace` and `ltrace` to trace system calls and library calls made by common binaries.

!!! tip
      As always, start with `man 1 strace` and `man 1 ltrace`.

1. Run `strace` on some basic commands
   ```sh
   strace pwd
   strace ls
   strace echo "Hello, World!"
   ```
   Some of the calls here are due to libc initialization.
   Can you identify where the binary's logic starts?

2. Run `ltrace` on the same binaries

3. Where do `ifconfig` and `ps` get information from? Try to retrieve the same information using simple shell commands.

4. The `ip` command is a newer replacement for `ifconfig`. Consider `ip address`: does it work the same way as `ifconfig`?

5. Try to use `strace` on a `sudo` command. Why doesn't it work?
