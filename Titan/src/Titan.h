#include <TitanParser.h>
#include "hal_data.h"
//#include "r_timer_api.h"
#define DEBUGGER 0
#define DEBUG 0
#define IPADDSTRING "192.168.10.181"
#define NXD_MQTT_MAX_MESSAGE_LENGTH 50
///Number of bytes in each UDP message. UDP packet:
/// control(1)|axis(1)|data(8)
/// 1 byte for control - Indicates the operation, such as 'd' for set direction.
/// 1 byte for axis - Indicates the target axis, such as 'x'
/// 8 bytes for data - 8 bytes is sufficient for a Double datatype, which is used for several
/// motor descriptors.
/// 10 bytes total.
#define UDPMSGLENGTH 10
#define UDPTIMEOUT 10
#define UDPTIMEOUTTICKS 50
#define MAXERROR 1.0 /// %
#define MINPRECISION 1.0 /// %
#define ACCEL 1
#define MAXACCEL 500000 // mm/s^2
///Number of steps between velocity adjustments.
#define ACCEL_STEP_INTERVAL 10
///Number of equal divisions used for acceleration processes.
#define ACCEL_DIV 4
///Number of OS ticks between acceleration steps.
#define ACCEL_STEP_TICKS 10
#define STEPZ 45.472
#define STEPX 45.472
#define STEPY 45.472
#define DEFAULTSPEED 60 //1kHz
#define DEFAULTSPEEDX 10
#define DEFAULTSPEEDY 10
#define DEFAULTSPEEDZ 10
#define DEFAULTJERKSPEEDX 500
#define DEFAULTJERKSPEEDY 500
#define DEFAULTJERKSPEEDZ 500
#define HOME_BACKOFF 250 ///Time in milliseconds the motor runs when reversing away from limit.
#define HOMEVX 60
#define HOMEVY 60
#define HOMEVZ 60
#define MINV 100
#define DEBOUNCE_TIME 10 //time in milliseconds for debouncing delay

//long double pi=acos(-1.0L)

void printList();
int length();
void initBuff();
void initMotorBlocks();

///unused - to remove?
struct listItem {
    struct listItem *prev;
    struct listItem *nxt;
    char itemCmd[50];
    float x;
    float y;
    float z;
};


/**This struct forms the links used in the doubly linked list instructions buffer*/
struct node
{
    char content[NXD_MQTT_MAX_MESSAGE_LENGTH];
    int data;
    int key;

    struct node *prev;
    struct node *next;
};

struct node * insertLink(char *content);
struct node * removeLink(struct node *link);

/**The motorController struct is stored in a block pool
there are no #define elements which describe the motor
instead, all constant and variable motor information is stored in the motorController block
this organizes all motor information in a single block of protected RAM, which is globally accessible
there is a separate motorController block for each motor

The PWM frequency of a motor is calculated as freq = 2 * (targetSpeed / (60 * stepSize)).
Dividing the target speed by the step size gets the frequency in pulses/min. Dividing by 60 brings
it to pulses/second. Doubling the frequency is then required because each pulse of the PWM signal
is used to trigger an interrupt which inverts the level of the STEP pin. This halves the actual signal
frequency and so it must be doubled to compensate.

Several variables of type "ioport_level_t" are used. This data type is provided by Renesas,
and is required when interacting with GPIO pins via their read and write functions.*/
struct motorController
{
    ///Flag used to tell if the motor block has been initialized. True = 1, False = XXX.
    int init;
    ///Flag used to indicate if the motor is in homing mode.
    ///While in homing mode, the motor handler will ignore the state of the limit pins.
    /// Because of this, during a homing routine, the G28 command handler is responsible
    /// for directing motor activity.
    int homing;
    ///Flag used to indicate whether the speed of the motor is to be set according
    /// to a velocity or frequency.
    /// 0 - Velocity (freq = vel/stepsize)
    /// 1 - Frequency (Hz)
    int freqSet;
    ///Flag used to indicate if the target position is supposed to be set according
    /// to steps, pos (mm), etc...
    int setPosSteps;
    ///Flag used to indicate if the motor is actively moving to a position.
    ///This is used by the STEP IRQ to determine if the motor must be stopped
    ///when the target position is reached.
    int moveToPos;
    ///Used to retain the current state of the DIR pin.
    ioport_level_t dir;
    ///Default direction is defined as the direction towards the limit.
    ioport_level_t defaultDir;
    /**Forward direction is defined as the positive direction relative to zero.
    Used for calculating position while the motor is running*/
    ioport_level_t fwdDir;
    ///The direction pin is needed for reference by all movement functions.
    int dirPin;
    ioport_level_t targetDir;

    long int origPosStepsAbs;

    ///Used to retain the original position, in motor steps, prior to setting target position.
    long int origPosSteps;

    long int posStepsAbs;

    ///Used to retain the position, in motor steps.
    long int posSteps;

    double origPosAbs;

    ///Used to retain the position, in mm.
    double origPos;

    double posAbs;
    ///Used to retain the position, in mm.
    double pos;
    double targetPosAbs;
    ///Used to retain the target position, in mm.
    double targetPos;
    long int targetPosStepsAbs;
    ///Used to retain the target position, in motor steps.
    long int targetPosSteps;

    ///Retains the offset, in steps, used for relative positioning movements.
    long int offsetSteps;

