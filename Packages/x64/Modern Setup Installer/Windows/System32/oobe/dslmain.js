
var g_dslmainImgDir="images/";

var dslmain_fNeedsLogon = true;

var L_StaticIpAddress_Text  = "S<U>t</U>atic IP address:";
var L_StaticIpAddress2_Text = "* S<U>t</U>atic IP address:";
var L_SubnetMask_Text       = "S<U>u</U>bnet mask:";
var L_SubnetMask2_Text      = "* S<U>u</U>bnet mask:";
var L_Defaultgateway_Text   = "D<U>e</U>fault gateway:";
var L_Defaultgateway2_Text  = "* D<U>e</U>fault gateway:";
var L_YourUsername_Text     = "Your <u>u</u>sername :";
var L_YourUsername2_Text    = "* Your <u>u</u>sername :";
var L_PreferredDNS_Text     = "<U>P</U>referred DNS:";
var L_PreferredDNS2_Text    = "* <U>P</U>referred DNS:";
var L_AlternateDNS_Text     = "<U>A</U>lternate DNS:";
var L_AlternateDNS2_Text    = "* <U>A</U>lternate DNS:";



var dslmain_UserName = '';
var dslmain_Password = '';
var dslmain_ServiceName = '';




var dslmain_fAutoIpAddress = true;
var dslmain_fAutoDns = true;



var dslmain_staticip = '';
var dslmain_staticip_a = 0;
var dslmain_staticip_b = 0;
var dslmain_staticip_c = 0;
var dslmain_staticip_d = 0;



var dslmain_subnetmask = '';
var dslmain_subnetmask_a = 0;
var dslmain_subnetmask_b = 0;
var dslmain_subnetmask_c = 0;
var dslmain_subnetmask_d = 0;



var dslmain_defgateway = '';
var dslmain_defgateway_a = 0;
var dslmain_defgateway_b = 0;
var dslmain_defgateway_c = 0;
var dslmain_defgateway_d = 0;



var dslmain_prefdns = '';
var dslmain_prefdns_a = 0;
var dslmain_prefdns_b = 0;
var dslmain_prefdns_c = 0;
var dslmain_prefdns_d = 0;



var dslmain_altdns = '';
var dslmain_altdns_a = 0;
var dslmain_altdns_b = 0;
var dslmain_altdns_c = 0;
var dslmain_altdns_d = 0;








function dslmainFirstPage_LoadMe()
{
    InitFrameRef('External');

    g_FirstFocusElement = g.btnNext;
    InitNewButtons();
    g_FirstFocusElement.focus();

    g.dsltypeform.radioDSLYes.checked = true;
}




function dslmainLastPage_LoadMe()
{
    InitFrameRef('External');
    g_FirstFocusElement = g.btnNext;
    InitNewButtons();
    g_FirstFocusElement.focus();

    g.Congrats_Title.innerText  = g_Congrats_Type_Heading;
    g.Congrats_text01.innerText = g_Congrats_Type;
}








var g_UserEnteredInvalidDSLsetting = false;
function dsl_pppoe_LoadMe()
{
    InitFrameRef('External');
    g_FirstFocusElement = g.dsl_username;

    InitNewButtons();

    if (g_UserEnteredInvalidDSLsetting)
    {
        g.DSLmainErrorText01.style.visibility = "visible";
        g.DSLmainErrorText01.className = "text-error";

        g.dslusernamelocaltext.innerHTML = L_YourUsername2_Text;
        g.dslusernamelocaltext.className = "text-error";
    }

    g.dsl_username.value = dslmain_UserName;
    g.dsl_password.value = dslmain_Password;
    g.dsl_servicename.value = dslmain_ServiceName;


    g_FirstFocusElement.focus();
}

function dsl_pppoe_OnNext()
{
    var fSuccess = pppoe_SaveData();
    if (fSuccess)
    {
        if (g_UserEnteredInvalidDSLsetting)
        {
            g_UserEnteredInvalidDSLsetting = false;
            g.DSLmainErrorText01.style.visibility = "hidden";
        }

        window.external.CreatePppoeConnectoid(dslmain_ServiceName,
                                              dslmain_fAutoIpAddress,
                                              dslmain_staticip_a,
                                              dslmain_staticip_b,
                                              dslmain_staticip_c,
                                              dslmain_staticip_d,
                                              dslmain_fAutoDns,
                                              dslmain_prefdns_a,
                                              dslmain_prefdns_b,
                                              dslmain_prefdns_c,
                                              dslmain_prefdns_d,
                                              dslmain_altdns_a,
                                              dslmain_altdns_b,
                                              dslmain_altdns_c,
                                              dslmain_altdns_d,
                                              dslmain_UserName,
                                              dslmain_Password
                                              );
        ResetConnectedToInternetEx();
    }
    return fSuccess;
}

