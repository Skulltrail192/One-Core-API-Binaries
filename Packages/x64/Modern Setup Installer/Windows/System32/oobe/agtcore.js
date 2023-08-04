



var kdwAgentUserShowed = 2;
var kdwAgentBalloonAutoPace = 8;                         
var kdwAgentRequestInProgress = 4;
var kdwAgentLeftButton = 1;
var kdwAgentRequestFailed = 1;




var kpszAgentFilename = window.external.API.get_SystemDirectory() + "\\oobe\\images\\qmark.acs";
var kdwAgentWelcomeIntroFinished = 9999;
var kdwAgentIdleLevel1Finished = 9995;
var kdwAgentIdleStart = 9994;
var kdwAgentNoop = 9993;
var kdwAgentKeepLookingTimeout = 5000;
var kdwAgentMoveSpeed = 400;
var kdwAgentPageIdleTimeout1 = 10000;
var kdwAgentPageIdleTimeout2 = 20000;
var kpszISPSpecialCommand1 = "ISPCommand1";
var kpszISPSpecialCommand2 = "ISPCommand2";
var kpszISPSpecialCommand3 = "ISPCommand3";










var kiAgentLeftArmOffsetX = 67;
var kiAgentRightArmOffsetX = 6;
var kiAgentRightBodyOffsetX = 30;
var kiAgentLeftArmOffsetY = 58;
var kiAgentRightArmOffsetY = 58;




var g_AgentDisabled; 
var g_AgentCharacter = null;
var g_strAgentCurrentPage = "";
var g_strAgentLastPage = "";
var g_bAgentFirstTime = false;
var g_bAgentPreWelcome = false;
var g_bAgentCongratulateOnFinish = true;
var g_bAgentWelcomeIntroFinished = false;
var g_bAgentRegister3ShortEmail = false;
var g_bAgentRegister3LongEmail = false;
var g_bAgentRegister3Privacy = false;
var g_bAgentRegister3VisitState = false;
var g_bAgentRegister3VisitProvince = false;
var g_bAgentRegister3VisitCountry = false;
var g_bAgentProductKeyCongratulate = false;
var g_bAgentDoneHide = true;
var g_bAgentFirstTimeClick = true;
var g_bAgentIgnoreSelectClick = false;
var g_bAgentIgnoreEvents = false;
var g_bAgentInternalIgnoreEvents = false;
var g_bAgentMenuUp = false;
var g_strAgentLastFocusID = "";
var g_AgentRequestHideImage = null;
var g_AgentRequestShowAssistSpan = null;
var g_AgentRequestShowPopup = null;
var g_AgentRequestIdling = null;
var g_AgentRequestLooking = null;
var g_AgentRequestIgnoreResetIdle = null;
var g_AgentKeepLookingTimer = null;
var g_AgentLookBlinkTimer = null;
var g_bAgentPlayLookReturn = false;
var g_strAgentLookAnimation = "";
var g_AgentLookElement = null;
var g_AgentPageIdleTimer = null;
var g_iAgentPageIdleLevel = 0;
var g_bAgentShowSpecialISPCommands = false;














function Agent_ShowAssistantSpan()
{
    document.all("spnAssist").style.visibility = "visible";
    document.all("AssistImg").style.visibility = "visible";
    document.all("agentStr1").style.visibility = "visible";
    document.all("agentStr2").style.visibility = "visible";
    document.all("agentStr3").style.visibility = "visible";
}

function Agent_HideAssistantSpan()
{
    document.all("spnAssist").style.visibility = "hidden";
    document.all("AssistImg").style.visibility = "hidden";
    document.all("agentStr1").style.visibility = "hidden";
    document.all("agentStr2").style.visibility = "hidden";
    document.all("agentStr3").style.visibility = "hidden";
}

function Agent_ResetFocus() {

        
        
        g.focus();
        
        
        
        
        
                
        if (g_strAgentLastFocusID != "") {
        
                try {
                        
                        Agent_InternalSetFocus(g.document.all(g_strAgentLastFocusID));

                }
                catch (e) {
                }
        }

        
        
        
        
        
        g_AgentCharacter.Activate(2);   
}

function Agent_IsGivingIdleInstructions() {

        if (g_AgentRequestIdling)
                return true;
        else
                return false;
}

var g_CallMusicOnce = false;
function Agent_OnHide(characterID) {

        
        
        
        g_bAgentMenuUp = false;
        
        
        Agent_StopPageIdleTimer();
        
        
        Agent_ShowAssistantSpan();

        
        if (!g_CallMusicOnce)
        {
            g_CallMusicOnce = true;
            window.parent.PlayBackgroundMusic();
        }
        
        
        
        
        Agent_ResetFocus();
}




function Agent_SetCharacterFilename(strFilename) {

        kpszAgentFilename = strFilename;
}

function Agent_Play(strAnimation, bWantRequest) {

        
        
        
        if (null == g_AgentCharacter)
                return null;
                
        
        
        Agent_StopLooking();
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        if (Agent_Play.arguments.length != 2)
                bWantRequest = false;
                
        if (bWantRequest) {
                return g_AgentCharacter.Play(strAnimation);
        }
        else {
                g_AgentCharacter.Play(strAnimation);
                return null;
        }
}

