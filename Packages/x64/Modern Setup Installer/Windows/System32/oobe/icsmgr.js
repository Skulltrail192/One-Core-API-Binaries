

var ICS_CONN_STARTING = 1;
var ICS_CONN_CONNECTING = 2;
var ICS_CONN_CONNECTED = 3;
var ICS_CONN_DISCONNECTING = 4;
var ICS_CONN_DISCONNECTED = 5;
var ICS_CONN_STOPPED = 6;
var ICS_CONN_PERMANENT = 7;
var ICS_CONN_UNK_DIAL_STATE = 8;
var ICS_CONN_CALLWAITING = 9;
var ICS_CONN_HN_UNAVAILABLE = 1000;
var ICS_CONN_TIMEOUT = 5000;

var oldState = ICS_CONN_DISCONNECTED;
var currentState = ICS_CONN_DISCONNECTED;


var ICS_CONN_TYPE_REG = 0;
var ICS_CONN_TYPE_ISP = 1;
var ICS_CONN_TYPE_REF = 2;
var ICS_CONN_TYPE_MIG = 3;

var ICS_Error_Id = 0;
var ICS_ERR_HN_UNAVAILABLE = ICS_Error_Id++;
var ICS_ERR_MODEM_DISCONNECTED = ICS_Error_Id++;
var ICS_ERR_MODEM_FAILED_CONNECT = ICS_Error_Id++;
var ICS_ERR_STOPPED = ICS_Error_Id++;
var ICS_ERR_PERM_DISCONNECTED = ICS_Error_Id++;
var ICS_ERR_TIMEOUT = ICS_Error_Id++;
var LAN_ERR_HW_UNAVAILABLE1 = ICS_Error_Id++;
var LAN_ERR_HW_UNAVAILABLE2 = ICS_Error_Id++;








var CONNECTIONTYPE_INVALID            = 0x00000000;



var CONNECTIONTYPE_NONE               = 0x00000001;



var CONNECTIONTYPE_MODEM              = 0x00000002;



var CONNECTIONTYPE_LAN_ICS            = 0x00000004;



var CONNECTIONTYPE_LAN_BROADBAND      = 0x00000008;




var CONNECTIONTYPE_LAN_INDETERMINATE  = 0x00000010;



var CONNECTIONTYPE_OTHER              = 0x00000020;


var CONNECTIONTYPE_LAN                = (CONNECTIONTYPE_LAN_ICS |
                                         CONNECTIONTYPE_LAN_BROADBAND |
                                         CONNECTIONTYPE_LAN_INDETERMINATE);



var INTERNET_UNKNOWN = -1;
var INTERNET_NO      = 0;
var INTERNET_YES     = 1;





var CONFIG_LAN_UNKNOWN = -1;
var CONFIG_LAN_NO      = 0;
var CONFIG_LAN_YES     = 1;

var g_ConnectionCapabilities      = CONNECTIONTYPE_INVALID;
var g_PreferredConnection         = null;  
var g_Internet                    = INTERNET_UNKNOWN;
var g_ModemTypes                  = null;
var g_CanConfigLan                = CONFIG_LAN_UNKNOWN;
var g_PublicLanCount              = 0;

var L_ERR_HN_UNAVAILABLE_Text = "Your computer is no longer connected to the Internet through your network. <br><br>To restore the connection, make sure that the network cable is securely plugged into your computer as well as the network.";


var L_ERRSTOPPED_Text = "Internet Connection Sharing is presently not available on your network. Click <b>Retry</b> to try connecting again.";
var L_ERR_PERM_DISCONNECTED_Text = "Internet Connection Sharing has been disconnected. If you're using a high-speed adapter to connect to the Internet (e.g. Broadband), please ensure that the adapter has been correctly configured and that the cables are securely attached.<br><br>Click <b>Retry</b> to try connecting again.";
var L_ERRTIMEOUT_Text = "There is a problem connecting to the Internet. Either your connection has timed out or this computer is not configured correctly to use the Shared Internet Connection. <br><br>Click <b>Retry</b> to try connecting again.";
var L_ERR_LAN_UNAVAILABLE1_Text = "Your computer is not connected to the Internet through your network. <br><br>Please check your network settings and confirm that you are able to connect to the Internet, then try again.";
var L_ERR_LAN_UNAVAILABLE2_Text = "Your computer is not connected to the Internet through your network. <br><br>Please select one of the following methods:";

