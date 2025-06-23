#include "ifconfig_script.h"

int main() {
    char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]; //A space for the total result msg of ifconfig

    char if_names[MAX_IF_NUMBER][MAX_IF_NAME_LEN]; //List of interfaces names
    int IF_num = get_IF_names(if_names);
    CHECK(IF_num > 0, "error in get_IF_names func");
    
    for (int i = 0; i<IF_num; i++) {
        CHECK(append_net_data(if_names[i], i, result_msg) == 0, "error in append_net_data func"); //Add netwoork data on each IF to result msg
    }
    
    CHECK(append_packet_stat(result_msg, if_names) == 0, "error in append_packet_stat func"); //Add data on packets stat of the interfaces

    for (__uint32_t i=0; i<(IF_num*MAX_LINE_PER_IF); i++){
        if(strlen(result_msg[i]) ==0 ) {
            continue;
        }
        printf("%s", result_msg[i]);
    }
    return 0;

error:
    return -1;
}

int get_IF_names(char if_names[MAX_IF_NUMBER][MAX_IF_NAME_LEN]) {
    DIR *if_dir = opendir(IF_dir_path);
    CHECK(if_dir, "Cannt open net dir");

    struct dirent *entry;
    __uint32_t count = 0;

    while ((entry = readdir(if_dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) //skip on '.' and '..' entries in dir
            continue;

        if (count >= MAX_IF_NUMBER) {
            printf("Reached max interface count (%d)\n", MAX_IF_NUMBER);
            break;
        }
        strncpy(if_names[count], entry->d_name, MAX_IF_NAME_LEN - 1);
        if_names[count][MAX_IF_NAME_LEN - 1] = '\0'; //Add null-termination
        count++;
    }

    closedir(if_dir);
    return count;

error:
    return -1;
}

int append_net_data(char if_name[MAX_IF_NAME_LEN], int IF_idx, char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]) {
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0); //Open socket for ioctl req.
    CHECK(socket_fd > 0, "error in open socket");

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name, IFNAMSIZ - 1);

    CHECK_W_SOCKET(add_name_flags_mtu(socket_fd, IF_idx, ifr, result_msg) == 0, "Error in add_name_flags_mtu");

    CHECK_W_SOCKET(add_ip_addr(socket_fd, IF_idx, ifr, result_msg) == 0, "Error in add_ip_addr");

    close(socket_fd);
    return 0;

close_socket:
    close(socket_fd);
error:
    return -1;
}

int add_name_flags_mtu(int socket_fd, int IF_idx, struct ifreq ifr, char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]){
    int ret = ioctl(socket_fd, SIOCGIFFLAGS, &ifr);
    CHECK(ret == 0, "Error in ioct");
    short flags = ifr.ifr_flags; //Save it before another ioctl req

    ret = ioctl(socket_fd, SIOCGIFMTU, &ifr);
    CHECK(ret == 0, "Error in ioct");

    snprintf(result_msg[(IF_idx*MAX_LINE_PER_IF)], MAX_LINE_LEN, "%s: flags = %d <> mtu = %d\n", ifr.ifr_name, flags, ifr.ifr_mtu);

    return 0;
error:
    return -1;
}

