


function InitSimpleNavMap_MouseTut() {

    var mousedir="html\\mouse\\";

    g_SimpleNavMap.Add("mouse.htm", mousedir+"mouse_a.htm");
    g_SimpleNavMap.Add("mouse_a.htm",mousedir+"mouse_b.htm");
    g_SimpleNavMap.Add("mouse_b.htm",mousedir+"mouse_c.htm");
    g_SimpleNavMap.Add("mouse_c.htm",mousedir+"mouse_d.htm");
    g_SimpleNavMap.Add("mouse_d.htm",mousedir+"mouse_e.htm");
    g_SimpleNavMap.Add("mouse_e.htm",mousedir+"mouse_f.htm");
    g_SimpleNavMap.Add("mouse_f.htm",mousedir+"mouse_g.htm");
    g_SimpleNavMap.Add("mouse_g.htm",mousedir+"mouse_h.htm");
    g_SimpleNavMap.Add("mouse_h.htm",mousedir+"mouse_i.htm");
    g_SimpleNavMap.Add("mouse_i.htm",mousedir+"mouse_j.htm");
    g_SimpleNavMap.Add("mouse_j.htm",mousedir+"mouse_k.htm");
}

var curRewardStr=0;

var L_RewardStr1_Text="Good job!";
var L_RewardStr2_Text="That's it!";
var L_RewardStr3_Text="You've got it!";
var L_RewardStr4_Text="Excellent!";
var L_RewardStr5_Text="It's that easy!";
var L_RewardStr6_Text="Outstanding!";
var L_RewardStr7_Text="Bravo!";
var L_RewardStr8_Text="Stupendous!";

var RewardStrs = new Array(8);
{
   for(var i=1;i<=8;i++) {
      eval("RewardStrs["+(i-1).toString()+"]=L_RewardStr"+i.toString()+"_Text;");
   }
}

function GetRewardStr(i) {
    return RewardStrs[i];
}

var ButTouched = new Array(4);

function MouseTut_LoadMe()
{
    InitFrameRef('External');
    if (g.btnNext != null)
        g_FirstFocusElement = g.btnNext;
    else if (g.btnSkip != null)
        g_FirstFocusElement = g.btnSkip;
    else if (g.btnBack != null)
        g_FirstFocusElement = g.btnBack;

    if (GetCurrentPageName() == "mouse.htm")
    {
        InitNewButtons(null, "SimpleNext");

        g.btnSkip.onmouseover = HandleNextButtonMouseOver;
        g.btnSkip.onmouseout  = HandleNextButtonMouseOut;
        g.btnSkip.onmousedown = HandleNextButtonMouseDown;
        g.btnSkip.className   = "newbuttonsNext";
    }
    else
        InitNewButtons("SimpleBack", "SimpleNext");

    if (g_FirstFocusElement != null)
        g_FirstFocusElement.focus();
    else
        g.document.body.focus();
}

var g_MouseImgDir="images/";

function HandleButMouseUp() {
    if(this.tagName=="IMG") {
      if(g.event.button!=1) {
         return;
      }
      var butnumchar=this.id.substr(3,1);
      this.src=g_MouseImgDir+"but"+butnumchar+"_up.gif";
    }
}

function HandleButMouseDown() {
    if(this.tagName=="IMG") {
      if(g.event.button!=1) {
         return;
      }
      var butnumchar=this.id.substr(3,1);
      this.src=g_MouseImgDir+"but"+butnumchar+"_dwn.gif";
    }
}

function HandleButMouseOut() {
    if(this.tagName=="IMG") {
      var butnumchar=this.id.substr(3,1);
      var butnum=parseInt(butnumchar);

      this.src=g_MouseImgDir+"but"+butnumchar+"_idl.gif";
      eval("g.rtxt"+butnumchar+".style.display='none';");
      ButTouched[butnum]=false;
    }
}


function HandleButMouseOver() {
    if(this.tagName=="IMG") {
      var butnumchar=this.id.substr(3,1);
      var butnum=parseInt(butnumchar);

      if(g.event.button==1) {
          this.src=g_MouseImgDir+"but"+butnumchar+"_dwn.gif";
      } else {
          this.src=g_MouseImgDir+"but"+butnumchar+"_up.gif";
      }

     if(!ButTouched[butnum]) {
        ButTouched[butnum]=true;
        eval("g.rtxt"+butnumchar+".innerText=GetRewardStr("+curRewardStr.toString()+");");
        eval("g.rtxt"+butnumchar+".style.display='inline';");
        curRewardStr=(curRewardStr+1) % RewardStrs.length;
        
        

        try {
        	Agent_MouseOver(g.event.srcElement);
        }
        
        catch(e) {
        }
     }
    }
}

