#include "Helix.h"
#include "UDP.h"
#define APP_UDP_PORT_BASE       5000
#define APP_UDP_INSTANT_RX      1
#define APP_UDP_RTT_LOG         (1)
#define     ECHO_SERVER_IP_ADDRESS IP_ADDRESS(192,2,2,211)
static NX_UDP_SOCKET g_udp_sck;

static void g_udp_sck_receive_cb(NX_UDP_SOCKET *);

/* UDP entry function */
void UDP_entry(void)
{
    int i;
    UINT status;
    ULONG event;
    ULONG link_status;
    /* Create all sockets and bind them to their respective ports */
    /* Wait for the IP stack and network hardware
     * to get initialized.
     */
    ///Should be replaced with a proper wait flag.
    tx_thread_sleep (50);

    ip_init0 ();
    tx_thread_sleep (50);
    while (machineGlobalsBlock->globalsInit != 1)
        tx_thread_sleep (1);
    status = nx_ip_status_check (&g_ip0, NX_IP_INITIALIZE_DONE, &link_status, NX_WAIT_FOREVER);
    if (DEBUG)
    {
        printf ("\nIP initialization complete. IP:%s", IPADDSTRING);
    }

    if (DEBUG)
    {
        printf ("\nUDP initializing...");
    }
    /* Check for error.  */
    if (DEBUGGER)
    {
        if (NX_SUCCESS != status)
        {
            printf ("\nFail.");
        }
        else
        {
            printf ("\nLink enabled.");
        }
    }
    status = nx_udp_socket_create(&g_ip0, &machineGlobalsBlock->g_udp_sck, "UDP Socket", NX_IP_NORMAL, NX_DONT_FRAGMENT,
                                  NX_IP_TIME_TO_LIVE, 512);
    if (DEBUG)
    {
        if (NX_SUCCESS != status)
        {
            printf ("\nFail.");
        }
        else
        {
            printf ("\nUDP socket creation successful.");
        }
    }
    status = nx_udp_socket_bind (&machineGlobalsBlock->g_udp_sck, APP_UDP_PORT_BASE,
    NX_NO_WAIT);
    if (DEBUG)
    {
        if (NX_SUCCESS != status)
        {
            printf ("\nFail.");
        }
        else
        {
            printf ("\nUDP socket binding successful.");
        }
    }
    status = nx_udp_socket_receive_notify (&machineGlobalsBlock->g_udp_sck, g_udp_sck_receive_cb);
    if (DEBUG)
    {
        if (NX_SUCCESS != status)
        {
            printf ("\nFail.");
        }
        else
        {
            printf ("\nUDP receive interrupt setup successful. Bound to port:%d",
                    machineGlobalsBlock->g_udp_sck.nx_udp_socket_port);
        }
    }

    ///Clear the event flags
    status = tx_event_flags_get (&g_udp_echo_received, 1, TX_AND_CLEAR, &event, NX_NO_WAIT);

    if (DEBUG)
    {
        printf ("\nUDP initialization complete.");
    }

    NX_PACKET *my_packet;
    NX_PACKET *receive_packet;
    UCHAR buffer[64];
    ULONG bytes_copied;
    char tmp[15];
    while (1)
    {
        tx_thread_sleep (1000);
        //        tx_thread_suspend (tx_thread_identify ());
    }
}

///This function sends the contents of the UDP buffer to a target slave device.
void UDPSend()
{
    UINT status;
    ULONG tmp = 0;
    char echoACK = 0;
    ioport_level_t level;
    NX_PACKET *my_packet;
    do
    {
        status = nx_packet_allocate (&g_packet_pool0, &my_packet, NX_UDP_PACKET, NX_WAIT_FOREVER);
        /* Check for error.  */
        nx_packet_data_append (my_packet, &machineGlobalsBlock->UDPBuffer, UDPMSGLENGTH, &g_packet_pool0,
        NX_WAIT_FOREVER);

        if (DEBUG)
        {
            printf ("\nSending:%s...", machineGlobalsBlock->UDPBuffer);
        }

        status = nx_udp_socket_send(&machineGlobalsBlock->g_udp_sck, my_packet, IP_ADDRESS(192,168,10,182), 5000);

        if (NX_SUCCESS == status)
        {
            if (DEBUG)
            {
                printf ("\nSend success.");
            }
        }
        else
        {
            if (DEBUG)
            {
                printf ("\nSend fail.");
            }
            nx_packet_release(my_packet);
        }

//        machineGlobalsBlock->echoWaitStart = 1;
        status = tx_event_flags_get (&g_udp_echo_received, 1, TX_AND_CLEAR, &tmp, 300);
    }
    while (tmp == 0);
    if (DEBUG)
    {
        printf ("\nSend complete.");
    }
    memset (machineGlobalsBlock->UDPBuffer, 0, UDPMSGLENGTH);
}