function Agent_MoveTo(x, y, speed, bWantRequest) {


        
        
        
        if (null == g_AgentCharacter)
                return null;
                
        Agent_StopLooking();
        
        if (Agent_MoveTo.arguments.length != 4)
                bWantRequest = false;
                
        if (bWantRequest) {
                return g_AgentCharacter.MoveTo(x, y, speed);
        }
        else {
                g_AgentCharacter.MoveTo(x, y, speed);
                return null;
        }
}

function Agent_Speak(text, bWantRequest) {

        
        
        
        if (null == g_AgentCharacter)
                return null;
                
        Agent_StopLooking();
        
        if (Agent_Speak.arguments.length != 2)
                bWantRequest = false;
        
        
        
        
        
        
        if (IsFarEastLocale() && !IsKoreanLocale()) {
        
                
                text = text + "\u200b \u200b \u200b \u200b \u200b "
                
                
                
                
                
                var i;
                var endloop = text.length/2;
                
                if (endloop > 50)
                        endloop = 50;
                        
                for (i=0; i < endloop; i++)
                        text = text + "\u200b ";
        
        }
        
                
        if (bWantRequest) {
                return g_AgentCharacter.Speak(text);
        }
        else {
                g_AgentCharacter.Speak(text);
                return null;
        }
}

function Agent_Think(text, bWantRequest) {

        
        
        
        if (null == g_AgentCharacter)
                return null;
                
        Agent_StopLooking();
        
        if (Agent_Think.arguments.length != 2)
                bWantRequest = false;
                
        if (bWantRequest) {
                return g_AgentCharacter.Think(text);
        }
        else {
                g_AgentCharacter.Think(text);
                return null;
        }
}

function Agent_Stop(request) {

        if (null == g_AgentCharacter)
                return;
                
        if (Agent_IsLooking()) {

                if (g_AgentLookBlinkTimer) {
                        window.clearTimeout(g_AgentLookBlinkTimer);
                        g_AgentLookBlinkTimer = null;
                }
                
                if (g_AgentKeepLookingTimer) {
                        window.clearTimeout(g_AgentKeepLookingTimer);
                        g_AgentKeepLookingTimer = null;
                }
                
                g_AgentRequestLooking = null;
        }

        if (Agent_Stop.arguments.length == 1)
                g_AgentCharacter.Stop(request);
        else
                g_AgentCharacter.Stop();
}

function Agent_StopAll(types) {

        if (null == g_AgentCharacter)
                return;
                
        if (Agent_IsLooking()) {

                if (g_AgentLookBlinkTimer) {
                        window.clearTimeout(g_AgentLookBlinkTimer);
                        g_AgentLookBlinkTimer = null;
                }
                
                if (g_AgentKeepLookingTimer) {
                        window.clearTimeout(g_AgentKeepLookingTimer);
                        g_AgentKeepLookingTimer = null;
                }
                
                g_AgentRequestLooking = null;
        }

        if (Agent_StopAll.arguments.length == 1)
                g_AgentCharacter.StopAll(types);
        else
                g_AgentCharacter.StopAll();
}
        
function Agent_MoveToElement(elem, side, speed, bWantRequest) {

        
        
        if (null == elem)
                return null;
                
        
        
        
        var curElem = elem;
        var x = elem.document.parentWindow.screenLeft;
        var y = elem.document.parentWindow.screenTop;
        
        while (curElem.tagName.toUpperCase() != "BODY") {
        
                x += curElem.offsetLeft + curElem.clientLeft;
                y += curElem.offsetTop + curElem.clientTop;
                
                curElem = curElem.offsetParent; 
        };

        
        
        
        
        switch (side) {
                case "Left":
                        x = x - g_AgentCharacter.Width + kiAgentLeftArmOffsetX;
                        y -= kiAgentLeftArmOffsetY;
                        break;
                        
                case "Right":
                        x = x + elem.offsetWidth - kiAgentRightArmOffsetX;
                        y -= kiAgentRightArmOffsetY;
                        break;
                        
                case "Top":
                        y -= g_AgentCharacter.Height;
                        break;
                        
                case "Bottom":
                        y += elem.offsetHeight;
                        break;
                        
                case "Center":
                        x += ((elem.offsetWidth - g_AgentCharacter.Width) / 2);
                        y += ((elem.offsetHeight - g_AgentCharacter.Height) / 2);
                        break;
                        
                case "LeftCenter":
                        x = x - g_AgentCharacter.Width + kiAgentLeftArmOffsetX;
                        y += ((elem.offsetHeight - g_AgentCharacter.Height) / 2);
                        break;

                case "RightCenter":
                        x = x + elem.offsetWidth - kiAgentRightArmOffsetX;
                        y += ((elem.offsetHeight - g_AgentCharacter.Height) / 2);
                        break;

                case "TopCenterWidth":
                        x += ((elem.offsetWidth - g_AgentCharacter.Width) / 2);
                        y -= g_AgentCharacter.Height;
                        break;
                                
                case "TopLeft":
                        x = x - g_AgentCharacter.Width + kiAgentLeftArmOffsetX;
                        y -= g_AgentCharacter.Height;
                        break;

                case "TopRight":
                        x = x + elem.offsetWidth - g_AgentCharacter.Width + kiAgentRightBodyOffsetX;
                        y -= g_AgentCharacter.Height;
                        break;
                                                                
                case "BottomRight":
                        x = x + elem.offsetWidth - kiAgentRightArmOffsetX;
                        y += elem.offsetHeight;
                        break;
                        
                case "BottomCenterWidthExactTop":
                        x += ((elem.offsetWidth - g_AgentCharacter.Width) / 2);
                        break;
                                
                case "BottomCenterWidthExactBottom":
                        x += ((elem.offsetWidth - g_AgentCharacter.Width) / 2);
                        y = y + elem.offsetHeight - g_AgentCharacter.Height;
                        break;
                                
                case "BottomCenterWidth":
                        x += ((elem.offsetWidth - g_AgentCharacter.Width) / 2);
                        y += elem.offsetHeight;
                        break;
                                
                case "Exact":
                        break;
        }
                
        var cArgs = Agent_MoveToElement.arguments.length;
        
        if (cArgs < 4) {
                bWantRequest = false;
                if (cArgs < 3)
                        speed = kdwAgentMoveSpeed;
        }
                
        return Agent_MoveTo(x, y, speed, bWantRequest); 
}       

