



var g_bMigration                        = 0;
var gselectedISPIndx                    = 0;
var gCurrISPURL                         = "about:blank";
var gpgType                             = -1;
var gLastProg                           = 0;
var PAGE_REFDIAL                        = 0;
var PAGE_ISPLIST                        = 1;
var PAGE_ISPDIAL                        = 2;
var PAGE_ISPPAGE                        = 3;
var PAGE_SERVERR                        = 4;
var PAGE_REFDRDY                        = 5;
var PAGE_MIGDRDY                        = 6;
var PAGE_REGDIAL                        = 7;

var PAGETYPE_ISP_NORMAL                 =  0x20;
var PAGETYPE_ISP_TOS                    =  0x40;
var PAGETYPE_ISP_FINISH                 =  0x80;
var PAGETYPE_ISP_CUSTOMFINISH           =  0x100;

var PAGEFLAG_SAVE_CHECKBOX              =  1;


var L_REF_STATUS_DIALING_Text           = "Dialing...";
var L_REF_STATUS_LOCATING_Text          = "Locating service";
var L_REF_STATUS_CONNECTING_Text        = "Connecting to service";
var L_REF_STATUS_CONNECTED_Text         = "Connected";

var L_REF_STATUS_DOWNLOAD_Text          = "Downloading information about ISPs...";
var L_REFDIALTITLE_TEXT                 = "Finding Internet service providers in your area";
var L_REFDIALINTRO_TEXT                 = "Microsoft is retrieving a list of Internet service providers (ISPs) in your area so you can select the one you want to use.";


var REFERALDLCOMPLETED = "{E9E228E0-7A58-11D3-8B99-00A0C91E7F3C}";

var gCurrPage = -1;
var gDialerr = 0;
var TEMP_KEY = "\\Temp";
var ISPINDX = "ISPindex";
var g_IgnoreDialErr = false;

var g_CustomISPName = null;

function SetCustomISPName(Name)
{
    g_CustomISPName = Name;
}







function DisableButtons()
{
    g.MigPagePlsWaitText.style.visibility = "visible";

    try
    {
        g.btnNext.disabled=true;
    }
    catch(e) {}
    try
    {
        g.btnBack.disabled=true;
    }
    catch(e) {}
    try
    {
        InitButtons();
    }
    catch(e) {}
}





var L_drdyref_2_ICS_Text = "Windows will now use your Shared Internet Connection to migrate your existing Internet account setting so you will be able to use it with your new computer";

var L_drdyref_4_ICS_Text = "To proceed, provide the information below and click <b>Next</b>. To continue without migrating an existing account, click <b>Skip</b>.";

var L_drdymig_2_ICS_Text = "Windows will now use your Shared Internet Connection to begin re-establishing your existing Internet account. &nbsp; Click <b>Next</b> to begin connecting.";

var L_drdymig_4_ICS_Text = "To proceed, provide the information below and click <b>Next</b>. To continue without re-establishing an existing Internet Account, click <b>Skip</b>.";


var g_ISPAuto = 1;
var g_CalledFromDrdyref = false;
function drdyref_LoadMe(btnSW)
{
    gCurrPage = PAGE_REFDRDY;
    InitFrameRef();
    if (g != null)
    {
        g_FirstFocusElement = g.btnNext;
        g_FirstFocusElement.focus();
    }

    InitNewButtons();

    var fTapi = TapiObj.IsTAPIConfigured;
    if (!fTapi && !g_bTapiDone)
    {
        g.drdyref_ctrl1.style.visibility="visible";

        if(g_CountryIdx < 0)
        {
            g_CountryIdx = TapiObj.get_CountryIndex;
        }

        g.selCountry.selectedIndex=g_CountryIdx;

        g.edtAreaCode.value    = TapiObj.get_AreaCode;

        if(1 == TapiObj.get_PhoneSystem)
            g.radioTouchToneYes.checked = true; 
        else
            g.radioTouchToneNo.checked = true; 

    }

    if (true == ApiObj.get_OOBEDebugMode)
    {
        g.spnOutsideLine.style.visibility = "visible";
    }
    drdyref_SetOutsideLine( );


    if (g_ISPAuto == 1)
    {
        g.radioYesISPAuto.checked = true;
        g.radioNoISPAuto.checked  = false;
    }
    else
    {
        g.radioYesISPAuto.checked = false;
        g.radioNoISPAuto.checked  = true;
    }

    g_CalledFromDrdyref = true;
    g.btnNext.onclick = ISPNextBtnHandler;
    g.btnSkip.onclick = ISPSkipBtnHandler;
}


