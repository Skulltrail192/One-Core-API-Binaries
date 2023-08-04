

var g_isptypeImgDir="images/";


var g_isptypeConnect1="1";
var g_isptypeConnect2="1";
var g_isptypeConnect3="1";

function isptypeFirstPage_LoadMe()
{
	InitFrameRef('External');
	IsptypeToggleAllRadioButtons(true);

	
	if (g_isptypeConnect1=='1') {
		g.isptypeTelModemSpn1.style.display = "inline";
		g.isptypeTelModemSpn2.style.display = "inline";
		}
	if (g_isptypeConnect2=='1') {
		g.isptypeHighSpeedSpn1.style.display = "inline";
		g.isptypeHighSpeedSpn2.style.display = "inline";
		}
	if (g_isptypeConnect3=='1') {
		g.isptypeSharedConnectionSpn1.style.display = "inline";
		g.isptypeSharedConnectionSpn2.style.display = "inline";
		}

	InitNewButtons();

	g_FirstFocusElement = g.btnNext;

	if (g_FirstFocusElement != null)
		g_FirstFocusElement.focus();
	else
		g.document.body.focus();
}

function IsptypeToggleAllRadioButtons(bSwitch)
{
    try
    {
        g.radioTelModem.disabled = !bSwitch;
        g.text_TelModem.disabled = !bSwitch;

        g.radioHighSpeed.disabled = !bSwitch;
        g.text_HighSpeed.disabled = !bSwitch;

        g.radioSharedConnection.disabled = !bSwitch;
        g.text_SharedConnection.disabled = !bSwitch;
    }
    catch (e)
    {
    }
}



