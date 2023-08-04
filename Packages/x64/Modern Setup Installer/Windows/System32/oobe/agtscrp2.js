


function Agent_AutoUpdateAddCommandsHelper() 
{
    var L_AutoUpdateCommand1_Text = "&What does Automatic Updates do?";
    var L_AutoUpdateCommand2_Text = "How are updates &downloaded?";
    var L_AutoUpdateCommand3_Text = "When are updates &installed?";
    var L_AutoUpdateCommand4_Text = "How can I change my Automatic Updates &settings?";
    var L_AutoUpdateCommand5_Text = "Is there &anything else I need to do?";

    g_AgentCharacter.Commands.Add("Agent_AutoUpdateWhatDoesAuDo",   L_AutoUpdateCommand1_Text);
    g_AgentCharacter.Commands.Add("Agent_AutoUpdateHowDownloaded",  L_AutoUpdateCommand2_Text);
    g_AgentCharacter.Commands.Add("Agent_AutoUpdateWhenInstalled",  L_AutoUpdateCommand3_Text);
    g_AgentCharacter.Commands.Add("Agent_AutoUpdateHowChange",      L_AutoUpdateCommand4_Text);
    g_AgentCharacter.Commands.Add("Agent_AutoUpdateAnythingElse",   L_AutoUpdateCommand5_Text);
}

function Agent_AutoUpdateAddCommands() 
{
    Agent_AutoUpdateAddCommandsHelper();
    
    var L_AutoUpdateCommand6_Text = "Why isn't the &Next button available?";

    g_AgentCharacter.Commands.Add("Agent_AutoUpdateWhereIsNextButton",  L_AutoUpdateCommand6_Text);
        
    Agent_AddAssistantanceCommand();
}

function Agent_OnAutoUpdatePreDisplayMenu() 
{
    if (g.btnNext.disabled) 
    {
        var L_AutoUpdateMenuCommand6_Text = "Why isn't the &Next button available?";

        try 
        {
            g_AgentCharacter.Commands.Insert("Agent_AutoUpdateWhereIsNextButton", "Agent_AutoUpdateAlreadyInstalled",false,L_AutoUpdateMenuCommand6_Text);
        }
        catch (e) 
        {
        }
    }

    else 
    {
        try 
        {
            g_AgentCharacter.Commands.Remove("Agent_AutoUpdateWhereIsNextButton");
        }
        catch (e) 
        {
        }
    }
}

function Agent_AutoUpdateWhatDoesAuDo() 
{    
    var L_AutoUpdateWhatDoesAuDo1_Text = "It can automatically deliver security and other important updates to your computer.";
    Agent_Speak(L_AutoUpdateWhatDoesAuDo1_Text);     
}

function Agent_AutoUpdateHowDownloaded() 
{    
    var L_AutoUpdateHowDownloaded1_Text = "Over unused bandwidth whenever you're connected to the Internet.";
    Agent_Speak(L_AutoUpdateHowDownloaded1_Text);     
}

function Agent_AutoUpdateWhenInstalled() 
{    
    var L_AutoUpdateWhenInstalled1_Text = "Daily, but you can specify when.";
    Agent_Speak(L_AutoUpdateWhenInstalled1_Text);     
}

function Agent_AutoUpdateHowChange() 
{    
    var L_AutoUpdateHowChange1_Text = "From the Desktop, open the System control panel and then click the Automatic Updates tab.";
    Agent_Speak(L_AutoUpdateHowChange1_Text);     
}

function Agent_AutoUpdateAnythingElse() 
{    
    var L_AutoUpdateAnythingElse1_Text = "For some updates, you may need to accept a licensing agreement or restart your computer.";
    Agent_Speak(L_AutoUpdateAnythingElse1_Text);     
}

function Agent_AutoUpdateWhereIsNextButton() 
{       
    var L_AutoUpdateWhereIsNextButton1_Text = "It is grayed out until you make a selection.";
    Agent_Speak(L_AutoUpdateWhereIsNextButton1_Text);
}

