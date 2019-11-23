
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>

using namespace std;

int main(int argc, char **argv)
{
	int s;
	struct sockaddr_can addr;
	struct can_filter rfilter[4];
	struct can_frame frame;
	int nbytes, i;
	struct ifreq ifr;
	char *ifname = "can0";
	int ifindex;
	int opt;
	int tempa, tempb;

	/* sockopt test */
	int loopback = 0;
	int set_loopback = 0;
	int recv_own_msgs = 0;
	int set_recv_own_msgs = 0;
	int send_one_frame = 0;
	int ignore_errors = 0;

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		return 1;
	}

	rfilter[0].can_id   = 273;
	rfilter[0].can_mask = CAN_SFF_MASK;
	rfilter[1].can_id   = 0x200;
	rfilter[1].can_mask = 0x700;
	rfilter[2].can_id   = 0x80123456;
	rfilter[2].can_mask = 0x1FFFF000;
	rfilter[3].can_id   = 0x80333333;
	rfilter[3].can_mask = CAN_EFF_MASK;

	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
/*
	if(set_loopback)
		setsockopt(s, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));

	if(set_recv_own_msgs)
		setsockopt(s, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs));
*/
	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);
	ifindex = ifr.ifr_ifindex;

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	}

	

	while (1) {
		
	cout << "Enter any input";
	cin >> send_one_frame;
	
	if(send_one_frame) {

		frame.can_id  = 0x123;
		frame.can_dlc = 1;
		frame.data[0] = 0x11;

		nbytes = write(s, &frame, sizeof(struct can_frame));
	}
		if ((nbytes = read(s, &frame, sizeof(struct can_frame))) < 0) {
			perror("read");
			if (!ignore_errors)
				return 1;
		} else if (nbytes < sizeof(struct can_frame)) {
			fprintf(stderr, "read: incomplete CAN frame\n");
			return 1;
		} else {
			if (frame.can_id & CAN_EFF_FLAG)
			{
				tempa = frame.can_id&CAN_EFF_MASK;
				cout << "id: " << tempa << endl;
			}

			else
			{
				tempb = frame.can_id&CAN_SFF_MASK;
				cout << "id: " << tempb << endl;
			}

			//cout << "data length: %d " << frame.can_dlc << endl;
			printf("data length: %d \n", frame.can_dlc);

			for (i = 0; i < frame.can_dlc; i++) {
				//cout <<"%02X" << frame.data[i] << endl;
				printf("%02X", frame.data[i]);
			}
			if (frame.can_id & CAN_RTR_FLAG)
				cout <<"remote request";
			cout << "\n";
			fflush(stdout);
		}
	}

	close(s);

	return 0;
}