function Agent_GestureAtElement(elem, side, bWantRequest) {


        
        
        Agent_MoveToElement(elem, side, kdwAgentMoveSpeed);
        
        
        
        switch (side) {
                case "Left":
                case "LeftCenter":
                        return Agent_Play("PointLeft", bWantRequest);
                        
                case "Right":
                case "RightCenter":
                case "BottomRight":
                        return Agent_Play("PointRight", bWantRequest);
                        
                case "Top":
                case "TopCenterWidth":
                case "TopLeft":
                case "TopRight":
                        return Agent_Play("PointDown", bWantRequest);
                        
                case "Bottom":
                case "BottomCenterWidthExactTop":
                case "BottomCenterWidthExactBottom":
                case "BottomCenterWidth":
                        return Agent_Play("PointUp", bWantRequest);
        }
        
        return null;
}

function Agent_AddCommand(strName, strCommand) {

        if (!Agent_Init())
                return;
                
        g_AgentCharacter.Commands.Add(strName, strCommand);
}






function QueueStartPageIdleTimer() {

        if (g_AgentRequestIgnoreResetIdle)
                g_AgentCharacter.Stop(g_AgentRequestIgnoreResetIdle);

        
        
        
        
        
        
        
        
                        
        g_AgentRequestIgnoreResetIdle = g_AgentCharacter.Think("\\mrk=" + kdwAgentIdleStart + "\\");
}

function Agent_IsLooking() {

        if (g_AgentRequestLooking)
                return true;
        else
                return false;
}

function Agent_GetLookAnimation(x) {

        if (g_AgentCharacter.Left > x)
                return "LookRight";
        else
                return "LookLeft";
}

function Agent_StartLookingAtElement(elem, animation) {

        
        
        
        var curElem = elem;
        var x = elem.document.parentWindow.screenLeft;
        var y = elem.document.parentWindow.screenTop;

        while (curElem.tagName.toUpperCase() != "BODY") {
        
                x += curElem.offsetLeft + curElem.clientLeft;
                y += curElem.offsetTop + curElem.clientTop;
                
                curElem = curElem.offsetParent; 
        };

        
        
        if (animation != "")
                g_strAgentLookAnimation = animation;
        else
                g_strAgentLookAnimation = Agent_GetLookAnimation(x);
        
        
        
        
        if (!Agent_IsLooking() || (g_AgentLookElement != elem)) {
        
                
                
                g_AgentCharacter.StopAll("play, speak");
                
                g_AgentRequestLooking = g_AgentCharacter.Play(g_strAgentLookAnimation);
        }
        
        
        
        Agent_KeepLooking();
        
        g_AgentLookElement = elem;
}

function Agent_KeepLooking() {

        
        
        if (g_AgentKeepLookingTimer)
                window.clearTimeout(g_AgentKeepLookingTimer);
                
        g_AgentKeepLookingTimer = window.setTimeout("Agent_StopLooking()", kdwAgentKeepLookingTimeout);
}

function Agent_StopLooking() {

        var animation;
        
        
        
        if (!Agent_IsLooking())
                return;

        if (g_bAgentMenuUp) {
                Agent_KeepLooking();
                return;
        }
        
        
        
        if (g_AgentKeepLookingTimer) {
                window.clearTimeout(g_AgentKeepLookingTimer);
                g_AgentKeepLookingTimer = null;
        }
        
        if (g_AgentLookBlinkTimer) {
                window.clearTimeout(g_AgentLookBlinkTimer);
                g_AgentLookBlinkTimer = null;
        }
        
        
        
        if (g_bAgentPlayLookReturn)
                animation = g_strAgentLookAnimation + "Return";
        else
                animation = "RestPose";

        if (!g_bAgentPlayLookReturn)
                g_AgentCharacter.StopAll();

        g_AgentRequestLooking = null;
        g_AgentLookElement = null;
        g_strAgentLookAnimation = "";
        g_bAgentPlayLookReturn = false;

        g_AgentCharacter.Play(animation);

        
        
        
        

        g_AgentCharacter.IdleOn = true;
}

