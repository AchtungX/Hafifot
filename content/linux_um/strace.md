# Strace

## Introduction
Strace is a powerful tool for tracing system calls and signals made by processes. This exercise will help you understand how to use `strace` and `ltrace` to trace system calls and library calls made by common coreutils binaries such as `ls`, `echo`, and `pwd`.

### Accessing the Exercise

1. **Run `strace` on `ls`**
   ```sh
   strace ls
   ```

2. **Run `strace` on `echo`**
   ```sh
   strace echo "Hello, World!"
   ```

3. **Run `strace` on `pwd`**
   ```sh
   strace pwd
   ```

### Example Outputs

#### Running `strace ls`