function checkISPSetupType()
{
    if (g.radioYesISPAuto.checked == true)
        g_ISPAuto = 1;
    else
        g_ISPAuto = 2;
}


function drdyref_OutsideLineClicked( )

{
    try
    {
        if (null == g.event)
            return;
    }
    catch(e)
    {
        return;
    }

    drdyref_SetOutsideLine( );
}

function drdyref_SetOutsideLine( )
{
    if (true == g.radioOutsideLineYes.checked &&
        "visible" == g.spnOutsideLine.style.visibility)
    {
        g.spnOutsideLineNumber.style.visibility = "visible";
        g.edtOutsideLineNumber.style.visibility = "visible";
        g.edtOutsideLineNumber.value = TapiObj.get_DialOut;
        if (0 == g.edtOutsideLineNumber.value.length)
        {
            g.edtOutsideLineNumber.value = "9";
        }
    }
    else
    {
        g.edtOutsideLineNumber.value = "";
        g.spnOutsideLineNumber.style.visibility = "hidden";
        g.spnOutsideLine.style.visibility = "hidden";
    }

}

function drdymig_LoadMe()
{

    gCurrPage = PAGE_MIGDRDY;
    InitFrameRef();
    if (g != null)
    {
        g_FirstFocusElement = g.btnNext;
        g_FirstFocusElement.focus();
    }

    InitNewButtons();

    var fTapi = TapiObj.IsTAPIConfigured;
    if (!fTapi && !g_bTapiDone)
    {
        g.drdyref_ctrl1.style.visibility="visible";

        if(g_CountryIdx < 0)
        {
            g_CountryIdx = TapiObj.get_CountryIndex;
        }

        g.selCountry.selectedIndex=g_CountryIdx;

        g.edtAreaCode.value    = TapiObj.get_AreaCode;

        if(1 == TapiObj.get_PhoneSystem)
            g.radioTouchToneYes.checked = true; 
        else
            g.radioTouchToneNo.checked = true; 

    }

    g.btnNext.onclick = ISPNextBtnHandler;
    g.btnSkip.onclick = ISPSkipBtnHandler;
}

var bMSgDisplay = true;

function DialIfNumberInPhonebook()
{
    if (window.external.CheckPhoneBook("msobe.isp"))
    {
            GoToDialingPage();
    }
    else
    {
        Navigate("isperror\\isppberr.htm");
    }
}

