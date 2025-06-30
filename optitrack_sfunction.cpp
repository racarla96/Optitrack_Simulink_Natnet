#define S_FUNCTION_NAME  optitrack_sfunction
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

// Incluir headers de NatNet
#include "NatNetTypes.h"
#include "NatNetClient.h"

// Estructura para almacenar datos del rigid body
typedef struct {
    float x, y, z;           // Posición
    float qx, qy, qz, qw;    // Cuaternión
    bool bTrackingValid;     // Estado de tracking
} RigidBodyData;

// Variables globales para el cliente NatNet
static NatNetClient* g_pClient = NULL;
static RigidBodyData g_rigidBodyData = {0};
static bool g_bConnected = false;
static int g_rigidBodyID = 1; // ID del rigid body a trackear

// Callback para recibir datos de frames
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData)
{
    if (!data) return;
    
    // Buscar el rigid body con el ID especificado
    for (int i = 0; i < data->nRigidBodies; i++)
    {
        sRigidBodyData* pRB = &data->RigidBodies[i];
        // printf("[OptiTrack S-Function] %i\n", pRB->ID);

        if (pRB->ID == g_rigidBodyID)
        {
            g_rigidBodyData.x = pRB->x;
            g_rigidBodyData.y = pRB->y;
            g_rigidBodyData.z = pRB->z;
            g_rigidBodyData.qx = pRB->qx;
            g_rigidBodyData.qy = pRB->qy;
            g_rigidBodyData.qz = pRB->qz;
            g_rigidBodyData.qw = pRB->qw;
            g_rigidBodyData.bTrackingValid = (pRB->params & 0x01) != 0;
            break;
        }
    }
}

/*====================*
 * S-function methods *
 *====================*/

static void mdlInitializeSizes(SimStruct *S)
{
    // Parámetros: [ServerIP, ClientIP, RigidBodyID]
    ssSetNumSFcnParams(S, 3);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    // Sin entradas
    if (!ssSetNumInputPorts(S, 0)) return;

    // 3 salidas: Posición (3x1), Cuaternión (4x1), Estado conexión (1x1)
    if (!ssSetNumOutputPorts(S, 3)) return;
    
    // Salida 1: Posición [x; y; z]
    ssSetOutputPortWidth(S, 0, 3);
    ssSetOutputPortDataType(S, 0, SS_DOUBLE);
    
    // Salida 2: Cuaternión [qw; qx; qy; qz]
    ssSetOutputPortWidth(S, 1, 4);
    ssSetOutputPortDataType(S, 1, SS_DOUBLE);
    
    // Salida 3: Estado de conexión
    ssSetOutputPortWidth(S, 2, 1);
    ssSetOutputPortDataType(S, 2, SS_DOUBLE);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

#define MDL_START
#if defined(MDL_START)
static void mdlStart(SimStruct *S)
{
    // Obtener parámetros
    const mxArray* serverIPParam = ssGetSFcnParam(S, 0);
    const mxArray* clientIPParam = ssGetSFcnParam(S, 1);
    const mxArray* rigidBodyIDParam = ssGetSFcnParam(S, 2);
    
    // Convertir parámetros
    char serverIP[64] = "127.0.0.1";
    char clientIP[64] = "127.0.0.1";
    
    if (mxIsChar(serverIPParam)) {
        mxGetString(serverIPParam, serverIP, sizeof(serverIP));
    }
    if (mxIsChar(clientIPParam)) {
        mxGetString(clientIPParam, clientIP, sizeof(clientIP));
    }
    if (mxIsDouble(rigidBodyIDParam)) {
        g_rigidBodyID = (int)mxGetScalar(rigidBodyIDParam);
    }
    
    // Crear cliente NatNet (constructor por defecto)
    g_pClient = new NatNetClient();
    
    // Configurar parámetros de conexión
    sNatNetClientConnectParams connectParams;
    connectParams.connectionType = ConnectionType_Unicast;
    connectParams.serverAddress = serverIP;
    connectParams.localAddress = clientIP;
    connectParams.serverCommandPort = 1510;
    connectParams.serverDataPort = 1511;
    connectParams.multicastAddress = "239.255.42.99";
    
    // Conectar
    int result = g_pClient->Connect(connectParams);
    if (result == ErrorCode_OK) {
        g_bConnected = true;
        
        // Configurar callback para recibir datos
        g_pClient->SetFrameReceivedCallback(DataHandler, g_pClient);
        
        // printf("[OptiTrack S-Function] Conectado al servidor %s\n", serverIP);
        // printf("[OptiTrack S-Function] Trackeando Rigid Body ID: %d\n", g_rigidBodyID);
    } else {
        g_bConnected = false;
        // printf("[OptiTrack S-Function] Error al conectar: %d\n", result);
        ssSetErrorStatus(S, "No se pudo conectar al servidor OptiTrack");
    }
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    // Obtener punteros a las salidas
    real_T *position = (real_T*)ssGetOutputPortSignal(S, 0);
    real_T *quaternion = (real_T*)ssGetOutputPortSignal(S, 1);
    real_T *connected = (real_T*)ssGetOutputPortSignal(S, 2);
    
    if (g_bConnected && g_pClient) {
        // Copiar datos de posición
        position[0] = (real_T)g_rigidBodyData.x;
        position[1] = (real_T)g_rigidBodyData.y;
        position[2] = (real_T)g_rigidBodyData.z;
        
        // Copiar datos de cuaternión [qw, qx, qy, qz]
        quaternion[0] = (real_T)g_rigidBodyData.qw;
        quaternion[1] = (real_T)g_rigidBodyData.qx;
        quaternion[2] = (real_T)g_rigidBodyData.qy;
        quaternion[3] = (real_T)g_rigidBodyData.qz;
        
        // Estado de conexión
        connected[0] = g_rigidBodyData.bTrackingValid ? 1.0 : 0.5; // 1.0 = tracking OK, 0.5 = conectado pero sin tracking
    } else {
        // Sin conexión - valores por defecto
        position[0] = position[1] = position[2] = 0.0;
        quaternion[0] = 1.0; // qw
        quaternion[1] = quaternion[2] = quaternion[3] = 0.0; // qx, qy, qz
        connected[0] = 0.0; // Desconectado
    }
}

static void mdlTerminate(SimStruct *S)
{
    if (g_pClient) {
        if (g_bConnected) {
            g_pClient->Disconnect();
            // printf("[OptiTrack S-Function] Desconectado del servidor\n");
        }
        delete g_pClient;
        g_pClient = NULL;
    }
    g_bConnected = false;
}

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif