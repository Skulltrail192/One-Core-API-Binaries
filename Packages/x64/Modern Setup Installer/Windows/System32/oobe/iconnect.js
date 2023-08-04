



var L_YourUserName_Text  = "Your <u>u</u>ser name:";
var L_YourPassword_Text  = "Your user pass<u>w</u>ord:";
var L_YourPhone_Text     = "Your ISP\'s <u>p</u>hone number:";
var L_StaticIPAddrX_Text = "* Stati<u>c</u> Internet Protocol (IP) address:";
var L_StaticIPAddr_Text  = "Stati<u>c</u> Internet Protocol (IP) address:";
var L_PreferredDNSX_Text = "* P<u>r</u>eferred Domain Name Server (DNS):";
var L_PreferredDNS_Text  = "P<u>r</u>eferred Domain Name Server (DNS):";
var L_AlternateDNSX_Text = "* <u>A</u>lternate Domain Name Server (DNS):";
var L_AlternateDNS_Text  = "<u>A</u>lternate Domain Name Server (DNS):";
var L_SampleIPRange_Text = "Range: 1-223.0-255.0-255.0-255";
var L_OptionalField_Text = "(Optional)";

var g_iconnectImgDir="images/";
var g_iconnect_SW1="0";




function iconnect_InitSimpleNavMap() 
{
    var iconnect_dir="html\\iconnect\\";
    g_SimpleNavMap.Add("iconnect.htm", iconnect_dir + "icntlast.htm");
}




pattern = /^([0-9]+)\.([0-9]+)\.([0-9]+)\.([0-9]+)$/
valid_ip_switch = false;

var ip_reg_exp1 = 0;
var ip_reg_exp2 = 0;
var ip_reg_exp3 = 0;
var ip_reg_exp4 = 0;

var iconnect_fAutoIPAddress = true;
var iconnect_ipaddr_a = 0;
var iconnect_ipaddr_b = 0;
var iconnect_ipaddr_c = 0;
var iconnect_ipaddr_d = 0;
var iconnect_fAutoDNS = true;
var iconnect_ipaddrDns_a = 0;
var iconnect_ipaddrDns_b = 0;
var iconnect_ipaddrDns_c = 0;
var iconnect_ipaddrDns_d = 0;
var iconnect_ipaddrDnsAlt_a = 0;
var iconnect_ipaddrDnsAlt_b = 0;
var iconnect_ipaddrDnsAlt_c = 0;
var iconnect_ipaddrDnsAlt_d = 0;

function validate_ip(instr, current_ip_field)
{
ip_addr = new RegExp(pattern)
    if (!ip_addr.test(instr))  
    {
        valid_ip_switch = false;

        if (current_ip_field == 1)  
        {
            iconnect_fAutoIPAddress = true;
            iconnect_ipaddr_a = 0;
            iconnect_ipaddr_b = 0;
            iconnect_ipaddr_c = 0;
            iconnect_ipaddr_d = 0;
        }

        if (current_ip_field == 2)
        {
            iconnect_fAutoDNS = true;
            iconnect_ipaddrDns_a = 0;
            iconnect_ipaddrDns_b = 0;
            iconnect_ipaddrDns_c = 0;
            iconnect_ipaddrDns_d = 0;
        }

        if ((instr != "") && (current_ip_field == 3))
        {
            iconnect_fAutoDNS = true;
            iconnect_ipaddrDnsAlt_a = 0;
            iconnect_ipaddrDnsAlt_b = 0;
            iconnect_ipaddrDnsAlt_c = 0;
            iconnect_ipaddrDnsAlt_d = 0;
        }

        if ((instr == "") && (current_ip_field == 3))
        {
            valid_ip_switch = true;
            iconnect_fAutoDNS = false;  
            iconnect_ipaddrDnsAlt_a = 0;
            iconnect_ipaddrDnsAlt_b = 0;
            iconnect_ipaddrDnsAlt_c = 0;
            iconnect_ipaddrDnsAlt_d = 0;
        }
    }
    else
    {
        ip_reg_exp1 = parseInt(RegExp.$1);
        ip_reg_exp2 = parseInt(RegExp.$2);
        ip_reg_exp3 = parseInt(RegExp.$3);
        ip_reg_exp4 = parseInt(RegExp.$4);

        
        if ((ip_reg_exp1<1 || ip_reg_exp1>223) || (ip_reg_exp2<0 || ip_reg_exp2>255) || (ip_reg_exp3<0 || ip_reg_exp3>255) || (ip_reg_exp4<0 || ip_reg_exp4>255))
        {
            valid_ip_switch = false;
        }
        else
        {
            valid_ip_switch = true;

            if (current_ip_field == 1)
            {
                iconnect_fAutoIPAddress = false;
                iconnect_ipaddr_a = ip_reg_exp1;
                iconnect_ipaddr_b = ip_reg_exp2;
                iconnect_ipaddr_c = ip_reg_exp3;
                iconnect_ipaddr_d = ip_reg_exp4;
            }

            if (current_ip_field == 2)
            {
                iconnect_fAutoDNS = false;
                iconnect_ipaddrDns_a = ip_reg_exp1;
                iconnect_ipaddrDns_b = ip_reg_exp2;
                iconnect_ipaddrDns_c = ip_reg_exp3;
                iconnect_ipaddrDns_d = ip_reg_exp4;
            }

            if (current_ip_field == 3)
            {
                iconnect_fAutoDNS = false;
                iconnect_ipaddrDnsAlt_a = ip_reg_exp1;
                iconnect_ipaddrDnsAlt_b = ip_reg_exp2;
                iconnect_ipaddrDnsAlt_c = ip_reg_exp3;
                iconnect_ipaddrDnsAlt_d = ip_reg_exp4;
            }

        }
    }
}

