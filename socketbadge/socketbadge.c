//
// Created by deker on 8/5/16.
//
// Quick hack to try and make the DEFCON 24 car hacking village badge into a pseudo-SocketCAN interface
//
// The basic idea here is to write a userspace SocketCAN client that will read/write the badge via USB
// and gateway that data to/from a SocketCAN virtual CAN (vcan) interface.
//

#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <getopt.h>
#include <libgen.h>
#include <string.h>
#include <sys/ioctl.h>
#include <zconf.h>
#include <pthread.h>
#include <libusb-1.0/libusb.h>

#define BADGEINTERFACES 2
#define VID 0x0483
#define PID 0x580C
#define ENDPOINT_IN 0x81
#define ENDPOINT_OUT 0x01
#define BADGE_CMD 0x01

#define USB_HDR_LEN 2
#define SC_FRAME_LEN 16
#define USB_MSG_LEN (USB_HDR_LEN + SC_FRAME_LEN)

struct usb_msg {
    uint8_t cmd;    // Badge command byte
    uint8_t dev;      // 0 for can0 1 for can1
    struct can_frame frame;
};

int cs0, cs1; // CAN Sockets

static volatile int running = 1;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -0 <can0-interface> -1 <can1_interface>\n", prg);
    fprintf(stderr, "  Use CTRL-C to terminate %s\n", prg);
    fprintf(stderr, "\t-0 <can0-interface>\t- Local CAN interface to bridge to badge can0\n");
    fprintf(stderr, "\t-1 <can1-interface>\t- Local CAN interface to bridge to badge can1\n");
}

void sigterm(int signo) {
    running = 0;
}

void *usb_thread(void *ptr) {
    libusb_device_handle *handle = (libusb_device_handle *) ptr;

    unsigned char data[64]; // Buffer for data transfer
    struct usb_msg *msg = (struct usb_msg *) data;
    unsigned int actual_length;
    int r;

    fprintf(stderr, "Entered USB thread\n");

    while (1) {
        r = libusb_bulk_transfer(handle, ENDPOINT_IN, data, sizeof(data), &actual_length, 0);
        if (r != 0) {
            fprintf(stderr, "libusb_bulk_transfer() failed");
            goto error;
        }

       /* fprintf(stderr, "Received a frame over USB\n");*/
        if (actual_length == USB_MSG_LEN) {

/*            int i;
            for (i=0; i<=23; i++)
            {
                fprintf(stdout, "%.02X ", data[i]);
            }
            fprintf(stdout, "\n---\n");*/

            struct can_frame out;
            // Prep USB message
            memcpy(&out.can_id, data+2, 4);
            memcpy(&out.can_dlc, data+6, 1);
            memcpy(&out.data, data+10, 8);


            if (msg->dev == 0) {
                write(cs0, (void *) &out, sizeof(struct can_frame));
            }
            else if (msg->dev == 1) {
                write(cs1, (void *) &out, sizeof(struct can_frame));
            }
            else {
                fprintf(stderr, "Invalid interface in message. Ignored.\n");
            }
        }
        else {
            fprintf(stderr, "USB frame read wrong length (%d:%d). Ignored\n", USB_MSG_LEN, actual_length);

            /*// For testing, just print data
            int i;
            fprintf(stderr, "msg command: %d\n", msg->cmd);
            fprintf(stderr, "bus number: %d\n", msg->dev);

            for (i=0; i<=23; i++)
            {
                fprintf(stdout, "%.02X ", data[i]);
            }
*/
/*            fprintf(stdout, "can%d %03X#", msg->dev, msg->frame.can_id);
            for (i = 0; i <= 7; i++) {
                fprintf(stdout, "%.02X", msg->frame.data[i]);
            }

            fprintf(stdout, "\n");
            */
        }

    }

    error:
    pthread_exit((void *) NULL);

}