function Agent_OnLookBlinkTimer() {

        
        
        if (g_AgentLookBlinkTimer) {
                window.clearTimeout(g_AgentLookBlinkTimer);
                g_AgentLookBlinkTimer = null;
        }

        

        if (null == g_AgentRequestLooking)
                return;

        

        g_AgentRequestLooking = g_AgentCharacter.Play(g_strAgentLookAnimation + "Blink");
        
        if (g_AgentRequestLooking.Status == kdwAgentRequestFailed)
                g_AgentCharacter.IdleOn = false;
        else
                g_bAgentPlayLookReturn = false;
}

function Agent_IsNextTabItem(elem1, elem2) {

        
        
        
        
        
        
        
        if ((elem1.tabIndex > 0) && (elem2.tabIndex > 0))
                return (elem1.tabIndex == (elem2.tabIndex-1));
        
        
        
        
        
        
        if (elem1.sourceIndex > elem2.sourceIndex)
                return false;
        
        var curIndex = elem1.sourceIndex + 1;
        var curElem;
        
        while (curIndex < elem2.sourceIndex) {
        
                curElem = g.document.all(curIndex);             
                
                ++curIndex;
                
                switch (curElem.tagName) {
                        case "INPUT":
                        case "SELECT":
                                break;
                                
                        default:
                                continue;
                }
                
                
                
                if ((curElem.style.visibility == "hidden") ||
                        (curElem.style.display == "none"))
                        continue;
                                
                
                
                
                return false;
        }
        
        return true;
}

function Agent_MoveToPreWelcomePos() {

        var elem = g.document.all("AgentPos");
        
        
        
        if (null == elem)
                return;
                
        
        
        
        var curElem = elem;
        var x = elem.document.parentWindow.screenLeft;
        var y = elem.document.parentWindow.screenTop;
        
        while (curElem.tagName.toUpperCase() != "BODY") {
        
                x += curElem.offsetLeft + curElem.clientLeft;
                y += curElem.offsetTop + curElem.clientTop;
                
                curElem = curElem.offsetParent; 
        };

        
        
        
        y = y - g_AgentCharacter.Height - 20;   
                
        Agent_MoveTo(x, y, 0);  
}       

function Agent_IsVisible() {

        if (null == g_AgentCharacter)
                return false;
        else
                return g_AgentCharacter.Visible;
}

function Agent_Show() 
{
        
        
        if (!Agent_Init())
                return false;
                
        
        
        
        if (!g_AgentCharacter.Visible) 
        {  
            Agent_HideAssistantSpan();
            
                if (!g_bAgentPreWelcome)
                        Agent_MoveToPreWelcomePos();
                else
                        Agent_MoveToElement(document.all("AssistImg"), "BottomCenterWidthExactBottom", 0);
                
                if (!g_bAgentPreWelcome)
                {
                    g_AgentCharacter.Show();
                    g_AgentCharacter.Play("Welcome");
                    return true;
                }
                else
                {
                    g_AgentCharacter.Show();
                    g_AgentCharacter.Play("WakeUp");
                    return true;
                }
                        
                if ("Finish" == g_strAgentCurrentPage)
                    return true;
        }
        
        return true;
}

function Agent_OnPageIdle() {

        g_AgentPageIdleTimer = null;    
                
        if (g_bAgentMenuUp) {
                Agent_ResetPageIdleTimer();
                return;
        }
        
        if (g_strAgentCurrentPage == "")
                return;
                
        if (g_iAgentPageIdleLevel == 0) {

                try {
                        eval("Agent_On" + g_strAgentCurrentPage + "Idle()");
                }
                catch(e) {
                }
                
                
                
                Agent_Think("\\mrk=" + kdwAgentIdleLevel1Finished + "\\");
                
        }
        else {
        
                
                
                try {
                        Agent_OnPageIdleLevel2();
                }
                catch (e) {
                }
        }
        
        
        
        
        g_AgentRequestIdling = Agent_Think("\\mrk=" + kdwAgentNoop + "\\", true);
}

function Agent_StopPageIdleTimer() {

        if (g_AgentPageIdleTimer) {
                window.clearTimeout(g_AgentPageIdleTimer);
                g_AgentPageIdleTimer = null;
        }
        
        g_iAgentPageIdleLevel = 0;
}

function Agent_StartPageIdleTimer() {

        if (g_AgentPageIdleTimer)
                return;
                
        if (g_iAgentPageIdleLevel == 0)
                g_AgentPageIdleTimer = window.setTimeout("Agent_OnPageIdle()", kdwAgentPageIdleTimeout1);
        else
                g_AgentPageIdleTimer = window.setTimeout("Agent_OnPageIdle()", kdwAgentPageIdleTimeout2);
}
        
