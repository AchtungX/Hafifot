#include "ifconfig_script.h"

int main() {
    //the total msg to be printed of ifconfig
    char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN];
    memset(result_msg, 0, sizeof(result_msg));

    //list of if names
    char if_names[MAX_IF_NUMBER][MAX_IF_NAME_LEN];
    memset(if_names, 0, sizeof(if_names));
    int IF_num = get_IF_names(if_names);
    CHECK(IF_num > 0, "error in get_IF_names func");
    
    //add netwoork data on IF to result msg for each IF
    for (int i = 0; i<IF_num; i++) {
        append_net_data(if_names[i], i, result_msg);
        CHECK(IF_num > 0, "error in append_net_data func");
    }
    
    //add data on packets stat in the interfaces
    append_packet_stat(result_msg, if_names);
    CHECK(IF_num > 0, "error in append_packet_stat func");

    for (__uint32_t i=0; i<(MAX_IF_NUMBER*MAX_LINE_PER_IF); i++){
        if(strlen(result_msg[i]) ==0 ) {
            continue;;
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
        //skip on '.' and '..' entries in dir
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (count >= MAX_IF_NUMBER) {
            printf("Reached max interface count (%d)\n", MAX_IF_NUMBER);
            break;
        }

        strncpy(if_names[count], entry->d_name, MAX_IF_NAME_LEN - 1);
        if_names[count][MAX_IF_NAME_LEN - 1] = '\0'; //add null-termination
        count++;
    }

    closedir(if_dir);
    return count;

error:
    return -1;
}

int append_net_data(char if_name[MAX_IF_NAME_LEN], int IF_idx, char result_msg[MAX_IF_NUMBER*MAX_LINE_PER_IF][MAX_LINE_LEN]) {
    //open socket for ioct req
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
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
    //add first line - if_name, flags and mtu
    int ret = ioctl(socket_fd, SIOCGIFFLAGS, &ifr);
    CHECK(ret == 0, "Error in ioct");
    short flags = ifr.ifr_flags; //save it before another ioctl req

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

    //add second line - inet, netmask, broadcast
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

        char *token =strtok(line, " ");  //split by space
        __uint32_t count = 0;
        char params_list[MAX_PARAMS][MAX_PARAM_LEN];


        //check if its the line of the interface - if so add the two lines 
        for(__uint32_t i =0; i<MAX_IF_NUMBER; i++) {
            if(strlen(if_names[i]) == 0 || token == NULL) {    //end when there is no more if to go through or when the line is empty
                break;
            }
            //if this line belongs to one of the interfaces - create params_list array
            if(strncmp(token, if_names[i], strlen(if_names[i]))== 0) {

                //read the rest of the line
                while (token != NULL && count < MAX_PARAMS) {
                strncpy(params_list[count], token, MAX_PARAM_LEN - 1);
                params_list[count][MAX_PARAM_LEN - 1] = '\0';  // null-terminate
                count++;    
                token = strtok(NULL, " ");
                }   

                //add the lines according to the format
                snprintf(result_msg[(i*MAX_LINE_PER_IF)+4], MAX_LINE_LEN, "\tRX packets %s bytes %s (%s MB)\n", params_list[2], params_list[1], params_list[1]);
                snprintf(result_msg[(i*MAX_LINE_PER_IF)+5], MAX_LINE_LEN, "\tRX errors %s dropped %s overruns %s fram %s\n", params_list[3], params_list[4], params_list[5], params_list[6]);
                snprintf(result_msg[(i*MAX_LINE_PER_IF)+6], MAX_LINE_LEN, "\tTX packets %s bytes %s (%s MB)\n", params_list[10], params_list[9], params_list[9]);
                snprintf(result_msg[(i*MAX_LINE_PER_IF)+7], MAX_LINE_LEN, "\tTX errors %s dropped %s overruns %s fram %s\n", params_list[11], params_list[12], params_list[13], params_list[14]);
            }
        }
    }   

    return 0;
error:
    return -1;
}