function HandleDragEvent() {
    g.event.cancelBubble=true;
    g.event.returnValue=false;
    return false;
}



function setToolBarButtonHandlers(but1,PageChar) {
  
  if(PageChar=="E") {
    but1.onmouseover=HandleButMouseOver_PageE;
    but1.onmousedown=HandleButMouseDown_PageE;
    but1.onmousemove=HandleButMouseOver_PageE;
    but1.onmouseup=HandleButMouseUp_PageE;
  } else {
    but1.onmouseover=HandleButMouseOver;
    but1.onmousedown=HandleButMouseDown;
    but1.onmousemove=HandleButMouseOver;
    but1.onmouseup=HandleButMouseUp;
  }

  but1.onmouseout=HandleButMouseOut;
  but1.ondragover=HandleDragEvent;
  but1.ondragenter=HandleDragEvent;
}

function MouseTut_LoadMe_PageC() {
  MouseTut_LoadMe();

  for(var i=0;i<ButTouched.length;i++)
      ButTouched[i]=false;

  setToolBarButtonHandlers(g.but1);
  setToolBarButtonHandlers(g.but2);
  setToolBarButtonHandlers(g.but3);
  setToolBarButtonHandlers(g.but4);
}

function HandleButMouseOver_PageE() {
    if(this.tagName=="IMG") {
      var butnumchar=this.id.substr(3,1);
      var butnum=parseInt(butnumchar);

      if(g.event.button==1) {
          this.src=g_MouseImgDir+"but"+butnumchar+"_dwn.gif";
      } else {
          this.src=g_MouseImgDir+"but"+butnumchar+"_up.gif";
      }
    }
}

function HandleButMouseDown_PageE() {
   if(this.tagName=="IMG") {
     if(g.event.button!=1) {
        return;
     }

      var butnumchar=this.id.substr(3,1);
      var butnum=parseInt(butnumchar);

      this.src=g_MouseImgDir+"but"+butnumchar+"_dwn.gif";

      if(!ButTouched[butnum]) {
         ButTouched[butnum]=true;
         eval("g.rtxt"+butnumchar+".innerText=GetRewardStr("+curRewardStr.toString()+");");
         eval("g.rtxt"+butnumchar+".style.display='inline';");
         curRewardStr=(curRewardStr+1) % RewardStrs.length;
      }
    }
}


function HandleButMouseUp_PageE() {
   if(this.tagName=="IMG") {
      if(g.event.button!=1) {
         return;
      }
      var butnumchar=this.id.substr(3,1);
      var butnum=parseInt(butnumchar);
      this.src=g_MouseImgDir+"but"+butnumchar+"_up.gif";
      eval("g.rtxt"+butnumchar+".style.display='none';");
      ButTouched[butnum]=false;
    }
}


function MouseTut_LoadMe_PageE() {
  MouseTut_LoadMe();

  for(var i=0;i<ButTouched.length;i++)
      ButTouched[i]=false;

  setToolBarButtonHandlers(g.but1,"E");
  setToolBarButtonHandlers(g.but2,"E");
  setToolBarButtonHandlers(g.but3,"E");
  setToolBarButtonHandlers(g.but4,"E");


}

var g_MouseTutCityIdx=0;
var g_MouseTutCityStr;

function CityFileName(CityName,IsBlackAndWhite) {
  if(IsBlackAndWhite) {
      return g_MouseImgDir+CityName.substr(0,7)+"m.jpg";
  } else {
      return g_MouseImgDir+CityName.substr(0,7)+".jpg";
  }
}

function HandleSelCityChange() {
  g_MouseTutCityIdx=g.selCity.selectedIndex;

  g_MouseTutCityStr=g.selCity.options(g.selCity.selectedIndex).name;

  if(g.selCity.selectedIndex==0) {
     g.cityImg.style.display="none";
     g.noCity.style.display="inline";
  } else {
     g.cityImg.src=CityFileName(g_MouseTutCityStr,false);
     g.noCity.style.display="none";
     g.cityImg.style.display="inline";
  }

  g.btnNext.disabled=(g.selCity.selectedIndex==0);
  InitNewButtons("SimpleBack", "SimpleNext");
}

function MouseTut_LoadMe_PageG() {
  InitFrameRef('External');

  g_FirstFocusElement = g.selCity;

  g.selCity.onchange = HandleSelCityChange;

  HandleSelCityChange();

  g_FirstFocusElement.focus();
}

var g_BW_selected=false;

