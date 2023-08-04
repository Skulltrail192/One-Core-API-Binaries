
var g_sconnectImgDir="images/";




function sconnect_InitSimpleNavMap()
{
    var sconnect_dir="html\\sconnect\\";
    g_SimpleNavMap.Add("sconnect.htm", sconnect_dir+"scntlast.htm");
}




function sconnectFirstPage_LoadMe()
{
    InitFrameRef('External');

    InitNewButtons();    

    if (g.btnNext != null)
        g_FirstFocusElement = g.btnNext;
    else
    if (g.btnSkip != null)
        g_FirstFocusElement = g.btnSkip;
    else
    if (g.btnBack != null)
        g_FirstFocusElement = g.btnBack;
    
    if (g_FirstFocusElement != null)
        g_FirstFocusElement.focus();
    else
        g.document.body.focus();
}




function sconnectInteriorPage_LoadMe()
{
    InitFrameRef('External');
    g_FirstFocusElement = g.btnNext;
    InitButtons("SimpleBack", "SimpleNext");
    g_FirstFocusElement.focus();
}




function sconnectLastPage_LoadMe() {
  InitFrameRef('External');
  g_FirstFocusElement = g.btnNext;
  
  InitNewButtons();
  g_FirstFocusElement.focus();
}

