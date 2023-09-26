//#include "UDP.h"
#include <TitanParser.h>
//#include <Ganymede.h>
#include "hal_data.h"
//#include "r_timer_api.h"
#define DEBUGGERPRIMARY 1
#define DEBUGGER 0
#define DEBUG 0
#define SECONDARYIP IP_ADDRESS(192,168,10,182)
#define IPADDSTRING "192.168.10.181"
#define WIFI_PACKET_SIZE 53
#define NXD_MQTT_MAX_MESSAGE_LENGTH 50
///Number of bytes in each UDP message. UDP packet:
/// control(1)|axis(1)|data(8)
/// 1 byte for control - Indicates the operation, such as 'd' for set direction.
/// 1 byte for axis - Indicates the target axis, such as 'x'
/// 8 bytes for data - 8 bytes is sufficient for a Double datatype, which is used for several
/// motor descriptors.
/// 10 bytes total.
#define UDPMSGLENGTH 50
#define UDPTIMEOUT 10
#define UDPTIMEOUTTICKS 50
#define MAXERROR 1.0 /// %
#define MINPRECISION 1.0 /// %
#define ACCEL 1
///Number of steps between velocity adjustments.
#define ACCEL_STEP_INTERVAL 10
///Number of equal divisions used for acceleration processes.
#define ACCEL_DIV 4
///Number of OS ticks between acceleration steps.
#define ACCEL_STEP_TICKS 10
#define STEPX 45.472
#define STEPY 45.472
#define STEPZ 15
#define STEPA 45.472
#define STEPC 45.472
#define STEPT 45.472
//#define STEPZ 22.5
//#define STEPX 22.5
//#define STEPY 22.5
#define DEFAULTSPEED 1200
#define DEFAULTJERKSPEEDX 500
#define DEFAULTJERKSPEEDY 500
#define DEFAULTJERKSPEEDZ 500
#define HOME_BACKOFF 250 ///Time in milliseconds the motor runs when reversing away from limit.
#define MINV 100
#define MINPRECISION 1.0 /// %
#define MAXACCEL 2000 // mm/s^2
#define MAXSPEED 500.0 ///Was 110.0
#define ACCELPERTICK 120
#define HOME_BACKOFF 250 ///Time in milliseconds the motor runs when reversing away from limit.
#define HOME_BACKOFF_DISTANCE .3 ///Distance in millimeters to back off of limit.
#define SOFTLIMITFORWARD 700
#define SOFTLIMITREVERSE 0
#define HOMEVX 1200
#define HOMEVY 1200
#define HOMEVZ 1200
#define HOMEVA 1200
#define HOMEVC 1200
#define HOMEVT 1200
#define DEBOUNCE_TIME 75 //time in milliseconds for debouncing delay
#define ENCODERPULSESPERREVOLUTION 4000
#define STEPSPERREVOLUTION 1600

#define     UX_STORAGE_BUFFER_SIZE      (64*1024)
#define DEBUGTEMPERATURE 0
#define LIMITACTIVESTATE IOPORT_LEVEL_HIGH
#define LIMITINACTIVESTATE IOPORT_LEVEL_LOW

//long double pi=acos(-1.0L)

void printList();
int length();
void initBuff();
void initMotorBlocks();

///unused - to remove?
struct listItem
{
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

struct node* insertLink(char *content);
struct node* removeLink(struct node *link);

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
    char controlCode;
    ///Flag used to indicate if the motor is in homing mode.
    ///While in homing mode, the motor handler will ignore the state of the limit pins.
    /// Because of this, during a homing routine, the G28 command handler is responsible
    /// for directing motor activity.
    int homing;
    ///The referenced flag indicates whether the motor has been referenced.
    char referenced;
    char hasLimits;
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

    double origPosStepsAbs;

    ///Used to retain the original position, in motor steps, prior to setting target position.
    double origPosSteps;

    double posStepsAbs;

    ///Used to retain the position, in motor steps.
    double posSteps;

    double origPosAbs;

    ///Used to retain the position, in mm.
    double origPos;

    double posAbs;
    ///Used to retain the position, in mm.
    double pos;
    double targetPosAbs;
    ///Used to retain the target position, in mm.
    double targetPos;
    double targetPosStepsAbs;
    ///Used to retain the target position, in motor steps.
    double targetPosSteps;

    ///Retains the offset, in steps, used for relative positioning movements.
    long int offsetSteps;

