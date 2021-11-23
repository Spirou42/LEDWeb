// DUmy file will be filled

const MessageType = {
    Color            : 1, 
    Palette          : 2,
    Effect           : 3,
    Parameter        : 4,
    EffectParameter  : 5,
  };

class payload {
    constructor(){};
    get type(){return 0};
}

class CRGB extends payload{
    constructor(red, green, blue){
        super()
        //console.log("RED:"+red);
        this.red = red
        this.green = green
        this.blue = blue
    }
    get type(){return MessageType.Color}
    
    distance(color) {
        var distCol = new CRGB(0,0,0);
        distCol.red = this.red - color.red;
        distCol.green = this.green - color.green;
        distCol.blue = this.blue - color.blue;
        
        return Math.sqrt(distCol.red * distCol.red + distCol.green*distCol.green + distCol.blue*distCol.blue)    
    }
}

class ClientMessage {
    constructor(type,data){
        this.type = type
        this.payload = data
    };
}
var lastColor = new CRGB(0,0,0);
var lastTime = Date.now() 

function updateColor(colorPicker) {
    //console.log("Babelfisch");
    var color = new CRGB(Math.round(colorPicker.rgb[0]),Math.round(colorPicker.rgb[1]),Math.round(colorPicker.rgb[2]))
    let distance = color.distance(lastColor);
    if( (distance > 10) && Date.now()-lastTime > 80 ){
        console.log("Distance: "+distance);
        var message = new ClientMessage(color.type,color)
        jmessage = JSON.stringify(message)
        wSocket.send(jmessage);
        lastColor = color;
        lastTime = Date.now()
    }
}

function onWSConnect(event){
    console.log("WS open");
    //wSocket.send("Here's some text that the server is urgently awaiting!"); 
}
function onWSError(event){
    console.log("some Error:"+event)
}

function setupWSClient() {
    console.log("Setup WSClient");
    wSocket = new WebSocket("ws://esp32rgb.local/ws");
    wSocket.onopen=onWSConnect;
    wSocket.onerror=onWSError;
}