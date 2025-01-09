const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
  <head>
    <title>Glowtie</title>
    <style>
      body{background-color:#000000; 
        font-family:"Courier New"; 
        color:#ffffff;
        height: 100%;
        min-height: 100%;}
      .button{width:150px;height:50px;
        background-color:gold;
        color:black;
        position:relative;
        margin:25px;
        font-family:'Courier New',monospace;
        font-size:20px;
        border:0px;
        border-radius:10px;}
      .slider{appearance:none;
        width:100%;height:25px;
        margin:10px;
        background:#000000;
        outline:none;
        border-radius:15px;}
      #colorUpdate{background-color:black;
        width:150px;height:30px;
        margin:auto;
        text-align:center;
        padding:20px;
        border-radius:15px;
        font-family:'Courier New',monospace;}
      .grid{display:grid;grid-template-columns: repeat(2, 1fr);}
      .container{position:relative;
        padding-left:50px;
        padding-top:10px;
        margin:15px;
        cursor:pointer;
        font-size:18px;font-weight:bold;
        -webkit-user-select:none;
        -moz-user-select:none;
        -ms-user-select:none;
        user-select:none;}
      /*hide default toggle*/.container input{position:absolute;
        opacity:0;
        cursor:pointer;}
      .checkmark{position:absolute;
        top:0;left:0;
        height:40px;width:40px;
        background-color:#2c3e50;
        border-radius:10px;}
      .container input:checked ~ .checkmark{background-color:gold;}
      .checkmark:after{content:"";position:absolute;display:none;}
      .container input:checked ~ .checkmark:after{display:block;}
      .container .checkmark:after{background:black;
        top:15px;left:10px;
        width:20px;height:10px;
        border-radius:5px;}
    </style>
    <meta name="viewport" content="width=device-width,height=device-height,initial-scale=1.0,user-scalable=0,minimum-scale=1.0,maximum-scale=1.0"/>
  </head>
  <body>
    <h1 id="colorUpdate">Glowtie</h1><br/>
    <h2 style="text-align:center">Battery:</h2>
    <h2 id="vcc" style="text-align:center"></h2>
    <form action="" name="settings" id="settings" method="post">
        <div style="margin:auto;width:75%;">
            RED<br/><input type="range" id="redS" class="slider" min="0" max="255" value="127" name="red" onchange="updateColor()" style="border: 3px solid red;"><br/>
            GREEN<br/><input type="range" id="greenS" class="slider" min="0" max="255" value="127" name="green" onchange="updateColor()" style="border: 3px solid green;"><br/>
            BLUE<br/><input type="range" id="blueS" class="slider" min="0" max="255" value="127" name="blue" onchange="updateColor()" style="border: 3px solid blue;"><br/>
        </div>
        <br/>
        <div style="margin:auto;width:75%">
            <div class="grid">
                <label class="container">Solid
                    <input id="sel0" type="radio" name="mode" value="0">
                    <span class="checkmark"></span>
                </label>
                <label class="container">Half
                    <input id="sel1" type="radio" name="mode" value="1">
                    <span class="checkmark"></span>
                </label>
                <label class="container">Solid Rainbow
                    <input id="sel2" type="radio" name="mode" value="2">
                    <span class="checkmark"></span>
                </label>
            </div>
            
            <hr />
                
            <div class="grid">
            <label class="container">Burst
                <input id="sel3" type="radio" name="mode" value="3">
                <span class="checkmark"></span>
            </label>
            <label class="container">Pulse
                <input id="sel4" type="radio" name="mode" value="4">
                <span class="checkmark"></span>
            </label>
            <label class="container">Bar
                <input id="sel5" type="radio" name="mode" value="5">
                <span class="checkmark"></span>
            </label>
            <label class="container">Scanner
                <input id="sel6" type="radio" name="mode" value="6">
                <span class="checkmark"></span>
            </label>
            <label class="container">Filler
                <input id="sel7" type="radio" name="mode" value="7">
                <span class="checkmark"></span>
            </label>
            <label class="container">Chaser
                <input id="sel8" type="radio" name="mode" value="8">
                <span class="checkmark"></span>
            </label>
            </div>
            
            <hr />
            
            <div class="grid">
            <label class="container">Infinity
                <input id="sel9" type="radio" name="mode" value="9">
                <span class="checkmark"></span>
            </label>
            <label class="container">Infinity Fill
                <input id="sel10" type="radio" name="mode" value="10">
                <span class="checkmark"></span>
            </label>
            <label class="container">Circles
                <input id="sel11" type="radio" name="mode" value="11">
                <span class="checkmark"></span>
            </label>
            <label class="container">Circles Fill
                <input id="sel12" type="radio" name="mode" value="12">
                <span class="checkmark"></span>
            </label>
            <label class="container">Symmetry
                <input id="sel13" type="radio" name="mode" value="13">
                <span class="checkmark"></span>
            </label>
            <label class="container">Symmetry Fill
                <input id="sel14" type="radio" name="mode" value="14">
                <span class="checkmark"></span>
            </label>
            <label class="container">Rainbow
                <input id="sel15" type="radio" name="mode" value="15">
                <span class="checkmark"></span>
            </label>
            <label class="container">Rainbow Fill
                <input id="sel16" type="radio" name="mode" value="16">
                <span class="checkmark"></span>
            </label>
            </div>
            
            <hr />
            
            <div class="grid">
              <label class="container">Breathe
                <input id="sel17" type="radio" name="mode" value="17">
                <span class="checkmark"></span>
              </label>
              <label class="container">Running Lights
                  <input id="sel18" type="radio" name="mode" value="18">
                  <span class="checkmark"></span>
              </label>
              <label class="container">Fireworks
                  <input id="sel19" type="radio" name="mode" value="19">
                  <span class="checkmark"></span>
              </label>
              <label class="container">Fireworks Random
                  <input id="sel20" type="radio" name="mode" value="20">
                  <span class="checkmark"></span>
              </label>
              <label class="container">Twinkle Fade
                  <input id="sel21" type="radio" name="mode" value="21">
                  <span class="checkmark"></span>
              </label>
              <label class="container">Twinkle FOX
                  <input id="sel22" type="radio" name="mode" value="22">
                  <span class="checkmark"></span>
              </label>
              <label class="container">Fire Flicker
                  <input id="sel23" type="radio" name="mode" value="23">
                  <span class="checkmark"></span>
              </label>
              <label class="container">Flash Sparkles
                  <input id="sel24" type="radio" name="mode" value="24">
                  <span class="checkmark"></span>
              </label>
            </div>
        </div>
        <div style="text-align:center"><button class="button" onclick="document.forms['settings'].submit()" type="button">UPDATE</button></div>
    </form>

    <hr><br/><h2 style="text-align:center">Settings</h2>

    <form action="" name="pref" id="pref" method="post">
        <h3>Random mode</h3>
        <div style="margin:auto;text-align:center;">
            <div style="margin:auto;margin-bottom:10px;margin-top:10px;">
                <label class="container">Change color
                    <input id="randomcolor" type="checkbox" name="randomcolor" value="0">
                    <span class="checkmark"></span>
                </label>
            </div><br/><h3>Mode change speed:</h3>
            <div class="grid">
              <label class="container">slow
                  <input id="rand0" type="radio" name="speed" value="0">
                  <span class="checkmark"></span>
              </label>
              <label class="container">normal
                <input id="rand1" type="radio" name="speed" value="1">
                <span class="checkmark"></span>
            </label>
              <label class="container">fast
                  <input id="rand2" type="radio" name="speed" value="2">
                  <span class="checkmark"></span>
              </label>
              <label class="container">rapid
                  <input id="rand3" type="radio" name="speed" value="3">
                  <span class="checkmark"></span>
              </label>
          </div>
        </div>
        <h3>LED brightness</h3>
        <div class="grid">
            <label class="container">dim
                <input id="bright0" type="radio" name="brightness" value="0">
                <span class="checkmark"></span>
            </label>
            <label class="container">normal
                <input id="bright1" type="radio" name="brightness" value="1">
                <span class="checkmark"></span>
            </label>
            <label class="container">shining
                <input id="bright2" type="radio" name="brightness" value="2">
                <span class="checkmark"></span>
            </label>
            <label class="container">bright
              <input id="bright3" type="radio" name="brightness" value="3">
              <span class="checkmark"></span>
          </label>
        </div>
        
        <div style="text-align:center"><button class="button" onclick="document.forms['pref'].submit()" type="button">UPDATE</button></div>
    </form>
	<script>
      getData('redS','red');getData('greenS','green');getData('blueS','blue');getData('','mode');
      getData('randomcolor','color');getData('','speed');getData('','brightness');
      setInterval(function(){getData('vcc','vcc');}, 2500);
      function updateColor(){
        var red = document.forms['settings'].red.value;
        var green = document.forms['settings'].green.value;
        var blue = document.forms['settings'].blue.value;
        var rgb = blue | (green << 8) | (red << 16);
        document.getElementById("colorUpdate").style.backgroundColor = "#" + (0x1000000 + rgb).toString(16).slice(1);
      }
      function getData(elementID,url){
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if(this.readyState==4 && this.status==200)
            if(url=='vcc')document.getElementById(elementID).innerHTML = this.responseText;
            else if(url=='mode')document.getElementById('sel'+this.responseText).checked = true;
            else if(url=='brightness')document.getElementById('bright'+this.responseText).checked = true;
            else if(url=='speed')document.getElementById('rand'+this.responseText).checked = true;
            else if(url=='color')document.getElementById(elementID).checked = (this.responseText==1);
            else {
              document.getElementById(elementID).value = this.responseText;
              updateColor();
            }
        };
        xhttp.open('GET',url,true);xhttp.send();
      }
    </script>
  </body>
</html>)rawliteral";