function pppoe_SaveData()
{
    var fSuccess = true;

    if ((g.dsl_username.value == null) || (g.dsl_username.value == ""))
    {
        g_UserEnteredInvalidDSLsetting = true;
        fSuccess = false;
        dslmain_UserName = '';
    }
    else
    {
        if (g_UserEnteredInvalidDSLsetting)
        {
            g.DSLmainErrorText01.style.visibility = "hidden";
            g.dslusernamelocaltext.innerHTML = L_YourUsername_Text;
            g.dslusernamelocaltext.className = "text-primary";
        }
        dslmain_UserName = g.dsl_username.value;
    }

    
    
    if ((g.dsl_password.value == null) || (g.dsl_password.value == ""))
    {
        dslmain_Password = ''; 
    }
    else
    {
        dslmain_Password = g.dsl_password.value; 
    }

    if ((g.dsl_servicename.value == null) || (g.dsl_servicename.value == ""))
    {
        dslmain_ServiceName = '';
    }
    else
    {
        dslmain_ServiceName = g.dsl_servicename.value;
    } 
    
    dslmain_fAutoIpAddress = true;
    dslmain_staticip = '';
    dslmain_staticip_a = 0;
    dslmain_staticip_b = 0;
    dslmain_staticip_c = 0;
    dslmain_staticip_d = 0;
    dslmain_subnetmask = '';
    dslmain_subnetmask_a = 0;
    dslmain_subnetmask_b = 0;
    dslmain_subnetmask_c = 0;
    dslmain_subnetmask_d = 0;
    dslmain_defgateway = '';
    dslmain_defgateway_a = 0;
    dslmain_defgateway_b = 0;
    dslmain_defgateway_c = 0;
    dslmain_defgateway_d = 0;

    dslmain_fAutoDns = true;
    dslmain_prefdns = '';
    dslmain_prefdns_a = 0;
    dslmain_prefdns_b = 0;
    dslmain_prefdns_c = 0;
    dslmain_prefdns_d = 0;
    dslmain_altdns = '';
    dslmain_altdns_a = 0;
    dslmain_altdns_b = 0;
    dslmain_altdns_c = 0;
    dslmain_altdns_d = 0;

    return fSuccess;
}





function dsl_broadband_LoadMe()
{
    InitFrameRef('External');
    g_FirstFocusElement = g.btnNext;

    InitNewButtons();
    if (g_fFirewallRequired)    
    {
        g.text_description_dsl_b1.style.display = "inline";
    }
    else 
    {
        g.text_description_dsl_b2.style.display = "inline";
    }
    
    dsl_autoipClickHandler();
    dsl_autodnsClickHandler();

    g_FirstFocusElement.focus();
}

function dsl_broadband_OnNext()
{
    var fDSLSuccess = dsl_SaveData();
    if (fDSLSuccess)
    {
        window.external.SetPreferredConnectionTcpipProperties(
                                              dslmain_fAutoIpAddress,
                                              dslmain_staticip_a,
                                              dslmain_staticip_b,
                                              dslmain_staticip_c,
                                              dslmain_staticip_d,
                                              dslmain_subnetmask_a,
                                              dslmain_subnetmask_b,
                                              dslmain_subnetmask_c,
                                              dslmain_subnetmask_d,
                                              dslmain_defgateway_a,
                                              dslmain_defgateway_b,
                                              dslmain_defgateway_c,
                                              dslmain_defgateway_d,
                                              dslmain_fAutoDns,
                                              dslmain_prefdns_a,
                                              dslmain_prefdns_b,
                                              dslmain_prefdns_c,
                                              dslmain_prefdns_d,
                                              dslmain_altdns_a,
                                              dslmain_altdns_b,
                                              dslmain_altdns_c,
                                              dslmain_altdns_d,
                                              g_fFirewallRequired
                                              );
        ResetConnectedToInternetEx();
    }

    return fDSLSuccess;
}


