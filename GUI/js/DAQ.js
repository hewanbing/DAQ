//__________________________________________
//
//__________________________________________
function loadFile(evt){
    var reader = new FileReader();
    reader.onload = function(file){
	var textArea=document.getElementById('patternContent');
	textArea.value=file.target.result;
	textArea.scrollTop=textArea.scrollHeight;
	DAQHandle.onPatternChange();
	DPCHandle.onPatternChange();
	MonitorHandle.onPatternChange();
	localStorage.setItem("pattern",document.getElementById('patternContent').value);    }
    reader.readAsText(evt.files[0]);
}
(function(){
    $('#fileClear').click(function(){
	document.getElementById('patternContent').value="";
	document.getElementById('fileInput').value="";
    });
})();
//__________________________________________
//Pattern
//__________________________________________
function pattern(elementID){
    var that=this;
    this.elementID=elementID;
    this.JSON=function(){
	var string=document.getElementById(that.elementID).value;
	string=string.replaceAll("\n", "");
	return JSON.parse(string);
    }
}
//__________________________________________
//DAQ
//__________________________________________
function DAQ(){
    var that=this;
    document.getElementById('DAQAutoRefresh').checked=true;
    document.getElementById('DAQBufferAutoRefresh').checked=true;
    this.IP=null;
    this.Port=null;
    this.pattern=null;
    this.action={"Machine":"DAQ"}
    this.status=-1;
    this.bufferStatus=-2;
    this.WS=null;
    this.onPatternChange=function(){
	var json=that.pattern.JSON();
	var ip=json.DAQIP;
	var port=json.DAQWSPort
	if(ip!=that.IP||port!=that.Port||that.WS==null){
	    that.IP=ip;
	    that.Port=port;
	    that.setupWS(ip,port);
	}
    }
    this.setupWS=function(ip,port){
	var ws=new WebSocket("ws://"+ip+":"+port);
	ws.onopen=function(){that.WS=ws;}
	ws.onclose=function(evt){
	    sleep(1000);
	    that.setupWS(that.IP,that.Port);
	}
	ws.onmessage=function(msg){
	    var msgJson=JSON.parse(msg.data);
	    if(msgJson["error"]!=null){
		var message="error:"+msgJson["error"];
	    }
	    var message;
	    if(msgJson["DAQStatus"]!=null){
		that.status=msgJson["DAQStatus"];
		message="status:"+msgJson["DAQStatus"];
	    }
	    if(msgJson["DAQBufferStatus"]!=null){
		that.bufferStatus=msgJson["DAQBufferStatus"];
		message="bufferStatus:"+msgJson["DAQBufferStatus"];
	    }
	    var textArea=document.getElementById('DAQMessages');
	    textArea.value+=message;
	    textArea.value+="\r\n";
	    textArea.scrollTop=textArea.scrollHeight;
	    that.updateGUI();
	    document.getElementById('DAQStatus').innerHTML='';
	}
	ws.onerror=function(err){
	    sleep(1000);
	    that.setupWS(that.IP,that.Port);
	}
    }
    this.updateGUI=function(){
	if(that.status==-1) {
	    document.getElementById('DAQInit').disabled=true;
	    document.getElementById('DAQStart').disabled=true;
	    document.getElementById('DAQStop').disabled=true;
	    document.getElementById('DAQPause').disabled=true;
	    document.getElementById('DAQResume').disabled=true;
	    $('#DAQInit').css('background-color','white');
	    $('#DAQStart').css('background-color','white');
	    $('#DAQStop').css('background-color','white');
	    $('#DAQPause').css('background-color','white');
	    $('#DAQResume').css('background-color','white');
	    document.getElementById('DAQSwitch').checked=false;
	}else if(that.status==0){
	    document.getElementById('DAQInit').disabled=false;
	    document.getElementById('DAQStart').disabled=true;
	    document.getElementById('DAQStop').disabled=true;
	    document.getElementById('DAQPause').disabled=true;
	    document.getElementById('DAQResume').disabled=true;
	    $('#DAQInit').css('background-color','white');
	    $('#DAQStart').css('background-color','white');
	    $('#DAQStop').css('background-color','white');
	    $('#DAQPause').css('background-color','white');
	    $('#DAQResume').css('background-color','white');
	    document.getElementById('DAQSwitch').checked=true;
	}else if(that.status==1){
	    document.getElementById('DAQInit').disabled=false;
	    document.getElementById('DAQStart').disabled=false;
	    document.getElementById('DAQStop').disabled=true;
	    document.getElementById('DAQPause').disabled=true;
	    document.getElementById('DAQResume').disabled=true;
	    $('#DAQInit').css('background-color','#f4816c');
	    $('#DAQStart').css('background-color','white');
	    $('#DAQStop').css('background-color','white');
	    $('#DAQPause').css('background-color','white');
	    $('#DAQResume').css('background-color','white');
	    document.getElementById('DAQSwitch').checked=true;
	}else if(that.status==2){
	    document.getElementById('DAQInit').disabled=true;
	    document.getElementById('DAQStart').disabled=true;
	    document.getElementById('DAQStop').disabled=false;
	    document.getElementById('DAQPause').disabled=false;
	    document.getElementById('DAQResume').disabled=true;
	    $('#DAQInit').css('background-color','white');
	    $('#DAQStart').css('background-color','#f4816c');
	    $('#DAQStop').css('background-color','white');
	    $('#DAQPause').css('background-color','white');
	    $('#DAQResume').css('background-color','white');
	    document.getElementById('DAQSwitch').checked=true;
	}else if(that.status==3){
	    document.getElementById('DAQInit').disabled=false;
	    document.getElementById('DAQStart').disabled=false;
	    document.getElementById('DAQStop').disabled=true;
	    document.getElementById('DAQPause').disabled=true;
	    document.getElementById('DAQResume').disabled=true;
	    $('#DAQInit').css('background-color','white');
	    $('#DAQStart').css('background-color','white');
	    $('#DAQStop').css('background-color','#f4816c');
	    $('#DAQPause').css('background-color','white');
	    $('#DAQResume').css('background-color','white');
	    document.getElementById('DAQSwitch').checked=true;
	}else if(that.status==4){
	    document.getElementById('DAQInit').disabled=true;
	    document.getElementById('DAQStart').disabled=true;
	    document.getElementById('DAQStop').disabled=true;
	    document.getElementById('DAQPause').disabled=true;
	    document.getElementById('DAQResume').disabled=false;
	    $('#DAQInit').css('background-color','white');
	    $('#DAQStart').css('background-color','white');
	    $('#DAQStop').css('background-color','white');
	    $('#DAQPause').css('background-color','#f4816c');
	    $('#DAQResume').css('background-color','white');
	    document.getElementById('DAQSwitch').checked=true;
	}else if(that.status==5){
	    document.getElementById('DAQInit').disabled=true;
	    document.getElementById('DAQStart').disabled=true;
	    document.getElementById('DAQStop').disabled=false;
	    document.getElementById('DAQPause').disabled=false;
	    document.getElementById('DAQResume').disabled=true;
	    $('#DAQInit').css('background-color','white');
	    $('#DAQStart').css('background-color','white');
	    $('#DAQStop').css('background-color','white');
	    $('#DAQPause').css('background-color','white');
	    $('#DAQResume').css('background-color','#f4816c');
	    document.getElementById('DAQSwitch').checked=true;
	}
	document.getElementById('DAQBufferStatus').value=that.bufferStatus;
    }
}
//__________________________________________
//DPC
//__________________________________________
function DPC(){
    var that=this;
    document.getElementById('DPCAutoRefresh').checked=true;
    document.getElementById('DPCBufferAutoRefresh').checked=true;
    this.IP=null;
    this.Port=null;
    this.pattern=null;
    this.action={"Machine":"DPC"}
    this.status=-1;
    this.bufferStatus=-2;
    this.WS=null;
    this.onPatternChange=function(){
	var json=that.pattern.JSON();
	var ip=json.DPCIP;
	var port=json.DPCWSPort
	if(ip!=that.IP||port!=that.Port||that.WS==null){
	    that.IP=ip;
	    that.Port=port;
	    that.setupWS(ip,port);
	}
    }
    this.setupWS=function(ip,port){
	var ws=new WebSocket("ws://"+ip+":"+port);
	ws.onopen=function(){that.WS=ws;}
	ws.onclose=function(evt){
	    sleep(1000);
	    that.setupWS(that.IP,that.Port);
	}
	ws.onmessage=function(msg){
	    var msgJson=JSON.parse(msg.data);
	    if(msgJson["error"]!=null){
		var message="error:"+msgJson["error"];
	    }
	    var message;
	    if(msgJson["DPCStatus"]!=null){
		that.status=msgJson["DPCStatus"];
		message="status:"+that.status;
	    }
	    if(msgJson["DPCBufferStatus"]!=null){
		that.bufferStatus=msgJson["DPCBufferStatus"];
		message="bufferStatus:"+that.bufferStatus;
	    }
	    var textArea=document.getElementById('DPCMessages');
	    textArea.value+=message;
	    textArea.value+="\r\n";
	    textArea.scrollTop=textArea.scrollHeight;
	    that.updateGUI();
	    document.getElementById('DPCStatus').innerHTML='';
	}
	ws.onerror=function(err){
	    sleep(1000);
	    that.setupWS(that.IP,that.Port);
	}
    }
    this.updateGUI=function(){
	if(that.status==-1) {
	    document.getElementById('DPCStart').disabled=true;
	    document.getElementById('DPCStop').disabled=true;
	    $('#DPCStart').css('background-color','white');
	    $('#DPCStop').css('background-color','white');
	    document.getElementById('DPCSwitch').checked=false;
	}else if(that.status==0){
	    document.getElementById('DPCStart').disabled=false;
	    document.getElementById('DPCStop').disabled=true;
	    $('#DPCStart').css('background-color','white');
	    $('#DPCStop').css('background-color','white');
	    document.getElementById('DPCSwitch').checked=true;
	}else if(that.status==1){
	    document.getElementById('DPCStart').disabled=true;
	    document.getElementById('DPCStop').disabled=false;
	    $('#DPCStart').css('background-color','#f4816c');
	    $('#DPCStop').css('background-color','white');
	    document.getElementById('DPCSwitch').checked=true;
	}else if(that.status==2){
	    document.getElementById('DPCStart').disabled=false;
	    document.getElementById('DPCStop').disabled=true;
	    $('#DPCStart').css('background-color','white');
	    $('#DPCStop').css('background-color','#f4816c');
	    document.getElementById('DPCSwitch').checked=true;
	}
	document.getElementById('DPCBufferStatus').value=that.bufferStatus;
    }
}
//__________________________________________
//Monitor
//__________________________________________
function Monitor(){
    var that=this;
    document.getElementById('MonitorAutoRefresh').checked=true;
    this.IP=null;
    this.Port=null;
    this.pattern=null;
    this.action={"Machine":"Monitor"}
    this.status=-1;
    this.WS=null;
    this.onPatternChange=function(){
	var json=that.pattern.JSON();
	var ip=json.MonitorIP;
	var port=json.MonitorWSPort
	if(ip!=that.IP||port!=that.Port||that.WS==null){
	    that.IP=ip;
	    that.Port=port;
	    that.setupWS(ip,port);
	}
    }
    this.setupWS=function(ip,port){
	var ws=new WebSocket("ws://"+ip+":"+port);
	ws.onopen=function(){that.WS=ws;}
	ws.onclose=function(evt){
	    sleep(1000);
	    that.setupWS(that.IP,that.Port);
	}
	ws.onmessage=function(msg){
	    var msgJson=JSON.parse(msg.data);
	    if(msgJson["error"]!=null){
		var message="error:"+msgJson["error"];
	    }
	    if(msgJson["MonStatus"]!=null){
		that.status=msgJson["MonStatus"];
		var message="status:"+that.status;
	    }
	    var textArea=document.getElementById('MonitorMessages');
	    textArea.value+=message;
	    textArea.value+="\r\n";
	    textArea.scrollTop=textArea.scrollHeight;
	    that.updateGUI();
	    document.getElementById('MonitorStatus').innerHTML='';
	}
	ws.onerror=function(err){
	    sleep(1000);
	    that.setupWS(that.IP,that.Port);
	}
    }
    this.updateGUI=function(){
	if(that.status==-1) {
	    document.getElementById('MonitorCleanData').disabled=true;
	    document.getElementById('MonitorSwitch').checked=false;
	}else if(that.status==0){
	    document.getElementById('MonitorCleanData').disabled=false;
	    document.getElementById('MonitorSwitch').checked=true;
	}
    }
}
//__________________________________________
//__________________________________________
DAQHandle=new DAQ();
DPCHandle=new DPC();
MonitorHandle=new Monitor();
Pattern=new pattern('patternContent');
DAQHandle.pattern=Pattern;
DPCHandle.pattern=Pattern;
MonitorHandle.pattern=Pattern;
DAQHandle.updateGUI();
DPCHandle.updateGUI();
MonitorHandle.updateGUI();
document.getElementById('patternContent').value=localStorage.getItem("pattern");
DAQHandle.onPatternChange();
DPCHandle.onPatternChange();
MonitorHandle.onPatternChange();
(function(){
    $('#Pattern').draggable();
    $('#Pattern').resizable();
    $('#DAQ').draggable();
    $('#DAQ').resizable();
    $('#DPC').draggable();
    $('#DPC').resizable();
    $('#DAQBuffer').draggable();
    $('#DAQBuffer').resizable();
    $('#DPCBuffer').draggable();
    $('#DPCBuffer').resizable();
    $('#Monitor').draggable();
    $('#Monitor').resizable();
})();
(function(){
    $('#patternContent').bind('input propertychange','textarea',function () {
	DAQHandle.onPatternChange();
	DPCHandle.onPatternChange();
	MonitorHandle.onPatternChange();
	localStorage.setItem("pattern",document.getElementById('patternContent').value);
    });
})();
(function(){
    $('#DAQInit').click(function(){
	el=document.getElementById('DAQInit');
	DAQHandle.action.action=el.textContent;
	DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
    });
})();
(function(){
    $('#DAQStart').click(function(){
	el=document.getElementById('DAQStart');
	DAQHandle.action.action=el.textContent;
	DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
    });
})();
(function(){
    $('#DAQStop').click(function(){
	el=document.getElementById('DAQStop');
	DAQHandle.action.action=el.textContent;
	DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
    });
})();
(function(){
    $('#DAQPause').click(function(){
	el=document.getElementById('DAQPause');
	DAQHandle.action.action=el.textContent;
	DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
    });
})();
(function(){
    $('#DAQResume').click(function(){
	el=document.getElementById('DAQResume');
	DAQHandle.action.action=el.textContent;
	DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
    });
})();
(function(){
    $('#DAQSwitch').click(function(){
	switchStatus=document.getElementById('DAQSwitch').checked;
	if(switchStatus){
	    if(DAQHandle.pattern.JSON().action=="update pattern"){
		DAQHandle.WS.send(JSON.stringify(DAQHandle.pattern.JSON()));
	    }
	    DAQHandle.action.action="TurnOn";
	    DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	    DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
	}else{
	    DAQHandle.action.action="TurnOff";
	    DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	    DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
	}
    });
})();
(function(){
    $('#DAQClear').click(function(){
	document.getElementById('DAQMessages').value="";
    });
})();
(function(){
    $('#DPCSwitch').click(function(){
	switchStatus=document.getElementById('DPCSwitch').checked;
	if(switchStatus){
	    if(DPCHandle.pattern.JSON().action=="update pattern"){
		DPCHandle.WS.send(JSON.stringify(DPCHandle.pattern.JSON()));
	    }
	    DPCHandle.action.action="TurnOn";
	    DPCHandle.action.user=DPCHandle.pattern.JSON().user;
	    DPCHandle.WS.send(JSON.stringify(DPCHandle.action));
	}else{
	    DPCHandle.action.action="TurnOff";	    
	    DPCHandle.action.user=DPCHandle.pattern.JSON().user;
	    DPCHandle.WS.send(JSON.stringify(DPCHandle.action));
	}
    });
})();
(function(){
    $('#DPCStart').click(function(){
	el=document.getElementById('DPCStart');
	DPCHandle.action.action=el.textContent;
	DPCHandle.action.user=DPCHandle.pattern.JSON().user;
	DPCHandle.WS.send(JSON.stringify(DPCHandle.action));
    });
})();
(function(){
    $('#DPCStop').click(function(){
	el=document.getElementById('DPCStop');
	DPCHandle.action.action=el.textContent;
	DPCHandle.action.user=DPCHandle.pattern.JSON().user;
	DPCHandle.WS.send(JSON.stringify(DPCHandle.action));
    });
})();
(function(){
    $('#DPCClear').click(function(){
	document.getElementById('DPCMessages').value="";
    });
})();
(function(){
    $('#MonitorSwitch').click(function(){
	switchStatus=document.getElementById('MonitorSwitch').checked;
	if(switchStatus){
	    if(MonitorHandle.pattern.JSON().action=="update pattern"){
		MonitorHandle.WS.send(JSON.stringify(MonitorHandle.pattern.JSON()));
	    }
	    MonitorHandle.action.action="TurnOn";
	    MonitorHandle.action.user=MonitorHandle.pattern.JSON().user;
	    MonitorHandle.WS.send(JSON.stringify(MonitorHandle.action));
	}else{
	    MonitorHandle.action.action="TurnOff";	    
	    MonitorHandle.action.user=MonitorHandle.pattern.JSON().user;
	    MonitorHandle.WS.send(JSON.stringify(MonitorHandle.action));
	}
    });
})();
(function(){
    $('#MonitorCleanData').click(function(){
	el=document.getElementById('MonitorCleanData');
	MonitorHandle.action.action=el.textContent;
	MonitorHandle.action.user=MonitorHandle.pattern.JSON().user;
	MonitorHandle.WS.send(JSON.stringify(MonitorHandle.action));
    });
})();
(function(){
    $('#MonitorClear').click(function(){
	document.getElementById('MonitorMessages').value="";
    });
})();
function update(){
    if(document.getElementById('DAQAutoRefresh').checked){
	DAQHandle.action.action="status request";
	DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
    }
    if(document.getElementById('DAQBufferAutoRefresh').checked){
	DAQHandle.action.action="buffer status request";
	DAQHandle.action.user=DAQHandle.pattern.JSON().user;
	DAQHandle.WS.send(JSON.stringify(DAQHandle.action));
    }
    if(document.getElementById('DPCAutoRefresh').checked){
	DPCHandle.action.action="status request";
	DPCHandle.action.user=DPCHandle.pattern.JSON().user;
	DPCHandle.WS.send(JSON.stringify(DPCHandle.action));
    }
    if(document.getElementById('DPCBufferAutoRefresh').checked){
	DPCHandle.action.action="buffer status request";
	DPCHandle.action.user=DPCHandle.pattern.JSON().user;
	DPCHandle.WS.send(JSON.stringify(DPCHandle.action));
    }
    if(document.getElementById('MonitorAutoRefresh').checked){
	MonitorHandle.action.action="status request";
	MonitorHandle.action.user=MonitorHandle.pattern.JSON().user;
	MonitorHandle.WS.send(JSON.stringify(MonitorHandle.action));
    }
}
self.setInterval("update()",3000);