function ISPNextBtnHandler()
{

    trace("ISPNextBtnHandler: CurrentPage: " + gCurrPage);
    switch (gCurrPage)
    {
        case PAGE_REFDRDY:
        case PAGE_MIGDRDY:
        {
            if (g.drdyref_ctrl1.style.visibility == "visible")
            {
                TapiObj.set_AreaCode = g.edtAreaCode.value;
                TapiObj.set_CountryIndex = g.selCountry.selectedIndex;

                if (g.PulseToneDialing.style.visibility != "hidden")
                {
                    if (g.radioTouchToneYes.checked)
                        TapiObj.set_PhoneSystem = 1;
                    else
                        TapiObj.set_PhoneSystem = 0;
                }

            }

            if ("visible" == g.spnOutsideLine.style.visible &&
                true == g.radioOutsideLineYes.checked &&
                "visible" == g.spnOutsideLineNumber.style.visibility
                )
            {
              TapiObj.set_DialOut = g.edtOutsideLineNumber.value;
            }

            if (!g_CalledFromDrdyref)
            {
                DialIfNumberInPhonebook();
            }
            else
            {
                g_CalledFromDrdyref = false;

                if (g_ISPAuto == 1)
                    DialIfNumberInPhonebook();
                else
                    GoNext();  
            }

            break;
        }

        case PAGE_REFDIAL:
        {
            
            break;
        }
        case PAGE_ISPLIST:
        {
            
            
            
            
            
            
            
            
            
            if (-1 == gselectedISPIndx)
            {
                
                if (bMSgDisplay && (g.selISPDropList.length != 1))
                {
                    
                    
                    
                    bMSgDisplay = false;

                    
                    
                    g.selISPDropList.style.display = "none";
                    g.drdyisp_4_spanID.style.display = "none";
                    g.btnManual.style.display = "none";
                    g.ManualBtnLocalText01.style.display = "none";
                    g.MigListTitle.style.display="none";
                    g.MigListTxt.style.display="none";
                    g.MigListTxt2.style.display="none";
                    
                    gselectedISPIndx = -1;

                    g.MigListNoOffer.style.display="inline";
                    g.MigListTxtNoOffer.style.display="inline";
                }
                else
                {
                    GoNavigate(CKPT_OEMCUST);
                }

            }
            else
            {
                
                
                GoNavigate(CKPT_MIGDIAL);
            }
            break;
        }
        case PAGE_ISPDIAL:
        {
            break;
        }
        case PAGE_ISPPAGE:  
        {
            trace("ISPNextBtnHandler: gpgType: " + gpgType);
            switch (gpgType)
            {
                case PAGETYPE_ISP_NORMAL:
                case PAGETYPE_ISP_TOS:
                {
                    DisableButtons();
                    if (false == window.external.Migrate_GoNext())
                    {
                        OnServerErrorEx(0);
                    }
                    break;
                }
                case PAGETYPE_ISP_FINISH:
                {
                    DisableButtons();
                    var szINSURL = window.external.get_URL(1);
                    if (true == window.external.ProcessINS(szINSURL))
                    {
                        
                        
                        
                        
                        if (HasNetwork())
                        {
                            window.external.FirewallPreferredConnection(false);
                        }
                        GoNavigate(CKPT_OEMCUST);
                    }
                    else
                        Navigate("isperror\\ispins.htm");
                    break;
                }
                case PAGETYPE_ISP_CUSTOMFINISH:
                {
                    break;
                }
                case PAGE_SERVERR:
                {
                    
                    GoNavigate(CKPT_MIGLIST);

                    break;
                }

            }
            break;
        }
    }

}

function ISPBackBtnHandler()
{
    g_bMigration = 0;
    switch (gCurrPage)
    {
        case PAGE_REFDIAL:
        {
            g_IgnoreDialErr = true;
            window.external.Hangup();
            PopCKPT(CKPT_REFDIAL); 
            GoBack();  
            break;
        }
        case PAGE_ISPLIST:
        {
            window.external.Hangup();
            if (!bMSgDisplay)
            {
                bMSgDisplay = true;

                g.selISPDropList.style.display = "inline";
                g.drdyisp_4_spanID.style.display = "inline";
                g.btnManual.style.display = "inline";
                g.ManualBtnLocalText01.style.display = "inline";
                g.MigListTitle.style.display="inline";
                g.MigListTxt.style.display="inline";
                g.MigListTxt2.style.display="inline";
                
                g.MigListTxtNoOffer.style.display="none";
                g.MigListNoOffer.style.display="none";
                g.selISPDropList.focus();

            }
            else
            {
                GoBack();
            }
            break;
        }
        case PAGE_ISPDIAL:
        {
            g_IgnoreDialErr = true;
            window.external.Hangup();
            PopCKPT(CKPT_MIGDIAL); 
            GoBack();  
            break;
        }
        case PAGE_ISPPAGE:
        {
            DisableButtons();
            if (false == window.external.Migrate_GoBack())
            {
                window.external.Hangup();
                PopCKPT(CKPT_MIGDIAL); 
                GoBack();
            }
            break;
        }
        case PAGE_REGDIAL:
        {
            window.external.Hangup();
            GoBack();
            break;
        }
        case PAGE_SERVERR:
        {
            window.external.Hangup();
            PopCKPT(); 
            GoNavigate(CKPT_MIGLIST);

            break;
        }
    }
}

