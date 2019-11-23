#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/ioctl.h> 
#include <net/if.h> 
#include <linux/can.h> 
#include <linux/can/raw.h>

int main(int argc, char **argv) {

	int s;
	struct sockaddr_can addr;
	struct can_filter rfilter[4];
	struct can_frame frame;
	int nbytes, i;
	struct ifreq ifr;
	char *ifname = "can0";
	int ifindex;
	int opt;

	/* sockopt test */

	int loopback = 0;
	int set_loopback = 0;
	int recv_own_msgs = 1;
	int set_recv_own_msgs = 1;
	int send_one_frame = 0;
	int ignore_errors = 0;

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		return 1;
	}

	rfilter[0].can_id = 0x123;
	rfilter[0].can_mask = CAN_SFF_MASK;
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
/*	rfilter[1].can_id = 0x200;
	rfilter[1].can_mask = 0x700;
	rfilter[2].can_id = 0x80123456;
	rfilter[2].can_mask = 0x1FFFF000;
	rfilter[3].can_id = 0x80333333;
	rfilter[3].can_mask = CAN_EFF_MASK;



	if(set_loopback)
		setsockopt(s, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
	if(set_recv_own_msgs)
		setsockopt(s, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs));
	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);
	ifindex = ifr.ifr_ifindex;
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	}

	if(send_one_frame) {
		frame.can_id = 0x123;
		frame.can_dlc = 2;
		frame.data[0] = 0x11;
		frame.data[1] = 0x22;
		nbytes = write(s, &frame, sizeof(struct can_frame));
	}
*/
	while (1) {
		
		printf("%d\n",s);
		nbytes=read(s, &frame, sizeof(struct can_frame));
		
		printf("Inside While Loop");

                if (nbytes<0) {
			printf("above perror, inside 1st if");
			perror("read");
			printf("below perror, inside 1st if");
			if (!ignore_errors) {
                               printf("inside ignore_errors");
				return 1;
                        }
		} else if (nbytes < sizeof(struct can_frame)) {
                        printf("inside else if");
                        fprintf(stderr, "read: incomplete CAN frame\n");
			return 1;
		} else {
                        printf("Inside else");
			if (frame.can_id & CAN_EFF_FLAG)
				printf("%8X ", frame.can_id & CAN_EFF_MASK);
			else
				printf("%3X ", frame.can_id & CAN_SFF_MASK);
			printf("[%d] ", frame.can_dlc);
			for (i = 0; i < frame.can_dlc; i++) {
				printf("%02X ", frame.data[i]);
			}
			if (frame.can_id & CAN_RTR_FLAG)
				printf("remote request");
			printf("\n");
			fflush(stdout);
		}
	}
	close(s);
	return 0;
}