char UDPGetStatus()
{
    UINT status;
    ULONG tmp = 0;
    tmp = 0;
    do
    {
        machineGlobalsBlock->UDPBuffer[0] = 't';
        UDPSend ();
        status = tx_event_flags_get (&g_udp_data_received, 1, TX_AND_CLEAR, &tmp, 300);
    }
    while (tmp == 0);
    return machineGlobalsBlock->UDPRxBuff[2];
}

///This function is used to set the temperature of a tool
UDPSetTemperature(char toolID, double temp)
{

    machineGlobalsBlock->UDPBuffer[0] = '4';
    machineGlobalsBlock->UDPBuffer[1] = toolID;
    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &temp, 8);

    UDPSend ();
}

///This function is used to get the current position of a target axis.
double UDPGetTemp(char toolID)
{
    UINT status;
    ULONG tmp = 0;
    double data;
    do
    {
        machineGlobalsBlock->UDPBuffer[0] = '5';
        switch (toolID)
        {
            case 'a':
                machineGlobalsBlock->UDPBuffer[1] = 'a';
            break;
            default:
            break;
        }
        UDPSend ();

        status = tx_event_flags_get (&g_udp_data_received, 1, TX_AND_CLEAR, &tmp, 300);
        memcpy (&data, (machineGlobalsBlock->UDPRxBuff + 2), 8);
    }
    while (tmp == 0);
    ///Reset receive flag after retrieval of received data.
//    machineGlobalsBlock->UDPRX = 0;

    return data;
}

///This function gets fresh tool status information, including hotend temperatures, from the slave(s)
void UDPGetToolUpdate()
{
    toolBlockA->tempRead = UDPGetTemp ('a');
}

void processUDPRx(NX_PACKET *p_packet)
{
    UINT status;
    if (memcmp (machineGlobalsBlock->UDPBuffer, p_packet->nx_packet_prepend_ptr, UDPMSGLENGTH) == 0)
    {
///An echo packet was received.
        status = tx_event_flags_set (&g_udp_echo_received, 1, TX_OR);
    }
    else
    {
///A data packet was received.
        memcpy (machineGlobalsBlock->UDPRxBuff, p_packet->nx_packet_prepend_ptr, UDPMSGLENGTH);
        status = tx_event_flags_set (&g_udp_data_received, 1, TX_OR);
    }
}

///This function is used to run a motor at a target frequency.
void UDPRunMotorFrequency(char axis, double freq)
{
    machineGlobalsBlock->UDPBuffer[0] = 'f';
    machineGlobalsBlock->UDPBuffer[1] = axis;
    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &freq, 8);

    UDPSend ();
    machineGlobalsBlock->motorFreqSet = 1;
}

void UDPStopMotor(char axis)
{

    machineGlobalsBlock->UDPBuffer[0] = 's';
    machineGlobalsBlock->UDPBuffer[1] = axis;
    UDPSend ();
}

void UDPZeroAxes()
{

    machineGlobalsBlock->UDPBuffer[0] = 'z';
    machineGlobalsBlock->UDPBuffer[1] = 'z';
    UDPSend ();
}

void UDPSetMotorDir(char axis, int fwd)
{

    machineGlobalsBlock->UDPBuffer[0] = 'd';

    machineGlobalsBlock->UDPBuffer[1] = axis;

    if (fwd)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'f';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'r';
    }

    UDPSend ();
}

