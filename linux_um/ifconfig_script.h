#ifndef IFCONFIG_SCRIPT_H
#define IFCONFIG_SCRIPT_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <string.h>
#include <net/if.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define CHECK(val, log_msg) \
    do { \
        if (!(val)) { \
            printf("%s\n", log_msg); \
            goto error; \
        } \
    } while (0)

#define CHECK_W_SOCKET(val, log_msg) \
    do { \
        if (!(val)) { \
            printf("%s\n", log_msg); \
            printf("ioctl error: %d\n", errno); \
            goto close_socket; \
        } \
    } while (0)

__uint32_t MAX_LINE_PER_IF = 8;
__uint32_t MAX_IF_NUMBER = 20;
__uint32_t MAX_IF_NAME_LEN = 32;
__uint32_t MAX_LINE_LEN = 128;
__uint32_t MAX_PARAMS = 15;
__uint32_t MAX_PARAM_LEN = 32;
__uint32_t IPv4_LEN = 16;

const char *IF_dir_path = "/sys/class/net/";
const char *IF_stat_file_path = "/proc/net/dev";

/*
get_IF_names - this function gets the name of the interfaces for this computer from "/sys/class/net/" folder
                input - 
                        (char [MAX_IF_NUMBER][MAX_IF_NAME_LEN]) interfaces - an array to save the interfaces names
                output - 
                        add to interfaces the name of the interfaces found and returns 0 in case of sucess, -1 otherwise
*/
int get_IF_names(char if_names[MAX_IF_NUMBER][MAX_IF_NAME_LEN]);

/*
append_net_data - this function gets the data on the interface setup using ioctl
                input -
                        (char [MAX_IF_NAME_LEN]) if_name - the name of the interface
                        (int) if_idx - the index of the interface - for offset in result msg
                        (char [MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]) result msg - a list of lines in the size nedded for ifconig message len
                output - 
                        adds to result_msg the relevant rows and returns 0 in case of sucess, -1 otherwise
*/
int append_net_data(char if_name[MAX_IF_NAME_LEN], int if_idx, char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]);

/*
append_packet_stat - this function gets the data on the interface packet stats, reads from "/proc/net/dev"
                input - 
                        (char [MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]) result msg - a list of lines in the size nedded for ifconig message len
                        (char [MAX_IF_NUMBER][MAX_IF_NAME_LEN]) if_names - an array of the interfaces names         
                output - 
                        adds to result_msg the relevant rows and returns 0 in case of sucess, -1 otherwise
*/
int append_packet_stat(char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN], char if_names[MAX_IF_NUMBER][MAX_IF_NAME_LEN]);

/*
add_name_flags_mtu - this function gets called inside append_net_data it gets the name, flags and mtu of a specific IF state by if_name using ioctl
                input - 
                        (int) socket_fd - a fd for the socket, already open
                        (int) if_idx - the index of the interface - for offset in result msg
                        (struct ifreq) ifr - a struct of ifreq for ioctl req, already set to if_name
                        (char [MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]) result msg - a list of lines in the size nedded for ifconig message len
                output - 
                        adds to result_msg the relevant rows and returns 0 in case of sucess, -1 otherwise
*/
int add_name_flags_mtu(int socket_fd, int if_idx, struct ifreq ifr, char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]);

/*
add_ip_addr - this function gets called inside append_net_data it gets the ip, netmask and broadcast addresses of a specific IF state by if_name using ioctl
                input - 
                        (int) socket_fd - a fd for the socket, already open
                        (int) if_idx - the index of the interface - for offset in result msg
                        (struct ifreq) ifr - a struct of ifreq for ioctl req, already set to if_name
                        (char [MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]) result msg - a list of lines in the size nedded for ifconig message len
                output - 
                        adds to result_msg the relevant rows and returns 0 in case of sucess, -1 otherwise
*/
int add_ip_addr(int socket_fd, int if_idx, struct ifreq ifr, char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]);

#endif