function Agent_ResetPageIdleTimer() {

        if (g_AgentRequestIgnoreResetIdle)
                return;
                
        if (Agent_IsGivingIdleInstructions())
                Agent_StopAll();
                
        if (g_AgentPageIdleTimer)
                window.clearTimeout(g_AgentPageIdleTimer);
        
        g_iAgentPageIdleLevel = 0;
        
        g_AgentPageIdleTimer = window.setTimeout("Agent_OnPageIdle()", kdwAgentPageIdleTimeout1);
}

function Agent_StopIgnoreEvents() {

        g_bAgentIgnoreEvents = false;
}

function Agent_StopInternalIgnoreEvents() {

        g_bAgentInternalIgnoreEvents = false;
}

function Agent_InternalIgnoreEvents(bIgnore) {

        if (bIgnore)
                g_bAgentInternalIgnoreEvents = bIgnore;
        else
                window.setTimeout("Agent_StopInternalIgnoreEvents();", 500);
}

function Agent_InternalSetFocus(elem) {

        var bPrevIgnore = g_bAgentInternalIgnoreEvents;
        
        Agent_InternalIgnoreEvents(true);
        
        elem.focus();
        
        g_AgentCharacter.Activate(2);
        
        if (bPrevIgnore == false)
                Agent_InternalIgnoreEvents(false);
}

function Agent_DoPage() {

        

        if ("Finish" == g_strAgentCurrentPage) {
        
                
                
                
                
                
                
                
                
                
                
                if (g_AgentCharacter) {
                        if (g_AgentCharacter.Visible)
                                g_bAgentDoneHide = false;
                        else if (!g_bAgentCongratulateOnFinish)
                                return; 
                }
        }
        
        
        
        
        
        
        if (!Agent_Show())
                return;

        
        
        Agent_StopPageIdleTimer();
        
        
        
        Agent_AddCommonCommands();
        
        
        

        if ((g_strAgentCurrentPage != "Welcome") || g_bAgentPreWelcome)
                Agent_MoveToElement(document.all("AssistImg"), "BottomCenterWidthExactBottom", 0);

        
                                
        var bTryOem = false;
                        
        if (g_strAgentCurrentPage == "") {
                bTryOem = true;
        }
        else {
                        
                
                                        
                try {                   
                        eval("Agent_" + g_strAgentCurrentPage + "AddCommands();");                      
                }
                catch(e) {
                        bTryOem = true;                 
                }
        
                if (!bTryOem) { 
                        try {
                                eval("Agent_" + g_strAgentCurrentPage + "Intro();");                    
                        }
                        catch(e) {
                        }
                }
        }                                               
                                                
        if (bTryOem) {
                        
                
                
                        
                try {                   
                        g.Agent_UserRequestAssistant();
                }
                catch(e) {
                }
                        
                return;
        }
        
        

        QueueStartPageIdleTimer();              
}














function Agent_OnRequestComplete(request) {

        if (g_AgentDisabled)
           return;

        if (request == g_AgentRequestLooking) {
        
                
                
                
                
                
                
                
                if (request.Status == 3) {
                        g_AgentRequestLooking = null;
                        return;
                }
                
                g_bAgentPlayLookReturn = (request.Status == 0);
                g_AgentLookBlinkTimer = window.setTimeout("Agent_OnLookBlinkTimer()", 1000);
                
                return;
        }
        else if (request == g_AgentRequestIdling) {
        
                g_AgentRequestIdling = null;
        }
        else if (request == g_AgentRequestIgnoreResetIdle) {
        
                g_AgentRequestIgnoreResetIdle = null;
        }
        else if (request == g_AgentRequestShowPopup) {
        
                g_bAgentMenuUp = true;
                        
                
        
                if (g_strAgentCurrentPage != "") {
                
                        try {
                                eval("Agent_On" + g_strAgentCurrentPage + "PreDisplayMenu()");
                        }
                        catch(e) {
                        }
                }
                        
                g_AgentCharacter.ShowPopupMenu(g_AgentCharacter.Left + g_AgentCharacter.Width - kiAgentLeftArmOffsetX, g_AgentCharacter.Top);
                        
                g_strAgentLookAnimation = "LookLeft";
                        
                Agent_OnF1();
                
                g_AgentRequestLooking = g_AgentCharacter.Play(g_strAgentLookAnimation);

                Agent_KeepLooking();
        }
        else if (request == g_AgentRequestShowAssistSpan) {
        
                
                
                document.all("spnAssist").style.visibility = "visible";
                
                
                
                
                
                
                if (request.Status == 0)
                    Agent_ShowAssistantSpan();
        }
        else if (request == g_AgentRequestHideImage) {
        
                
                
                
                
                if (request.Status != 0)
                    Agent_HideAssistantSpan();
                        
                g_AgentRequestHideImage = null;
        }
        else {
        
                
                
                
                
                
                
                
                
                
                                
                try {
                
                        eval("g.Agent_RequestComplete(request);");
                        
                }
                catch (e) {
                }
        }       
}