function dsl_SaveData()
{
    var fSuccess = true;
    var ip_regex = new RegExp(g_ip_regex);
    
    
    
    dslmain_UserName = (null != g.dsl_username)
                     ? g.dsl_username.value
                     : ''; 
    dslmain_Password = (null != g.dsl_password)
                     ? g.dsl_password.value 
                     : ''; 
    dslmain_ServiceName = (null != g.dsl_servicename)
                        ? g.dsl_servicename.value 
                        : ''; 

    
    

    
    
    if (g.dsl_autoip.checked)
    {
        dslmain_fAutoIpAddress = true;

        dslmain_staticip = '';
        dslmain_staticip_a = 0;
        dslmain_staticip_b = 0;
        dslmain_staticip_c = 0;
        dslmain_staticip_d = 0;
        g.dsl_lbl_staticip.className="text-primary";
    
        dslmain_subnetmask = '';
        dslmain_subnetmask_a = 0;
        dslmain_subnetmask_b = 0;
        dslmain_subnetmask_c = 0;
        dslmain_subnetmask_d = 0;
        g.dsl_lbl_subnetmask.className="text-primary";

        dslmain_defgateway = '';
        dslmain_defgateway_a = 0;
        dslmain_defgateway_b = 0;
        dslmain_defgateway_c = 0;
        dslmain_defgateway_d = 0;
        g.dsl_lbl_defgateway.className="text-primary";
    }
    else
    {
        dslmain_fAutoIpAddress = false;

        if (validate_ip_addr(g.dsl_staticip.value)) 
        {
            dslmain_staticip = g.dsl_staticip.value;
            ip_regex.test(dslmain_staticip);
            dslmain_staticip_a = parseInt(RegExp.$1);
            dslmain_staticip_b = parseInt(RegExp.$2);
            dslmain_staticip_c = parseInt(RegExp.$3);
            dslmain_staticip_d = parseInt(RegExp.$4);
            g.dsl_lbl_staticip.innerHTML=L_StaticIpAddress_Text;
            g.dsl_lbl_staticip.className="text-primary";
        }
        else
        {
            fSuccess = false;
            g.dsl_lbl_staticip.innerHTML=L_StaticIpAddress2_Text;
            g.dsl_lbl_staticip.className="text-error";
        }

        if (validate_subnet_mask(g.dsl_subnetmask.value))
        {
            dslmain_subnetmask = g.dsl_subnetmask.value;
            ip_regex.test(dslmain_subnetmask);
            dslmain_subnetmask_a = parseInt(RegExp.$1);
            dslmain_subnetmask_b = parseInt(RegExp.$2);
            dslmain_subnetmask_c = parseInt(RegExp.$3);
            dslmain_subnetmask_d = parseInt(RegExp.$4);
            g.dsl_lbl_subnetmask.innerHTML=L_SubnetMask_Text;
            g.dsl_lbl_subnetmask.className="text-primary";
        }
        else
        {
            fSuccess = false;
            g.dsl_lbl_subnetmask.innerHTML=L_SubnetMask2_Text;
            g.dsl_lbl_subnetmask.className="text-error";
        }

        if (validate_ip_addr(g.dsl_defgateway.value)) 
        {
            dslmain_defgateway = g.dsl_defgateway.value;
            ip_regex.test(dslmain_defgateway);
            dslmain_defgateway_a = parseInt(RegExp.$1);
            dslmain_defgateway_b = parseInt(RegExp.$2);
            dslmain_defgateway_c = parseInt(RegExp.$3);
            dslmain_defgateway_d = parseInt(RegExp.$4);
            g.dsl_lbl_defgateway.innerHTML=L_Defaultgateway_Text;
            g.dsl_lbl_defgateway.className="text-primary";
        }
        else
        {
            fSuccess = false;
            g.dsl_lbl_defgateway.innerHTML=L_Defaultgateway2_Text;
            g.dsl_lbl_defgateway.className="text-error";
        }
    }

    
    
    if (g.dsl_autodns.checked)
    {
        dslmain_fAutoDns = true;

        dslmain_prefdns = '';
        dslmain_prefdns_a = 0;
        dslmain_prefdns_b = 0;
        dslmain_prefdns_c = 0;
        dslmain_prefdns_d = 0;
        g.dsl_lbl_prefdns="text-primary";
    
        dslmain_altdns = '';
        dslmain_altdns_a = 0;
        dslmain_altdns_b = 0;
        dslmain_altdns_c = 0;
        dslmain_altdns_d = 0;
        g.dsl_lbl_altdns="text-primary";
    }
    else
    {
        dslmain_fAutoDns = false;
        if (validate_ip_addr(g.dsl_prefdns.value)) 
        {
            dslmain_prefdns = g.dsl_prefdns.value;
            ip_regex.test(dslmain_prefdns);
            dslmain_prefdns_a = parseInt(RegExp.$1);
            dslmain_prefdns_b = parseInt(RegExp.$2);
            dslmain_prefdns_c = parseInt(RegExp.$3);
            dslmain_prefdns_d = parseInt(RegExp.$4);
            g.dsl_lbl_prefdns.innerHTML=L_PreferredDNS_Text;
            g.dsl_lbl_prefdns.className="text-primary";
        }
        else
        {
            fSuccess = false;
            g.dsl_lbl_prefdns.innerHTML=L_PreferredDNS2_Text;
            g.dsl_lbl_prefdns.className="text-error";
        }


        
        if (g.dsl_altdns.value == "")
        {
            dslmain_altdns = '';
            dslmain_altdns_a = 0;
            dslmain_altdns_b = 0;
            dslmain_altdns_c = 0;
            dslmain_altdns_d = 0;
            g.dsl_lbl_altdns.innerHTML=L_AlternateDNS_Text;
            g.dsl_lbl_altdns.className="text-primary";        
        }
        else
        {
            if (validate_ip_addr(g.dsl_altdns.value)) 
            {
                dslmain_altdns = g.dsl_altdns.value;
                ip_regex.test(dslmain_altdns);
                dslmain_altdns_a = parseInt(RegExp.$1);
                dslmain_altdns_b = parseInt(RegExp.$2);
                dslmain_altdns_c = parseInt(RegExp.$3);
                dslmain_altdns_d = parseInt(RegExp.$4);
                g.dsl_lbl_altdns.innerHTML=L_AlternateDNS_Text;
                g.dsl_lbl_altdns.className="text-primary";
            }
            else
            {
                fSuccess = false;
                g.dsl_lbl_altdns.innerHTML=L_AlternateDNS2_Text;
                g.dsl_lbl_altdns.className="text-error";
            }
        }

    }


    if (fSuccess)
        g.dslinvalidtexterror.style.display="none";
    else
        g.dslinvalidtexterror.style.display="inline";

    return fSuccess;

}

