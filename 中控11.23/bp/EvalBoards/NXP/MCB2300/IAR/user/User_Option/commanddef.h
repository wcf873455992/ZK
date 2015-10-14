#ifndef SA_COMMON_SOCK_COMMANDDEF__H
#define SA_COMMON_SOCK_COMMANDDEF__H
/*
  brief  : command define
  author : wuminghao
  date   : 2014-12-30
*/


//Camera Control action
enum EMCmaerAction
{
    EMCCACTION = 0,
    EMCCACTIONUP,
    EMCCACTIONDOWN,
    EMCCACTIONLEFT,
    EMCCACTIONRIGHT,
    EMCCACTIONLEFTUP,
    EMCCACTIONLEFTDOWN,
    EMCCACTIONRIGHTUP,
    EMCCACTIONRIGHTDOWN,
    EMCCACTIONSTOP,
    EMCCACTIONZOOMIN,
    EMCCACTIONZOOMOUT,
    EMCCACTIONZOOMSTOP,
    EMCCACTIONFOCUSFAR,
    EMCCACTIONFOCUSNEAR,
    EMCCACTIONFOCUSSTOP,
    EMCCACTIONSETPRESET,
    EMCCACTIONCALLPRESET,
    EMCCACTIONSETPTZ,
    EMCCACTIONGETPTZ,
    EMCCACTIONGETPT,
    EMCCACTIONGETZ,
    EMCCACTIONSETSPEED,
};

// Remote Controller action
enum EMRCAction
{
    EMRCACTION = 0,
    EMRCACTIONUP,
    EMRCACTIONDOWN,
    EMRCACTIONLEFT,
    EMRCACTIONRIGHT,
    EMRCACTIONOK,
    EMRCACTIONPOWERON,//6
    EMRCACTIONPOWEROFF,//7
    EMRCACTIONVOLUMEADD,
    EMRCACTIONVOLUMEDEC,
    EMRCACTIONMUTE,
    EMRCACTIONRETURN,
    EMRCACTIONMENU,
    EMRCACTIONHOME,
    EMRCACTIONRECORD,
    EMRCACTIONUNDEFINED2,// undefined1-3
    EMRCACTIONUNDEFINED3,
    EMRCACTIONRECORDSTART,//11
    EMRCACTIONRECORDPAUSE,//12
    EMRCACTIONRECORDSTOP,//13
    EMRCACTIONDEVPC,//14
    EMRCACTIONDEVNPC,//15
    EMRCACTIONDEVSHOW,//16

};

enum EMParticipator
{
    EMPARTICIPATOR = 0,
    EMPARTICIPATORCONTROLBOARD,
    EMPARTICIPATORDEVCONTROLLER,
    EMPARTICIPATORLIVEMEDIAMASTER,            // 8168-1
    EMPARTICIPATORLIVEMEDIASLAVE,             // 8168-2
    EMPARTICIPATORMANUALRECORDER,
    EMPARTICIPATORTHIRDPARTY,
};

enum EMScene
{
    EMSCENE = 0,
    EMSCENET,
    EMSCENES,
    EMSCENETS,
    EMSCENEV,
    EMSCENET2,
    EMSCENES2,
    EMSCENET3,
    EMSCENES3,
    EMSCENERROR = 10,                         // This divide line is for track module, these scenes after this line is invalid.
    EMSCENEVT,
    EMSCENEVTS,
    EMSCENECOUNT,
};

enum EMSystem
{
    EMSYSTEM = 0,
    EMSYSTEMHALT,
};

enum EMAck
{
    EMACK = 0xff,
};