<!--REQUIRED FUNCTION NAME-->
function OnIcsConnectionStatus(iStatus)
{
    oldState = currentState;
    currentState = iStatus;

    switch (g_CurrentCKPT)
    {
    case CKPT_REGISTER1:
    case CKPT_REGISTER3:
    case CKPT_ISPSIGNUP:
    case CKPT_REFDIAL:
    case CKPT_MIGDIAL:
        break;
    default:
        return;
    }

    switch(iStatus)
    {
        case ICS_CONN_TIMEOUT:
                ICS_Error_Id = ICS_ERR_TIMEOUT;
                Navigate("icserror\\icsdc.htm");
                break;
        case ICS_CONN_PERMANENT:
                break;
        case ICS_CONN_STOPPED:
                switch (oldState)
                {
                    case ICS_CONN_PERMANENT:
                         ICS_Error_Id = ICS_ERR_STOPPED;
                         Navigate("icserror\\icsdc.htm");
                         break;
                    default:
                         break;
                }
                break;
        case ICS_CONN_HN_UNAVAILABLE:
                ICS_Error_Id = ICS_ERR_HN_UNAVAILABLE;
                Navigate("icserror\\icsdc.htm");
                break;
        default:
                break;
    }
}





function IcsDcError_LoadMe()
{
    InitFrameRef();
    InitNewButtons();
    g.btnNext.onclick = IcsErrorNextBtnHandler;
    g.btnSkip.onclick = IcsErrorSkipBtnHandler;

    switch (ICS_Error_Id)
    {

    case ICS_ERR_HN_UNAVAILABLE:
        g.ErrorPara1.innerHTML = L_ERR_HN_UNAVAILABLE_Text;
        break;
    case ICS_ERR_STOPPED:
        g.ErrorPara1.innerHTML = L_ERRSTOPPED_Text;
        break;
    case ICS_ERR_PERM_DISCONNECTED:
        g.ErrorPara1.innerHTML = L_ERR_PERM_DISCONNECTED_Text;
        break;
    case ICS_ERR_TIMEOUT:
        g.ErrorPara1.innerHTML = L_ERRTIMEOUT_Text;
        break;
    case LAN_ERR_HW_UNAVAILABLE1:
    case LAN_ERR_HW_UNAVAILABLE2:
        g.btnBack.style.visibility = "visible";
        g.BackBtnLocalText.style.visibility = "visible";
        g.btnBack.onclick = IcsErrorBackBtnHandler;
        if (ICS_Error_Id == LAN_ERR_HW_UNAVAILABLE1)
        {
            g.ErrorPara1.innerHTML = L_ERR_LAN_UNAVAILABLE1_Text;
        }
        else
        {
            g.ErrorPara1.innerHTML = L_ERR_LAN_UNAVAILABLE2_Text;
            g.ErrorPara2.style.display = "inline";
        }
        break;
    }
    g.btnNext.focus();
}

function IcsErrorBackBtnHandler()
{
    GoNavigate(g_CurrentCKPT);
}

function IcsErrorNextBtnHandler()
{
    DisableAllButtons();
    ResetConnectedToInternetEx();
    switch (g_CurrentCKPT)
    {
    case CKPT_REGISTER1:
    case CKPT_REGISTER3:
        if (ICS_Error_Id == LAN_ERR_HW_UNAVAILABLE1)
        {
            LanActReg();
        }
        else if (ICS_Error_Id == LAN_ERR_HW_UNAVAILABLE2)
        {
            if (g.rb_conn_modem.checked)
            {
                GoNavigate(CKPT_REGDIAL);
            }
            else
            {
                LanActReg();
            }
        }
        else
        {
            IcsConnect(ICS_CONN_TYPE_REG);
        }
        break;
    case CKPT_ISPSIGNUP:
        IcsConnect(ICS_CONN_TYPE_ISP);
        break;
    case CKPT_REFDIAL:
        IcsConnect(ICS_CONN_TYPE_REF);
        break;
    case CKPT_MIGDIAL:
        IcsConnect(ICS_CONN_TYPE_MIG);
        break;
    default:
        break;
    }
}