function ISPSkipBtnHandler()
{
    window.external.Hangup();
    PopCKPT();
    g_bMigration = 0;
    GoCancel();
}


function HandleISPDropListChange()
{
    gselectedISPIndx = g.selISPDropList.selectedIndex;
    
    ApiObj.set_RegValue(HKEY_LOCAL_MACHINE, OOBE_MAIN_REG_KEY + TEMP_KEY, ISPINDX, gselectedISPIndx);
    
    
    
    if ((gselectedISPIndx+1) == g.selISPDropList.length)
        gselectedISPIndx = -1;
}




function Refdial_LoadMe()
{

    InitFrameRef()
    g.btnNext.disabled = true;
    InitNewButtons();
    gCurrPage = PAGE_REFDIAL;
    g.btnNext.onclick = ISPNextBtnHandler;
    g.btnBack.onclick = ISPBackBtnHandler;
    g.btnSkip.onclick = ISPSkipBtnHandler;
    if (g != null && g.spnReferralStatus != null)
        g.spnReferralStatus.innerText = L_REF_STATUS_DIALING_Text;

    setTimeout("DoReferralDial()" , 40); 

}

function Miglist_Manual_Btn_Handler()
{
    GoNavigate(CKPT_ICONNECT);
}


function Miglist_LoadMe()
{
    InitFrameRef()
    InitNewButtons();
    gCurrPage = PAGE_ISPLIST;
    g.btnNext.onclick = ISPNextBtnHandler;
    g.btnBack.onclick = ISPBackBtnHandler;
    g.btnSkip.onclick = ISPSkipBtnHandler;
    g.selISPDropList.style.width = 300;

    g.btnManual.onmouseover = HandleNextButtonMouseOver;
    g.btnManual.onmouseout  = HandleNextButtonMouseOut;
    g.btnManual.onmousedown = HandleNextButtonMouseDown;
    g.btnManual.className="newbuttonsNext";
    g.btnManual.onclick = Miglist_Manual_Btn_Handler;

    try
    {
        gselectedISPIndx = ApiObj.get_RegValue(HKEY_LOCAL_MACHINE, OOBE_MAIN_REG_KEY + TEMP_KEY, ISPINDX)
    }
    catch (e)
    {
        gselectedISPIndx = 0;
    }
    if (g.selISPDropList.length <= 0)
    {
        
        PopCKPT(); 
        ISPBackBtnHandler();
    }
    else if (g.selISPDropList.length == 1)
    {
        
        gselectedISPIndx = -1;
        g.MigListNoOffer.style.display="inline";
        g.MigListTxtNoOffer.style.display="inline";
    }
    else
    {
        if ((gselectedISPIndx + 1) > g.selISPDropList.length)
            gselectedISPIndx = 0;

        g.selISPDropList.onchange = HandleISPDropListChange;
        g.selISPDropList.selectedIndex = gselectedISPIndx;
        

        
        if ((gselectedISPIndx + 1) == g.selISPDropList.length)
            gselectedISPIndx = -1;

        g.selISPDropList.style.display = "inline";
        g.drdyisp_4_spanID.style.display = "inline";
        g.btnManual.style.display = "inline";
        g.ManualBtnLocalText01.style.display = "inline";
        g.MigListTitle.style.display="inline";
        g.MigListTxt.style.display="inline";
        g.MigListTxt2.style.display="inline";
        
        g.selISPDropList.focus();

    }
}


function Migdial_LoadMe()
{

    InitFrameRef()
    g.btnNext.disabled = true;
    InitNewButtons();
    gCurrPage = PAGE_ISPDIAL;

    g.btnNext.onclick = ISPNextBtnHandler;
    g.btnBack.onclick = ISPBackBtnHandler;
    g.btnSkip.onclick = ISPSkipBtnHandler;
    try
    {
        gselectedISPIndx = ApiObj.get_RegValue(HKEY_LOCAL_MACHINE, OOBE_MAIN_REG_KEY + TEMP_KEY, ISPINDX)
    }
    catch (e)
    {
        gselectedISPIndx = 0;
    }

    setTimeout("DoISPDial(" + gselectedISPIndx + ")" , 40); 

}