enum EMCommand
{
    EMCOMMAND = 0,
    EMCOMMANDHEARTBEAT,
    EMCOMMANDREMOTECONTROLLER,
    EMCOMMANDCAMERACONTROL,
    EMCOMMANDTAKEOVER,
    EMCOMMANDSCENE,
    EMCOMMANDSYSTEM,
    EMCOMMANDMESSAGE,
    EMCOMMANDRECORDBASEPARAM,               // unicode parameter
    EMCOMMANDRECORDUTF8PARAM,               // utf8 parameter
    EMCOMMANDCOM1 = 20,
    EMCOMMANDCOM2,
    EMCOMMANDCOM3,
    EMCOMMANDCOM4,
    EMCOMMANDSETBAUDRATE,    
    EMCOMMANDFILEREQ = 70,
    EMCOMMANDFILESEND,
    EMCOMMANDRESET,
};

enum EMTakeOverStatus
{
    EMTakeOver = 0,
    EMTakeOverBegin,
    EMTakeOverEnd,
};
enum EMMessage
{
    EMMESSAGEINFOR = 0,                     // common message dividing line
    EMMESSAGEVGASTATECHANGE,                // vga state change.
    EMMESSAGERECMODEMICRO,                  // record mode: micro
    EMMESSAGERECMODEMOVIE,                  // record mode: movie
    EMMESSAGERECMODERESOURCE,               // record mode: resource
    EMMESSAGERECMODEINTERACTION,            // record mode: interaction
    EMMESSAGESTATUSOK,                      // current status is ok
    EMMESSAGEEXCEPTIONERROR,                // current exception error

    EMMESSAGEERROR = 80,                    // error message dividing line
    EMMESSAGERECORDFAILED,                  // record failed.
    EMMESSAGELIVERECORDNOTSUPPORT,          // live record not support.
    EMMESSAGEOPENCAMERAFAIL,                // open camera failed, please check the camera setting and power.
    EMMESSAGEVGARESOLUTIONNOTSUPPORT,       // current vga resolution is too high, not support.

    EMMESSAGEWARNING = 160,                 // warning message dividing line
    EMMESSAGEDISKFULL,                      // the disk is full,to delete old media file.
    EMMESSAGEPRESSFAST,                     // user press too fast.
};

// this flag is valid when EMCOMMANDRECORD
enum EMFlagRecordParam
{
    EMFLAGRECORDPARAM = 0,
    EMFLAGRECORDPARAMCOURSENAME,
    EMFLAGRECORDPARAMTITLENAME,
    EMFLAGRECORDPARAMTEACHERNAME,
    EMFLAGRECORDPARAMRTMPADDRESS,
};

enum EMRecordCtrl
{
    EMRECORDCTRLNULL = 0,
    EMRECORDCTRLSTART,
    EMRECORDCTRLPAUSE,
    EMRECORDCTRLSTOP,
    EMRECORDCTRLHEAD,
    EMRECORDCTRLTAIL,
    EMRECORDCTRLPREVIEW,
    EMRECORDCTRLREPLAY,
    EMRECORDCTRLCOURSEOVER,
    EMRECORDCTRLSWITCHSCENE,
    EMRECORDCTRLCOUNT,
};

//球机名称
enum CamName
{
    EMCAMNAME = 0,
    EMCAMTEACHERONE = 1,
    EMCAMSTUDENTONE,
    EMCAMTEACHERTWO,
    EMCAMSTUDENTTWO,
    EMCAMTEACHERTHREE,
    EMCAMSTUDENTTHREE,
};

// 16 bytes
typedef struct _tagCommand
{
    unsigned char sender;
    unsigned char receiver;
    unsigned char command;
    unsigned char flag;
    unsigned int  extended[3];
} tagCommand;

// 256 bytes
typedef struct _tagMtgCommand
{
    unsigned char sender;
    unsigned char receiver;
    unsigned char command;
    unsigned char flag;
    unsigned char extended[252];
} tagMtgCommand;

typedef struct _tagPTZ
{
    int     p;
    int     t;
    int     z;
    // CamName num; //which cam
} tagPTZ;

#endif //SA_COMMON_SOCK_COMMANDDEF__H