function IcsErrorSkipBtnHandler()
{
    switch (g_CurrentCKPT)
    {
    case CKPT_REGISTER1:
    case CKPT_REGISTER3:
        
        g_CurrentCKPT = CKPT_ACT_MSG;
        break;
    default:
        break;
    }
    GoCancel();
}

function LanActReg()
{
    if (g_Internet == INTERNET_UNKNOWN)
    {
        ConnectedToInternetEx(true, "LanActReg2();");
        g.document.body.style.cursor = "wait";
    }
    else
    {
        LanActReg2();
    }
}

function LanActReg2()
{
    if (!ConnectedToInternetEx(false))
    {
        if (HasModem())
        {
            ICS_Error_Id = LAN_ERR_HW_UNAVAILABLE2;
        }
        else
        {
            ICS_Error_Id = LAN_ERR_HW_UNAVAILABLE1;
        }
        Navigate("icserror\\icsdc.htm");
    }
    else
    {
        CommonActReg();
    }
}

function IcsConnect(iConnectionType)
{
    
    
    
    if (g_Internet == INTERNET_UNKNOWN)
    {
        ConnectedToInternetEx(true, "IcsConnect2("+iConnectionType+")");
    }
    else
    {
        IcsConnect2(iConnectionType);
    }
}

function IcsConnect2(iConnectionType)
{
    if (!ConnectedToInternetEx(false))
    {
         OnIcsConnectionStatus ( ICS_CONN_HN_UNAVAILABLE );
         return;
    }

    switch (iConnectionType)
    {
    case ICS_CONN_TYPE_REG:
        CommonActReg();
        break;
    case ICS_CONN_TYPE_ISP:
        
        window.external.Connect(CONNECTED_ISP_SIGNUP, "msobe.isp");
        break;
    case ICS_CONN_TYPE_REF:
        
        window.external.Connect(CONNECTED_REFFERAL, "migrate.isp");
        break;
    case ICS_CONN_TYPE_MIG:
        
        
        window.external.Connect(CONNECTED_ISP_MIGRATE, gselectedISPIndx);
        break;
    default:
        break;
    }
}

function CommonActReg()
{
    
    
    window.external.Connect(CONNECTED_REGISTRATION, "reg.isp");
    if (g_DoActivation)
    {
        window.external.AsyncActivate(g_PostToMS);
    }
    else 
    {
        if (g_PostToMS)
        {
            window.external.PostRegData(POST_TO_MS);
            StatusObj.set_Status(MS_REGISTRATION, true);
        }
        ActivationComplete();
    }
}

function UseIcs()
{

    fResult = ((CONNECTIONTYPE_LAN_ICS == GetPreferredConnection())
                && HasIcs()) ? true : false;
    trace("UseIcs: " + fResult);
    return fResult;
}   

function HasIcs()
{
    fResult = (CONNECTIONTYPE_LAN_ICS & GetConnectionCapabilities()) ? true : false;
    trace("HasIcs: " + fResult);
    return fResult;
} 

function UseBroadband()
{
    fResult = (CONNECTIONTYPE_LAN_BROADBAND == GetPreferredConnection() && HasBroadband()) ? true : false;
    trace("UseBroadband: " + fResult);
    return fResult;
}   

function HasBroadband()
{
    fResult = (CONNECTIONTYPE_LAN_BROADBAND & GetConnectionCapabilities()) ? true : false;
    trace("HasBroadband: " + fResult);
    return fResult;
} 