function dsl_autoipClickHandler()
{
    if (g.dsl_autoip.checked)
    {
        g.dsl_staticip.value='';
        g.dsl_subnetmask.value='';
        g.dsl_defgateway.value='';
        g.dsl_staticip.style.backgroundColor='#dddddd';
        g.dsl_subnetmask.style.backgroundColor='#dddddd';
        g.dsl_defgateway.style.backgroundColor='#dddddd';
        g.dsl_staticip.disabled=true;
        g.dsl_subnetmask.disabled=true;
        g.dsl_defgateway.disabled=true;

        g.dsl_lbl_staticip.innerHTML=L_StaticIpAddress_Text;
        g.dsl_lbl_subnetmask.innerHTML=L_SubnetMask_Text;
        g.dsl_lbl_defgateway.innerHTML=L_Defaultgateway_Text;
        g.dsl_lbl_staticip.className="text-primary";
        g.dsl_lbl_subnetmask.className="text-primary";
        g.dsl_lbl_defgateway.className="text-primary";
    }
    else
    {
        g.dsl_staticip.value=dslmain_staticip;
        g.dsl_subnetmask.value=dslmain_subnetmask;
        g.dsl_defgateway.value=dslmain_defgateway;
        g.dsl_staticip.style.backgroundColor='#ffffff';
        g.dsl_subnetmask.style.backgroundColor='#ffffff';
        g.dsl_defgateway.style.backgroundColor='#ffffff';
        g.dsl_staticip.disabled=false;
        g.dsl_subnetmask.disabled=false;
        g.dsl_defgateway.disabled=false;
        g.dsl_staticip.select();
        g.dsl_staticip.focus();
    }
}

function dsl_autodnsClickHandler()
{
    if (g.dsl_autodns.checked)
    {
        g.dsl_prefdns.value='';
        g.dsl_altdns.value='';
        g.dsl_prefdns.style.backgroundColor='#dddddd';
        g.dsl_altdns.style.backgroundColor='#dddddd';
        g.dsl_prefdns.disabled=true;
        g.dsl_altdns.disabled=true;

        g.dsl_lbl_prefdns.innerHTML=L_PreferredDNS_Text;
        g.dsl_lbl_altdns.innerHTML=L_AlternateDNS_Text;
        g.dsl_lbl_prefdns.className="text-primary";
        g.dsl_lbl_altdns.className="text-primary";
    }
    else
    {
        g.dsl_prefdns.value=dslmain_prefdns;
        g.dsl_altdns.value=dslmain_altdns;
        g.dsl_prefdns.style.backgroundColor='#ffffff';
        g.dsl_altdns.style.backgroundColor='#ffffff';
        g.dsl_prefdns.disabled=false;
        g.dsl_altdns.disabled=false;
        g.dsl_prefdns.select();
        g.dsl_prefdns.focus();
    }
}