///Sets relay high or low. Level is 'h' or 'l'.
void UDPSetRelay(char axis, char relay, char level)
{

    machineGlobalsBlock->UDPBuffer[0] = 'r';
    switch (axis)
    {
        case 'x':
            machineGlobalsBlock->UDPBuffer[1] = 'x';
        break;
        case 'y':
            machineGlobalsBlock->UDPBuffer[1] = 'y';
        break;
        case 'z':
            machineGlobalsBlock->UDPBuffer[1] = 'z';
        break;
        default:
        break;
    }

    machineGlobalsBlock->UDPBuffer[2] = relay;

    machineGlobalsBlock->UDPBuffer[3] = level;

    UDPSend ();
}

void UDPHomeMotor(char axis)
{

    machineGlobalsBlock->UDPBuffer[0] = 'h';
    switch (axis)
    {
        case 'x':
            machineGlobalsBlock->UDPBuffer[1] = 'x';
        break;
        case 'y':
            machineGlobalsBlock->UDPBuffer[1] = 'y';
        break;
        case 'z':
            machineGlobalsBlock->UDPBuffer[1] = 'z';
        break;
        default:
        break;
    }
    UDPSend ();
}

void UDPSendINIX()
{
    if (DEBUG)
    {
        printf ("\nSending X INI data.");
    }
    machineGlobalsBlock->UDPBuffer[0] = '0';
    machineGlobalsBlock->UDPBuffer[1] = 'x';

    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &motorBlockX->stepSize, 8);

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '1';
    machineGlobalsBlock->UDPBuffer[1] = 'x';
    if (motorBlockX->fwdDir == IOPORT_LEVEL_HIGH)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'h';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'l';
    }

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '2';
    machineGlobalsBlock->UDPBuffer[1] = 'x';
    if (motorBlockX->defaultDir == IOPORT_LEVEL_HIGH)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'h';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'l';
    }

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '3';
    machineGlobalsBlock->UDPBuffer[1] = 'x';
    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &motorBlockX->homeSpeed, 8);

    UDPSend ();
}

void UDPSendINIY()
{
    if (DEBUG)
    {
        printf ("\nSending Y INI data.");
    }
    machineGlobalsBlock->UDPBuffer[0] = '0';
    machineGlobalsBlock->UDPBuffer[1] = 'y';

    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &motorBlockY->stepSize, 8);

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '1';
    machineGlobalsBlock->UDPBuffer[1] = 'y';
    if (motorBlockY->fwdDir == IOPORT_LEVEL_HIGH)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'h';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'l';
    }

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '2';
    machineGlobalsBlock->UDPBuffer[1] = 'y';
    if (motorBlockY->defaultDir == IOPORT_LEVEL_HIGH)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'h';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'l';
    }

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '3';
    machineGlobalsBlock->UDPBuffer[1] = 'y';
    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &motorBlockY->homeSpeed, 8);

    UDPSend ();
}

void UDPSendINIZ()
{
    if (DEBUG)
    {
        printf ("\nSending Z INI data.");
    }
    machineGlobalsBlock->UDPBuffer[0] = '0';
    machineGlobalsBlock->UDPBuffer[1] = 'z';

    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &motorBlockZ->stepSize, 8);

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '1';
    machineGlobalsBlock->UDPBuffer[1] = 'z';
    if (motorBlockZ->fwdDir == IOPORT_LEVEL_HIGH)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'h';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'l';
    }

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '2';
    machineGlobalsBlock->UDPBuffer[1] = 'z';
    if (motorBlockZ->defaultDir == IOPORT_LEVEL_HIGH)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'h';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'l';
    }

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '3';
    machineGlobalsBlock->UDPBuffer[1] = 'z';
    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &motorBlockZ->homeSpeed, 8);

    UDPSend ();
}

void UDPSendINIA()
{
    if (DEBUG)
    {
        printf ("\nSending A INI data.");
    }
    machineGlobalsBlock->UDPBuffer[0] = '0';
    machineGlobalsBlock->UDPBuffer[1] = 'a';

    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &toolBlockA->motorBlock->stepSize, 8);

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '1';
    machineGlobalsBlock->UDPBuffer[1] = 'a';
    if (motorBlockA->fwdDir == IOPORT_LEVEL_HIGH)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'h';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'l';
    }

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '2';
    machineGlobalsBlock->UDPBuffer[1] = 'a';
    if (motorBlockA->defaultDir == IOPORT_LEVEL_HIGH)
    {
        machineGlobalsBlock->UDPBuffer[2] = 'h';
    }
    else
    {
        machineGlobalsBlock->UDPBuffer[2] = 'l';
    }

    UDPSend ();

    machineGlobalsBlock->UDPBuffer[0] = '3';
    machineGlobalsBlock->UDPBuffer[1] = 'a';
    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &toolBlockA->motorBlock->homeSpeed, 8);

    UDPSend ();
}