function Migpage_LoadMe()
{
    InitFrameRef();
    g.spnSaveInfo.style.display="none";
    InitNewButtons();
    gCurrPage = PAGE_ISPPAGE;
    g.MigPagePlsWaitText.style.visibility = "hidden";
    g.TOSFramespn.style.display = "none";

    g.btnNext.onclick = ISPNextBtnHandler;
    g.btnBack.onclick = ISPBackBtnHandler;
    g.btnSkip.onclick = ISPSkipBtnHandler;
    g.ISPContent.style.display = "none";
    g.TOSFramespn.style.display = "none";
    g.TOSRadioSpn.style.display = "none";

    g.ISPContent.innerHTML = ReadContent();

    window.external.walk();
    gpgType = window.external.get_PageType();
    switch (gpgType)
    {
        case PAGETYPE_ISP_NORMAL:
        {
            g.ISPContent.style.display = "inline";
            break;
        }
        case PAGETYPE_ISP_TOS:
        {
            g.TOSFrame.navigate(gCurrISPURL);

            g.TOSFramespn.style.display = "inline";
            g.TOSRadioSpn.style.display = "inline";

            break;
        }
       default:
       {
            g.ISPContent.style.display = "inline";
            break;
       }
    }
    var pgFlag = window.external.get_PageFlag();
    switch (pgFlag)
    {
        case PAGEFLAG_SAVE_CHECKBOX:
        {
            
            g.spnSaveInfo.style.display="inline";
            break;
        }
    }
}

function ReadContent()
{
    var fso, f, r = "";
    var ForReading = 1;
    try
    {
    fso = new ActiveXObject("Scripting.FileSystemObject");
    if ("" == gCurrISPURL)
        return;
    f = fso.OpenTextFile(gCurrISPURL, ForReading);
    r = f.ReadAll();
    f.Close();
    }
    catch(e) {}
    return r;
}

function DoReferralDial()
{
    g_bMigration = 1;
    g_IgnoreDialErr = false;
    if ((UseModem()) && window.external.CheckOnlineStatus &&
        window.external.CheckStayConnected("migrate.isp"))
    {
        window.external.Connect(CONNECTED_REFFERAL, "migrate.isp");
    }
    else
    {
        window.external.DialEx(CONNECTED_REFFERAL, "migrate.isp");
    }
}