function HasIndeterminateLan()
{
    fResult = (CONNECTIONTYPE_LAN_INDETERMINATE & GetConnectionCapabilities()) ? true : false;
    trace("HasIndeterminateLan: " + fResult);
    return fResult;
} 



function UseModem()
{
    fResult = (CONNECTIONTYPE_MODEM == GetPreferredConnection() && HasModem()) ? true : false;
    trace("UseModem: " + fResult);
    return fResult;
}   


function HasModem()
{
    
    
    
    fResult = (CONNECTIONTYPE_MODEM & GetConnectionCapabilities()) ? true : false;
    trace("HasModem: " + fResult);
    return fResult;

} 

function HasAnalogModem()
{
    GetConnectionCapabilities();
    fResult = (g_ModemTypes == ERR_COMM_NO_ERROR);
    trace("HasAnalogModem: " + fResult);
    return fResult;
} 

function UseAnalogModem()
{
    fResult = (CONNECTIONTYPE_MODEM == GetPreferredConnection() && HasAnalogModem()) ? true : false;
    trace("UseAnalogModem: " + fResult);
    return fResult;
} 

function UseNetwork()
{
    return (UseIcs() || UseBroadband() || HasIndeterminateLan());
} 

function HasNetwork()
{
    var fResult = (GetConnectionCapabilities() & CONNECTIONTYPE_LAN) ? true : false;
    trace("HasNetwork: " + fResult);
    return fResult;
} 

function HasConnection()
{
    fResult = (HasNetwork() || HasModem());
    trace("HasConnection: " + fResult);
    return fResult;
} 

function SetPreferredConnection(ctPreferred)
{
    
    
    
    if (window.external.SetPreferredConnection(ctPreferred))
    {
        g_PreferredConnection = ctPreferred;
    }
    else if ((ctPreferred == CONNECTIONTYPE_MODEM) && HasModem())
    {
        g_PreferredConnection = CONNECTIONTYPE_MODEM;
    }

    
    ApiObj.set_RegValue(HKEY_LOCAL_MACHINE, OOBE_MAIN_REG_KEY + "\\TEMP", PREFCONN_REG_VALUE, g_PreferredConnection);

}   

function GetPreferredConnection()
{
    if (CONNECTIONTYPE_INVALID == g_PreferredConnection
        )
    {
        trace("Calling window.external.GetPreferredConnection");
        g_PreferredConnection = window.external.GetPreferredConnection();

        
        
        
        if (CONNECTIONTYPE_NONE == g_PreferredConnection
            && HasModem() )
        {
            g_PreferredConnection = CONNECTIONTYPE_MODEM;
        }
    }

    trace("GetPreferredConnection: " + g_PreferredConnection);
    return g_PreferredConnection;

} 

function GetConnectionCapabilities()
{
    
    if (CONNECTIONTYPE_INVALID == g_ConnectionCapabilities)
    {
        trace("Calling window.external.GetConnectionCapabilities");
        g_ConnectionCapabilities = window.external.GetConnectionCapabilities();

        
        
        
        g_ModemTypes = window.external.CheckDialReady();
        if (ERR_COMM_NO_ERROR == g_ModemTypes ||
            ERR_COMM_ISDN == g_ModemTypes ||
            ERR_COMM_PHONE_AND_ISDN == g_ModemTypes)
        {
            g_ConnectionCapabilities |= CONNECTIONTYPE_MODEM;
        }        
    }

    trace("GetConnectionCapabilities: " + g_ConnectionCapabilities);
    return g_ConnectionCapabilities;

} 

function InternetCapabilityExists()
{
    var     fExists = false;

    
    
    if (HasNetwork())
    {
        fExists = true;
    }

    if (!fExists && HasModem())
    {
        fExists = true;
    }
    trace("InternetCapabilityExists: " + fExists);
    return fExists;

} 

var ICS_MINIMUM_CALLBACK_SLEEP_TIME = 6000;
var ICS_MINIMUM_TIMEOUT = 40;

