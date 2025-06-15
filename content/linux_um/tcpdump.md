# TCPDUMP

In this exercise, we'll write a `tcpdump`-like utility.
If you haven't already, do play around with the original `tcpdump` (as allways, `man 1 tcpdump` for info).

## Level 1
Write a basic utility that sniffs all ethernet packets seen by your machine.
Parse the ethernet headers, and print each packet's source and destination MAC addresses.

!!! tip
    See `socket(2)`, `raw(7)`, `packet(7)`.
    Take the time to understand what all of `socket()`'s arguments mean.
    Manpages are a greate place to explore this, especially the ones in section 7 that `socket(2)` references.

## Level 2
Now, support getting a specific network interface as input (with `-i IFACE`, just like `tcpdump` does).
Only show packets originating from this interface.

## Level 3
Like `tcpdump`, we want to be able to filter the packets we receive.
Luckily, the kernel has built-in support using BPF (Berkley Packet Filter). Take the time to read about it.

!!! warning
    Confusingly, BPF has almost nothing to do with eBPF (enhanced BPF). Keep this in mind when reading.

!!! tip
    1. Refer to `socket(7)` (especially the `SO_ATTACH_BPF` part).
    Do read the referenced guide in the Linux kernel source.
    2. Make sure you understand why both `socket(2)` and `socket(7)` exist.

Now, implement this feature to you utility. You may use a compile-time constant BPF.
!!! tip
    1. Use `tcpdump -dd` to generate C-compatible BPF code, and `tcpdump -d` for human-readable format.
    2. As always, it may be helpful to `strace` existing utilities.

## Level 4
TODO - libpcap