    ///Used to retain the motor step size, in steps per mm.
    char encoderActive;
    double stepsPerMM;
    double stepSize;
    double encoderMMPerPulse;
    double stepsPerEncoderPulse;
    int encoderAPin;
    int encoderBPin;
    ioport_level_t encoderAState;
    ioport_level_t encoderBState;
    char encoderCWFWD;
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
    ssp_err_t (*start)(timer_ctrl_t *const p_ctrl);
    ///Function pointer for reference to the Renesas function which sets the GPT timer period.
    ssp_err_t (*periodSet)(timer_ctrl_t *const p_ctrl, timer_size_t const period, timer_unit_t const unit);
    ///Function pointer for reference to the Renesas function which sets the GPT timer duty cycle.
    ssp_err_t (*dutyCycleSet)(timer_ctrl_t *const p_ctrl, timer_size_t const duty_cycle,
            timer_pwm_unit_t const duty_cycle_unit, uint8_t const pin);
    ///Function pointer for reference to the Renesas function which stops the GPT timer.
    ssp_err_t (*stop)(timer_ctrl_t *const p_ctrl);
    /**Each GPT timer has an instance. The "timer_instance_t" datatype stores the GPT timer information
     required by Renesas functions when starting, stopping, or otherwise adjusting the activity
     of a GPT timer.*/
    timer_instance_t g_timer_gpt_x;
};

extern struct motorController *motorBlockX, *motorBlockY, *motorBlockZ, *motorBlockT, *motorBlockA, *motorBlockB,
        *motorBlockC, *motorBlockD;

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
};

extern struct toolBlock *toolBlockA;

/**Retains information about the properties and state of the machine
 * At this point there is only one global variable
 other items of worth may be build plate dimensions, number of extruders, etc...*/
struct machineGlobals
{
    char globalsInit;
    char motorsInit;
    char iniInit;
    char printJob;
    char chainedMovement;
    ///Array of pointers to motor controller blocks, in series XYZABC.
    struct motorController *controllerBlocks[4];
    ///How many controller blocks there are.
    int numOfControllers;
    ///Current index number for controller list.
    int controllerIndex;
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
    FX_MEDIA *p_media;
    CHAR volume[32];
    FX_FILE iniFile;
//    UX_HOST_CLASS_STORAGE_MEDIA *p_ux_host_class_storage_media;
    ULONG USBFileIndex;
    ULONG gCodeFileIndex;
    ULONG local_bufferIndex;
    char *local_buffer;
    FX_FILE gcodeFile;
    char *USBBufferB;
    char USBBufferHasData;

    ///Stores the next available Secondary IP address.
    ULONG nextIP;

    char UDPBuffer[UDPMSGLENGTH];
    char UDPRxBuff[UDPMSGLENGTH];

    ///Used for storing the IP of the sender.
    ULONG UDPRxIP;

    char UDPFlowControl;
    UINT UDPTimeout;
//    NX_UDP_SOCKET g_udp_sck;
    ///UDPRX is a flag which indicates if data has been received via UDP. Active HIGH.
    char UDPRX;
    char echoWaitStart;
    double targetPosX;
    double targetPosY;
    double targetPosZ;
    double targetPosT;
    double targetSpeed;
};

extern struct machineGlobals *machineGlobalsBlock;

/**This struct retains information relevant to instructions after they are parsed.
 * After parsing, information such as floating point data related to character flags to axes ZXY
 * are retained for usage by different processes.
 */
struct instruction
{
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
    double t;
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
double returnNumber(char *searchString, char searchChar);

//void motorHandler(struct motorController *motorBlock);
//void moveToPosition(struct motorController *motorBlock, int calRun);
//void runMotor(struct motorController *motorBlock, int calRun);
void stopMotor(struct motorController *motorBlock);
//void G01(struct motorController *motorBlock);
//void G28(struct motorController *motorBlock);
double percentError(double target, double source);
double UDPGetPosition(struct motorController *motorBlock);
char UDPGetStatus();
//void processUDPRx(NX_PACKET *p_packet);
void UDPGetToolUpdate();
void initToolBlocks();
void UDPSend(ULONG ip_address);
void setupMode();
void UDPHomeMotor(struct motorController *motorBlock);
void rxFile(long long fileSize);
void serialHandler(char *uartRx);
void loadINI();
void saveINI();
void rebuildLinkedListFromSD();
void printJob();
void openGCode();
void autoBuildPlateLevel();
void calCmdHandler(struct instruction *data);
void calRoutine(struct instruction *data, struct motorController *motorBlock, long int targetPosSteps, int targetFreq);
char isInRange(char in);
void UDPZeroTool();