function Agent_OnBookmark(id) {

        if (g_AgentDisabled)
           return;

        switch (id) {
                case kdwAgentWelcomeIntroFinished:              
                        g_bAgentWelcomeIntroFinished = true;
                        break;

                case kdwAgentIdleStart:
                        Agent_StartPageIdleTimer();
                        break;
                                                
                case kdwAgentIdleLevel1Finished:
                                        
                        g_iAgentPageIdleLevel++;
                
                        Agent_StartPageIdleTimer();
                        
                        break;
                                                
                default:
                                                
                        
                        
                        
                        try {
                
                                eval("g.Agent_Bookmark(id);");
                                
                        }
                        catch (e) {
                        }
                        
                        break;
        }
}

function Agent_OnCommand(userInput) {

        if (g_AgentDisabled)
           return;

        g_bAgentMenuUp = false;
        
        
        
                        
        Agent_StopAll();
        Agent_Play("Acknowledge");
                        
        
                
        Agent_ResetFocus();     
        
        
        
        Agent_StopPageIdleTimer();
        
        switch (userInput.Name) {
        
                
                        
                case "":
                case "CloseMenu":
                
                        
                        
                
                        break;
                
                case kpszISPSpecialCommand1:
                        Agent_DoSpecialISPCommand1();
                        break;
                        
                case kpszISPSpecialCommand2:
                        Agent_DoSpecialISPCommand2();
                        break;
                
                case kpszISPSpecialCommand3:
                        Agent_DoSpecialISPCommand3();           
                        break;
                                                                
                default:
                                
                        
                        
                        
                                                                
                        try {
                                
                                
                                
                                
                                
                                
                                
                                eval(userInput.Name + "();");
                                        
                                break;  
                        }
                        catch(e) {

                                
                                
                                

                                try {
                        
                                        eval("g.Agent_Command(userInput.Name);");
                                                
                                        return;
                                }
                                catch (e) {
                                }
                        }                               
        }

        
        
        
        QueueStartPageIdleTimer();
}

function Agent_OnDragComplete(characterID, button, shift, x, y) {

        if (g_AgentDisabled)
           return;

        
        
        
        Agent_ResetFocus();     
        Agent_ResetPageIdleTimer();
}

function Agent_OnClick(characterID, button, shift, x, y) {

        if (g_AgentDisabled)
           return;

        
        
        Agent_StopAll();
        
        
        
        Agent_StopPageIdleTimer();
        
        
        
        if (g_strAgentCurrentPage != "") {
                try {
                        eval("Agent_On" + g_strAgentCurrentPage + "PreDisplayMenu()");
                }
                catch(e) {
                }
        }
                
        
        
        
        
        
        if (g_bAgentFirstTimeClick) {
                g_bAgentFirstTimeClick = false;
                Agent_Play("Surprised");
        }

        Agent_Play("LookDown");
        Agent_Play("LookDownBlink");
        Agent_Play("LookDownReturn");
        Agent_Play("Blink");

        g_bAgentMenuUp = true;

        
        
        
        g_strAgentLookAnimation = "LookDown";

        if (button == kdwAgentLeftButton)       
                g_AgentCharacter.ShowPopupMenu(x, y);

        

        QueueStartPageIdleTimer();              
}

function Agent_OnIdleStart(characterID) {

        if (g_AgentDisabled)
           return;

        if (g_bAgentMenuUp) {
                g_AgentRequestLooking = g_AgentCharacter.Play(g_strAgentLookAnimation);
                Agent_KeepLooking();
                return;
        }
}

function Agent_OnIdleComplete(characterID) {

        if (g_AgentDisabled)
           return;

}



function Agent_Init() {

        if (g_AgentDisabled)
           return false;

        if (g_AgentCharacter)
                return true;
                        
        
        
        

        document.body.insertAdjacentHTML("BeforeEnd", "<OBJECT classid=clsid:D45FD31B-5C6E-11D1-9EC1-00C04FD7081F width=0 height=0 id=Agent></OBJECT>");

        try {
        
                Agent.RaiseRequestErrors = false;
        
                Agent.Characters.Load("Character", kpszAgentFilename);

                g_AgentCharacter = Agent.Characters.Character("Character");
        
                
                        
                
                g_AgentCharacter.Balloon.Style = 0x2200007;
        
                
                
                
                
                
                
                
                
                
                        
                Agent.Characters.Load("AgentCmdFix", "AgentCmd.dat");
                        
        }
        catch(e) {
                return false;
        }
        
        try {
        
                

                g_AgentCharacter.LanguageID = window.external.get_AppLANGID();
                
        }
        catch(e) {
        }
        
        return true;
}

function Agent_Hide() 
{

        if (g_AgentDisabled)
           return;

        if (null == g_AgentCharacter)
                return;
                
        g_AgentCharacter.Hide(); 
}