function AsyncCallback(callback)
{
    this._callback = callback;
    this._startTime = (new Date()).getTime();
}

function InvokeCallback()
{
    if (this._callback != null)
    {
        var timeRemained = ICS_MINIMUM_CALLBACK_SLEEP_TIME -
            ((new Date()).getTime() - this._startTime);

        
        
        
        
        if (timeRemained < ICS_MINIMUM_TIMEOUT)
        {
            
            
            setTimeout(this._callback, ICS_MINIMUM_TIMEOUT);
        }
        else
        {
            setTimeout(this._callback, timeRemained);
        }
    }
}

AsyncCallback.prototype.InvokeCallback = InvokeCallback;

var g_ConnectedToInternetCallback = null;








function ConnectedToInternetEx(bRefresh, callback)
{
    var retval = false;

    if (UseModem() && (GetConnectionCapabilities() == CONNECTIONTYPE_MODEM))
    {
        
        
        if (callback == null)
        {
            retval = window.external.ConnectedToInternet();
        }
        else
        {
            trace("ConnectedToInternetEx("+bRefresh+", "+callback+")");
            setTimeout(callback, ICS_MINIMUM_CALLBACK_SLEEP_TIME);
        }
    }
    else if (bRefresh && g_Internet == INTERNET_UNKNOWN)
    {
        
        if (callback == null)
        {
            retval = window.external.ConnectedToInternetEx()
            g_Internet = (retval) ? INTERNET_YES : INTERNET_NO;
        }
        else
        {
            g_ConnectedToInternetCallback = new AsyncCallback(callback);
            setTimeout("window.external.AsyncConnectedToInternetEx()",
                ICS_MINIMUM_TIMEOUT);
            
        }
    }
    else
    {
        
        if (callback == null)
        {
            retval = (g_Internet == INTERNET_YES);
        }
        else
        {
            setTimeout(callback, ICS_MINIMUM_CALLBACK_SLEEP_TIME);
        }
    }

    return retval;
}

function ResetConnectedToInternetEx()
{
    g_Internet = INTERNET_UNKNOWN;
}

<!--REQUIRED FUNCTION NAME-->
function ReturnConnectedToInternetEx(bConnected)
{
    g_Internet = (bConnected) ? INTERNET_YES : INTERNET_NO;
    if (g_ConnectedToInternetCallback != null)
    {
        g_ConnectedToInternetCallback.InvokeCallback();
        g_ConnectedToInternetCallback = null;
    }
}

function CanConfigLan()
{
    if (HasBroadband() || HasIndeterminateLan())
    {
        return g_CanConfigLan;
    }
    
    return CONFIG_LAN_NO;
        
}

function GetPublicLanCount()
{
    return g_PublicLanCount;
}

var g_CheckIfConfigInternetCallback = null;

















function CheckIfConfigLan(callback)
{
    g_CheckIfConfigInternetCallback = new AsyncCallback(callback);
    setTimeout("window.external.AsyncGetPublicLanCount()", ICS_MINIMUM_TIMEOUT);
    
}

<!--REQUIRED FUNCTION NAME-->
function ReturnGetPublicLanCount(bConnected, cPublicLan)
{
    
    
    g_Internet = (bConnected) ? INTERNET_YES : INTERNET_NO;

    if (cPublicLan)
    {
        g_PublicLanCount = cPublicLan;
    }
    else
    {
        g_PublicLanCount = 0;
    }

    
    

    if (g_Internet == INTERNET_YES)
    {
        if (g_PublicLanCount == 0)
        {
            if (HasBroadband())
            {
                g_PublicLanCount = 1;
            }
        }
    }

    if (g_PublicLanCount >= 1 || HasBroadband())
    {
        g_CanConfigLan = CONFIG_LAN_YES;
    }
    else
    {
        g_CanConfigLan = CONFIG_LAN_NO;
    }
    
    if (g_CheckIfConfigInternetCallback != null)
    {
        g_CheckIfConfigInternetCallback.InvokeCallback();
        g_CheckIfConfigInternetCallback = null;
    }
}