function iconnect_validate1()
{
var iconnect_UserName=g.enableform.iconnect_user.value;
var iconnect_UserPass=g.enableform.iconnect_pass.value;
var iconnect_ISPPhone=g.enableform.iconnect_ispphone.value;
var iconnect_ISPAreaCode=g.enableform.iconnect_ispareacode.value;

    if (iconnect_UserName == "")
    {
        FormatRequiredFieldLabel(g.iconnect_spn_username,1)
    }
    else
    {
        FormatRequiredFieldLabel(g.iconnect_spn_username,0)
    }

    if (iconnect_UserPass == "")
    {
        FormatRequiredFieldLabel(g.iconnect_spn_password,1)
    }
    else
    {
        FormatRequiredFieldLabel(g.iconnect_spn_password,0)
    }

    if (iconnect_ISPPhone == "")
    {
        FormatRequiredFieldLabel(g.iconnect_spn_phoneno,1)
    }
    else
    {
        FormatRequiredFieldLabel(g.iconnect_spn_phoneno,0)
    }


    var p_ip = false;
    if (g.enableform.iconnect_obtainip.checked)
    {
    p_ip = true;
    }
    else
    {
        var iconnect_staticIP=g.enableform.iconnect_staticip.value;
        validate_ip(iconnect_staticIP,1);
        if ((iconnect_staticIP == "") || (!valid_ip_switch))
        {
            g.iconnect_spn_staticIP.innerHTML=L_StaticIPAddrX_Text;
            g.iconnect_spn_staticIP.className = "text-error";
            g.iconnect_spn_obtainIP_tx1.style.display="inline";
            g.iconnect_spn_staticIP_exp.style.display="inline";
        }
        else
        {
            g.iconnect_spn_staticIP.innerHTML=L_StaticIPAddr_Text;
            g.iconnect_spn_staticIP.className = "text-primary";
            g.iconnect_spn_obtainIP_tx1.style.display="none";
            g.iconnect_spn_staticIP_exp.style.display="none";
            p_ip = true;
        }
    }

    var p_dns = false;
    var a_dns = false;
    if (g.enableform.iconnect_obtaindns.checked)
    {
    p_dns = true;
    a_dns = true;
    }
    else
    {
        var iconnect_pref_DNS=g.enableform.iconnect_prefdns.value;
        validate_ip(iconnect_pref_DNS,2);
        if ((iconnect_pref_DNS == "") || (!valid_ip_switch))
        {
            g.iconnect_spn_prefrDNS.innerHTML=L_PreferredDNSX_Text;
            g.iconnect_spn_prefrDNS.className = "text-error";
            g.iconnect_spn_prefrdns_exp.style.display="inline";
        }
        else
        {
            g.iconnect_spn_prefrDNS.innerHTML=L_PreferredDNS_Text;
            g.iconnect_spn_prefrDNS.className = "text-primary";
            g.iconnect_spn_prefrdns_exp.style.display="none";
            p_dns = true;
        }

        var iconnect_altr_DNS=g.enableform.iconnect_altdns.value;
        validate_ip(iconnect_altr_DNS,3);
        
        if ((iconnect_altr_DNS != "") && (!valid_ip_switch))
        {
            g.iconnect_spn_alterDNS.innerHTML=L_AlternateDNSX_Text;
            g.iconnect_spn_alterDNS.className = "text-error";
            g.iconnect_spn_alterdns_exp.innerHTML=L_SampleIPRange_Text;
            g.iconnect_spn_alterdns_exp.className = "text-error-small";
            g.iconnect_spn_alterdns_exp.style.display="inline";
        }
        else
        {
            g.iconnect_spn_alterDNS.innerHTML=L_AlternateDNS_Text;
            g.iconnect_spn_alterDNS.className = "text-primary";
            g.iconnect_spn_alterdns_exp.style.display="none";
            g.iconnect_spn_alterdns_exp.innerHTML=L_OptionalField_Text;
            g.iconnect_spn_alterdns_exp.className = "text-primary";
            g.iconnect_spn_alterdns_exp.style.display="inline";
            a_dns = true;
        }

        if ((iconnect_pref_DNS == "") || (!p_dns || !a_dns))
            {g.iconnect_spn_obtainDNS_tx1.style.display="inline";}
        else
            {g.iconnect_spn_obtainDNS_tx1.style.display="none";}
    }

    
    
    if (iconnect_UserName == ""  || iconnect_UserPass == "" || iconnect_ISPPhone == "" || !p_ip || !p_dns || !a_dns)
      {
      g_iconnect_SW1="0";
      }
    else
      {
      g_iconnect_SW1="1";
      window.external.CreateModemConnectoid(
      	iconnect_ISPAreaCode,
        iconnect_ISPPhone,
        iconnect_fAutoIPAddress,
        iconnect_ipaddr_a,
        iconnect_ipaddr_b,
        iconnect_ipaddr_c,
        iconnect_ipaddr_d,
        iconnect_fAutoDNS,
        iconnect_ipaddrDns_a,
        iconnect_ipaddrDns_b,
        iconnect_ipaddrDns_c,
        iconnect_ipaddrDns_d,
        iconnect_ipaddrDnsAlt_a,
        iconnect_ipaddrDnsAlt_b,
        iconnect_ipaddrDnsAlt_c,
        iconnect_ipaddrDnsAlt_d,
        iconnect_UserName,
        iconnect_UserPass);
      
      
      
      
      if (HasNetwork())
      {
        window.external.FirewallPreferredConnection(false);
      }
      
      ApiObj.set_RegValue(HKEY_LOCAL_MACHINE, OOBE_MAIN_REG_KEY + "\\TEMP", "ISPSignup", 1);
      }
}

function iconnectNextBtnHandler() 
{
    iconnect_validate1();
    if (g_iconnect_SW1=="1")
    {
        SimpleNavNext();
    }
}