function DoISPDial(dISP)
{
    g_bMigration = 1;
    g_IgnoreDialErr = false;
    window.external.DialEx(CONNECTED_ISP_MIGRATE, "", dISP);
}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnDialingErrorEx(derr)
{
    window.external.Hangup();

    if (g_IgnoreDialErr)
        return;
    
    switch (derr)
    {
     case DERR_PORT_OR_DEVICE:
     case DERR_PORT_ALREADY_OPEN:
     case DERR_HARDWARE_FAILURE:
     case DERR_DIALTONE:
        Navigate("isperror\\ispdtone.htm");
        break;

     case DERR_BUSY:
        Navigate("isperror\\ispphbsy.htm");
        break;

     case DERR_VOICEANSWER:
     case DERR_PPP_TIMEOUT:
        Navigate("isperror\\isphdshk.htm");
        break;

     case DERR_NO_CARRIER:
     case DERR_REMOTE_DISCONNECT:
        Navigate("isperror\\ispcnerr.htm");
        break;

     case DERR_NOANSWER:
        Navigate("isperror\\ispnoanw.htm");
        break;

     default:
        Navigate("isperror\\isphdshk.htm");
        break;
    }
}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnServerErrorEx(derr)
{
    window.external.Hangup();

    if (g_IgnoreDialErr)
        return;

    Navigate("isperror\\ispsbusy.htm");
}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
var L_NAME_OF_ISP_Text = "MSN";
function OnDialingEx()
{
    

    if (g != null && g.spnISPName != null)
    {
        switch (g_CurrentCKPT)
        {
            case CKPT_ISPDIAL:
                g.spnISPName.innerText = (g_CustomISPName == null) ? 
                    L_NAME_OF_ISP_Text : g_CustomISPName;
                break;
            default:
                g.spnISPName.innerText = window.external.get_ISPName;
                break;
        }

    }

    if (g != null && g.spnDialing != null)
        g.spnDialing.innerText = window.external.get_DialNumber;
}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnConnectingEx()
{
    switch (g_CurrentCKPT)
    {
        case CKPT_REFDIAL:
            if (g != null && g.spnReferralStatus != null)
                g.spnReferralStatus.innerText = L_REF_STATUS_CONNECTING_Text;
            break;
    }

}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnDownloadingEx()
{
        
        

        Agent_TurnOnISPSpecialCommands();



    

       try
       {
           window.parent.document.frames("connDelay").frameElement.style.zIndex=3;
           window.parent.document.frames("connDelay").frameElement.style.display="inline";
           window.parent.document.frames("connDelay").document.body.style.cursor='wait';
       }
       catch (e)
       {
       }

    switch (g_CurrentCKPT)
    {
        case CKPT_REFDIAL:
            if (g != null)
            {
                if (g.spnReferralStatus != null)
                {
                    g.spnReferralStatus.innerText = L_REF_STATUS_CONNECTED_Text;
                }
                if (g.spnTblProgressBar != null)
                {
                    g.spnTblProgressBar.style.visibility="visible";
                }
            }
            break;
    }

}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnDisconnectEx()
{
}

function SetProg(i)
{
    
    if (0 == i )
        return;
    if ((i>0) && (i<=100))
    {
        for (var j = gLastProg; j < i; j++)
        {
            if (g != null && g.tblProg != null)
                g.tblProg.rows(0).cells(j).style.backgroundColor = "green";
        }
        gLastProg = i;
    }

}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnConnectedEx()
{
    g_bTapiDone = true;
    switch (g_CurrentCKPT)
    {
        case CKPT_REFDIAL:
            if (g != null && g.spnReferralStatus != null)
                g.spnReferralStatus.innerText = L_REF_STATUS_CONNECTED_Text;
            if (g != null && g.spnTblProgressBar != null)
                g.spnTblProgressBar.style.visibility="visible";
            break;
    }
}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnRefDownloadProgressEx(dprog)
{
    if (g != null && g.spnReferralStatus != null)
    {
        g.spnReferralStatus.innerText = L_REF_STATUS_DOWNLOAD_Text;
        g.refdial_title1.innerText = L_REFDIALTITLE_TEXT;
        g.refdial_intro1.innerText = L_REFDIALINTRO_TEXT;
    }

    
    SetProg(dprog);
}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnRefDownloadCompleteEx(bRet)
{
    var TIME_FOR_PROGRESS_BAR_FILL = 1000;
    
    if (1 == bRet)
    {
        window.external.Hangup();
        
        StatusObj.set_Status(REFERALDLCOMPLETED, true);
        setTimeout("GoNavigate(" + CKPT_MIGLIST + ")", TIME_FOR_PROGRESS_BAR_FILL);
    }
    else
    {
        OnServerErrorEx(0);

    }

}

<!--REQUIRED FUNCTION NAME:: DO NOT OVERLOAD OR ALTER-->
function OnISPDownloadCompleteEx(szURL)
{

    

    if ("" == szURL)
    {
        OnServerErrorEx(0);
    }
    else
    {
        gCurrISPURL = szURL;
        Navigate("setup\\Migpage.htm");
    }
}
function TOSRadioClick()
{
    if (g.radioAgree.checked)
    {
        g.btnNext.disabled=false;
        g.btnNext.className="buttons";
    }
    else if (g.radioDisAgree.checked)
    {
        g.btnNext.disabled=true;
        g.btnNext.className="buttons-disabled";
    }

}