function Agent_OnUserRequestAssistant() 
{
    if (g_AgentDisabled)
       return;

    if ((null == g_AgentCharacter) || (!g_AgentCharacter.Visible)) 
    {       
        
        
        
        
        
                                
        if ("Finish" == g_strAgentCurrentPage) 
        {
                g_bAgentCongratulateOnFinish = true;
                g_bAgentDoneHide = false;
        }
                        
        Agent_DoPage();
        Agent_Play("Shimmer");

        
        
        if (window.parent.document.dir == "rtl")
        {
            Agent_MoveTo(screen.width - g_AgentCharacter.Width + kiAgentLeftArmOffsetX - 650, g_AgentCharacter.Top, 4);
        }
        else
        {
            Agent_MoveTo(screen.width - g_AgentCharacter.Width + kiAgentLeftArmOffsetX - 300, g_AgentCharacter.Top, 4);
        }
        
        g_AgentRequestShowPopup = Agent_Play("PointLeft", true);
    }
    else 
    {                
        Agent_StopAll();
        Agent_Play("Acknowledge");
          
        if (window.parent.document.dir == "rtl")
        {                       
            if ((g_AgentCharacter.Left + g_AgentCharacter.Width - kiAgentLeftArmOffsetX + 650) > screen.width)
            {
                Agent_MoveTo(screen.width - g_AgentCharacter.Width + kiAgentLeftArmOffsetX - 650, g_AgentCharacter.Top, 4);
            }
        }
        else
        {                       
            if ((g_AgentCharacter.Left + g_AgentCharacter.Width - kiAgentLeftArmOffsetX + 300) > screen.width)
            {
                Agent_MoveTo(screen.width - g_AgentCharacter.Width + kiAgentLeftArmOffsetX - 300, g_AgentCharacter.Top, 4);
            }        
        }
                        
        g_AgentRequestShowPopup = Agent_Play("PointLeft", true);
    } 

}
                
function Agent_OnFocus(elem) {

        if (g_AgentDisabled)
           return;

        
        
        
        g_strAgentLastFocusID = elem.id;
        
        
        
        if (null == g_AgentCharacter)
                return;
        
        if (g_bAgentIgnoreEvents || g_bAgentInternalIgnoreEvents)
                return;

        
        
        Agent_StopPageIdleTimer();
        
        
        
        if (!g_AgentCharacter.Visible)
                return;
        
        if (g_strAgentCurrentPage == "")
                return;
                
        
                
        try {
        
                eval("Agent_On" + g_strAgentCurrentPage + "GotFocus(elem);");
                
        }
        catch(e) {
        }

        

        QueueStartPageIdleTimer();              
}

function Agent_OnKeyDown(elem) {

        if (g_AgentDisabled)
           return;

        if (g_bAgentIgnoreEvents)
                return;

        

        if (g.event.keyCode == 112) {
        
                Agent_OnUserRequestAssistant();
                
                return;
        }
        
        
        
        if (elem.id == "")
                return;
                
        
        
        if (null == g_AgentCharacter)
                return;
        
        
                
        Agent_StopPageIdleTimer();

        
        
        
        if (!g_AgentCharacter.Visible)
                return;
                
        
        
        
        
        if (g_strAgentCurrentPage == "")
                return;
                
        try {
        
                eval("Agent_On" + g_strAgentCurrentPage + "KeyDown(elem, g.event.keyCode);");
                
        }
        catch(e) {
        }

        

        QueueStartPageIdleTimer();              
}


function Agent_OnElementClick(elem) {

        if (g_AgentDisabled)
           return;

        if (g_bAgentIgnoreEvents)
                return;
        
        
        
        if (elem.id == "")
                return;
                
        
        
        if (null == g_AgentCharacter)
                return;
        
        
                
        Agent_StopPageIdleTimer();

        
        
        
        if (!g_AgentCharacter.Visible)
                return;
        
        if (g_strAgentCurrentPage == "")
                return;
                
        try {

                eval("Agent_On" + g_strAgentCurrentPage + "ElementClick(elem);");
                
        }
        catch(e) {
        }

        

        QueueStartPageIdleTimer();              
}

function Agent_OnSelectClick(elem) {

        if (g_AgentDisabled)
           return;

        if (null == g_AgentCharacter)
                return;
        
        if (g_bAgentIgnoreEvents)
                return;

        
                
        Agent_StopPageIdleTimer();

        
        
        
        if (g_bAgentIgnoreSelectClick) {
                g_bAgentIgnoreSelectClick = false;
                return;
        }
        
        Agent_StopAll();
        
        
        
        if (g.document.dir == "rtl") {
                
                Agent_MoveToElement(elem, "Left", 0);   
                Agent_StartLookingAtElement(elem, "LookLeft");

        }
        
        else {
        
                Agent_MoveToElement(elem, "Right", 0);  
                Agent_StartLookingAtElement(elem, "LookRight");
        }
        
    switch (elem.id) {
            case "selUSState":
                    g_bAgentRegister3VisitState = true;
            break;

        case "selCAProvince":
            g_bAgentRegister3VisitProvince = true;
            break;

        case "selCountry":
            g_bAgentRegister3VisitCountry = true;
            break;
   }
        
        

        QueueStartPageIdleTimer();              
}