function iconnectFirstPage_LoadMe()
{
    InitFrameRef('External');

    if (g.btnNext != null)
        g_FirstFocusElement = g.btnNext;
    else
    if (g.btnSkip != null)
        g_FirstFocusElement = g.btnSkip;
    else
    if (g.btnBack != null)
        g_FirstFocusElement = g.btnBack;

    InitNewButtons(null, "SimpleNext");

    checkme2();
    checkme3();

    if (g_FirstFocusElement != null)
        g_FirstFocusElement.focus();
    else
        g.document.body.focus();

    g.enableform.iconnect_ispareacode.value = TapiObj.get_AreaCode();

    g.btnNext.onclick = iconnectNextBtnHandler;
}




function iconnectLastPage_LoadMe()
{
    InitFrameRef('External');
    InitNewButtons("SimpleBack", null);

    g.iconnect_spn_congrats.style.display="inline";
    g.iconnect_spn_valid.style.display="inline";
    g_FirstFocusElement = g.btnNext;
    g_FirstFocusElement.focus();
}








function checkme2()
{
    if (g.enableform.iconnect_obtainip.checked)
    {
        g.enableform.iconnect_staticip.value='';
        g.enableform.iconnect_staticip.style.backgroundColor='#dddddd';
        g.enableform.iconnect_staticip.disabled=true;
        g.iconnect_spn_staticIP.innerHTML=L_StaticIPAddr_Text;
        g.iconnect_spn_staticIP.className = "text-primary";
        g.iconnect_spn_obtainIP_tx1.style.display="none";
        g.iconnect_spn_staticIP_exp.style.display="none";
            iconnect_fAutoIPAddress = true;
            iconnect_ipaddr_a=0;
            iconnect_ipaddr_b=0;
            iconnect_ipaddr_c=0;
            iconnect_ipaddr_d=0;
    }
    else
    {
        g.enableform.iconnect_staticip.value='';
        g.enableform.iconnect_staticip.style.backgroundColor='#ffffff';
        g.enableform.iconnect_staticip.disabled=false;
        g.enableform.iconnect_staticip.focus();
    }
}

function checkme3()
{
    if (g.enableform.iconnect_obtaindns.checked)
    {
        g.enableform.iconnect_prefdns.value='';
        g.enableform.iconnect_altdns.value='';
        g.enableform.iconnect_prefdns.style.backgroundColor='#dddddd';
        g.enableform.iconnect_altdns.style.backgroundColor='#dddddd';
        g.enableform.iconnect_prefdns.disabled=true;
        g.enableform.iconnect_altdns.disabled=true;
        g.iconnect_spn_prefrDNS.innerHTML=L_PreferredDNS_Text;
        g.iconnect_spn_prefrDNS.className = "text-primary";
        g.iconnect_spn_obtainDNS_tx1.style.display="none";
        g.iconnect_spn_prefrdns_exp.style.display="none";
        g.iconnect_spn_alterDNS.innerHTML=L_AlternateDNS_Text;
        g.iconnect_spn_alterDNS.className = "text-primary";
        g.iconnect_spn_alterdns_exp.style.display="none";
        g.iconnect_spn_alterdns_exp.innerHTML=L_OptionalField_Text;
        g.iconnect_spn_alterdns_exp.className = "text-primary";
        g.iconnect_spn_alterdns_exp.style.display="inline";
            iconnect_fAutoDNS=true;
            iconnect_ipaddrDns_a=0;
            iconnect_ipaddrDns_b=0;
            iconnect_ipaddrDns_c=0;
            iconnect_ipaddrDns_d=0;
            iconnect_ipaddrDnsAlt_a=0;
            iconnect_ipaddrDnsAlt_b=0;
            iconnect_ipaddrDnsAlt_c=0;
            iconnect_ipaddrDnsAlt_d=0;
    }
    else
    {
        g.enableform.iconnect_prefdns.value='';
        g.enableform.iconnect_altdns.value='';
        g.enableform.iconnect_prefdns.style.backgroundColor='#ffffff';
        g.enableform.iconnect_altdns.style.backgroundColor='#ffffff';
        g.enableform.iconnect_prefdns.disabled=false;
        g.enableform.iconnect_altdns.disabled=false;
        g.enableform.iconnect_prefdns.focus();
    }
}

