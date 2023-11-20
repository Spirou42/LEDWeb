// EventClient


function ge(s){ return document.getElementById(s);}
function ce(s){ return document.createElement(s);}
function stb(){ window.scrollTo(0, document.body.scrollHeight || document.documentElement.scrollHeight); }
function addMessage(m){
  var msg = ce("div");
  var k = document.createElement("div");
  k.innerText = m;
  msg.appendChild(k);
  ge("dbg").appendChild(msg);
  stb();
}

function idForHint(hint) {
  var result = ""
  switch (hint) {
    case "System":
      result = "#ParametersGroup"
      break;
    case "Palettes":
      result = "#PalettesGroup"
      break;
    case "Effects":
      result = "#EffectsGroup"
      break;
    default:
      break;
  }
  return result
}


function spinnerChange(event, ui) {
  console.log("event: " + event)
  console.log("UI: " + JSON.stringify(ui))
  console.log("----")
}
function setupEventSource(){
  var eventSource = new EventSource('/events');
  eventSource.onopen = function (e) {
    addMessage("Events Opened");
  };
  
  eventSource.onerror = function (e) {
    if (e.target.readyState != EventSource.OPEN) {
      addMessage("Events Closed");
    }
  };
  
  eventSource.onmessage = function (e) {
    addMessage("Event: " + e.data);
  };

  eventSource.addEventListener('SPAL', function (e) {

    var stuff = JSON.parse(e.data);

    var array = stuff["SystemPalettes"];
    $("#SystemPalettes").empty()
    for (let index = 0; index < array.length; index++) {
      const element = array[index];
      let palName = element["PaletteName"]
      let palIdx = element["PaletteID"]
      let selected = element["current"]
      let entry = $( "<option>",{ html: palName, "value":palIdx, "selected":selected} ) 
      entry.appendTo("#SystemPalettes")
    }
    $("#SystemPalettes").selectmenu("refresh");
    $("#SystemPalettes").selectmenu({
      change: function (event, data) {
        let item = data.item
        console.log("Palettes: " + Object.keys(data.item) + " " + item.element)
        let payload = { "Palette": item.value }
        let message = new ClientMessage(MessageType.Palette, payload)
        let jmessage = JSON.stringify(message)
        wSocket.send(jmessage);
      }
    })
    $("#SystemPalettes").selectmenu("menuWidget").addClass("overflow")
  }, false);
  
  eventSource.addEventListener('SPAR', function (e) {
    //console.log("SystemParameter List");
    var stuff = JSON.parse(e.data);
    var array = stuff["SystemParameters"];
    array.forEach(element => {
      let parID = element["ID"]
      let docID = idForHint(element["Hint"])
      let docElement = $(docID)
      let labelID = element["uiName"] + "-Label"
      let spinnerID = element["uiName"] + "-Spinner"
      $("#" + labelID).remove()
      $("#" + spinnerID).remove()
      let label = $("<label>", { html: element["name"], "for": spinnerID, "class": "ui-controlgroup-label", "id":labelID})
      let spinner = $("<input>", { "id": spinnerID, "class": "ui-spinner-input", "name": element["uiName"], "value": element["value"] })
      let group = $("<section>",{"class": "blockGroup"})
      label.appendTo(docElement)
      spinner.appendTo(docElement)
      //group.appendTo(docElement)
      spinner.spinner()
      spinner.spinner("option", "max", element["maxValue"])
      spinner.spinner("option", "min", element["minValue"])
      spinner.spinner({
        stop: function (event, ui) {
          console.log("Changed " + element["uiName"] + " to " + $(this).spinner("value"))
          let payload = { "Name": element["uiName"],"value":$(this).spinner("value") }
          let message = new ClientMessage(MessageType.Parameter, payload)
          let jmessage = JSON.stringify(message)
          wSocket.send(jmessage)
        }
      })
      docElement.controlgroup("refresh")
    });
  }, false);

  eventSource.addEventListener('SEFF', function (e) {
    //console.log("SystemEffect List");
    let stuff = JSON.parse(e.data);
    let array = stuff["SystemEffects"];
    $("#SystemEffects").empty();
    for (let index = 0; index < array.length; index++) {
      const element = array[index];
      let efName = element["effectName"]
      let efIdx = element["effectID"]
      let selected = element["current"]
      let entry = $("<option>",{html:efName, "value":efIdx, "selected":selected})
      entry.appendTo("#SystemEffects")
    }
    $("#SystemEffects").selectmenu("refresh")
    $("#SystemEffects").selectmenu({
      change: function (event, data) {
        let item = data.item
        console.log("Effects: " + Object.keys(data.item) + " " + item.element)
        let payload = { "Effect": item.value }
        let message = new ClientMessage(MessageType.Effect, payload)
        let jmessage = JSON.stringify(message)
        wSocket.send(jmessage);
      }
    })
    $("#SystemEffects").selectmenu("menuWidget").addClass("overflow")
  }, false);

  eventSource.addEventListener("PAEF", function (e) {
    let stuff = JSON.parse(e.data);
    let array = stuff["Parameters"];
    if (array.length == 0) {
      $("#EffectParameterSet").hide();
    } else {
      $("#EffectParameterSet").show();
    }
    let docElement = $("#EffectParameterGroup")
    docElement.empty();
    array.forEach(element => {
      let labelID = element["uiName"] + "-Label"
      let spinnerID = element["uiName"] + "-Spinner"
      $("#" + labelID).remove()
      $("#" + spinnerID).remove()
      let label = $("<label>", { html: element["name"], "for": spinnerID, "class": "ui-controlgroup-label", "id":labelID})
      let spinner = $("<input>", { "id": spinnerID, "class": "ui-spinner-input", "name": element["uiName"] ,"value": element["value"]})
      label.appendTo(docElement)
      spinner.appendTo(docElement)
      spinner.spinner()
      spinner.spinner("option", "max", element["maxValue"])
      spinner.spinner("option", "min", element["minValue"])
      spinner.spinner({
        stop: function (event, ui) {
          console.log("Changed " + element["uiName"] + " to " + $(this).spinner("value"))
          let payload = { "Name": element["uiName"],"value":$(this).spinner("value"), "idx":element["idx"]}
          let message = new ClientMessage(MessageType.EffectParameter, payload)
          let jmessage = JSON.stringify(message)
          wSocket.send(jmessage)
        }
      })
      docElement.controlgroup("refresh")
    })
    addMessage("Event [PAEF]" + e.data);
  }, false);
}