int main(int argc, char **argv) {
    static char *ifacenames[BADGEINTERFACES];
    int opt;
    int got0, got1 = 0;
    struct sockaddr_can addr;
    struct ifreq ifr;
    int maxfd = 0;
    fd_set rdfs;
    int ret = -1;
    int nbytes;
    int i, r;
    int j = 0;
    struct can_frame frame;
    unsigned char buf[64];
    struct usb_msg *msg = (struct usb_msg *) buf;
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 5000;

    // libusb stuff
    libusb_device_handle *handle;
    libusb_device *dev;
    libusb_device **devs;
    int actual_length;
    int cnt;

    //pthreads
    pthread_t usbthread;


    signal(SIGTERM, sigterm);
    signal(SIGHUP, sigterm);
    signal(SIGINT, sigterm);

    while ((opt = getopt(argc, argv, "0:1:")) != -1) {
        switch (opt) {
            case '0':

                if (strlen(optarg) >= IFNAMSIZ) {
                    fprintf(stderr, "Name of CAN device '%s' is too long!\n\n", optarg);
                    return 1;
                } else {
                    cs0 = socket(PF_CAN, SOCK_RAW, CAN_RAW);
                    if (cs0 < 0) {
                        perror("can0 socket");
                        return 1;
                    }
                    addr.can_family = AF_CAN;
                    strcpy(ifr.ifr_name, optarg);

                    if (ioctl(cs0, SIOCGIFINDEX, &ifr) < 0) {
                        perror("can0 SIOCFIFINDEX");
                        return 1;
                    }

                    addr.can_ifindex = ifr.ifr_ifindex;

                    if (!addr.can_ifindex) {
                        perror("invalid can0 interface");
                        return 1;
                    }

                    if (bind(cs0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
                        perror("can0 bind");
                        return 1;
                    }
                }
                got0++;

                break;
            case '1':

                if (strlen(optarg) >= IFNAMSIZ) {
                    fprintf(stderr, "Name of CAN device '%s' is too long!\n\n", optarg);
                    return 1;
                } else {
                    cs1 = socket(PF_CAN, SOCK_RAW, CAN_RAW);
                    if (cs1 < 0) {
                        perror("can1 socket");
                        return 1;
                    }
                    addr.can_family = AF_CAN;
                    strcpy(ifr.ifr_name, optarg);

                    if (ioctl(cs1, SIOCGIFINDEX, &ifr) < 0) {
                        perror("can1 SIOCFIFINDEX");
                        return 1;
                    }

                    addr.can_ifindex = ifr.ifr_ifindex;

                    if (!addr.can_ifindex) {
                        perror("invalid can0 interface");
                        return 1;
                    }

                    if (bind(cs1, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
                        perror("can1 bind");
                        return 1;
                    }
                }

                got1++;

                break;
            default:
                print_usage(basename(argv[0]));
                return 1;
        }
    }

    // Make sure we got options for both interfaces

    if ((got0 == 0) || (got1 == 0)) {
        close(cs0);
        close(cs1);
        fprintf(stderr, "Didn't specify both required interfaces\n");

        print_usage(basename(argv[0]));
        return 1;
    }

    r = libusb_init(NULL);
    if (r != 0) {
        fprintf(stderr, "libusb_init() failed\n");
        return r;
    }


    fprintf(stderr, "Attempting to open USB device with VID:PID %.04X:%.04X\n", VID, PID);
    handle = libusb_open_device_with_vid_pid(0, VID, PID);
    //libusb_open(dev, &handle);
    if (handle == NULL) {
        fprintf(stderr, "libusb_open_device() failed\n");
        return 1;
    }

    // Start USB reader thread
    ret = pthread_create(&usbthread, NULL, (void *) usb_thread, (void *) handle);
    if (ret != 0) {
        fprintf(stderr, "pthread_create() bailed\n");
        return 1;
    }

    // Get our largest file descriptor for select() loop
    if (cs0 > maxfd) {
        maxfd = cs0;
    }
    if (cs1 > maxfd) {
        maxfd = cs1;
    }

    while (running) {

        // Check for a dead USB reader thread
        ret = pthread_tryjoin_np(usbthread, NULL);
        if (ret == 0)
        {
            // Thread exited. We should too...
            fprintf(stderr, "USB reader thread exited.");
            return 1;
        }

        // select() on socket descriptors for CAN interfaces to get host->badge data
        FD_ZERO(&rdfs);
        FD_SET(cs0, &rdfs);
        FD_SET(cs1, &rdfs);

        if ((ret = select(maxfd + 1, &rdfs, NULL, NULL, &timeout)) <= 0) {
            //perror("select");
            //running = 0;
            continue;
        }

        if (FD_ISSET(cs0, &rdfs)) {
            // Data available to send to badge can0
            nbytes = recv(cs0, &frame, sizeof(struct can_frame), 0);

/*            // For testing, just print data
            fprintf(stdout, "can0 %03X#", frame.can_id);
            for (i = 0; i <= frame.can_dlc; i++) {
                fprintf(stdout, "%.02X", frame.data[i]);
            }
            fprintf(stdout, "\n");*/


            // Prep USB message
            memset(buf, 0x00, 64);
            buf[0]=BADGE_CMD;
            buf[1]=0x00;
            memcpy(buf+2, &frame.can_id, 4);
            memcpy(buf+6, &frame.can_dlc, 1);
            memcpy(buf+10, &frame.data, 8);

        }


        if (FD_ISSET(cs1, &rdfs)) {
            // Data available to send to badge can1
            nbytes = recv(cs1, &frame, sizeof(struct can_frame), 0);

           /* // For testing, just print data
            fprintf(stdout, "can1 %03x#", frame.can_id);
            for (i = 0; i <= frame.can_dlc; i++) {
  s              fprintf(stdout, "%.02x", frame.data[i]);
            }
            fprintf(stdout, "\n");*/

            // Prep USB message
            memset(buf, 0x00, 64);
            buf[0]=BADGE_CMD;
            buf[1]=0x01;
            memcpy(buf+2, &frame.can_id, 4);
            memcpy(buf+6, &frame.can_dlc, 1);
            memcpy(buf+10, &frame.data, 8);

        }

        // Send data to USB
        fprintf(stderr, "Sending frame via USB\n");
        r = libusb_bulk_transfer(handle, ENDPOINT_OUT, buf, USB_MSG_LEN, &actual_length, 500);
        if (r != 0) {
            fprintf(stderr, "libusb_bulk_transfer() failed in sending");
            return 1;
        }

        if (actual_length != USB_MSG_LEN) {
            fprintf(stderr, "USB frame wrote wrong length (%d). Ignored\n", actual_length);
        }

    }

    // Clean up sockets
    close(cs0);
    close(cs1);

}