function Agent_IgnoreEvents(bIgnore) {
        
        if (g_AgentDisabled)
           return;

        if (bIgnore)
                g_bAgentIgnoreEvents = true;
        else
                window.setTimeout("Agent_StopIgnoreEvents();", 500);
}

function Agent_OnSelectChange(elem) {

        if (g_AgentDisabled)
           return;

        if (null == g_AgentCharacter)
                return;

        if (g_bAgentIgnoreEvents)
                return;

        
                
        Agent_StopPageIdleTimer();

        
        
        if (Agent_IsLooking())
                Agent_StopLooking();
        else
                Agent_StopAll();
        
        
        
        Agent_Play("Acknowledge");      
        
        
        
        
        g_bAgentIgnoreSelectClick = true;

        

        QueueStartPageIdleTimer();              
}

function Agent_Activate(strPage) {
        
        if (g_AgentDisabled)
           return;

        
        
        
        if (g_strAgentCurrentPage != "")
                Agent_Deactivate();
                
        
        
        g_strAgentCurrentPage = strPage;
        
        if (g_strAgentCurrentPage == "ISP")
                g_bAgentShowSpecialISPCommands = false;
                
        
        
        
        
        if ("Welcome" == g_strAgentCurrentPage) {
                if (!g_bAgentPreWelcome) {
                        Agent_DoPage();
                        return;
                }
        }
        
        
        
        
        
        
        
        if ((g_CurrentCKPT > CKPT_WELCOME) && !g_bAgentPreWelcome) {
                g_bAgentPreWelcome = true;
                Agent_ShowAssistantSpan();
                return;
        }
        
        
        
        if (null == g_AgentCharacter)
                return;
        
        
        
        
        
        
        if (!g_AgentCharacter.Visible)
                return;
        
        
        
        
        
        
        if ("Register3" == g_strAgentCurrentPage)
                Agent_IgnoreEvents(true);
        
        
        
        
        
        if ("" == g_strAgentLastPage) {
                        
                if (Agent_IsLooking())
                        Agent_StopLooking();
                else
                        Agent_StopAll();
        }
        
        
                
        Agent_DoPage();
}

function Agent_Deactivate() 
{    
    if (g_AgentDisabled)
    {
        return;
    }

    g_strAgentLastPage = g_strAgentCurrentPage;
    g_strAgentCurrentPage = "";

    
        
    
    g_strAgentLastFocusID = "";
        
    
    Agent_StopPageIdleTimer();
        
    
    if (null == g_AgentCharacter)     
    {
        return;
    }
        
    
    Agent_AddCommonCommands();
        
    
    if (!g_AgentCharacter.Visible)
    {
        return;
    }
        
    if (g_AgentRequestHideImage) 
    {
        Agent_StopAll();
        g_AgentCharacter.Hide();
        g_bAgentCongratulateOnFinish = false;
        return;
    }
        
    
    
    
    
    
                
    Agent_Stop();        
                
    
    
    

    Agent_Play("RestPose");    
    g_AgentCharacter.Hide();
}

function Agent_TurnOnISPSpecialCommands() {

        if (g_AgentDisabled)
           return;

        g_bAgentShowSpecialISPCommands = true;
                
        if (null == g_AgentCharacter)
                return;
                
        Agent_AddCommonCommands();
}

function Agent_TurnOffISPSpecialCommands() {

        if (g_AgentDisabled)
           return;

        g_bAgentShowSpecialISPCommands = false;
        
        if (null == g_AgentCharacter)
                return;
                
        try {
        g_AgentCharacter.Commands.Remove(kpszISPSpecialCommand1);
        g_AgentCharacter.Commands.Remove(kpszISPSpecialCommand2);
        g_AgentCharacter.Commands.Remove(kpszISPSpecialCommand3);
    }
    catch (e) {
        }
}

function Agent_OnFinish() {

        if (g_AgentDisabled)
           return;

        
        
        if (null == g_AgentCharacter)
                return;
        
        Agent_StopAll();
        
        g_AgentCharacter.Hide(true);
}

function Agent_OnNavigate() {

}



function Agent_OnProductKeyKeyboardHelper(elem, keyCode) {

        if (g_AgentDisabled)
           return;

        
        

        if (g_bAgentIgnoreEvents)
                return;

        

        if (elem.id == "")
                return;

        

        if (null == g_AgentCharacter)
                return;

        

        Agent_StopPageIdleTimer();

        
        

        if (!g_AgentCharacter.Visible)
                return;

        Agent_OnProductKeyKeyDown(elem, keyCode);

        

        QueueStartPageIdleTimer();
}



function Agent_MouseOver(elem) {

        if (g_AgentDisabled)
           return;

        

        if (null == g_AgentCharacter)
                return;

        if (g_bAgentIgnoreEvents || g_bAgentInternalIgnoreEvents)
                return;

        

        if (!g_AgentCharacter.Visible)
                return;

        if (g_strAgentCurrentPage == "")
                return;

        Agent_StopAll();

        Agent_MoveToElement(elem, "TopCenterWidth", 0);

        Agent_StartLookingAtElement(elem, "Look" + "Down");

}
