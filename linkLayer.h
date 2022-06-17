/*
* --- AUTORES ---
* Óscar Baños Campos
* Eduardo Fraile González
* Grupos: L8
*/
#include <stdio.h>
#include <string.h> 
#include <pcap.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <time.h>
#include <ifaddrs.h> 
#include <netpacket/packet.h> 
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h> 

#ifndef _LINKLAYER_H
#define _LINKLAYER_H
#include "lista.h" 
#include "kbhit.h" 


typedef struct interface {
    char deviceName[10];
    unsigned char MACaddr[6];
    pcap_t *handle;

    //Handle's stadistics:
    int typeValue;
    unsigned int packetsPassed;
    unsigned int packetsNotPassed;

    //size buffer
    int buffsize;
}interface_t;


//Return the list of available network devices
pcap_if_t *GetAvailAdapters(); 

/*Print the list of available network devices and let choose one. Set the selected device on iface.
* @ sDevice: list of devices
* @ iface: interface struct
* Return 0 when there is no error
* List of errors:
* 1: the list of network devices is not loaded.
* 2: c error
*/
int SelectAdapter(pcap_if_t *sDevice, interface_t *iface);


/*Set device name on iface
* @ iface: interface struct 
* @ name: device name
* Return 0 when there is no error
*/
int setDeviceName(interface_t *iface, char *name); //return 0 //ADAPTER


/*Get the MAC address from the loaded device and set in iface
* @ iface: interface struct 
* Return 0 when there is no error
*/
int GetMACAdapter(interface_t *iface);

/*Print the MAC address from iface
* @ iface: interface struct 
* Return 0 when there is no error
*/
int PrintMACAdapter(interface_t *iface); 

/*Open the handle
* @ device: device name
* @ iface: interface struct 
* Return 0 when there is no error
* List of errors:
* 1: interface capture: failed
*/
int OpenAdapter(interface_t *iface); 

/*Close the handle
* @ iface: interface struct 
* Return 0 when there is no error
* List of errors:
* 1: Failed closing the handle.
*/
int CloseAdapter(interface_t *iface); 


/*Receive a frame 
* @ iface: interface struct 
* Return apacket_t .
*/
apacket_t ReceiveFrame(interface_t *iface);


/*Send a Frame  a packet
* @ iface: interface struct 
* @ p: payload
* @ payloadSize: Size of the payload
* Return apacket_t typedef.
*/
int SendFrame(interface_t *iface, unsigned char *p, int payloadSize);


/*Build a frame 
* @ srcMAC: source MAC 
* @ dstMAC: destination MAC
* @ protocol: Protocol/Type 
* @ p: payload: Payload of the frame 
* Return Frame.
*/
unsigned char * BuildFrame(unsigned char *srcMAC, unsigned char *dstMAC, unsigned char* protocol, unsigned char *payload);

/*Build a frame without payload
* @ srcMAC: source MAC 
* @ dstMAC: destination MAC
* @ protocol: Protocol/Type 
* Return Frame without payload.
*/
unsigned char * BuildHeader(unsigned char *srcMAC, unsigned char *dstMAC, unsigned char* protocol);

/*Clear Buffer
*/
void FreeBuffer();


#endif