int add_ip_addr(int socket_fd, int IF_idx, struct ifreq ifr, char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]) {
    char str_ip_addr[IPv4_LEN];
    char str_ip_addr_cp[IPv4_LEN];
    char str_nm_addr[IPv4_LEN];
    char str_nm_addr_cp[IPv4_LEN];
    char str_br_addr[IPv4_LEN];
    char str_br_addr_cp[IPv4_LEN];

    int ret = ioctl(socket_fd, SIOCGIFADDR, &ifr);
    CHECK(ret == 0, "Error in ioct");
    struct sockaddr_in *ip_addr = (struct sockaddr_in *)&ifr.ifr_addr;
    CHECK(inet_ntop(AF_INET, &ip_addr->sin_addr, str_ip_addr, IPv4_LEN-1), "Error in inet_ntop");
    str_ip_addr[IPv4_LEN-1] = '\0';
    snprintf(str_ip_addr_cp, IPv4_LEN, "%s", str_ip_addr);

    ret = ioctl(socket_fd, SIOCGIFNETMASK, &ifr);
    CHECK(ret == 0, "Error in ioct");
    struct sockaddr_in *nm_addr = (struct sockaddr_in *)&ifr.ifr_netmask;
    CHECK(inet_ntop(AF_INET, &nm_addr->sin_addr, str_nm_addr, IPv4_LEN-1), "Error in inet_ntop");
    str_nm_addr[IPv4_LEN-1] = '\0';
    snprintf(str_nm_addr_cp, IPv4_LEN, "%s", str_nm_addr);

    ret = ioctl(socket_fd, SIOCGIFBRDADDR, &ifr);
    CHECK(ret == 0, "Error in ioct");
    struct sockaddr_in *br_addr = (struct sockaddr_in *)&ifr.ifr_broadaddr;
    CHECK(inet_ntop(AF_INET, &br_addr->sin_addr, str_br_addr, IPv4_LEN-1), "Error in inet_ntop");
    str_br_addr[IPv4_LEN-1] = '\0';
    snprintf(str_br_addr_cp, IPv4_LEN, "%s", str_br_addr);

    snprintf(result_msg[(IF_idx*MAX_LINE_PER_IF)+1], MAX_LINE_LEN, "\tinet %s  netmask %s  broadcast  %s\n", str_ip_addr_cp, str_nm_addr_cp, str_br_addr_cp); 
    return 0;

error:
    return -1;
}

int append_packet_stat(char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN], char if_names[MAX_IF_NUMBER][MAX_IF_NAME_LEN]) {
    char line[MAX_LINE_LEN];
    int line_num = 0;

    FILE *packets_stat_file = fopen(IF_stat_file_path, "r");
    CHECK(packets_stat_file, "Couldn't open stat file");

    while(fgets(line, MAX_LINE_LEN-1, packets_stat_file)) {
        line_num ++;
        line[MAX_LINE_LEN] = '\0';

        char *token =strtok(line, " "); //Split line by space
        __uint32_t count = 0;
        char params_list[MAX_PARAMS][MAX_PARAM_LEN];

 
        for(__uint32_t i =0; i<MAX_IF_NUMBER-1; i++) {
            if(strlen(if_names[i]) == 0 || token == NULL || strlen(token) == 0) { //End when there is no more interfaces to go through or when the line is empty
                result_msg[(i*MAX_LINE_PER_IF)][0] = '\0';
                break;
            }
            
            if(strncmp(token, if_names[i], strlen(if_names[i]))== 0) { //Check if this line belongs to one of the interfaces, if so create params_list array

                while (token != NULL && count < MAX_PARAMS) { //Read the rest of the line
                    strncpy(params_list[count], token, MAX_PARAM_LEN - 1);
                    params_list[count][MAX_PARAM_LEN - 1] = '\0';  //Add null-terminate
                    count++;    
                    token = strtok(NULL, " ");
                }

                snprintf(result_msg[(i*MAX_LINE_PER_IF)+4], MAX_LINE_LEN, "\tRX packets %s bytes %s (%s MB)\n", params_list[RX_PACKETS_IDX], params_list[RX_BYTES_IDX], params_list[RX_BYTES_IDX]); //Add lines to result_msg according to ifconfig format
                snprintf(result_msg[(i*MAX_LINE_PER_IF)+5], MAX_LINE_LEN, "\tRX errors %s dropped %s overruns %s fram %s\n", params_list[RX_ERR_IDX], params_list[RX_DROP_IDX], params_list[RX_OVERRUN_IDX], params_list[RX_FRAME_IDX]);
                snprintf(result_msg[(i*MAX_LINE_PER_IF)+6], MAX_LINE_LEN, "\tTX packets %s bytes %s (%s MB)\n", params_list[TX_PACKETS_IDX], params_list[TX_BYTES_IDX], params_list[TX_BYTES_IDX]);
                snprintf(result_msg[(i*MAX_LINE_PER_IF)+7], MAX_LINE_LEN, "\tTX errors %s dropped %s overruns %s fram %s\n", params_list[TX_ERR_IDX], params_list[TX_DROP_IDX], params_list[TX_OVERRUN_IDX], params_list[TX_FRAME_IDX]);
            }
        }
    }   

    return 0;
error:
    return -1;
}