function MouseTut_LoadMe_PageH() {
  InitFrameRef('External');

  g.bwsel.checked=g_BW_selected;
  g.colorsel.checked=!g_BW_selected;

  if(g_MouseTutCityStr==null) {
     g_MouseTutCityStr="Verona"; 
  }

  g.cityImg.src=CityFileName(g_MouseTutCityStr,g_BW_selected);
  if(g_BW_selected)
     g_FirstFocusElement = g.bwsel;
   else g_FirstFocusElement = g.colorsel;

  InitNewButtons("SimpleBack", "SimpleNext");

  g.colorsel.onclick = PageH_radioClicked;
  g.bwsel.onclick = PageH_radioClicked;

  g_FirstFocusElement.focus();
}

function PageH_radioClicked()
{
    try
    {
        if (g.event == null)
            return;
    }
    catch(e)
    {
        return;
    }

    g_BW_selected=g.bwsel.checked;

    g.cityImg.src=CityFileName(g_MouseTutCityStr,g_BW_selected);

    InitNewButtons("SimpleBack", "SimpleNext");
}

var g_MatSelected=false;
var g_FrameSelected=false;
var g_BorderSelected=false;

function setCityImgBorders() {
    if(g_MatSelected) {
       g.mat_table.bgColor="sandybrown";
       g.mat_table.style.border="thick solid sandybrown"
    } else {
       g.mat_table.bgColor="";
       g.mat_table.style.border="";
    }

    if(g_FrameSelected) {
       g.frametable.bgColor="sienna";
       g.frameclrcel.style.border="thick inset sienna"
    } else {
       g.frametable.bgColor="";
       g.frameclrcel.style.border="";
    }

    if(g_BorderSelected) {
        g.cityImg.style.border="thin solid black";
    } else {
        g.cityImg.style.border="";
    }
}

function PageI_chkboxClicked() {
    g_MatSelected=g.mattesel.checked;
    g_BorderSelected=g.bordersel.checked;
    g_FrameSelected=g.framesel.checked;

    setCityImgBorders();

    InitNewButtons("SimpleBack", "SimpleNext");
}

function MouseTut_LoadMe_PageI() {
  InitFrameRef('External');

  g.mattesel.checked=g_MatSelected;
  g.framesel.checked=g_FrameSelected;
  g.bordersel.checked=g_BorderSelected;

  if(g_MouseTutCityStr==null) {
   
     g_MouseTutCityStr="Bulzano";
     g_BW_selected=true;
  }

  g.cityImg.src=CityFileName(g_MouseTutCityStr,g_BW_selected);

  setCityImgBorders();

  g_FirstFocusElement = g.bordersel;

  InitNewButtons("SimpleBack", "SimpleNext");

  g.mattesel.onclick=PageI_chkboxClicked;
  g.framesel.onclick=PageI_chkboxClicked;
  g.bordersel.onclick=PageI_chkboxClicked;

  g_FirstFocusElement.focus();
}

var g_CityCaptionStr="";

function HandleCityCaptionChange() {
 g_CityCaptionStr =g.caption.value;
}

function MouseTut_LoadMe_PageJ() {
  InitFrameRef('External');

  if(g_MouseTutCityStr==null) {
   
     g_MouseTutCityStr="Bulzano";
     g_BW_selected=true;
     g_MatSelected=true;
     g_FrameSelected=true;
     g_BorderSelected=true;
  }

  g.cityImg.src=CityFileName(g_MouseTutCityStr,g_BW_selected);

  g.caption.value=g_CityCaptionStr;

  setCityImgBorders();

  g_FirstFocusElement = g.caption;

  InitNewButtons("SimpleBack", "SimpleNext");

  g.caption.onchange=HandleCityCaptionChange;

  g_FirstFocusElement.focus();
}

function MouseTut_LoadMe_PageK() {
  InitFrameRef('External');

  if(g_MouseTutCityStr==null) {
   
     g_MouseTutCityStr="Bulzano";
     g_BW_selected=true;
     g_MatSelected=true;
     g_FrameSelected=true;
     g_BorderSelected=true;
     var L_CaptionString_Text = "Seattle Seahawks won against the Steelers.";
     g_CityCaptionStr = L_CaptionString_Text;
  }

  var re = new RegExp("^\\s*$");

   
  if(re.exec(g.caption.value) == null) {
    g.captioncell.style.display="inline";
  }

  
  

  g.cityImg.src=CityFileName(g_MouseTutCityStr,g_BW_selected);

  g.caption.innerText=g_CityCaptionStr;

  setCityImgBorders();




  g_FirstFocusElement = g.btnNext;

  InitNewButtons("SimpleBack");

  g_FirstFocusElement.focus();
}

