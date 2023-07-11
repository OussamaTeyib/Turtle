#include <windows.h>                            

enum facility {
    INIT,
    SHOW,
    CREATE_CANVAS,
    WNDPROC,
    CLEANUP,
    POST_COMMAND,
    EXECUTE_COMMANDS,	
    FORWARD,
    LEFT,
    RIGHT, 
    SETPOS,
    CIRCLE,
    RECTANGLE,
    COLOR,
};

enum err_code {
    TURTLE_NOT_INITIALIZED = 1,
    TURTLE_CMD_QUEUE_NOT_INITIALIZED = 2,
    TURTLE_CMD_PARAMS_NOT_INITIALIZED = 4,
    WINDOW_CLASS_NOT_REGISTRED = 8,
    WINDOW_NOT_CREATED = 16,
};

enum severity {
    SEVERITY_SUCCESS,
    SEVERITY_FAILURE,
};