void UDPCalibrateMotor(char axis, double freq, double distance, int dir)
{
    UDPSetMotorDir (axis, dir);
    UDPSetMotorTargetSteps (axis, distance);
    UDPSetMotorFrequency (axis, freq);

    machineGlobalsBlock->UDPBuffer[0] = 'c';
    machineGlobalsBlock->UDPBuffer[1] = axis;
    machineGlobalsBlock->motorFreqSet = 1;
    UDPSend ();
}

///This function is used to set a target frequency for a motor.
void UDPSetMotorFrequency(char axis, double freq)
{
    machineGlobalsBlock->UDPBuffer[0] = 'g';
    machineGlobalsBlock->UDPBuffer[1] = axis;

    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &freq, 8);

    UDPSend ();
}

///This function is used to set a target position in steps for a motor.
void UDPSetMotorTargetSteps(char axis, double steps)
{
    machineGlobalsBlock->UDPBuffer[0] = 'q';
    machineGlobalsBlock->UDPBuffer[1] = axis;

    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &steps, 8);

    UDPSend ();
}

///This function is used to set the freqSet setting for a target motor.
void UDPSetMotorFreqSet(int freqSet)
{
    machineGlobalsBlock->UDPBuffer[0] = 'i';
    machineGlobalsBlock->UDPBuffer[2] = freqSet;

    UDPSend ();

    machineGlobalsBlock->motorFreqSet = freqSet;
}

///This function is used to set a target velocity for an axis.
void UDPSetTargetVelocity(char axis, double velocity)
{
    machineGlobalsBlock->UDPBuffer[0] = 'v';
    machineGlobalsBlock->UDPBuffer[1] = axis;

    memcpy ((machineGlobalsBlock->UDPBuffer + 2), &velocity, 8);

    UDPSend ();
}

///This function is used to get the current position of a target axis.
double UDPGetPosition(char axis)
{
    UINT status;
    ULONG tmp = 0;
    double data;
    do
    {
        machineGlobalsBlock->UDPBuffer[0] = 'u';
        switch (axis)
        {
            case 'x':
                machineGlobalsBlock->UDPBuffer[1] = 'x';
            break;
            case 'y':
                machineGlobalsBlock->UDPBuffer[1] = 'y';
            break;
            case 'z':
                machineGlobalsBlock->UDPBuffer[1] = 'z';
            break;
            default:
            break;
        }
        UDPSend ();

        status = tx_event_flags_get (&g_udp_data_received, 1, TX_AND_CLEAR, &tmp, 300);
        memcpy (&data, (machineGlobalsBlock->UDPRxBuff + 2), 8);
    }
    while (tmp == 0);
    ///Reset receive flag after retrieval of received data.
//    machineGlobalsBlock->UDPRX = 0;

    return data;
}

static void g_udp_sck_receive_cb(NX_UDP_SOCKET *p_sck)
{
    ULONG ip;
    UINT client_port;
    UINT status;
    NX_PACKET *p_packet = NULL;
    ULONG *length;
    ULONG tmp;
    char buff[50];
    /* Receive data here */
    nx_udp_socket_receive (p_sck, &p_packet, NX_NO_WAIT);
///No echo from the master
//    nx_udp_source_extract (p_packet, &ip, &client_port);
//    status = nx_udp_socket_send(p_sck, p_packet, ip, client_port);
    if (NULL != p_packet)
    {
        if (DEBUG)
        {
            printf ("\nUDP packet Received:%s.", p_packet->nx_packet_prepend_ptr);
        }
//        nx_packet_data_retrieve (p_packet, &machineGlobalsBlock->UDPRxBuff, length);

        processUDPRx (p_packet);
        nx_packet_release(p_packet);
    }
}
