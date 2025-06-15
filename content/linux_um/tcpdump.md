# TCPDUMP

In this exercise, we'll write a `tcpdump`-like utility.
If you haven't already, do play around with the original `tcpdump` (as allways, `man 1 tcpdump` for info).

## Level 1
Write a basic utility that sniffs all ethernet packets seen by your machine.
Parse the ethernet headers, and print each packet's source and destination MAC addresses.

## Level 2
Now, support getting a specific network interface as input (with `-i IFACE`, just like `tcpdump` does).
Only show packets originating from this interface.

## Level 3, 4
TODO - Attaching bpf, using libpcap