    ///Used to retain the motor step size, in mm/step.
    double stepSize;
    /**Used to retain the integer value which corresponds to the GPIO pin being used for STEP output.
    This integer value is found in the #define values provided by Renesas headers.*/
    int stepPin;
    ///Used to retain the state of the STEP pin. Currently unused.
    ioport_level_t stepState;
    ///Used to retain the homing speed, in mm/min.
    double homeSpeed;
    ///Used to retain the rapid speed, in mm/min.
    double rapidSpeed;
    ///Used to retain the current frequency of the motor, in Hz.
    /// This variable is set by the motor handler after making any
    /// adjustment to STEP timer frequency.
    int frequency;
    ///Used to retain the current speed of the motor, in mm/min.
    double speed;
    ///Used to retain the target speed of the motor, in mm/min.
    double targetSpeed;
    ///Used to retain the target jerk speed of the motor, in mm/min.
    double targetJerkSpeed;
    ///Used to store the set frequency. Used for debugging.
    int targetFreq;
    /**Flag used to enable motor acceleration.
     * When not enabled, the motor is run with the default speed.
     * Default is used for homing movements. Without knowing the position
     * of the limit switch, it is not possible to decelerate prior to hitting the switch.
     * */
    int accelEN;
    double accelStepSize;
    ///Used to retain the acceleration rate of the motor, in mm/min^2.
    double accel;
    ///Used to retain the target acceleration rate of the motor, in mm/min^2.
    double targetAccel;
    ///Used to retain the speed at the start of any acceleration or deceleration process.
    double startSpeed;
    int intervalSteps; ///What is this?
    /**Used to retain the integer value which corresponds to the pin being used for limit0 input.
    This integer value is found in the #define values provided by Renesas headers.*/
    int limit0Pin;
    ///Used to retain the state of the limit pin for relevant comparison logic.
    ioport_level_t limit0State;
    /**String variable which is available for retaining status information about the motor.
    Information held here is reported to the GUI when status updates are requested. Currently used.*/
    char status[50];
    ///Function pointer for reference to the Renesas function which starts the GPT timer.
} *motorBlockX, *motorBlockY, *motorBlockZ, *motorBlockA;

struct toolBlock
{
    ///Flag used to tell if the motor block has been initialized. True = 1, False = XXX.
    int init;
    ///Code that identifies the type of tool:
    /// 0 - Extruder
    /// 1 - Welder
    int toolType;

    double tempRead;
    double tempSet;
    //temp sensor pin
    //ADC channel

    struct motorController *motorBlock;
} *toolBlockA;

/**Retains information about the properties and state of the machine
 * At this point there is only one global variable
    other items of worth may be build plate dimensions, number of extruders, etc...*/
struct machineGlobals
{
    char globalsInit;
    char motorsInit;
    ///motorFreqSet is a flag which is set to indicate that a freqSet value of 1 has already been
    /// sent to the slave.
    char motorFreqSet;
    UINT receivingMsg;
    UINT calibRunning;
    UINT relativePositioningEN;
    char USBPlugIn;
    char USBBufferOpen;
    char USBINIReady;
    UINT linkedListNodeCount;
    char rebuildLinkedList;
    char getUpdate;
    ///This variable retains the target velocity for the toolhead.
    /// By default this is initialized to the default velocity defined
    /// in "Helix.h".
    double targetSpeed;
    FX_MEDIA * p_media;
    CHAR                        volume[32];
    FX_FILE my_file;
    UX_HOST_CLASS_STORAGE_MEDIA * p_ux_host_class_storage_media;
    ULONG USBFileIndex;
    ULONG local_bufferIndex;
    char *local_buffer;
    char *USBBufferB;
    char USBBufferHasData;

    char UDPBuffer[UDPMSGLENGTH];
    char UDPRxBuff[UDPMSGLENGTH];
    char UDPFlowControl;
    UINT UDPTimeout;
    NX_UDP_SOCKET g_udp_sck;
    ///UDPRX is a flag which indicates if data has been received via UDP. Active HIGH.
    char UDPRX;
    char echoWaitStart;
} *machineGlobalsBlock;

/**This struct retains information relevant to instructions after they are parsed.
 * After parsing, information such as floating point data related to character flags to axes ZXY
 * are retained for usage by different processes.
 */
struct instruction{
    char cmd[4];
    char cmdString[50];
    double a;
    double b;
    double c;
    double d;
    double x;
    double y;
    double z;
    double e;
    ///Used for Feed Rate and Frequency data
    double f;
    ///Used for Home Speed data
    double h;
    ///Used for Jerk Speed data
    double j;
    double speed;
//    ioport_level_t calDir;
};
void initGlobalsBlock();
struct instruction parseLine(struct node *input);
//void commandHandler(struct instruction data);
//void velocityHandler(struct motorController *motorBlock);
void setSpeed(struct motorController *motorBlock, int freqSet, double targetSpeed, int targetFreq);
double returnNumber(char * searchString, char searchChar);

//void motorHandler(struct motorController *motorBlock);
//void moveToPosition(struct motorController *motorBlock, int calRun);
//void runMotor(struct motorController *motorBlock, int calRun);
void stopMotor(struct motorController *motorBlock);
//void G01(struct motorController *motorBlock);
//void G28(struct motorController *motorBlock);
double percentError(double target, double source);
double UDPGetPosition(char axis);
char UDPGetStatus();
void processUDPRx(NX_PACKET *p_packet);
void UDPGetToolUpdate();
void initToolBlocks();
void UDPSend();
